/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "canal_region_model.h"

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

void restore_C_0047bae0(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0047bae0 */
    f->r[0]=MI_U32((0x00000004u+f->r[4]));
    /* 0047bae4 */
    f->r[1]=MI_U32((0x0000000cu+f->r[4]));
    /* 0047bae8 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047bae9 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047baea */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047baeb */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0047baed */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047baee */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0047baf6;
    /* 0047baf0 */
    f->r[6]=f->r[0];
    /* 0047baf2 */
    f->r[5]=f->r[1];
    /* 0047baf4 */
    goto mi_0047bafe;
mi_0047baf6:;
    /* 0047baf6 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0047bafa */
    f->r[5]=f->r[0];
    /* 0047bafc */
    f->r[6]=f->r[1];
mi_0047bafe:;
    /* 0047bafe */
    f->r[1]=MI_U32((0x00000018u+f->r[4]));
    /* 0047bb02 */
    f->r[2]=MI_U32((0x00000020u+f->r[4]));
    /* 0047bb06 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0047bb08 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0047bb12;
    /* 0047bb0a */
    f->r[0]=f->r[1];
    /* 0047bb0c */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0047bb10 */
    goto mi_0047bb18;
mi_0047bb12:;
    /* 0047bb12 */
    f->r[0]=f->r[2];
    /* 0047bb14 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[1]);
mi_0047bb18:;
    /* 0047bb18 */
    f->r[2]=MI_U32((0x00000014u+f->r[4]));
    /* 0047bb1c */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0047bb1d */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 0047bb1e */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0047bb1f */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0047bb20 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[6]);
    /* 0047bb24 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0047bb28 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0047bb2a */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0047bb2c */
    if(!mi_signed_less(f->flags))goto mi_0047bb34;
    /* 0047bb2e */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0047bb32 */
    f->r[6]=f->r[2];
mi_0047bb34:;
    /* 0047bb34 */
    f->r[1]=MI_U32((0x00bca2d0u));
    /* 0047bb3a */
    a=f->r[5];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0047bb3c */
    if(mi_signed_less(f->flags))goto mi_0047bb41;
    /* 0047bb3e */
    f->r[5]=(0xffffffffu+f->r[1]);
mi_0047bb41:;
    /* 0047bb41 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0047bb43 */
    if(!mi_signed_less(f->flags))goto mi_0047bb47;
    /* 0047bb45 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_0047bb47:;
    /* 0047bb47 */
    f->r[1]=MI_U32((0x00bca2d4u));
    /* 0047bb4d */
    f->r[7]=MI_U32((0x00000014u+f->r[4]));
    /* 0047bb51 */
    a=f->r[7];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0047bb53 */
    if(mi_signed_less(f->flags))goto mi_0047bb5a;
    /* 0047bb55 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0047bb56 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[1]);
mi_0047bb5a:;
    /* 0047bb5a */
    f->r[3]=MI_U32((0x00bca2d8u));
    /* 0047bb60 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 0047bb63 */
    f->r[7]=f->r[5];
    /* 0047bb65 */
    MI_U32((0x00bc9760u))=(uint32_t)(f->r[0]);
    /* 0047bb6a */
    f->r[1]=(0x00000000u+f->r[0]+f->r[1]*2u);
    /* 0047bb6d */
    a=f->r[7];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 0047bb6f */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 0047bb70 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 0047bb73 */
    f->r[1]=(0x00000000u+f->r[3]+f->r[1]*4u);
    /* 0047bb76 */
    f->r[3]=MI_U32((0x00000014u+f->r[4]));
    /* 0047bb7a */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0047bb7c */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 0047bb7e */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[1]);
    /* 0047bb84 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0047bd25;
    /* 0047bb8a */
    f->r[3]=(f->r[3]&0xffffff00u)|(((0x00000040u)&0xffu)<<0);
