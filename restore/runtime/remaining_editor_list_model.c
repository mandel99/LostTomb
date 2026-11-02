/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "remaining_editor_list_actions.h"

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
static __inline uint32_t mi_alu(struct restore_remaining_editor_frame *f,uint32_t a,uint32_t b,uint32_t width,uint32_t operation)
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
static __inline uint32_t mi_shift(struct restore_remaining_editor_frame *f,uint32_t a,uint32_t count,uint32_t width,uint32_t arithmetic)
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
static __inline uint32_t mi_imul(struct restore_remaining_editor_frame *f,uint32_t a,uint32_t b)
{
 int64_t product=(int64_t)(int32_t)a*(int32_t)b;uint32_t result=(uint32_t)product;
 f->flags&=~(MI_CF|MI_OF);if(product!=(int64_t)(int32_t)result)f->flags|=MI_CF|MI_OF;
 return result;
}
static __inline int mi_signed_less(uint32_t flags){return ((flags&MI_SF)!=0)^((flags&MI_OF)!=0);}

void restore_remaining_editor_C_0040c8a0(struct restore_remaining_editor_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0040c8a0 */
    MI_U8((0x00e38ddau))=(uint8_t)(0x0000004bu);
    /* 0040c8a7 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 0040c8ae */
    f->r[4]+=4;return;
}

void restore_remaining_editor_C_0040c920(struct restore_remaining_editor_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0040c920 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 0040c923 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00784de4u));
    /* 0040c92a */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040c92b */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040c92c */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040c92d */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040c92e */
    f->r[7]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00784de0u));
    /* 0040c935 */
    f->r[6]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00784de2u));
    /* 0040c93c */
    f->r[3]=0x00000001u;
    /* 0040c941 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 0040c945 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[6]);
    /* 0040c949 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0040c94d */
    MI_U32((0x006057f4u))=(uint32_t)(0x00000000u);
    /* 0040c957 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 0040c95b */
    f->r[5]=f->r[3];
    /* 0040c95d */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040c962u;f->boundary(0x0040c95du,0x00426b30u,f);
    /* 0040c962 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 0040c964 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0040cac6;
    /* 0040c96a */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(0x006057f8u);
    /* 0040c972 */
    f->r[3]=0x005d19d8u;
    /* 0040c977 */
    goto mi_0040c981;
mi_0040c979:;
    /* 0040c979 */
    f->r[6]=MI_U32((0x0000001cu+f->r[4]));
    /* 0040c97d */
    f->r[7]=MI_U32((0x00000020u+f->r[4]));
mi_0040c981:;
    /* 0040c981 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040c982 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040c987u;f->boundary(0x0040c982u,0x0040c8b0u,f);
    /* 0040c987 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040c98a */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0040c98c */
    if((f->flags&MI_ZF))goto mi_0040cab1;
    /* 0040c992 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 0040c995 */
    f->r[1]=f->r[3];
    /* 0040c997 */
    f->r[2]=f->r[4];
    /* 0040c999 */
    f->r[0]=MI_U32((0x00000000u+f->r[1]));
    /* 0040c99b */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[0]);
    /* 0040c99d */
    f->r[0]=MI_U32((0x00000004u+f->r[1]));
    /* 0040c9a0 */
    MI_U32((0x00000004u+f->r[2]))=(uint32_t)(f->r[0]);
    /* 0040c9a3 */
    f->r[0]=MI_U32((0x00000008u+f->r[1]));
    /* 0040c9a6 */
    MI_U32((0x00000008u+f->r[2]))=(uint32_t)(f->r[0]);
    /* 0040c9a9 */
    f->r[1]=MI_U32((0x0000000cu+f->r[1]));
    /* 0040c9ac */
    MI_U32((0x0000000cu+f->r[2]))=(uint32_t)(f->r[1]);
    /* 0040c9af */
    f->r[2]=MI_U32((0x00784cbcu));
    /* 0040c9b5 */
    a=f->r[2];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0040c9bb */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040c9bc */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040c9c1u;f->boundary(0x0040c9bcu,0x0040cb40u,f);
    /* 0040c9c1 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040c9c4 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0040c9c6 */
    if((f->flags&MI_ZF))goto mi_0040cab1;
    /* 0040c9cc */
    f->r[0]=MI_U32((0x00d3b198u));
    /* 0040c9d1 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 0040c9d3 */
    a=f->r[0];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0040c9d6 */
    if((f->flags&MI_ZF))goto mi_0040c9df;
    /* 0040c9d8 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0040c9d9 */
    f->r[0]=f->r[7];
    /* 0040c9db */
    if(!(f->flags&MI_ZF))goto mi_0040c9e5;
    /* 0040c9dd */
    goto mi_0040c9e1;
