/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "road_scan_model.h"

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

void restore_C_00441040(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00441040 */
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
    /* 00441044 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 00441047 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00441048 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 0044104a */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 0044104c */
    if(!(f->flags&MI_ZF))goto mi_00441055;
    /* 0044104e */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,5);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00441050 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00441051 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00441054 */
    f->r[4]+=4;return;
mi_00441055:;
    /* 00441055 */
    f->r[2]=MI_U32((0x00000018u+f->r[4]));
    /* 00441059 */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 0044105d */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 00441060 */
    MI_U32((0x00000004u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 00441064 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00441071;
    /* 00441066 */
    f->r[1]=f->r[0];
    /* 00441068 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0044106a */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0044106b */
    MI_U32((0x00000008u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0044106f */
    goto mi_00441075;
mi_00441071:;
    /* 00441071 */
    MI_U32((0x00000008u+f->r[4]))=(uint32_t)(f->r[0]);
mi_00441075:;
    /* 00441075 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00441076 */
    f->r[6]=MI_U32((0x00000018u+f->r[4]));
    /* 0044107a */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0044107d */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00441082;
    /* 0044107f */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00441081 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
mi_00441082:;
    /* 00441082 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00441086 */
    f->r[0]=MI_U32((0x0000000cu+f->r[4]));
    /* 0044108a */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 0044108c */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 00441090 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00441117;
    /* 00441096 */
    f->r[0]=MI_U32((0x00000018u+f->r[4]));
    /* 0044109a */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044109b */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
mi_0044109c:;
    /* 0044109c */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 0044109e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004410a0 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00441104;
    /* 004410a2 */
    f->r[0]=MI_U32((0x0000002cu+f->r[4]));
    /* 004410a6 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 004410a8 */
    f->r[3]=(0x00000000u+f->r[0]+f->r[3]*8u);
mi_004410ab:;
    /* 004410ab */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 004410af */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004410b1 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004410b3 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004410df;
mi_004410b5:;
    /* 004410b5 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004410b7 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004410b9 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004410d6;
    /* 004410bb */
    f->r[0]=(0x00000000u+f->r[7]+f->r[2]*1u);
    /* 004410be */
    a=f->r[0];b=MI_U32((0x0000001cu+f->r[4]));t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004410c2 */
    f->r[0]=(0x00775614u+f->r[0]*4u);
mi_004410c9:;
    /* 004410c9 */
    a=MI_U32((0x00000000u+f->r[0]));b=0x00000000u;t=mi_alu(f,a,b,32,1);
    /* 004410cc */
    if(!(f->flags&MI_ZF))goto mi_004410f4;
    /* 004410ce */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004410cf */
    a=f->r[0];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004410d2 */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004410d4 */
    if(mi_signed_less(f->flags))goto mi_004410c9;
mi_004410d6:;
    /* 004410d6 */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 004410da */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004410db */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004410dd */
    if(mi_signed_less(f->flags))goto mi_004410b5;
mi_004410df:;
    /* 004410df */
    f->r[1]=MI_U32((0x0000001cu+f->r[4]));
    /* 004410e3 */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(f->r[5]);
    /* 004410e5 */
    MI_U32((0x00000004u+f->r[3]))=(uint32_t)(f->r[1]);
    /* 004410e8 */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
    /* 004410ec */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004410ed */
    a=f->r[3];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 004410f0 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[1]);
mi_004410f4:;
    /* 004410f4 */
    f->r[0]=MI_U32((0x00000020u+f->r[4]));
    /* 004410f8 */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 004410f9 */
    a=f->r[7];b=0x00000005u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004410fc */
    a=f->r[5];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004410fe */
    if(mi_signed_less(f->flags))goto mi_004410ab;
    /* 00441100 */
    f->r[3]=MI_U32((0x00000010u+f->r[4]));
mi_00441104:;
    /* 00441104 */
    f->r[1]=MI_U32((0x0000001cu+f->r[4]));
    /* 00441108 */
    f->r[2]=MI_U32((0x00000014u+f->r[4]));
    /* 0044110c */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0044110d */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0044110f */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00441113 */
    if(mi_signed_less(f->flags))goto mi_0044109c;
    /* 00441115 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00441116 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
mi_00441117:;
    /* 00441117 */
    f->r[2]=MI_U32((0x00000020u+f->r[4]));
    /* 0044111b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044111c */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 0044111e */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[3]);
    /* 00441120 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00441121 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00441124 */
    f->r[4]+=4;return;
}