mi_0047bb8c:;
    /* 0047bb8c */
    a=f->r[6];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 0047bb8e */
    MI_U32((0x00bc9764u))=(uint32_t)(f->r[6]);
    /* 0047bb94 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0047bcf0;
mi_0047bb9a:;
    /* 0047bb9a */
    f->r[0]=MI_U32((0x00af598cu+f->r[1]*4u));
    /* 0047bba1 */
    a=((f->r[0]>>8)&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,3);
    /* 0047bba4 */
    if((f->flags&MI_ZF))goto mi_0047bcd6;
    /* 0047bbaa */
    a=(f->r[0]&0xffu);b=0x0000000cu;t=mi_alu(f,a,b,8,3);
    /* 0047bbac */
    if(!(f->flags&MI_ZF))goto mi_0047bcd6;
    /* 0047bbb2 */
    a=MI_U8((0x00ae8e7cu+f->r[1]));b=0x0000000fu;t=mi_alu(f,a,b,8,1);
    /* 0047bbb9 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0047bcd6;
    /* 0047bbbf */
    f->r[1]=MI_U32((0x00b7482cu+f->r[1]*4u));
    /* 0047bbc6 */
    f->r[2]=MI_U32((0x00f56b84u));
    /* 0047bbcc */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0047bbce */
    a=f->r[1];b=0x00000030u;t=mi_alu(f,a,b,32,1);
    /* 0047bbd1 */
    if(mi_signed_less(f->flags))goto mi_0047bbda;
    /* 0047bbd3 */
    f->r[6]=0x00000030u;
    /* 0047bbd8 */
    goto mi_0047bbe7;
mi_0047bbda:;
    /* 0047bbda */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0047bbdc */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0047bbde */
    t=(!mi_signed_less(f->flags))?1u:0u;f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0047bbe1 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0047bbe2 */
    a=f->r[2];b=0x00000030u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0047bbe5 */
    f->r[6]=f->r[2];
mi_0047bbe7:;
    /* 0047bbe7 */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 0047bbeb */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0047bbed */
    if((f->flags&MI_ZF))goto mi_0047bbf4;
    /* 0047bbef */
    f->r[6]=0x00000030u;
mi_0047bbf4:;
    /* 0047bbf4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047bbf9u;f->boundary(0x0047bbf4u,0x0047e870u,f);
    /* 0047bbf9 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047bbfb */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047bbfd */
    a=0x00000010u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047bbff */
    a=0x005cc678u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047bc04 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047bc09u;f->boundary(0x0047bc04u,0x00484be0u,f);
    /* 0047bc09 */
    f->r[1]=MI_U32((0x00bc9768u));
    /* 0047bc0f */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047bc12 */
    a=MI_U8((0x00af598cu+f->r[1]*4u));b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0047bc19 */
    if((f->flags&MI_ZF))goto mi_0047bc6d;
    /* 0047bc1b */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00b285ccu+f->r[1])))&0xffu)<<0);
    /* 0047bc21 */
    f->r[0]=MI_U32((0x00bc9744u));
    /* 0047bc26 */
    a=(f->r[2]&0xffu);b=0x0000007fu;t=mi_alu(f,a,b,8,3);f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0047bc29 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0047bc2b */
    MI_U8((0x00b285ccu+f->r[1]))=(uint8_t)((f->r[2]&0xffu));
    /* 0047bc31 */
    if(!(f->flags&MI_ZF))goto mi_0047bc4c;
    /* 0047bc33 */
    f->r[0]=MI_U32((0x00af55fcu+f->r[1]*4u));
    /* 0047bc3a */
    f->r[0]=(f->r[0]&0xffffff00u)|((((~((f->r[0]&0xffu))))&0xffu)<<0);
    /* 0047bc3c */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 0047bc41 */
    a=f->r[0];t=a>>6u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>5u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 0047bc44 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 0047bc47 */
    MI_U32((0x00bc9748u))=(uint32_t)(f->r[0]);
mi_0047bc4c:;
    /* 0047bc4c */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047bc51u;f->boundary(0x0047bc4cu,0x0047b920u,f);
    /* 0047bc51 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0047bc53 */
    f->r[0]=MI_U32((0x00bc9748u));
    /* 0047bc58 */
    if((f->flags&MI_ZF))goto mi_0047bc5f;
    /* 0047bc5a */
    a=f->r[0];b=0x0000002au;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0047bc5d */
    goto mi_0047bc62;
