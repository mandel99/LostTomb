/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "drag_commit_model.h"

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

void restore_C_00478440(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00478440 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 00478443 */
    f->r[2]=MI_U32((0x00bca2d8u));
    /* 00478449 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047844a */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047844b */
    f->r[5]=MI_U32((0x0000001cu+f->r[4]));
    /* 0047844f */
    f->r[3]=MI_U32((0x00000018u+f->r[4]));
    /* 00478453 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478454 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478455 */
    f->r[0]=(0x00000000u+f->r[5]+f->r[5]*8u);
    /* 00478459 */
    f->r[0]=(0x00000000u+f->r[5]+f->r[0]*2u);
    /* 0047845d */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 00478460 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[1]*4u);
    /* 00478463 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00478465 */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[0]);
    /* 0047846a */
    f->r[1]=MI_U32((0x00af598cu+f->r[0]*4u));
    /* 00478471 */
    f->r[0]=MI_U32((0x006059e8u));
    /* 00478476 */
    a=f->r[1];b=0xeefed73fu;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 0047847c */
    a=f->r[0];b=0x00000077u;t=mi_alu(f,a,b,32,1);
    /* 0047847f */
    if(!(f->flags&MI_ZF))goto mi_0047849f;
    /* 00478481 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00478483 */
    if(!(f->flags&MI_ZF))goto mi_00478d2a;
    /* 00478489 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0047848a */
    MI_U16((0x00784c62u))=(uint16_t)((f->r[5]&0xffffu));
    /* 00478491 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00478492 */
    MI_U16((0x00784c60u))=(uint16_t)((f->r[3]&0xffffu));
    /* 00478499 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0047849a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0047849b */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047849e */
    f->r[4]+=4;return;
mi_0047849f:;
    /* 0047849f */
    a=f->r[0];b=0x0000007fu;t=mi_alu(f,a,b,32,1);
    /* 004784a2 */
    if(!(f->flags&MI_ZF))goto mi_004784e5;
    /* 004784a4 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004784a6 */
    if(!(f->flags&MI_ZF))goto mi_00478d2a;
    /* 004784ac */
    f->r[0]=MI_U32((0x00bca2ecu));
    /* 004784b1 */
    f->r[1]=MI_U32((0x00bca2e8u));
    /* 004784b7 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004784b9 */
    a=0x00000004u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004784bb */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004784bd */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004784be */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004784bf */
    f->r[4]-=4;MI_U32(f->r[4])=0x004784c4u;f->boundary(0x004784bfu,0x0046f9a0u,f);
    /* 004784c4 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004784c7 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004784c9 */
    if((f->flags&MI_ZF))goto mi_00478d2a;
    /* 004784cf */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004784d0 */
    MI_U16((0x00784c66u))=(uint16_t)((f->r[5]&0xffffu));
    /* 004784d7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004784d8 */
    MI_U16((0x00784c64u))=(uint16_t)((f->r[3]&0xffffu));
    /* 004784df */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004784e0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004784e1 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004784e4 */
    f->r[4]+=4;return;
mi_004784e5:;
    /* 004784e5 */
    a=f->r[0];b=0x00000080u;t=mi_alu(f,a,b,32,1);
    /* 004784ea */
    if(!(f->flags&MI_ZF))goto mi_0047852d;
    /* 004784ec */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004784ee */
    if(!(f->flags&MI_ZF))goto mi_00478d2a;
    /* 004784f4 */
    f->r[2]=MI_U32((0x00bca2ecu));
    /* 004784fa */
    f->r[0]=MI_U32((0x00bca2e8u));
    /* 004784ff */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00478501 */
    a=0x00000004u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478503 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00478505 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478506 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478507 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047850cu;f->boundary(0x00478507u,0x0046f9a0u,f);
    /* 0047850c */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047850f */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00478511 */
    if((f->flags&MI_ZF))goto mi_00478d2a;
    /* 00478517 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00478518 */
    MI_U16((0x00784c6au))=(uint16_t)((f->r[5]&0xffffu));
    /* 0047851f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00478520 */
    MI_U16((0x00784c68u))=(uint16_t)((f->r[3]&0xffffu));
    /* 00478527 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00478528 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00478529 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047852c */
    f->r[4]+=4;return;
mi_0047852d:;
    /* 0047852d */
    a=f->r[0];b=0x0000007eu;t=mi_alu(f,a,b,32,1);
    /* 00478530 */
    if(!(f->flags&MI_ZF))goto mi_0047858f;
    /* 00478532 */
    a=MI_U32((0x00605a28u));b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 00478539 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00478545;
    /* 0047853b */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0047853d */
    if(!(f->flags&MI_ZF))goto mi_00478d2a;
    /* 00478543 */
    goto mi_0047854e;
mi_00478545:;
    /* 00478545 */
    a=(f->r[1]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,3);
    /* 00478548 */
    if((f->flags&MI_ZF))goto mi_00478d2a;
mi_0047854e:;
    /* 0047854e */
    f->r[1]=MI_U32((0x00bca2ecu));
    /* 00478554 */
    f->r[2]=MI_U32((0x00bca2e8u));
    /* 0047855a */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0047855c */
    a=0x00000004u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047855e */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00478560 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478561 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478562 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00478567u;f->boundary(0x00478562u,0x0046f9a0u,f);
    /* 00478567 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047856a */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0047856c */
    if((f->flags&MI_ZF))goto mi_00478d2a;
    /* 00478572 */
    f->r[0]=MI_U32((0x00605a28u));
    /* 00478577 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00478578 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00478579 */
    MI_U16((0x00784bbeu+f->r[0]*2u))=(uint16_t)((f->r[3]&0xffffu));
    /* 00478581 */
    MI_U16((0x00784bdeu+f->r[0]*2u))=(uint16_t)((f->r[5]&0xffffu));
    /* 00478589 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0047858a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0047858b */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047858e */
    f->r[4]+=4;return;
