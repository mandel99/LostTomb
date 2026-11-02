/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "road_paint_model.h"

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

void restore_C_0047b960(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0047b960 */
    f->r[0]=MI_U32((0x00bca2ecu));
    /* 0047b965 */
    f->r[1]=MI_U32((0x00bca2e8u));
    /* 0047b96b */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047b96c */
    f->r[6]=MI_U32((0x0000000cu+f->r[4]));
    /* 0047b970 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047b971 */
    f->r[7]=MI_U32((0x0000000cu+f->r[4]));
    /* 0047b975 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0047b977 */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0047b979 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047b97a */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047b97b */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047b980u;f->boundary(0x0047b97bu,0x0046f930u,f);
    /* 0047b980 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047b983 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0047b985 */
    if((f->flags&MI_ZF))goto mi_0047bad5;
    /* 0047b98b */
    f->r[2]=(0x00000000u+f->r[6]+f->r[6]*8u);
    /* 0047b98e */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047b98f */
    f->r[0]=(0x00000000u+f->r[6]+f->r[2]*2u);
    /* 0047b992 */
    f->r[2]=MI_U32((0x00bca2d8u));
    /* 0047b998 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0047b99b */
    f->r[0]=(0x00000000u+f->r[2]+f->r[0]*4u);
    /* 0047b99e */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0047b9a0 */
    f->r[1]=MI_U32((0x00af598cu+f->r[0]*4u));
    /* 0047b9a7 */
    a=((f->r[1]>>8)&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,3);
    /* 0047b9aa */
    if(!(f->flags&MI_ZF))goto mi_0047b9b2;
    /* 0047b9ac */
    a=MI_U32((0x00bc971cu));b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;MI_U32((0x00bc971cu))=(uint32_t)(t);
mi_0047b9b2:;
    /* 0047b9b2 */
    a=MI_U32((0x0000001cu+f->r[4]));b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0047b9b7 */
    if(!(f->flags&MI_ZF))goto mi_0047b9be;
    /* 0047b9b9 */
    a=((f->r[1]>>8)&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,4);f->r[1]=(f->r[1]&0xffff00ffu)|(((t)&0xffu)<<8);
    /* 0047b9bc */
    goto mi_0047b9c1;
mi_0047b9be:;
    /* 0047b9be */
    a=((f->r[1]>>8)&0xffu);b=0x000000feu;t=mi_alu(f,a,b,8,3);f->r[1]=(f->r[1]&0xffff00ffu)|(((t)&0xffu)<<8);
mi_0047b9c1:;
    /* 0047b9c1 */
    MI_U32((0x00af598cu+f->r[0]*4u))=(uint32_t)(f->r[1]);
    /* 0047b9c8 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00b285ccu+f->r[0])))&0xffu)<<0);
    /* 0047b9ce */
    a=(f->r[1]&0xffu);b=0x000000efu;t=mi_alu(f,a,b,8,3);f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0047b9d1 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 0047b9d2 */
    MI_U8((0x00b285ccu+f->r[0]))=(uint8_t)((f->r[1]&0xffu));
    /* 0047b9d8 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0047b9d9 */
    f->r[1]=0x00000003u;
    /* 0047b9de */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 0047b9e0 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0047b9e4 */
    if(!mi_signed_less(f->flags))goto mi_0047b9f0;
    /* 0047b9e6 */
    a=f->r[7];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0047b9e8 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 0047b9ec */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 0047b9ee */
    goto mi_0047ba02;
mi_0047b9f0:;
    /* 0047b9f0 */
    f->r[0]=MI_U32((0x00bca2d0u));
    /* 0047b9f5 */
    f->r[3]=(0x00000003u+f->r[7]);
    /* 0047b9f8 */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0047b9fa */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0047ba02;
    /* 0047b9fc */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0047b9fe */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
mi_0047ba02:;
    /* 0047ba02 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 0047ba04 */
    if(!mi_signed_less(f->flags))goto mi_0047ba0d;
    /* 0047ba06 */
    f->r[1]=(0x00000003u+f->r[6]);
    /* 0047ba09 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 0047ba0b */
    goto mi_0047ba1d;
mi_0047ba0d:;
    /* 0047ba0d */
    f->r[0]=MI_U32((0x00bca2d4u));
    /* 0047ba12 */
    f->r[3]=(0x00000003u+f->r[6]);
    /* 0047ba15 */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0047ba17 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0047ba1d;
    /* 0047ba19 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0047ba1b */
    f->r[1]=f->r[0];
mi_0047ba1d:;
    /* 0047ba1d */
    f->r[0]=(0x00000000u+f->r[6]+f->r[6]*8u);
    /* 0047ba20 */
    f->r[3]=(0x00000000u+f->r[1]+f->r[6]*1u);
    /* 0047ba23 */
    MI_U32((0x00bc9760u))=(uint32_t)(f->r[6]);
    /* 0047ba29 */
    f->r[0]=(0x00000000u+f->r[6]+f->r[0]*2u);
    /* 0047ba2c */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0047ba2f */
    f->r[0]=(0x00000000u+f->r[2]+f->r[0]*4u);
    /* 0047ba32 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0047ba34 */
    a=f->r[6];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 0047ba36 */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[0]);
    /* 0047ba3b */
    if(!mi_signed_less(f->flags))goto mi_0047bac5;
    /* 0047ba41 */
    f->r[1]=MI_U32((0x00000014u+f->r[4]));
    /* 0047ba45 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047ba46 */
    f->r[5]=(0x00000000u+f->r[1]+f->r[7]*1u);
