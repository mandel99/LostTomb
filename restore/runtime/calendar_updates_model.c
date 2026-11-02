/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "calendar_updates.h"
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
static __inline uint32_t mi_alu(struct restore_calendar_frame *f,uint32_t a,uint32_t b,uint32_t width,uint32_t operation)
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
static __inline uint32_t mi_shift(struct restore_calendar_frame *f,uint32_t a,uint32_t count,uint32_t width,uint32_t arithmetic)
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
static __inline uint32_t mi_imul(struct restore_calendar_frame *f,uint32_t a,uint32_t b)
{
 int64_t product=(int64_t)(int32_t)a*(int32_t)b;uint32_t result=(uint32_t)product;
 f->flags&=~(MI_CF|MI_OF);if(product!=(int64_t)(int32_t)result)f->flags|=MI_CF|MI_OF;
 return result;
}
static __inline int mi_signed_less(uint32_t flags){return ((flags&MI_SF)!=0)^((flags&MI_OF)!=0);}


static __inline void mi_idiv32(struct restore_calendar_frame *f,uint32_t divisor_bits)
{
 int32_t divisor=(int32_t)divisor_bits;
 int64_t dividend=(int64_t)(int32_t)f->r[2]*INT64_C(4294967296)+(int64_t)f->r[0];
 int64_t quotient,remainder;
 /* Native #DE cases explicitly fail; no C division overflow or fake result. */
 if(!divisor||(dividend==INT64_MIN&&divisor==-1))abort();
 quotient=dividend/(int64_t)divisor;
 if(quotient<INT32_MIN||quotient>INT32_MAX)abort();
 remainder=dividend%(int64_t)divisor;
 f->r[0]=(uint32_t)(int32_t)quotient;f->r[2]=(uint32_t)(int32_t)remainder;
 /* IDIV leaves arithmetic flags undefined. Keep their previous frame bits. */
}