mi_0047858f:;
    /* 0047858f */
    a=f->r[0];b=0x00000084u;t=mi_alu(f,a,b,32,1);
    /* 00478594 */
    if(!(f->flags&MI_ZF))goto mi_004785bc;
    /* 00478596 */
    a=(f->r[1]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,3);
    /* 00478599 */
    if((f->flags&MI_ZF))goto mi_00478d2a;
    /* 0047859f */
    f->r[0]=MI_U32((0x00605a24u));
    /* 004785a4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004785a5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004785a6 */
    MI_U16((0x00784b72u+f->r[0]*2u))=(uint16_t)((f->r[3]&0xffffu));
    /* 004785ae */
    MI_U16((0x00784b82u+f->r[0]*2u))=(uint16_t)((f->r[5]&0xffffu));
    /* 004785b6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004785b7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004785b8 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004785bb */
    f->r[4]+=4;return;
mi_004785bc:;
    /* 004785bc */
    a=f->r[0];b=0x00000085u;t=mi_alu(f,a,b,32,1);
    /* 004785c1 */
    if(!(f->flags&MI_ZF))goto mi_004785ff;
    /* 004785c3 */
    f->r[0]=MI_U32((0x00bca300u));
    /* 004785c8 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004785ca */
    f->r[1]=MI_U32((0x00af598cu+f->r[0]*4u));
    /* 004785d1 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004785d2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004785d7u;f->boundary(0x004785d2u,0x004ec410u,f);
    /* 004785d7 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004785da */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004785dc */
    if((f->flags&MI_ZF))goto mi_00478d2a;
    /* 004785e2 */
    f->r[0]=MI_U32((0x00605a1cu));
    /* 004785e7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004785e8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004785e9 */
    MI_U16((0x00784b62u+f->r[0]*2u))=(uint16_t)((f->r[3]&0xffffu));
    /* 004785f1 */
    MI_U16((0x00784b6au+f->r[0]*2u))=(uint16_t)((f->r[5]&0xffffu));
    /* 004785f9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004785fa */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004785fb */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004785fe */
    f->r[4]+=4;return;
mi_004785ff:;
    /* 004785ff */
    a=f->r[0];b=0x0000008bu;t=mi_alu(f,a,b,32,1);
    /* 00478604 */
    if(!(f->flags&MI_ZF))goto mi_00478641;
    /* 00478606 */
    f->r[2]=MI_U32((0x00bca300u));
    /* 0047860c */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047860e */
    f->r[0]=MI_U32((0x00af598cu+f->r[2]*4u));
    /* 00478615 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478616 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047861bu;f->boundary(0x00478616u,0x004ec410u,f);
    /* 0047861b */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047861e */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00478620 */
    if((f->flags&MI_ZF))goto mi_00478d2a;
    /* 00478626 */
    f->r[0]=MI_U32((0x00605a1cu));
    /* 0047862b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0047862c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0047862d */
    MI_U32((0x00784cbcu+f->r[0]*4u))=(uint32_t)(f->r[3]);
    /* 00478634 */
    MI_U32((0x00784cccu+f->r[0]*4u))=(uint32_t)(f->r[5]);
    /* 0047863b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0047863c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0047863d */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00478640 */
    f->r[4]+=4;return;
mi_00478641:;
    /* 00478641 */
    a=f->r[0];b=0x000000d6u;t=mi_alu(f,a,b,32,1);
    /* 00478646 */
    if(!(f->flags&MI_ZF))goto mi_0047867e;
    /* 00478648 */
    a=(f->r[1]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,3);
    /* 0047864b */
    if((f->flags&MI_ZF))goto mi_00478d2a;
    /* 00478651 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478652 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478653 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00478658u;f->boundary(0x00478653u,0x00477d40u,f);
    /* 00478658 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047865b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0047865d */
    if((f->flags&MI_ZF))goto mi_00478d2a;
    /* 00478663 */
    f->r[0]=MI_U32((0x00605a20u));
    /* 00478668 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00478669 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0047866a */
    MI_U32((0x00784dc0u+f->r[0]*4u))=(uint32_t)(f->r[3]);
    /* 00478671 */
    MI_U32((0x00784dccu+f->r[0]*4u))=(uint32_t)(f->r[5]);
    /* 00478678 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00478679 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0047867a */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047867d */
    f->r[4]+=4;return;
mi_0047867e:;
    /* 0047867e */
    a=f->r[0];b=0x00000082u;t=mi_alu(f,a,b,32,1);
    /* 00478683 */
    if(!(f->flags&MI_ZF))goto mi_004786e7;
    /* 00478685 */
    f->r[1]=MI_U32((0x00bca2ecu));
    /* 0047868b */
    f->r[2]=MI_U32((0x00bca2e8u));
    /* 00478691 */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00478693 */
    a=0x00000004u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478695 */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00478697 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478698 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478699 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047869eu;f->boundary(0x00478699u,0x0046f9a0u,f);
    /* 0047869e */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004786a1 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004786a3 */
    if((f->flags&MI_ZF))goto mi_00478d2a;
    /* 004786a9 */
    f->r[0]=MI_U32((0x00bc9768u));
    /* 004786ae */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004786af */
    f->r[4]-=4;MI_U32(f->r[4])=0x004786b4u;f->boundary(0x004786afu,0x0047ed60u,f);
    /* 004786b4 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004786b7 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004786b9 */
    if((f->flags&MI_ZF))goto mi_00478d2a;
    /* 004786bf */
    f->r[1]=(uint32_t)(int32_t)(int16_t)(f->r[5]&0xffffu);
    /* 004786c2 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)(f->r[3]&0xffffu);
    /* 004786c5 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004786c7 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004786c8 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004786c9 */
    MI_U16((0x00784c8cu))=(uint16_t)((f->r[3]&0xffffu));
    /* 004786d0 */
    MI_U16((0x00784c8eu))=(uint16_t)((f->r[5]&0xffffu));
    /* 004786d7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004786dcu;f->boundary(0x004786d7u,0x004bbd40u,f);
    /* 004786dc */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004786df */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004786e0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004786e1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004786e2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004786e3 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004786e6 */
    f->r[4]+=4;return;
