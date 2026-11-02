/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "map_gates.h"

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

void restore_C_00482810(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00482810 */
    f->r[2]=MI_U32((0x0000000cu+f->r[4]));
    /* 00482814 */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 00482818 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00482819 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048281a */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048281b */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048281c */
    f->r[7]=MI_U32((0x00000014u+f->r[4]));
    /* 00482820 */
    f->r[1]=(0x00000000u+f->r[7]+f->r[2]*1u);
    /* 00482823 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 00482825 */
    f->r[6]=(0xffffffffu+f->r[1]+f->r[0]*1u);
    /* 00482829 */
    f->r[1]=MI_U32((0x00000018u+f->r[4]));
    /* 0048282d */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0048282f */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00482831 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 00482833 */
    f->r[2]=(0xffffffffu+f->r[2]+f->r[0]*1u);
    /* 00482837 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0048283b */
    if(!mi_signed_less(f->flags))goto mi_0048283f;
    /* 0048283d */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
mi_0048283f:;
    /* 0048283f */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00482841 */
    if(!mi_signed_less(f->flags))goto mi_00482845;
    /* 00482843 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_00482845:;
    /* 00482845 */
    f->r[0]=MI_U32((0x00bca2d0u));
    /* 0048284a */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0048284c */
    if(mi_signed_less(f->flags))goto mi_00482851;
    /* 0048284e */
    f->r[6]=(0xffffffffu+f->r[0]);
mi_00482851:;
    /* 00482851 */
    f->r[0]=MI_U32((0x00bca2d4u));
    /* 00482856 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00482858 */
    if(mi_signed_less(f->flags))goto mi_00482861;
    /* 0048285a */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0048285b */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0048285f */
    f->r[2]=f->r[0];
mi_00482861:;
    /* 00482861 */
    f->r[3]=MI_U32((0x00bca2d8u));
    /* 00482867 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 0048286a */
    f->r[5]=f->r[1];
    /* 0048286c */
    f->r[0]=(0x00000000u+f->r[1]+f->r[0]*2u);
    /* 0048286f */
    MI_U32((0x00d3b4e0u))=(uint32_t)(f->r[5]);
    /* 00482875 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 00482878 */
    f->r[0]=(0x00000000u+f->r[3]+f->r[0]*4u);
    /* 0048287b */
    f->r[3]=f->r[7];
    /* 0048287d */
    a=f->r[3];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 0048287f */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00482881 */
    a=f->r[3];b=0x000000e3u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 00482887 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 00482889 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004828c9;
mi_0048288b:;
    /* 0048288b */
    f->r[1]=f->r[7];
    /* 0048288d */
    a=f->r[7];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 0048288f */
    MI_U32((0x00d3b4e4u))=(uint32_t)(f->r[1]);
    /* 00482895 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004828bc;
    /* 00482897 */
    f->r[2]=(0x00af598cu+f->r[0]*4u);
mi_0048289e:;
    /* 0048289e */
    a=f->r[0];b=MI_U32((0x00000024u+f->r[4]));t=mi_alu(f,a,b,32,1);
    /* 004828a2 */
    if((f->flags&MI_ZF))goto mi_004828a9;
    /* 004828a4 */
    a=MI_U32((0x00000000u+f->r[2]));b=0x00000000u;t=mi_alu(f,a,b,32,1);
    /* 004828a7 */
    if((f->flags&MI_ZF))goto mi_004828d0;
mi_004828a9:;
    /* 004828a9 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004828aa */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004828ab */
    a=f->r[2];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004828ae */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004828b0 */
    MI_U32((0x00d3b4e4u))=(uint32_t)(f->r[1]);
    /* 004828b6 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0048289e;
    /* 004828b8 */
    f->r[2]=MI_U32((0x00000014u+f->r[4]));
mi_004828bc:;
    /* 004828bc */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 004828bd */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004828bf */
    a=f->r[5];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004828c1 */
    MI_U32((0x00d3b4e0u))=(uint32_t)(f->r[5]);
    /* 004828c7 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0048288b;
mi_004828c9:;
    /* 004828c9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004828ca */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004828cb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004828cc */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004828ce */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004828cf */
    f->r[4]+=4;return;
mi_004828d0:;
    /* 004828d0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004828d1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004828d2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004828d3 */
    f->r[0]=0x00000001u;
    /* 004828d8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004828d9 */
    f->r[4]+=4;return;
}