void restore_calendar_C_004599d0(struct restore_calendar_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004599d0 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004599d1 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004599d2 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004599d3 */
    f->r[5]=MI_U32((0x00eaac28u));
    /* 004599d9 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004599db */
    f->r[0]=f->r[5];
    /* 004599dd */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004599de */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004599e4 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004599e5 */
    f->r[1]=0x00000014u;
    /* 004599ea */
    MI_U32((0x00ea4490u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 004599f0 */
    MI_U32((0x00ea4494u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 004599f6 */
    MI_U32((0x00ea4498u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 004599fc */
    MI_U32((0x00ea449cu+f->r[0]))=(uint32_t)(f->r[3]);
    /* 00459a02 */
    MI_U32((0x00ea44c8u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 00459a08 */
    MI_U32((0x00ea44c4u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 00459a0e */
    MI_U32((0x00ea44d0u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 00459a14 */
    MI_U32((0x00ea44ccu+f->r[0]))=(uint32_t)(f->r[3]);
    /* 00459a1a */
    f->r[7]=(0x00ea3efcu+f->r[0]);
    /* 00459a20 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00459a22 */
    while(f->r[1]){MI_U32(f->r[7])=f->r[0];f->r[7]+=(f->flags&0x400u)?0xfffffffcu:4u;--f->r[1];}
    /* 00459a24 */
    f->r[1]=MI_U32((0x0078d790u));
    /* 00459a2a */
    f->r[0]=0x00000001u;
    /* 00459a2f */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00459a31 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00459a35 */
    if(mi_signed_less(f->flags))goto mi_00459b69;
    /* 00459a3b */
    f->r[6]=0x00799af2u;
mi_00459a40:;
    /* 00459a40 */
    a=MI_U8((0xffffffeeu+f->r[6]));b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 00459a44 */
    if(!(f->flags&MI_ZF))goto mi_00459b4c;
    /* 00459a4a */
    a=MI_U8((0xfffffff3u+f->r[6]));b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 00459a4d */
    if((f->flags&MI_ZF))goto mi_00459b4c;
    /* 00459a53 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00000000u+f->r[6])))&0xffffu)<<0);
    /* 00459a56 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 00459a58 */
    a=(f->r[0]&0xffffu);b=0x0000000eu;t=mi_alu(f,a,b,16,1);
    /* 00459a5c */
    f->r[0]=(uint32_t)(int32_t)(int16_t)(f->r[0]&0xffffu);
    /* 00459a5f */
    t=(!mi_signed_less(f->flags))?1u:0u;f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00459a62 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 00459a65 */
    f->r[7]=f->r[1];
    /* 00459a67 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00000076u+f->r[6])))&0xffu)<<0);
    /* 00459a6a */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*2u);
    /* 00459a6d */
    a=(f->r[1]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 00459a6f */
    f->r[0]=MI_U32((0x0070ce18u+f->r[0]*8u));
    /* 00459a76 */
    if((f->flags&MI_ZF))goto mi_00459a89;
    /* 00459a78 */
    a=0x0000006eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00459a7a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00459a7b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00459a80u;f->boundary(0x00459a7bu,0x004cde40u,f);
    /* 00459a80 */
    f->r[5]=MI_U32((0x00eaac28u));
    /* 00459a86 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00459a89:;
    /* 00459a89 */
    f->r[1]=f->r[5];
    /* 00459a8b */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[6]));
    /* 00459a8e */
    a=f->r[1];b=0x00001276u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 00459a94 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00459a96 */
    f->r[2]=(0x00ea3efcu+f->r[1]*4u);
    /* 00459a9d */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x0000000au+f->r[6]));
    /* 00459aa1 */
    a=MI_U32((0x00000000u+f->r[2]));b=f->r[1];t=mi_alu(f,a,b,32,0);MI_U32((0x00000000u+f->r[2]))=(uint32_t)(t);
    /* 00459aa3 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x0000003cu+f->r[6])))&0xffu)<<0);
    /* 00459aa6 */
    a=(f->r[2]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 00459aa8 */
    if((f->flags&MI_ZF))goto mi_00459afa;
    /* 00459aaa */
    a=f->r[7];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 00459aac */
    f->r[2]=f->r[5];
    /* 00459aae */
    if((f->flags&MI_ZF))goto mi_00459abe;
    /* 00459ab0 */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 00459ab6 */
    a=MI_U32((0x00ea4494u+f->r[2]));b=f->r[1];t=mi_alu(f,a,b,32,0);MI_U32((0x00ea4494u+f->r[2]))=(uint32_t)(t);
    /* 00459abc */
    goto mi_00459aca;
mi_00459abe:;
    /* 00459abe */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 00459ac4 */
    a=MI_U32((0x00ea4490u+f->r[2]));b=f->r[1];t=mi_alu(f,a,b,32,0);MI_U32((0x00ea4490u+f->r[2]))=(uint32_t)(t);
mi_00459aca:;
    /* 00459aca */
    a=f->r[1];b=f->r[0];t=mi_imul(f,a,b);f->r[1]=t;
    /* 00459acd */
    f->r[0]=MI_U32((0x00e39080u));
    /* 00459ad2 */
    f->r[0]=MI_U32((0x005d1974u+f->r[0]*4u));
    /* 00459ad9 */
    a=f->r[0];b=MI_U32((0x00ea1878u+f->r[2]));t=mi_imul(f,a,b);f->r[0]=t;
    /* 00459ae0 */
    a=f->r[0];b=f->r[1];t=mi_imul(f,a,b);f->r[0]=t;
    /* 00459ae3 */
    a=MI_U32((0x0000007au+f->r[6]));b=f->r[0];t=mi_alu(f,a,b,32,0);MI_U32((0x0000007au+f->r[6]))=(uint32_t)(t);
    /* 00459ae6 */
    a=f->r[7];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 00459ae8 */
    if((f->flags&MI_ZF))goto mi_00459af2;
    /* 00459aea */
    a=MI_U32((0x00ea44ccu+f->r[2]));b=f->r[1];t=mi_alu(f,a,b,32,0);MI_U32((0x00ea44ccu+f->r[2]))=(uint32_t)(t);
    /* 00459af0 */
    goto mi_00459b4c;
mi_00459af2:;
    /* 00459af2 */
    a=MI_U32((0x00ea44c4u+f->r[2]));b=f->r[1];t=mi_alu(f,a,b,32,0);MI_U32((0x00ea44c4u+f->r[2]))=(uint32_t)(t);
    /* 00459af8 */
    goto mi_00459b4c;
mi_00459afa:;
    /* 00459afa */
    a=f->r[7];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 00459afc */
    f->r[2]=f->r[5];
    /* 00459afe */
    if((f->flags&MI_ZF))goto mi_00459b27;
    /* 00459b00 */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 00459b06 */
    f->r[7]=MI_U32((0x00ea449cu+f->r[2]));
    /* 00459b0c */
    a=f->r[7];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 00459b0e */
    a=f->r[1];b=f->r[0];t=mi_imul(f,a,b);f->r[1]=t;
    /* 00459b11 */
    MI_U32((0x00ea449cu+f->r[2]))=(uint32_t)(f->r[7]);
    /* 00459b17 */
    f->r[0]=MI_U32((0x00ea44d0u+f->r[2]));
    /* 00459b1d */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00459b1f */
    MI_U32((0x00ea44d0u+f->r[2]))=(uint32_t)(f->r[0]);
    /* 00459b25 */
    goto mi_00459b4c;
mi_00459b27:;
    /* 00459b27 */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 00459b2d */
    f->r[7]=MI_U32((0x00ea4498u+f->r[2]));
    /* 00459b33 */
    a=f->r[7];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 00459b35 */
    a=f->r[1];b=f->r[0];t=mi_imul(f,a,b);f->r[1]=t;
    /* 00459b38 */
    MI_U32((0x00ea4498u+f->r[2]))=(uint32_t)(f->r[7]);
    /* 00459b3e */
    f->r[0]=MI_U32((0x00ea44c8u+f->r[2]));
    /* 00459b44 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00459b46 */
    MI_U32((0x00ea44c8u+f->r[2]))=(uint32_t)(f->r[0]);
mi_00459b4c:;
    /* 00459b4c */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 00459b50 */
    f->r[1]=MI_U32((0x0078d790u));
    /* 00459b56 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00459b57 */
    a=f->r[6];b=0x00000108u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00459b5d */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 00459b5f */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00459b63 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00459a40;
mi_00459b69:;
    /* 00459b69 */
    f->r[0]=f->r[5];
    /* 00459b6b */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 00459b71 */
    f->r[1]=MI_U32((0x00ea1878u+f->r[0]));
    /* 00459b77 */
    f->r[2]=MI_U32((0x00ea44ccu+f->r[0]));
    /* 00459b7d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00459b7e */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00459b7f */
    f->r[4]-=4;MI_U32(f->r[4])=0x00459b84u;f->boundary(0x00459b7fu,0x004cde40u,f);
    /* 00459b84 */
    f->r[1]=MI_U32((0x00e39080u));
    /* 00459b8a */
    f->r[2]=MI_U32((0x005d1974u+f->r[1]*4u));
    /* 00459b91 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00459b92 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00459b93 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00459b98u;f->boundary(0x00459b93u,0x004cde40u,f);
    /* 00459b98 */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 00459b9e */
    f->r[7]=f->r[0];
    /* 00459ba0 */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 00459ba6 */
    f->r[2]=MI_U32((0x00ea44b0u+f->r[1]));
    /* 00459bac */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00459bae */
    MI_U32((0x00ea44b0u+f->r[1]))=(uint32_t)(f->r[2]);
    /* 00459bb4 */
    f->r[0]=MI_U32((0x00ea1878u+f->r[1]));
    /* 00459bba */
    f->r[1]=MI_U32((0x00ea44c4u+f->r[1]));
    /* 00459bc0 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00459bc1 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00459bc2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00459bc7u;f->boundary(0x00459bc2u,0x004cde40u,f);
    /* 00459bc7 */
    f->r[2]=MI_U32((0x00e39080u));
    /* 00459bcd */
    f->r[1]=MI_U32((0x005d1974u+f->r[2]*4u));
    /* 00459bd4 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00459bd5 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00459bd6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00459bdbu;f->boundary(0x00459bd6u,0x004cde40u,f);
    /* 00459bdb */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 00459be1 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 00459be3 */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 00459be9 */
    f->r[6]=MI_U32((0x00ea44acu+f->r[1]));
    /* 00459bef */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00459bf1 */
    MI_U32((0x00ea44acu+f->r[1]))=(uint32_t)(f->r[6]);
    /* 00459bf7 */
    f->r[2]=MI_U32((0x00ea1878u+f->r[1]));
    /* 00459bfd */
    f->r[0]=MI_U32((0x00ea44d0u+f->r[1]));
    /* 00459c03 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00459c04 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00459c05 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00459c0au;f->boundary(0x00459c05u,0x004cde40u,f);
    /* 00459c0a */
    f->r[1]=MI_U32((0x00e39080u));
    /* 00459c10 */
    f->r[2]=MI_U32((0x005d1974u+f->r[1]*4u));
    /* 00459c17 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00459c18 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00459c19 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00459c1eu;f->boundary(0x00459c19u,0x004cde40u,f);
    /* 00459c1e */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 00459c24 */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 00459c2a */
    f->r[3]=MI_U32((0x00ea44b8u+f->r[1]));
    /* 00459c30 */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 00459c32 */
    MI_U32((0x00ea44b8u+f->r[1]))=(uint32_t)(f->r[3]);
    /* 00459c38 */
    f->r[0]=MI_U32((0x00ea1878u+f->r[1]));
    /* 00459c3e */
    f->r[1]=MI_U32((0x00ea44c8u+f->r[1]));
    /* 00459c44 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00459c45 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00459c46 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00459c4bu;f->boundary(0x00459c46u,0x004cde40u,f);
    /* 00459c4b */
    f->r[2]=MI_U32((0x00e39080u));
    /* 00459c51 */
    f->r[1]=MI_U32((0x005d1974u+f->r[2]*4u));
    /* 00459c58 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00459c59 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00459c5a */
    f->r[4]-=4;MI_U32(f->r[4])=0x00459c5fu;f->boundary(0x00459c5au,0x004cde40u,f);
    /* 00459c5f */
    f->r[6]=MI_U32((0x00eaac28u));
    /* 00459c65 */
    a=f->r[4];b=0x00000040u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00459c68 */
    f->r[1]=f->r[6];
    /* 00459c6a */
    a=f->r[6];b=0x000049d8u;t=mi_imul(f,a,b);f->r[6]=t;
    /* 00459c70 */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 00459c76 */
    f->r[2]=MI_U32((0x00ea44b4u+f->r[1]));
    /* 00459c7c */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00459c7e */
    MI_U32((0x00ea44b4u+f->r[1]))=(uint32_t)(f->r[2]);
    /* 00459c84 */
    f->r[5]=MI_U32((0x00ea187cu+f->r[1]));
    /* 00459c8a */
    a=f->r[5];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 00459c8c */
    MI_U32((0x00ea187cu+f->r[1]))=(uint32_t)(f->r[5]);
    /* 00459c92 */
    f->r[1]=MI_U32((0x00ea4490u+f->r[1]));
    /* 00459c98 */
    f->r[2]=MI_U32((0x00ea4498u+f->r[6]));
    /* 00459c9e */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00459ca0 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00459ca1 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00459ca2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00459ca7u;f->boundary(0x00459ca2u,0x004cde60u,f);
    /* 00459ca7 */
    MI_U32((0x00ea44a0u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 00459cad */
    f->r[0]=MI_U32((0x00ea4494u+f->r[6]));
    /* 00459cb3 */
    f->r[6]=MI_U32((0x00eaac28u));
    /* 00459cb9 */
    a=f->r[6];b=0x000049d8u;t=mi_imul(f,a,b);f->r[6]=t;
    /* 00459cbf */
    f->r[1]=MI_U32((0x00ea449cu+f->r[6]));
    /* 00459cc5 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00459cc7 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00459cc8 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00459cc9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00459cceu;f->boundary(0x00459cc9u,0x004cde60u,f);
    /* 00459cce */
    MI_U32((0x00ea44a4u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 00459cd4 */
    f->r[2]=MI_U32((0x00ea449cu+f->r[6]));
    /* 00459cda */
    f->r[5]=MI_U32((0x00ea4498u+f->r[6]));
    /* 00459ce0 */
    f->r[1]=MI_U32((0x00ea4490u+f->r[6]));
    /* 00459ce6 */
    f->r[0]=MI_U32((0x00ea4494u+f->r[6]));
    /* 00459cec */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00459cee */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00459cf0 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00459cf2 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00459cf4 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00459cf5 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00459cf6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00459cfbu;f->boundary(0x00459cf6u,0x004cde60u,f);
    /* 00459cfb */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 00459d01 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00459d04 */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 00459d0a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00459d0b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00459d0c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00459d0d */
    MI_U32((0x00ea44a8u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 00459d13 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00459d14 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 00459d15 */
    f->r[4]+=4;return;
}

void restore_calendar_C_0045a350(struct restore_calendar_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0045a350 */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 0045a356 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045a357 */
    f->r[0]=f->r[1];
    /* 0045a359 */
    f->r[6]=MI_U32((0x00784ddcu));
    /* 0045a35f */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0045a365 */
    a=MI_U16((0x00ea5ce8u+f->r[0]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 0045a36d */
    if((f->flags&MI_ZF))goto mi_0045a398;
    /* 0045a36f */
    f->r[0]=MI_U32((0x00ea5f9cu+f->r[0]));
    /* 0045a375 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045a376 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0045a37bu;f->boundary(0x0045a376u,0x004c5120u,f);
    /* 0045a37b */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0045a37e */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0045a381 */
    if(!(f->flags&MI_ZF))goto mi_0045a392;
    /* 0045a383 */
    f->r[0]=(0xfffffffbu+f->r[6]);
    /* 0045a386 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0045a388 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0045a38a */
    t=(mi_signed_less(f->flags))?1u:0u;f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0045a38d */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0045a38e */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 0045a390 */
    f->r[6]=f->r[1];
mi_0045a392:;
    /* 0045a392 */
    f->r[1]=MI_U32((0x00eaac28u));
mi_0045a398:;
    /* 0045a398 */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 0045a39e */
    f->r[0]=MI_U32((0x00ea187cu+f->r[1]));
    /* 0045a3a4 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0045a3a6 */
    if(!mi_signed_less(f->flags))goto mi_0045a3ed;
    /* 0045a3a8 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,0,a,32,1);f->r[0]=t;
    /* 0045a3aa */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045a3ab */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045a3ac */
    f->r[4]-=4;MI_U32(f->r[4])=0x0045a3b1u;f->boundary(0x0045a3acu,0x004cde40u,f);
    /* 0045a3b1 */
    f->r[1]=f->r[0];
    /* 0045a3b3 */
    f->r[0]=0x2aaaaaabu;
    /* 0045a3b8 */
    a=f->r[0];b=f->r[1];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 0045a3ba */
    a=f->r[2];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 0045a3bc */
    f->r[0]=f->r[2];
    /* 0045a3be */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0045a3c1 */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 0045a3c4 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0045a3c6 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 0045a3cb */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0045a3d1 */
    f->r[6]=MI_U32((0x00ea187cu+f->r[0]));
    /* 0045a3d7 */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 0045a3d9 */
    MI_U32((0x00ea187cu+f->r[0]))=(uint32_t)(f->r[6]);
    /* 0045a3df */
    f->r[1]=MI_U32((0x00ea44e4u+f->r[0]));
    /* 0045a3e5 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0045a3e7 */
    MI_U32((0x00ea44e4u+f->r[0]))=(uint32_t)(f->r[1]);
mi_0045a3ed:;
    /* 0045a3ed */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0045a3ee */
    f->r[4]+=4;return;
}

void restore_calendar_C_0045a5d0(struct restore_calendar_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0045a5d0 */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 0045a5d6 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045a5d7 */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 0045a5dd */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045a5de */
    f->r[0]=MI_U32((0x00ea5cc8u+f->r[1]));
    /* 0045a5e4 */
    MI_U32((0x00ea5cc4u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 0045a5ea */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0045a5ec */
    MI_U32((0x00ea5cc8u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 0045a5f2 */
    f->r[2]=MI_U32((0x00ea6020u+f->r[1]));
    /* 0045a5f8 */
    MI_U32((0x00ea601cu+f->r[1]))=(uint32_t)(f->r[2]);
    /* 0045a5fe */
    MI_U32((0x00ea6020u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 0045a604 */
    MI_U32((0x00ea5d58u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 0045a60a */
    f->r[7]=MI_U32((0x00ea44c0u+f->r[1]));
    /* 0045a610 */
    f->r[6]=MI_U32((0x00ea44d8u+f->r[1]));
    /* 0045a616 */
    f->r[2]=MI_U32((0x00ea601cu+f->r[1]));
    /* 0045a61c */
    a=f->r[7];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0045a61e */
    f->r[6]=MI_U32((0x00ea5cc4u+f->r[1]));
    /* 0045a624 */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0045a626 */
    f->r[2]=MI_U32((0x00ea44e8u+f->r[1]));
    /* 0045a62c */
    a=f->r[7];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0045a62e */
    f->r[6]=MI_U32((0x00ea4500u+f->r[1]));
    /* 0045a634 */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0045a636 */
    f->r[2]=MI_U32((0x00ea44f8u+f->r[1]));
    /* 0045a63c */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0045a63e */
    f->r[2]=MI_U32((0x00ea448cu+f->r[1]));
    /* 0045a644 */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0045a646 */
    f->r[2]=MI_U32((0x00ea44e0u+f->r[1]));
    /* 0045a64c */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0045a64e */
    f->r[2]=MI_U32((0x00ea5cbcu+f->r[1]));
    /* 0045a654 */
    a=f->r[2];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 0045a657 */
    if(!(f->flags&MI_ZF))goto mi_0045a66a;
    /* 0045a659 */
    MI_U32((0x00ea5d58u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 0045a65f */
    MI_U32((0x00ea5d5cu+f->r[1]))=(uint32_t)(f->r[0]);
    /* 0045a665 */
    goto mi_0045a762;
mi_0045a66a:;
    /* 0045a66a */
    a=MI_U32((0x00ea187cu+f->r[1]));b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0045a670 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0045a68e;
    /* 0045a672 */
    MI_U32((0x00ea5d58u+f->r[1]))=(uint32_t)(0x00000001u);
    /* 0045a67c */
    f->r[2]=MI_U32((0x00ea5d5cu+f->r[1]));
    /* 0045a682 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0045a683 */
    MI_U32((0x00ea5d5cu+f->r[1]))=(uint32_t)(f->r[2]);
    /* 0045a689 */
    goto mi_0045a762;
mi_0045a68e:;
    /* 0045a68e */
    a=f->r[7];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 0045a690 */
    MI_U32((0x00ea5d5cu+f->r[1]))=(uint32_t)(f->r[0]);
    /* 0045a696 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0045a6d0;
    /* 0045a698 */
    f->r[2]=MI_U32((0x00ea1894u+f->r[1]));
    /* 0045a69e */
    a=f->r[2];b=0x000007d1u;t=mi_alu(f,a,b,32,1);
    /* 0045a6a4 */
    if(mi_signed_less(f->flags))goto mi_0045a6b5;
    /* 0045a6a6 */
    MI_U32((0x00ea5d54u+f->r[1]))=(uint32_t)(0x000000c8u);
    /* 0045a6b0 */
    goto mi_0045a768;
mi_0045a6b5:;
    /* 0045a6b5 */
    a=f->r[2];b=0x000003e9u;t=mi_alu(f,a,b,32,1);
    /* 0045a6bb */
    if(mi_signed_less(f->flags))goto mi_0045a762;
    /* 0045a6c1 */
    MI_U32((0x00ea5d54u+f->r[1]))=(uint32_t)(0x00000064u);
    /* 0045a6cb */
    goto mi_0045a768;
mi_0045a6d0:;
    /* 0045a6d0 */
    f->r[0]=MI_U32((0x00ea1894u+f->r[1]));
    /* 0045a6d6 */
    a=f->r[0];b=0x00001389u;t=mi_alu(f,a,b,32,1);
    /* 0045a6db */
    if(mi_signed_less(f->flags))goto mi_0045a6e9;
    /* 0045a6dd */
    MI_U32((0x00ea5d54u+f->r[1]))=(uint32_t)(0x000001f4u);
    /* 0045a6e7 */
    goto mi_0045a73f;
mi_0045a6e9:;
    /* 0045a6e9 */
    a=f->r[0];b=0x00000bb9u;t=mi_alu(f,a,b,32,1);
    /* 0045a6ee */
    if(mi_signed_less(f->flags))goto mi_0045a6fc;
    /* 0045a6f0 */
    MI_U32((0x00ea5d54u+f->r[1]))=(uint32_t)(0x00000190u);
    /* 0045a6fa */
    goto mi_0045a73f;
mi_0045a6fc:;
    /* 0045a6fc */
    a=f->r[0];b=0x000007d1u;t=mi_alu(f,a,b,32,1);
    /* 0045a701 */
    if(mi_signed_less(f->flags))goto mi_0045a70f;
    /* 0045a703 */
    MI_U32((0x00ea5d54u+f->r[1]))=(uint32_t)(0x0000012cu);
    /* 0045a70d */
    goto mi_0045a73f;
mi_0045a70f:;
    /* 0045a70f */
    a=f->r[0];b=0x000003e9u;t=mi_alu(f,a,b,32,1);
    /* 0045a714 */
    if(mi_signed_less(f->flags))goto mi_0045a722;
    /* 0045a716 */
    MI_U32((0x00ea5d54u+f->r[1]))=(uint32_t)(0x000000e1u);
    /* 0045a720 */
    goto mi_0045a73f;
mi_0045a722:;
    /* 0045a722 */
    a=f->r[0];b=0x000001f5u;t=mi_alu(f,a,b,32,1);
    /* 0045a727 */
    if(mi_signed_less(f->flags))goto mi_0045a735;
    /* 0045a729 */
    MI_U32((0x00ea5d54u+f->r[1]))=(uint32_t)(0x00000096u);
    /* 0045a733 */
    goto mi_0045a73f;
mi_0045a735:;
    /* 0045a735 */
    MI_U32((0x00ea5d54u+f->r[1]))=(uint32_t)(0x00000032u);
mi_0045a73f:;
    /* 0045a73f */
    f->r[0]=f->r[7];
    /* 0045a741 */
    a=0x00000019u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045a743 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0045a745 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045a746 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0045a74bu;f->boundary(0x0045a746u,0x004cde40u,f);
    /* 0045a74b */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 0045a751 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0045a754 */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 0045a75a */
    a=f->r[0];b=MI_U32((0x00ea5d54u+f->r[1]));t=mi_alu(f,a,b,32,1);
    /* 0045a760 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0045a768;
mi_0045a762:;
    /* 0045a762 */
    MI_U32((0x00ea5d54u+f->r[1]))=(uint32_t)(f->r[0]);
mi_0045a768:;
    /* 0045a768 */
    f->r[2]=MI_U32((0x00ea5d54u+f->r[1]));
    /* 0045a76e */
    f->r[0]=MI_U32((0x00ea187cu+f->r[1]));
    /* 0045a774 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0045a776 */
    MI_U32((0x00ea187cu+f->r[1]))=(uint32_t)(f->r[0]);
    /* 0045a77c */
    f->r[0]=MI_U32((0x00ea5d54u+f->r[1]));
    /* 0045a782 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0045a784 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0045a789u;f->boundary(0x0045a784u,0x0045a7b0u,f);
    /* 0045a789 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 0045a78e */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0045a794 */
    f->r[1]=MI_U32((0x00ea187cu+f->r[0]));
    /* 0045a79a */
    MI_U32((0x00ea452cu+f->r[0]))=(uint32_t)(f->r[1]);
    /* 0045a7a0 */
    MI_U32((0x00ea4514u+f->r[0]))=(uint32_t)(f->r[7]);
    /* 0045a7a6 */
    MI_U32((0x00ea451cu+f->r[0]))=(uint32_t)(f->r[6]);
    /* 0045a7ac */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0045a7ad */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0045a7ae */
    f->r[4]+=4;return;
}

void restore_calendar_C_004e81d0(struct restore_calendar_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004e81d0 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 004e81d3 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 004e81d8 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00e38ea8u)))&0xffu)<<0);
    /* 004e81de */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004e81e4 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e81e5 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e81e6 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 004e81e8 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004e81ea */
    a=(f->r[1]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 004e81ed */
    MI_U32((0x00ea5fc0u+f->r[0]))=(uint32_t)(f->r[5]);
    /* 004e81f3 */
    MI_U8((0x0000000bu+f->r[4]))=(uint8_t)((f->r[3]&0xffu));
    /* 004e81f7 */
    if(!mi_signed_less(f->flags))goto mi_004e821c;
    /* 004e81f9 */
    a=MI_U32((0x00e38ebcu));b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004e81ff */
    if(!(f->flags&MI_ZF))goto mi_004e821c;
    /* 004e8201 */
    f->r[1]=0x0000004bu;
    /* 004e8206 */
    MI_U8((0x0000000bu+f->r[4]))=(uint8_t)(0x00000001u);
    /* 004e820b */
    MI_U32((0x00ea1888u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 004e8211 */
    MI_U32((0x00ea1884u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 004e8217 */
    goto mi_004e8579;
mi_004e821c:;
    /* 004e821c */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e821d */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e821e */
    f->r[7]=0x00000001u;
    /* 004e8223 */
    f->r[6]=0x00799ae5u;
mi_004e8228:;
    /* 004e8228 */
    a=MI_U8((0xfffffffbu+f->r[6]));b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004e822c */
    if(!(f->flags&MI_ZF))goto mi_004e8310;
    /* 004e8232 */
    a=MI_U8((0x00000000u+f->r[6]));b=0x00000000u;t=mi_alu(f,a,b,8,1);
    /* 004e8235 */
    if((f->flags&MI_ZF))goto mi_004e8310;
    /* 004e823b */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00000017u+f->r[6])))&0xffffu)<<0);
    /* 004e823f */
    a=(f->r[0]&0xffffu);b=(f->r[0]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 004e8242 */
    if((f->flags&MI_ZF))goto mi_004e8310;
    /* 004e8248 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00eaac2cu)))&0xffu)<<0);
    /* 004e824e */
    f->r[0]=(uint32_t)(int32_t)(int16_t)(f->r[0]&0xffffu);
    /* 004e8251 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e8252 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e8253 */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 004e8255 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e825au;f->boundary(0x004e8255u,0x004e7f00u,f);
    /* 004e825a */
    f->r[1]=MI_U32((0x000000e3u+f->r[6]));
    /* 004e8260 */
    f->r[0]=0x000003e8u;
    /* 004e8265 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e8268 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004e826a */
    if(!(f->flags&MI_ZF))goto mi_004e8292;
    /* 004e826c */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e826d */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e8272u;f->boundary(0x004e826du,0x00430060u,f);
    /* 004e8272 */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 004e8278 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e827b */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 004e8281 */
    f->r[1]=MI_U32((0x00ea5fc0u+f->r[2]));
    /* 004e8287 */
    f->r[0]=(0x00ea5fc0u+f->r[2]);
    /* 004e828d */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004e828e */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 004e8290 */
    goto mi_004e82a3;
mi_004e8292:;
    /* 004e8292 */
    a=MI_U32((0x000000f7u+f->r[6]));b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004e8298 */
    if(!(f->flags&MI_ZF))goto mi_004e82a3;
    /* 004e829a */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e829b */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e82a0u;f->boundary(0x004e829bu,0x00430040u,f);
    /* 004e82a0 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004e82a3:;
    /* 004e82a3 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000073u+f->r[6])))&0xffu)<<0);
    /* 004e82a6 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 004e82ae */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004e82b0 */
    if((f->flags&MI_ZF))goto mi_004e82ba;
    /* 004e82b2 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(0x00000019u);
mi_004e82ba:;
    /* 004e82ba */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00000069u+f->r[6])))&0xffu)<<0);
    /* 004e82bd */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004e82bf */
    if((f->flags&MI_ZF))goto mi_004e82c6;
    /* 004e82c1 */
    a=MI_U32((0x00000014u+f->r[4]));b=0x0000000fu;t=mi_alu(f,a,b,32,0);MI_U32((0x00000014u+f->r[4]))=(uint32_t)(t);
mi_004e82c6:;
    /* 004e82c6 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x0000007au+f->r[6])))&0xffu)<<0);
    /* 004e82c9 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004e82cb */
    if((f->flags&MI_ZF))goto mi_004e82d2;
    /* 004e82cd */
    a=MI_U32((0x00000014u+f->r[4]));b=0x0000000au;t=mi_alu(f,a,b,32,0);MI_U32((0x00000014u+f->r[4]))=(uint32_t)(t);
mi_004e82d2:;
    /* 004e82d2 */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004e82d4 */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_004e82db;
    /* 004e82d6 */
    a=MI_U32((0x00000014u+f->r[4]));b=0x00000005u;t=mi_alu(f,a,b,32,0);MI_U32((0x00000014u+f->r[4]))=(uint32_t)(t);
mi_004e82db:;
    /* 004e82db */
    a=(f->r[0]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 004e82dd */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_004e82e4;
    /* 004e82df */
    a=MI_U32((0x00000014u+f->r[4]));b=0x00000005u;t=mi_alu(f,a,b,32,0);MI_U32((0x00000014u+f->r[4]))=(uint32_t)(t);
mi_004e82e4:;
    /* 004e82e4 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004e82e6 */
    if((f->flags&MI_ZF))goto mi_004e82f1;
    /* 004e82e8 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004e82ea */
    if((f->flags&MI_ZF))goto mi_004e82f1;
    /* 004e82ec */
    a=MI_U32((0x00000014u+f->r[4]));b=0x00000028u;t=mi_alu(f,a,b,32,0);MI_U32((0x00000014u+f->r[4]))=(uint32_t)(t);
mi_004e82f1:;
    /* 004e82f1 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000017u+f->r[6]));
    /* 004e82f5 */
    p=(0x00000014u+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004e82f9 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004e82fd */
    p=(0x0056f7d4u);
    __asm { mov eax,p }
    __asm { fmul dword ptr [eax] }
    /* 004e8303 */
    p=(0x00000014u+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004e8307 */
    __asm { fmulp st(1),st(0) }
    /* 004e8309 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e830eu;f->boundary(0x004e8309u,0x0056133cu,f);
    /* 004e830e */
    a=f->r[5];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[5]=t;
mi_004e8310:;
    /* 004e8310 */
    a=f->r[6];b=0x00000108u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004e8316 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 004e8317 */
    a=f->r[6];b=0x0089b6ddu;t=mi_alu(f,a,b,32,1);
    /* 004e831d */
    if(mi_signed_less(f->flags))goto mi_004e8228;
    /* 004e8323 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e8324 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e8325 */
    MI_U8((0x00eaac2cu))=(uint8_t)(0x00000000u);
    /* 004e832c */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e8331u;f->boundary(0x004e832cu,0x004cde60u,f);
    /* 004e8331 */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 004e8337 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e833a */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 004e8340 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 004e8342 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 004e8344 */
    MI_U32((0x00ea1884u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 004e834a */
    f->r[5]=MI_U32((0x00ea1894u+f->r[1]));
    /* 004e8350 */
    f->r[2]=0x005e4b8cu;
mi_004e8355:;
    /* 004e8355 */
    a=f->r[5];b=MI_U32((0x00000000u+f->r[2]));t=mi_alu(f,a,b,32,1);
    /* 004e8357 */
    if(mi_signed_less(f->flags))goto mi_004e8367;
    /* 004e8359 */
    a=f->r[2];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004e835c */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 004e835d */
    a=f->r[2];b=0x005e4bdcu;t=mi_alu(f,a,b,32,1);
    /* 004e8363 */
    if(mi_signed_less(f->flags))goto mi_004e8355;
    /* 004e8365 */
    goto mi_004e836e;
mi_004e8367:;
    /* 004e8367 */
    f->r[7]=MI_U32((0x005e4b90u+f->r[6]*8u));
mi_004e836e:;
    /* 004e836e */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004e8370 */
    MI_U32((0x00ea1884u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 004e8376 */
    a=MI_U16((0x00ea5ce8u+f->r[1]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 004e837e */
    if((f->flags&MI_ZF))goto mi_004e83e4;
    /* 004e8380 */
    f->r[1]=MI_U32((0x00ea5f9cu+f->r[1]));
    /* 004e8386 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e8387 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e838cu;f->boundary(0x004e8387u,0x004c5120u,f);
    /* 004e838c */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e838f */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 004e8392 */
    if(!(f->flags&MI_ZF))goto mi_004e83e4;
    /* 004e8394 */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 004e839a */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e839c */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 004e83a2 */
    f->r[0]=MI_U32((0x00ea5f9cu+f->r[2]));
    /* 004e83a8 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e83a9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e83aeu;f->boundary(0x004e83a9u,0x004c52d0u,f);
    /* 004e83ae */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e83b1 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004e83b3 */
    if((f->flags&MI_ZF))goto mi_004e83e4;
    /* 004e83b5 */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 004e83bb */
    a=0x00000069u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e83bd */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 004e83c3 */
    f->r[2]=MI_U32((0x00ea1884u+f->r[1]));
    /* 004e83c9 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e83ca */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e83cfu;f->boundary(0x004e83cau,0x004cde40u,f);
    /* 004e83cf */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 004e83d5 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e83d8 */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 004e83de */
    MI_U32((0x00ea1884u+f->r[1]))=(uint32_t)(f->r[0]);
mi_004e83e4:;
    /* 004e83e4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e83e9u;f->boundary(0x004e83e4u,0x0042fbb0u,f);
    /* 004e83e9 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004e83eb */
    if((f->flags&MI_ZF))goto mi_004e83f7;
    /* 004e83ed */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e83ef */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e83f4u;f->boundary(0x004e83efu,0x0042fcf0u,f);
    /* 004e83f4 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004e83f7:;
    /* 004e83f7 */
    f->r[3]=MI_U32((0x00eaac28u));
    /* 004e83fd */
    f->r[1]=f->r[3];
    /* 004e83ff */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 004e8405 */
    f->r[0]=MI_U32((0x00ea1888u+f->r[1]));
    /* 004e840b */
    f->r[2]=MI_U32((0x00ea1884u+f->r[1]));
    /* 004e8411 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004e8413 */
    f->r[5]=f->r[0];
    /* 004e8415 */
    if(!mi_signed_less(f->flags))goto mi_004e84b0;
    /* 004e841b */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004e841e */
    MI_U32((0x00ea1888u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 004e8424 */
    f->r[2]=MI_U32((0x00ea1884u+f->r[1]));
    /* 004e842a */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004e842c */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e8434;
    /* 004e842e */
    MI_U32((0x00ea1888u+f->r[1]))=(uint32_t)(f->r[2]);
mi_004e8434:;
    /* 004e8434 */
    f->r[7]=MI_U32((0x00ea1888u+f->r[1]));
    /* 004e843a */
    f->r[0]=0x66666667u;
    /* 004e843f */
    a=f->r[0];b=f->r[7];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 004e8441 */
    f->r[6]=f->r[2];
    /* 004e8443 */
    f->r[0]=0x66666667u;
    /* 004e8448 */
    a=f->r[6];b=0x00000002u;t=mi_shift(f,a,b,32,1);f->r[6]=t;
    /* 004e844b */
    f->r[2]=f->r[6];
    /* 004e844d */
    a=f->r[2];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[2]=t;
    /* 004e8450 */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004e8452 */
    a=f->r[0];b=f->r[5];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 004e8454 */
    a=f->r[2];b=0x00000002u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 004e8457 */
    f->r[0]=f->r[2];
    /* 004e8459 */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 004e845c */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004e845e */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004e8460 */
    if((f->flags&MI_ZF))goto mi_004e854f;
    /* 004e8466 */
    a=MI_U32((0x00ea1894u+f->r[1]));b=0x0000012cu;t=mi_alu(f,a,b,32,1);
    /* 004e8470 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e854f;
    /* 004e8476 */
    a=f->r[7];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 004e8479 */
    if(mi_signed_less(f->flags))goto mi_004e854f;
    /* 004e847f */
    a=f->r[7];b=0x00000014u;t=mi_alu(f,a,b,32,1);
    /* 004e8482 */
    if(!mi_signed_less(f->flags))goto mi_004e848e;
    /* 004e8484 */
    f->r[0]=0x00000063u;
    /* 004e8489 */
    goto mi_004e853c;
mi_004e848e:;
    /* 004e848e */
    a=f->r[7];b=0x0000001eu;t=mi_alu(f,a,b,32,1);
    /* 004e8491 */
    if(!mi_signed_less(f->flags))goto mi_004e849d;
    /* 004e8493 */
    f->r[0]=0x00000064u;
    /* 004e8498 */
    goto mi_004e853c;
mi_004e849d:;
    /* 004e849d */
    a=f->r[7];b=0x00000028u;t=mi_alu(f,a,b,32,1);
    /* 004e84a0 */
    if(!mi_signed_less(f->flags))goto mi_004e854f;
    /* 004e84a6 */
    f->r[0]=0x00000065u;
    /* 004e84ab */
    goto mi_004e853c;
mi_004e84b0:;
    /* 004e84b0 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e854f;
    /* 004e84b6 */
    a=f->r[0];b=0xfffffffeu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004e84b9 */
    MI_U32((0x00ea1888u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 004e84bf */
    f->r[2]=MI_U32((0x00ea1884u+f->r[1]));
    /* 004e84c5 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004e84c7 */
    if(!mi_signed_less(f->flags))goto mi_004e84cf;
    /* 004e84c9 */
    MI_U32((0x00ea1888u+f->r[1]))=(uint32_t)(f->r[2]);
mi_004e84cf:;
    /* 004e84cf */
    f->r[7]=MI_U32((0x00ea1888u+f->r[1]));
    /* 004e84d5 */
    f->r[0]=0x66666667u;
    /* 004e84da */
    a=f->r[0];b=f->r[7];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 004e84dc */
    f->r[6]=f->r[2];
    /* 004e84de */
    f->r[0]=0x66666667u;
    /* 004e84e3 */
    a=f->r[6];b=0x00000002u;t=mi_shift(f,a,b,32,1);f->r[6]=t;
    /* 004e84e6 */
    f->r[2]=f->r[6];
    /* 004e84e8 */
    a=f->r[2];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[2]=t;
    /* 004e84eb */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004e84ed */
    a=f->r[0];b=f->r[5];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 004e84ef */
    a=f->r[2];b=0x00000002u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 004e84f2 */
    f->r[0]=f->r[2];
    /* 004e84f4 */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 004e84f7 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004e84f9 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004e84fb */
    if((f->flags&MI_ZF))goto mi_004e8509;
    /* 004e84fd */
    a=MI_U32((0x00ea1894u+f->r[1]));b=0x0000012cu;t=mi_alu(f,a,b,32,1);
    /* 004e8507 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e850e;
mi_004e8509:;
    /* 004e8509 */
    a=f->r[7];b=0x00000014u;t=mi_alu(f,a,b,32,1);
    /* 004e850c */
    if(!mi_signed_less(f->flags))goto mi_004e854f;
mi_004e850e:;
    /* 004e850e */
    a=f->r[7];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 004e8511 */
    if(!mi_signed_less(f->flags))goto mi_004e851a;
    /* 004e8513 */
    f->r[0]=0x0000005fu;
    /* 004e8518 */
    goto mi_004e853c;
mi_004e851a:;
    /* 004e851a */
    a=f->r[7];b=0x00000014u;t=mi_alu(f,a,b,32,1);
    /* 004e851d */
    if(!mi_signed_less(f->flags))goto mi_004e8526;
    /* 004e851f */
    f->r[0]=0x00000060u;
    /* 004e8524 */
    goto mi_004e853c;
mi_004e8526:;
    /* 004e8526 */
    a=f->r[7];b=0x0000001eu;t=mi_alu(f,a,b,32,1);
    /* 004e8529 */
    if(!mi_signed_less(f->flags))goto mi_004e8532;
    /* 004e852b */
    f->r[0]=0x00000061u;
    /* 004e8530 */
    goto mi_004e853c;
mi_004e8532:;
    /* 004e8532 */
    a=f->r[7];b=0x00000028u;t=mi_alu(f,a,b,32,1);
    /* 004e8535 */
    if(!mi_signed_less(f->flags))goto mi_004e854f;
    /* 004e8537 */
    f->r[0]=0x00000062u;
mi_004e853c:;
    /* 004e853c */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e853e */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e8540 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e8541 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e8546u;f->boundary(0x004e8541u,0x004e2970u,f);
    /* 004e8546 */
    f->r[3]=MI_U32((0x00eaac28u));
    /* 004e854c */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004e854f:;
    /* 004e854f */
    a=f->r[3];b=0x000049d8u;t=mi_imul(f,a,b);f->r[3]=t;
    /* 004e8555 */
    f->r[1]=MI_U32((0x00ea1888u+f->r[3]));
    /* 004e855b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004e855c */
    f->r[0]=(0x00ea1888u+f->r[3]);
    /* 004e8562 */
    a=f->r[1];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 004e8565 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e8566 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e856e;
    /* 004e8568 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(0x00000064u);
mi_004e856e:;
    /* 004e856e */
    a=MI_U32((0x00000000u+f->r[0]));b=0x00000000u;t=mi_alu(f,a,b,32,1);
    /* 004e8571 */
    if(!mi_signed_less(f->flags))goto mi_004e8579;
    /* 004e8573 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(0x00000000u);
mi_004e8579:;
    /* 004e8579 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004e857a */
    f->r[0]=0x00799bd0u;
    /* 004e857f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
mi_004e8580:;
    /* 004e8580 */
    a=MI_U8((0xffffff10u+f->r[0]));b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004e8587 */
    if(!(f->flags&MI_ZF))goto mi_004e85aa;
    /* 004e8589 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0xffffff15u+f->r[0])))&0xffu)<<0);
    /* 004e858f */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004e8591 */
    if((f->flags&MI_ZF))goto mi_004e85aa;
    /* 004e8593 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0xffffff97u+f->r[0])))&0xffu)<<0);
    /* 004e8596 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004e8598 */
    if((f->flags&MI_ZF))goto mi_004e85aa;
    /* 004e859a */
    f->r[1]=MI_U32((0x00000000u+f->r[0]));
    /* 004e859c */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004e859e */
    if((f->flags&MI_ZF))goto mi_004e85aa;
    /* 004e85a0 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004e85a1 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004e85a3 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 004e85a5 */
    if(!(f->flags&MI_ZF))goto mi_004e85aa;
    /* 004e85a7 */
    MI_U8((0xffffff97u+f->r[0]))=(uint8_t)((f->r[1]&0xffu));
mi_004e85aa:;
    /* 004e85aa */
    a=f->r[0];b=0x00000108u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004e85af */
    a=f->r[0];b=0x0089b7c8u;t=mi_alu(f,a,b,32,1);
    /* 004e85b4 */
    if(mi_signed_less(f->flags))goto mi_004e8580;
    /* 004e85b6 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000003u+f->r[4])))&0xffu)<<0);
    /* 004e85ba */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004e85bc */
    if(!(f->flags&MI_ZF))goto mi_004e85c8;
    /* 004e85be */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e85c3u;f->boundary(0x004e85beu,0x00430250u,f);
    /* 004e85c3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e85c8u;f->boundary(0x004e85c3u,0x004302b0u,f);
mi_004e85c8:;
    /* 004e85c8 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e85cb */
    f->r[4]+=4;return;
}

void restore_calendar_C_004e8840(struct restore_calendar_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004e8840 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 004e8843 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e8844 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e8845 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e8846 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e8847 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e884cu;f->boundary(0x004e8847u,0x004c38f0u,f);
    /* 004e884c */
    f->r[6]=f->r[0];
    /* 004e884e */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 004e8853 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e8854 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e8859u;f->boundary(0x004e8854u,0x00458f80u,f);
    /* 004e8859 */
    f->r[7]=MI_U32((0x00eaac28u));
    /* 004e885f */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e8862 */
    f->r[1]=f->r[7];
    /* 004e8864 */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 004e886a */
    f->r[1]=MI_U32((0x00ea1894u+f->r[1]));
    /* 004e8870 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004e8872 */
    if(!mi_signed_less(f->flags))goto mi_004e8886;
    /* 004e8874 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004e8876 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e8877 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e8878 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e887du;f->boundary(0x004e8878u,0x004e9350u,f);
    /* 004e887d */
    f->r[7]=MI_U32((0x00eaac28u));
    /* 004e8883 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004e8886:;
    /* 004e8886 */
    a=f->r[7];b=0x000049d8u;t=mi_imul(f,a,b);f->r[7]=t;
    /* 004e888c */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004e888e */
    f->r[0]=MI_U32((0x00ea44a8u+f->r[7]));
    /* 004e8894 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 004e8898 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004e889a */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 004e889e */
    if(!(f->flags&MI_ZF))goto mi_004e88a4;
    /* 004e88a0 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004e88a2 */
    goto mi_004e88c2;
mi_004e88a4:;
    /* 004e88a4 */
    f->r[1]=(0x0000000au+f->r[0]);
    /* 004e88a7 */
    f->r[0]=0x66666667u;
    /* 004e88ac */
    a=f->r[0];b=f->r[1];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 004e88ae */
    a=f->r[2];b=0x00000002u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 004e88b1 */
    f->r[0]=f->r[2];
    /* 004e88b3 */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 004e88b6 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004e88b8 */
    a=f->r[2];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 004e88bb */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e88c2;
    /* 004e88bd */
    f->r[2]=0x0000000au;
mi_004e88c2:;
    /* 004e88c2 */
    f->r[0]=MI_U32((0x00ea1878u+f->r[7]));
    /* 004e88c8 */
    f->r[5]=MI_U32((0x00ea447cu+f->r[7]));
    /* 004e88ce */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004e88d0 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 004e88d3 */
    f->r[2]=(0x00000000u+f->r[2]+f->r[1]*2u);
    /* 004e88d6 */
    f->r[1]=MI_U32((0x00ea4478u+f->r[7]));
    /* 004e88dc */
    f->r[0]=MI_U32((0x00ea1384u+f->r[2]*4u));
    /* 004e88e3 */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004e88e7 */
    f->r[0]=f->r[1];
    /* 004e88e9 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004e88eb */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004e88ed */
    f->r[6]=0xffffffffu;
    /* 004e88f2 */
    if((f->flags&MI_SF))goto mi_004e892f;
    /* 004e88f4 */
    a=f->r[0];b=0x00000007u;t=mi_alu(f,a,b,32,1);
    /* 004e88f7 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e8903;
    /* 004e88f9 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00000004u);
    /* 004e8901 */
    goto mi_004e893e;
mi_004e8903:;
    /* 004e8903 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 004e8906 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e8912;
    /* 004e8908 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00000003u);
    /* 004e8910 */
    goto mi_004e893e;
mi_004e8912:;
    /* 004e8912 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004e8915 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e8921;
    /* 004e8917 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00000002u);
    /* 004e891f */
    goto mi_004e893e;
mi_004e8921:;
    /* 004e8921 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004e8923 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e893e;
    /* 004e8925 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00000001u);
    /* 004e892d */
    goto mi_004e893e;
mi_004e892f:;
    /* 004e892f */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004e8930 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004e8932 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004e8934 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004e8938 */
    if(!(f->flags&MI_ZF))goto mi_004e893e;
    /* 004e893a */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[6]);
mi_004e893e:;
    /* 004e893e */
    MI_U32((0x00ea5ea8u+f->r[7]))=(uint32_t)(f->r[1]);
    /* 004e8944 */
    f->r[0]=MI_U32((0x00ea446cu+f->r[7]));
    /* 004e894a */
    a=f->r[0];b=0x00000019u;t=mi_alu(f,a,b,32,1);
    /* 004e894d */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e8959;
    /* 004e894f */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(0xfffffffdu);
    /* 004e8957 */
    goto mi_004e897f;
mi_004e8959:;
    /* 004e8959 */
    a=f->r[0];b=0x00000011u;t=mi_alu(f,a,b,32,1);
    /* 004e895c */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e8968;
    /* 004e895e */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(0xfffffffeu);
    /* 004e8966 */
    goto mi_004e897f;
mi_004e8968:;
    /* 004e8968 */
    a=f->r[0];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 004e896b */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e8973;
    /* 004e896d */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 004e8971 */
    goto mi_004e897f;
mi_004e8973:;
    /* 004e8973 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004e8975 */
    a=f->r[0];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 004e8978 */
    t=(mi_signed_less(f->flags))?1u:0u;f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004e897b */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[1]);
mi_004e897f:;
    /* 004e897f */
    MI_U32((0x00ea5da4u+f->r[7]))=(uint32_t)(f->r[0]);
    /* 004e8985 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00ea5efbu+f->r[7])))&0xffu)<<0);
    /* 004e898b */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004e898d */
    if((f->flags&MI_ZF))goto mi_004e89ad;
    /* 004e898f */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e8994u;f->boundary(0x004e898fu,0x004e8ec0u,f);
    /* 004e8994 */
    f->r[7]=MI_U32((0x00eaac28u));
    /* 004e899a */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004e899e */
    a=f->r[7];b=0x000049d8u;t=mi_imul(f,a,b);f->r[7]=t;
    /* 004e89a4 */
    MI_U8((0x00ea5efbu+f->r[7]))=(uint8_t)(0x00000000u);
    /* 004e89ab */
    goto mi_004e89b8;
mi_004e89ad:;
    /* 004e89ad */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[3]);
    /* 004e89b1 */
    MI_U8((0x00ea5efbu+f->r[7]))=(uint8_t)(0x00000001u);
mi_004e89b8:;
    /* 004e89b8 */
    f->r[5]=MI_U32((0x00e39080u));
    /* 004e89be */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[3]);
    /* 004e89c2 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 004e89c6 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 004e89c8 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 004e89cc */
    f->r[1]=0x00799bc4u;
mi_004e89d1:;
    /* 004e89d1 */
    a=MI_U8((0xffffff1cu+f->r[1]));b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004e89d8 */
    if(!(f->flags&MI_ZF))goto mi_004e8b50;
    /* 004e89de */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0xffffff21u+f->r[1])))&0xffu)<<0);
    /* 004e89e4 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004e89e6 */
    if((f->flags&MI_ZF))goto mi_004e8b50;
    /* 004e89ec */
    a=MI_U16((0xffffff38u+f->r[1]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 004e89f4 */
    if(!(f->flags&MI_ZF))goto mi_004e8a08;
    /* 004e89f6 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x005d199cu+f->r[5]*4u)))&0xffu)<<0);
    /* 004e89fd */
    a=(f->r[2]&0xffu);b=0x0000000au;t=mi_alu(f,a,b,8,0);f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004e8a00 */
    MI_U8((0xffffffdeu+f->r[1]))=(uint8_t)((f->r[2]&0xffu));
    /* 004e8a03 */
    goto mi_004e8b50;
mi_004e8a08:;
    /* 004e8a08 */
    f->r[2]=MI_U32((0x00ea1894u+f->r[7]));
    /* 004e8a0e */
    a=f->r[2];b=0x0000012cu;t=mi_alu(f,a,b,32,1);
    /* 004e8a14 */
    if(!mi_signed_less(f->flags))goto mi_004e8a58;
    /* 004e8a16 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004e8a18 */
    a=f->r[2];b=0x000000c8u;t=mi_alu(f,a,b,32,1);
    /* 004e8a1e */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 004e8a22 */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 004e8a26 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 004e8a2a */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 004e8a2e */
    if(!mi_signed_less(f->flags))goto mi_004e8a43;
    /* 004e8a30 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x005d199cu+f->r[5]*4u)))&0xffu)<<0);
    /* 004e8a37 */
    a=(f->r[0]&0xffu);b=0x0000000au;t=mi_alu(f,a,b,8,0);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004e8a39 */
    MI_U8((0xffffffdeu+f->r[1]))=(uint8_t)((f->r[0]&0xffu));
    /* 004e8a3c */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[3]);
    /* 004e8a3e */
    goto mi_004e8b50;
mi_004e8a43:;
    /* 004e8a43 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x005d199cu+f->r[5]*4u)))&0xffu)<<0);
    /* 004e8a4a */
    MI_U8((0xffffffdeu+f->r[1]))=(uint8_t)((f->r[2]&0xffu));
    /* 004e8a4d */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(0x00000000u);
    /* 004e8a53 */
    goto mi_004e8b50;
mi_004e8a58:;
    /* 004e8a58 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0xffffff2eu+f->r[1]));
    /* 004e8a5f */
    f->r[5]=MI_U32((0x00000000u+f->r[1]));
    /* 004e8a61 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((MI_U8((0x00000010u+f->r[4])))&0xffu)<<0);
    /* 004e8a65 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[2]*4u);
    /* 004e8a68 */
    f->r[2]=(0x00000000u+f->r[2]+f->r[0]*2u);
    /* 004e8a6b */
    f->r[0]=MI_U32((0x0070ce08u+f->r[2]*8u));
    /* 004e8a72 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00000014u+f->r[4])))&0xffu)<<0);
    /* 004e8a76 */
    a=f->r[5];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 004e8a78 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000028u+f->r[4])))&0xffu)<<0);
    /* 004e8a7c */
    a=(f->r[2]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,0);f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004e8a7e */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[5]);
    /* 004e8a80 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((MI_U8((0xffffffdeu+f->r[1])))&0xffu)<<0);
    /* 004e8a83 */
    a=(f->r[2]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,0);f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004e8a85 */
    a=(f->r[3]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,0);f->r[3]=(f->r[3]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004e8a87 */
    f->r[0]=MI_U32((0x00000020u+f->r[4]));
    /* 004e8a8b */
    MI_U8((0xffffffdeu+f->r[1]))=(uint8_t)((f->r[3]&0xffu));
    /* 004e8a8e */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004e8a8f */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0xffffff2eu+f->r[1]));
    /* 004e8a96 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004e8a9a */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004e8a9c */
    f->r[5]=(0x00000000u+f->r[2]+f->r[2]*4u);
    /* 004e8a9f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004e8aa1 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004e8aa5 */
    f->r[2]=(0x00000000u+f->r[2]+f->r[5]*2u);
    /* 004e8aa8 */
    f->r[5]=MI_U32((0x0070cdf4u+f->r[2]*8u));
    /* 004e8aaf */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 004e8ab1 */
    if((f->flags&MI_ZF))goto mi_004e8b1a;
    /* 004e8ab3 */
    f->r[5]=MI_U32((0x0000001cu+f->r[4]));
    /* 004e8ab7 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0xffffff9bu+f->r[1])))&0xffu)<<0);
    /* 004e8aba */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 004e8abb */
    a=(f->r[2]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 004e8abe */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[5]);
    /* 004e8ac2 */
    if((f->flags&MI_CF))goto mi_004e8ad0;
    /* 004e8ac4 */
    f->r[3]=0x00000002u;
    /* 004e8ac9 */
    MI_U8((0xffffffd8u+f->r[1]))=(uint8_t)((f->r[0]&0xffu));
    /* 004e8acc */
    a=f->r[6];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004e8ace */
    goto mi_004e8b30;
mi_004e8ad0:;
    /* 004e8ad0 */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004e8ad2 */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_004e8ae0;
    /* 004e8ad4 */
    f->r[3]=0x00000001u;
    /* 004e8ad9 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 004e8ada */
    MI_U8((0xffffffd8u+f->r[1]))=(uint8_t)(0x00000000u);
    /* 004e8ade */
    goto mi_004e8b30;
mi_004e8ae0:;
    /* 004e8ae0 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((MI_U8((0xffffffd8u+f->r[1])))&0xffu)<<0);
    /* 004e8ae3 */
    a=(f->r[3]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=(f->r[3]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004e8ae5 */
    f->r[2]=(f->r[2]&0xffffff00u)|((((f->r[3]&0xffu))&0xffu)<<0);
    /* 004e8ae7 */
    MI_U8((0xffffffd8u+f->r[1]))=(uint8_t)((f->r[3]&0xffu));
    /* 004e8aea */
    a=(f->r[2]&0xffu);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 004e8aed */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_004e8af3;
    /* 004e8aef */
    MI_U8((0xffffffd8u+f->r[1]))=(uint8_t)(0x00000003u);
mi_004e8af3:;
    /* 004e8af3 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0xffffffd8u+f->r[1])))&0xffu)<<0);
    /* 004e8af6 */
    a=(f->r[2]&0xffu);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 004e8af9 */
    if((f->flags&MI_CF))goto mi_004e8b05;
    /* 004e8afb */
    f->r[3]=0xfffffffdu;
    /* 004e8b00 */
    a=f->r[6];b=0x00000003u;t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 004e8b03 */
    goto mi_004e8b30;
mi_004e8b05:;
    /* 004e8b05 */
    a=(f->r[2]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 004e8b08 */
    if((f->flags&MI_CF))goto mi_004e8b14;
    /* 004e8b0a */
    f->r[3]=0xfffffffeu;
    /* 004e8b0f */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 004e8b12 */
    goto mi_004e8b30;
mi_004e8b14:;
    /* 004e8b14 */
    a=f->r[3];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[3]=t;
    /* 004e8b17 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 004e8b18 */
    goto mi_004e8b30;
mi_004e8b1a:;
    /* 004e8b1a */
    f->r[0]=MI_U32((0x0000002cu+f->r[4]));
    /* 004e8b1e */
    f->r[2]=MI_U32((0x00000024u+f->r[4]));
    /* 004e8b22 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004e8b24 */
    MI_U8((0xffffffd8u+f->r[1]))=(uint8_t)(0x00000000u);
    /* 004e8b28 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004e8b2c */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[2]);
mi_004e8b30:;
    /* 004e8b30 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0xffffffdeu+f->r[1])))&0xffu)<<0);
    /* 004e8b33 */
    a=(f->r[0]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,0);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004e8b35 */
    a=(f->r[2]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,0);f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004e8b37 */
    MI_U8((0xffffffdeu+f->r[1]))=(uint8_t)((f->r[2]&0xffu));
    /* 004e8b3a */
    if(!(f->flags&MI_SF))goto mi_004e8b40;
    /* 004e8b3c */
    MI_U8((0xffffffdeu+f->r[1]))=(uint8_t)(0x00000000u);
mi_004e8b40:;
    /* 004e8b40 */
    a=MI_U8((0xffffffdeu+f->r[1]));b=0x00000064u;t=mi_alu(f,a,b,8,1);
    /* 004e8b44 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e8b4a;
    /* 004e8b46 */
    MI_U8((0xffffffdeu+f->r[1]))=(uint8_t)(0x00000064u);
mi_004e8b4a:;
    /* 004e8b4a */
    f->r[5]=MI_U32((0x00e39080u));
mi_004e8b50:;
    /* 004e8b50 */
    a=f->r[1];b=0x00000108u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004e8b56 */
    a=f->r[1];b=0x0089b7bcu;t=mi_alu(f,a,b,32,1);
    /* 004e8b5c */
    if(mi_signed_less(f->flags))goto mi_004e89d1;
    /* 004e8b62 */
    f->r[0]=MI_U32((0x0000001cu+f->r[4]));
    /* 004e8b66 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e8b68 */
    if((f->flags&MI_ZF))goto mi_004e8b73;
    /* 004e8b6a */
    f->r[0]=f->r[6];
    /* 004e8b6c */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004e8b6d */
    a=MI_U32((0x0000001cu+f->r[4]));mi_idiv32(f,a);
    /* 004e8b71 */
    f->r[3]=f->r[0];
mi_004e8b73:;
    /* 004e8b73 */
    f->r[0]=MI_U32((0x00000020u+f->r[4]));
    /* 004e8b77 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e8b79 */
    if((f->flags&MI_ZF))goto mi_004e8b88;
    /* 004e8b7b */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 004e8b7f */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004e8b80 */
    a=MI_U32((0x00000020u+f->r[4]));mi_idiv32(f,a);
    /* 004e8b84 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[0]);
mi_004e8b88:;
    /* 004e8b88 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004e8b8a */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 004e8b8c */
    f->r[1]=0x00799ae5u;
mi_004e8b91:;
    /* 004e8b91 */
    a=MI_U8((0xfffffffbu+f->r[1]));b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004e8b95 */
    if(!(f->flags&MI_ZF))goto mi_004e8bad;
    /* 004e8b97 */
    a=MI_U8((0x00000000u+f->r[1]));b=0x00000000u;t=mi_alu(f,a,b,8,1);
    /* 004e8b9a */
    if((f->flags&MI_ZF))goto mi_004e8bad;
    /* 004e8b9c */
    a=MI_U16((0x00000017u+f->r[1]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 004e8ba1 */
    if((f->flags&MI_ZF))goto mi_004e8bad;
    /* 004e8ba3 */
    f->r[2]=(uint32_t)(int32_t)(int8_t)MI_U8((0x000000bdu+f->r[1]));
    /* 004e8baa */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 004e8bab */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
mi_004e8bad:;
    /* 004e8bad */
    a=f->r[1];b=0x00000108u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004e8bb3 */
    a=f->r[1];b=0x0089b6ddu;t=mi_alu(f,a,b,32,1);
    /* 004e8bb9 */
    if(mi_signed_less(f->flags))goto mi_004e8b91;
    /* 004e8bbb */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004e8bbd */
    if((f->flags&MI_ZF))goto mi_004e8c32;
    /* 004e8bbf */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004e8bc0 */
    a=f->r[6];mi_idiv32(f,a);
    /* 004e8bc2 */
    MI_U32((0x00ea1880u+f->r[7]))=(uint32_t)(f->r[0]);
    /* 004e8bc8 */
    a=MI_U16((0x00ea5ce8u+f->r[7]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 004e8bd0 */
    if((f->flags&MI_ZF))goto mi_004e8c3c;
    /* 004e8bd2 */
    f->r[0]=MI_U32((0x00ea5f9cu+f->r[7]));
    /* 004e8bd8 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e8bd9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e8bdeu;f->boundary(0x004e8bd9u,0x004c5120u,f);
    /* 004e8bde */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e8be1 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 004e8be4 */
    if(!(f->flags&MI_ZF))goto mi_004e8c3c;
    /* 004e8be6 */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 004e8bec */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e8bee */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 004e8bf4 */
    f->r[2]=MI_U32((0x00ea5f9cu+f->r[1]));
    /* 004e8bfa */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e8bfb */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e8c00u;f->boundary(0x004e8bfbu,0x004c52d0u,f);
    /* 004e8c00 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e8c03 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004e8c05 */
    if((f->flags&MI_ZF))goto mi_004e8c3c;
    /* 004e8c07 */
    f->r[5]=MI_U32((0x00eaac28u));
    /* 004e8c0d */
    f->r[0]=f->r[5];
    /* 004e8c0f */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004e8c15 */
    f->r[1]=(0x00ea1880u+f->r[0]);
    /* 004e8c1b */
    f->r[0]=MI_U32((0x00ea1880u+f->r[0]));
    /* 004e8c21 */
    a=f->r[0];b=0x0000000au;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004e8c24 */
    a=f->r[0];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 004e8c27 */
    if(mi_signed_less(f->flags))goto mi_004e8c2e;
    /* 004e8c29 */
    f->r[0]=0x00000064u;
mi_004e8c2e:;
    /* 004e8c2e */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 004e8c30 */
    goto mi_004e8c42;
mi_004e8c32:;
    /* 004e8c32 */
    MI_U32((0x00ea1880u+f->r[7]))=(uint32_t)(0x0000003cu);
mi_004e8c3c:;
    /* 004e8c3c */
    f->r[5]=MI_U32((0x00eaac28u));
mi_004e8c42:;
    /* 004e8c42 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004e8c44 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 004e8c46 */
    if(!mi_signed_less(f->flags))goto mi_004e8c5c;
    /* 004e8c48 */
    f->r[1]=f->r[5];
    /* 004e8c4a */
    f->r[0]=f->r[3];
    /* 004e8c4c */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 004e8c52 */
    MI_U32((0x00ea5db0u+f->r[1]))=(uint32_t)(0x00000001u);
mi_004e8c5c:;
    /* 004e8c5c */
    f->r[1]=MI_U32((0x00000014u+f->r[4]));
    /* 004e8c60 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004e8c62 */
    if(!mi_signed_less(f->flags))goto mi_004e8c78;
    /* 004e8c64 */
    f->r[2]=f->r[5];
    /* 004e8c66 */
    f->r[0]=f->r[1];
    /* 004e8c68 */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 004e8c6e */
    MI_U32((0x00ea5db0u+f->r[2]))=(uint32_t)(0x00000002u);
mi_004e8c78:;
    /* 004e8c78 */
    f->r[1]=MI_U32((0x00000028u+f->r[4]));
    /* 004e8c7c */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004e8c7e */
    if(!mi_signed_less(f->flags))goto mi_004e8c94;
    /* 004e8c80 */
    f->r[0]=f->r[1];
    /* 004e8c82 */
    f->r[1]=f->r[5];
    /* 004e8c84 */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 004e8c8a */
    MI_U32((0x00ea5db0u+f->r[1]))=(uint32_t)(0x00000003u);
mi_004e8c94:;
    /* 004e8c94 */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
    /* 004e8c98 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004e8c9a */
    if(!mi_signed_less(f->flags))goto mi_004e8cb0;
    /* 004e8c9c */
    f->r[2]=f->r[5];
    /* 004e8c9e */
    f->r[0]=f->r[1];
    /* 004e8ca0 */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 004e8ca6 */
    MI_U32((0x00ea5db0u+f->r[2]))=(uint32_t)(0x00000004u);
mi_004e8cb0:;
    /* 004e8cb0 */
    a=MI_U32((0x00000018u+f->r[4]));b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004e8cb4 */
    if(!mi_signed_less(f->flags))goto mi_004e8cc8;
    /* 004e8cb6 */
    f->r[0]=f->r[5];
    /* 004e8cb8 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004e8cbe */
    MI_U32((0x00ea5db0u+f->r[0]))=(uint32_t)(0x00000005u);
mi_004e8cc8:;
    /* 004e8cc8 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38ea8u)))&0xffu)<<0);
    /* 004e8ccd */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004e8ccf */
    a=(f->r[0]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 004e8cd1 */
    if(!mi_signed_less(f->flags))goto mi_004e8ce0;
    /* 004e8cd3 */
    f->r[0]=MI_U32((0x00e38ebcu));
    /* 004e8cd8 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e8cda */
    if((f->flags&MI_ZF))goto mi_004e8d8f;
mi_004e8ce0:;
    /* 004e8ce0 */
    f->r[1]=f->r[5];
    /* 004e8ce2 */
    f->r[0]=0x66666667u;
    /* 004e8ce7 */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 004e8ced */
    f->r[6]=MI_U32((0x00ea1880u+f->r[1]));
    /* 004e8cf3 */
    a=f->r[0];b=f->r[6];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 004e8cf5 */
    a=f->r[2];b=0x00000002u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 004e8cf8 */
    f->r[0]=f->r[2];
    /* 004e8cfa */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 004e8cfd */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004e8cff */
    f->r[0]=0x66666667u;
    /* 004e8d04 */
    f->r[7]=f->r[2];
    /* 004e8d06 */
    f->r[2]=MI_U32((0x00ea5da8u+f->r[1]));
    /* 004e8d0c */
    a=f->r[0];b=f->r[2];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 004e8d0e */
    a=f->r[2];b=0x00000002u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 004e8d11 */
    f->r[0]=f->r[2];
    /* 004e8d13 */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 004e8d16 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004e8d18 */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004e8d1a */
    if((f->flags&MI_ZF))goto mi_004e8d8f;
    /* 004e8d1c */
    a=MI_U32((0x00ea1894u+f->r[1]));b=0x0000012cu;t=mi_alu(f,a,b,32,1);
    /* 004e8d26 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e8d8f;
    /* 004e8d28 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004e8d2a */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e8d40;
    /* 004e8d2c */
    f->r[1]=MI_U32((0x00ea5db0u+f->r[1]));
    /* 004e8d32 */
    f->r[0]=0x00000069u;
    /* 004e8d37 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004e8d39 */
    if((f->flags&MI_ZF))goto mi_004e8d65;
    /* 004e8d3b */
    f->r[3]=(0x00000074u+f->r[1]);
    /* 004e8d3e */
    goto mi_004e8d65;
mi_004e8d40:;
    /* 004e8d40 */
    a=f->r[6];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 004e8d43 */
    if(mi_signed_less(f->flags))goto mi_004e8d4c;
    /* 004e8d45 */
    f->r[0]=0x00000074u;
    /* 004e8d4a */
    goto mi_004e8d65;
mi_004e8d4c:;
    /* 004e8d4c */
    a=f->r[6];b=0x00000032u;t=mi_alu(f,a,b,32,1);
    /* 004e8d4f */
    f->r[0]=(0x0000006au+f->r[7]);
    /* 004e8d52 */
    if(!mi_signed_less(f->flags))goto mi_004e8d61;
    /* 004e8d54 */
    f->r[1]=MI_U32((0x00ea5db0u+f->r[1]));
    /* 004e8d5a */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004e8d5c */
    if((f->flags&MI_ZF))goto mi_004e8d61;
    /* 004e8d5e */
    f->r[3]=(0x00000074u+f->r[1]);
mi_004e8d61:;
    /* 004e8d61 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e8d63 */
    if((f->flags&MI_ZF))goto mi_004e8d78;
mi_004e8d65:;
    /* 004e8d65 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e8d67 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e8d69 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e8d6a */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e8d6fu;f->boundary(0x004e8d6au,0x004e2970u,f);
    /* 004e8d6f */
    f->r[5]=MI_U32((0x00eaac28u));
    /* 004e8d75 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004e8d78:;
    /* 004e8d78 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 004e8d7a */
    if((f->flags&MI_ZF))goto mi_004e8d8f;
    /* 004e8d7c */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e8d7e */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e8d80 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e8d81 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e8d86u;f->boundary(0x004e8d81u,0x004e2970u,f);
    /* 004e8d86 */
    f->r[5]=MI_U32((0x00eaac28u));
    /* 004e8d8c */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004e8d8f:;
    /* 004e8d8f */
    f->r[0]=f->r[5];
    /* 004e8d91 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004e8d92 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004e8d98 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e8d99 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004e8d9a */
    f->r[1]=MI_U32((0x00ea1880u+f->r[0]));
    /* 004e8da0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004e8da1 */
    MI_U32((0x00ea5da8u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 004e8da7 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e8daa */
    f->r[4]+=4;return;
}

void restore_calendar_C_004e8db0(struct restore_calendar_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004e8db0 */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 004e8db6 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e8db7 */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 004e8dbd */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e8dbe */
    f->r[2]=0x00000050u;
    /* 004e8dc3 */
    f->r[7]=MI_U32((0x00ea1880u+f->r[1]));
    /* 004e8dc9 */
    f->r[0]=0x66666667u;
    /* 004e8dce */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004e8dd0 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 004e8dd2 */
    a=f->r[0];b=f->r[2];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 004e8dd4 */
    a=f->r[2];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 004e8dd6 */
    f->r[0]=f->r[2];
    /* 004e8dd8 */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 004e8ddb */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004e8ddd */
    a=MI_U16((0x00ea5ce8u+f->r[1]));b=(f->r[6]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 004e8de4 */
    f->r[7]=f->r[2];
    /* 004e8de6 */
    if((f->flags&MI_ZF))goto mi_004e8e21;
    /* 004e8de8 */
    f->r[1]=MI_U32((0x00ea5f9cu+f->r[1]));
    /* 004e8dee */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e8def */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e8df4u;f->boundary(0x004e8defu,0x004c5120u,f);
    /* 004e8df4 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e8df7 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004e8dfa */
    if(!(f->flags&MI_ZF))goto mi_004e8e21;
    /* 004e8dfc */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 004e8e02 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e8e03 */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 004e8e09 */
    f->r[0]=MI_U32((0x00ea5f9cu+f->r[2]));
    /* 004e8e0f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e8e10 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e8e15u;f->boundary(0x004e8e10u,0x004c52d0u,f);
    /* 004e8e15 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e8e18 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004e8e1a */
    if((f->flags&MI_ZF))goto mi_004e8e21;
    /* 004e8e1c */
    f->r[6]=0xfffffffeu;
mi_004e8e21:;
    /* 004e8e21 */
    f->r[0]=0x00799bc4u;
mi_004e8e26:;
    /* 004e8e26 */
    a=MI_U8((0xffffff1cu+f->r[0]));b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004e8e2d */
    if(!(f->flags&MI_ZF))goto mi_004e8e62;
    /* 004e8e2f */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0xffffff21u+f->r[0])))&0xffu)<<0);
    /* 004e8e35 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004e8e37 */
    if((f->flags&MI_ZF))goto mi_004e8e62;
    /* 004e8e39 */
    a=MI_U16((0xffffff38u+f->r[0]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 004e8e41 */
    if((f->flags&MI_ZF))goto mi_004e8e62;
    /* 004e8e43 */
    f->r[2]=MI_U32((0x00000000u+f->r[0]));
    /* 004e8e45 */
    f->r[1]=(0x00000000u+f->r[6]+f->r[7]*1u);
    /* 004e8e48 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004e8e4a */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[2]);
    /* 004e8e4c */
    if(!(f->flags&MI_SF))goto mi_004e8e54;
    /* 004e8e4e */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(0x00000000u);
mi_004e8e54:;
    /* 004e8e54 */
    a=MI_U32((0x00000000u+f->r[0]));b=0x000003e8u;t=mi_alu(f,a,b,32,1);
    /* 004e8e5a */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e8e62;
    /* 004e8e5c */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(0x000003e8u);
mi_004e8e62:;
    /* 004e8e62 */
    a=f->r[0];b=0x00000108u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004e8e67 */
    a=f->r[0];b=0x0089b7bcu;t=mi_alu(f,a,b,32,1);
    /* 004e8e6c */
    if(mi_signed_less(f->flags))goto mi_004e8e26;
    /* 004e8e6e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004e8e6f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e8e70 */
    f->r[4]+=4;return;
}

void restore_calendar_C_00430060(struct restore_calendar_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00430060 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 00430063 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00430064 */
    f->r[6]=MI_U32((0x00000020u+f->r[4]));
    /* 00430068 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00430069 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043006eu;f->boundary(0x00430069u,0x0042fb30u,f);
    /* 0043006e */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 00430073 */
    f->r[1]=0x00000064u;
    /* 00430078 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0043007e */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00430081 */
    MI_U32((0x0070e130u))=(uint32_t)(0x00000001u);
    /* 0043008b */
    f->r[2]=MI_U32((0x00ea1888u+f->r[0]));
    /* 00430091 */
    f->r[0]=0x66666667u;
    /* 00430096 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00430098 */
    MI_U32((0x0070e124u))=(uint32_t)(f->r[6]);
    /* 0043009e */
    a=f->r[0];b=f->r[1];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 004300a0 */
    f->r[0]=MI_U32((0x0078d7c0u));
    /* 004300a5 */
    f->r[1]=f->r[2];
    /* 004300a7 */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,1);f->r[1]=t;
    /* 004300aa */
    f->r[2]=f->r[1];
    /* 004300ac */
    a=f->r[2];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[2]=t;
    /* 004300af */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004300b1 */
    a=f->r[0];b=0x00000009u;t=mi_alu(f,a,b,32,1);
    /* 004300b4 */
    if((f->flags&MI_ZF))goto mi_004300c0;
    /* 004300b6 */
    a=f->r[0];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 004300b9 */
    if((f->flags&MI_ZF))goto mi_004300c0;
    /* 004300bb */
    a=f->r[0];b=0x0000000bu;t=mi_alu(f,a,b,32,1);
    /* 004300be */
    if(!(f->flags&MI_ZF))goto mi_004300c1;
mi_004300c0:;
    /* 004300c0 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
mi_004300c1:;
    /* 004300c1 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004300c3 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004300cc;
    /* 004300c5 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,5);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004300c7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004300c8 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004300cb */
    f->r[4]+=4;return;
mi_004300cc:;
    /* 004300cc */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 004300cf */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004300d6;
    /* 004300d1 */
    f->r[1]=0x00000004u;
mi_004300d6:;
    /* 004300d6 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004300d7 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004300d8 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004300d9 */
    f->r[7]=f->r[6];
    /* 004300db */
    a=f->r[7];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 004300de */
    a=f->r[7];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004300e0 */
    a=f->r[7];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 004300e3 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[7]));
    /* 004300ea */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[7]));
    /* 004300f1 */
    f->r[6]=(0x00000000u+f->r[2]+f->r[1]*1u);
    /* 004300f4 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004300f6 */
    f->r[5]=(0x00000000u+f->r[0]+f->r[1]*1u);
    /* 004300f9 */
    f->r[3]=f->r[2];
    /* 004300fb */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004300fd */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[6]);
    /* 00430101 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 00430103 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[5]);
    /* 00430107 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 0043010b */
    if(!mi_signed_less(f->flags))goto mi_00430119;
    /* 0043010d */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 00430115 */
    f->r[3]=MI_U32((0x00000010u+f->r[4]));
mi_00430119:;
    /* 00430119 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0043011b */
    if(!mi_signed_less(f->flags))goto mi_0043011f;
    /* 0043011d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_0043011f:;
    /* 0043011f */
    f->r[1]=MI_U32((0x00bca2d0u));
    /* 00430125 */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 00430127 */
    if(mi_signed_less(f->flags))goto mi_00430130;
    /* 00430129 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0043012a */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0043012e */
    f->r[6]=f->r[1];
mi_00430130:;
    /* 00430130 */
    f->r[1]=MI_U32((0x00bca2d4u));
    /* 00430136 */
    a=f->r[5];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 00430138 */
    if(mi_signed_less(f->flags))goto mi_0043013f;
    /* 0043013a */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0043013b */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[1]);
mi_0043013f:;
    /* 0043013f */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 00430142 */
    f->r[5]=f->r[0];
    /* 00430144 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 00430148 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[1]*2u);
    /* 0043014b */
    f->r[2]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 0043014e */
    f->r[1]=MI_U32((0x00bca2d8u));
    /* 00430154 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[2]*4u);
    /* 00430157 */
    f->r[2]=f->r[3];
    /* 00430159 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0043015b */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0043015d */
    a=f->r[2];b=0x000000e3u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00430163 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00430167 */
    f->r[2]=MI_U32((0x0000001cu+f->r[4]));
    /* 0043016b */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0043016d */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0043023a;
mi_00430173:;
    /* 00430173 */
    a=f->r[3];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 00430175 */
    f->r[2]=f->r[3];
    /* 00430177 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00430223;
    /* 0043017d */
    f->r[0]=(0x00b4e6fcu+f->r[1]*2u);
    /* 00430184 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00430188 */
    f->r[0]=f->r[6];
    /* 0043018a */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0043018c */
    f->r[1]=(0x00000001u+f->r[1]+f->r[0]*1u);
    /* 00430190 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[1]);
mi_00430194:;
    /* 00430194 */
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
    /* 00430198 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[0]));
    /* 0043019b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0043019d */
    if((f->flags&MI_ZF))goto mi_0043020b;
    /* 0043019f */
    f->r[6]=f->r[0];
    /* 004301a1 */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004301a4 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004301a6 */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004301a9 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x007999d8u+f->r[6])))&0xffu)<<0);
    /* 004301af */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004301b1 */
    if((f->flags&MI_ZF))goto mi_00430207;
    /* 004301b3 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x007999ddu+f->r[6])))&0xffu)<<0);
    /* 004301b9 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004301bb */
    if((f->flags&MI_ZF))goto mi_00430207;
    /* 004301bd */
    a=MI_U16((0x007999f4u+f->r[6]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 004301c5 */
    if((f->flags&MI_ZF))goto mi_00430207;
    /* 004301c7 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[7]));
    /* 004301ce */
    f->r[0]=f->r[2];
    /* 004301d0 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004301d2 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004301d4 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004301d6 */
    f->r[3]=f->r[0];
    /* 004301d8 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004301dc;
    /* 004301da */
    f->r[3]=f->r[1];
mi_004301dc:;
    /* 004301dc */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[7]));
    /* 004301e3 */
    f->r[0]=f->r[5];
    /* 004301e5 */
    f->r[5]=MI_U32((0x00000018u+f->r[4]));
    /* 004301e9 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004301eb */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004301ed */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004301ef */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004301f3;
    /* 004301f1 */
    f->r[0]=f->r[1];
mi_004301f3:;
    /* 004301f3 */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004301f5 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004301f9;
    /* 004301f7 */
    f->r[0]=f->r[3];
mi_004301f9:;
    /* 004301f9 */
    f->r[5]=MI_U32((0x00000018u+f->r[4]));
    /* 004301fd */
    f->r[3]=MI_U32((0x00000010u+f->r[4]));
    /* 00430201 */
    MI_U32((0x00799ac4u+f->r[6]))=(uint32_t)(f->r[0]);
mi_00430207:;
    /* 00430207 */
    f->r[6]=MI_U32((0x0000002cu+f->r[4]));
mi_0043020b:;
    /* 0043020b */
    f->r[1]=MI_U32((0x00000014u+f->r[4]));
    /* 0043020f */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00430210 */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00430213 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 00430215 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00430219 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00430194;
    /* 0043021f */
    f->r[1]=MI_U32((0x00000020u+f->r[4]));
mi_00430223:;
    /* 00430223 */
    f->r[2]=MI_U32((0x00000024u+f->r[4]));
    /* 00430227 */
    f->r[0]=MI_U32((0x0000001cu+f->r[4]));
    /* 0043022b */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 0043022c */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0043022e */
    a=f->r[5];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00430230 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 00430234 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00430173;
mi_0043023a:;
    /* 0043023a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0043023b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0043023c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0043023d */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 0043023f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00430240 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00430243 */
    f->r[4]+=4;return;
}

void restore_calendar_C_00430040(struct restore_calendar_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00430040 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00430041 */
    f->r[6]=MI_U32((0x00000008u+f->r[4]));
    /* 00430045 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00430046 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043004bu;f->boundary(0x00430046u,0x0042fb30u,f);
    /* 0043004b */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043004e */
    f->r[0]=0x00000001u;
    /* 00430053 */
    MI_U32((0x0070e128u))=(uint32_t)(f->r[6]);
    /* 00430059 */
    MI_U32((0x0070e134u))=(uint32_t)(f->r[0]);
    /* 0043005e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0043005f */
    f->r[4]+=4;return;
}

void restore_calendar_C_00430250(struct restore_calendar_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00430250 */
    f->r[0]=MI_U32((0x0078d790u));
    /* 00430255 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00430256 */
    f->r[7]=0x00000001u;
    /* 0043025b */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 0043025d */
    if(mi_signed_less(f->flags))goto mi_004302aa;
    /* 0043025f */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00430260 */
    f->r[6]=0x00799bccu;
mi_00430265:;
    /* 00430265 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0xffffff14u+f->r[6])))&0xffu)<<0);
    /* 0043026b */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0043026d */
    if((f->flags&MI_ZF))goto mi_00430299;
    /* 0043026f */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0xffffff19u+f->r[6])))&0xffu)<<0);
    /* 00430275 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00430277 */
    if((f->flags&MI_ZF))goto mi_00430299;
    /* 00430279 */
    a=MI_U16((0xffffff30u+f->r[6]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 00430281 */
    if((f->flags&MI_ZF))goto mi_00430299;
    /* 00430283 */
    f->r[0]=MI_U32((0x00000000u+f->r[6]));
    /* 00430285 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00430287 */
    if((f->flags&MI_ZF))goto mi_00430299;
    /* 00430289 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0043028a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0043028c */
    MI_U32((0x00000000u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 0043028e */
    if(!(f->flags&MI_ZF))goto mi_00430299;
    /* 00430290 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00430291 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00430296u;f->boundary(0x00430291u,0x0042fb30u,f);
    /* 00430296 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00430299:;
    /* 00430299 */
    f->r[0]=MI_U32((0x0078d790u));
    /* 0043029e */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 0043029f */
    a=f->r[6];b=0x00000108u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004302a5 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004302a7 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00430265;
    /* 004302a9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
mi_004302aa:;
    /* 004302aa */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004302ab */
    f->r[4]+=4;return;
}

void restore_calendar_C_004302b0(struct restore_calendar_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004302b0 */
    f->r[0]=MI_U32((0x0070e138u));
    /* 004302b5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004302b7 */
    if((f->flags&MI_ZF))goto mi_004302fd;
    /* 004302b9 */
    f->r[1]=MI_U32((0x0070e12cu));
    /* 004302bf */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004302c1 */
    f->r[0]=f->r[1];
    /* 004302c3 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004302c5 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004302c8 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004302ca */
    MI_U32((0x00e950dcu))=(uint32_t)(0x00000001u);
    /* 004302d4 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004302d7 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e8u+f->r[0]));
    /* 004302de */
    f->r[1]=MI_U32((0x007999e4u+f->r[0]));
    /* 004302e4 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004302e5 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004302e6 */
    a=0x00000147u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004302eb */
    f->r[4]-=4;MI_U32(f->r[4])=0x004302f0u;f->boundary(0x004302ebu,0x004e0cf0u,f);
    /* 004302f0 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004302f3 */
    MI_U32((0x0070e138u))=(uint32_t)(0x00000000u);
mi_004302fd:;
    /* 004302fd */
    f->r[0]=MI_U32((0x0070e134u));
    /* 00430302 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00430304 */
    if((f->flags&MI_ZF))goto mi_0043034a;
    /* 00430306 */
    f->r[1]=MI_U32((0x0070e128u));
    /* 0043030c */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043030e */
    f->r[0]=f->r[1];
    /* 00430310 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00430312 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00430315 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00430317 */
    MI_U32((0x00e950dcu))=(uint32_t)(0x00000001u);
    /* 00430321 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00430324 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e8u+f->r[0]));
    /* 0043032b */
    f->r[1]=MI_U32((0x007999e4u+f->r[0]));
    /* 00430331 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00430332 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00430333 */
    a=0x00000146u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00430338 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043033du;f->boundary(0x00430338u,0x004e0cf0u,f);
    /* 0043033d */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00430340 */
    MI_U32((0x0070e134u))=(uint32_t)(0x00000000u);
mi_0043034a:;
    /* 0043034a */
    f->r[0]=MI_U32((0x0070e130u));
    /* 0043034f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00430351 */
    if((f->flags&MI_ZF))goto mi_00430397;
    /* 00430353 */
    f->r[1]=MI_U32((0x0070e124u));
    /* 00430359 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043035b */
    f->r[0]=f->r[1];
    /* 0043035d */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043035f */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00430362 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00430364 */
    MI_U32((0x00e950dcu))=(uint32_t)(0x00000001u);
    /* 0043036e */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00430371 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e8u+f->r[0]));
    /* 00430378 */
    f->r[1]=MI_U32((0x007999e4u+f->r[0]));
    /* 0043037e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043037f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00430380 */
    a=0x00000148u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00430385 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043038au;f->boundary(0x00430385u,0x004e0cf0u,f);
    /* 0043038a */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043038d */
    MI_U32((0x0070e130u))=(uint32_t)(0x00000000u);
mi_00430397:;
    /* 00430397 */
    f->r[4]+=4;return;
}

void restore_calendar_C_00458f80(struct restore_calendar_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00458f80 */
    f->r[1]=MI_U32((0x00000004u+f->r[4]));
    /* 00458f84 */
    f->r[2]=MI_U32((0x0078d790u));
    /* 00458f8a */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 00458f90 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00458f91 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 00458f93 */
    MI_U32((0x00ea3f78u+f->r[1]))=(uint32_t)(f->r[3]);
    /* 00458f99 */
    MI_U32((0x00ea3f7cu+f->r[1]))=(uint32_t)(f->r[3]);
    /* 00458f9f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00458fa1 */
    MI_U32((0x00ea5eacu+f->r[1]))=(uint32_t)(f->r[3]);
    /* 00458fa7 */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 00458faa */
    MI_U32((0x00ea5eb0u+f->r[1]))=(uint32_t)(f->r[3]);
    /* 00458fb0 */
    if(mi_signed_less(f->flags))goto mi_0045901c;
    /* 00458fb2 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00458fb3 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00458fb4 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00458fb5 */
    f->r[7]=0x00799ae0u;
    /* 00458fba */
    f->r[5]=f->r[2];
mi_00458fbc:;
    /* 00458fbc */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00000000u+f->r[7])))&0xffu)<<0);
    /* 00458fbe */
    a=(f->r[2]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 00458fc0 */
    if((f->flags&MI_ZF))goto mi_00459010;
    /* 00458fc2 */
    a=(f->r[2]&0xffu);b=0x00000005u;t=mi_alu(f,a,b,8,1);
    /* 00458fc5 */
    if((f->flags&MI_ZF))goto mi_00459010;
    /* 00458fc7 */
    a=(f->r[2]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 00458fca */
    if((f->flags&MI_ZF))goto mi_00459010;
    /* 00458fcc */
    a=(f->r[2]&0xffu);b=0x00000006u;t=mi_alu(f,a,b,8,1);
    /* 00458fcf */
    if((f->flags&MI_ZF))goto mi_00459010;
    /* 00458fd1 */
    a=MI_U8((0x00000005u+f->r[7]));b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 00458fd4 */
    if((f->flags&MI_ZF))goto mi_00459010;
    /* 00458fd6 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x0000001cu+f->r[7]));
    /* 00458fda */
    f->r[6]=(f->r[6]&0xffff0000u)|(((MI_U16((0x00000012u+f->r[7])))&0xffffu)<<0);
    /* 00458fde */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00458fe0 */
    a=(f->r[6]&0xffffu);b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 00458fe4 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00458fec;
    /* 00458fe6 */
    a=MI_U32((0x00ea5eacu+f->r[1]));b=f->r[2];t=mi_alu(f,a,b,32,0);MI_U32((0x00ea5eacu+f->r[1]))=(uint32_t)(t);
mi_00458fec:;
    /* 00458fec */
    a=(f->r[6]&0xffffu);b=0x00000003u;t=mi_alu(f,a,b,16,1);
    /* 00458ff0 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00458ff8;
    /* 00458ff2 */
    a=MI_U32((0x00ea3f78u+f->r[1]));b=f->r[2];t=mi_alu(f,a,b,32,0);MI_U32((0x00ea3f78u+f->r[1]))=(uint32_t)(t);
mi_00458ff8:;
    /* 00458ff8 */
    a=(f->r[6]&0xffffu);b=0x0000000au;t=mi_alu(f,a,b,16,1);
    /* 00458ffc */
    if(mi_signed_less(f->flags))goto mi_00459004;
    /* 00458ffe */
    a=MI_U32((0x00ea5eb0u+f->r[1]));b=f->r[2];t=mi_alu(f,a,b,32,0);MI_U32((0x00ea5eb0u+f->r[1]))=(uint32_t)(t);
mi_00459004:;
    /* 00459004 */
    a=(f->r[6]&0xffffu);b=0x0000000eu;t=mi_alu(f,a,b,16,1);
    /* 00459008 */
    if(mi_signed_less(f->flags))goto mi_00459010;
    /* 0045900a */
    a=MI_U32((0x00ea3f7cu+f->r[1]));b=f->r[2];t=mi_alu(f,a,b,32,0);MI_U32((0x00ea3f7cu+f->r[1]))=(uint32_t)(t);
mi_00459010:;
    /* 00459010 */
    a=f->r[7];b=0x00000108u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 00459016 */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 00459017 */
    if(!(f->flags&MI_ZF))goto mi_00458fbc;
    /* 00459019 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0045901a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0045901b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
mi_0045901c:;
    /* 0045901c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0045901d */
    f->r[4]+=4;return;
}

void restore_calendar_C_004e8ec0(struct restore_calendar_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004e8ec0 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 004e8ec5 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004e8ecb */
    f->r[1]=MI_U32((0x00ea1894u+f->r[0]));
    /* 004e8ed1 */
    f->r[2]=MI_U32((0x00ea5eacu+f->r[0]));
    /* 004e8ed7 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e8ed8 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e8ed9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e8edeu;f->boundary(0x004e8ed9u,0x004cde60u,f);
    /* 004e8ede */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 004e8ee4 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e8ee7 */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 004e8eed */
    f->r[2]=MI_U32((0x00ea3f7cu+f->r[1]));
    /* 004e8ef3 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 004e8ef5 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e8f23;
    /* 004e8ef7 */
    a=f->r[0];b=0x00000038u;t=mi_alu(f,a,b,32,1);
    /* 004e8efa */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e8eff;
    /* 004e8efc */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004e8efe */
    f->r[4]+=4;return;
mi_004e8eff:;
    /* 004e8eff */
    a=f->r[0];b=0x00000028u;t=mi_alu(f,a,b,32,1);
    /* 004e8f02 */
    if(mi_signed_less(f->flags))goto mi_004e8f0a;
    /* 004e8f04 */
    f->r[0]=0xfffffffdu;
    /* 004e8f09 */
    f->r[4]+=4;return;
mi_004e8f0a:;
    /* 004e8f0a */
    a=f->r[0];b=0x0000001au;t=mi_alu(f,a,b,32,1);
    /* 004e8f0d */
    if(mi_signed_less(f->flags))goto mi_004e8f15;
    /* 004e8f0f */
    f->r[0]=0xfffffffcu;
    /* 004e8f14 */
    f->r[4]+=4;return;
mi_004e8f15:;
    /* 004e8f15 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004e8f17 */
    a=f->r[0];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 004e8f1a */
    t=(!mi_signed_less(f->flags))?1u:0u;f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004e8f1d */
    a=f->r[1];b=0xfffffffau;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004e8f20 */
    f->r[0]=f->r[1];
    /* 004e8f22 */
    f->r[4]+=4;return;
mi_004e8f23:;
    /* 004e8f23 */
    f->r[2]=MI_U32((0x00ea5eb0u+f->r[1]));
    /* 004e8f29 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 004e8f2b */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e8f59;
    /* 004e8f2d */
    a=f->r[0];b=0x00000038u;t=mi_alu(f,a,b,32,1);
    /* 004e8f30 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e8f35;
    /* 004e8f32 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004e8f34 */
    f->r[4]+=4;return;
mi_004e8f35:;
    /* 004e8f35 */
    a=f->r[0];b=0x00000028u;t=mi_alu(f,a,b,32,1);
    /* 004e8f38 */
    if(mi_signed_less(f->flags))goto mi_004e8f40;
    /* 004e8f3a */
    f->r[0]=0xfffffffeu;
    /* 004e8f3f */
    f->r[4]+=4;return;
mi_004e8f40:;
    /* 004e8f40 */
    a=f->r[0];b=0x0000001au;t=mi_alu(f,a,b,32,1);
    /* 004e8f43 */
    if(mi_signed_less(f->flags))goto mi_004e8f4b;
    /* 004e8f45 */
    f->r[0]=0xfffffffdu;
    /* 004e8f4a */
    f->r[4]+=4;return;
mi_004e8f4b:;
    /* 004e8f4b */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004e8f4d */
    a=f->r[0];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 004e8f50 */
    t=(!mi_signed_less(f->flags))?1u:0u;f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004e8f53 */
    a=f->r[2];b=0xfffffffbu;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004e8f56 */
    f->r[0]=f->r[2];
    /* 004e8f58 */
    f->r[4]+=4;return;
mi_004e8f59:;
    /* 004e8f59 */
    a=f->r[0];b=0x00000028u;t=mi_alu(f,a,b,32,1);
    /* 004e8f5c */
    if(mi_signed_less(f->flags))goto mi_004e8f61;
    /* 004e8f5e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004e8f60 */
    f->r[4]+=4;return;
mi_004e8f61:;
    /* 004e8f61 */
    a=f->r[0];b=0x0000001au;t=mi_alu(f,a,b,32,1);
    /* 004e8f64 */
    if(mi_signed_less(f->flags))goto mi_004e8f6a;
    /* 004e8f66 */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[0]=t;
    /* 004e8f69 */
    f->r[4]+=4;return;
mi_004e8f6a:;
    /* 004e8f6a */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004e8f6c */
    a=f->r[0];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 004e8f6f */
    t=(!mi_signed_less(f->flags))?1u:0u;f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004e8f72 */
    a=f->r[1];b=0xfffffffdu;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004e8f75 */
    f->r[0]=f->r[1];
    /* 004e8f77 */
    f->r[4]+=4;return;
}

void restore_calendar_C_0042fb30(struct restore_calendar_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0042fb30 */
    f->r[0]=MI_U32((0x00000004u+f->r[4]));
    /* 0042fb34 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0042fb35 */
    f->r[6]=f->r[0];
    /* 0042fb37 */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 0042fb3d */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0042fb40 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0042fb42 */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0042fb45 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999f4u+f->r[6]));
    /* 0042fb4c */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,0,a,32,1);f->r[0]=t;
    /* 0042fb4e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0042fb4f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0042fb50 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0042fb55u;f->boundary(0x0042fb50u,0x004e90f0u,f);
    /* 0042fb55 */
    f->r[1]=MI_U32((0x00e38ebcu));
    /* 0042fb5b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0042fb5d */
    MI_U16((0x007999f4u+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0042fb64 */
    MI_U8((0x00799a5fu+f->r[6]))=(uint8_t)(0x00000001u);
    /* 0042fb6b */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0042fb6e */
    MI_U32((0x00799ac8u+f->r[6]))=(uint32_t)(0x00000002u);
    /* 0042fb78 */
    MI_U32((0x00799ad4u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 0042fb7e */
    MI_U32((0x00799ac0u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 0042fb84 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0042fb86 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0042fb87 */
    if(!(f->flags&MI_ZF))goto mi_0042fba8;
    /* 0042fb89 */
    f->r[2]=(uint32_t)(int32_t)(int8_t)MI_U8((0x00e38ea8u));
    /* 0042fb90 */
    a=f->r[2];b=MI_U32((0x00579558u));t=mi_alu(f,a,b,32,1);
    /* 0042fb96 */
    if(!(f->flags&MI_ZF))goto mi_0042fba8;
    /* 0042fb98 */
    a=MI_U8((0x007027beu));b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 0042fb9e */
    if(!(f->flags&MI_ZF))goto mi_0042fba8;
    /* 0042fba0 */
    a=0x00000006u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0042fba2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0042fba7u;f->boundary(0x0042fba2u,0x00417ae0u,f);
    /* 0042fba7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
mi_0042fba8:;
    /* 0042fba8 */
    f->r[4]+=4;return;
}

void restore_calendar_C_0042fbb0(struct restore_calendar_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0042fbb0 */
    f->r[0]=MI_U32((0x00d3b1c4u));
    /* 0042fbb5 */
    f->r[1]=MI_U32((0x00d3b374u));
    /* 0042fbbb */
    MI_U32((0x00d3b1c8u+f->r[0]*4u))=(uint32_t)(f->r[1]);
    /* 0042fbc2 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0042fbc3 */
    a=f->r[0];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 0042fbc6 */
    MI_U32((0x00d3b1c4u))=(uint32_t)(f->r[0]);
    /* 0042fbcb */
    if(mi_signed_less(f->flags))goto mi_0042fbd7;
    /* 0042fbcd */
    MI_U32((0x00d3b1c4u))=(uint32_t)(0x00000000u);
mi_0042fbd7:;
    /* 0042fbd7 */
    f->r[2]=MI_U32((0x00d3b384u));
    /* 0042fbdd */
    f->r[1]=MI_U32((0x00d3b380u));
    /* 0042fbe3 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0042fbe4 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0042fbe5 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0042fbe6 */
    f->r[7]=0x0000001fu;
mi_0042fbeb:;
    /* 0042fbeb */
    f->r[0]=f->r[2];
    /* 0042fbed */
    f->r[6]=f->r[2];
    /* 0042fbef */
    a=f->r[0];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 0042fbf2 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 0042fbf5 */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 0042fbf8 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0042fbfa */
    f->r[6]=f->r[1];
    /* 0042fbfc */
    a=f->r[6];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[6]=t;
    /* 0042fbff */
    f->r[3]=f->r[1];
    /* 0042fc01 */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 0042fc04 */
    a=f->r[3];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[3]=t;
    /* 0042fc07 */
    a=f->r[6];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 0042fc09 */
    a=f->r[2];t=a>>1u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|((a>>0u)&1u)|mi_szp(t,32)|((a&0x80000000u)?MI_OF:0);f->r[2]=t;
    /* 0042fc0b */
    a=f->r[1];t=a>>1u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|((a>>0u)&1u)|mi_szp(t,32)|((a&0x80000000u)?MI_OF:0);f->r[1]=t;
    /* 0042fc0d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0042fc0f */
    if((f->flags&MI_ZF))goto mi_0042fc17;
    /* 0042fc11 */
    a=f->r[2];b=0x40000000u;t=mi_alu(f,a,b,32,4);f->r[2]=t;
mi_0042fc17:;
    /* 0042fc17 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 0042fc19 */
    if((f->flags&MI_ZF))goto mi_0042fc21;
    /* 0042fc1b */
    a=f->r[1];b=0x40000000u;t=mi_alu(f,a,b,32,4);f->r[1]=t;
mi_0042fc21:;
    /* 0042fc21 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 0042fc22 */
    if(!(f->flags&MI_ZF))goto mi_0042fbeb;
    /* 0042fc24 */
    MI_U32((0x00d3b384u))=(uint32_t)(f->r[2]);
    /* 0042fc2a */
    f->r[0]=f->r[2];
    /* 0042fc2c */
    f->r[6]=f->r[2];
    /* 0042fc2e */
    a=f->r[2];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0042fc31 */
    MI_U32((0x00d3b378u))=(uint32_t)(f->r[2]);
    /* 0042fc37 */
    f->r[2]=f->r[1];
    /* 0042fc39 */
    a=f->r[2];b=0x00007fffu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0042fc3f */
    MI_U32((0x00d3b380u))=(uint32_t)(f->r[1]);
    /* 0042fc45 */
    MI_U32((0x00d3b364u))=(uint32_t)(f->r[2]);
    /* 0042fc4b */
    f->r[2]=f->r[1];
    /* 0042fc4d */
    a=f->r[1];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 0042fc50 */
    a=f->r[6];b=0x0000007fu;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 0042fc53 */
    MI_U32((0x00d3b36cu))=(uint32_t)(f->r[1]);
    /* 0042fc59 */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 0042fc5f */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 0042fc65 */
    a=f->r[2];b=0x0000007fu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0042fc68 */
    MI_U32((0x00d3b374u))=(uint32_t)(f->r[6]);
    /* 0042fc6e */
    MI_U32((0x00d3b368u))=(uint32_t)(f->r[2]);
    /* 0042fc74 */
    f->r[2]=MI_U32((0x00ea1888u+f->r[1]));
    /* 0042fc7a */
    a=f->r[0];b=0x00007fffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 0042fc7f */
    f->r[6]=0x00000028u;
    /* 0042fc84 */
    MI_U32((0x00d3b370u))=(uint32_t)(f->r[0]);
    /* 0042fc89 */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 0042fc8b */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0042fc8c */
    f->r[1]=0x00000064u;
    /* 0042fc91 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0042fc92 */
    a=f->r[1];mi_idiv32(f,a);
    /* 0042fc94 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 0042fc96 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0042fc97 */
    t=(mi_signed_less(f->flags))?1u:0u;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0042fc9a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0042fc9b */
    f->r[4]+=4;return;
}

void restore_calendar_C_004e90f0(struct restore_calendar_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004e90f0 */
    f->r[1]=MI_U32((0x00000004u+f->r[4]));
    /* 004e90f4 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e90f5 */
    f->r[6]=f->r[1];
    /* 004e90f7 */
    f->r[0]=MI_U32((0x0000000cu+f->r[4]));
    /* 004e90fb */
    a=f->r[6];b=0x000049d8u;t=mi_imul(f,a,b);f->r[6]=t;
    /* 004e9101 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e9103 */
    MI_U32((0x00ea3f6cu+f->r[6]))=(uint32_t)(f->r[0]);
    /* 004e9109 */
    if(!mi_signed_less(f->flags))goto mi_004e9116;
    /* 004e910b */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,0,a,32,1);f->r[0]=t;
    /* 004e910d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e910e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e910f */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e9114u;f->boundary(0x004e910fu,0x004e9350u,f);
    /* 004e9114 */
    goto mi_004e911f;
mi_004e9116:;
    /* 004e9116 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e9122;
    /* 004e9118 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e9119 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e911a */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e911fu;f->boundary(0x004e911au,0x004e9400u,f);
mi_004e911f:;
    /* 004e911f */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004e9122:;
    /* 004e9122 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e9123 */
    MI_U32((0x00ea1894u+f->r[6]))=(uint32_t)(0x00000000u);
    /* 004e912d */
    f->r[0]=(0x00ea3e34u+f->r[6]);
    /* 004e9133 */
    f->r[1]=0x00000064u;
mi_004e9138:;
    /* 004e9138 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[0]));
    /* 004e913b */
    f->r[7]=MI_U32((0x00ea1894u+f->r[6]));
    /* 004e9141 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004e9144 */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004e9146 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004e9147 */
    MI_U32((0x00ea1894u+f->r[6]))=(uint32_t)(f->r[7]);
    /* 004e914d */
    if(!(f->flags&MI_ZF))goto mi_004e9138;
    /* 004e914f */
    f->r[1]=MI_U32((0x00ea5f0cu+f->r[6]));
    /* 004e9155 */
    f->r[0]=f->r[7];
    /* 004e9157 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004e9159 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004e915a */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e9162;
    /* 004e915c */
    MI_U32((0x00ea5f0cu+f->r[6]))=(uint32_t)(f->r[0]);
mi_004e9162:;
    /* 004e9162 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e9163 */
    f->r[4]+=4;return;
}

void restore_calendar_C_004e9400(struct restore_calendar_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004e9400 */
    f->r[0]=MI_U32((0x00000008u+f->r[4]));
    /* 004e9404 */
    f->r[1]=MI_U32((0x00d3b1c4u));
    /* 004e940a */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e940b */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 004e940d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e940f */
    if((f->flags&MI_ZF))goto mi_004e945e;
    /* 004e9411 */
    f->r[2]=MI_U32((0x00000008u+f->r[4]));
    /* 004e9415 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e9416 */
    a=f->r[2];b=0x000024ecu;t=mi_imul(f,a,b);f->r[2]=t;
    /* 004e941c */
    f->r[7]=f->r[0];
mi_004e941e:;
    /* 004e941e */
    f->r[0]=MI_U32((0x00d3b1c8u+f->r[1]*4u));
    /* 004e9425 */
    a=f->r[0];b=0x0000003fu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004e9428 */
    a=f->r[0];b=0x00000032u;t=mi_alu(f,a,b,32,1);
    /* 004e942b */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e9432;
    /* 004e942d */
    a=f->r[0];b=0x0000001eu;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004e9430 */
    goto mi_004e943e;
mi_004e9432:;
    /* 004e9432 */
    a=f->r[0];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 004e9435 */
    if(!mi_signed_less(f->flags))goto mi_004e943e;
    /* 004e9437 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004e9439 */
    if((f->flags&MI_ZF))goto mi_004e943e;
    /* 004e943b */
    a=f->r[0];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
mi_004e943e:;
    /* 004e943e */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004e9440 */
    a=MI_U16((0x00ea3e34u+f->r[0]*2u));b=f->flags&MI_CF;t=mi_alu(f,a,1,16,0);f->flags=(f->flags&~MI_CF)|b;MI_U16((0x00ea3e34u+f->r[0]*2u))=(uint16_t)(t);
    /* 004e9448 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004e9449 */
    a=f->r[1];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 004e944c */
    if(mi_signed_less(f->flags))goto mi_004e9450;
    /* 004e944e */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_004e9450:;
    /* 004e9450 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004e9452 */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004e9455 */
    t=(!(f->flags&MI_ZF))?1u:0u;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004e9458 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 004e9459 */
    f->r[6]=f->r[0];
    /* 004e945b */
    if(!(f->flags&MI_ZF))goto mi_004e941e;
    /* 004e945d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
mi_004e945e:;
    /* 004e945e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e945f */
    f->r[4]+=4;return;
}

void restore_calendar_C_004e7700(struct restore_calendar_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004e7700 */
    f->r[0]=MI_U32((0x0078d79cu));
    /* 004e7705 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 004e7708 */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,1);
    /* 004e770b */
    if(!(f->flags&MI_ZF))goto mi_004e7abc;
    /* 004e7711 */
    f->r[1]=0x00d05720u;
    /* 004e7716 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e771bu;f->boundary(0x004e7716u,0x004becf0u,f);
    /* 004e771b */
    f->r[1]=MI_U32((0x0078d7c0u));
    /* 004e7721 */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004e7724 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004e7726 */
    if((f->flags&MI_ZF))goto mi_004e7743;
    /* 004e7728 */
    f->r[1]=0x00d05720u;
    /* 004e772d */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e7732u;f->boundary(0x004e772du,0x004becf0u,f);
    /* 004e7732 */
    f->r[2]=MI_U32((0x0078d7c0u));
    /* 004e7738 */
    a=f->r[2];b=0xfffffff6u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004e773b */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004e773d */
    if(!(f->flags&MI_ZF))goto mi_004e7abc;
mi_004e7743:;
    /* 004e7743 */
    f->r[1]=MI_U32((0x0078d790u));
    /* 004e7749 */
    f->r[0]=0x00000001u;
    /* 004e774e */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004e7750 */
    MI_U32((0x0078d79cu))=(uint32_t)(0x00000a48u);
    /* 004e775a */
    MI_U32((0x00000004u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004e775e */
    if(mi_signed_less(f->flags))goto mi_004e7a67;
    /* 004e7764 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7765 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7766 */
    f->r[3]=0x00799ae6u;
    /* 004e776b */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e776c */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e776d */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[3]);
mi_004e7771:;
    /* 004e7771 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7772 */
    f->r[1]=0x00bca350u;
    /* 004e7777 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e777cu;f->boundary(0x004e7777u,0x00488030u,f);
    /* 004e777c */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004e777e */
    if((f->flags&MI_ZF))goto mi_004e7a42;
    /* 004e7784 */
    f->r[0]=MI_U32((0x00d3b1c4u));
    /* 004e7789 */
    f->r[5]=MI_U32((0x00d3b374u));
    /* 004e778f */
    f->r[6]=MI_U32((0x00d3b384u));
    /* 004e7795 */
    f->r[1]=MI_U32((0x00d3b380u));
    /* 004e779b */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 004e77a3 */
    goto mi_004e77aa;
mi_004e77a5:;
    /* 004e77a5 */
    f->r[0]=MI_U32((0x00d3b1c4u));
mi_004e77aa:;
    /* 004e77aa */
    MI_U32((0x00d3b1c8u+f->r[0]*4u))=(uint32_t)(f->r[5]);
    /* 004e77b1 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004e77b2 */
    a=f->r[0];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 004e77b5 */
    MI_U32((0x00d3b1c4u))=(uint32_t)(f->r[0]);
    /* 004e77ba */
    if(mi_signed_less(f->flags))goto mi_004e77c6;
    /* 004e77bc */
    MI_U32((0x00d3b1c4u))=(uint32_t)(0x00000000u);
mi_004e77c6:;
    /* 004e77c6 */
    f->r[7]=0x0000001fu;
mi_004e77cb:;
    /* 004e77cb */
    f->r[0]=f->r[6];
    /* 004e77cd */
    f->r[2]=f->r[6];
    /* 004e77cf */
    a=f->r[0];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 004e77d2 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004e77d5 */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004e77d8 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004e77da */
    f->r[2]=f->r[1];
    /* 004e77dc */
    a=f->r[2];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[2]=t;
    /* 004e77df */
    f->r[5]=f->r[1];
    /* 004e77e1 */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004e77e4 */
    a=f->r[5];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[5]=t;
    /* 004e77e7 */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004e77e9 */
    a=f->r[6];t=a>>1u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|((a>>0u)&1u)|mi_szp(t,32)|((a&0x80000000u)?MI_OF:0);f->r[6]=t;
    /* 004e77eb */
    a=f->r[1];t=a>>1u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|((a>>0u)&1u)|mi_szp(t,32)|((a&0x80000000u)?MI_OF:0);f->r[1]=t;
    /* 004e77ed */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e77ef */
    if((f->flags&MI_ZF))goto mi_004e77f7;
    /* 004e77f1 */
    a=f->r[6];b=0x40000000u;t=mi_alu(f,a,b,32,4);f->r[6]=t;
mi_004e77f7:;
    /* 004e77f7 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 004e77f9 */
    if((f->flags&MI_ZF))goto mi_004e7801;
    /* 004e77fb */
    a=f->r[1];b=0x40000000u;t=mi_alu(f,a,b,32,4);f->r[1]=t;
mi_004e7801:;
    /* 004e7801 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 004e7802 */
    if(!(f->flags&MI_ZF))goto mi_004e77cb;
    /* 004e7804 */
    f->r[0]=f->r[6];
    /* 004e7806 */
    MI_U32((0x00d3b380u))=(uint32_t)(f->r[1]);
    /* 004e780c */
    a=f->r[0];b=0x0000007fu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004e780f */
    f->r[2]=f->r[1];
    /* 004e7811 */
    MI_U32((0x00d3b374u))=(uint32_t)(f->r[0]);
    /* 004e7816 */
    f->r[0]=f->r[1];
    /* 004e7818 */
    a=f->r[1];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 004e781b */
    a=f->r[0];b=0x0000007fu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004e781e */
    MI_U32((0x00d3b36cu))=(uint32_t)(f->r[1]);
    /* 004e7824 */
    f->r[1]=MI_U32((0x00bca2d0u));
    /* 004e782a */
    a=f->r[2];b=0x00007fffu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004e7830 */
    MI_U32((0x00d3b368u))=(uint32_t)(f->r[0]);
    /* 004e7835 */
    f->r[0]=f->r[1];
    /* 004e7837 */
    MI_U32((0x00d3b364u))=(uint32_t)(f->r[2]);
    /* 004e783d */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004e783e */
    MI_U32((0x00d3b384u))=(uint32_t)(f->r[6]);
    /* 004e7844 */
    f->r[7]=f->r[6];
    /* 004e7846 */
    a=f->r[2];b=0x00000003u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004e7849 */
    a=f->r[6];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 004e784c */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004e784e */
    a=f->r[7];b=0x00007fffu;t=mi_alu(f,a,b,32,3);f->r[7]=t;
    /* 004e7854 */
    MI_U32((0x00d3b378u))=(uint32_t)(f->r[6]);
    /* 004e785a */
    f->r[6]=f->r[0];
    /* 004e785c */
    f->r[0]=f->r[7];
    /* 004e785e */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7860 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004e7861 */
    a=f->r[6];b=0x00000002u;t=mi_shift(f,a,b,32,1);f->r[6]=t;
    /* 004e7864 */
    a=f->r[6];mi_idiv32(f,a);
    /* 004e7866 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 004e7869 */
    MI_U32((0x00d3b370u))=(uint32_t)(f->r[7]);
    /* 004e786f */
    f->r[6]=f->r[2];
    /* 004e7871 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004e7872 */
    a=f->r[2];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004e7875 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004e7877 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004e787a */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004e787c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e787d */
    f->r[0]=(0x00000000u+f->r[1]*8u);
    /* 004e7884 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004e7886 */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 004e788c */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004e788d */
    a=f->r[2];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004e7890 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004e7892 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004e7895 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004e7897 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7898 */
    a=0x0000006bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e789a */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e789b */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e78a0u;f->boundary(0x004e789bu,0x004b4800u,f);
    /* 004e78a0 */
    f->r[1]=MI_U32((0x00d3b374u));
    /* 004e78a6 */
    f->r[7]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004e78a9 */
    a=f->r[7];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 004e78ac */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x00000028u+f->r[4])))&0xffffu)<<0);
    /* 004e78b1 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004e78b3 */
    MI_U16((0x00000028u+f->r[3]))=(uint16_t)((f->r[0]&0xffffu));
    /* 004e78b7 */
    f->r[0]=MI_U32((0x00d3b1c4u));
    /* 004e78bc */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e78bf */
    a=f->r[7];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 004e78c2 */
    MI_U32((0x00d3b1c8u+f->r[0]*4u))=(uint32_t)(f->r[1]);
    /* 004e78c9 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004e78ca */
    MI_U8((0x00bcd630u+f->r[7]))=(uint8_t)(0x00000008u);
    /* 004e78d1 */
    a=f->r[0];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 004e78d4 */
    MI_U16((0x00bcd654u+f->r[7]))=(uint16_t)((f->r[2]&0xffffu));
    /* 004e78db */
    MI_U32((0x00d3b1c4u))=(uint32_t)(f->r[0]);
    /* 004e78e0 */
    if(mi_signed_less(f->flags))goto mi_004e78ec;
    /* 004e78e2 */
    MI_U32((0x00d3b1c4u))=(uint32_t)(0x00000000u);
mi_004e78ec:;
    /* 004e78ec */
    f->r[6]=MI_U32((0x00d3b384u));
    /* 004e78f2 */
    f->r[1]=MI_U32((0x00d3b380u));
    /* 004e78f8 */
    f->r[3]=0x0000001fu;
mi_004e78fd:;
    /* 004e78fd */
    f->r[0]=f->r[6];
    /* 004e78ff */
    f->r[2]=f->r[6];
    /* 004e7901 */
    a=f->r[0];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 004e7904 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004e7907 */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004e790a */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004e790c */
    f->r[2]=f->r[1];
    /* 004e790e */
    a=f->r[2];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[2]=t;
    /* 004e7911 */
    f->r[5]=f->r[1];
    /* 004e7913 */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004e7916 */
    a=f->r[5];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[5]=t;
    /* 004e7919 */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004e791b */
    a=f->r[6];t=a>>1u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|((a>>0u)&1u)|mi_szp(t,32)|((a&0x80000000u)?MI_OF:0);f->r[6]=t;
    /* 004e791d */
    a=f->r[1];t=a>>1u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|((a>>0u)&1u)|mi_szp(t,32)|((a&0x80000000u)?MI_OF:0);f->r[1]=t;
    /* 004e791f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e7921 */
    if((f->flags&MI_ZF))goto mi_004e7929;
    /* 004e7923 */
    a=f->r[6];b=0x40000000u;t=mi_alu(f,a,b,32,4);f->r[6]=t;
mi_004e7929:;
    /* 004e7929 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 004e792b */
    if((f->flags&MI_ZF))goto mi_004e7933;
    /* 004e792d */
    a=f->r[1];b=0x40000000u;t=mi_alu(f,a,b,32,4);f->r[1]=t;
mi_004e7933:;
    /* 004e7933 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 004e7934 */
    if(!(f->flags&MI_ZF))goto mi_004e78fd;
    /* 004e7936 */
    f->r[2]=f->r[6];
    /* 004e7938 */
    f->r[0]=f->r[6];
    /* 004e793a */
    a=f->r[2];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004e793d */
    a=f->r[0];b=0x00007fffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004e7942 */
    MI_U32((0x00d3b378u))=(uint32_t)(f->r[2]);
    /* 004e7948 */
    f->r[2]=f->r[1];
    /* 004e794a */
    a=f->r[2];b=0x00007fffu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004e7950 */
    MI_U32((0x00d3b370u))=(uint32_t)(f->r[0]);
    /* 004e7955 */
    MI_U32((0x00d3b364u))=(uint32_t)(f->r[2]);
    /* 004e795b */
    f->r[2]=f->r[1];
    /* 004e795d */
    a=f->r[2];b=0x0000007fu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004e7960 */
    f->r[3]=0x0000012cu;
    /* 004e7965 */
    MI_U32((0x00d3b368u))=(uint32_t)(f->r[2]);
    /* 004e796b */
    f->r[2]=f->r[1];
    /* 004e796d */
    a=f->r[2];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004e7970 */
    f->r[5]=f->r[6];
    /* 004e7972 */
    MI_U32((0x00d3b36cu))=(uint32_t)(f->r[2]);
    /* 004e7978 */
    a=f->r[5];b=0x0000007fu;t=mi_alu(f,a,b,32,3);f->r[5]=t;
    /* 004e797b */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004e797c */
    a=f->r[3];mi_idiv32(f,a);
    /* 004e797e */
    f->r[3]=MI_U32((0x00000010u+f->r[4]));
    /* 004e7982 */
    f->r[0]=0x55555556u;
    /* 004e7987 */
    MI_U32((0x00d3b380u))=(uint32_t)(f->r[1]);
    /* 004e798d */
    MI_U32((0x00d3b384u))=(uint32_t)(f->r[6]);
    /* 004e7993 */
    MI_U32((0x00d3b374u))=(uint32_t)(f->r[5]);
    /* 004e7999 */
    MI_U16((0x00bcd62eu+f->r[7]))=(uint16_t)((f->r[2]&0xffffu));
    /* 004e79a0 */
    a=f->r[0];b=f->r[3];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 004e79a2 */
    f->r[0]=f->r[2];
    /* 004e79a4 */
    MI_U8((0x00bcd65cu+f->r[7]))=(uint8_t)(0x00000000u);
    /* 004e79ab */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 004e79ae */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004e79b0 */
    f->r[0]=MI_U32((0x00000018u+f->r[4]));
    /* 004e79b4 */
    MI_U8((0x00bcd66au+f->r[7]))=(uint8_t)(0x0000000bu);
    /* 004e79bb */
    a=(f->r[2]&0xffffu);b=MI_U16((0x00000000u+f->r[0]));t=mi_alu(f,a,b,16,0);f->r[2]=(f->r[2]&0xffff0000u)|(((t)&0xffffu)<<0);
    /* 004e79be */
    f->r[0]=f->r[3];
    /* 004e79c0 */
    f->r[3]=0x00000003u;
    /* 004e79c5 */
    MI_U16((0x00bcd61cu+f->r[7]))=(uint16_t)((f->r[2]&0xffffu));
    /* 004e79cc */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004e79cd */
    a=f->r[3];mi_idiv32(f,a);
    /* 004e79cf */
    f->r[3]=MI_U32((0x00000018u+f->r[4]));
    /* 004e79d3 */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 004e79d7 */
    a=(f->r[2]&0xffffu);b=MI_U16((0x00000002u+f->r[3]));t=mi_alu(f,a,b,16,0);f->r[2]=(f->r[2]&0xffff0000u)|(((t)&0xffffu)<<0);
    /* 004e79db */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004e79dc */
    a=f->r[0];b=0x00000009u;t=mi_alu(f,a,b,32,1);
    /* 004e79df */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004e79e3 */
    MI_U16((0x00bcd61eu+f->r[7]))=(uint16_t)((f->r[2]&0xffffu));
    /* 004e79ea */
    if(mi_signed_less(f->flags))goto mi_004e77a5;
    /* 004e79f0 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 004e79f2 */
    a=MI_U16((0x0000003au+f->r[3]));b=(f->r[6]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 004e79f6 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e7a42;
    /* 004e79f8 */
    MI_U8((0x000000a1u+f->r[3]))=(uint8_t)(0x00000000u);
    /* 004e79ff */
    MI_U16((0x0000003au+f->r[3]))=(uint16_t)((f->r[6]&0xffffu));
    /* 004e7a03 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x000000aeu+f->r[3]));
    /* 004e7a0a */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004e7a0c */
    if((f->flags&MI_ZF))goto mi_004e7a42;
    /* 004e7a0e */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 004e7a11 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004e7a14 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004e7a16 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004e7a19 */
    a=MI_U8((0x00bcd606u+f->r[0]));b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 004e7a20 */
    if((f->flags&MI_ZF))goto mi_004e7a3b;
    /* 004e7a22 */
    a=MI_U8((0x00bcd602u+f->r[0]));b=0x00000055u;t=mi_alu(f,a,b,8,1);
    /* 004e7a29 */
    if(!(f->flags&MI_ZF))goto mi_004e7a3b;
    /* 004e7a2b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7a2c */
    MI_U8((0x00bcd606u+f->r[0]))=(uint8_t)(0x00000002u);
    /* 004e7a33 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e7a38u;f->boundary(0x004e7a33u,0x004b5420u,f);
    /* 004e7a38 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004e7a3b:;
    /* 004e7a3b */
    MI_U16((0x000000aeu+f->r[3]))=(uint16_t)((f->r[6]&0xffffu));
mi_004e7a42:;
    /* 004e7a42 */
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
    /* 004e7a46 */
    f->r[1]=MI_U32((0x0078d790u));
    /* 004e7a4c */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004e7a4d */
    a=f->r[3];b=0x00000108u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 004e7a53 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004e7a55 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004e7a59 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 004e7a5d */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e7771;
    /* 004e7a63 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004e7a64 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e7a65 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004e7a66 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
mi_004e7a67:;
    /* 004e7a67 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7a69 */
    f->r[1]=0x00e92bf8u;
    /* 004e7a6e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e7a73u;f->boundary(0x004e7a6eu,0x0055f5a1u,f);
    /* 004e7a73 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e7a75 */
    if((f->flags&MI_ZF))goto mi_004e7a83;
    /* 004e7a77 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7a79 */
    f->r[1]=0x00e92bf8u;
    /* 004e7a7e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e7a83u;f->boundary(0x004e7a7eu,0x0055f561u,f);
mi_004e7a83:;
    /* 004e7a83 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7a85 */
    f->r[1]=0x00e92bf8u;
    /* 004e7a8a */
    MI_U32((0x00f55b94u))=(uint32_t)(0x00000001u);
    /* 004e7a94 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e7a99u;f->boundary(0x004e7a94u,0x0055f5a1u,f);
    /* 004e7a99 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e7a9b */
    if((f->flags&MI_ZF))goto mi_004e7aa9;
    /* 004e7a9d */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7a9f */
    f->r[1]=0x00e92bf8u;
    /* 004e7aa4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e7aa9u;f->boundary(0x004e7aa4u,0x0055f561u,f);
mi_004e7aa9:;
    /* 004e7aa9 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7aab */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7aad */
    a=0x005e4990u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7ab2 */
    f->r[1]=0x00e92bf8u;
    /* 004e7ab7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e7abcu;f->boundary(0x004e7ab7u,0x0055f449u,f);
mi_004e7abc:;
    /* 004e7abc */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e7abf */
    f->r[4]+=4;return;
}

void restore_calendar_C_004bf920(struct restore_calendar_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004bf920 */
    f->r[0]=MI_U32((0x00000004u+f->r[4]));
    /* 004bf924 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bf925 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bf926 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bf927 */
    f->r[6]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004bf92a */
    f->r[1]=0x00bca350u;
    /* 004bf92f */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004bf932 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004bf934 */
    a=f->r[6];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004bf937 */
    f->r[7]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd656u+f->r[6]));
    /* 004bf93e */
    f->r[3]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd652u+f->r[6]));
    /* 004bf945 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bf946 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bf94bu;f->boundary(0x004bf946u,0x00488030u,f);
    /* 004bf94b */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004bf94d */
    if((f->flags&MI_ZF))goto mi_004bf9b5;
    /* 004bf94f */
    f->r[0]=MI_U32((0x0078d79cu));
    /* 004bf954 */
    MI_U8((0x00bcd606u+f->r[6]))=(uint8_t)(0x00000002u);
    /* 004bf95b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004bf95d */
    if(!(f->flags&MI_ZF))goto mi_004bf9c4;
    /* 004bf95f */
    f->r[0]=f->r[3];
    /* 004bf961 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004bf964 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004bf966 */
    MI_U8((0x00799a7fu+f->r[0]*8u))=(uint8_t)(0x00000020u);
    /* 004bf96e */
    f->r[0]=f->r[7];
    /* 004bf970 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004bf973 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004bf975 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004bf976 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004bf979 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004bf97a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004bf97b */
    MI_U8((0x00799a7fu+f->r[0]))=(uint8_t)(0x00000060u);
    /* 004bf982 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00799a7eu+f->r[0])))&0xffu)<<0);
    /* 004bf988 */
    a=(f->r[1]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004bf98a */
    MI_U8((0x00799a7eu+f->r[0]))=(uint8_t)((f->r[1]&0xffu));
    /* 004bf990 */
    MI_U16((0x00799a8cu+f->r[0]))=(uint16_t)(0x00000000u);
    /* 004bf999 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e8u+f->r[0]));
    /* 004bf9a0 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*4u);
    /* 004bf9a3 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x00705eb8u+f->r[1]*8u)))&0xffffu)<<0);
    /* 004bf9ab */
    MI_U16((0x00799a18u+f->r[0]))=(uint16_t)((f->r[2]&0xffffu));
    /* 004bf9b2 */
    f->r[4]+=8;return;
mi_004bf9b5:;
    /* 004bf9b5 */
    f->r[0]=f->r[3];
    /* 004bf9b7 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004bf9ba */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004bf9bc */
    MI_U8((0x00799a80u+f->r[0]*8u))=(uint8_t)(0x00000020u);
mi_004bf9c4:;
    /* 004bf9c4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004bf9c5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004bf9c6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004bf9c7 */
    f->r[4]+=8;return;
}