mi_0040c9df:;
    /* 0040c9df */
    f->r[0]=f->r[6];
mi_0040c9e1:;
    /* 0040c9e1 */
    f->r[6]=MI_U32((0x00000014u+f->r[4]));
mi_0040c9e5:;
    /* 0040c9e5 */
    f->r[7]=(f->r[7]&0xffff0000u)|(((MI_U16((0x00000000u+f->r[3])))&0xffffu)<<0);
    /* 0040c9e8 */
    a=(f->r[7]&0xffffu);b=0x00000002u;t=mi_alu(f,a,b,16,1);
    /* 0040c9ec */
    if(!(f->flags&MI_ZF))goto mi_0040c9f9;
    /* 0040c9ee */
    a=MI_U16((0x00000002u+f->r[3]));b=0x00000005u;t=mi_alu(f,a,b,16,1);
    /* 0040c9f3 */
    if(!mi_signed_less(f->flags))goto mi_0040ca9b;
mi_0040c9f9:;
    /* 0040c9f9 */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0040c9fc */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x005d19c8u+f->r[0])))&0xffffu)<<0);
    /* 0040ca03 */
    a=(f->r[1]&0xffffu);b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 0040ca06 */
    if(!(f->flags&MI_ZF))goto mi_0040ca49;
    /* 0040ca08 */
    a=(f->r[1]&0xffffu);b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 0040ca0c */
    if((f->flags&MI_ZF))goto mi_0040ca1e;
    /* 0040ca0e */
    a=(f->r[1]&0xffffu);b=0x00000002u;t=mi_alu(f,a,b,16,1);
    /* 0040ca12 */
    if(!(f->flags&MI_ZF))goto mi_0040ca45;
    /* 0040ca14 */
    a=MI_U16((0x005d19cau+f->r[0]));b=0x00000005u;t=mi_alu(f,a,b,16,1);
    /* 0040ca1c */
    if(!mi_signed_less(f->flags))goto mi_0040ca2c;
mi_0040ca1e:;
    /* 0040ca1e */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x005d19cau+f->r[0])))&0xffffu)<<0);
    /* 0040ca25 */
    a=(f->r[2]&0xffffu);b=MI_U16((0x00000002u+f->r[3]));t=mi_alu(f,a,b,16,1);
    /* 0040ca29 */
    t=((f->flags&MI_ZF))?1u:0u;f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
mi_0040ca2c:;
    /* 0040ca2c */
    a=(f->r[1]&0xffffu);b=0x00000002u;t=mi_alu(f,a,b,16,1);
    /* 0040ca30 */
    if(!(f->flags&MI_ZF))goto mi_0040ca45;
    /* 0040ca32 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x005d19cau+f->r[0])))&0xffffu)<<0);
    /* 0040ca39 */
    a=(f->r[0]&0xffffu);b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 0040ca3d */
    if((f->flags&MI_ZF))goto mi_0040cab1;
    /* 0040ca3f */
    a=(f->r[0]&0xffffu);b=0x00000004u;t=mi_alu(f,a,b,16,1);
    /* 0040ca43 */
    if((f->flags&MI_ZF))goto mi_0040cab1;
