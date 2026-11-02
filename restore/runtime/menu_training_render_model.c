/* Whole 8-body/1416-instruction training UI closure contribution.
 * Pinned PE fde7cd65...d33c8d; every native cache item independently byte-checked.
 * Five exact two-target tables and five12-byte mappers are immutable C data.
 * DWORD player/coordinates/counts, WORD resource fields, BYTE flags and native
 * transient ordered publications are preserved without inferred repair clamps. */
#include "menu_training_render.h"
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
static __inline uint32_t mi_alu(struct restore_training_render_frame *f,uint32_t a,uint32_t b,uint32_t width,uint32_t operation)
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
static __inline uint32_t mi_shift(struct restore_training_render_frame *f,uint32_t a,uint32_t count,uint32_t width,uint32_t arithmetic)
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
static __inline uint32_t mi_imul(struct restore_training_render_frame *f,uint32_t a,uint32_t b)
{
 int64_t product=(int64_t)(int32_t)a*(int32_t)b;uint32_t result=(uint32_t)product;
 f->flags&=~(MI_CF|MI_OF);if(product!=(int64_t)(int32_t)result)f->flags|=MI_CF|MI_OF;
 return result;
}
static __inline int mi_signed_less(uint32_t flags){return ((flags&MI_SF)!=0)^((flags&MI_OF)!=0);}
void restore_training_render_C_0040c760(struct restore_training_render_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0040c760 */
    f->r[0]=MI_U32((0x00d3b198u));
    /* 0040c765 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0040c767 */
    if(!(f->flags&MI_ZF))goto mi_0040c797;
    /* 0040c769 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 0040c76e */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0040c774 */
    f->r[2]=MI_U32((0x00ea60a4u+f->r[0]));
    /* 0040c77a */
    f->r[0]=(0x00ea60a4u+f->r[0]);
    /* 0040c780 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0040c781 */
    f->r[1]=f->r[2];
    /* 0040c783 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0040c785 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[2]);
    /* 0040c787 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0040c7c4;
    /* 0040c789 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(0x00000001u);
    /* 0040c78f */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 0040c796 */
    f->r[4]+=4;return;
mi_0040c797:;
    /* 0040c797 */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 0040c79d */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 0040c7a3 */
    f->r[0]=(0x00ea60a4u+f->r[1]);
    /* 0040c7a9 */
    f->r[1]=MI_U32((0x00ea60a4u+f->r[1]));
    /* 0040c7af */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0040c7b0 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 0040c7b2 */
    f->r[2]=(0x0000000du+f->r[1]);
    /* 0040c7b5 */
    f->r[1]=MI_U32((0x00f53e6cu));
    /* 0040c7bb */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0040c7bd */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0040c7c4;
    /* 0040c7bf */
    a=f->r[1];b=0xfffffff3u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0040c7c2 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[1]);
mi_0040c7c4:;
    /* 0040c7c4 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 0040c7cb */
    f->r[4]+=4;return;
}

void restore_training_render_C_004cd680(struct restore_training_render_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004cd680 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd681 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd682 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd683 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd684 */
    f->r[7]=MI_U32((0x00000014u+f->r[4]));
    /* 004cd688 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 004cd68a */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004cd68e;
    /* 004cd68c */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
mi_004cd68e:;
    /* 004cd68e */
    f->r[1]=MI_U32((0x0000001cu+f->r[4]));
    /* 004cd692 */
    f->r[0]=MI_U32((0x00e38d9cu));
    /* 004cd697 */
    f->r[2]=(0x00000000u+f->r[7]+f->r[1]*1u);
    /* 004cd69a */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004cd69c */
    if(mi_signed_less(f->flags))goto mi_004cd6a2;
    /* 004cd69e */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004cd6a0 */
    f->r[1]=f->r[0];
mi_004cd6a2:;
    /* 004cd6a2 */
    f->r[6]=MI_U32((0x00000018u+f->r[4]));
    /* 004cd6a6 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004cd6a8 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004cd6ac;
    /* 004cd6aa */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
mi_004cd6ac:;
    /* 004cd6ac */
    f->r[5]=MI_U32((0x00000020u+f->r[4]));
    /* 004cd6b0 */
    f->r[0]=MI_U32((0x00e38d98u));
    /* 004cd6b5 */
    f->r[2]=(0x00000000u+f->r[6]+f->r[5]*1u);
    /* 004cd6b8 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004cd6ba */
    if(mi_signed_less(f->flags))goto mi_004cd6c0;
    /* 004cd6bc */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004cd6be */
    f->r[5]=f->r[0];
mi_004cd6c0:;
    /* 004cd6c0 */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 004cd6c4 */
    f->r[3]=(0xffffffffu+f->r[7]+f->r[1]*1u);
    /* 004cd6c8 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd6c9 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd6ca */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd6cb */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd6cc */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd6cd */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cd6d2u;f->boundary(0x004cd6cdu,0x004cd510u,f);
    /* 004cd6d2 */
    f->r[1]=MI_U32((0x00000038u+f->r[4]));
    /* 004cd6d6 */
    f->r[5]=(0xffffffffu+f->r[6]+f->r[5]*1u);
    /* 004cd6da */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd6db */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd6dc */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd6dd */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd6de */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd6df */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cd6e4u;f->boundary(0x004cd6dfu,0x004cd510u,f);
    /* 004cd6e4 */
    f->r[2]=MI_U32((0x0000004cu+f->r[4]));
    /* 004cd6e8 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd6e9 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd6ea */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd6eb */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd6ec */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd6ed */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cd6f2u;f->boundary(0x004cd6edu,0x004cd510u,f);
    /* 004cd6f2 */
    f->r[0]=MI_U32((0x00000060u+f->r[4]));
    /* 004cd6f6 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd6f7 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd6f8 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd6f9 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd6fa */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd6fb */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cd700u;f->boundary(0x004cd6fbu,0x004cd510u,f);
    /* 004cd700 */
    a=f->r[4];b=0x00000050u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004cd703 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004cd704 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004cd705 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004cd706 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004cd707 */
    f->r[4]+=4;return;
}

void restore_training_render_C_004d65b0(struct restore_training_render_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004d65b0 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d65b1 */
    f->r[0]=MI_U32((0x00e94d9cu));
    /* 004d65b6 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004d65b8 */
    if(!(f->flags&MI_ZF))goto mi_004d65bf;
    /* 004d65ba */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d65bfu;f->boundary(0x004d65bau,0x004d5d90u,f);
mi_004d65bf:;
    /* 004d65bf */
    f->r[0]=MI_U32((0x00000008u+f->r[4]));
    /* 004d65c3 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d65c4 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d65c5 */
    f->r[5]=MI_U32((0x00000024u+f->r[4]));
    /* 004d65c9 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d65ca */
    f->r[6]=MI_U32((0x00000024u+f->r[4]));
    /* 004d65ce */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004d65d0 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d65d1 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004d65d5 */
    f->r[7]=f->r[0];
    /* 004d65d7 */
    f->r[0]=MI_U32((0x0000001cu+f->r[4]));
    /* 004d65db */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 004d65df */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004d65e1 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004d65e5 */
    f->r[3]=f->r[0];
    /* 004d65e7 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004d65e9 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d65ea */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38ddau)))&0xffu)<<0);
    /* 004d65ef */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d65f0 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d65f1 */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 004d65f5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d65fau;f->boundary(0x004d65f5u,0x004d5d70u,f);
    /* 004d65fa */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d65fd */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d65fe */
    a=0x00000004u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d6600 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d6605u;f->boundary(0x004d6600u,0x004d5fe0u,f);
    /* 004d6605 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d6608 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004d660a */
    if((f->flags&MI_ZF))goto mi_004d6630;
    /* 004d660c */
    f->r[1]=MI_U32((0x00000008u+f->r[0]));
    /* 004d660f */
    f->r[2]=MI_U32((0x0000000cu+f->r[0]));
    /* 004d6612 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004d6616 */
    f->r[1]=MI_U32((0x00000010u+f->r[0]));
    /* 004d6619 */
    f->r[7]=MI_U32((0x00000018u+f->r[4]));
    /* 004d661d */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004d6621 */
    f->r[2]=MI_U32((0x00000014u+f->r[0]));
    /* 004d6624 */
    f->r[3]=MI_U32((0x00000028u+f->r[4]));
    /* 004d6628 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004d662c */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[2]);
mi_004d6630:;
    /* 004d6630 */
    f->r[0]=MI_U32((0x00e94d98u));
    /* 004d6635 */
    a=f->r[7];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004d6637 */
    a=f->r[3];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 004d6639 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 004d663c */
    if(!(f->flags&MI_ZF))goto mi_004d66d6;
    /* 004d6642 */
    f->r[0]=MI_U32((0x00e38e34u));
    /* 004d6647 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004d6649 */
    if(!mi_signed_less(f->flags))goto mi_004d66d6;
    /* 004d664f */
    f->r[5]=MI_U32((0x00000020u+f->r[4]));
    /* 004d6653 */
    a=f->r[5];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[5]=t;
    /* 004d6656 */
    f->r[1]=(0x00000000u+f->r[7]+f->r[5]*1u);
    /* 004d6659 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004d665b */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004d66d6;
    /* 004d665d */
    f->r[0]=MI_U32((0x00e38e30u));
    /* 004d6662 */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004d6664 */
    if(!mi_signed_less(f->flags))goto mi_004d66d6;
    /* 004d6666 */
    f->r[6]=MI_U32((0x00000024u+f->r[4]));
    /* 004d666a */
    a=f->r[6];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004d666d */
    f->r[2]=(0x00000000u+f->r[6]+f->r[3]*1u);
    /* 004d6670 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004d6672 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004d66d6;
    /* 004d6674 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d6679u;f->boundary(0x004d6674u,0x004d6050u,f);
    /* 004d6679 */
    f->r[0]=MI_U32((0x00000018u+f->r[4]));
    /* 004d667d */
    f->r[1]=MI_U32((0x00000028u+f->r[4]));
    /* 004d6681 */
    f->r[2]=MI_U32((0x00000020u+f->r[4]));
    /* 004d6685 */
    a=0x0000ffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d668a */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d668b */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d668c */
    MI_U32((0x00e94600u))=(uint32_t)(f->r[0]);
    /* 004d6691 */
    f->r[0]=MI_U32((0x00000030u+f->r[4]));
    /* 004d6695 */
    MI_U32((0x00e94604u))=(uint32_t)(f->r[1]);
    /* 004d669b */
    f->r[1]=MI_U32((0x0000001cu+f->r[4]));
    /* 004d669f */
    MI_U32((0x00e94608u))=(uint32_t)(f->r[2]);
    /* 004d66a5 */
    f->r[2]=MI_U32((0x00000028u+f->r[4]));
    /* 004d66a9 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d66aa */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d66ab */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d66ad */
    MI_U32((0x00e9460cu))=(uint32_t)(f->r[0]);
    /* 004d66b2 */
    MI_U32((0x00e945f8u))=(uint32_t)(f->r[1]);
    /* 004d66b8 */
    MI_U32((0x00e945fcu))=(uint32_t)(f->r[2]);
    /* 004d66be */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d66c3u;f->boundary(0x004d66beu,0x00536c30u,f);
    /* 004d66c3 */
    a=0x0000ffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d66c8 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d66c9 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d66ca */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d66cb */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d66cc */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d66ce */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d66d3u;f->boundary(0x004d66ceu,0x00536c30u,f);
    /* 004d66d3 */
    a=f->r[4];b=0x00000030u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004d66d6:;
    /* 004d66d6 */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 004d66da */
    f->r[1]=MI_U32((0x00000020u+f->r[4]));
    /* 004d66de */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d66df */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d66e0 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d66e1 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d66e2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d66e7u;f->boundary(0x004d66e2u,0x00536a30u,f);
    /* 004d66e7 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d66ea */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d66eb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d66ec */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d66ed */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d66ee */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004d66ef */
    f->r[4]+=4;return;
}