mi_004786e7:;
    /* 004786e7 */
    a=f->r[0];b=0x00000083u;t=mi_alu(f,a,b,32,1);
    /* 004786ec */
    if(!(f->flags&MI_ZF))goto mi_0047873e;
    /* 004786ee */
    f->r[0]=MI_U32((0x00bca2ecu));
    /* 004786f3 */
    f->r[1]=MI_U32((0x00bca2e8u));
    /* 004786f9 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004786fb */
    a=0x00000004u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004786fd */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004786ff */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478700 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478701 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00478706u;f->boundary(0x00478701u,0x0046f9a0u,f);
    /* 00478706 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00478709 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0047870b */
    if((f->flags&MI_ZF))goto mi_00478d2a;
    /* 00478711 */
    f->r[2]=MI_U32((0x00bc9768u));
    /* 00478717 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478718 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047871du;f->boundary(0x00478718u,0x0047ed60u,f);
    /* 0047871d */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00478720 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00478722 */
    if((f->flags&MI_ZF))goto mi_00478d2a;
    /* 00478728 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00478729 */
    MI_U16((0x00784c92u))=(uint16_t)((f->r[5]&0xffffu));
    /* 00478730 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00478731 */
    MI_U16((0x00784c90u))=(uint16_t)((f->r[3]&0xffffu));
    /* 00478738 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00478739 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0047873a */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047873d */
    f->r[4]+=4;return;
mi_0047873e:;
    /* 0047873e */
    f->r[2]=MI_U32((0x00605a34u));
    /* 00478744 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 00478746 */
    if(!(f->flags&MI_ZF))goto mi_0047874f;
    /* 00478748 */
    f->r[6]=0x00000001u;
    /* 0047874d */
    goto mi_00478776;
mi_0047874f:;
    /* 0047874f */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 00478752 */
    if(!(f->flags&MI_ZF))goto mi_0047875b;
    /* 00478754 */
    f->r[6]=0x00000005u;
    /* 00478759 */
    goto mi_00478776;
mi_0047875b:;
    /* 0047875b */
    a=f->r[2];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0047875e */
    if(!(f->flags&MI_ZF))goto mi_00478767;
    /* 00478760 */
    f->r[6]=0x0000000du;
    /* 00478765 */
    goto mi_00478776;
mi_00478767:;
    /* 00478767 */
    f->r[6]=f->r[2];
    /* 00478769 */
    a=f->r[6];b=0x00000003u;t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 0047876c */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,0,a,32,1);f->r[6]=t;
    /* 0047876e */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,2);f->r[6]=t;
    /* 00478770 */
    a=f->r[6];b=0x00000010u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 00478773 */
    a=f->r[6];b=0x00000019u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
mi_00478776:;
    /* 00478776 */
    a=MI_U32((0x00605a30u));b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 0047877d */
    if((f->flags&MI_ZF))goto mi_00478786;
    /* 0047877f */
    a=f->r[0];b=0x00000081u;t=mi_alu(f,a,b,32,1);
    /* 00478784 */
    if(!(f->flags&MI_ZF))goto mi_0047878b;
mi_00478786:;
    /* 00478786 */
    f->r[6]=0x00000001u;
mi_0047878b:;
    /* 0047878b */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 0047878d */
    MI_U8((0x00000024u+f->r[4]))=(uint8_t)(0x00000000u);
    /* 00478792 */
    if((f->flags&MI_ZF))goto mi_00478a7d;
    /* 00478798 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00478a7d;
    /* 0047879e */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x005cc77cu);
    /* 004787a6 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[6]);
mi_004787aa:;
    /* 004787aa */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
    /* 004787ae */
    f->r[7]=MI_U32((0xfffffffcu+f->r[1]));
    /* 004787b1 */
    f->r[6]=MI_U32((0x00000000u+f->r[1]));
    /* 004787b3 */
    a=f->r[7];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004787b5 */
    a=f->r[6];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004787b7 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 004787b9 */
    if(mi_signed_less(f->flags))goto mi_004789fa;
    /* 004787bf */
    a=f->r[7];b=MI_U32((0x00bca2d0u));t=mi_alu(f,a,b,32,1);
    /* 004787c5 */
    if(!mi_signed_less(f->flags))goto mi_004789fa;
    /* 004787cb */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004787cd */
    if(mi_signed_less(f->flags))goto mi_004789fa;
    /* 004787d3 */
    a=f->r[6];b=MI_U32((0x00bca2d4u));t=mi_alu(f,a,b,32,1);
    /* 004787d9 */
    if(!mi_signed_less(f->flags))goto mi_004789fa;
    /* 004787df */
    f->r[1]=(0x00000000u+f->r[6]+f->r[6]*8u);
    /* 004787e2 */
    f->r[1]=(0x00000000u+f->r[6]+f->r[1]*2u);
    /* 004787e5 */
    f->r[2]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 004787e8 */
    f->r[1]=MI_U32((0x00bca2d8u));
    /* 004787ee */
    f->r[1]=(0x00000000u+f->r[1]+f->r[2]*4u);
    /* 004787f1 */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004787f3 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004787f7 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00b4e6fcu+f->r[1]*2u));
    /* 004787ff */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00478801 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00478805 */
    if((f->flags&MI_ZF))goto mi_00478823;
    /* 00478807 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478808 */
    f->r[2]=f->r[1];
    /* 0047880a */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047880b */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047880c */
    f->r[4]-=4;MI_U32(f->r[4])=0x00478811u;f->boundary(0x0047880cu,0x0047c480u,f);
    /* 00478811 */
    f->r[0]=MI_U32((0x00000020u+f->r[4]));
    /* 00478815 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478816 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047881bu;f->boundary(0x00478816u,0x0046ac10u,f);
    /* 0047881b */
    f->r[0]=MI_U32((0x006059e8u));
    /* 00478820 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00478823:;
    /* 00478823 */
    f->r[1]=MI_U32((0x00000020u+f->r[4]));
    /* 00478827 */
    a=f->r[0];b=0x00000074u;t=mi_alu(f,a,b,32,1);
    /* 0047882a */
    f->r[2]=MI_U32((0x00af598cu+f->r[1]*4u));
    /* 00478831 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00478835 */
    if(!(f->flags&MI_ZF))goto mi_00478843;
    /* 00478837 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478838 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478839 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047883eu;f->boundary(0x00478839u,0x00477960u,f);
    /* 0047883e */
    goto mi_00478980;