mi_0040ca45:;
    /* 0040ca45 */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0040ca47 */
    if((f->flags&MI_ZF))goto mi_0040cab1;
mi_0040ca49:;
    /* 0040ca49 */
    a=f->r[6];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0040ca4c */
    f->r[0]=f->r[6];
    /* 0040ca4e */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x005d19c8u+f->r[0])))&0xffffu)<<0);
    /* 0040ca55 */
    a=(f->r[1]&0xffffu);b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 0040ca58 */
    if(!(f->flags&MI_ZF))goto mi_0040ca97;
    /* 0040ca5a */
    a=(f->r[1]&0xffffu);b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 0040ca5e */
    if((f->flags&MI_ZF))goto mi_0040ca70;
    /* 0040ca60 */
    a=(f->r[1]&0xffffu);b=0x00000002u;t=mi_alu(f,a,b,16,1);
    /* 0040ca64 */
    if(!(f->flags&MI_ZF))goto mi_0040ca97;
    /* 0040ca66 */
    a=MI_U16((0x005d19cau+f->r[0]));b=0x00000005u;t=mi_alu(f,a,b,16,1);
    /* 0040ca6e */
    if(!mi_signed_less(f->flags))goto mi_0040ca7e;
mi_0040ca70:;
    /* 0040ca70 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x005d19cau+f->r[0])))&0xffffu)<<0);
    /* 0040ca77 */
    a=(f->r[2]&0xffffu);b=MI_U16((0x00000002u+f->r[3]));t=mi_alu(f,a,b,16,1);
    /* 0040ca7b */
    t=((f->flags&MI_ZF))?1u:0u;f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
mi_0040ca7e:;
    /* 0040ca7e */
    a=(f->r[1]&0xffffu);b=0x00000002u;t=mi_alu(f,a,b,16,1);
    /* 0040ca82 */
    if(!(f->flags&MI_ZF))goto mi_0040ca97;
    /* 0040ca84 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x005d19cau+f->r[0])))&0xffffu)<<0);
    /* 0040ca8b */
    a=(f->r[0]&0xffffu);b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 0040ca8f */
    if((f->flags&MI_ZF))goto mi_0040cab1;
    /* 0040ca91 */
    a=(f->r[0]&0xffffu);b=0x00000004u;t=mi_alu(f,a,b,16,1);
    /* 0040ca95 */
    if((f->flags&MI_ZF))goto mi_0040cab1;
mi_0040ca97:;
    /* 0040ca97 */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0040ca99 */
    if((f->flags&MI_ZF))goto mi_0040cab1;
mi_0040ca9b:;
    /* 0040ca9b */
    f->r[0]=MI_U32((0x00000018u+f->r[4]));
    /* 0040ca9f */
    f->r[2]=MI_U32((0x00000010u+f->r[4]));
    /* 0040caa3 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0040caa4 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[5]);
    /* 0040caa6 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0040caa9 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0040caad */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[0]);
mi_0040cab1:;
    /* 0040cab1 */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 0040cab2 */
    a=f->r[3];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 0040cab5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040cabau;f->boundary(0x0040cab5u,0x00426b30u,f);
    /* 0040caba */
    a=f->r[5];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0040cabc */
    if(mi_signed_less(f->flags))goto mi_0040c979;
    /* 0040cac2 */
    f->r[3]=MI_U32((0x00000010u+f->r[4]));
mi_0040cac6:;
    /* 0040cac6 */
    f->r[0]=MI_U32((0x00e38e68u));
    /* 0040cacb */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0040cacc */
    if((f->flags&MI_ZF))goto mi_0040cae9;
    /* 0040cace */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0040cacf */
    if((f->flags&MI_ZF))goto mi_0040cadd;
    /* 0040cad1 */
    f->r[1]=0x0000012eu;
    /* 0040cad6 */
    f->r[6]=0x0000011cu;
    /* 0040cadb */
    goto mi_0040caf3;