void restore_training_render_C_00536a30(struct restore_training_render_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00536a30 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 00536a33 */
    f->r[1]=MI_U32((0x00000018u+f->r[4]));
    /* 00536a37 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00536a38 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00536a39 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 00536a3b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00536a3d */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 00536a3f */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00536a41 */
    MI_U32((0x00000008u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00536a45 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00536b93;
    /* 00536a4b */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00536a4c */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
mi_00536a4d:;
    /* 00536a4d */
    f->r[1]=MI_U32((0x00000024u+f->r[4]));
    /* 00536a51 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 00536a53 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 00536a55 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00536a57 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00536b77;
    /* 00536a5d */
    f->r[2]=MI_U32((0x00000020u+f->r[4]));
    /* 00536a61 */
    f->r[1]=f->r[0];
    /* 00536a63 */
    a=f->r[1];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00536a66 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00536a68 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[1]);
mi_00536a6c:;
    /* 00536a6c */
    f->r[2]=MI_U32((0x0000001cu+f->r[4]));
    /* 00536a70 */
    f->r[1]=f->r[7];
    /* 00536a72 */
    a=f->r[1];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00536a75 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00536a77 */
    f->r[2]=MI_U32((0x00000014u+f->r[4]));
    /* 00536a7b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00536a7d */
    MI_U32((0x00d3b5e0u))=(uint32_t)(f->r[1]);
    /* 00536a83 */
    MI_U32((0x00d3b5dcu))=(uint32_t)(f->r[2]);
    /* 00536a89 */
    if(!(f->flags&MI_ZF))goto mi_00536ace;
    /* 00536a8b */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 00536a8d */
    if(!(f->flags&MI_ZF))goto mi_00536aa0;
    /* 00536a8f */
    f->r[0]=MI_U32((0x00f56844u));
    /* 00536a94 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 00536a96 */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[0]);
    /* 00536a9b */
    goto mi_00536b52;
mi_00536aa0:;
    /* 00536aa0 */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 00536aa4 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00536aa5 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00536aa7 */
    f->r[0]=MI_U32((0x00f56844u));
    /* 00536aac */
    if(mi_signed_less(f->flags))goto mi_00536abd;
    /* 00536aae */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00536ab1 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 00536ab3 */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[0]);
    /* 00536ab8 */
    goto mi_00536b52;
mi_00536abd:;
    /* 00536abd */
    f->r[0]=(0x00000001u+f->r[0]+f->r[6]*1u);
    /* 00536ac1 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 00536ac2 */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[0]);
    /* 00536ac7 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 00536ac9 */
    goto mi_00536b52;
mi_00536ace:;
    /* 00536ace */
    f->r[3]=MI_U32((0x00000028u+f->r[4]));
    /* 00536ad2 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 00536ad3 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 00536ad5 */
    if(mi_signed_less(f->flags))goto mi_00536b14;
    /* 00536ad7 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 00536ad9 */
    if(!(f->flags&MI_ZF))goto mi_00536aec;
    /* 00536adb */
    f->r[0]=MI_U32((0x00f56844u));
    /* 00536ae0 */
    a=f->r[0];b=0x0000002au;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00536ae3 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 00536ae5 */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[0]);
    /* 00536aea */
    goto mi_00536b52;
mi_00536aec:;
    /* 00536aec */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 00536af0 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00536af1 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00536af3 */
    f->r[0]=MI_U32((0x00f56844u));
    /* 00536af8 */
    if(mi_signed_less(f->flags))goto mi_00536b06;
    /* 00536afa */
    a=f->r[0];b=0x00000030u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00536afd */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 00536aff */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[0]);
    /* 00536b04 */
    goto mi_00536b52;
mi_00536b06:;
    /* 00536b06 */
    f->r[0]=(0x0000002bu+f->r[0]+f->r[6]*1u);
    /* 00536b0a */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 00536b0b */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[0]);
    /* 00536b10 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 00536b12 */
    goto mi_00536b52;
mi_00536b14:;
    /* 00536b14 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 00536b16 */
    if(!(f->flags&MI_ZF))goto mi_00536b28;
    /* 00536b18 */
    f->r[0]=MI_U32((0x00f56844u));
    /* 00536b1d */
    f->r[0]=(0x00000007u+f->r[0]+f->r[5]*1u);
    /* 00536b21 */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[0]);
    /* 00536b26 */
    goto mi_00536b4d;
mi_00536b28:;
    /* 00536b28 */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 00536b2c */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00536b2d */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00536b2f */
    f->r[0]=MI_U32((0x00f56844u));
    /* 00536b34 */
    if(mi_signed_less(f->flags))goto mi_00536b41;
    /* 00536b36 */
    f->r[0]=(0x0000000du+f->r[0]+f->r[5]*1u);
    /* 00536b3a */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[0]);
    /* 00536b3f */
    goto mi_00536b4d;
mi_00536b41:;
    /* 00536b41 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00536b43 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 00536b44 */
    f->r[0]=(0x00000008u+f->r[0]+f->r[5]*1u);
    /* 00536b48 */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[0]);
mi_00536b4d:;
    /* 00536b4d */
    f->r[3]=0x00000007u;
mi_00536b52:;
    /* 00536b52 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00536b53 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00536b54 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00536b55 */
    f->r[1]=0x00f55bb0u;
    /* 00536b5a */
    f->r[4]-=4;MI_U32(f->r[4])=0x00536b5fu;f->boundary(0x00536b5au,0x0053e590u,f);
    /* 00536b5f */
    a=f->r[6];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 00536b62 */
    if(mi_signed_less(f->flags))goto mi_00536b66;
    /* 00536b64 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
mi_00536b66:;
    /* 00536b66 */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 00536b6a */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 00536b6b */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00536b6d */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 00536b71 */
    if(mi_signed_less(f->flags))goto mi_00536a6c;
mi_00536b77:;
    /* 00536b77 */
    a=f->r[5];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 00536b79 */
    a=f->r[5];b=0x00000023u;t=mi_alu(f,a,b,32,1);
    /* 00536b7c */
    if(mi_signed_less(f->flags))goto mi_00536b80;
    /* 00536b7e */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
mi_00536b80:;
    /* 00536b80 */
    f->r[1]=MI_U32((0x00000028u+f->r[4]));
    /* 00536b84 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00536b85 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 00536b87 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00536b8b */
    if(mi_signed_less(f->flags))goto mi_00536a4d;
    /* 00536b91 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00536b92 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
mi_00536b93:;
    /* 00536b93 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00536b94 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00536b95 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00536b98 */
    f->r[4]+=4;return;
}

void restore_training_render_C_00559e10(struct restore_training_render_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00559e10 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559e11 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00559e16u;f->boundary(0x00559e11u,0x005596d0u,f);
    /* 00559e16 */
    f->r[7]=MI_U32((0x00f53f48u));
    /* 00559e1c */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 00559e22 */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 00559e28 */
    f->r[0]=0x00000005u;
    /* 00559e2d */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00559e2f */
    MI_U32((0x00f53f0cu))=(uint32_t)(f->r[1]);
    /* 00559e35 */
    a=f->r[0];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 00559e38 */
    MI_U32((0x00f53f08u))=(uint32_t)(f->r[2]);
    /* 00559e3e */
    MI_U32((0x00f53f04u))=(uint32_t)(0x00000028u);
    /* 00559e48 */
    MI_U32((0x00f53f00u))=(uint32_t)(0x0000001bu);
    /* 00559e52 */
    if(!mi_signed_less(f->flags))goto mi_00559fb1;
    /* 00559e58 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00559e5a */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00559e8d;
    /* 00559e5c */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559e5d */
    f->r[6]=(0x00000000u+f->r[0]+f->r[0]*1u);
    /* 00559e60 */
    f->r[7]=0x0000001bu;
    /* 00559e65 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559e67 */
    a=f->r[7];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 00559e69 */
    f->r[2]=(0x00000000u+f->r[2]+f->r[0]*8u);
    /* 00559e6c */
    a=f->r[7];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 00559e6f */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559e70 */
    f->r[7]=0x00000028u;
    /* 00559e75 */
    a=f->r[7];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 00559e77 */
    a=f->r[7];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 00559e7a */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00559e7d */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559e7e */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00559e80 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559e81 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559e82 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00559e87u;f->boundary(0x00559e82u,0x004cd680u,f);
    /* 00559e87 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00559e8a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00559e8b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00559e8c */
    f->r[4]+=4;return;
mi_00559e8d:;
    /* 00559e8d */
    a=0x0000001bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559e8f */
    a=0x00000028u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559e91 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559e92 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559e93 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00559e98u;f->boundary(0x00559e93u,0x00536820u,f);
    /* 00559e98 */
    f->r[0]=MI_U32((0x00f53f08u));
    /* 00559e9d */
    f->r[1]=MI_U32((0x00f53f0cu));
    /* 00559ea3 */
    f->r[2]=MI_U32((0x00f5681cu));
    /* 00559ea9 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00559eac */
    a=f->r[0];b=0x0000000au;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00559eaf */
    a=f->r[1];b=0x0000000au;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00559eb2 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559eb3 */
    a=f->r[2];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00559eb6 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559eb7 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559eb8 */
    f->r[1]=0x00f55bb0u;
    /* 00559ebd */
    f->r[4]-=4;MI_U32(f->r[4])=0x00559ec2u;f->boundary(0x00559ebdu,0x0053e590u,f);
    /* 00559ec2 */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 00559ec7 */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 00559ecd */
    f->r[2]=MI_U32((0x00f56fb4u));
    /* 00559ed3 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559ed4 */
    f->r[0]=MI_U32((0x00f53f08u));
    /* 00559ed9 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559eda */
    f->r[1]=MI_U32((0x00f53f0cu));
    /* 00559ee0 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559ee2 */
    a=f->r[0];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00559ee5 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559ee6 */
    a=f->r[1];b=0x0000003cu;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00559ee9 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559eea */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559eeb */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559eed */
    a=0x00000036u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559eef */
    f->r[4]-=4;MI_U32(f->r[4])=0x00559ef4u;f->boundary(0x00559eefu,0x004d62f0u,f);
    /* 00559ef4 */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 00559efa */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00559efd */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 00559f03 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00ea623cu+f->r[2])))&0xffu)<<0);
    /* 00559f09 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00559f0b */
    if(!(f->flags&MI_ZF))goto mi_00559f44;
    /* 00559f0d */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 00559f12 */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 00559f18 */
    f->r[2]=MI_U32((0x00f56f8cu));
    /* 00559f1e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559f1f */
    f->r[0]=MI_U32((0x00f53f08u));
    /* 00559f24 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559f25 */
    f->r[1]=MI_U32((0x00f53f0cu));
    /* 00559f2b */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559f2d */
    a=f->r[0];b=0x00000026u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00559f30 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559f31 */
    a=f->r[1];b=0x00000064u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00559f34 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559f35 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559f36 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559f38 */
    a=0x00000036u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559f3a */
    f->r[4]-=4;MI_U32(f->r[4])=0x00559f3fu;f->boundary(0x00559f3au,0x004d62f0u,f);
    /* 00559f3f */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00559f42 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00559f43 */
    f->r[4]+=4;return;
