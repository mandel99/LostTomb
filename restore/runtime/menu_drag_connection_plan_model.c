/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "connection_plan_model.h"

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

void restore_C_0047eda0(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0047eda0 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 0047eda3 */
    f->r[0]=MI_U32((0x00705fe4u));
    /* 0047eda8 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047eda9 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047edaa */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047edab */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047edac */
    a=0x00032c40u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047edb1 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 0047edb3 */
    a=0x00af598cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047edb8 */
    a=0x009de62cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047edbd */
    MI_U32((0x00000030u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0047edc1 */
    MI_U32((0x00bc9724u))=(uint32_t)(f->r[3]);
    /* 0047edc7 */
    MI_U32((0x00bc970cu))=(uint32_t)(f->r[3]);
    /* 0047edcd */
    MI_U32((0x00bc9708u))=(uint32_t)(f->r[3]);
    /* 0047edd3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047edd8u;f->boundary(0x0047edd3u,0x0040f860u,f);
    /* 0047edd8 */
    a=0x0000cb10u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047eddd */
    a=0x00ae8e7cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047ede2 */
    a=0x009d1b1cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047ede7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047edecu;f->boundary(0x0047ede7u,0x0040f860u,f);
    /* 0047edec */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047eded */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047edf2u;f->boundary(0x0047ededu,0x00485830u,f);
    /* 0047edf2 */
    f->r[7]=MI_U32((0x0000004cu+f->r[4]));
    /* 0047edf6 */
    f->r[6]=MI_U32((0x00000054u+f->r[4]));
    /* 0047edfa */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047edfd */
    a=f->r[7];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 0047edff */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0047ee07;
    /* 0047ee01 */
    f->r[1]=f->r[7];
    /* 0047ee03 */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0047ee05 */
    goto mi_0047ee11;
mi_0047ee07:;
    /* 0047ee07 */
    if(!mi_signed_less(f->flags))goto mi_0047ee0f;
    /* 0047ee09 */
    f->r[1]=f->r[6];
    /* 0047ee0b */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0047ee0d */
    goto mi_0047ee11;
mi_0047ee0f:;
    /* 0047ee0f */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_0047ee11:;
    /* 0047ee11 */
    f->r[3]=MI_U32((0x00000034u+f->r[4]));
    /* 0047ee15 */
    f->r[5]=MI_U32((0x0000003cu+f->r[4]));
    /* 0047ee19 */
    a=f->r[3];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 0047ee1b */
    MI_U32((0x00d3b4fcu))=(uint32_t)(f->r[1]);
    /* 0047ee21 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0047ee29;
    /* 0047ee23 */
    f->r[0]=f->r[3];
    /* 0047ee25 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0047ee27 */
    goto mi_0047ee33;
mi_0047ee29:;
    /* 0047ee29 */
    if(!mi_signed_less(f->flags))goto mi_0047ee31;
    /* 0047ee2b */
    f->r[0]=f->r[5];
    /* 0047ee2d */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0047ee2f */
    goto mi_0047ee33;
mi_0047ee31:;
    /* 0047ee31 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_0047ee33:;
    /* 0047ee33 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0047ee35 */
    MI_U32((0x00d3b4f8u))=(uint32_t)(f->r[0]);
    /* 0047ee3a */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0047ee3e */
    if(mi_signed_less(f->flags))goto mi_0047ee44;
    /* 0047ee40 */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(f->r[1]);
mi_0047ee44:;
    /* 0047ee44 */
    f->r[0]=MI_U32((0x0000002cu+f->r[4]));
    /* 0047ee48 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0047ee4a */
    if((f->flags&MI_ZF))goto mi_0047ee5f;
    /* 0047ee4c */
    f->r[0]=MI_U32((0x006059e4u));
    /* 0047ee51 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0047ee53 */
    if(!(f->flags&MI_ZF))goto mi_0047ee5f;
    /* 0047ee55 */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 0047ee5d */
    goto mi_0047eec2;
mi_0047ee5f:;
    /* 0047ee5f */
    f->r[0]=MI_U32((0x00000034u+f->r[4]));
    /* 0047ee63 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0047ee65 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0047eec2;
    /* 0047ee67 */
    f->r[1]=(0x00000000u+f->r[3]+f->r[3]*8u);
    /* 0047ee6a */
    f->r[0]=(0x00000000u+f->r[3]+f->r[1]*2u);
    /* 0047ee6d */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0047ee70 */
    f->r[0]=MI_U32((0x00bca2d8u));
    /* 0047ee75 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[2]*4u);
    /* 0047ee78 */
    f->r[0]=(0xffffff1bu+f->r[1]+f->r[7]*1u);
    /* 0047ee7f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047ee80 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047ee85u;f->boundary(0x0047ee80u,0x0047f190u,f);
    /* 0047ee85 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047ee88 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0047ee8a */
    if((f->flags&MI_ZF))goto mi_0047ee98;
    /* 0047ee8c */
    MI_U32((0x00bc970cu))=(uint32_t)(0x00000002u);
    /* 0047ee96 */
    goto mi_0047eec2;
mi_0047ee98:;
    /* 0047ee98 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047ee9a */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047ee9c */
    a=0x0000ffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047eea1 */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047eea3 */
    f->r[2]=(0xffffffffu+f->r[3]);
    /* 0047eea6 */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047eea8 */
    f->r[0]=(0xffffffffu+f->r[7]);
    /* 0047eeab */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047eeac */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047eead */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047eeb2u;f->boundary(0x0047eeadu,0x0047c880u,f);
    /* 0047eeb2 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047eeb5 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,0,a,32,1);f->r[0]=t;
    /* 0047eeb7 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,2);f->r[0]=t;
    /* 0047eeb9 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 0047eebc */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0047eebd */
    MI_U32((0x00bc970cu))=(uint32_t)(f->r[0]);
mi_0047eec2:;
    /* 0047eec2 */
    f->r[1]=(0x00000000u+f->r[5]+f->r[5]*8u);
    /* 0047eec6 */
    f->r[0]=(0x00000000u+f->r[5]+f->r[1]*2u);
    /* 0047eeca */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0047eecd */
    f->r[0]=MI_U32((0x00bca2d8u));
    /* 0047eed2 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[2]*4u);
    /* 0047eed5 */
    f->r[0]=(0xffffff1bu+f->r[1]+f->r[6]*1u);
    /* 0047eedc */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047eedd */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047eee2u;f->boundary(0x0047eeddu,0x0047f190u,f);
    /* 0047eee2 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047eee5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0047eee7 */
    if((f->flags&MI_ZF))goto mi_0047eef0;
    /* 0047eee9 */
    f->r[0]=0x00000002u;
    /* 0047eeee */
    goto mi_0047ef13;
mi_0047eef0:;
    /* 0047eef0 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047eef2 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047eef4 */
    a=0x0000ffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047eef9 */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047eefb */
    f->r[2]=(0xffffffffu+f->r[5]);
    /* 0047eefe */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047ef00 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0047ef01 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047ef02 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047ef03 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047ef08u;f->boundary(0x0047ef03u,0x0047c880u,f);
    /* 0047ef08 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047ef0b */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,0,a,32,1);f->r[0]=t;
    /* 0047ef0d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,2);f->r[0]=t;
    /* 0047ef0f */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 0047ef12 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
mi_0047ef13:;
    /* 0047ef13 */
    f->r[1]=MI_U32((0x00bc970cu));
    /* 0047ef19 */
    MI_U32((0x00bc9708u))=(uint32_t)(f->r[0]);
    /* 0047ef1e */
    a=f->r[1];b=0xffffffffu;t=mi_alu(f,a,b,32,1);
    /* 0047ef21 */
    if((f->flags&MI_ZF))goto mi_0047f17c;
    /* 0047ef27 */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,1);
    /* 0047ef2a */
    if((f->flags&MI_ZF))goto mi_0047f17c;
    /* 0047ef30 */
    a=f->r[1];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0047ef33 */
    if(!(f->flags&MI_ZF))goto mi_0047ef44;
    /* 0047ef35 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0047ef37 */
    if(!(f->flags&MI_ZF))goto mi_0047ef44;
    /* 0047ef39 */
    a=MI_U32((0x00000034u+f->r[4]));b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 0047ef3e */
    if(mi_signed_less(f->flags))goto mi_0047f17c;
mi_0047ef44:;
    /* 0047ef44 */
    f->r[0]=MI_U32((0x00000034u+f->r[4]));
    /* 0047ef48 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0047ef4a */
    if(!(f->flags&MI_ZF))goto mi_0047ef63;
    /* 0047ef4c */
    f->r[0]=MI_U32((0x00706cb4u));
    /* 0047ef51 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0047ef52 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0047ef53 */
    MI_U32((0x00bc9724u))=(uint32_t)(f->r[0]);
    /* 0047ef58 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0047ef59 */
    f->r[0]=0x00000001u;
    /* 0047ef5e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0047ef5f */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047ef62 */
    f->r[4]+=4;return;
mi_0047ef63:;
    /* 0047ef63 */
    f->r[1]=MI_U32((0x0000002cu+f->r[4]));
    /* 0047ef67 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047ef69 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047ef6a */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047ef6b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047ef6c */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047ef71u;f->boundary(0x0047ef6cu,0x0051b680u,f);
    /* 0047ef71 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047ef74 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0047ef76 */
    if((f->flags&MI_ZF))goto mi_0047f17c;
    /* 0047ef7c */
    f->r[0]=MI_U32((0x00bc970cu));
    /* 0047ef81 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0047ef83 */
    if((f->flags&MI_ZF))goto mi_0047efa8;
    /* 0047ef85 */
    f->r[6]=(0xffffffffu+f->r[3]);
    /* 0047ef88 */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047ef8a */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 0047ef8b */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047ef8d */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047ef8e */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047ef8f */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047ef91 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047ef96u;f->boundary(0x0047ef91u,0x0047ca50u,f);
    /* 0047ef96 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047ef98 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047ef9a */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047ef9c */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047ef9d */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047ef9e */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047efa0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047efa5u;f->boundary(0x0047efa0u,0x0047d360u,f);
    /* 0047efa5 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0047efa8:;
    /* 0047efa8 */
    f->r[0]=MI_U32((0x00bc9708u));
    /* 0047efad */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0047efaf */
    if((f->flags&MI_ZF))goto mi_0047efda;
    /* 0047efb1 */
    f->r[2]=MI_U32((0x00000038u+f->r[4]));
    /* 0047efb5 */
    f->r[6]=(0xffffffffu+f->r[5]);
    /* 0047efb8 */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047efba */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047efbc */
    f->r[7]=(0xffffffffu+f->r[2]);
    /* 0047efbf */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047efc0 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047efc1 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047efc3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047efc8u;f->boundary(0x0047efc3u,0x0047ca50u,f);
    /* 0047efc8 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047efca */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047efcc */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047efce */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047efcf */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047efd0 */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047efd2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047efd7u;f->boundary(0x0047efd2u,0x0047d360u,f);
    /* 0047efd7 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0047efda:;
    /* 0047efda */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0047efdc */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(0x00002710u);
    /* 0047efe4 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0047efe8 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0047efec */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0047eff0 */
    f->r[0]=0x005dc18cu;
    /* 0047eff5 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0047eff9 */
    goto mi_0047efff;
mi_0047effb:;
    /* 0047effb */
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
mi_0047efff:;
    /* 0047efff */
    f->r[6]=MI_U32((0xfffffffcu+f->r[0]));
    /* 0047f002 */
    f->r[7]=MI_U32((0x00000000u+f->r[0]));
    /* 0047f004 */
    f->r[1]=0x005dc18cu;
    /* 0047f009 */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 0047f011 */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0047f015 */
    goto mi_0047f01b;
mi_0047f017:;
    /* 0047f017 */
    f->r[1]=MI_U32((0x0000003cu+f->r[4]));
mi_0047f01b:;
    /* 0047f01b */
    f->r[2]=MI_U32((0x0000002cu+f->r[4]));
    /* 0047f01f */
    f->r[0]=MI_U32((0xfffffffcu+f->r[1]));
    /* 0047f022 */
    f->r[1]=MI_U32((0x00000000u+f->r[1]));
    /* 0047f024 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047f026 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 0047f028 */
    if((f->flags&MI_ZF))goto mi_0047f044;
    /* 0047f02a */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0047f02c */
    f->r[2]=(0x00000000u+f->r[7]+f->r[3]*1u);
    /* 0047f02f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047f030 */
    f->r[1]=MI_U32((0x00000040u+f->r[4]));
    /* 0047f034 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0047f036 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047f037 */
    f->r[0]=MI_U32((0x0000003cu+f->r[4]));
    /* 0047f03b */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047f03c */
    f->r[1]=(0x00000000u+f->r[6]+f->r[0]*1u);
    /* 0047f03f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047f040 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047f042 */
    goto mi_0047f05c;
mi_0047f044:;
    /* 0047f044 */
    f->r[2]=MI_U32((0x0000003cu+f->r[4]));
    /* 0047f048 */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0047f04a */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047f04b */
    f->r[1]=MI_U32((0x00000038u+f->r[4]));
    /* 0047f04f */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0047f051 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047f052 */
    f->r[0]=(0x00000000u+f->r[7]+f->r[3]*1u);
    /* 0047f055 */
    f->r[2]=(0x00000000u+f->r[6]+f->r[1]*1u);
    /* 0047f058 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047f059 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047f05a */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
mi_0047f05c:;
    /* 0047f05c */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047f061u;f->boundary(0x0047f05cu,0x0051c930u,f);
    /* 0047f061 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047f064 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0047f066 */
    if((f->flags&MI_ZF))goto mi_0047f08b;
    /* 0047f068 */
    f->r[0]=MI_U32((0x00bc971cu));
    /* 0047f06d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0047f06f */
    if((f->flags&MI_ZF))goto mi_0047f08b;
    /* 0047f071 */
    a=f->r[0];b=MI_U32((0x00000018u+f->r[4]));t=mi_alu(f,a,b,32,1);
    /* 0047f075 */
    if(!mi_signed_less(f->flags))goto mi_0047f08b;
    /* 0047f077 */
    f->r[1]=MI_U32((0x00000034u+f->r[4]));
    /* 0047f07b */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0047f07f */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 0047f083 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0047f087 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[0]);