void restore_calendar_C_004b03b0(struct restore_calendar_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004b03b0 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b03b1 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b03b2 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b03b3 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b03b4 */
    f->r[7]=MI_U32((0x00d056f8u));
    /* 004b03ba */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004b03bc */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004b03be */
    f->r[1]=(0x00000000u+f->r[7]+f->r[7]*2u);
    /* 004b03c1 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004b03c4 */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004b03c6 */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004b03c9 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00bcd602u+f->r[1])))&0xffu)<<0);
    /* 004b03cf */
    MI_U8((0x00bcd66au+f->r[1]))=(uint8_t)(0x0000000bu);
    /* 004b03d6 */
    MI_U32((0x00bcd778u+f->r[1]))=(uint32_t)(f->r[3]);
    /* 004b03dc */
    f->r[2]=(0x00000000u+f->r[0]*8u);
    /* 004b03e3 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004b03e5 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd654u+f->r[1]));
    /* 004b03ec */
    f->r[6]=f->r[0];
    /* 004b03ee */
    f->r[2]=MI_U32((0x007090ecu+f->r[2]*8u));
    /* 004b03f5 */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004b03f8 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004b03fa */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004b03fd */
    a=MI_U8((0x007999d8u+f->r[6]));b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004b0404 */
    if((f->flags&MI_ZF))goto mi_004b0412;
    /* 004b0406 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004b0407 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b0408 */
    MI_U8((0x00bcd606u+f->r[1]))=(uint8_t)(0x00000002u);
    /* 004b040f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004b0410 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004b0411 */
    f->r[4]+=4;return;