mi_0047ba49:;
    /* 0047ba49 */
    a=f->r[7];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 0047ba4b */
    MI_U32((0x00bc9764u))=(uint32_t)(f->r[7]);
    /* 0047ba51 */
    if(!mi_signed_less(f->flags))goto mi_0047ba9b;
mi_0047ba53:;
    /* 0047ba53 */
    f->r[1]=MI_U32((0x00af598cu+f->r[0]*4u));
    /* 0047ba5a */
    a=((f->r[1]>>8)&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,3);
    /* 0047ba5d */
    if((f->flags&MI_ZF))goto mi_0047ba84;
    /* 0047ba5f */
    a=(f->r[1]&0xffu);b=0x0000000cu;t=mi_alu(f,a,b,8,3);
    /* 0047ba62 */
    if(!(f->flags&MI_ZF))goto mi_0047ba84;
    /* 0047ba64 */
    a=MI_U8((0x00b285ccu+f->r[0]));b=0x00000080u;t=mi_alu(f,a,b,8,3);
    /* 0047ba6b */
    if(!(f->flags&MI_ZF))goto mi_0047ba84;
    /* 0047ba6d */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047ba72u;f->boundary(0x0047ba6du,0x0047e870u,f);
    /* 0047ba72 */
    f->r[2]=MI_U32((0x0000001cu+f->r[4]));
    /* 0047ba76 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047ba77 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047ba7cu;f->boundary(0x0047ba77u,0x0047be30u,f);
    /* 0047ba7c */
    f->r[0]=MI_U32((0x00bc9768u));
    /* 0047ba81 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0047ba84:;
    /* 0047ba84 */
    f->r[1]=MI_U32((0x00bc9764u));
    /* 0047ba8a */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0047ba8b */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0047ba8c */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 0047ba8e */
    MI_U32((0x00bc9764u))=(uint32_t)(f->r[1]);
    /* 0047ba94 */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[0]);
    /* 0047ba99 */
    if(mi_signed_less(f->flags))goto mi_0047ba53;
mi_0047ba9b:;
    /* 0047ba9b */
    f->r[1]=MI_U32((0x00bc9760u));
    /* 0047baa1 */
    f->r[2]=MI_U32((0x00000018u+f->r[4]));
    /* 0047baa5 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0047baa6 */
    MI_U32((0x00bc9760u))=(uint32_t)(f->r[1]);
    /* 0047baac */
    f->r[1]=0x000000e4u;
    /* 0047bab1 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0047bab3 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0047bab5 */
    f->r[1]=MI_U32((0x00bc9760u));
    /* 0047babb */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 0047babd */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[0]);
    /* 0047bac2 */
    if(mi_signed_less(f->flags))goto mi_0047ba49;
    /* 0047bac4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
mi_0047bac5:;
    /* 0047bac5 */
    f->r[2]=MI_U32((0x00000014u+f->r[4]));
    /* 0047bac9 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047baca */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047bacb */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047bacc */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047bad1u;f->boundary(0x0047baccu,0x004746d0u,f);
    /* 0047bad1 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047bad4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
mi_0047bad5:;
    /* 0047bad5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0047bad6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0047bad7 */
    f->r[4]+=4;return;
}