mi_0047f08b:;
    /* 0047f08b */
    f->r[0]=MI_U32((0x00000034u+f->r[4]));
    /* 0047f08f */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0047f090 */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0047f094 */
    f->r[0]=MI_U32((0x0000003cu+f->r[4]));
    /* 0047f098 */
    a=f->r[0];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0047f09b */
    a=f->r[0];b=0x005dc1acu;t=mi_alu(f,a,b,32,1);
    /* 0047f0a0 */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0047f0a4 */
    if(mi_signed_less(f->flags))goto mi_0047f017;
    /* 0047f0aa */
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
    /* 0047f0ae */
    f->r[6]=MI_U32((0x00000010u+f->r[4]));
    /* 0047f0b2 */
    a=f->r[0];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0047f0b5 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0047f0b6 */
    a=f->r[0];b=0x005dc1acu;t=mi_alu(f,a,b,32,1);
    /* 0047f0bb */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 0047f0bf */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0047f0c3 */
    if(mi_signed_less(f->flags))goto mi_0047effb;
    /* 0047f0c9 */
    f->r[7]=MI_U32((0x00000018u+f->r[4]));
    /* 0047f0cd */
    a=f->r[7];b=0x00002710u;t=mi_alu(f,a,b,32,1);
    /* 0047f0d3 */
    if((f->flags&MI_ZF))goto mi_0047f17c;
    /* 0047f0d9 */
    f->r[1]=MI_U32((0x0000001cu+f->r[4]));
    /* 0047f0dd */
    f->r[6]=MI_U32((0x00000020u+f->r[4]));
    /* 0047f0e1 */
    a=MI_U32((0x0000002cu+f->r[4]));b=0x00000000u;t=mi_alu(f,a,b,32,1);
    /* 0047f0e6 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047f0e8 */
    f->r[0]=MI_U32((0x005dc188u+f->r[1]*8u));
    /* 0047f0ef */
    f->r[2]=MI_U32((0x005dc188u+f->r[6]*8u));
    /* 0047f0f6 */
    f->r[1]=MI_U32((0x005dc18cu+f->r[1]*8u));
    /* 0047f0fd */
    f->r[6]=MI_U32((0x005dc18cu+f->r[6]*8u));
    /* 0047f104 */
    if((f->flags&MI_ZF))goto mi_0047f11e;
    /* 0047f106 */
    a=f->r[6];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0047f108 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0047f10a */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047f10b */
    f->r[6]=MI_U32((0x00000040u+f->r[4]));
    /* 0047f10f */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0047f111 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047f112 */
    f->r[2]=MI_U32((0x0000003cu+f->r[4]));
    /* 0047f116 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0047f118 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047f119 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047f11a */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047f11c */
    goto mi_0047f134;