mi_004b0412:;
    /* 004b0412 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00bcd5f9u+f->r[1])))&0xffu)<<0);
    /* 004b0418 */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004b041a */
    a=(f->r[0]&0xffu);b=0x00000006u;t=mi_alu(f,a,b,8,1);
    /* 004b041c */
    MI_U8((0x00bcd5f9u+f->r[1]))=(uint8_t)((f->r[0]&0xffu));
    /* 004b0422 */
    if((f->flags&MI_CF))goto mi_004b042a;
    /* 004b0424 */
    MI_U8((0x00bcd5f9u+f->r[1]))=(uint8_t)((f->r[3]&0xffu));
mi_004b042a:;
    /* 004b042a */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00bcd630u+f->r[1])))&0xffu)<<0);
    /* 004b0430 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b0431 */
    a=(f->r[0]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 004b0433 */
    if(!(f->flags&MI_ZF))goto mi_004b0449;
    /* 004b0435 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b0436 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b043bu;f->boundary(0x004b0436u,0x004b1d50u,f);
    /* 004b043b */
    f->r[7]=MI_U32((0x00d056f8u));
    /* 004b0441 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b0444 */
    goto mi_004b0713;
mi_004b0449:;
    /* 004b0449 */
    a=(f->r[0]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,1);
    /* 004b044b */
    if(!(f->flags&MI_ZF))goto mi_004b04a9;
    /* 004b044d */
    MI_U8((0x00bcd65du+f->r[1]))=(uint8_t)(0x00000001u);
    /* 004b0454 */
    MI_U8((0x00bcd5f9u+f->r[1]))=(uint8_t)((f->r[3]&0xffu));
    /* 004b045a */
    a=MI_U16((0x00bcd62eu+f->r[1]));b=f->flags&MI_CF;t=mi_alu(f,a,1,16,1);f->flags=(f->flags&~MI_CF)|b;MI_U16((0x00bcd62eu+f->r[1]))=(uint16_t)(t);
    /* 004b0461 */
    a=MI_U16((0x00bcd62eu+f->r[1]));b=(f->r[3]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 004b0468 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004b0713;
    /* 004b046e */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b046f */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b0474u;f->boundary(0x004b046fu,0x004b2d20u,f);
    /* 004b0474 */
    f->r[7]=MI_U32((0x00d056f8u));
    /* 004b047a */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b047d */
    f->r[0]=(0x00000000u+f->r[7]+f->r[7]*2u);
    /* 004b0480 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004b0483 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004b0485 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004b0488 */
    a=MI_U16((0x00bcd632u+f->r[0]));b=(f->r[3]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 004b048f */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004b049d;
    /* 004b0491 */
    MI_U8((0x00bcd606u+f->r[0]))=(uint8_t)(0x00000002u);
    /* 004b0498 */
    goto mi_004b0713;
mi_004b049d:;
    /* 004b049d */
    MI_U8((0x00bcd630u+f->r[0]))=(uint8_t)(0x00000009u);
    /* 004b04a4 */
    goto mi_004b0713;
mi_004b04a9:;
    /* 004b04a9 */
    a=(f->r[0]&0xffu);b=0x00000009u;t=mi_alu(f,a,b,8,1);
    /* 004b04ab */
    if(!(f->flags&MI_ZF))goto mi_004b0551;
    /* 004b04b1 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b04b2 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b04b3 */
    MI_U8((0x00bcd65du+f->r[1]))=(uint8_t)((f->r[3]&0xffu));
    /* 004b04b9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b04beu;f->boundary(0x004b04b9u,0x004a5da0u,f);
    /* 004b04be */
    f->r[7]=MI_U32((0x00d056f8u));
    /* 004b04c4 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b04c7 */
    f->r[0]=(0x00000000u+f->r[7]+f->r[7]*2u);
    /* 004b04ca */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004b04cd */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004b04cf */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004b04d2 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00bcd609u+f->r[0])))&0xffu)<<0);
    /* 004b04d8 */
    a=(f->r[1]&0xffu);b=0x00000009u;t=mi_alu(f,a,b,8,1);
    /* 004b04db */
    if(!(f->flags&MI_ZF))goto mi_004b0501;
    /* 004b04dd */
    MI_U8((0x00bcd630u+f->r[0]))=(uint8_t)(0x00000008u);
    /* 004b04e4 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b04e5 */
    MI_U16((0x00bcd62eu+f->r[0]))=(uint16_t)(0x00000001u);
    /* 004b04ee */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b04f3u;f->boundary(0x004b04eeu,0x004b3250u,f);
    /* 004b04f3 */
    f->r[7]=MI_U32((0x00d056f8u));
    /* 004b04f9 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b04fc */
    goto mi_004b0713;