mi_00478843:;
    /* 00478843 */
    a=f->r[0];b=0x00000075u;t=mi_alu(f,a,b,32,1);
    /* 00478846 */
    if(!(f->flags&MI_ZF))goto mi_00478854;
    /* 00478848 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478849 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047884a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047884fu;f->boundary(0x0047884au,0x004779f0u,f);
    /* 0047884f */
    goto mi_00478980;
mi_00478854:;
    /* 00478854 */
    a=f->r[0];b=0x00000076u;t=mi_alu(f,a,b,32,1);
    /* 00478857 */
    if(!(f->flags&MI_ZF))goto mi_00478865;
    /* 00478859 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047885a */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047885b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00478860u;f->boundary(0x0047885bu,0x00477c20u,f);
    /* 00478860 */
    goto mi_00478980;
mi_00478865:;
    /* 00478865 */
    a=f->r[0];b=0x00000078u;t=mi_alu(f,a,b,32,1);
    /* 00478868 */
    if(!(f->flags&MI_ZF))goto mi_00478876;
    /* 0047886a */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047886b */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047886c */
    f->r[4]-=4;MI_U32(f->r[4])=0x00478871u;f->boundary(0x0047886cu,0x0051f780u,f);
    /* 00478871 */
    goto mi_00478980;
mi_00478876:;
    /* 00478876 */
    a=f->r[0];b=0x000000a3u;t=mi_alu(f,a,b,32,1);
    /* 0047887b */
    if(!(f->flags&MI_ZF))goto mi_0047888e;
    /* 0047887d */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047887f */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478880 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478881 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00478886u;f->boundary(0x00478881u,0x00477a40u,f);
    /* 00478886 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00478889 */
    goto mi_00478983;
mi_0047888e:;
    /* 0047888e */
    a=f->r[0];b=0x000000dfu;t=mi_alu(f,a,b,32,1);
    /* 00478893 */
    if(!(f->flags&MI_ZF))goto mi_004788a9;
    /* 00478895 */
    a=0x20000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047889a */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047889b */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047889c */
    f->r[4]-=4;MI_U32(f->r[4])=0x004788a1u;f->boundary(0x0047889cu,0x00477a40u,f);
    /* 004788a1 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004788a4 */
    goto mi_00478983;
mi_004788a9:;
    /* 004788a9 */
    a=f->r[0];b=0x000000a4u;t=mi_alu(f,a,b,32,1);
    /* 004788ae */
    if(!(f->flags&MI_ZF))goto mi_004788c4;
    /* 004788b0 */
    a=0x00100002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004788b5 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004788b6 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004788b7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004788bcu;f->boundary(0x004788b7u,0x00477a40u,f);
    /* 004788bc */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004788bf */
    goto mi_00478983;
mi_004788c4:;
    /* 004788c4 */
    a=f->r[0];b=0x000000a5u;t=mi_alu(f,a,b,32,1);
    /* 004788c9 */
    if(!(f->flags&MI_ZF))goto mi_004788df;
    /* 004788cb */
    a=0x00200002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004788d0 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004788d1 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004788d2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004788d7u;f->boundary(0x004788d2u,0x00477a40u,f);
    /* 004788d7 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004788da */
    goto mi_00478983;
mi_004788df:;
    /* 004788df */
    a=f->r[0];b=0x000000a6u;t=mi_alu(f,a,b,32,1);
    /* 004788e4 */
    if(!(f->flags&MI_ZF))goto mi_004788fa;
    /* 004788e6 */
    a=0x00300002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004788eb */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004788ec */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004788ed */
    f->r[4]-=4;MI_U32(f->r[4])=0x004788f2u;f->boundary(0x004788edu,0x00477a40u,f);
    /* 004788f2 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004788f5 */
    goto mi_00478983;
mi_004788fa:;
    /* 004788fa */
    a=f->r[0];b=0x000000c6u;t=mi_alu(f,a,b,32,1);
    /* 004788ff */
    if(!(f->flags&MI_ZF))goto mi_0047890a;
    /* 00478901 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478902 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478903 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00478908u;f->boundary(0x00478903u,0x00477ab0u,f);
    /* 00478908 */
    goto mi_00478980;
mi_0047890a:;
    /* 0047890a */
    a=f->r[0];b=0x0000007au;t=mi_alu(f,a,b,32,1);
    /* 0047890d */
    if(!(f->flags&MI_ZF))goto mi_00478918;
    /* 0047890f */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478910 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478911 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00478916u;f->boundary(0x00478911u,0x00477b10u,f);
    /* 00478916 */
    goto mi_00478980;
mi_00478918:;
    /* 00478918 */
    a=f->r[0];b=0x000000c5u;t=mi_alu(f,a,b,32,1);
    /* 0047891d */
    if(!(f->flags&MI_ZF))goto mi_00478928;
    /* 0047891f */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478920 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478921 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00478926u;f->boundary(0x00478921u,0x00477b70u,f);
    /* 00478926 */
    goto mi_00478980;
mi_00478928:;
    /* 00478928 */
    a=f->r[0];b=0x00000086u;t=mi_alu(f,a,b,32,1);
    /* 0047892d */
    if(!(f->flags&MI_ZF))goto mi_00478938;
    /* 0047892f */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478930 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478931 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00478936u;f->boundary(0x00478931u,0x00477bd0u,f);
    /* 00478936 */
    goto mi_00478980;
mi_00478938:;
    /* 00478938 */
    a=f->r[0];b=0x00000087u;t=mi_alu(f,a,b,32,1);
    /* 0047893d */
    if(!(f->flags&MI_ZF))goto mi_00478948;
    /* 0047893f */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478940 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478941 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00478946u;f->boundary(0x00478941u,0x00477ce0u,f);
    /* 00478946 */
    goto mi_00478980;
mi_00478948:;
    /* 00478948 */
    a=f->r[0];b=0x0000007du;t=mi_alu(f,a,b,32,1);
    /* 0047894b */
    if(!(f->flags&MI_ZF))goto mi_00478956;
    /* 0047894d */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047894e */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047894f */
    f->r[4]-=4;MI_U32(f->r[4])=0x00478954u;f->boundary(0x0047894fu,0x00477c80u,f);
    /* 00478954 */
    goto mi_00478980;