mi_0047f11e:;
    /* 0047f11e */
    a=f->r[6];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0047f120 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0047f122 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047f123 */
    f->r[6]=MI_U32((0x00000040u+f->r[4]));
    /* 0047f127 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0047f129 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047f12a */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047f12b */
    f->r[1]=MI_U32((0x00000040u+f->r[4]));
    /* 0047f12f */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0047f131 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047f132 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
mi_0047f134:;
    /* 0047f134 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047f139u;f->boundary(0x0047f134u,0x0051c930u,f);
    /* 0047f139 */
    f->r[0]=MI_U32((0x00bc970cu));
    /* 0047f13e */
    f->r[1]=0x00000001u;
    /* 0047f143 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047f146 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0047f148 */
    f->r[0]=MI_U32((0x00706cb4u));
    /* 0047f14d */
    if(!(f->flags&MI_ZF))goto mi_0047f155;
    /* 0047f14f */
    a=MI_U32((0x00bc9724u));b=f->r[0];t=mi_alu(f,a,b,32,0);MI_U32((0x00bc9724u))=(uint32_t)(t);
mi_0047f155:;
    /* 0047f155 */
    a=MI_U32((0x00bc9708u));b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0047f15b */
    if(!(f->flags&MI_ZF))goto mi_0047f163;
    /* 0047f15d */
    a=MI_U32((0x00bc9724u));b=f->r[0];t=mi_alu(f,a,b,32,0);MI_U32((0x00bc9724u))=(uint32_t)(t);
mi_0047f163:;
    /* 0047f163 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 0047f165 */
    if((f->flags&MI_ZF))goto mi_0047f172;
    /* 0047f167 */
    a=f->r[7];b=MI_U32((0x00000024u+f->r[4]));t=mi_imul(f,a,b);f->r[7]=t;
    /* 0047f16c */
    a=MI_U32((0x00bc9724u));b=f->r[7];t=mi_alu(f,a,b,32,0);MI_U32((0x00bc9724u))=(uint32_t)(t);
mi_0047f172:;
    /* 0047f172 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0047f173 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0047f174 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0047f175 */
    f->r[0]=f->r[1];
    /* 0047f177 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0047f178 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047f17b */
    f->r[4]+=4;return;
mi_0047f17c:;
    /* 0047f17c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0047f17d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0047f17e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0047f17f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0047f181 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0047f182 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047f185 */
    f->r[4]+=4;return;
}