mi_004b0501:;
    /* 004b0501 */
    a=(f->r[1]&0xffu);b=0x0000000au;t=mi_alu(f,a,b,8,1);
    /* 004b0504 */
    if(!(f->flags&MI_ZF))goto mi_004b0520;
    /* 004b0506 */
    MI_U16((0x00799a06u+f->r[6]))=(uint16_t)((f->r[3]&0xffffu));
    /* 004b050d */
    MI_U16((0x007999f8u+f->r[6]))=(uint16_t)((f->r[3]&0xffffu));
    /* 004b0514 */
    MI_U8((0x00bcd606u+f->r[0]))=(uint8_t)(0x00000002u);
    /* 004b051b */
    goto mi_004b0713;
mi_004b0520:;
    /* 004b0520 */
    a=(f->r[1]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,1);
    /* 004b0523 */
    if(!(f->flags&MI_ZF))goto mi_004b0713;
    /* 004b0529 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x007999deu+f->r[6])))&0xffffu)<<0);
    /* 004b0530 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x007999e0u+f->r[6])))&0xffffu)<<0);
    /* 004b0537 */
    MI_U8((0x00bcd630u+f->r[0]))=(uint8_t)(0x0000000au);
    /* 004b053e */
    MI_U16((0x00bcd61cu+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004b0545 */
    MI_U16((0x00bcd61eu+f->r[0]))=(uint16_t)((f->r[2]&0xffffu));
    /* 004b054c */
    goto mi_004b0713;
mi_004b0551:;
    /* 004b0551 */
    a=(f->r[0]&0xffu);b=0x0000000au;t=mi_alu(f,a,b,8,1);
    /* 004b0553 */
    if(!(f->flags&MI_ZF))goto mi_004b06c9;
    /* 004b0559 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd654u+f->r[1]));
    /* 004b0560 */
    f->r[2]=f->r[0];
    /* 004b0562 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004b0565 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004b0567 */
    f->r[5]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00799a2cu+f->r[2]*8u));
    /* 004b056f */
    f->r[0]=(0x00799a2cu+f->r[2]*8u);
    /* 004b0576 */
    f->r[2]=MI_U32((0x0078d79cu));
    /* 004b057c */
    f->r[6]=(0xfffffc18u+f->r[2]);
    /* 004b0582 */
    a=f->r[5];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004b0584 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004b058f;
    /* 004b0586 */
    f->r[6]=(0xfffffc18u+f->r[2]);
    /* 004b058c */
    MI_U16((0x00000000u+f->r[0]))=(uint16_t)((f->r[6]&0xffffu));