mi_00478956:;
    /* 00478956 */
    a=f->r[0];b=0x0000007bu;t=mi_alu(f,a,b,32,1);
    /* 00478959 */
    if(!(f->flags&MI_ZF))goto mi_00478964;
    /* 0047895b */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047895c */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047895d */
    f->r[4]-=4;MI_U32(f->r[4])=0x00478962u;f->boundary(0x0047895du,0x004793e0u,f);
    /* 00478962 */
    goto mi_00478980;
mi_00478964:;
    /* 00478964 */
    a=f->r[0];b=0x0000007cu;t=mi_alu(f,a,b,32,1);
    /* 00478967 */
    if(!(f->flags&MI_ZF))goto mi_00478972;
    /* 00478969 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047896a */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047896b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00478970u;f->boundary(0x0047896bu,0x00479460u,f);
    /* 00478970 */
    goto mi_00478980;
mi_00478972:;
    /* 00478972 */
    a=f->r[0];b=0x00000081u;t=mi_alu(f,a,b,32,1);
    /* 00478977 */
    if(!(f->flags&MI_ZF))goto mi_00478983;
    /* 00478979 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047897a */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047897b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00478980u;f->boundary(0x0047897bu,0x0051f780u,f);
mi_00478980:;
    /* 00478980 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00478983:;
    /* 00478983 */
    f->r[0]=MI_U32((0x00000020u+f->r[4]));
    /* 00478987 */
    f->r[0]=MI_U32((0x00af598cu+f->r[0]*4u));
    /* 0047898e */
    a=f->r[0];b=0x00010000u;t=mi_alu(f,a,b,32,3);
    /* 00478993 */
    if((f->flags&MI_ZF))goto mi_0047899e;
    /* 00478995 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478996 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478997 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047899cu;f->boundary(0x00478997u,0x00477f70u,f);
    /* 0047899c */
    goto mi_004789c7;
mi_0047899e:;
    /* 0047899e */
    a=f->r[0];b=0x22341a33u;t=mi_alu(f,a,b,32,3);
    /* 004789a3 */
    if((f->flags&MI_ZF))goto mi_004789ae;
    /* 004789a5 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004789a6 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004789a7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004789acu;f->boundary(0x004789a7u,0x00478090u,f);
    /* 004789ac */
    goto mi_004789c7;
mi_004789ae:;
    /* 004789ae */
    a=(f->r[0]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,3);
    /* 004789b0 */
    if((f->flags&MI_ZF))goto mi_004789c0;
    /* 004789b2 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004789b4 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004789b5 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004789b6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004789bbu;f->boundary(0x004789b6u,0x00477f60u,f);
    /* 004789bb */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004789be */
    goto mi_004789ca;
mi_004789c0:;
    /* 004789c0 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004789c1 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004789c2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004789c7u;f->boundary(0x004789c2u,0x00478270u,f);
mi_004789c7:;
    /* 004789c7 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004789ca:;
    /* 004789ca */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00000024u+f->r[4])))&0xffu)<<0);
    /* 004789ce */
    a=(f->r[1]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,4);f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004789d0 */
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
    /* 004789d4 */
    MI_U8((0x00000024u+f->r[4]))=(uint8_t)((f->r[1]&0xffu));
    /* 004789d8 */
    f->r[1]=MI_U32((0x00000020u+f->r[4]));
    /* 004789dc */
    a=f->r[0];b=MI_U32((0x00af598cu+f->r[1]*4u));t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004789e3 */
    a=f->r[0];b=0x00050004u;t=mi_alu(f,a,b,32,3);
    /* 004789e8 */
    if((f->flags&MI_ZF))goto mi_004789ef;
    /* 004789ea */
    a=MI_U8((0x00000024u+f->r[4]));b=0x00000001u;t=mi_alu(f,a,b,8,4);MI_U8((0x00000024u+f->r[4]))=(uint8_t)(t);
mi_004789ef:;
    /* 004789ef */
    f->r[0]=MI_U32((0x006059e8u));
    /* 004789f4 */
    f->r[2]=MI_U32((0x00605a34u));
mi_004789fa:;
    /* 004789fa */
    f->r[6]=MI_U32((0x00000010u+f->r[4]));
    /* 004789fe */
    f->r[1]=MI_U32((0x00000018u+f->r[4]));
    /* 00478a02 */
    a=f->r[6];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00478a05 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00478a06 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 00478a0a */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00478a0e */
    if(!(f->flags&MI_ZF))goto mi_004787aa;
    /* 00478a14 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00000024u+f->r[4])))&0xffu)<<0);
    /* 00478a18 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00478a1a */
    if((f->flags&MI_ZF))goto mi_00478a7d;
    /* 00478a1c */
    f->r[0]=MI_U32((0x00e92e20u));
    /* 00478a21 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00478a23 */
    if((f->flags&MI_ZF))goto mi_00478a6e;
    /* 00478a25 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00784cb0u)))&0xffu)<<0);
    /* 00478a2a */
    f->r[6]=0x00000010u;
    /* 00478a2f */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00478a31 */
    if(!(f->flags&MI_ZF))goto mi_00478a3a;
    /* 00478a33 */
    f->r[6]=0x0000002eu;
    /* 00478a38 */
    goto mi_00478a43;
