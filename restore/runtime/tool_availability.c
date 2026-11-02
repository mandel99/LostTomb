/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "tool_availability.h"

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
static __inline uint32_t mi_alu(struct restore_unlock_frame *f,uint32_t a,uint32_t b,uint32_t width,uint32_t operation)
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
static __inline uint32_t mi_shift(struct restore_unlock_frame *f,uint32_t a,uint32_t count,uint32_t width,uint32_t arithmetic)
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
static __inline uint32_t mi_imul(struct restore_unlock_frame *f,uint32_t a,uint32_t b)
{
 int64_t product=(int64_t)(int32_t)a*(int32_t)b;uint32_t result=(uint32_t)product;
 f->flags&=~(MI_CF|MI_OF);if(product!=(int64_t)(int32_t)result)f->flags|=MI_CF|MI_OF;
 return result;
}
static __inline int mi_signed_less(uint32_t flags){return ((flags&MI_SF)!=0)^((flags&MI_OF)!=0);}

void restore_unlock_C_00404350(struct restore_unlock_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00404350 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 00404353 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00404354 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00404355 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00404356 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00404357 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 00404359 */
    f->r[3]=0x00000001u;
    /* 0040435e */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[7]);
mi_00404362:;
    /* 00404362 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[7]);
mi_00404366:;
    /* 00404366 */
    a=MI_U32((0x00605a0cu));b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 0040436c */
    if((f->flags&MI_ZF))goto mi_00405127;
    /* 00404372 */
    a=MI_U32((0x00605a3cu));b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 00404378 */
    if((f->flags&MI_ZF))goto mi_00405127;
    /* 0040437e */
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
    /* 00404382 */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
    /* 00404386 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 00404389 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 0040438c */
    f->r[6]=(0x00000000u+f->r[1]+f->r[0]*2u);
    /* 0040438f */
    a=f->r[6];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 00404392 */
    f->r[5]=MI_U32((0x005ce3bcu+f->r[6]));
    /* 00404398 */
    MI_U32((0x0070a8dcu+f->r[6]))=(uint32_t)(f->r[7]);
    /* 0040439e */
    a=f->r[5];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 004043a1 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[5]);
    /* 004043a5 */
    if(mi_signed_less(f->flags))goto mi_004043b2;
    /* 004043a7 */
    a=f->r[5];b=0x0000001du;t=mi_alu(f,a,b,32,1);
    /* 004043aa */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004043b2;
    /* 004043ac */
    MI_U32((0x0070a8dcu+f->r[6]))=(uint32_t)(f->r[3]);