mi_0040cadd:;
    /* 0040cadd */
    f->r[1]=0x000000beu;
    /* 0040cae2 */
    f->r[6]=0x000000c8u;
    /* 0040cae7 */
    goto mi_0040caf3;
mi_0040cae9:;
    /* 0040cae9 */
    f->r[1]=0x0000006eu;
    /* 0040caee */
    f->r[6]=0x0000008cu;
mi_0040caf3:;
    /* 0040caf3 */
    f->r[0]=(0x00000001u+f->r[3]);
    /* 0040caf6 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040caf8 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0040caf9 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0040cafb */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 0040cafd */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040cafe */
    f->r[0]=MI_U32((0x00d3b198u));
    /* 0040cb03 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040cb05 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040cb07 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040cb09 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040cb0b */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040cb0d */
    f->r[2]=(0x00784de0u+f->r[0]*2u);
    /* 0040cb14 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040cb16 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040cb17 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040cb19 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040cb1b */
    a=0x000000c6u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040cb20 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040cb21 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040cb22 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040cb23 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040cb28u;f->boundary(0x0040cb23u,0x00403c00u,f);
    /* 0040cb28 */
    a=f->r[4];b=0x0000003cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040cb2b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040cb2c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040cb2d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0040cb2e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040cb2f */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040cb32 */
    f->r[4]+=4;return;
}

void restore_remaining_editor_C_0040cbe0(struct restore_remaining_editor_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0040cbe0 */
    f->r[0]=MI_U32((0x00e38e68u));
    /* 0040cbe5 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0040cbe6 */
    if((f->flags&MI_ZF))goto mi_0040cc03;
    /* 0040cbe8 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0040cbe9 */
    if((f->flags&MI_ZF))goto mi_0040cbf7;
    /* 0040cbeb */
    f->r[0]=0x0000012eu;
    /* 0040cbf0 */
    f->r[1]=0x0000011cu;
    /* 0040cbf5 */
    goto mi_0040cc0d;
mi_0040cbf7:;
    /* 0040cbf7 */
    f->r[0]=0x000000beu;
    /* 0040cbfc */
    f->r[1]=0x000000c8u;
    /* 0040cc01 */
    goto mi_0040cc0d;
mi_0040cc03:;
    /* 0040cc03 */
    f->r[0]=0x0000006eu;
    /* 0040cc08 */
    f->r[1]=0x0000008cu;
mi_0040cc0d:;
    /* 0040cc0d */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040cc0f */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040cc11 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040cc13 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040cc15 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040cc17 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040cc19 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040cc1b */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040cc1d */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040cc1f */
    a=0x00784cbcu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040cc24 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040cc26 */
    a=0x00000137u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040cc2b */
    a=0x00000005u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040cc2d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040cc2e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040cc2f */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040cc34u;f->boundary(0x0040cc2fu,0x00403c00u,f);
    /* 0040cc34 */
    a=f->r[4];b=0x0000003cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040cc37 */
    f->r[4]+=4;return;
}

void restore_remaining_editor_C_0040cc40(struct restore_remaining_editor_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0040cc40 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040cc41 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040cc42 */
    f->r[6]=MI_U32((0x0000000cu+f->r[4]));
    /* 0040cc46 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040cc47 */
    f->r[7]=MI_U32((0x00605a3cu));
    /* 0040cc4d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0040cc4f */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0040cc51 */
    f->r[1]=0x00784de8u;
mi_0040cc56:;
    /* 0040cc56 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 0040cc58 */
    if((f->flags&MI_ZF))goto mi_0040cc6c;
    /* 0040cc5a */
    f->r[3]=MI_U32((0x00000000u+f->r[1]));
    /* 0040cc5c */
    a=f->r[7];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0040cc5f */
    if(!(f->flags&MI_ZF))goto mi_0040cc67;
    /* 0040cc61 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 0040cc63 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0040cc6c;
    /* 0040cc65 */
    goto mi_0040cc6b;
mi_0040cc67:;
    /* 0040cc67 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 0040cc69 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0040cc6c;
mi_0040cc6b:;
    /* 0040cc6b */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
mi_0040cc6c:;
    /* 0040cc6c */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0040cc6f */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0040cc70 */
    a=f->r[1];b=0x00784e78u;t=mi_alu(f,a,b,32,1);
    /* 0040cc76 */
    if(mi_signed_less(f->flags))goto mi_0040cc56;
    /* 0040cc78 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040cc79 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040cc7a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040cc7b */
    f->r[4]+=4;return;
}