mi_00559f44:;
    /* 00559f44 */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 00559f4a */
    f->r[0]=MI_U32((0x00e38da8u));
    /* 00559f4f */
    f->r[1]=MI_U32((0x00f56f8cu));
    /* 00559f55 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559f56 */
    f->r[2]=MI_U32((0x00f53f08u));
    /* 00559f5c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559f5d */
    f->r[0]=MI_U32((0x00f53f0cu));
    /* 00559f62 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559f64 */
    a=f->r[2];b=0x00000026u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00559f67 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559f68 */
    a=f->r[0];b=0x000000afu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00559f6d */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559f6e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559f6f */
    a=0x00000016u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559f71 */
    a=0x00000036u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559f73 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00559f78u;f->boundary(0x00559f73u,0x004d62f0u,f);
    /* 00559f78 */
    f->r[1]=MI_U32((0x00e38da4u));
    /* 00559f7e */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 00559f84 */
    f->r[0]=MI_U32((0x00f56f8cu));
    /* 00559f89 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559f8a */
    f->r[1]=MI_U32((0x00f53f08u));
    /* 00559f90 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559f91 */
    f->r[2]=MI_U32((0x00f53f0cu));
    /* 00559f97 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559f99 */
    a=f->r[1];b=0x00000026u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00559f9c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559f9d */
    a=f->r[2];b=0x00000113u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00559fa3 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559fa4 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559fa5 */
    a=0x00000017u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559fa7 */
    a=0x00000036u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559fa9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00559faeu;f->boundary(0x00559fa9u,0x004d62f0u,f);
    /* 00559fae */
    a=f->r[4];b=0x00000040u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00559fb1:;
    /* 00559fb1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00559fb2 */
    f->r[4]+=4;return;
}

void restore_training_render_C_00559fc0(struct restore_training_render_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00559fc0 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559fc1 */
    f->r[3]=MI_U32((0x00000008u+f->r[4]));
    /* 00559fc5 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559fc6 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559fc7 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559fc8 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 00559fca */
    f->r[7]=0x00000001u;
    /* 00559fcf */
    f->r[6]=0x0077de6fu;
mi_00559fd4:;
    /* 00559fd4 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0xfffffffbu+f->r[6])))&0xffu)<<0);
    /* 00559fd7 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00559fd9 */
    if((f->flags&MI_ZF))goto mi_0055a031;
    /* 00559fdb */
    a=MI_U8((0x00000000u+f->r[6]));b=0x00000000u;t=mi_alu(f,a,b,8,1);
    /* 00559fde */
    if((f->flags&MI_ZF))goto mi_0055a031;
    /* 00559fe0 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 00559fe5 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 00559feb */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00ea40bcu+f->r[0])))&0xffffu)<<0);
    /* 00559ff2 */
    a=(f->r[0]&0xffffu);b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 00559ff6 */
    if((f->flags&MI_ZF))goto mi_00559ffe;
    /* 00559ff8 */
    a=(f->r[0]&0xffffu);b=0x00000003u;t=mi_alu(f,a,b,16,1);
    /* 00559ffc */
    if(!(f->flags&MI_ZF))goto mi_0055a031;
mi_00559ffe:;
    /* 00559ffe */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000038u+f->r[6])))&0xffu)<<0);
    /* 0055a001 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0055a003 */
    if((f->flags&MI_ZF))goto mi_0055a031;
    /* 0055a005 */
    a=0x00000013u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a007 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a008 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0055a00du;f->boundary(0x0055a008u,0x00447790u,f);
    /* 0055a00d */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0055a010 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0055a012 */
    if((f->flags&MI_ZF))goto mi_0055a031;
    /* 0055a014 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0055a016 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0xfffffffeu+f->r[6])))&0xffu)<<0);
    /* 0055a019 */
    f->r[2]=MI_U32((0x005d2a3cu+f->r[1]*8u));
    /* 0055a020 */
    f->r[1]=MI_U32((0xffffff68u+f->r[3]+f->r[2]*4u));
    /* 0055a027 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0055a028 */
    f->r[0]=(0xffffff68u+f->r[3]+f->r[2]*4u);
    /* 0055a02f */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[1]);
mi_0055a031:;
    /* 0055a031 */
    a=f->r[6];b=0x0000006au;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0055a034 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 0055a035 */
    a=f->r[6];b=0x0077f747u;t=mi_alu(f,a,b,32,1);
    /* 0055a03b */
    if(mi_signed_less(f->flags))goto mi_00559fd4;
    /* 0055a03d */
    f->r[0]=f->r[3];
    /* 0055a03f */
    f->r[1]=0x00000010u;
mi_0055a044:;
    /* 0055a044 */
    a=MI_U32((0x00000000u+f->r[0]));b=0x00000000u;t=mi_alu(f,a,b,32,1);
    /* 0055a047 */
    if((f->flags&MI_ZF))goto mi_0055a04a;
    /* 0055a049 */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
mi_0055a04a:;
    /* 0055a04a */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0055a04d */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0055a04e */
    if(!(f->flags&MI_ZF))goto mi_0055a044;
    /* 0055a050 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0055a051 */
    f->r[0]=f->r[5];
    /* 0055a053 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0055a054 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0055a055 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0055a056 */
    f->r[4]+=4;return;
}

void restore_training_render_C_0055a060(struct restore_training_render_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0055a060 */
    f->r[2]=MI_U32((0x00f53e6cu));
    /* 0055a066 */
    f->r[0]=(0xfffffff1u+f->r[2]);
    /* 0055a069 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0055a06b */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0055a0dd;
    /* 0055a06d */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 0055a073 */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 0055a079 */
    f->r[1]=MI_U32((0x00ea60a4u+f->r[1]));
    /* 0055a07f */
    a=f->r[1];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0055a082 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0055a088;
    /* 0055a084 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0055a086 */
    goto mi_0055a0a3;
mi_0055a088:;
    /* 0055a088 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a089 */
    f->r[6]=(0x0000000eu+f->r[1]);
    /* 0055a08c */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0055a08e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0055a08f */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0055a098;
    /* 0055a091 */
    f->r[0]=0x00000064u;
    /* 0055a096 */
    goto mi_0055a0a3;
mi_0055a098:;
    /* 0055a098 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0055a099 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a09a */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a09b */
    f->r[4]-=4;MI_U32(f->r[4])=0x0055a0a0u;f->boundary(0x0055a09bu,0x004cde60u,f);
    /* 0055a0a0 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0055a0a3:;
    /* 0055a0a3 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a0a4 */
    a=0x00000100u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a0a9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0055a0aeu;f->boundary(0x0055a0a9u,0x004cde40u,f);
    /* 0055a0ae */
    f->r[2]=MI_U32((0x00f53f08u));
    /* 0055a0b4 */
    f->r[1]=MI_U32((0x00f53f0cu));
    /* 0055a0ba */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0055a0bd */
    a=f->r[1];b=0x00000255u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0055a0c3 */
    f->r[0]=(0x0000004eu+f->r[2]+f->r[0]*1u);
    /* 0055a0c7 */
    f->r[2]=MI_U32((0x00f56494u));
    /* 0055a0cd */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a0ce */
    a=f->r[2];b=0x00000027u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0055a0d1 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a0d2 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a0d3 */
    f->r[1]=0x00f55bb0u;
    /* 0055a0d8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0055a0ddu;f->boundary(0x0055a0d8u,0x0053e590u,f);
mi_0055a0dd:;
    /* 0055a0dd */
    f->r[4]+=4;return;
}