mi_004b058f:;
    /* 004b058f */
    a=MI_U16((0x00000000u+f->r[0]));b=(f->r[3]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 004b0592 */
    if(!mi_signed_less(f->flags))goto mi_004b0597;
    /* 004b0594 */
    MI_U16((0x00000000u+f->r[0]))=(uint16_t)((f->r[3]&0xffffu));
mi_004b0597:;
    /* 004b0597 */
    a=f->r[2];b=0x000003e8u;t=mi_alu(f,a,b,32,1);
    /* 004b059d */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004b0713;
    /* 004b05a3 */
    f->r[0]=MI_U32((0x00d3b1c4u));
    /* 004b05a8 */
    f->r[2]=MI_U32((0x00d3b374u));
    /* 004b05ae */
    MI_U8((0x00bcd630u+f->r[1]))=(uint8_t)(0x0000000bu);
    /* 004b05b5 */
    MI_U32((0x00d3b1c8u+f->r[0]*4u))=(uint32_t)(f->r[2]);
    /* 004b05bc */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004b05bd */
    a=f->r[0];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 004b05c0 */
    MI_U32((0x00d3b1c4u))=(uint32_t)(f->r[0]);
    /* 004b05c5 */
    if(mi_signed_less(f->flags))goto mi_004b05cd;
    /* 004b05c7 */
    MI_U32((0x00d3b1c4u))=(uint32_t)(f->r[3]);
mi_004b05cd:;
    /* 004b05cd */
    f->r[2]=MI_U32((0x00d3b384u));
    /* 004b05d3 */
    f->r[0]=MI_U32((0x00d3b380u));
    /* 004b05d8 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x0000001fu);
mi_004b05e0:;
    /* 004b05e0 */
    f->r[6]=f->r[2];
    /* 004b05e2 */
    f->r[3]=f->r[2];
    /* 004b05e4 */
    a=f->r[6];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[6]=t;
    /* 004b05e7 */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 004b05ea */
    a=f->r[3];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[3]=t;
    /* 004b05ed */
    a=f->r[6];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 004b05ef */
    f->r[3]=f->r[0];
    /* 004b05f1 */
    a=f->r[3];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[3]=t;
    /* 004b05f4 */
    f->r[5]=f->r[0];
    /* 004b05f6 */
    a=f->r[3];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[3]=t;
    /* 004b05f9 */
    a=f->r[5];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[5]=t;
    /* 004b05fc */
    a=f->r[3];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004b05fe */
    a=f->r[2];t=a>>1u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|((a>>0u)&1u)|mi_szp(t,32)|((a&0x80000000u)?MI_OF:0);f->r[2]=t;
    /* 004b0600 */
    a=f->r[0];t=a>>1u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|((a>>0u)&1u)|mi_szp(t,32)|((a&0x80000000u)?MI_OF:0);f->r[0]=t;
    /* 004b0602 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004b0604 */
    if((f->flags&MI_ZF))goto mi_004b060c;
    /* 004b0606 */
    a=f->r[2];b=0x40000000u;t=mi_alu(f,a,b,32,4);f->r[2]=t;
mi_004b060c:;
    /* 004b060c */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 004b060e */
    if((f->flags&MI_ZF))goto mi_004b0615;
    /* 004b0610 */
    a=f->r[0];b=0x40000000u;t=mi_alu(f,a,b,32,4);f->r[0]=t;
mi_004b0615:;
    /* 004b0615 */
    f->r[6]=MI_U32((0x00000010u+f->r[4]));
    /* 004b0619 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 004b061a */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 004b061e */
    if(!(f->flags&MI_ZF))goto mi_004b05e0;
    /* 004b0620 */
    MI_U32((0x00d3b384u))=(uint32_t)(f->r[2]);
    /* 004b0626 */
    f->r[3]=f->r[2];
    /* 004b0628 */
    f->r[6]=f->r[2];
    /* 004b062a */
    a=f->r[2];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004b062d */
    MI_U32((0x00d3b378u))=(uint32_t)(f->r[2]);
    /* 004b0633 */
    f->r[2]=f->r[0];
    /* 004b0635 */
    a=f->r[2];b=0x00007fffu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004b063b */
    a=f->r[6];b=0x0000007fu;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 004b063e */
    MI_U32((0x00d3b364u))=(uint32_t)(f->r[2]);
    /* 004b0644 */
    MI_U32((0x00d3b380u))=(uint32_t)(f->r[0]);
    /* 004b0649 */
    f->r[2]=f->r[0];
    /* 004b064b */
    MI_U32((0x00d3b374u))=(uint32_t)(f->r[6]);
    /* 004b0651 */
    f->r[6]=MI_U32((0x00bca2d0u));
    /* 004b0657 */
    a=f->r[0];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004b065a */
    a=f->r[2];b=0x0000007fu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004b065d */
    MI_U32((0x00d3b36cu))=(uint32_t)(f->r[0]);
    /* 004b0662 */
    f->r[0]=f->r[6];
    /* 004b0664 */
    MI_U32((0x00d3b368u))=(uint32_t)(f->r[2]);
    /* 004b066a */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004b066b */
    a=f->r[2];b=0x00000003u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004b066e */
    a=f->r[3];b=0x00007fffu;t=mi_alu(f,a,b,32,3);f->r[3]=t;
    /* 004b0674 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004b0676 */
    MI_U32((0x00d3b370u))=(uint32_t)(f->r[3]);
    /* 004b067c */
    f->r[5]=f->r[0];
    /* 004b067e */
    f->r[0]=f->r[3];
    /* 004b0680 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004b0681 */
    a=f->r[5];b=0x00000002u;t=mi_shift(f,a,b,32,1);f->r[5]=t;
    /* 004b0684 */
    a=f->r[5];mi_idiv32(f,a);
    /* 004b0686 */
    f->r[0]=(0x00000000u+f->r[6]+f->r[6]*2u);
    /* 004b0689 */
    f->r[3]=f->r[2];
    /* 004b068b */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004b068c */
    a=f->r[2];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004b068f */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004b0691 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004b0694 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004b0696 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004b0699 */
    MI_U16((0x00bcd61cu+f->r[1]))=(uint16_t)((f->r[0]&0xffffu));
    /* 004b06a0 */
    f->r[0]=(0x00000000u+f->r[6]*8u);
    /* 004b06a7 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004b06a9 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004b06aa */
    a=f->r[2];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004b06ad */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004b06af */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004b06b2 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004b06b4 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004b06b7 */
    MI_U16((0x00bcd61eu+f->r[1]))=(uint16_t)((f->r[0]&0xffffu));
    /* 004b06be */
    MI_U16((0x00bcd63cu+f->r[1]))=(uint16_t)(0x00000000u);
    /* 004b06c7 */
    goto mi_004b0713;
mi_004b06c9:;
    /* 004b06c9 */
    a=(f->r[0]&0xffu);b=0x0000000bu;t=mi_alu(f,a,b,8,1);
    /* 004b06cb */
    if(!(f->flags&MI_ZF))goto mi_004b070c;
    /* 004b06cd */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b06ce */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b06cf */
    MI_U8((0x00bcd65du+f->r[1]))=(uint8_t)((f->r[3]&0xffu));
    /* 004b06d5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b06dau;f->boundary(0x004b06d5u,0x004a5da0u,f);
    /* 004b06da */
    f->r[7]=MI_U32((0x00d056f8u));
    /* 004b06e0 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b06e3 */
    f->r[0]=(0x00000000u+f->r[7]+f->r[7]*2u);
    /* 004b06e6 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004b06e9 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004b06eb */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004b06ee */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00bcd609u+f->r[0])))&0xffu)<<0);
    /* 004b06f4 */
    a=(f->r[1]&0xffu);b=0x00000009u;t=mi_alu(f,a,b,8,1);
    /* 004b06f7 */
    if((f->flags&MI_ZF))goto mi_004b0703;
    /* 004b06f9 */
    a=(f->r[1]&0xffu);b=0x0000000au;t=mi_alu(f,a,b,8,1);
    /* 004b06fc */
    if((f->flags&MI_ZF))goto mi_004b0703;
    /* 004b06fe */
    a=(f->r[1]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,1);
    /* 004b0701 */
    if(!(f->flags&MI_ZF))goto mi_004b0713;