mi_0047bc5f:;
    /* 0047bc5f */
    a=f->r[0];b=0x0000000fu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
mi_0047bc62:;
    /* 0047bc62 */
    f->r[1]=MI_U32((0x00bc9768u));
    /* 0047bc68 */
    MI_U32((0x00bc9748u))=(uint32_t)(f->r[0]);
mi_0047bc6d:;
    /* 0047bc6d */
    f->r[2]=MI_U32((0x00bc9740u));
    /* 0047bc73 */
    f->r[0]=MI_U32((0x00f56b84u));
    /* 0047bc78 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0047bc7a */
    f->r[2]=MI_U32((0x00bc9748u));
    /* 0047bc80 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0047bc82 */
    f->r[2]=MI_U32((0x00af598cu+f->r[1]*4u));
    /* 0047bc89 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0047bc8b */
    a=f->r[2];b=0x00010000u;t=mi_alu(f,a,b,32,3);
    /* 0047bc91 */
    if((f->flags&MI_ZF))goto mi_0047bca2;
    /* 0047bc93 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047bc94 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047bc99u;f->boundary(0x0047bc94u,0x0051c620u,f);
    /* 0047bc99 */
    f->r[1]=MI_U32((0x00bc9768u));
    /* 0047bc9f */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0047bca2:;
    /* 0047bca2 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00b285ccu+f->r[1])))&0xffu)<<0);
    /* 0047bca8 */
    f->r[6]=MI_U32((0x0000001cu+f->r[4]));
    /* 0047bcac */
    MI_U32((0x00b7482cu+f->r[1]*4u))=(uint32_t)(f->r[0]);
    /* 0047bcb3 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00b67d1cu+f->r[1])))&0xffu)<<0);
    /* 0047bcb9 */
    a=(f->r[2]&0xffu);b=0x000000f0u;t=mi_alu(f,a,b,8,3);f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0047bcbc */
    a=(f->r[0]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,4);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0047bcbe */
    MI_U8((0x00b285ccu+f->r[1]))=(uint8_t)((f->r[2]&0xffu));
    /* 0047bcc4 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00bc9744u)))&0xffu)<<0);
    /* 0047bcca */
    MI_U8((0x00b67d1cu+f->r[1]))=(uint8_t)((f->r[0]&0xffu));
    /* 0047bcd0 */
    MI_U8((0x00ae8e7cu+f->r[1]))=(uint8_t)((f->r[2]&0xffu));
mi_0047bcd6:;
    /* 0047bcd6 */
    f->r[0]=MI_U32((0x00bc9764u));
    /* 0047bcdb */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0047bcdc */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0047bcdd */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 0047bcdf */
    MI_U32((0x00bc9764u))=(uint32_t)(f->r[0]);
    /* 0047bce4 */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[1]);
    /* 0047bcea */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0047bb9a;
mi_0047bcf0:;
    /* 0047bcf0 */
    f->r[0]=MI_U32((0x00bc9760u));
    /* 0047bcf5 */
    f->r[2]=0x000000e4u;
    /* 0047bcfa */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0047bcfc */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0047bcfd */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0047bcff */
    f->r[2]=MI_U32((0x00000014u+f->r[4]));
    /* 0047bd03 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0047bd05 */
    MI_U32((0x00bc9760u))=(uint32_t)(f->r[0]);
    /* 0047bd0a */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[1]);
    /* 0047bd10 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0047bb8c;
    /* 0047bd16 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0047bd17 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0047bd18 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0047bd19 */
    MI_U32((0x00bca328u))=(uint32_t)(0x00000000u);
    /* 0047bd23 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0047bd24 */
    f->r[4]+=4;return;
mi_0047bd25:;
    /* 0047bd25 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0047bd26 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0047bd27 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0047bd28 */
    MI_U32((0x00bca328u))=(uint32_t)(f->r[2]);
    /* 0047bd2e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0047bd2f */
    f->r[4]+=4;return;
}