mi_00478a3a:;
    /* 00478a3a */
    a=(f->r[0]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 00478a3c */
    if(!(f->flags&MI_ZF))goto mi_00478a43;
    /* 00478a3e */
    f->r[6]=0x00000008u;
mi_00478a43:;
    /* 00478a43 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 00478a44 */
    f->r[1]=f->r[5];
    /* 00478a46 */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00478a48 */
    f->r[2]=(0x00000000u+f->r[6]+f->r[5]*1u);
    /* 00478a4b */
    f->r[0]=(0x00000000u+f->r[6]+f->r[3]*1u);
    /* 00478a4e */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478a4f */
    f->r[2]=f->r[3];
    /* 00478a51 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478a52 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00478a54 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478a55 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478a56 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00478a5bu;f->boundary(0x00478a56u,0x00487450u,f);
    /* 00478a5b */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478a5c */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478a5d */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478a5e */
    f->r[4]-=4;MI_U32(f->r[4])=0x00478a63u;f->boundary(0x00478a5eu,0x00477dd0u,f);
    /* 00478a63 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00478a66 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00478a67 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00478a68 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00478a69 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00478a6a */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00478a6d */
    f->r[4]+=4;return;
mi_00478a6e:;
    /* 00478a6e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00478a6f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00478a70 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00478a71 */
    MI_U8((0x00bca30cu))=(uint8_t)(0x00000001u);
    /* 00478a78 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00478a79 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00478a7c */
    f->r[4]+=4;return;
mi_00478a7d:;
    /* 00478a7d */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 00478a7f */
    if(!(f->flags&MI_ZF))goto mi_00478a88;
    /* 00478a81 */
    f->r[6]=0x00000001u;
    /* 00478a86 */
    goto mi_00478aad;
mi_00478a88:;
    /* 00478a88 */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 00478a8b */
    if(!(f->flags&MI_ZF))goto mi_00478a94;
    /* 00478a8d */
    f->r[6]=0x00000002u;
    /* 00478a92 */
    goto mi_00478aad;
mi_00478a94:;
    /* 00478a94 */
    a=f->r[2];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 00478a97 */
    if(!(f->flags&MI_ZF))goto mi_00478aa0;
    /* 00478a99 */
    f->r[6]=0x00000003u;
    /* 00478a9e */
    goto mi_00478aad;
mi_00478aa0:;
    /* 00478aa0 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 00478aa2 */
    a=f->r[2];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 00478aa5 */
    t=(!(f->flags&MI_ZF))?1u:0u;f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00478aa8 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00478aab */
    f->r[6]=f->r[1];
mi_00478aad:;
    /* 00478aad */
    f->r[1]=MI_U32((0x00605a30u));
    /* 00478ab3 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 00478ab4 */
    a=f->r[1];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 00478ab7 */
    if(!(f->flags&MI_ZF))goto mi_00478abe;
    /* 00478ab9 */
    f->r[6]=0x00000001u;
mi_00478abe:;
    /* 00478abe */
    a=f->r[0];b=0x00000074u;t=mi_alu(f,a,b,32,1);
    /* 00478ac1 */
    if(!(f->flags&MI_ZF))goto mi_00478ae1;
    /* 00478ac3 */
    f->r[2]=(0x00000000u+f->r[6]+f->r[5]*1u);
    /* 00478ac6 */
    f->r[0]=(0x00000000u+f->r[6]+f->r[3]*1u);
    /* 00478ac9 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478aca */
    a=f->r[5];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[5]=t;
    /* 00478acc */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478acd */
    a=f->r[3];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 00478acf */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478ad0 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478ad1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00478ad6u;f->boundary(0x00478ad1u,0x00478d40u,f);
    /* 00478ad6 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00478ad9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00478ada */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00478adb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00478adc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00478add */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00478ae0 */
    f->r[4]+=4;return;
mi_00478ae1:;
    /* 00478ae1 */
    a=f->r[0];b=0x00000075u;t=mi_alu(f,a,b,32,1);
    /* 00478ae4 */
    if(!(f->flags&MI_ZF))goto mi_00478b04;
    /* 00478ae6 */
    f->r[1]=(0x00000000u+f->r[6]+f->r[5]*1u);
    /* 00478ae9 */
    f->r[2]=(0x00000000u+f->r[6]+f->r[3]*1u);
    /* 00478aec */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478aed */
    a=f->r[5];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[5]=t;
    /* 00478aef */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478af0 */
    a=f->r[3];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 00478af2 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478af3 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478af4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00478af9u;f->boundary(0x00478af4u,0x00478e10u,f);
    /* 00478af9 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00478afc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00478afd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00478afe */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00478aff */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00478b00 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00478b03 */
    f->r[4]+=4;return;
mi_00478b04:;
    /* 00478b04 */
    a=f->r[0];b=0x00000076u;t=mi_alu(f,a,b,32,1);
    /* 00478b07 */
    if(!(f->flags&MI_ZF))goto mi_00478b27;
    /* 00478b09 */
    f->r[0]=(0x00000000u+f->r[6]+f->r[5]*1u);
    /* 00478b0c */
    f->r[1]=(0x00000000u+f->r[6]+f->r[3]*1u);
    /* 00478b0f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478b10 */
    a=f->r[5];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[5]=t;
    /* 00478b12 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478b13 */
    a=f->r[3];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 00478b15 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478b16 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478b17 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00478b1cu;f->boundary(0x00478b17u,0x00478ea0u,f);
    /* 00478b1c */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00478b1f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00478b20 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00478b21 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00478b22 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00478b23 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00478b26 */
    f->r[4]+=4;return;
mi_00478b27:;
    /* 00478b27 */
    a=f->r[0];b=0x00000078u;t=mi_alu(f,a,b,32,1);
    /* 00478b2a */
    if(!(f->flags&MI_ZF))goto mi_00478b4a;
    /* 00478b2c */
    f->r[2]=(0x00000000u+f->r[6]+f->r[5]*1u);
    /* 00478b2f */
    f->r[0]=(0x00000000u+f->r[6]+f->r[3]*1u);
    /* 00478b32 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478b33 */
    a=f->r[5];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[5]=t;
    /* 00478b35 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478b36 */
    a=f->r[3];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 00478b38 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478b39 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478b3a */
    f->r[4]-=4;MI_U32(f->r[4])=0x00478b3fu;f->boundary(0x00478b3au,0x00478f40u,f);
    /* 00478b3f */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00478b42 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00478b43 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00478b44 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00478b45 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00478b46 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00478b49 */
    f->r[4]+=4;return;
mi_00478b4a:;
    /* 00478b4a */
    a=f->r[0];b=0x000000a3u;t=mi_alu(f,a,b,32,1);
    /* 00478b4f */
    if(!(f->flags&MI_ZF))goto mi_00478b6f;
    /* 00478b51 */
    f->r[1]=(0x00000000u+f->r[6]+f->r[5]*1u);
    /* 00478b54 */
    f->r[2]=(0x00000000u+f->r[6]+f->r[3]*1u);
    /* 00478b57 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478b58 */
    a=f->r[5];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[5]=t;
    /* 00478b5a */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478b5b */
    a=f->r[3];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 00478b5d */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478b5e */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478b5f */
    f->r[4]-=4;MI_U32(f->r[4])=0x00478b64u;f->boundary(0x00478b5fu,0x00478fd0u,f);
    /* 00478b64 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00478b67 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00478b68 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00478b69 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00478b6a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00478b6b */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00478b6e */
    f->r[4]+=4;return;
mi_00478b6f:;
    /* 00478b6f */
    a=f->r[0];b=0x000000dfu;t=mi_alu(f,a,b,32,1);
    /* 00478b74 */
    if((f->flags&MI_ZF))goto mi_00478bc7;
    /* 00478b76 */
    a=f->r[0];b=0x000000a4u;t=mi_alu(f,a,b,32,1);
    /* 00478b7b */
    if(!(f->flags&MI_ZF))goto mi_00478b9b;
    /* 00478b7d */
    f->r[2]=(0x00000000u+f->r[6]+f->r[5]*1u);
    /* 00478b80 */
    f->r[0]=(0x00000000u+f->r[6]+f->r[3]*1u);
    /* 00478b83 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478b84 */
    a=f->r[5];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[5]=t;
    /* 00478b86 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478b87 */
    a=f->r[3];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 00478b89 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478b8a */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478b8b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00478b90u;f->boundary(0x00478b8bu,0x00478fd0u,f);
    /* 00478b90 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00478b93 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00478b94 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00478b95 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00478b96 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00478b97 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00478b9a */
    f->r[4]+=4;return;
mi_00478b9b:;
    /* 00478b9b */
    a=f->r[0];b=0x000000a5u;t=mi_alu(f,a,b,32,1);
    /* 00478ba0 */
    if(!(f->flags&MI_ZF))goto mi_00478bc0;
    /* 00478ba2 */
    f->r[1]=(0x00000000u+f->r[6]+f->r[5]*1u);
    /* 00478ba5 */
    f->r[2]=(0x00000000u+f->r[6]+f->r[3]*1u);
    /* 00478ba8 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478ba9 */
    a=f->r[5];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[5]=t;
    /* 00478bab */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478bac */
    a=f->r[3];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 00478bae */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478baf */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478bb0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00478bb5u;f->boundary(0x00478bb0u,0x00478fd0u,f);
    /* 00478bb5 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00478bb8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00478bb9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00478bba */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00478bbb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00478bbc */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00478bbf */
    f->r[4]+=4;return;
mi_00478bc0:;
    /* 00478bc0 */
    a=f->r[0];b=0x000000a6u;t=mi_alu(f,a,b,32,1);
    /* 00478bc5 */
    if(!(f->flags&MI_ZF))goto mi_00478be5;
mi_00478bc7:;
    /* 00478bc7 */
    f->r[0]=(0x00000000u+f->r[6]+f->r[5]*1u);
    /* 00478bca */
    f->r[1]=(0x00000000u+f->r[6]+f->r[3]*1u);
    /* 00478bcd */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478bce */
    a=f->r[5];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[5]=t;
    /* 00478bd0 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478bd1 */
    a=f->r[3];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 00478bd3 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478bd4 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478bd5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00478bdau;f->boundary(0x00478bd5u,0x00478fd0u,f);
    /* 00478bda */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00478bdd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00478bde */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00478bdf */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00478be0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00478be1 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00478be4 */
    f->r[4]+=4;return;
mi_00478be5:;
    /* 00478be5 */
    a=f->r[0];b=0x000000c6u;t=mi_alu(f,a,b,32,1);
    /* 00478bea */
    if(!(f->flags&MI_ZF))goto mi_00478c0a;
    /* 00478bec */
    f->r[2]=(0x00000000u+f->r[6]+f->r[5]*1u);
    /* 00478bef */
    f->r[0]=(0x00000000u+f->r[6]+f->r[3]*1u);
    /* 00478bf2 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478bf3 */
    a=f->r[5];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[5]=t;
    /* 00478bf5 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478bf6 */
    a=f->r[3];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 00478bf8 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478bf9 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478bfa */
    f->r[4]-=4;MI_U32(f->r[4])=0x00478bffu;f->boundary(0x00478bfau,0x00479060u,f);
    /* 00478bff */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00478c02 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00478c03 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00478c04 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00478c05 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00478c06 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00478c09 */
    f->r[4]+=4;return;
mi_00478c0a:;
    /* 00478c0a */
    a=f->r[0];b=0x000000c5u;t=mi_alu(f,a,b,32,1);
    /* 00478c0f */
    if(!(f->flags&MI_ZF))goto mi_00478c2f;
    /* 00478c11 */
    f->r[1]=(0x00000000u+f->r[6]+f->r[5]*1u);
    /* 00478c14 */
    f->r[2]=(0x00000000u+f->r[6]+f->r[3]*1u);
    /* 00478c17 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478c18 */
    a=f->r[5];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[5]=t;
    /* 00478c1a */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478c1b */
    a=f->r[3];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 00478c1d */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478c1e */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478c1f */
    f->r[4]-=4;MI_U32(f->r[4])=0x00478c24u;f->boundary(0x00478c1fu,0x00479170u,f);
    /* 00478c24 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00478c27 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00478c28 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00478c29 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00478c2a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00478c2b */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00478c2e */
    f->r[4]+=4;return;
mi_00478c2f:;
    /* 00478c2f */
    a=f->r[0];b=0x0000007au;t=mi_alu(f,a,b,32,1);
    /* 00478c32 */
    if(!(f->flags&MI_ZF))goto mi_00478c82;
    /* 00478c34 */
    f->r[7]=f->r[5];
    /* 00478c36 */
    f->r[0]=f->r[3];
    /* 00478c38 */
    a=f->r[7];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 00478c3a */
    f->r[1]=(0x00000001u+f->r[6]+f->r[5]*1u);
    /* 00478c3e */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00478c40 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478c42 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478c43 */
    f->r[2]=(0x00000001u+f->r[6]+f->r[3]*1u);
    /* 00478c47 */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00478c4b */
    f->r[1]=(0xffffffffu+f->r[7]);
    /* 00478c4e */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478c4f */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00478c50 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478c51 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478c52 */
    f->r[1]=0x00bca350u;
    /* 00478c57 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00478c5cu;f->boundary(0x00478c57u,0x00488e90u,f);
    /* 00478c5c */
    f->r[1]=MI_U32((0x00000024u+f->r[4]));
    /* 00478c60 */
    f->r[2]=(0x00000004u+f->r[6]+f->r[5]*1u);
    /* 00478c64 */
    f->r[0]=(0x00000004u+f->r[6]+f->r[3]*1u);
    /* 00478c68 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478c69 */
    a=f->r[7];b=0xfffffffcu;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 00478c6c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478c6d */
    a=f->r[1];b=0xfffffffcu;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00478c70 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478c71 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478c72 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00478c77u;f->boundary(0x00478c72u,0x004790e0u,f);
    /* 00478c77 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00478c7a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00478c7b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00478c7c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00478c7d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00478c7e */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00478c81 */
    f->r[4]+=4;return;
mi_00478c82:;
    /* 00478c82 */
    a=f->r[0];b=0x00000086u;t=mi_alu(f,a,b,32,1);
    /* 00478c87 */
    if(!(f->flags&MI_ZF))goto mi_00478ca7;
    /* 00478c89 */
    f->r[2]=(0x00000000u+f->r[6]+f->r[5]*1u);
    /* 00478c8c */
    f->r[0]=(0x00000000u+f->r[6]+f->r[3]*1u);
    /* 00478c8f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478c90 */
    a=f->r[5];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[5]=t;
    /* 00478c92 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478c93 */
    a=f->r[3];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 00478c95 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478c96 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478c97 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00478c9cu;f->boundary(0x00478c97u,0x00479200u,f);
    /* 00478c9c */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00478c9f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00478ca0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00478ca1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00478ca2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00478ca3 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00478ca6 */
    f->r[4]+=4;return;
mi_00478ca7:;
    /* 00478ca7 */
    a=f->r[0];b=0x00000087u;t=mi_alu(f,a,b,32,1);
    /* 00478cac */
    if(!(f->flags&MI_ZF))goto mi_00478ccc;
    /* 00478cae */
    f->r[1]=(0x00000000u+f->r[6]+f->r[5]*1u);
    /* 00478cb1 */
    f->r[2]=(0x00000000u+f->r[6]+f->r[3]*1u);
    /* 00478cb4 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478cb5 */
    a=f->r[5];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[5]=t;
    /* 00478cb7 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478cb8 */
    a=f->r[3];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 00478cba */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478cbb */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478cbc */
    f->r[4]-=4;MI_U32(f->r[4])=0x00478cc1u;f->boundary(0x00478cbcu,0x00479340u,f);
    /* 00478cc1 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00478cc4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00478cc5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00478cc6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00478cc7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00478cc8 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00478ccb */
    f->r[4]+=4;return;
mi_00478ccc:;
    /* 00478ccc */
    a=f->r[0];b=0x0000007du;t=mi_alu(f,a,b,32,1);
    /* 00478ccf */
    if(!(f->flags&MI_ZF))goto mi_00478cef;
    /* 00478cd1 */
    f->r[0]=(0x00000000u+f->r[6]+f->r[5]*1u);
    /* 00478cd4 */
    f->r[1]=(0x00000000u+f->r[6]+f->r[3]*1u);
    /* 00478cd7 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478cd8 */
    a=f->r[5];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[5]=t;
    /* 00478cda */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478cdb */
    a=f->r[3];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 00478cdd */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478cde */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478cdf */
    f->r[4]-=4;MI_U32(f->r[4])=0x00478ce4u;f->boundary(0x00478cdfu,0x004792b0u,f);
    /* 00478ce4 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00478ce7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00478ce8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00478ce9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00478cea */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00478ceb */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00478cee */
    f->r[4]+=4;return;
mi_00478cef:;
    /* 00478cef */
    a=f->r[0];b=0x0000007bu;t=mi_alu(f,a,b,32,1);
    /* 00478cf2 */
    if(!(f->flags&MI_ZF))goto mi_00478cfe;
    /* 00478cf4 */
    f->r[2]=(0x00000000u+f->r[6]+f->r[5]*1u);
    /* 00478cf7 */
    f->r[0]=(0x00000000u+f->r[6]+f->r[3]*1u);
    /* 00478cfa */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478cfb */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478cfc */
    goto mi_00478d1c;
mi_00478cfe:;
    /* 00478cfe */
    a=f->r[0];b=0x0000007cu;t=mi_alu(f,a,b,32,1);
    /* 00478d01 */
    if(!(f->flags&MI_ZF))goto mi_00478d0d;
    /* 00478d03 */
    f->r[1]=(0x00000000u+f->r[6]+f->r[5]*1u);
    /* 00478d06 */
    f->r[2]=(0x00000000u+f->r[6]+f->r[3]*1u);
    /* 00478d09 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478d0a */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478d0b */
    goto mi_00478d1c;
mi_00478d0d:;
    /* 00478d0d */
    a=f->r[0];b=0x00000081u;t=mi_alu(f,a,b,32,1);
    /* 00478d12 */
    if(!(f->flags&MI_ZF))goto mi_00478d2a;
    /* 00478d14 */
    f->r[0]=(0x00000000u+f->r[6]+f->r[5]*1u);
    /* 00478d17 */
    f->r[1]=(0x00000000u+f->r[6]+f->r[3]*1u);
    /* 00478d1a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478d1b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
mi_00478d1c:;
    /* 00478d1c */
    a=f->r[5];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[5]=t;
    /* 00478d1e */
    a=f->r[3];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 00478d20 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478d21 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00478d22 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00478d27u;f->boundary(0x00478d22u,0x00479510u,f);
    /* 00478d27 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00478d2a:;
    /* 00478d2a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00478d2b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00478d2c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00478d2d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00478d2e */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00478d31 */
    f->r[4]+=4;return;
}