static const uint8_t mi_rom_0055a0e0_0055acc4[]={0xf1,0xa3,0x55,0x00,0xe2,0xa3,0x55,0x00};
static const uint8_t mi_rom_0055a0e0_0055accc[]={0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x00};
static const uint8_t mi_rom_0055a0e0_0055acd8[]={0x67,0xa7,0x55,0x00,0x58,0xa7,0x55,0x00};
static const uint8_t mi_rom_0055a0e0_0055ace0[]={0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x00};
static const uint8_t mi_rom_0055a0e0_0055acec[]={0xc6,0xa7,0x55,0x00,0x8e,0xa7,0x55,0x00};
static const uint8_t mi_rom_0055a0e0_0055acf4[]={0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x00};
static const uint8_t mi_rom_0055a0e0_0055ad00[]={0x81,0xa8,0x55,0x00,0x72,0xa8,0x55,0x00};
static const uint8_t mi_rom_0055a0e0_0055ad08[]={0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x00};
static const uint8_t mi_rom_0055a0e0_0055ad14[]={0xd8,0xaa,0x55,0x00,0xdf,0xaa,0x55,0x00};
static const uint8_t mi_rom_0055a0e0_0055ad1c[]={0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x00};
static __inline uint32_t mi_read_0055a0e0(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x0055acc4u&&width<=8u&&(offset=address-0x0055acc4u)<=8u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0055a0e0_0055acc4[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0055acccu&&width<=12u&&(offset=address-0x0055acccu)<=12u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0055a0e0_0055accc[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0055acd8u&&width<=8u&&(offset=address-0x0055acd8u)<=8u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0055a0e0_0055acd8[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0055ace0u&&width<=12u&&(offset=address-0x0055ace0u)<=12u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0055a0e0_0055ace0[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0055acecu&&width<=8u&&(offset=address-0x0055acecu)<=8u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0055a0e0_0055acec[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0055acf4u&&width<=12u&&(offset=address-0x0055acf4u)<=12u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0055a0e0_0055acf4[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0055ad00u&&width<=8u&&(offset=address-0x0055ad00u)<=8u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0055a0e0_0055ad00[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0055ad08u&&width<=12u&&(offset=address-0x0055ad08u)<=12u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0055a0e0_0055ad08[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0055ad14u&&width<=8u&&(offset=address-0x0055ad14u)<=8u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0055a0e0_0055ad14[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0055ad1cu&&width<=12u&&(offset=address-0x0055ad1cu)<=12u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0055a0e0_0055ad1c[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}
void restore_training_render_C_0055a0e0(struct restore_training_render_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0055a0e0 */
    a=f->r[4];b=0x0000004cu;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 0055a0e3 */
    f->r[0]=mi_read_0055a0e0((0x00eaac28u),4u);
    /* 0055a0e8 */
    f->r[1]=mi_read_0055a0e0((0x00f53e6cu),4u);
    /* 0055a0ee */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0055a0f4 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a0f5 */
    f->r[3]=mi_read_0055a0e0((0x00ea60a4u+f->r[0]),4u);
    /* 0055a0fb */
    f->r[0]=(0x00ea60a4u+f->r[0]);
    /* 0055a101 */
    f->r[2]=f->r[1];
    /* 0055a103 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a104 */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0055a106 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 0055a108 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0055a109 */
    a=f->r[2];b=0x0000000fu;t=mi_alu(f,a,b,32,1);
    /* 0055a10c */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a10d */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 0055a111 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[5]);
    /* 0055a115 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 0055a119 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 0055a11d */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 0055a121 */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[5]);
    /* 0055a125 */
    MI_U32((0x00000030u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 0055a129 */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 0055a12d */
    MI_U32((0x00000038u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 0055a131 */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(f->r[5]);
    /* 0055a135 */
    MI_U32((0x00000040u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 0055a139 */
    MI_U32((0x00000044u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 0055a13d */
    MI_U32((0x00000048u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 0055a141 */
    MI_U32((0x0000004cu+f->r[4]))=(uint32_t)(f->r[5]);
    /* 0055a145 */
    MI_U32((0x00000050u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 0055a149 */
    MI_U32((0x00000054u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 0055a14d */
    if(!mi_signed_less(f->flags))goto mi_0055a154;
    /* 0055a14f */
    a=f->r[1];b=0xfffffff2u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0055a152 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[1]);
mi_0055a154:;
    /* 0055a154 */
    a=mi_read_0055a0e0((0x00000000u+f->r[0]),4u);b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0055a157 */
    if(!mi_signed_less(f->flags))goto mi_0055a15f;
    /* 0055a159 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(0x00000001u);
mi_0055a15f:;
    /* 0055a15f */
    f->r[6]=mi_read_0055a0e0((0x00000000u+f->r[0]),4u);
    /* 0055a161 */
    f->r[0]=mi_read_0055a0e0((0x00f53f48u),4u);
    /* 0055a166 */
    a=f->r[0];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 0055a169 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 0055a16d */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0055acba;
    /* 0055a173 */
    f->r[0]=mi_read_0055a0e0((0x00e38da4u),4u);
    /* 0055a178 */
    f->r[1]=mi_read_0055a0e0((0x00e38da8u),4u);
    /* 0055a17e */
    f->r[2]=mi_read_0055a0e0((0x00f53f08u),4u);
    /* 0055a184 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a185 */
    f->r[0]=mi_read_0055a0e0((0x00f53f0cu),4u);
    /* 0055a18a */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a18b */
    a=0x00000015u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a18d */
    a=f->r[2];b=0x00000034u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0055a190 */
    a=0x00000024u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a192 */
    a=f->r[0];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0055a195 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a196 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a197 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0055a19cu;f->boundary(0x0055a197u,0x004d65b0u,f);
    /* 0055a19c */
    f->r[0]=mi_read_0055a0e0((0x00f53e6cu),4u);
    /* 0055a1a1 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0055a1a4 */
    a=f->r[0];b=0x0000000fu;t=mi_alu(f,a,b,32,1);
    /* 0055a1a7 */
    if(mi_signed_less(f->flags))goto mi_0055a2b8;
    /* 0055a1ad */
    f->r[1]=mi_read_0055a0e0((0x00f53f08u),4u);
    /* 0055a1b3 */
    f->r[2]=mi_read_0055a0e0((0x00f53f0cu),4u);
    /* 0055a1b9 */
    a=0x0000000au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a1bb */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a1bd */
    a=f->r[1];b=0x00000034u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0055a1c0 */
    a=0x005c5b90u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a1c5 */
    a=f->r[2];b=0x0000023du;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0055a1cb */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a1cc */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a1cd */
    f->r[4]-=4;MI_U32(f->r[4])=0x0055a1d2u;f->boundary(0x0055a1cdu,0x00425e80u,f);
    /* 0055a1d2 */
    f->r[0]=mi_read_0055a0e0((0x00f53f08u),4u);
    /* 0055a1d7 */
    f->r[1]=mi_read_0055a0e0((0x00f53f0cu),4u);
    /* 0055a1dd */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a1df */
    a=f->r[0];b=0x00000034u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0055a1e2 */
    a=0x005c5b90u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a1e7 */
    a=f->r[1];b=0x0000023du;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0055a1ed */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a1ee */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a1ef */
    f->r[4]-=4;MI_U32(f->r[4])=0x0055a1f4u;f->boundary(0x0055a1efu,0x00425c40u,f);
    /* 0055a1f4 */
    f->r[2]=mi_read_0055a0e0((0x00f53f08u),4u);
    /* 0055a1fa */
    f->r[0]=mi_read_0055a0e0((0x00f53f0cu),4u);
    /* 0055a1ff */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a201 */
    a=f->r[2];b=0x00000034u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0055a204 */
    a=0x005c5b90u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a209 */
    a=f->r[0];b=0x0000023du;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0055a20e */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a20f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a210 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0055a215u;f->boundary(0x0055a210u,0x004256a0u,f);
    /* 0055a215 */
    f->r[1]=mi_read_0055a0e0((0x00f53f08u),4u);
    /* 0055a21b */
    f->r[2]=mi_read_0055a0e0((0x00f53f0cu),4u);
    /* 0055a221 */
    a=0x0000000au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a223 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a225 */
    a=f->r[1];b=0x0000016cu;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0055a22b */
    a=0x005c5b78u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a230 */
    a=f->r[2];b=0x00000255u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0055a236 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a237 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a238 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0055a23du;f->boundary(0x0055a238u,0x00425e80u,f);
    /* 0055a23d */
    f->r[0]=mi_read_0055a0e0((0x00f53f08u),4u);
    /* 0055a242 */
    f->r[1]=mi_read_0055a0e0((0x00f53f0cu),4u);
    /* 0055a248 */
    a=f->r[4];b=0x00000048u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0055a24b */
    a=f->r[0];b=0x0000016cu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0055a250 */
    a=f->r[1];b=0x00000255u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0055a256 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a258 */
    a=0x005c5b78u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a25d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a25e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a25f */
    f->r[4]-=4;MI_U32(f->r[4])=0x0055a264u;f->boundary(0x0055a25fu,0x00425c40u,f);
    /* 0055a264 */
    f->r[2]=mi_read_0055a0e0((0x00f53f08u),4u);
    /* 0055a26a */
    f->r[0]=mi_read_0055a0e0((0x00f53f0cu),4u);
    /* 0055a26f */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a271 */
    a=f->r[2];b=0x0000016cu;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0055a277 */
    a=0x005c5b78u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a27c */
    a=f->r[0];b=0x00000255u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0055a281 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a282 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a283 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0055a288u;f->boundary(0x0055a283u,0x004256a0u,f);
    /* 0055a288 */
    f->r[1]=mi_read_0055a0e0((0x00e38da4u),4u);
    /* 0055a28e */
    f->r[2]=mi_read_0055a0e0((0x00e38da8u),4u);
    /* 0055a294 */
    f->r[0]=mi_read_0055a0e0((0x00f53f08u),4u);
    /* 0055a299 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a29a */
    f->r[1]=mi_read_0055a0e0((0x00f53f0cu),4u);
    /* 0055a2a0 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a2a1 */
    a=0x00000012u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a2a3 */
    a=f->r[0];b=0x0000004cu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0055a2a6 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a2a8 */
    a=f->r[1];b=0x00000251u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0055a2ae */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a2af */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a2b0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0055a2b5u;f->boundary(0x0055a2b0u,0x004d65b0u,f);
    /* 0055a2b5 */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0055a2b8:;
    /* 0055a2b8 */
    f->r[2]=(0x00f53e6cu+f->r[6]*4u);
    /* 0055a2bf */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a2c0 */
    MI_U16((0x00603322u))=(uint16_t)((f->r[5]&0xffffu));
    /* 0055a2c7 */
    MI_U16((0x00603326u))=(uint16_t)((f->r[5]&0xffffu));
    /* 0055a2ce */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 0055a2d2 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 0055a2d4 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0055a2d8 */
    goto mi_0055a2dc;
mi_0055a2da:;
    /* 0055a2da */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
mi_0055a2dc:;
    /* 0055a2dc */
    f->r[0]=mi_read_0055a0e0((0x00000014u+f->r[4]),4u);
    /* 0055a2e0 */
    f->r[1]=mi_read_0055a0e0((0x00f53f08u),4u);
    /* 0055a2e6 */
    f->r[6]=mi_read_0055a0e0((0x00000000u+f->r[0]),4u);
    /* 0055a2e8 */
    f->r[0]=mi_read_0055a0e0((0x00f53f0cu),4u);
    /* 0055a2ed */
    f->r[2]=(0x00000039u+f->r[7]+f->r[1]*1u);
    /* 0055a2f1 */
    f->r[1]=mi_read_0055a0e0((0x00f571fcu),4u);
    /* 0055a2f7 */
    a=f->r[0];b=0x00000016u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0055a2fa */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a2fb */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0055a2fd */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a2fe */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a2ff */
    f->r[1]=0x00f55bb0u;
    /* 0055a304 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0055a309u;f->boundary(0x0055a304u,0x0053e590u,f);
    /* 0055a309 */
    f->r[2]=mi_read_0055a0e0((0x007056c8u),4u);
    /* 0055a30f */
    f->r[0]=mi_read_0055a0e0((0x00000010u+f->r[4]),4u);
    /* 0055a313 */
    a=f->r[2];b=0xfffffffcu;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0055a316 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0055a318 */
    if(!(f->flags&MI_ZF))goto mi_0055a34e;
    /* 0055a31a */
    f->r[0]=mi_read_0055a0e0((0x00e38da4u),4u);
    /* 0055a31f */
    f->r[1]=mi_read_0055a0e0((0x00e38da8u),4u);
    /* 0055a325 */
    f->r[2]=mi_read_0055a0e0((0x00f53f08u),4u);
    /* 0055a32b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a32c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a32d */
    f->r[1]=mi_read_0055a0e0((0x00f53f0cu),4u);
    /* 0055a333 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a335 */
    a=0x00000018u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a337 */
    f->r[0]=(0x00000036u+f->r[7]+f->r[2]*1u);
    /* 0055a33b */
    a=0x0000023au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a340 */
    a=f->r[1];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0055a343 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a344 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a345 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a346 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0055a34bu;f->boundary(0x0055a346u,0x004d63d0u,f);
    /* 0055a34b */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0055a34e:;
    /* 0055a34e */
    f->r[2]=mi_read_0055a0e0((0x00eaac28u),4u);
    /* 0055a354 */
    f->r[0]=mi_read_0055a0e0((0x00e38da4u),4u);
    /* 0055a359 */
    a=f->r[2];b=0x000024ecu;t=mi_imul(f,a,b);f->r[2]=t;
    /* 0055a35f */
    f->r[1]=mi_read_0055a0e0((0x00e38da8u),4u);
    /* 0055a365 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0055a367 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a368 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a369 */
    a=mi_read_0055a0e0((0x00ea4126u+f->r[2]*2u),2u);b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 0055a372 */
    f->r[2]=mi_read_0055a0e0((0x00f56fa4u),4u);
    /* 0055a378 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a379 */
    if((f->flags&MI_ZF))goto mi_0055a381;
    /* 0055a37b */
    f->r[2]=mi_read_0055a0e0((0x00f56f9cu),4u);
mi_0055a381:;
    /* 0055a381 */
    f->r[0]=mi_read_0055a0e0((0x00f53f08u),4u);
    /* 0055a386 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a387 */
    f->r[2]=mi_read_0055a0e0((0x00f53f0cu),4u);
    /* 0055a38d */
    f->r[1]=(0x0000003du+f->r[7]+f->r[0]*1u);
    /* 0055a391 */
    a=f->r[2];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0055a394 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a395 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a396 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a397 */
    a=0x00000017u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a399 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0055a39eu;f->boundary(0x0055a399u,0x004d62f0u,f);
    /* 0055a39e */
    f->r[0]=mi_read_0055a0e0((0x00eaac28u),4u);
    /* 0055a3a3 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0055a3a6 */
    f->r[1]=f->r[0];
    /* 0055a3a8 */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 0055a3ae */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read_0055a0e0((0x00ea623cu+f->r[1]),1u))&0xffu)<<0);
    /* 0055a3b4 */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0055a3b6 */
    if(!(f->flags&MI_ZF))goto mi_0055aa55;
    /* 0055a3bc */
    a=f->r[0];b=0x00001276u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0055a3c2 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0055a3c4 */
    f->r[3]=(0xffffffe8u+f->r[6]);
    /* 0055a3c7 */
    a=f->r[3];b=0x0000000bu;t=mi_alu(f,a,b,32,1);
    /* 0055a3ca */
    f->r[2]=mi_read_0055a0e0((0x00ea6160u+f->r[0]*4u),4u);
    /* 0055a3d1 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0055a3e2;
    /* 0055a3d3 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0055a3d5 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read_0055a0e0((0x0055acccu+f->r[3]),1u))&0xffu)<<0);
    /* 0055a3db */
    switch(mi_read_0055a0e0((0x0055acc4u+f->r[0]*4u),4u)){case 0x0055a3e2u:goto mi_0055a3e2;case 0x0055a3f1u:goto mi_0055a3f1;default:abort();}
mi_0055a3e2:;
    /* 0055a3e2 */
    a=f->r[6];b=0x0000001cu;t=mi_alu(f,a,b,32,1);
    /* 0055a3e5 */
    if((f->flags&MI_ZF))goto mi_0055a3f1;
    /* 0055a3e7 */
    a=f->r[6];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 0055a3ea */
    if((f->flags&MI_ZF))goto mi_0055a3f1;
    /* 0055a3ec */
    a=f->r[6];b=0x00000024u;t=mi_alu(f,a,b,32,1);
    /* 0055a3ef */
    if(!(f->flags&MI_ZF))goto mi_0055a402;
mi_0055a3f1:;
    /* 0055a3f1 */
    f->r[0]=0x51eb851fu;
    /* 0055a3f6 */
    a=f->r[0];b=f->r[2];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 0055a3f8 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 0055a3fb */
    f->r[1]=f->r[2];
    /* 0055a3fd */
    a=f->r[1];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[1]=t;
    /* 0055a400 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
mi_0055a402:;
    /* 0055a402 */
    f->r[0]=mi_read_0055a0e0((0x00f56f9cu),4u);
    /* 0055a407 */
    f->r[1]=mi_read_0055a0e0((0x00f53f08u),4u);
    /* 0055a40d */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a40e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a40f */
    f->r[0]=(0x0000003du+f->r[7]+f->r[1]*1u);
    /* 0055a413 */
    f->r[1]=mi_read_0055a0e0((0x00f53f0cu),4u);
    /* 0055a419 */
    a=0x00000064u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a41b */
    a=f->r[1];b=0x0000008cu;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0055a421 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a422 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a423 */
    a=0x0057c048u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a428 */
    a=0x00000040u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a42a */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a42b */
    f->r[4]-=4;MI_U32(f->r[4])=0x0055a430u;f->boundary(0x0055a42bu,0x004cce90u,f);
    /* 0055a430 */
    f->r[0]=mi_read_0055a0e0((0x00eaac28u),4u);
    /* 0055a435 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0055a438 */
    f->r[2]=f->r[0];
    /* 0055a43a */
    a=f->r[2];b=0x00001276u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 0055a440 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0055a442 */
    a=mi_read_0055a0e0((0x00ea42f8u+f->r[2]*4u),4u);b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 0055a449 */
    if((f->flags&MI_ZF))goto mi_0055a488;
    /* 0055a44b */
    f->r[0]=mi_read_0055a0e0((0x00e38da4u),4u);
    /* 0055a450 */
    f->r[1]=mi_read_0055a0e0((0x00e38da8u),4u);
    /* 0055a456 */
    f->r[2]=mi_read_0055a0e0((0x00f56f9cu),4u);
    /* 0055a45c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a45d */
    f->r[0]=mi_read_0055a0e0((0x00f53f08u),4u);
    /* 0055a462 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a463 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a464 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a465 */
    f->r[2]=mi_read_0055a0e0((0x00f53f0cu),4u);
    /* 0055a46b */
    f->r[1]=(0x0000003du+f->r[7]+f->r[0]*1u);
    /* 0055a46f */
    a=f->r[2];b=0x000000e8u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0055a475 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a476 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a477 */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a479 */
    a=0x00000036u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a47b */
    f->r[4]-=4;MI_U32(f->r[4])=0x0055a480u;f->boundary(0x0055a47bu,0x004d62f0u,f);
    /* 0055a480 */
    f->r[0]=mi_read_0055a0e0((0x00eaac28u),4u);
    /* 0055a485 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0055a488:;
    /* 0055a488 */
    a=f->r[0];b=0x000024ecu;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0055a48e */
    f->r[5]=mi_read_0055a0e0((0x00f56f9cu),4u);
    /* 0055a494 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0055a496 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0055a498 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read_0055a0e0((0x00ea4096u+f->r[0]*2u),2u))&0xffffu)<<0);
    /* 0055a4a0 */
    a=(f->r[0]&0xffffu);b=(f->r[1]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 0055a4a3 */
    if(!(f->flags&MI_ZF))goto mi_0055a572;
    /* 0055a4a9 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a4aa */
    f->r[4]-=4;MI_U32(f->r[4])=0x0055a4afu;f->boundary(0x0055a4aau,0x00443950u,f);
    /* 0055a4af */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0055a4b2 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0055a4b4 */
    if((f->flags&MI_ZF))goto mi_0055a4ca;
    /* 0055a4b6 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a4b7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0055a4bcu;f->boundary(0x0055a4b7u,0x00443b20u,f);
    /* 0055a4bc */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0055a4bf */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0055a4c1 */
    if((f->flags&MI_ZF))goto mi_0055a4ca;
    /* 0055a4c3 */
    f->r[0]=0x00000021u;
    /* 0055a4c8 */
    goto mi_0055a543;
mi_0055a4ca:;
    /* 0055a4ca */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a4cb */
    f->r[4]-=4;MI_U32(f->r[4])=0x0055a4d0u;f->boundary(0x0055a4cbu,0x00443950u,f);
    /* 0055a4d0 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0055a4d3 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0055a4d5 */
    if((f->flags&MI_ZF))goto mi_0055a4de;
    /* 0055a4d7 */
    f->r[0]=0x0000001fu;
    /* 0055a4dc */
    goto mi_0055a543;
mi_0055a4de:;
    /* 0055a4de */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a4df */
    f->r[4]-=4;MI_U32(f->r[4])=0x0055a4e4u;f->boundary(0x0055a4dfu,0x00443b20u,f);
    /* 0055a4e4 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0055a4e7 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0055a4e9 */
    if((f->flags&MI_ZF))goto mi_0055a4f2;
    /* 0055a4eb */
    f->r[0]=0x00000020u;
    /* 0055a4f0 */
    goto mi_0055a543;
mi_0055a4f2:;
    /* 0055a4f2 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a4f3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0055a4f8u;f->boundary(0x0055a4f3u,0x00443a20u,f);
    /* 0055a4f8 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0055a4fb */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0055a4fd */
    if((f->flags&MI_ZF))goto mi_0055a513;
    /* 0055a4ff */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a500 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0055a505u;f->boundary(0x0055a500u,0x004439c0u,f);
    /* 0055a505 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0055a508 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0055a50a */
    if((f->flags&MI_ZF))goto mi_0055a513;
    /* 0055a50c */
    f->r[0]=0x00000024u;
    /* 0055a511 */
    goto mi_0055a53d;
mi_0055a513:;
    /* 0055a513 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a514 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0055a519u;f->boundary(0x0055a514u,0x004439c0u,f);
    /* 0055a519 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0055a51c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0055a51e */
    if((f->flags&MI_ZF))goto mi_0055a527;
    /* 0055a520 */
    f->r[0]=0x00000022u;
    /* 0055a525 */
    goto mi_0055a53d;
mi_0055a527:;
    /* 0055a527 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a528 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0055a52du;f->boundary(0x0055a528u,0x00443a20u,f);
    /* 0055a52d */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0055a530 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0055a532 */
    if((f->flags&MI_ZF))goto mi_0055ab2f;
    /* 0055a538 */
    f->r[0]=0x00000023u;
mi_0055a53d:;
    /* 0055a53d */
    f->r[5]=mi_read_0055a0e0((0x00f56f94u),4u);
mi_0055a543:;
    /* 0055a543 */
    f->r[1]=mi_read_0055a0e0((0x00e38da4u),4u);
    /* 0055a549 */
    f->r[2]=mi_read_0055a0e0((0x00e38da8u),4u);
    /* 0055a54f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a550 */
    f->r[1]=mi_read_0055a0e0((0x00f53f08u),4u);
    /* 0055a556 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a557 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a559 */
    f->r[2]=(0x0000003du+f->r[7]+f->r[1]*1u);
    /* 0055a55d */
    f->r[1]=mi_read_0055a0e0((0x00f53f0cu),4u);
    /* 0055a563 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a564 */
    a=f->r[1];b=0x00000144u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0055a56a */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a56b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a56c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a56d */
    goto mi_0055ab25;
mi_0055a572:;
    /* 0055a572 */
    a=(f->r[0]&0xffffu);b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 0055a576 */
    if(!(f->flags&MI_ZF))goto mi_0055a7fe;
    /* 0055a57c */
    a=f->r[6];b=0x00000013u;t=mi_alu(f,a,b,32,1);
    /* 0055a57f */
    MI_U32((0x00e20778u))=(uint32_t)(f->r[1]);
    /* 0055a585 */
    if(!(f->flags&MI_ZF))goto mi_0055a6f4;
    /* 0055a58b */
    f->r[2]=(0x0000001cu+f->r[4]);
    /* 0055a58f */
    MI_U32((0x00e20778u))=(uint32_t)(f->r[1]);
    /* 0055a595 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a596 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0055a59bu;f->boundary(0x0055a596u,0x00559fc0u,f);
    /* 0055a59b */
    f->r[1]=mi_read_0055a0e0((0x00e38da8u),4u);
    /* 0055a5a1 */
    f->r[2]=mi_read_0055a0e0((0x00f56f9cu),4u);
    /* 0055a5a7 */
    f->r[6]=f->r[0];
    /* 0055a5a9 */
    f->r[0]=mi_read_0055a0e0((0x00e38da4u),4u);
    /* 0055a5ae */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a5af */
    f->r[0]=mi_read_0055a0e0((0x00f53f08u),4u);
    /* 0055a5b4 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a5b5 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a5b7 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a5b8 */
    f->r[2]=mi_read_0055a0e0((0x00f53f0cu),4u);
    /* 0055a5be */
    f->r[1]=(0x0000003du+f->r[7]+f->r[0]*1u);
    /* 0055a5c2 */
    a=f->r[2];b=0x00000144u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0055a5c8 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a5c9 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a5ca */
    a=0x00000005u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a5cc */
    a=0x00000036u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a5ce */
    f->r[4]-=4;MI_U32(f->r[4])=0x0055a5d3u;f->boundary(0x0055a5ceu,0x004d62f0u,f);
    /* 0055a5d3 */
    f->r[0]=mi_read_0055a0e0((0x00eaac28u),4u);
    /* 0055a5d8 */
    f->r[1]=mi_read_0055a0e0((0x00e38da4u),4u);
    /* 0055a5de */
    f->r[2]=mi_read_0055a0e0((0x00e38da8u),4u);
    /* 0055a5e4 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0055a5ea */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read_0055a0e0((0x00ea4104u+f->r[0]),2u);
    /* 0055a5f1 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a5f2 */
    f->r[1]=mi_read_0055a0e0((0x00f56f9cu),4u);
    /* 0055a5f8 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a5f9 */
    f->r[2]=mi_read_0055a0e0((0x00f53f08u),4u);
    /* 0055a5ff */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a601 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a602 */
    f->r[1]=(0x0000003du+f->r[7]+f->r[2]*1u);
    /* 0055a606 */
    f->r[2]=mi_read_0055a0e0((0x00f53f0cu),4u);
    /* 0055a60c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a60d */
    f->r[1]=mi_read_0055a0e0((0x00e20778u),4u);
    /* 0055a613 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 0055a616 */
    f->r[2]=(0x00000144u+f->r[1]+f->r[2]*1u);
    /* 0055a61d */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 0055a620 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a621 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0055a624 */
    a=0x0057c048u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a629 */
    a=0x00000040u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a62b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a62c */
    f->r[4]-=4;MI_U32(f->r[4])=0x0055a631u;f->boundary(0x0055a62cu,0x004d6860u,f);
    /* 0055a631 */
    a=f->r[4];b=0x00000048u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0055a634 */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0055a637 */
    if(!(f->flags&MI_ZF))goto mi_0055a693;
    /* 0055a639 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0055a63b */
    f->r[1]=(0x0000001cu+f->r[4]);
mi_0055a63f:;
    /* 0055a63f */
    a=mi_read_0055a0e0((0x00000000u+f->r[1]),4u);b=0x00000000u;t=mi_alu(f,a,b,32,1);
    /* 0055a642 */
    if(!(f->flags&MI_ZF))goto mi_0055a652;
    /* 0055a644 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0055a645 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0055a648 */
    a=f->r[0];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 0055a64b */
    if(mi_signed_less(f->flags))goto mi_0055a63f;
    /* 0055a64d */
    goto mi_0055ab2f;
mi_0055a652:;
    /* 0055a652 */
    f->r[1]=mi_read_0055a0e0((0x00e38da4u),4u);
    /* 0055a658 */
    f->r[2]=mi_read_0055a0e0((0x00e38da8u),4u);
    /* 0055a65e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a65f */
    f->r[1]=mi_read_0055a0e0((0x00f56f9cu),4u);
    /* 0055a665 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a666 */
    f->r[2]=mi_read_0055a0e0((0x00f53f08u),4u);
    /* 0055a66c */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a66e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a66f */
    f->r[1]=(0x0000003du+f->r[7]+f->r[2]*1u);
    /* 0055a673 */
    f->r[2]=mi_read_0055a0e0((0x00f53f0cu),4u);
    /* 0055a679 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a67a */
    f->r[1]=mi_read_0055a0e0((0x00e20778u),4u);
    /* 0055a680 */
    a=f->r[0];b=0x00000027u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0055a683 */
    f->r[2]=(0x00000144u+f->r[1]+f->r[2]*1u);
    /* 0055a68a */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a68b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a68c */
    a=0x00000017u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a68e */
    goto mi_0055ab27;
mi_0055a693:;
    /* 0055a693 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 0055a695 */
    if((f->flags&MI_ZF))goto mi_0055ab2f;
    /* 0055a69b */
    f->r[0]=mi_read_0055a0e0((0x00000010u+f->r[4]),4u);
    /* 0055a69f */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 0055a6a2 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[1]*2u);
    /* 0055a6a5 */
    f->r[1]=mi_read_0055a0e0((0x00e38da8u),4u);
    /* 0055a6ab */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0055a6ad */
    f->r[2]=(0x00000038u+f->r[0]);
    /* 0055a6b0 */
    a=f->r[0];b=0x0000004cu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0055a6b3 */
    MI_U16((0x00603326u))=(uint16_t)((f->r[0]&0xffffu));
    /* 0055a6b9 */
    f->r[0]=mi_read_0055a0e0((0x00e38da4u),4u);
    /* 0055a6be */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a6bf */
    f->r[0]=mi_read_0055a0e0((0x00f53f08u),4u);
    /* 0055a6c4 */
    MI_U16((0x00603322u))=(uint16_t)((f->r[2]&0xffffu));
    /* 0055a6cb */
    f->r[2]=mi_read_0055a0e0((0x00f56f9cu),4u);
    /* 0055a6d1 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a6d2 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a6d4 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a6d5 */
    f->r[2]=mi_read_0055a0e0((0x00f53f0cu),4u);
    /* 0055a6db */
    f->r[1]=(0x0000003du+f->r[7]+f->r[0]*1u);
    /* 0055a6df */
    f->r[0]=mi_read_0055a0e0((0x00e20778u),4u);
    /* 0055a6e4 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a6e5 */
    f->r[1]=(0x00000144u+f->r[0]+f->r[2]*1u);
    /* 0055a6ec */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a6ed */
    a=0x0000002du;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a6ef */
    goto mi_0055ab25;
mi_0055a6f4:;
    /* 0055a6f4 */
    f->r[2]=mi_read_0055a0e0((0x00e38da4u),4u);
    /* 0055a6fa */
    f->r[0]=mi_read_0055a0e0((0x00e38da8u),4u);
    /* 0055a6ff */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a700 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a701 */
    f->r[0]=mi_read_0055a0e0((0x00f53f0cu),4u);
    /* 0055a706 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a707 */
    f->r[1]=mi_read_0055a0e0((0x00f53f08u),4u);
    /* 0055a70d */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a70e */
    a=f->r[0];b=0x00000144u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0055a713 */
    f->r[2]=(0x0000003du+f->r[7]+f->r[1]*1u);
    /* 0055a717 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a718 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a719 */
    a=0x00000005u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a71b */
    a=0x00000036u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a71d */
    f->r[4]-=4;MI_U32(f->r[4])=0x0055a722u;f->boundary(0x0055a71du,0x004d62f0u,f);
    /* 0055a722 */
    f->r[1]=mi_read_0055a0e0((0x00eaac28u),4u);
    /* 0055a728 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0055a72b */
    a=f->r[1];b=0x000024ecu;t=mi_imul(f,a,b);f->r[1]=t;
    /* 0055a731 */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0055a733 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read_0055a0e0((0x00ea40deu+f->r[1]*2u),2u);
    /* 0055a73b */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 0055a73e */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 0055a741 */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0055a744 */
    a=f->r[3];b=0x0000000bu;t=mi_alu(f,a,b,32,1);
    /* 0055a747 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0055a758;
    /* 0055a749 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0055a74b */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read_0055a0e0((0x0055ace0u+f->r[3]),1u))&0xffu)<<0);
    /* 0055a751 */
    switch(mi_read_0055a0e0((0x0055acd8u+f->r[2]*4u),4u)){case 0x0055a758u:goto mi_0055a758;case 0x0055a767u:goto mi_0055a767;default:abort();}
mi_0055a758:;
    /* 0055a758 */
    a=f->r[6];b=0x0000001cu;t=mi_alu(f,a,b,32,1);
    /* 0055a75b */
    if((f->flags&MI_ZF))goto mi_0055a767;
    /* 0055a75d */
    a=f->r[6];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 0055a760 */
    if((f->flags&MI_ZF))goto mi_0055a767;
    /* 0055a762 */
    a=f->r[6];b=0x00000024u;t=mi_alu(f,a,b,32,1);
    /* 0055a765 */
    if(!(f->flags&MI_ZF))goto mi_0055a77a;
mi_0055a767:;
    /* 0055a767 */
    f->r[0]=0x51eb851fu;
    /* 0055a76c */
    a=f->r[0];b=f->r[1];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 0055a76e */
    f->r[1]=f->r[2];
    /* 0055a770 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,1);f->r[1]=t;
    /* 0055a773 */
    f->r[0]=f->r[1];
    /* 0055a775 */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 0055a778 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
mi_0055a77a:;
    /* 0055a77a */
    a=f->r[3];b=0x0000000bu;t=mi_alu(f,a,b,32,1);
    /* 0055a77d */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0055a78e;
    /* 0055a77f */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0055a781 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read_0055a0e0((0x0055acf4u+f->r[3]),1u))&0xffu)<<0);
    /* 0055a787 */
    switch(mi_read_0055a0e0((0x0055acecu+f->r[2]*4u),4u)){case 0x0055a78eu:goto mi_0055a78e;case 0x0055a7c6u:goto mi_0055a7c6;default:abort();}
mi_0055a78e:;
    /* 0055a78e */
    f->r[0]=mi_read_0055a0e0((0x00e38da4u),4u);
    /* 0055a793 */
    f->r[2]=mi_read_0055a0e0((0x00e38da8u),4u);
    /* 0055a799 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a79a */
    f->r[0]=mi_read_0055a0e0((0x00f56f9cu),4u);
    /* 0055a79f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a7a0 */
    f->r[2]=mi_read_0055a0e0((0x00f53f08u),4u);
    /* 0055a7a6 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a7a8 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a7a9 */
    f->r[0]=(0x0000003du+f->r[7]+f->r[2]*1u);
    /* 0055a7ad */
    f->r[2]=mi_read_0055a0e0((0x00f53f0cu),4u);
    /* 0055a7b3 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a7b4 */
    f->r[0]=mi_read_0055a0e0((0x00e20778u),4u);
    /* 0055a7b9 */
    f->r[2]=(0x00000144u+f->r[0]+f->r[2]*1u);
    /* 0055a7c0 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a7c1 */
    goto mi_0055a8c7;
mi_0055a7c6:;
    /* 0055a7c6 */
    f->r[0]=mi_read_0055a0e0((0x00f56f9cu),4u);
    /* 0055a7cb */
    f->r[2]=mi_read_0055a0e0((0x00f53f08u),4u);
    /* 0055a7d1 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a7d3 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a7d4 */
    f->r[0]=(0x0000003du+f->r[7]+f->r[2]*1u);
    /* 0055a7d8 */
    f->r[2]=mi_read_0055a0e0((0x00f53f0cu),4u);
    /* 0055a7de */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a7df */
    f->r[0]=mi_read_0055a0e0((0x00e20778u),4u);
    /* 0055a7e4 */
    f->r[2]=(0x00000144u+f->r[0]+f->r[2]*1u);
    /* 0055a7eb */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a7ec */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a7ed */
    a=0x00000038u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a7ef */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a7f1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0055a7f6u;f->boundary(0x0055a7f1u,0x004ccc30u,f);
    /* 0055a7f6 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0055a7f9 */
    goto mi_0055ab2f;
mi_0055a7fe:;
    /* 0055a7fe */
    a=(f->r[0]&0xffffu);b=0x00000002u;t=mi_alu(f,a,b,16,1);
    /* 0055a802 */
    if(!(f->flags&MI_ZF))goto mi_0055a8dc;
    /* 0055a808 */
    f->r[0]=mi_read_0055a0e0((0x00e38da4u),4u);
    /* 0055a80d */
    f->r[2]=mi_read_0055a0e0((0x00e38da8u),4u);
    /* 0055a813 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a814 */
    f->r[0]=mi_read_0055a0e0((0x00f53f08u),4u);
    /* 0055a819 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a81a */
    f->r[2]=mi_read_0055a0e0((0x00f53f0cu),4u);
    /* 0055a820 */
    MI_U32((0x00e20778u))=(uint32_t)(f->r[1]);
    /* 0055a826 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a827 */
    f->r[1]=(0x0000003du+f->r[7]+f->r[0]*1u);
    /* 0055a82b */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a82c */
    a=f->r[2];b=0x00000144u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0055a832 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a833 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a834 */
    a=0x00000006u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a836 */
    a=0x00000036u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a838 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0055a83du;f->boundary(0x0055a838u,0x004d62f0u,f);
    /* 0055a83d */
    f->r[0]=mi_read_0055a0e0((0x00eaac28u),4u);
    /* 0055a842 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0055a845 */
    a=f->r[0];b=0x000024ecu;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0055a84b */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0055a84d */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read_0055a0e0((0x00ea40deu+f->r[0]*2u),2u);
    /* 0055a855 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 0055a858 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 0055a85b */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0055a85e */
    a=f->r[3];b=0x0000000bu;t=mi_alu(f,a,b,32,1);
    /* 0055a861 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0055a872;
    /* 0055a863 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0055a865 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read_0055a0e0((0x0055ad08u+f->r[3]),1u))&0xffu)<<0);
    /* 0055a86b */
    switch(mi_read_0055a0e0((0x0055ad00u+f->r[2]*4u),4u)){case 0x0055a872u:goto mi_0055a872;case 0x0055a881u:goto mi_0055a881;default:abort();}
mi_0055a872:;
    /* 0055a872 */
    a=f->r[6];b=0x0000001cu;t=mi_alu(f,a,b,32,1);
    /* 0055a875 */
    if((f->flags&MI_ZF))goto mi_0055a881;
    /* 0055a877 */
    a=f->r[6];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 0055a87a */
    if((f->flags&MI_ZF))goto mi_0055a881;
    /* 0055a87c */
    a=f->r[6];b=0x00000024u;t=mi_alu(f,a,b,32,1);
    /* 0055a87f */
    if(!(f->flags&MI_ZF))goto mi_0055a894;
mi_0055a881:;
    /* 0055a881 */
    f->r[0]=0x51eb851fu;
    /* 0055a886 */
    a=f->r[0];b=f->r[1];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 0055a888 */
    f->r[1]=f->r[2];
    /* 0055a88a */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,1);f->r[1]=t;
    /* 0055a88d */
    f->r[0]=f->r[1];
    /* 0055a88f */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 0055a892 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
mi_0055a894:;
    /* 0055a894 */
    f->r[2]=mi_read_0055a0e0((0x00e38da4u),4u);
    /* 0055a89a */
    f->r[0]=mi_read_0055a0e0((0x00e38da8u),4u);
    /* 0055a89f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a8a0 */
    f->r[2]=mi_read_0055a0e0((0x00f56f9cu),4u);
    /* 0055a8a6 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a8a7 */
    f->r[0]=mi_read_0055a0e0((0x00f53f08u),4u);
    /* 0055a8ac */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a8ae */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a8af */
    f->r[2]=(0x0000003du+f->r[7]+f->r[0]*1u);
    /* 0055a8b3 */
    f->r[0]=mi_read_0055a0e0((0x00f53f0cu),4u);
    /* 0055a8b8 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a8b9 */
    f->r[2]=mi_read_0055a0e0((0x00e20778u),4u);
    /* 0055a8bf */
    f->r[0]=(0x00000144u+f->r[2]+f->r[0]*1u);
    /* 0055a8c6 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
mi_0055a8c7:;
    /* 0055a8c7 */
    a=0x0057c048u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a8cc */
    a=0x00000040u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a8ce */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a8cf */
    f->r[4]-=4;MI_U32(f->r[4])=0x0055a8d4u;f->boundary(0x0055a8cfu,0x004d6860u,f);
    /* 0055a8d4 */
    a=f->r[4];b=0x00000024u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0055a8d7 */
    goto mi_0055ab2f;
mi_0055a8dc:;
    /* 0055a8dc */
    a=(f->r[0]&0xffffu);b=0x00000003u;t=mi_alu(f,a,b,16,1);
    /* 0055a8e0 */
    if(!(f->flags&MI_ZF))goto mi_0055aa29;
    /* 0055a8e6 */
    a=f->r[6];b=0x00000013u;t=mi_alu(f,a,b,32,1);
    /* 0055a8e9 */
    if(!(f->flags&MI_ZF))goto mi_0055a9fd;
    /* 0055a8ef */
    MI_U32((0x00e20778u))=(uint32_t)(f->r[1]);
    /* 0055a8f5 */
    f->r[1]=(0x0000001cu+f->r[4]);
    /* 0055a8f9 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a8fa */
    f->r[4]-=4;MI_U32(f->r[4])=0x0055a8ffu;f->boundary(0x0055a8fau,0x00559fc0u,f);
    /* 0055a8ff */
    f->r[2]=mi_read_0055a0e0((0x00e38da4u),4u);
    /* 0055a905 */
    f->r[1]=mi_read_0055a0e0((0x00f56f9cu),4u);
    /* 0055a90b */
    f->r[6]=f->r[0];
    /* 0055a90d */
    f->r[0]=mi_read_0055a0e0((0x00e38da8u),4u);
    /* 0055a912 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a913 */
    f->r[2]=mi_read_0055a0e0((0x00f53f08u),4u);
    /* 0055a919 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a91a */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a91c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a91d */
    f->r[1]=mi_read_0055a0e0((0x00f53f0cu),4u);
    /* 0055a923 */
    f->r[0]=(0x0000003du+f->r[7]+f->r[2]*1u);
    /* 0055a927 */
    a=f->r[1];b=0x00000144u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0055a92d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a92e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a92f */
    a=0x00000025u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a931 */
    a=0x00000036u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a933 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0055a938u;f->boundary(0x0055a933u,0x004d62f0u,f);
    /* 0055a938 */
    a=f->r[4];b=0x00000024u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0055a93b */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0055a93e */
    if(!(f->flags&MI_ZF))goto mi_0055a99a;
    /* 0055a940 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0055a942 */
    f->r[1]=(0x0000001cu+f->r[4]);
mi_0055a946:;
    /* 0055a946 */
    a=mi_read_0055a0e0((0x00000000u+f->r[1]),4u);b=0x00000000u;t=mi_alu(f,a,b,32,1);
    /* 0055a949 */
    if(!(f->flags&MI_ZF))goto mi_0055a959;
    /* 0055a94b */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0055a94c */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0055a94f */
    a=f->r[0];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 0055a952 */
    if(mi_signed_less(f->flags))goto mi_0055a946;
    /* 0055a954 */
    goto mi_0055ab2f;
mi_0055a959:;
    /* 0055a959 */
    f->r[2]=mi_read_0055a0e0((0x00e38da4u),4u);
    /* 0055a95f */
    f->r[1]=mi_read_0055a0e0((0x00e38da8u),4u);
    /* 0055a965 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a966 */
    f->r[2]=mi_read_0055a0e0((0x00f56f9cu),4u);
    /* 0055a96c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a96d */
    f->r[1]=mi_read_0055a0e0((0x00f53f08u),4u);
    /* 0055a973 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a975 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a976 */
    f->r[2]=(0x0000003du+f->r[7]+f->r[1]*1u);
    /* 0055a97a */
    f->r[1]=mi_read_0055a0e0((0x00f53f0cu),4u);
    /* 0055a980 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a981 */
    f->r[2]=mi_read_0055a0e0((0x00e20778u),4u);
    /* 0055a987 */
    a=f->r[0];b=0x00000027u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0055a98a */
    f->r[1]=(0x00000144u+f->r[2]+f->r[1]*1u);
    /* 0055a991 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a992 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a993 */
    a=0x00000017u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a995 */
    goto mi_0055ab27;
mi_0055a99a:;
    /* 0055a99a */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 0055a99c */
    if((f->flags&MI_ZF))goto mi_0055ab2f;
    /* 0055a9a2 */
    f->r[0]=mi_read_0055a0e0((0x00000010u+f->r[4]),4u);
    /* 0055a9a6 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 0055a9a9 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*2u);
    /* 0055a9ac */
    f->r[2]=mi_read_0055a0e0((0x00e38da4u),4u);
    /* 0055a9b2 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0055a9b4 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a9b5 */
    f->r[2]=mi_read_0055a0e0((0x00f53f08u),4u);
    /* 0055a9bb */
    f->r[1]=(0x00000038u+f->r[0]);
    /* 0055a9be */
    a=f->r[0];b=0x0000004cu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0055a9c1 */
    MI_U16((0x00603326u))=(uint16_t)((f->r[0]&0xffffu));
    /* 0055a9c7 */
    f->r[0]=mi_read_0055a0e0((0x00e38da8u),4u);
    /* 0055a9cc */
    MI_U16((0x00603322u))=(uint16_t)((f->r[1]&0xffffu));
    /* 0055a9d3 */
    f->r[1]=mi_read_0055a0e0((0x00f56f9cu),4u);
    /* 0055a9d9 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a9da */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a9dc */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a9dd */
    f->r[1]=mi_read_0055a0e0((0x00f53f0cu),4u);
    /* 0055a9e3 */
    f->r[0]=(0x0000003du+f->r[7]+f->r[2]*1u);
    /* 0055a9e7 */
    f->r[2]=mi_read_0055a0e0((0x00e20778u),4u);
    /* 0055a9ed */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a9ee */
    f->r[0]=(0x00000144u+f->r[2]+f->r[1]*1u);
    /* 0055a9f5 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a9f6 */
    a=0x0000002du;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055a9f8 */
    goto mi_0055ab25;
mi_0055a9fd:;
    /* 0055a9fd */
    f->r[2]=mi_read_0055a0e0((0x00e38da4u),4u);
    /* 0055aa03 */
    f->r[0]=mi_read_0055a0e0((0x00e38da8u),4u);
    /* 0055aa08 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055aa09 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055aa0a */
    f->r[0]=mi_read_0055a0e0((0x00f53f0cu),4u);
    /* 0055aa0f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055aa10 */
    f->r[1]=mi_read_0055a0e0((0x00f53f08u),4u);
    /* 0055aa16 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055aa17 */
    a=f->r[0];b=0x00000144u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0055aa1c */
    f->r[2]=(0x0000003du+f->r[7]+f->r[1]*1u);
    /* 0055aa20 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055aa21 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055aa22 */
    a=0x00000025u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055aa24 */
    goto mi_0055ab25;
mi_0055aa29:;
    /* 0055aa29 */
    f->r[2]=mi_read_0055a0e0((0x00e38da4u),4u);
    /* 0055aa2f */
    f->r[0]=mi_read_0055a0e0((0x00e38da8u),4u);
    /* 0055aa34 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055aa35 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055aa36 */
    f->r[0]=mi_read_0055a0e0((0x00f53f0cu),4u);
    /* 0055aa3b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055aa3c */
    f->r[1]=mi_read_0055a0e0((0x00f53f08u),4u);
    /* 0055aa42 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055aa43 */
    a=f->r[0];b=0x00000144u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0055aa48 */
    f->r[2]=(0x0000003du+f->r[7]+f->r[1]*1u);
    /* 0055aa4c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055aa4d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055aa4e */
    a=0x00000026u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055aa50 */
    goto mi_0055ab25;
mi_0055aa55:;
    /* 0055aa55 */
    f->r[1]=mi_read_0055a0e0((0x00f56f9cu),4u);
    /* 0055aa5b */
    f->r[2]=mi_read_0055a0e0((0x00f53f08u),4u);
    /* 0055aa61 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055aa62 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055aa63 */
    f->r[1]=mi_read_0055a0e0((0x00f53f0cu),4u);
    /* 0055aa69 */
    f->r[0]=(0x0000003du+f->r[7]+f->r[2]*1u);
    /* 0055aa6d */
    f->r[2]=mi_read_0055a0e0((0x005d2c48u+f->r[6]*8u),4u);
    /* 0055aa74 */
    a=0x00000064u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055aa76 */
    a=f->r[1];b=0x000000afu;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0055aa7c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055aa7d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055aa7e */
    a=0x0057c048u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055aa83 */
    a=0x00000040u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055aa85 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055aa86 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0055aa8bu;f->boundary(0x0055aa86u,0x004cce90u,f);
    /* 0055aa8b */
    f->r[0]=mi_read_0055a0e0((0x00f56f9cu),4u);
    /* 0055aa90 */
    f->r[1]=mi_read_0055a0e0((0x00f53f08u),4u);
    /* 0055aa96 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055aa97 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055aa98 */
    f->r[0]=mi_read_0055a0e0((0x00f53f0cu),4u);
    /* 0055aa9d */
    f->r[2]=(0x0000003du+f->r[7]+f->r[1]*1u);
    /* 0055aaa1 */
    f->r[1]=mi_read_0055a0e0((0x005d2c4cu+f->r[6]*8u),4u);
    /* 0055aaa8 */
    a=0x00000064u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055aaaa */
    a=f->r[0];b=0x00000113u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0055aaaf */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055aab0 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055aab1 */
    a=0x0057c048u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055aab6 */
    a=0x00000040u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055aab8 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055aab9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0055aabeu;f->boundary(0x0055aab9u,0x004cce90u,f);
    /* 0055aabe */
    f->r[0]=(0xffffffe8u+f->r[6]);
    /* 0055aac1 */
    a=f->r[4];b=0x00000040u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0055aac4 */
    a=f->r[0];b=0x0000000bu;t=mi_alu(f,a,b,32,1);
    /* 0055aac7 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0055aadf;
    /* 0055aac9 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0055aacb */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read_0055a0e0((0x0055ad1cu+f->r[0]),1u))&0xffu)<<0);
    /* 0055aad1 */
    switch(mi_read_0055a0e0((0x0055ad14u+f->r[2]*4u),4u)){case 0x0055aad8u:goto mi_0055aad8;case 0x0055aadfu:goto mi_0055aadf;default:abort();}
mi_0055aad8:;
    /* 0055aad8 */
    f->r[6]=0x00000031u;
    /* 0055aadd */
    goto mi_0055aaf8;
mi_0055aadf:;
    /* 0055aadf */
    a=f->r[6];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 0055aae2 */
    if(!(f->flags&MI_ZF))goto mi_0055aaeb;
    /* 0055aae4 */
    f->r[6]=0x0000002fu;
    /* 0055aae9 */
    goto mi_0055aaf8;
mi_0055aaeb:;
    /* 0055aaeb */
    a=f->r[6];b=0x0000001cu;t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 0055aaee */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,0,a,32,1);f->r[6]=t;
    /* 0055aaf0 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,2);f->r[6]=t;
    /* 0055aaf2 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 0055aaf5 */
    a=f->r[6];b=0x00000030u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
mi_0055aaf8:;
    /* 0055aaf8 */
    f->r[0]=mi_read_0055a0e0((0x00e38da4u),4u);
    /* 0055aafd */
    f->r[1]=mi_read_0055a0e0((0x00e38da8u),4u);
    /* 0055ab03 */
    f->r[2]=mi_read_0055a0e0((0x00f56f9cu),4u);
    /* 0055ab09 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055ab0a */
    f->r[0]=mi_read_0055a0e0((0x00f53f08u),4u);
    /* 0055ab0f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055ab10 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055ab11 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055ab12 */
    f->r[2]=mi_read_0055a0e0((0x00f53f0cu),4u);
    /* 0055ab18 */
    f->r[1]=(0x0000003du+f->r[7]+f->r[0]*1u);
    /* 0055ab1c */
    a=f->r[2];b=0x000001a9u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0055ab22 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055ab23 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055ab24 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
mi_0055ab25:;
    /* 0055ab25 */
    a=0x00000036u;f->r[4]-=4;MI_U32(f->r[4])=a;
mi_0055ab27:;
    /* 0055ab27 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0055ab2cu;f->boundary(0x0055ab27u,0x004d62f0u,f);
    /* 0055ab2c */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0055ab2f:;
    /* 0055ab2f */
    f->r[0]=mi_read_0055a0e0((0x00000018u+f->r[4]),4u);
    /* 0055ab33 */
    f->r[2]=mi_read_0055a0e0((0x00000014u+f->r[4]),4u);
    /* 0055ab37 */
    f->r[1]=mi_read_0055a0e0((0x00f53e6cu),4u);
    /* 0055ab3d */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0055ab3e */
    a=f->r[2];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0055ab41 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0055ab43 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0055ab47 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0055ab4b */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0055ab65;
    /* 0055ab4d */
    f->r[2]=mi_read_0055a0e0((0x00000010u+f->r[4]),4u);
    /* 0055ab51 */
    a=f->r[7];b=0x00000016u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0055ab54 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0055ab55 */
    a=f->r[7];b=0x0000014au;t=mi_alu(f,a,b,32,1);
    /* 0055ab5b */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0055ab5f */
    if(mi_signed_less(f->flags))goto mi_0055a2da;
mi_0055ab65:;
    /* 0055ab65 */
    f->r[3]=mi_read_0055a0e0((0x007056c8u),4u);
    /* 0055ab6b */
    f->r[1]=mi_read_0055a0e0((0x00e38da4u),4u);
    /* 0055ab71 */
    f->r[2]=mi_read_0055a0e0((0x00e38da8u),4u);
    /* 0055ab77 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0055ab79 */
    a=f->r[3];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0055ab7c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055ab7d */
    f->r[1]=mi_read_0055a0e0((0x00f53f0cu),4u);
    /* 0055ab83 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055ab84 */
    t=((f->flags&MI_ZF))?1u:0u;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0055ab87 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055ab88 */
    f->r[0]=mi_read_0055a0e0((0x00f53f08u),4u);
    /* 0055ab8d */
    a=0x00000018u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055ab8f */
    a=f->r[0];b=0x0000018cu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0055ab94 */
    a=0x000000c8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055ab99 */
    a=f->r[1];b=0x0000018eu;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0055ab9f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055aba0 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055aba1 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055aba3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0055aba8u;f->boundary(0x0055aba3u,0x004d63d0u,f);
    /* 0055aba8 */
    f->r[2]=mi_read_0055a0e0((0x00eaac28u),4u);
    /* 0055abae */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0055abb1 */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 0055abb7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0055abb8 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read_0055a0e0((0x00ea623cu+f->r[2]),1u))&0xffu)<<0);
    /* 0055abbe */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0055abc0 */
    if((f->flags&MI_ZF))goto mi_0055abf9;
    /* 0055abc2 */
    f->r[0]=mi_read_0055a0e0((0x00e38da4u),4u);
    /* 0055abc7 */
    f->r[1]=mi_read_0055a0e0((0x00e38da8u),4u);
    /* 0055abcd */
    f->r[2]=mi_read_0055a0e0((0x00f56f8cu),4u);
    /* 0055abd3 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055abd4 */
    f->r[0]=mi_read_0055a0e0((0x00f53f08u),4u);
    /* 0055abd9 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055abda */
    f->r[1]=mi_read_0055a0e0((0x00f53f0cu),4u);
    /* 0055abe0 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055abe2 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055abe3 */
    a=f->r[0];b=0x00000192u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0055abe8 */
    a=0x000000c8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055abed */
    a=f->r[1];b=0x00000190u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0055abf3 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055abf4 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055abf5 */
    a=0x0000002eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055abf7 */
    goto mi_0055ac2e;
mi_0055abf9:;
    /* 0055abf9 */
    f->r[2]=mi_read_0055a0e0((0x00e38da4u),4u);
    /* 0055abff */
    f->r[0]=mi_read_0055a0e0((0x00e38da8u),4u);
    /* 0055ac04 */
    f->r[1]=mi_read_0055a0e0((0x00f56f8cu),4u);
    /* 0055ac0a */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055ac0b */
    f->r[2]=mi_read_0055a0e0((0x00f53f08u),4u);
    /* 0055ac11 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055ac12 */
    f->r[0]=mi_read_0055a0e0((0x00f53f0cu),4u);
    /* 0055ac17 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055ac19 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055ac1a */
    a=f->r[2];b=0x00000192u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0055ac20 */
    a=0x000000c8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055ac25 */
    a=f->r[0];b=0x00000190u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0055ac2a */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055ac2b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055ac2c */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
mi_0055ac2e:;
    /* 0055ac2e */
    a=0x00000036u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055ac30 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0055ac35u;f->boundary(0x0055ac30u,0x004d6250u,f);
    /* 0055ac35 */
    f->r[1]=mi_read_0055a0e0((0x007056c8u),4u);
    /* 0055ac3b */
    f->r[2]=mi_read_0055a0e0((0x00e38da8u),4u);
    /* 0055ac41 */
    a=f->r[4];b=0x00000024u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0055ac44 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0055ac46 */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0055ac49 */
    f->r[1]=mi_read_0055a0e0((0x00e38da4u),4u);
    /* 0055ac4f */
    t=((f->flags&MI_ZF))?1u:0u;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0055ac52 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055ac53 */
    f->r[1]=mi_read_0055a0e0((0x00f53f0cu),4u);
    /* 0055ac59 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055ac5a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055ac5b */
    f->r[0]=mi_read_0055a0e0((0x00f53f08u),4u);
    /* 0055ac60 */
    a=0x00000018u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055ac62 */
    a=f->r[0];b=0x0000018cu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0055ac67 */
    a=0x000000c8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055ac6c */
    a=f->r[1];b=0x00000062u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0055ac6f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055ac70 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055ac71 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055ac73 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0055ac78u;f->boundary(0x0055ac73u,0x004d63d0u,f);
    /* 0055ac78 */
    f->r[2]=mi_read_0055a0e0((0x00e38da4u),4u);
    /* 0055ac7e */
    f->r[0]=mi_read_0055a0e0((0x00e38da8u),4u);
    /* 0055ac83 */
    f->r[1]=mi_read_0055a0e0((0x00f56f8cu),4u);
    /* 0055ac89 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055ac8a */
    f->r[2]=mi_read_0055a0e0((0x00f53f08u),4u);
    /* 0055ac90 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055ac91 */
    f->r[0]=mi_read_0055a0e0((0x00f53f0cu),4u);
    /* 0055ac96 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055ac98 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055ac99 */
    a=f->r[2];b=0x00000192u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0055ac9f */
    a=0x000000c8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055aca4 */
    a=f->r[0];b=0x00000064u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0055aca7 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055aca8 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055aca9 */
    a=0x0000001eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055acab */
    a=0x00000036u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055acad */
    f->r[4]-=4;MI_U32(f->r[4])=0x0055acb2u;f->boundary(0x0055acadu,0x004d6250u,f);
    /* 0055acb2 */
    a=f->r[4];b=0x00000044u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0055acb5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0055acbau;f->boundary(0x0055acb5u,0x0055a060u,f);
mi_0055acba:;
    /* 0055acba */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0055acbb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0055acbc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0055acbd */
    a=f->r[4];b=0x0000004cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0055acc0 */
    f->r[4]+=4;return;
}