void restore_remaining_editor_C_0040cc80(struct restore_remaining_editor_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0040cc80 */
    f->r[0]=MI_U32((0x00d3b198u));
    /* 0040cc85 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040cc86 */
    a=f->r[0];b=0x0000001cu;t=mi_alu(f,a,b,32,1);
    /* 0040cc89 */
    if(!(f->flags&MI_ZF))goto mi_0040cc8e;
    /* 0040cc8b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040cc8c */
    goto mi_0040cc94;
mi_0040cc8e:;
    /* 0040cc8e */
    a=f->r[0];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 0040cc91 */
    if(!(f->flags&MI_ZF))goto mi_0040cccf;
    /* 0040cc93 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
mi_0040cc94:;
    /* 0040cc94 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040cc99u;f->boundary(0x0040cc94u,0x00442870u,f);
    /* 0040cc99 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040cc9c */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0040cc9e */
    if((f->flags&MI_ZF))goto mi_0040ccde;
    /* 0040cca0 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
mi_0040cca2:;
    /* 0040cca2 */
    f->r[0]=MI_U32((0x00d3b198u));
    /* 0040cca7 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040cca8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040ccadu;f->boundary(0x0040cca8u,0x0040cc40u,f);
    /* 0040ccad */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040ccb0 */
    a=f->r[0];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 0040ccb3 */
    if(mi_signed_less(f->flags))goto mi_0040ccf8;
    /* 0040ccb5 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040ccb7 */
    a=0x0051f780u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040ccbc */
    a=0x0000001au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040ccbe */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040ccc3u;f->boundary(0x0040ccbeu,0x004264d0u,f);
    /* 0040ccc3 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040ccc6 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 0040cccd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040ccce */
    f->r[4]+=4;return;
mi_0040cccf:;
    /* 0040cccf */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040ccd0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040ccd5u;f->boundary(0x0040ccd0u,0x00442870u,f);
    /* 0040ccd5 */
    f->r[3]=(f->r[3]&0xffffff00u)|((((f->r[0]&0xffu))&0xffu)<<0);
    /* 0040ccd7 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040ccda */
    a=(f->r[3]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0040ccdc */
    if(!(f->flags&MI_ZF))goto mi_0040cca2;
mi_0040ccde:;
    /* 0040ccde */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040cce0 */
    a=0x0051f780u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040cce5 */
    a=0x0000001cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040cce7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040ccecu;f->boundary(0x0040cce7u,0x004264d0u,f);
    /* 0040ccec */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040ccef */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 0040ccf6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040ccf7 */
    f->r[4]+=4;return;
mi_0040ccf8:;
    /* 0040ccf8 */
    a=(f->r[3]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0040ccfa */
    if((f->flags&MI_ZF))goto mi_0040cd24;
    /* 0040ccfc */
    f->r[1]=MI_U32((0x00d3b198u));
    /* 0040cd02 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040cd04 */
    f->r[2]=(0x00784de8u+f->r[1]*4u);
    /* 0040cd0b */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040cd0c */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040cd0e */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040cd10 */
    a=0x00000004u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040cd12 */
    a=0x000000c8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040cd17 */
    a=0x0000012cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040cd1c */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040cd21u;f->boundary(0x0040cd1cu,0x00403790u,f);
    /* 0040cd21 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0040cd24:;
    /* 0040cd24 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 0040cd2b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040cd2c */
    f->r[4]+=4;return;
}