mi_004043b2:;
    /* 004043b2 */
    a=f->r[5];b=0x00000009u;t=mi_alu(f,a,b,32,1);
    /* 004043b5 */
    if((f->flags&MI_ZF))goto mi_0040470d;
    /* 004043bb */
    a=f->r[5];b=0x0000005cu;t=mi_alu(f,a,b,32,1);
    /* 004043be */
    if((f->flags&MI_ZF))goto mi_0040470d;
    /* 004043c4 */
    a=f->r[5];b=0x000000b4u;t=mi_alu(f,a,b,32,1);
    /* 004043ca */
    if(!(f->flags&MI_ZF))goto mi_004043d8;
    /* 004043cc */
    a=MI_U16((0x00784d04u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 004043d3 */
    goto mi_004046ac;
mi_004043d8:;
    /* 004043d8 */
    a=f->r[5];b=0x00000089u;t=mi_alu(f,a,b,32,1);
    /* 004043de */
    if((f->flags&MI_ZF))goto mi_0040470d;
    /* 004043e4 */
    a=f->r[5];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 004043e7 */
    if((f->flags&MI_ZF))goto mi_0040470d;
    /* 004043ed */
    a=f->r[5];b=0x00000051u;t=mi_alu(f,a,b,32,1);
    /* 004043f0 */
    if((f->flags&MI_ZF))goto mi_0040470d;
    /* 004043f6 */
    a=f->r[5];b=0x000000a7u;t=mi_alu(f,a,b,32,1);
    /* 004043fc */
    if((f->flags&MI_ZF))goto mi_0040470d;
    /* 00404402 */
    a=f->r[5];b=0x00000073u;t=mi_alu(f,a,b,32,1);
    /* 00404405 */
    if(!(f->flags&MI_ZF))goto mi_00404413;
    /* 00404407 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00404409 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040440eu;f->boundary(0x00404409u,0x00443b90u,f);
    /* 0040440e */
    goto mi_004046a7;
mi_00404413:;
    /* 00404413 */
    a=f->r[5];b=0x00000047u;t=mi_alu(f,a,b,32,1);
    /* 00404416 */
    if(!(f->flags&MI_ZF))goto mi_00404424;
    /* 00404418 */
    a=MI_U16((0x00784ceeu));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 0040441f */
    goto mi_004046ac;
mi_00404424:;
    /* 00404424 */
    a=f->r[5];b=0x00000033u;t=mi_alu(f,a,b,32,1);
    /* 00404427 */
    if(!(f->flags&MI_ZF))goto mi_00404435;
    /* 00404429 */
    a=MI_U16((0x00784d00u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404430 */
    goto mi_004046ac;
mi_00404435:;
    /* 00404435 */
    a=f->r[5];b=0x00000035u;t=mi_alu(f,a,b,32,1);
    /* 00404438 */
    if(!(f->flags&MI_ZF))goto mi_00404446;
    /* 0040443a */
    a=MI_U16((0x00784d02u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404441 */
    goto mi_004046ac;
mi_00404446:;
    /* 00404446 */
    a=f->r[5];b=0x000000ceu;t=mi_alu(f,a,b,32,1);
    /* 0040444c */
    if(!(f->flags&MI_ZF))goto mi_0040445a;
    /* 0040444e */
    a=MI_U16((0x00784d0au));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404455 */
    goto mi_004046ac;
mi_0040445a:;
    /* 0040445a */
    a=f->r[5];b=0x0000002eu;t=mi_alu(f,a,b,32,1);
    /* 0040445d */
    if(!(f->flags&MI_ZF))goto mi_0040446b;
    /* 0040445f */
    a=MI_U16((0x00784d08u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404466 */
    goto mi_004046ac;
mi_0040446b:;
    /* 0040446b */
    a=f->r[5];b=0x0000002fu;t=mi_alu(f,a,b,32,1);
    /* 0040446e */
    if(!(f->flags&MI_ZF))goto mi_0040447c;
    /* 00404470 */
    a=MI_U16((0x00784d0cu));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404477 */
    goto mi_004046ac;
mi_0040447c:;
    /* 0040447c */
    a=f->r[5];b=0x000000b8u;t=mi_alu(f,a,b,32,1);
    /* 00404482 */
    if(!(f->flags&MI_ZF))goto mi_00404490;
    /* 00404484 */
    a=MI_U16((0x00784d10u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 0040448b */
    goto mi_004046ac;
mi_00404490:;
    /* 00404490 */
    a=f->r[5];b=0x00000037u;t=mi_alu(f,a,b,32,1);
    /* 00404493 */
    if((f->flags&MI_ZF))goto mi_0040470d;
    /* 00404499 */
    a=f->r[5];b=0x00000088u;t=mi_alu(f,a,b,32,1);
    /* 0040449f */
    if(!(f->flags&MI_ZF))goto mi_004044ad;
    /* 004044a1 */
    a=MI_U16((0x00784d1au));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 004044a8 */
    goto mi_004046ac;
mi_004044ad:;
    /* 004044ad */
    a=f->r[5];b=0x000000a1u;t=mi_alu(f,a,b,32,1);
    /* 004044b3 */
    if(!(f->flags&MI_ZF))goto mi_004044c1;
    /* 004044b5 */
    a=MI_U16((0x00784ce4u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 004044bc */
    goto mi_004046ac;
mi_004044c1:;
    /* 004044c1 */
    a=f->r[5];b=0x0000001fu;t=mi_alu(f,a,b,32,1);
    /* 004044c4 */
    if(!(f->flags&MI_ZF))goto mi_004044d2;
    /* 004044c6 */
    a=MI_U16((0x00784cf6u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 004044cd */
    goto mi_004046ac;
mi_004044d2:;
    /* 004044d2 */
    a=f->r[5];b=0x00000024u;t=mi_alu(f,a,b,32,1);
    /* 004044d5 */
    if(!(f->flags&MI_ZF))goto mi_004044e3;
    /* 004044d7 */
    a=MI_U16((0x00784cf6u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 004044de */
    goto mi_004046ac;
mi_004044e3:;
    /* 004044e3 */
    a=f->r[5];b=0x000000d1u;t=mi_alu(f,a,b,32,1);
    /* 004044e9 */
    if(!(f->flags&MI_ZF))goto mi_004044f7;
    /* 004044eb */
    a=MI_U16((0x00784cfeu));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 004044f2 */
    goto mi_004046ac;
mi_004044f7:;
    /* 004044f7 */
    a=f->r[5];b=0x000000c4u;t=mi_alu(f,a,b,32,1);
    /* 004044fd */
    if(!(f->flags&MI_ZF))goto mi_0040450b;
    /* 004044ff */
    a=0x00000006u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00404501 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00404506u;f->boundary(0x00404501u,0x00443b90u,f);
    /* 00404506 */
    goto mi_004046a7;
mi_0040450b:;
    /* 0040450b */
    a=f->r[5];b=0x00000065u;t=mi_alu(f,a,b,32,1);
    /* 0040450e */
    if(!(f->flags&MI_ZF))goto mi_0040451c;
    /* 00404510 */
    a=0x00000010u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00404512 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00404517u;f->boundary(0x00404512u,0x00443b90u,f);
    /* 00404517 */
    goto mi_004046a7;
mi_0040451c:;
    /* 0040451c */
    a=f->r[5];b=0x000000e0u;t=mi_alu(f,a,b,32,1);
    /* 00404522 */
    if(!(f->flags&MI_ZF))goto mi_00404530;
    /* 00404524 */
    a=0x00000020u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00404526 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040452bu;f->boundary(0x00404526u,0x00443b90u,f);
    /* 0040452b */
    goto mi_004046a7;
mi_00404530:;
    /* 00404530 */
    a=f->r[5];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 00404533 */
    if(!(f->flags&MI_ZF))goto mi_00404541;
    /* 00404535 */
    a=0x0000000eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00404537 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040453cu;f->boundary(0x00404537u,0x00443b90u,f);
    /* 0040453c */
    goto mi_004046a7;
mi_00404541:;
    /* 00404541 */
    a=f->r[5];b=0x0000006du;t=mi_alu(f,a,b,32,1);
    /* 00404544 */
    if(!(f->flags&MI_ZF))goto mi_00404552;
    /* 00404546 */
    a=0x0000000bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00404548 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040454du;f->boundary(0x00404548u,0x00443b90u,f);
    /* 0040454d */
    goto mi_004046a7;
mi_00404552:;
    /* 00404552 */
    a=f->r[5];b=0x00000072u;t=mi_alu(f,a,b,32,1);
    /* 00404555 */
    if(!(f->flags&MI_ZF))goto mi_0040455e;
    /* 00404557 */
    a=0x0000000bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00404559 */
    goto mi_004046a2;
mi_0040455e:;
    /* 0040455e */
    a=f->r[5];b=0x00000027u;t=mi_alu(f,a,b,32,1);
    /* 00404561 */
    if(!(f->flags&MI_ZF))goto mi_0040456f;
    /* 00404563 */
    a=MI_U16((0x00784d1cu));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 0040456a */
    goto mi_004046ac;
mi_0040456f:;
    /* 0040456f */
    a=f->r[5];b=0x0000006eu;t=mi_alu(f,a,b,32,1);
    /* 00404572 */
    if(!(f->flags&MI_ZF))goto mi_0040457b;
    /* 00404574 */
    a=0x0000000fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00404576 */
    goto mi_004046a2;
mi_0040457b:;
    /* 0040457b */
    a=f->r[5];b=0x0000008au;t=mi_alu(f,a,b,32,1);
    /* 00404581 */
    if(!(f->flags&MI_ZF))goto mi_0040458f;
    /* 00404583 */
    a=MI_U16((0x00784d16u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 0040458a */
    goto mi_004046ac;
mi_0040458f:;
    /* 0040458f */
    a=f->r[5];b=0x00000052u;t=mi_alu(f,a,b,32,1);
    /* 00404592 */
    if(!(f->flags&MI_ZF))goto mi_004045a0;
    /* 00404594 */
    a=MI_U16((0x00784d18u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 0040459b */
    goto mi_004046ac;
mi_004045a0:;
    /* 004045a0 */
    a=f->r[5];b=0x00000056u;t=mi_alu(f,a,b,32,1);
    /* 004045a3 */
    if(!(f->flags&MI_ZF))goto mi_004045b1;
    /* 004045a5 */
    a=MI_U16((0x00784d0eu));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 004045ac */
    goto mi_004046ac;
mi_004045b1:;
    /* 004045b1 */
    a=f->r[5];b=0x000000cbu;t=mi_alu(f,a,b,32,1);
    /* 004045b7 */
    if(!(f->flags&MI_ZF))goto mi_004045c0;
    /* 004045b9 */
    a=0x00000016u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004045bb */
    goto mi_004046a2;
mi_004045c0:;
    /* 004045c0 */
    a=f->r[5];b=0x000000e8u;t=mi_alu(f,a,b,32,1);
    /* 004045c6 */
    if(!(f->flags&MI_ZF))goto mi_004045e1;
    /* 004045c8 */
    a=0x0000001fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004045ca */
    f->r[4]-=4;MI_U32(f->r[4])=0x004045cfu;f->boundary(0x004045cau,0x00443d90u,f);
    /* 004045cf */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004045d2 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004045d4 */
    if((f->flags&MI_ZF))goto mi_00404713;
    /* 004045da */
    a=0x0000000du;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004045dc */
    goto mi_004046a2;
mi_004045e1:;
    /* 004045e1 */
    a=f->r[5];b=0x000000e9u;t=mi_alu(f,a,b,32,1);
    /* 004045e7 */
    if(!(f->flags&MI_ZF))goto mi_004045f0;
    /* 004045e9 */
    a=0x00000020u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004045eb */
    goto mi_004046a2;
mi_004045f0:;
    /* 004045f0 */
    a=f->r[5];b=0x000000c3u;t=mi_alu(f,a,b,32,1);
    /* 004045f6 */
    if(!(f->flags&MI_ZF))goto mi_00404604;
    /* 004045f8 */
    a=0x00000016u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004045fa */
    f->r[4]-=4;MI_U32(f->r[4])=0x004045ffu;f->boundary(0x004045fau,0x00443b90u,f);
    /* 004045ff */
    goto mi_004046a7;
mi_00404604:;
    /* 00404604 */
    a=f->r[5];b=0x00000046u;t=mi_alu(f,a,b,32,1);
    /* 00404607 */
    if(!(f->flags&MI_ZF))goto mi_00404615;
    /* 00404609 */
    a=MI_U16((0x00784cf0u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404610 */
    goto mi_004046ac;
mi_00404615:;
    /* 00404615 */
    a=f->r[5];b=0x00000048u;t=mi_alu(f,a,b,32,1);
    /* 00404618 */
    if(!(f->flags&MI_ZF))goto mi_00404626;
    /* 0040461a */
    a=MI_U16((0x00784cf2u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404621 */
    goto mi_004046ac;
mi_00404626:;
    /* 00404626 */
    a=f->r[5];b=0x000000c7u;t=mi_alu(f,a,b,32,1);
    /* 0040462c */
    if(!(f->flags&MI_ZF))goto mi_00404637;
    /* 0040462e */
    a=MI_U16((0x00784cecu));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404635 */
    goto mi_004046ac;
mi_00404637:;
    /* 00404637 */
    a=f->r[5];b=0x0000006fu;t=mi_alu(f,a,b,32,1);
    /* 0040463a */
    if(!(f->flags&MI_ZF))goto mi_00404640;
    /* 0040463c */
    a=0x00000010u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040463e */
    goto mi_004046a2;
mi_00404640:;
    /* 00404640 */
    a=f->r[5];b=0x0000003cu;t=mi_alu(f,a,b,32,1);
    /* 00404643 */
    if(!(f->flags&MI_ZF))goto mi_0040464e;
    /* 00404645 */
    a=MI_U16((0x007848e2u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 0040464c */
    goto mi_004046ac;
mi_0040464e:;
    /* 0040464e */
    a=f->r[5];b=0x00000040u;t=mi_alu(f,a,b,32,1);
    /* 00404651 */
    if(!(f->flags&MI_ZF))goto mi_0040465c;
    /* 00404653 */
    a=MI_U16((0x007848eau));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 0040465a */
    goto mi_004046ac;
mi_0040465c:;
    /* 0040465c */
    a=f->r[5];b=0x0000008cu;t=mi_alu(f,a,b,32,1);
    /* 00404662 */
    if(!(f->flags&MI_ZF))goto mi_0040466d;
    /* 00404664 */
    a=MI_U16((0x007848e2u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 0040466b */
    goto mi_004046ac;
mi_0040466d:;
    /* 0040466d */
    a=f->r[5];b=0x00000090u;t=mi_alu(f,a,b,32,1);
    /* 00404673 */
    if(!(f->flags&MI_ZF))goto mi_0040467e;
    /* 00404675 */
    a=MI_U16((0x007848eau));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 0040467c */
    goto mi_004046ac;
mi_0040467e:;
    /* 0040467e */
    a=f->r[5];b=0x000000b1u;t=mi_alu(f,a,b,32,1);
    /* 00404684 */
    if(!(f->flags&MI_ZF))goto mi_0040468a;
    /* 00404686 */
    a=0x00000014u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00404688 */
    goto mi_004046a2;
mi_0040468a:;
    /* 0040468a */
    a=f->r[5];b=0x000000b2u;t=mi_alu(f,a,b,32,1);
    /* 00404690 */
    if(!(f->flags&MI_ZF))goto mi_004046b0;
    /* 00404692 */
    a=0x0000000cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00404694 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00404699u;f->boundary(0x00404694u,0x00443b90u,f);
    /* 00404699 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040469c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0040469e */
    if(!(f->flags&MI_ZF))goto mi_0040470d;
    /* 004046a0 */
    a=0x0000000cu;f->r[4]-=4;MI_U32(f->r[4])=a;
mi_004046a2:;
    /* 004046a2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004046a7u;f->boundary(0x004046a2u,0x00443d90u,f);
mi_004046a7:;
    /* 004046a7 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004046aa */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
mi_004046ac:;
    /* 004046ac */
    if((f->flags&MI_ZF))goto mi_00404713;
    /* 004046ae */
    goto mi_0040470d;
mi_004046b0:;
    /* 004046b0 */
    a=f->r[5];b=0x0000004au;t=mi_alu(f,a,b,32,1);
    /* 004046b3 */
    if(!(f->flags&MI_ZF))goto mi_004046d2;
    /* 004046b5 */
    a=MI_U16((0x00784ceau));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 004046bc */
    if(!(f->flags&MI_ZF))goto mi_0040470d;
    /* 004046be */
    a=MI_U16((0x00784d38u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 004046c5 */
    if(!(f->flags&MI_ZF))goto mi_0040470d;
    /* 004046c7 */
    a=MI_U16((0x00784d36u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 004046ce */
    if((f->flags&MI_ZF))goto mi_00404713;
    /* 004046d0 */
    goto mi_0040470d;
mi_004046d2:;
    /* 004046d2 */
    a=f->r[5];b=0x00000031u;t=mi_alu(f,a,b,32,1);
    /* 004046d5 */
    if(!(f->flags&MI_ZF))goto mi_004046e2;
    /* 004046d7 */
    a=MI_U16((0x00784d06u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 004046de */
    if((f->flags&MI_ZF))goto mi_00404713;
    /* 004046e0 */
    goto mi_0040470d;
mi_004046e2:;
    /* 004046e2 */
    a=f->r[5];b=0x000000bbu;t=mi_alu(f,a,b,32,1);
    /* 004046e8 */
    if(!(f->flags&MI_ZF))goto mi_00404713;
    /* 004046ea */
    a=MI_U16((0x00784d12u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 004046f1 */
    if((f->flags&MI_ZF))goto mi_00404713;
    /* 004046f3 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 004046f8 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004046fe */
    f->r[0]=MI_U32((0x00ea5cbcu+f->r[0]));
    /* 00404704 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 00404706 */
    if(mi_signed_less(f->flags))goto mi_00404713;
    /* 00404708 */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 0040470b */
    if(!mi_signed_less(f->flags))goto mi_00404713;
mi_0040470d:;
    /* 0040470d */
    MI_U32((0x0070a8dcu+f->r[6]))=(uint32_t)(f->r[3]);
mi_00404713:;
    /* 00404713 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00404718u;f->boundary(0x00404713u,0x00403ef0u,f);
    /* 00404718 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0040471a */
    if((f->flags&MI_ZF))goto mi_00404765;
    /* 0040471c */
    a=f->r[5];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 0040471f */
    if(!(f->flags&MI_ZF))goto mi_00404765;
    /* 00404721 */
    a=0x000000b3u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00404726 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040472bu;f->boundary(0x00404726u,0x004f5470u,f);
    /* 0040472b */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040472e */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00404730 */
    if(!(f->flags&MI_ZF))goto mi_00404b0d;
    /* 00404736 */
    a=0x000000b2u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040473b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00404740u;f->boundary(0x0040473bu,0x004f5470u,f);
    /* 00404740 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00404743 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00404745 */
    if(!(f->flags&MI_ZF))goto mi_00404b0d;
    /* 0040474b */
    a=0x000000b1u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00404750 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00404755u;f->boundary(0x00404750u,0x004f5470u,f);
    /* 00404755 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00404758 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0040475a */
    if((f->flags&MI_ZF))goto mi_00404b13;
    /* 00404760 */
    goto mi_00404b0d;
mi_00404765:;
    /* 00404765 */
    a=f->r[5];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 00404768 */
    if(!(f->flags&MI_ZF))goto mi_00404810;
    /* 0040476e */
    a=0x00000016u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00404770 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00404775u;f->boundary(0x00404770u,0x00443b90u,f);
    /* 00404775 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00404778 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0040477a */
    if(!(f->flags&MI_ZF))goto mi_00404b0d;
    /* 00404780 */
    a=0x0000000bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00404782 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00404787u;f->boundary(0x00404782u,0x00443b90u,f);
    /* 00404787 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040478a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0040478c */
    if(!(f->flags&MI_ZF))goto mi_00404b0d;
    /* 00404792 */
    a=0x00000014u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00404794 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00404799u;f->boundary(0x00404794u,0x00443b90u,f);
    /* 00404799 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040479c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0040479e */
    if(!(f->flags&MI_ZF))goto mi_00404b0d;
    /* 004047a4 */
    a=0x00000018u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004047a6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004047abu;f->boundary(0x004047a6u,0x00443b90u,f);
    /* 004047ab */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004047ae */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004047b0 */
    if(!(f->flags&MI_ZF))goto mi_00404b0d;
    /* 004047b6 */
    a=0x00000019u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004047b8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004047bdu;f->boundary(0x004047b8u,0x00443b90u,f);
    /* 004047bd */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004047c0 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004047c2 */
    if(!(f->flags&MI_ZF))goto mi_00404b0d;
    /* 004047c8 */
    a=0x0000001eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004047ca */
    f->r[4]-=4;MI_U32(f->r[4])=0x004047cfu;f->boundary(0x004047cau,0x00443b90u,f);
    /* 004047cf */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004047d2 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004047d4 */
    if(!(f->flags&MI_ZF))goto mi_00404b0d;
    /* 004047da */
    a=0x0000001du;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004047dc */
    f->r[4]-=4;MI_U32(f->r[4])=0x004047e1u;f->boundary(0x004047dcu,0x00443b90u,f);
    /* 004047e1 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004047e4 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004047e6 */
    if(!(f->flags&MI_ZF))goto mi_00404b0d;
    /* 004047ec */
    a=0x00000012u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004047ee */
    f->r[4]-=4;MI_U32(f->r[4])=0x004047f3u;f->boundary(0x004047eeu,0x00443b90u,f);
    /* 004047f3 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004047f6 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004047f8 */
    if(!(f->flags&MI_ZF))goto mi_00404b0d;
    /* 004047fe */
    a=MI_U16((0x00784ce4u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404805 */
    if((f->flags&MI_ZF))goto mi_00404b13;
    /* 0040480b */
    goto mi_00404b0d;
mi_00404810:;
    /* 00404810 */
    a=f->r[5];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 00404813 */
    if(!(f->flags&MI_ZF))goto mi_004048ad;
    /* 00404819 */
    a=0x0000000eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040481b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00404820u;f->boundary(0x0040481bu,0x00443b90u,f);
    /* 00404820 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00404823 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00404825 */
    if(!(f->flags&MI_ZF))goto mi_00404b0d;
    /* 0040482b */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040482c */
    f->r[4]-=4;MI_U32(f->r[4])=0x00404831u;f->boundary(0x0040482cu,0x00443b90u,f);
    /* 00404831 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00404834 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00404836 */
    if(!(f->flags&MI_ZF))goto mi_00404b0d;
    /* 0040483c */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040483e */
    f->r[4]-=4;MI_U32(f->r[4])=0x00404843u;f->boundary(0x0040483eu,0x00443b90u,f);
    /* 00404843 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00404846 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00404848 */
    if(!(f->flags&MI_ZF))goto mi_00404b0d;
    /* 0040484e */
    a=0x00000004u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00404850 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00404855u;f->boundary(0x00404850u,0x00443b90u,f);
    /* 00404855 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00404858 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0040485a */
    if(!(f->flags&MI_ZF))goto mi_00404b0d;
    /* 00404860 */
    a=0x00000010u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00404862 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00404867u;f->boundary(0x00404862u,0x00443b90u,f);
    /* 00404867 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040486a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0040486c */
    if(!(f->flags&MI_ZF))goto mi_00404b0d;
    /* 00404872 */
    a=0x00000005u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00404874 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00404879u;f->boundary(0x00404874u,0x00443b90u,f);
    /* 00404879 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040487c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0040487e */
    if(!(f->flags&MI_ZF))goto mi_00404b0d;
    /* 00404884 */
    a=0x00000006u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00404886 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040488bu;f->boundary(0x00404886u,0x00443b90u,f);
    /* 0040488b */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040488e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00404890 */
    if(!(f->flags&MI_ZF))goto mi_00404b0d;
    /* 00404896 */
    a=0x00000020u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00404898 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040489du;f->boundary(0x00404898u,0x00443b90u,f);
    /* 0040489d */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004048a0 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004048a2 */
    if((f->flags&MI_ZF))goto mi_00404b13;
    /* 004048a8 */
    goto mi_00404b0d;
mi_004048ad:;
    /* 004048ad */
    a=f->r[5];b=0x00000041u;t=mi_alu(f,a,b,32,1);
    /* 004048b0 */
    if(!(f->flags&MI_ZF))goto mi_004048c4;
    /* 004048b2 */
    a=MI_U16((0x00784db0u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 004048b9 */
    if((f->flags&MI_ZF))goto mi_00404b13;
    /* 004048bf */
    goto mi_00404b0d;
mi_004048c4:;
    /* 004048c4 */
    a=f->r[5];b=0x00000043u;t=mi_alu(f,a,b,32,1);
    /* 004048c7 */
    if(!(f->flags&MI_ZF))goto mi_004048db;
    /* 004048c9 */
    a=MI_U16((0x00784db4u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 004048d0 */
    if((f->flags&MI_ZF))goto mi_00404b13;
    /* 004048d6 */
    goto mi_00404b0d;
mi_004048db:;
    /* 004048db */
    a=f->r[5];b=0x00000044u;t=mi_alu(f,a,b,32,1);
    /* 004048de */
    if(!(f->flags&MI_ZF))goto mi_004048f2;
    /* 004048e0 */
    a=MI_U16((0x00784db6u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 004048e7 */
    if((f->flags&MI_ZF))goto mi_00404b13;
    /* 004048ed */
    goto mi_00404b0d;
mi_004048f2:;
    /* 004048f2 */
    a=f->r[5];b=0x00000042u;t=mi_alu(f,a,b,32,1);
    /* 004048f5 */
    if(!(f->flags&MI_ZF))goto mi_00404909;
    /* 004048f7 */
    a=MI_U16((0x00784db2u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 004048fe */
    if((f->flags&MI_ZF))goto mi_00404b13;
    /* 00404904 */
    goto mi_00404b0d;
mi_00404909:;
    /* 00404909 */
    a=f->r[5];b=0x00000045u;t=mi_alu(f,a,b,32,1);
    /* 0040490c */
    if(!(f->flags&MI_ZF))goto mi_00404920;
    /* 0040490e */
    a=MI_U16((0x00784db8u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404915 */
    if((f->flags&MI_ZF))goto mi_00404b13;
    /* 0040491b */
    goto mi_00404b0d;
mi_00404920:;
    /* 00404920 */
    a=f->r[5];b=0x00000061u;t=mi_alu(f,a,b,32,1);
    /* 00404923 */
    if(!(f->flags&MI_ZF))goto mi_0040496b;
    /* 00404925 */
    a=MI_U16((0x00784db0u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 0040492c */
    if(!(f->flags&MI_ZF))goto mi_00404b0d;
    /* 00404932 */
    a=MI_U16((0x00784db4u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404939 */
    if(!(f->flags&MI_ZF))goto mi_00404b0d;
    /* 0040493f */
    a=MI_U16((0x00784db6u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404946 */
    if(!(f->flags&MI_ZF))goto mi_00404b0d;
    /* 0040494c */
    a=MI_U16((0x00784db2u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404953 */
    if(!(f->flags&MI_ZF))goto mi_00404b0d;
    /* 00404959 */
    a=MI_U16((0x00784db8u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404960 */
    if((f->flags&MI_ZF))goto mi_00404b13;
    /* 00404966 */
    goto mi_00404b0d;
mi_0040496b:;
    /* 0040496b */
    a=f->r[5];b=0x0000003du;t=mi_alu(f,a,b,32,1);
    /* 0040496e */
    if(!(f->flags&MI_ZF))goto mi_00404982;
    /* 00404970 */
    a=MI_U16((0x007848e4u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404977 */
    if((f->flags&MI_ZF))goto mi_00404b13;
    /* 0040497d */
    goto mi_00404b0d;
mi_00404982:;
    /* 00404982 */
    a=f->r[5];b=0x0000003eu;t=mi_alu(f,a,b,32,1);
    /* 00404985 */
    if(!(f->flags&MI_ZF))goto mi_00404999;
    /* 00404987 */
    a=MI_U16((0x007848e6u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 0040498e */
    if((f->flags&MI_ZF))goto mi_00404b13;
    /* 00404994 */
    goto mi_00404b0d;
mi_00404999:;
    /* 00404999 */
    a=f->r[5];b=0x0000003fu;t=mi_alu(f,a,b,32,1);
    /* 0040499c */
    if(!(f->flags&MI_ZF))goto mi_004049b0;
    /* 0040499e */
    a=MI_U16((0x007848e8u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 004049a5 */
    if((f->flags&MI_ZF))goto mi_00404b13;
    /* 004049ab */
    goto mi_00404b0d;
mi_004049b0:;
    /* 004049b0 */
    a=f->r[5];b=0x00000060u;t=mi_alu(f,a,b,32,1);
    /* 004049b3 */
    if(!(f->flags&MI_ZF))goto mi_004049fb;
    /* 004049b5 */
    a=MI_U16((0x007848e2u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 004049bc */
    if(!(f->flags&MI_ZF))goto mi_00404b0d;
    /* 004049c2 */
    a=MI_U16((0x007848e6u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 004049c9 */
    if(!(f->flags&MI_ZF))goto mi_00404b0d;
    /* 004049cf */
    a=MI_U16((0x007848e8u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 004049d6 */
    if(!(f->flags&MI_ZF))goto mi_00404b0d;
    /* 004049dc */
    a=MI_U16((0x007848e4u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 004049e3 */
    if(!(f->flags&MI_ZF))goto mi_00404b0d;
    /* 004049e9 */
    a=MI_U16((0x007848eau));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 004049f0 */
    if((f->flags&MI_ZF))goto mi_00404b13;
    /* 004049f6 */
    goto mi_00404b0d;
mi_004049fb:;
    /* 004049fb */
    a=f->r[5];b=0x0000008du;t=mi_alu(f,a,b,32,1);
    /* 00404a01 */
    if(!(f->flags&MI_ZF))goto mi_00404a15;
    /* 00404a03 */
    a=MI_U16((0x007848e4u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404a0a */
    if((f->flags&MI_ZF))goto mi_00404b13;
    /* 00404a10 */
    goto mi_00404b0d;
mi_00404a15:;
    /* 00404a15 */
    a=f->r[5];b=0x0000008eu;t=mi_alu(f,a,b,32,1);
    /* 00404a1b */
    if(!(f->flags&MI_ZF))goto mi_00404a2f;
    /* 00404a1d */
    a=MI_U16((0x007848e6u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404a24 */
    if((f->flags&MI_ZF))goto mi_00404b13;
    /* 00404a2a */
    goto mi_00404b0d;
mi_00404a2f:;
    /* 00404a2f */
    a=f->r[5];b=0x0000008fu;t=mi_alu(f,a,b,32,1);
    /* 00404a35 */
    if(!(f->flags&MI_ZF))goto mi_00404a49;
    /* 00404a37 */
    a=MI_U16((0x007848e8u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404a3e */
    if((f->flags&MI_ZF))goto mi_00404b13;
    /* 00404a44 */
    goto mi_00404b0d;
mi_00404a49:;
    /* 00404a49 */
    a=f->r[5];b=0x00000096u;t=mi_alu(f,a,b,32,1);
    /* 00404a4f */
    if(!(f->flags&MI_ZF))goto mi_00404a94;
    /* 00404a51 */
    a=MI_U16((0x007848e2u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404a58 */
    if(!(f->flags&MI_ZF))goto mi_00404b0d;
    /* 00404a5e */
    a=MI_U16((0x007848e6u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404a65 */
    if(!(f->flags&MI_ZF))goto mi_00404b0d;
    /* 00404a6b */
    a=MI_U16((0x007848e8u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404a72 */
    if(!(f->flags&MI_ZF))goto mi_00404b0d;
    /* 00404a78 */
    a=MI_U16((0x007848e4u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404a7f */
    if(!(f->flags&MI_ZF))goto mi_00404b0d;
    /* 00404a85 */
    a=MI_U16((0x007848eau));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404a8c */
    if((f->flags&MI_ZF))goto mi_00404b13;
    /* 00404a92 */
    goto mi_00404b0d;
mi_00404a94:;
    /* 00404a94 */
    a=f->r[5];b=0x00000029u;t=mi_alu(f,a,b,32,1);
    /* 00404a97 */
    if(!(f->flags&MI_ZF))goto mi_00404aa4;
    /* 00404a99 */
    a=MI_U16((0x00784d20u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404aa0 */
    if((f->flags&MI_ZF))goto mi_00404b13;
    /* 00404aa2 */
    goto mi_00404b0d;
mi_00404aa4:;
    /* 00404aa4 */
    a=f->r[5];b=0x0000002au;t=mi_alu(f,a,b,32,1);
    /* 00404aa7 */
    if(!(f->flags&MI_ZF))goto mi_00404ab4;
    /* 00404aa9 */
    a=MI_U16((0x00784d20u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404ab0 */
    if((f->flags&MI_ZF))goto mi_00404b13;
    /* 00404ab2 */
    goto mi_00404b0d;
mi_00404ab4:;
    /* 00404ab4 */
    a=f->r[5];b=0x0000002bu;t=mi_alu(f,a,b,32,1);
    /* 00404ab7 */
    if(!(f->flags&MI_ZF))goto mi_00404ac4;
    /* 00404ab9 */
    a=MI_U16((0x00784d20u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404ac0 */
    if((f->flags&MI_ZF))goto mi_00404b13;
    /* 00404ac2 */
    goto mi_00404b0d;
mi_00404ac4:;
    /* 00404ac4 */
    a=f->r[5];b=0x0000005bu;t=mi_alu(f,a,b,32,1);
    /* 00404ac7 */
    if(!(f->flags&MI_ZF))goto mi_00404ae6;
    /* 00404ac9 */
    a=MI_U16((0x00784d20u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404ad0 */
    if(!(f->flags&MI_ZF))goto mi_00404b0d;
    /* 00404ad2 */
    a=MI_U16((0x00784d1eu));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404ad9 */
    if(!(f->flags&MI_ZF))goto mi_00404b0d;
    /* 00404adb */
    a=MI_U16((0x00784d1cu));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404ae2 */
    if((f->flags&MI_ZF))goto mi_00404b13;
    /* 00404ae4 */
    goto mi_00404b0d;
mi_00404ae6:;
    /* 00404ae6 */
    a=f->r[5];b=0x00000026u;t=mi_alu(f,a,b,32,1);
    /* 00404ae9 */
    if(!(f->flags&MI_ZF))goto mi_00404af6;
    /* 00404aeb */
    a=MI_U16((0x00784d1eu));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404af2 */
    if((f->flags&MI_ZF))goto mi_00404b13;
    /* 00404af4 */
    goto mi_00404b0d;
mi_00404af6:;
    /* 00404af6 */
    a=f->r[5];b=0x00000034u;t=mi_alu(f,a,b,32,1);
    /* 00404af9 */
    if(!(f->flags&MI_ZF))goto mi_00404b13;
    /* 00404afb */
    a=MI_U16((0x00784d1au));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404b02 */
    if(!(f->flags&MI_ZF))goto mi_00404b0d;
    /* 00404b04 */
    a=MI_U16((0x00784d18u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404b0b */
    if((f->flags&MI_ZF))goto mi_00404b13;
mi_00404b0d:;
    /* 00404b0d */
    MI_U32((0x0070a8dcu+f->r[6]))=(uint32_t)(f->r[3]);
mi_00404b13:;
    /* 00404b13 */
    f->r[1]=MI_U32((0x00e38ebcu));
    /* 00404b19 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38ea8u)))&0xffu)<<0);
    /* 00404b1e */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 00404b20 */
    if(!(f->flags&MI_ZF))goto mi_00404b44;
    /* 00404b22 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00404b24 */
    if(!(f->flags&MI_ZF))goto mi_00404b44;
    /* 00404b26 */
    a=f->r[5];b=0x00000037u;t=mi_alu(f,a,b,32,1);
    /* 00404b29 */
    if(!(f->flags&MI_ZF))goto mi_00404b44;
    /* 00404b2b */
    f->r[1]=(uint32_t)(int32_t)(int8_t)(f->r[0]&0xffu);
    /* 00404b2e */
    MI_U32((0x0070a8dcu+f->r[6]))=(uint32_t)(f->r[7]);
    /* 00404b34 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 00404b36 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00404b3a */
    f->r[0]=0x005794b4u;
    /* 00404b3f */
    goto mi_004050e3;
mi_00404b44:;
    /* 00404b44 */
    a=f->r[5];b=0x00000066u;t=mi_alu(f,a,b,32,1);
    /* 00404b47 */
    if(!(f->flags&MI_ZF))goto mi_00404b5f;
    /* 00404b49 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
mi_00404b4a:;
    /* 00404b4a */
    f->r[4]-=4;MI_U32(f->r[4])=0x00404b4fu;f->boundary(0x00404b4au,0x00443b90u,f);
mi_00404b4f:;
    /* 00404b4f */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00404b52 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
mi_00404b54:;
    /* 00404b54 */
    if((f->flags&MI_ZF))goto mi_004050c0;
    /* 00404b5a */
    goto mi_004050ba;
mi_00404b5f:;
    /* 00404b5f */
    a=f->r[5];b=0x000000c2u;t=mi_alu(f,a,b,32,1);
    /* 00404b65 */
    if(!(f->flags&MI_ZF))goto mi_00404b6b;
    /* 00404b67 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00404b69 */
    goto mi_00404b4a;
mi_00404b6b:;
    /* 00404b6b */
    a=f->r[5];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 00404b6e */
    if(!(f->flags&MI_ZF))goto mi_00404b79;
    /* 00404b70 */
    a=MI_U16((0x00784ce8u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404b77 */
    goto mi_00404b54;
mi_00404b79:;
    /* 00404b79 */
    a=f->r[5];b=0x000000bcu;t=mi_alu(f,a,b,32,1);
    /* 00404b7f */
    if(!(f->flags&MI_ZF))goto mi_00404bb7;
    /* 00404b81 */
    a=MI_U16((0x00784d12u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
mi_00404b88:;
    /* 00404b88 */
    if((f->flags&MI_ZF))goto mi_004050c0;
    /* 00404b8e */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 00404b94 */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 00404b9a */
    f->r[0]=MI_U32((0x00ea5cbcu+f->r[2]));
    /* 00404ba0 */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 00404ba3 */
    if(mi_signed_less(f->flags))goto mi_004050c0;
    /* 00404ba9 */
    a=f->r[0];b=0x00000008u;t=mi_alu(f,a,b,32,1);
mi_00404bac:;
    /* 00404bac */
    if(!mi_signed_less(f->flags))goto mi_004050c0;
    /* 00404bb2 */
    goto mi_004050ba;
mi_00404bb7:;
    /* 00404bb7 */
    a=f->r[5];b=0x000000bdu;t=mi_alu(f,a,b,32,1);
    /* 00404bbd */
    if(!(f->flags&MI_ZF))goto mi_00404bf4;
    /* 00404bbf */
    a=MI_U16((0x00784d12u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404bc6 */
    if((f->flags&MI_ZF))goto mi_004050c0;
    /* 00404bcc */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 00404bd1 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 00404bd7 */
    f->r[0]=MI_U32((0x00ea5cbcu+f->r[0]));
    /* 00404bdd */
    a=f->r[0];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 00404be0 */
    if(mi_signed_less(f->flags))goto mi_004050c0;
    /* 00404be6 */
    a=f->r[0];b=0x0000000bu;t=mi_alu(f,a,b,32,1);
    /* 00404be9 */
    if(!mi_signed_less(f->flags))goto mi_004050c0;
    /* 00404bef */
    goto mi_004050ba;
mi_00404bf4:;
    /* 00404bf4 */
    a=f->r[5];b=0x0000004du;t=mi_alu(f,a,b,32,1);
    /* 00404bf7 */
    if(!(f->flags&MI_ZF))goto mi_00404c2e;
    /* 00404bf9 */
    a=MI_U16((0x00784d14u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404c00 */
    if((f->flags&MI_ZF))goto mi_004050c0;
    /* 00404c06 */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 00404c0c */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 00404c12 */
    f->r[0]=MI_U32((0x00ea5cbcu+f->r[1]));
    /* 00404c18 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 00404c1a */
    if(mi_signed_less(f->flags))goto mi_004050c0;
    /* 00404c20 */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 00404c23 */
    if(!mi_signed_less(f->flags))goto mi_004050c0;
    /* 00404c29 */
    goto mi_004050ba;
mi_00404c2e:;
    /* 00404c2e */
    a=f->r[5];b=0x0000004eu;t=mi_alu(f,a,b,32,1);
    /* 00404c31 */
    if(!(f->flags&MI_ZF))goto mi_00404c3f;
    /* 00404c33 */
    a=MI_U16((0x00784d14u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404c3a */
    goto mi_00404b88;
mi_00404c3f:;
    /* 00404c3f */
    a=f->r[5];b=0x0000004fu;t=mi_alu(f,a,b,32,1);
    /* 00404c42 */
    if(!(f->flags&MI_ZF))goto mi_00404c73;
    /* 00404c44 */
    a=MI_U16((0x00784d14u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404c4b */
    if((f->flags&MI_ZF))goto mi_004050c0;
    /* 00404c51 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 00404c56 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 00404c5c */
    f->r[0]=MI_U32((0x00ea5cbcu+f->r[0]));
    /* 00404c62 */
    a=f->r[0];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 00404c65 */
    if(mi_signed_less(f->flags))goto mi_004050c0;
    /* 00404c6b */
    a=f->r[0];b=0x0000000bu;t=mi_alu(f,a,b,32,1);
    /* 00404c6e */
    goto mi_00404bac;
mi_00404c73:;
    /* 00404c73 */
    a=f->r[5];b=0x00000007u;t=mi_alu(f,a,b,32,1);
    /* 00404c76 */
    if(!(f->flags&MI_ZF))goto mi_00404c84;
    /* 00404c78 */
    a=MI_U16((0x00784ce6u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404c7f */
    goto mi_00404b54;
mi_00404c84:;
    /* 00404c84 */
    a=f->r[5];b=0x0000001eu;t=mi_alu(f,a,b,32,1);
    /* 00404c87 */
    if(!(f->flags&MI_ZF))goto mi_00404c95;
    /* 00404c89 */
    a=MI_U16((0x00784cf8u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404c90 */
    goto mi_00404b54;
mi_00404c95:;
    /* 00404c95 */
    a=f->r[5];b=0x00000022u;t=mi_alu(f,a,b,32,1);
    /* 00404c98 */
    if(!(f->flags&MI_ZF))goto mi_00404ca6;
    /* 00404c9a */
    a=MI_U16((0x00784cf8u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404ca1 */
    goto mi_00404b54;
mi_00404ca6:;
    /* 00404ca6 */
    a=f->r[5];b=0x00000021u;t=mi_alu(f,a,b,32,1);
    /* 00404ca9 */
    if(!(f->flags&MI_ZF))goto mi_00404cb7;
    /* 00404cab */
    a=MI_U16((0x00784cfau));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404cb2 */
    goto mi_00404b54;
mi_00404cb7:;
    /* 00404cb7 */
    a=f->r[5];b=0x00000023u;t=mi_alu(f,a,b,32,1);
    /* 00404cba */
    if(!(f->flags&MI_ZF))goto mi_00404cc8;
    /* 00404cbc */
    a=MI_U16((0x00784cfau));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404cc3 */
    goto mi_00404b54;
mi_00404cc8:;
    /* 00404cc8 */
    a=f->r[5];b=0x00000020u;t=mi_alu(f,a,b,32,1);
    /* 00404ccb */
    if(!(f->flags&MI_ZF))goto mi_00404cd9;
    /* 00404ccd */
    a=MI_U16((0x00784cfcu));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404cd4 */
    goto mi_00404b54;
mi_00404cd9:;
    /* 00404cd9 */
    a=f->r[5];b=0x000000e2u;t=mi_alu(f,a,b,32,1);
    /* 00404cdf */
    if(!(f->flags&MI_ZF))goto mi_00404ced;
    /* 00404ce1 */
    a=MI_U16((0x00784d3au));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404ce8 */
    goto mi_00404b54;
mi_00404ced:;
    /* 00404ced */
    a=f->r[5];b=0x0000004bu;t=mi_alu(f,a,b,32,1);
    /* 00404cf0 */
    if(!(f->flags&MI_ZF))goto mi_00404cfe;
    /* 00404cf2 */
    a=MI_U16((0x00784cf4u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404cf9 */
    goto mi_00404b54;
mi_00404cfe:;
    /* 00404cfe */
    a=f->r[5];b=0x000000c0u;t=mi_alu(f,a,b,32,1);
    /* 00404d04 */
    if(!(f->flags&MI_ZF))goto mi_00404d12;
    /* 00404d06 */
    a=MI_U16((0x00784cf4u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404d0d */
    goto mi_00404b54;
mi_00404d12:;
    /* 00404d12 */
    a=f->r[5];b=0x0000004cu;t=mi_alu(f,a,b,32,1);
    /* 00404d15 */
    if(!(f->flags&MI_ZF))goto mi_00404d35;
    /* 00404d17 */
    a=0x00000007u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00404d19 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00404d1eu;f->boundary(0x00404d19u,0x00443b90u,f);
    /* 00404d1e */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00404d21 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00404d23 */
    if((f->flags&MI_ZF))goto mi_004050c0;
    /* 00404d29 */
    a=MI_U16((0x00784ceau));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404d30 */
    goto mi_00404b54;
mi_00404d35:;
    /* 00404d35 */
    a=f->r[5];b=0x000000b5u;t=mi_alu(f,a,b,32,1);
    /* 00404d3b */
    if(!(f->flags&MI_ZF))goto mi_00404d49;
    /* 00404d3d */
    a=MI_U16((0x00784d38u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404d44 */
    goto mi_00404b54;
mi_00404d49:;
    /* 00404d49 */
    a=f->r[5];b=0x000000b6u;t=mi_alu(f,a,b,32,1);
    /* 00404d4f */
    if(!(f->flags&MI_ZF))goto mi_00404d5d;
    /* 00404d51 */
    a=MI_U16((0x00784d36u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404d58 */
    goto mi_00404b54;
mi_00404d5d:;
    /* 00404d5d */
    a=f->r[5];b=0x000000a9u;t=mi_alu(f,a,b,32,1);
    /* 00404d63 */
    if(!(f->flags&MI_ZF))goto mi_00404d71;
    /* 00404d65 */
    a=MI_U16((0x00784d22u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404d6c */
    goto mi_00404b54;
mi_00404d71:;
    /* 00404d71 */
    a=f->r[5];b=0x000000adu;t=mi_alu(f,a,b,32,1);
    /* 00404d77 */
    if(!(f->flags&MI_ZF))goto mi_00404d85;
    /* 00404d79 */
    a=MI_U16((0x00784d24u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404d80 */
    goto mi_00404b54;
mi_00404d85:;
    /* 00404d85 */
    a=f->r[5];b=0x000000cau;t=mi_alu(f,a,b,32,1);
    /* 00404d8b */
    if(!(f->flags&MI_ZF))goto mi_00404d99;
    /* 00404d8d */
    a=MI_U16((0x00784d26u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404d94 */
    goto mi_00404b54;
mi_00404d99:;
    /* 00404d99 */
    a=f->r[5];b=0x00000039u;t=mi_alu(f,a,b,32,1);
    /* 00404d9c */
    if(!(f->flags&MI_ZF))goto mi_00404dc4;
    /* 00404d9e */
    a=MI_U16((0x00784d2au));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404da5 */
    if(!(f->flags&MI_ZF))goto mi_004050ba;
    /* 00404dab */
    a=MI_U16((0x00784d2cu));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404db2 */
    if(!(f->flags&MI_ZF))goto mi_004050ba;
    /* 00404db8 */
    a=MI_U16((0x00784d2eu));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404dbf */
    goto mi_00404b54;
mi_00404dc4:;
    /* 00404dc4 */
    a=f->r[5];b=0x0000005eu;t=mi_alu(f,a,b,32,1);
    /* 00404dc7 */
    if(!(f->flags&MI_ZF))goto mi_00404dd5;
    /* 00404dc9 */
    a=MI_U16((0x00784d30u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404dd0 */
    goto mi_00404b54;
mi_00404dd5:;
    /* 00404dd5 */
    a=f->r[5];b=0x00000028u;t=mi_alu(f,a,b,32,1);
    /* 00404dd8 */
    if(!(f->flags&MI_ZF))goto mi_00404de6;
    /* 00404dda */
    a=MI_U16((0x00784d2eu));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404de1 */
    goto mi_00404b54;
mi_00404de6:;
    /* 00404de6 */
    a=f->r[5];b=0x0000002cu;t=mi_alu(f,a,b,32,1);
    /* 00404de9 */
    if(!(f->flags&MI_ZF))goto mi_00404df7;
    /* 00404deb */
    a=MI_U16((0x00784d2cu));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404df2 */
    goto mi_00404b54;
mi_00404df7:;
    /* 00404df7 */
    a=f->r[5];b=0x0000002du;t=mi_alu(f,a,b,32,1);
    /* 00404dfa */
    if(!(f->flags&MI_ZF))goto mi_00404e08;
    /* 00404dfc */
    a=MI_U16((0x00784d2au));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404e03 */
    goto mi_00404b54;
mi_00404e08:;
    /* 00404e08 */
    a=f->r[5];b=0x0000005fu;t=mi_alu(f,a,b,32,1);
    /* 00404e0b */
    if(!(f->flags&MI_ZF))goto mi_00404e19;
    /* 00404e0d */
    a=MI_U16((0x00784d28u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404e14 */
    goto mi_00404b54;
mi_00404e19:;
    /* 00404e19 */
    a=f->r[5];b=0x000000beu;t=mi_alu(f,a,b,32,1);
    /* 00404e1f */
    if(!(f->flags&MI_ZF))goto mi_00404e2d;
    /* 00404e21 */
    a=MI_U16((0x00784cf0u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404e28 */
    goto mi_00404b54;
mi_00404e2d:;
    /* 00404e2d */
    a=f->r[5];b=0x000000bfu;t=mi_alu(f,a,b,32,1);
    /* 00404e33 */
    if(!(f->flags&MI_ZF))goto mi_00404e41;
    /* 00404e35 */
    a=MI_U16((0x00784ceeu));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404e3c */
    goto mi_00404b54;
mi_00404e41:;
    /* 00404e41 */
    a=f->r[5];b=0x000000c1u;t=mi_alu(f,a,b,32,1);
    /* 00404e47 */
    if(!(f->flags&MI_ZF))goto mi_00404e55;
    /* 00404e49 */
    a=MI_U16((0x00784cf2u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404e50 */
    goto mi_00404b54;
mi_00404e55:;
    /* 00404e55 */
    a=f->r[5];b=0x000000b0u;t=mi_alu(f,a,b,32,1);
    /* 00404e5b */
    if(!(f->flags&MI_ZF))goto mi_00404e83;
    /* 00404e5d */
    a=MI_U16((0x00784d22u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404e64 */
    if(!(f->flags&MI_ZF))goto mi_004050ba;
    /* 00404e6a */
    a=MI_U16((0x00784d24u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404e71 */
    if(!(f->flags&MI_ZF))goto mi_004050ba;
    /* 00404e77 */
    a=MI_U16((0x00784d26u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404e7e */
    goto mi_00404b54;
mi_00404e83:;
    /* 00404e83 */
    a=f->r[5];b=0x00000067u;t=mi_alu(f,a,b,32,1);
    /* 00404e86 */
    if(!(f->flags&MI_ZF))goto mi_00404e8f;
    /* 00404e88 */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00404e8a */
    goto mi_00404b4a;
mi_00404e8f:;
    /* 00404e8f */
    a=f->r[5];b=0x00000069u;t=mi_alu(f,a,b,32,1);
    /* 00404e92 */
    if(!(f->flags&MI_ZF))goto mi_00404e9b;
    /* 00404e94 */
    a=0x00000004u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00404e96 */
    goto mi_00404b4a;
mi_00404e9b:;
    /* 00404e9b */
    a=f->r[5];b=0x00000068u;t=mi_alu(f,a,b,32,1);
    /* 00404e9e */
    if(!(f->flags&MI_ZF))goto mi_00404ea7;
    /* 00404ea0 */
    a=0x00000005u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00404ea2 */
    goto mi_00404b4a;
mi_00404ea7:;
    /* 00404ea7 */
    a=f->r[5];b=0x00000070u;t=mi_alu(f,a,b,32,1);
    /* 00404eaa */
    if(!(f->flags&MI_ZF))goto mi_00404eb8;
    /* 00404eac */
    a=MI_U16((0x00784d32u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404eb3 */
    goto mi_00404b54;
mi_00404eb8:;
    /* 00404eb8 */
    a=f->r[5];b=0x00000071u;t=mi_alu(f,a,b,32,1);
    /* 00404ebb */
    if(!(f->flags&MI_ZF))goto mi_00404eed;
    /* 00404ebd */
    a=0x00000012u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00404ebf */
    f->r[4]-=4;MI_U32(f->r[4])=0x00404ec4u;f->boundary(0x00404ebfu,0x00443d90u,f);
    /* 00404ec4 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00404ec7 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00404ec9 */
    if((f->flags&MI_ZF))goto mi_004050c0;
    /* 00404ecf */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38ea8u)))&0xffu)<<0);
    /* 00404ed4 */
    a=(f->r[0]&0xffu);b=0x00000005u;t=mi_alu(f,a,b,8,1);
    /* 00404ed6 */
    if(!(f->flags&MI_ZF))goto mi_004050ba;
    /* 00404edc */
    a=MI_U32((0x00e38ebcu));b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 00404ee2 */
    if((f->flags&MI_ZF))goto mi_004050d1;
    /* 00404ee8 */
    goto mi_004050ba;
mi_00404eed:;
    /* 00404eed */
    a=f->r[5];b=0x0000006au;t=mi_alu(f,a,b,32,1);
    /* 00404ef0 */
    if(!(f->flags&MI_ZF))goto mi_00404ef9;
    /* 00404ef2 */
    a=0x00000018u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00404ef4 */
    goto mi_00404b4a;
mi_00404ef9:;
    /* 00404ef9 */
    a=f->r[5];b=0x000000a2u;t=mi_alu(f,a,b,32,1);
    /* 00404eff */
    if(!(f->flags&MI_ZF))goto mi_00404f08;
    /* 00404f01 */
    a=0x00000012u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00404f03 */
    goto mi_00404b4a;
mi_00404f08:;
    /* 00404f08 */
    a=f->r[5];b=0x000000d9u;t=mi_alu(f,a,b,32,1);
    /* 00404f0e */
    if(!(f->flags&MI_ZF))goto mi_00404f17;
    /* 00404f10 */
    a=0x0000001du;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00404f12 */
    goto mi_00404b4a;
mi_00404f17:;
    /* 00404f17 */
    a=f->r[5];b=0x0000006cu;t=mi_alu(f,a,b,32,1);
    /* 00404f1a */
    if(!(f->flags&MI_ZF))goto mi_00404f23;
    /* 00404f1c */
    a=0x00000014u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00404f1e */
    goto mi_00404b4a;
mi_00404f23:;
    /* 00404f23 */
    a=f->r[5];b=0x000000ddu;t=mi_alu(f,a,b,32,1);
    /* 00404f29 */
    if(!(f->flags&MI_ZF))goto mi_00404f32;
    /* 00404f2b */
    a=0x0000001eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00404f2d */
    goto mi_00404b4a;
mi_00404f32:;
    /* 00404f32 */
    a=f->r[5];b=0x0000006bu;t=mi_alu(f,a,b,32,1);
    /* 00404f35 */
    if(!(f->flags&MI_ZF))goto mi_00404f3e;
    /* 00404f37 */
    a=0x00000019u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00404f39 */
    goto mi_00404b4a;
mi_00404f3e:;
    /* 00404f3e */
    a=f->r[5];b=0x000000d8u;t=mi_alu(f,a,b,32,1);
    /* 00404f44 */
    if(!(f->flags&MI_ZF))goto mi_00404f4d;
    /* 00404f46 */
    a=0x0000001au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00404f48 */
    goto mi_00404b4a;
mi_00404f4d:;
    /* 00404f4d */
    a=f->r[5];b=0x000000b3u;t=mi_alu(f,a,b,32,1);
    /* 00404f53 */
    if(!(f->flags&MI_ZF))goto mi_00404f5c;
    /* 00404f55 */
    a=0x00000018u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00404f57 */
    goto mi_00404b4a;
mi_00404f5c:;
    /* 00404f5c */
    a=f->r[5];b=0x000000cdu;t=mi_alu(f,a,b,32,1);
    /* 00404f62 */
    if(!(f->flags&MI_ZF))goto mi_00404f70;
    /* 00404f64 */
    a=MI_U16((0x00784d34u));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00404f6b */
    goto mi_00404b54;
mi_00404f70:;
    /* 00404f70 */
    a=f->r[5];b=0x000000ccu;t=mi_alu(f,a,b,32,1);
    /* 00404f76 */
    if(!(f->flags&MI_ZF))goto mi_00405020;
    /* 00404f7c */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 00404f7e */
    if(!(f->flags&MI_ZF))goto mi_00404f88;
    /* 00404f80 */
    a=(f->r[0]&0xffu);b=0x00000007u;t=mi_alu(f,a,b,8,1);
    /* 00404f82 */
    if(mi_signed_less(f->flags))goto mi_004050c0;
mi_00404f88:;
    /* 00404f88 */
    a=0x0000000bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00404f8a */
    f->r[4]-=4;MI_U32(f->r[4])=0x00404f8fu;f->boundary(0x00404f8au,0x00443b90u,f);
    /* 00404f8f */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00404f92 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00404f94 */
    if(!(f->flags&MI_ZF))goto mi_00404fa4;
    /* 00404f96 */
    a=0x0000000bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00404f98 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00404f9du;f->boundary(0x00404f98u,0x00443d90u,f);
    /* 00404f9d */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00404fa0 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00404fa2 */
    if((f->flags&MI_ZF))goto mi_00404fb5;
mi_00404fa4:;
    /* 00404fa4 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00404fa5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00404faau;f->boundary(0x00404fa5u,0x00443b90u,f);
    /* 00404faa */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00404fad */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00404faf */
    if(!(f->flags&MI_ZF))goto mi_004050ba;
mi_00404fb5:;
    /* 00404fb5 */
    a=0x0000000bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00404fb7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00404fbcu;f->boundary(0x00404fb7u,0x00443b90u,f);
    /* 00404fbc */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00404fbf */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00404fc1 */
    if(!(f->flags&MI_ZF))goto mi_00404fd1;
    /* 00404fc3 */
    a=0x0000000bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00404fc5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00404fcau;f->boundary(0x00404fc5u,0x00443d90u,f);
    /* 00404fca */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00404fcd */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00404fcf */
    if((f->flags&MI_ZF))goto mi_00404fe3;
mi_00404fd1:;
    /* 00404fd1 */
    a=0x00000009u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00404fd3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00404fd8u;f->boundary(0x00404fd3u,0x004439c0u,f);
    /* 00404fd8 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00404fdb */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00404fdd */
    if(!(f->flags&MI_ZF))goto mi_004050ba;
mi_00404fe3:;
    /* 00404fe3 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00404fe4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00404fe9u;f->boundary(0x00404fe4u,0x00443b90u,f);
    /* 00404fe9 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00404fec */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00404fee */
    if((f->flags&MI_ZF))goto mi_00405002;
    /* 00404ff0 */
    a=0x0000000bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00404ff2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00404ff7u;f->boundary(0x00404ff2u,0x004439c0u,f);
    /* 00404ff7 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00404ffa */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00404ffc */
    if(!(f->flags&MI_ZF))goto mi_004050ba;
mi_00405002:;
    /* 00405002 */
    a=0x00000009u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00405004 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00405009u;f->boundary(0x00405004u,0x004439c0u,f);
    /* 00405009 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040500c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0040500e */
    if((f->flags&MI_ZF))goto mi_004050c0;
    /* 00405014 */
    a=0x0000000bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00405016 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040501bu;f->boundary(0x00405016u,0x004439c0u,f);
    /* 0040501b */
    goto mi_00404b4f;
mi_00405020:;
    /* 00405020 */
    a=f->r[5];b=0x0000009cu;t=mi_alu(f,a,b,32,1);
    /* 00405026 */
    if((f->flags&MI_ZF))goto mi_00405048;
    /* 00405028 */
    a=f->r[5];b=0x0000009du;t=mi_alu(f,a,b,32,1);
    /* 0040502e */
    if((f->flags&MI_ZF))goto mi_00405048;
    /* 00405030 */
    a=f->r[5];b=0x0000009eu;t=mi_alu(f,a,b,32,1);
    /* 00405036 */
    if((f->flags&MI_ZF))goto mi_00405048;
    /* 00405038 */
    a=f->r[5];b=0x0000009fu;t=mi_alu(f,a,b,32,1);
    /* 0040503e */
    if((f->flags&MI_ZF))goto mi_00405048;
    /* 00405040 */
    a=f->r[5];b=0x000000a0u;t=mi_alu(f,a,b,32,1);
    /* 00405046 */
    if(!(f->flags&MI_ZF))goto mi_0040504e;
mi_00405048:;
    /* 00405048 */
    MI_U32((0x0070a8dcu+f->r[6]))=(uint32_t)(f->r[7]);
mi_0040504e:;
    /* 0040504e */
    a=f->r[5];b=0x00000097u;t=mi_alu(f,a,b,32,1);
    /* 00405054 */
    if((f->flags&MI_ZF))goto mi_00405076;
    /* 00405056 */
    a=f->r[5];b=0x00000098u;t=mi_alu(f,a,b,32,1);
    /* 0040505c */
    if((f->flags&MI_ZF))goto mi_00405076;
    /* 0040505e */
    a=f->r[5];b=0x00000099u;t=mi_alu(f,a,b,32,1);
    /* 00405064 */
    if((f->flags&MI_ZF))goto mi_00405076;
    /* 00405066 */
    a=f->r[5];b=0x0000009au;t=mi_alu(f,a,b,32,1);
    /* 0040506c */
    if((f->flags&MI_ZF))goto mi_00405076;
    /* 0040506e */
    a=f->r[5];b=0x0000009bu;t=mi_alu(f,a,b,32,1);
    /* 00405074 */
    if(!(f->flags&MI_ZF))goto mi_0040507c;
mi_00405076:;
    /* 00405076 */
    MI_U32((0x0070a8dcu+f->r[6]))=(uint32_t)(f->r[7]);
mi_0040507c:;
    /* 0040507c */
    a=f->r[5];b=0x00000091u;t=mi_alu(f,a,b,32,1);
    /* 00405082 */
    if((f->flags&MI_ZF))goto mi_004050a4;
    /* 00405084 */
    a=f->r[5];b=0x00000092u;t=mi_alu(f,a,b,32,1);
    /* 0040508a */
    if((f->flags&MI_ZF))goto mi_004050a4;
    /* 0040508c */
    a=f->r[5];b=0x00000093u;t=mi_alu(f,a,b,32,1);
    /* 00405092 */
    if((f->flags&MI_ZF))goto mi_004050a4;
    /* 00405094 */
    a=f->r[5];b=0x00000094u;t=mi_alu(f,a,b,32,1);
    /* 0040509a */
    if((f->flags&MI_ZF))goto mi_004050a4;
    /* 0040509c */
    a=f->r[5];b=0x00000095u;t=mi_alu(f,a,b,32,1);
    /* 004050a2 */
    if(!(f->flags&MI_ZF))goto mi_004050aa;
mi_004050a4:;
    /* 004050a4 */
    MI_U32((0x0070a8dcu+f->r[6]))=(uint32_t)(f->r[7]);
mi_004050aa:;
    /* 004050aa */
    a=f->r[5];b=0x000000d3u;t=mi_alu(f,a,b,32,1);
    /* 004050b0 */
    if((f->flags&MI_ZF))goto mi_004050ba;
    /* 004050b2 */
    a=f->r[5];b=0x000000d4u;t=mi_alu(f,a,b,32,1);
    /* 004050b8 */
    if(!(f->flags&MI_ZF))goto mi_004050c0;
mi_004050ba:;
    /* 004050ba */
    MI_U32((0x0070a8dcu+f->r[6]))=(uint32_t)(f->r[3]);
mi_004050c0:;
    /* 004050c0 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38ea8u)))&0xffu)<<0);
    /* 004050c5 */
    a=(f->r[0]&0xffu);b=0x0000000au;t=mi_alu(f,a,b,8,1);
    /* 004050c7 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0040513f;
    /* 004050c9 */
    a=MI_U32((0x00e38ebcu));b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004050cf */
    if(!(f->flags&MI_ZF))goto mi_0040513f;
mi_004050d1:;
    /* 004050d1 */
    f->r[1]=(uint32_t)(int32_t)(int8_t)(f->r[0]&0xffu);
    /* 004050d4 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 004050d6 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004050da */
    f->r[0]=0x005794b4u;
    /* 004050df */
    goto mi_004050e3;
mi_004050e1:;
    /* 004050e1 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
mi_004050e3:;
    /* 004050e3 */
    f->r[2]=MI_U32((0x00000018u+f->r[4]));
    /* 004050e7 */
    f->r[1]=MI_U32((0xfffffffcu+f->r[0]));
    /* 004050ea */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004050ec */
    if(!(f->flags&MI_ZF))goto mi_00405118;
    /* 004050ee */
    f->r[2]=MI_U32((0x00000000u+f->r[0]));
    /* 004050f0 */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004050f2 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00405118;
    /* 004050f4 */
    f->r[1]=MI_U32((0x00000004u+f->r[0]));
    /* 004050f7 */
    f->r[7]=f->r[2];
mi_004050f9:;
    /* 004050f9 */
    f->r[2]=MI_U32((0x0000001cu+f->r[4]));
    /* 004050fd */
    a=f->r[2];b=MI_U32((0x00000000u+f->r[1]));t=mi_alu(f,a,b,32,1);
    /* 004050ff */
    if(!(f->flags&MI_ZF))goto mi_00405112;
    /* 00405101 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 00405103 */
    a=MI_U8((0x007027b8u+f->r[5]));b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 00405109 */
    t=(!(f->flags&MI_ZF))?1u:0u;f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0040510c */
    MI_U32((0x0070a8dcu+f->r[6]))=(uint32_t)(f->r[2]);
mi_00405112:;
    /* 00405112 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00405115 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 00405116 */
    if(!(f->flags&MI_ZF))goto mi_004050f9;
mi_00405118:;
    /* 00405118 */
    a=f->r[0];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0040511b */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 0040511c */
    a=f->r[0];b=0x0057978cu;t=mi_alu(f,a,b,32,1);
    /* 00405121 */
    if(mi_signed_less(f->flags))goto mi_004050e1;
    /* 00405123 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 00405125 */
    goto mi_0040513f;
mi_00405127:;
    /* 00405127 */
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
    /* 0040512b */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
    /* 0040512f */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 00405132 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 00405135 */
    f->r[2]=(0x00000000u+f->r[1]+f->r[0]*2u);
    /* 00405138 */
    MI_U32((0x0070a8dcu+f->r[2]*4u))=(uint32_t)(f->r[3]);
mi_0040513f:;
    /* 0040513f */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 00405143 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00405144 */
    a=f->r[0];b=0x0000001eu;t=mi_alu(f,a,b,32,1);
    /* 00405147 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0040514b */
    if(mi_signed_less(f->flags))goto mi_00404366;
    /* 00405151 */
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
    /* 00405155 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00405156 */
    a=f->r[0];b=0x00000026u;t=mi_alu(f,a,b,32,1);
    /* 00405159 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0040515d */
    if(mi_signed_less(f->flags))goto mi_00404362;
    /* 00405163 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00405164 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00405165 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00405166 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00405167 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040516a */
    f->r[4]+=4;return;
}