mi_004b0703:;
    /* 004b0703 */
    MI_U8((0x00bcd606u+f->r[0]))=(uint8_t)(0x00000002u);
    /* 004b070a */
    goto mi_004b0713;
mi_004b070c:;
    /* 004b070c */
    MI_U8((0x00bcd630u+f->r[1]))=(uint8_t)(0x00000008u);
mi_004b0713:;
    /* 004b0713 */
    f->r[0]=(0x00000000u+f->r[7]+f->r[7]*2u);
    /* 004b0716 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004b0717 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004b071a */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004b071c */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004b071f */
    a=MI_U8((0x00bcd630u+f->r[0]));b=0x0000000au;t=mi_alu(f,a,b,8,1);
    /* 004b0726 */
    if(!(f->flags&MI_ZF))goto mi_004b0743;
    /* 004b0728 */
    f->r[2]=MI_U32((0x00f5714cu));
    /* 004b072e */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004b0730 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00bcd5f9u+f->r[0])))&0xffu)<<0);
    /* 004b0736 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004b0737 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004b0739 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b073a */
    MI_U32((0x00bcd5fcu+f->r[0]))=(uint32_t)(f->r[1]);
    /* 004b0740 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004b0741 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004b0742 */
    f->r[4]+=4;return;
mi_004b0743:;
    /* 004b0743 */
    f->r[1]=MI_U32((0x00f5713cu));
    /* 004b0749 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004b074b */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00bcd5f9u+f->r[0])))&0xffu)<<0);
    /* 004b0751 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004b0752 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004b0754 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b0755 */
    MI_U32((0x00bcd5fcu+f->r[0]))=(uint32_t)(f->r[2]);
    /* 004b075b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004b075c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004b075d */
    f->r[4]+=4;return;
}
