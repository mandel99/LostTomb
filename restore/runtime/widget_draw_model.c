/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "widget_draw.h"
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
static __inline uint32_t mi_alu(struct restore_widget_draw_frame *f,uint32_t a,uint32_t b,uint32_t width,uint32_t operation)
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
static __inline uint32_t mi_shift(struct restore_widget_draw_frame *f,uint32_t a,uint32_t count,uint32_t width,uint32_t arithmetic)
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
static __inline uint32_t mi_imul(struct restore_widget_draw_frame *f,uint32_t a,uint32_t b)
{
 int64_t product=(int64_t)(int32_t)a*(int32_t)b;uint32_t result=(uint32_t)product;
 f->flags&=~(MI_CF|MI_OF);if(product!=(int64_t)(int32_t)result)f->flags|=MI_CF|MI_OF;
 return result;
}
static __inline int mi_signed_less(uint32_t flags){return ((flags&MI_SF)!=0)^((flags&MI_OF)!=0);}

static const uint8_t mi_rom_00422370[]={0x20,0x1c,0x42,0x00,0x48,0x19,0x42,0x00,0xb0,0x19,0x42,0x00,0x18,0x1a,0x42,0x00,0x80,0x1a,0x42,0x00,0xe8,0x1a,0x42,0x00,0x88,0x1c,0x42,0x00,0xf0,0x1c,0x42,0x00,0x58,0x1d,0x42,0x00,0xc0,0x1d,0x42,0x00,0x28,0x1e,0x42,0x00,0x90,0x1e,0x42,0x00,0xf8,0x1e,0x42,0x00,0x60,0x1f,0x42,0x00,0xc8,0x1f,0x42,0x00,0x50,0x1b,0x42,0x00,0xb8,0x1b,0x42,0x00,0x6b,0x23,0x42,0x00,0x6b,0x23,0x42,0x00,0x30,0x20,0x42,0x00,0x98,0x20,0x42,0x00,0x00,0x21,0x42,0x00,0x68,0x21,0x42,0x00,0xd0,0x21,0x42,0x00,0x38,0x22,0x42,0x00,0xa0,0x22,0x42,0x00,0x08,0x23,0x42,0x00,0x6b,0x23,0x42,0x00,0x6b,0x23,0x42,0x00,0x48,0x19,0x42,0x00};
static __inline uint32_t mi_read(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x00422370u&&width<=120u&&(offset=address-0x00422370u)<=120u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00422370[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}

void restore_widget_draw_C_0041fff0(struct restore_widget_draw_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0041fff0 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041fff1 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041fff2 */
    f->r[3]=MI_U32((0x0000000cu+f->r[4]));
    /* 0041fff6 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041fff7 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041fff8 */
    f->r[7]=MI_U32((0x00000018u+f->r[4]));
    /* 0041fffc */
    f->r[6]=0x005c2750u;
mi_00420001:;
    /* 00420001 */
    f->r[0]=(0x00000018u+f->r[4]);
    /* 00420005 */
    f->r[1]=(0x00000014u+f->r[4]);
    /* 00420009 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0042000a */
    f->r[2]=(0x00000010u+f->r[4]);
    /* 0042000e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0042000f */
    f->r[1]=MI_U32((0x00000000u+f->r[6]));
    /* 00420011 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00420012 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00420017u;f->boundary(0x00420012u,0x0041f910u,f);
    /* 00420017 */
    f->r[0]=MI_U32((0x00000018u+f->r[4]));
    /* 0042001b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0042001d */
    if((f->flags&MI_ZF))goto mi_00420038;
    /* 0042001f */
    f->r[1]=MI_U32((0x00000014u+f->r[4]));
    /* 00420023 */
    f->r[2]=MI_U32((0x0000000cu+f->r[4]));
    /* 00420027 */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00420029 */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0042002b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0042002c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0042002d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0042002e */
    f->r[1]=0x00f55bb0u;
    /* 00420033 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00420038u;f->boundary(0x00420033u,0x0053e590u,f);
mi_00420038:;
    /* 00420038 */
    a=f->r[6];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0042003b */
    a=f->r[6];b=0x005c28b0u;t=mi_alu(f,a,b,32,1);
    /* 00420041 */
    if(mi_signed_less(f->flags))goto mi_00420001;
    /* 00420043 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00420044 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00420045 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00420046 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 00420047 */
    f->r[4]+=4;return;
}

void restore_widget_draw_C_00420050(struct restore_widget_draw_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00420050 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00420051 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00420052 */
    f->r[3]=MI_U32((0x0000000cu+f->r[4]));
    /* 00420056 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00420057 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00420058 */
    f->r[7]=MI_U32((0x00000018u+f->r[4]));
    /* 0042005c */
    f->r[6]=0x00596088u;
mi_00420061:;
    /* 00420061 */
    f->r[0]=(0x00000018u+f->r[4]);
    /* 00420065 */
    f->r[1]=(0x00000014u+f->r[4]);
    /* 00420069 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0042006a */
    f->r[2]=(0x00000010u+f->r[4]);
    /* 0042006e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0042006f */
    f->r[1]=MI_U32((0x00000000u+f->r[6]));
    /* 00420071 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00420072 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00420077u;f->boundary(0x00420072u,0x0041f910u,f);
    /* 00420077 */
    f->r[0]=MI_U32((0x00000018u+f->r[4]));
    /* 0042007b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0042007d */
    if((f->flags&MI_ZF))goto mi_00420098;
    /* 0042007f */
    f->r[1]=MI_U32((0x00000014u+f->r[4]));
    /* 00420083 */
    f->r[2]=MI_U32((0x0000000cu+f->r[4]));
    /* 00420087 */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00420089 */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0042008b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0042008c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0042008d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0042008e */
    f->r[1]=0x00f55bb0u;
    /* 00420093 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00420098u;f->boundary(0x00420093u,0x0053e590u,f);
mi_00420098:;
    /* 00420098 */
    a=f->r[6];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0042009b */
    a=f->r[6];b=0x00596158u;t=mi_alu(f,a,b,32,1);
    /* 004200a1 */
    if(mi_signed_less(f->flags))goto mi_00420061;
    /* 004200a3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004200a4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004200a5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004200a6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004200a7 */
    f->r[4]+=4;return;
}

void restore_widget_draw_C_004206f0(struct restore_widget_draw_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004206f0 */
    f->r[0]=MI_U32((0x00bca2e0u));
    /* 004206f5 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 004206f8 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004206f9 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004206fa */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004206fc */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004206fd */
    f->r[6]=f->r[0];
    /* 004206ff */
    a=f->r[6];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[6]=t;
    /* 00420701 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00420702 */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 00420705 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0042070au;f->boundary(0x00420705u,0x0041f8b0u,f);
    /* 0042070a */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0042070c */
    if((f->flags&MI_ZF))goto mi_00420717;
    /* 0042070e */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0042070f */
    f->r[4]-=4;MI_U32(f->r[4])=0x00420714u;f->boundary(0x0042070fu,0x0041f8c0u,f);
    /* 00420714 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00420717:;
    /* 00420717 */
    f->r[7]=MI_U32((0x00000020u+f->r[4]));
    /* 0042071b */
    f->r[3]=MI_U32((0x0000001cu+f->r[4]));
    /* 0042071f */
    f->r[6]=0x0059edf0u;
mi_00420724:;
    /* 00420724 */
    f->r[0]=(0x0000000cu+f->r[4]);
    /* 00420728 */
    f->r[1]=(0x00000010u+f->r[4]);
    /* 0042072c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0042072d */
    f->r[2]=(0x00000018u+f->r[4]);
    /* 00420731 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00420732 */
    f->r[1]=MI_U32((0x00000000u+f->r[6]));
    /* 00420734 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00420735 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0042073au;f->boundary(0x00420735u,0x0041f910u,f);
    /* 0042073a */
    f->r[0]=MI_U32((0x0000000cu+f->r[4]));
    /* 0042073e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00420740 */
    if((f->flags&MI_ZF))goto mi_0042075b;
    /* 00420742 */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
    /* 00420746 */
    f->r[2]=MI_U32((0x00000014u+f->r[4]));
    /* 0042074a */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0042074c */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0042074e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0042074f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00420750 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00420751 */
    f->r[1]=0x00f55bb0u;
    /* 00420756 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0042075bu;f->boundary(0x00420756u,0x0053e590u,f);
mi_0042075b:;
    /* 0042075b */
    a=f->r[6];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0042075e */
    a=f->r[6];b=0x0059efa0u;t=mi_alu(f,a,b,32,1);
    /* 00420764 */
    if(mi_signed_less(f->flags))goto mi_00420724;
    /* 00420766 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00420767 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00420768 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00420769 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0042076c */
    f->r[4]+=4;return;
}

void restore_widget_draw_C_00420a30(struct restore_widget_draw_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00420a30 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00420a31 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00420a32 */
    f->r[3]=MI_U32((0x0000000cu+f->r[4]));
    /* 00420a36 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00420a37 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00420a38 */
    f->r[7]=MI_U32((0x00000018u+f->r[4]));
    /* 00420a3c */
    f->r[6]=0x005a8148u;
mi_00420a41:;
    /* 00420a41 */
    f->r[0]=(0x00000018u+f->r[4]);
    /* 00420a45 */
    f->r[1]=(0x00000014u+f->r[4]);
    /* 00420a49 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00420a4a */
    f->r[2]=(0x00000010u+f->r[4]);
    /* 00420a4e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00420a4f */
    f->r[1]=MI_U32((0x00000000u+f->r[6]));
    /* 00420a51 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00420a52 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00420a57u;f->boundary(0x00420a52u,0x0041f910u,f);
    /* 00420a57 */
    f->r[0]=MI_U32((0x00000018u+f->r[4]));
    /* 00420a5b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00420a5d */
    if((f->flags&MI_ZF))goto mi_00420a78;
    /* 00420a5f */
    f->r[1]=MI_U32((0x00000014u+f->r[4]));
    /* 00420a63 */
    f->r[2]=MI_U32((0x0000000cu+f->r[4]));
    /* 00420a67 */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00420a69 */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00420a6b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00420a6c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00420a6d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00420a6e */
    f->r[1]=0x00f55bb0u;
    /* 00420a73 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00420a78u;f->boundary(0x00420a73u,0x0053e590u,f);
mi_00420a78:;
    /* 00420a78 */
    a=f->r[6];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00420a7b */
    a=f->r[6];b=0x005a84c8u;t=mi_alu(f,a,b,32,1);
    /* 00420a81 */
    if(mi_signed_less(f->flags))goto mi_00420a41;
    /* 00420a83 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00420a84 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00420a85 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00420a86 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 00420a87 */
    f->r[4]+=4;return;
}

void restore_widget_draw_C_00420d60(struct restore_widget_draw_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00420d60 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00420d61 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00420d62 */
    f->r[3]=MI_U32((0x0000000cu+f->r[4]));
    /* 00420d66 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00420d67 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00420d68 */
    f->r[7]=MI_U32((0x00000018u+f->r[4]));
    /* 00420d6c */
    f->r[6]=0x005ad218u;
mi_00420d71:;
    /* 00420d71 */
    f->r[0]=(0x00000018u+f->r[4]);
    /* 00420d75 */
    f->r[1]=(0x00000014u+f->r[4]);
    /* 00420d79 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00420d7a */
    f->r[2]=(0x00000010u+f->r[4]);
    /* 00420d7e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00420d7f */
    f->r[1]=MI_U32((0x00000000u+f->r[6]));
    /* 00420d81 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00420d82 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00420d87u;f->boundary(0x00420d82u,0x0041f910u,f);
    /* 00420d87 */
    f->r[0]=MI_U32((0x00000018u+f->r[4]));
    /* 00420d8b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00420d8d */
    if((f->flags&MI_ZF))goto mi_00420da8;
    /* 00420d8f */
    f->r[1]=MI_U32((0x00000014u+f->r[4]));
    /* 00420d93 */
    f->r[2]=MI_U32((0x0000000cu+f->r[4]));
    /* 00420d97 */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00420d99 */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00420d9b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00420d9c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00420d9d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00420d9e */
    f->r[1]=0x00f55bb0u;
    /* 00420da3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00420da8u;f->boundary(0x00420da3u,0x0053e590u,f);
mi_00420da8:;
    /* 00420da8 */
    a=f->r[6];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00420dab */
    a=f->r[6];b=0x005ad3c8u;t=mi_alu(f,a,b,32,1);
    /* 00420db1 */
    if(mi_signed_less(f->flags))goto mi_00420d71;
    /* 00420db3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00420db4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00420db5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00420db6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 00420db7 */
    f->r[4]+=4;return;
}

void restore_widget_draw_C_00421060(struct restore_widget_draw_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00421060 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421061 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421062 */
    f->r[3]=MI_U32((0x0000000cu+f->r[4]));
    /* 00421066 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421067 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421068 */
    f->r[7]=MI_U32((0x00000018u+f->r[4]));
    /* 0042106c */
    f->r[6]=0x005b0060u;
mi_00421071:;
    /* 00421071 */
    f->r[0]=(0x00000018u+f->r[4]);
    /* 00421075 */
    f->r[1]=(0x00000014u+f->r[4]);
    /* 00421079 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0042107a */
    f->r[2]=(0x00000010u+f->r[4]);
    /* 0042107e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0042107f */
    f->r[1]=MI_U32((0x00000000u+f->r[6]));
    /* 00421081 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421082 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00421087u;f->boundary(0x00421082u,0x0041f910u,f);
    /* 00421087 */
    f->r[0]=MI_U32((0x00000018u+f->r[4]));
    /* 0042108b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0042108d */
    if((f->flags&MI_ZF))goto mi_004210a8;
    /* 0042108f */
    f->r[1]=MI_U32((0x00000014u+f->r[4]));
    /* 00421093 */
    f->r[2]=MI_U32((0x0000000cu+f->r[4]));
    /* 00421097 */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00421099 */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0042109b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0042109c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0042109d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0042109e */
    f->r[1]=0x00f55bb0u;
    /* 004210a3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004210a8u;f->boundary(0x004210a3u,0x0053e590u,f);
mi_004210a8:;
    /* 004210a8 */
    a=f->r[6];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004210ab */
    a=f->r[6];b=0x005b0150u;t=mi_alu(f,a,b,32,1);
    /* 004210b1 */
    if(mi_signed_less(f->flags))goto mi_00421071;
    /* 004210b3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004210b4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004210b5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004210b6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004210b7 */
    f->r[4]+=4;return;
}

void restore_widget_draw_C_00421920(struct restore_widget_draw_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00421920 */
    f->r[0]=mi_read((0x0000000cu+f->r[4]),4u);
    /* 00421924 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421925 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421926 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421927 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421928 */
    f->r[7]=f->r[0];
    /* 0042192a */
    a=f->r[7];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 0042192d */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0042192f */
    a=f->r[7];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 00421932 */
    f->r[0]=mi_read((0x00799a40u+f->r[7]),4u);
    /* 00421938 */
    a=f->r[0];b=0x0000001du;t=mi_alu(f,a,b,32,1);
    /* 0042193b */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0042236b;
    /* 00421941 */
    switch(mi_read((0x00422370u+f->r[0]*4u),4u)){case 0x00421948u:goto mi_00421948;case 0x004219b0u:goto mi_004219b0;case 0x00421a18u:goto mi_00421a18;case 0x00421a80u:goto mi_00421a80;case 0x00421ae8u:goto mi_00421ae8;case 0x00421b50u:goto mi_00421b50;case 0x00421bb8u:goto mi_00421bb8;case 0x00421c20u:goto mi_00421c20;case 0x00421c88u:goto mi_00421c88;case 0x00421cf0u:goto mi_00421cf0;case 0x00421d58u:goto mi_00421d58;case 0x00421dc0u:goto mi_00421dc0;case 0x00421e28u:goto mi_00421e28;case 0x00421e90u:goto mi_00421e90;case 0x00421ef8u:goto mi_00421ef8;case 0x00421f60u:goto mi_00421f60;case 0x00421fc8u:goto mi_00421fc8;case 0x00422030u:goto mi_00422030;case 0x00422098u:goto mi_00422098;case 0x00422100u:goto mi_00422100;case 0x00422168u:goto mi_00422168;case 0x004221d0u:goto mi_004221d0;case 0x00422238u:goto mi_00422238;case 0x004222a0u:goto mi_004222a0;case 0x00422308u:goto mi_00422308;case 0x0042236bu:goto mi_0042236b;default:abort();}
mi_00421948:;
    /* 00421948 */
    f->r[3]=mi_read((0x00000018u+f->r[4]),4u);
    /* 0042194c */
    f->r[5]=mi_read((0x00000014u+f->r[4]),4u);
    /* 00421950 */
    f->r[6]=0x005be510u;
mi_00421955:;
    /* 00421955 */
    f->r[0]=mi_read((0x00799a38u+f->r[7]),4u);
    /* 0042195b */
    f->r[1]=mi_read((0xfffffff8u+f->r[6]),4u);
    /* 0042195e */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 00421960 */
    if(!(f->flags&MI_ZF))goto mi_004219a0;
    /* 00421962 */
    f->r[1]=mi_read((0x00799a68u+f->r[7]),4u);
    /* 00421968 */
    f->r[2]=(0x00000018u+f->r[4]);
    /* 0042196c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0042196d */
    f->r[0]=(0x00000018u+f->r[4]);
    /* 00421971 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421972 */
    f->r[1]=(0x00000024u+f->r[4]);
    /* 00421976 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421977 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421978 */
    f->r[1]=mi_read((0x00000000u+f->r[6]),4u);
    /* 0042197a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0042197fu;f->boundary(0x0042197au,0x0041fad0u,f);
    /* 0042197f */
    f->r[0]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00421983 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00421985 */
    if((f->flags&MI_ZF))goto mi_004219a0;
    /* 00421987 */
    f->r[2]=mi_read((0x00000014u+f->r[4]),4u);
    /* 0042198b */
    f->r[1]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 0042198f */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00421991 */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00421993 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421994 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421995 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421996 */
    f->r[1]=0x00f55bb0u;
    /* 0042199b */
    f->r[4]-=4;MI_U32(f->r[4])=0x004219a0u;f->boundary(0x0042199bu,0x0053e590u,f);
mi_004219a0:;
    /* 004219a0 */
    a=f->r[6];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004219a3 */
    a=f->r[6];b=0x005be540u;t=mi_alu(f,a,b,32,1);
    /* 004219a9 */
    if(mi_signed_less(f->flags))goto mi_00421955;
    /* 004219ab */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004219ac */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004219ad */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004219ae */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004219af */
    f->r[4]+=4;return;
mi_004219b0:;
    /* 004219b0 */
    f->r[3]=mi_read((0x00000018u+f->r[4]),4u);
    /* 004219b4 */
    f->r[5]=mi_read((0x00000014u+f->r[4]),4u);
    /* 004219b8 */
    f->r[6]=0x005be540u;
mi_004219bd:;
    /* 004219bd */
    f->r[2]=mi_read((0x00799a38u+f->r[7]),4u);
    /* 004219c3 */
    f->r[0]=mi_read((0xfffffff8u+f->r[6]),4u);
    /* 004219c6 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004219c8 */
    if(!(f->flags&MI_ZF))goto mi_00421a08;
    /* 004219ca */
    f->r[0]=mi_read((0x00799a68u+f->r[7]),4u);
    /* 004219d0 */
    f->r[1]=(0x00000018u+f->r[4]);
    /* 004219d4 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004219d5 */
    f->r[2]=(0x00000018u+f->r[4]);
    /* 004219d9 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004219da */
    f->r[1]=mi_read((0x00000000u+f->r[6]),4u);
    /* 004219dc */
    f->r[0]=(0x00000024u+f->r[4]);
    /* 004219e0 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004219e1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004219e2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004219e7u;f->boundary(0x004219e2u,0x0041fad0u,f);
    /* 004219e7 */
    f->r[0]=mi_read((0x00000018u+f->r[4]),4u);
    /* 004219eb */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004219ed */
    if((f->flags&MI_ZF))goto mi_00421a08;
    /* 004219ef */
    f->r[1]=mi_read((0x00000014u+f->r[4]),4u);
    /* 004219f3 */
    f->r[2]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 004219f7 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004219f9 */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004219fb */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004219fc */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004219fd */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004219fe */
    f->r[1]=0x00f55bb0u;
    /* 00421a03 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00421a08u;f->boundary(0x00421a03u,0x0053e590u,f);
mi_00421a08:;
    /* 00421a08 */
    a=f->r[6];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00421a0b */
    a=f->r[6];b=0x005be570u;t=mi_alu(f,a,b,32,1);
    /* 00421a11 */
    if(mi_signed_less(f->flags))goto mi_004219bd;
    /* 00421a13 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00421a14 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00421a15 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00421a16 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00421a17 */
    f->r[4]+=4;return;
mi_00421a18:;
    /* 00421a18 */
    f->r[3]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00421a1c */
    f->r[5]=mi_read((0x00000014u+f->r[4]),4u);
    /* 00421a20 */
    f->r[6]=0x005be570u;
mi_00421a25:;
    /* 00421a25 */
    f->r[0]=mi_read((0x00799a38u+f->r[7]),4u);
    /* 00421a2b */
    f->r[1]=mi_read((0xfffffff8u+f->r[6]),4u);
    /* 00421a2e */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 00421a30 */
    if(!(f->flags&MI_ZF))goto mi_00421a70;
    /* 00421a32 */
    f->r[1]=mi_read((0x00799a68u+f->r[7]),4u);
    /* 00421a38 */
    f->r[2]=(0x00000018u+f->r[4]);
    /* 00421a3c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421a3d */
    f->r[0]=(0x00000018u+f->r[4]);
    /* 00421a41 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421a42 */
    f->r[1]=(0x00000024u+f->r[4]);
    /* 00421a46 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421a47 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421a48 */
    f->r[1]=mi_read((0x00000000u+f->r[6]),4u);
    /* 00421a4a */
    f->r[4]-=4;MI_U32(f->r[4])=0x00421a4fu;f->boundary(0x00421a4au,0x0041fad0u,f);
    /* 00421a4f */
    f->r[0]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00421a53 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00421a55 */
    if((f->flags&MI_ZF))goto mi_00421a70;
    /* 00421a57 */
    f->r[2]=mi_read((0x00000014u+f->r[4]),4u);
    /* 00421a5b */
    f->r[1]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 00421a5f */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00421a61 */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00421a63 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421a64 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421a65 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421a66 */
    f->r[1]=0x00f55bb0u;
    /* 00421a6b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00421a70u;f->boundary(0x00421a6bu,0x0053e590u,f);
mi_00421a70:;
    /* 00421a70 */
    a=f->r[6];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00421a73 */
    a=f->r[6];b=0x005be5a0u;t=mi_alu(f,a,b,32,1);
    /* 00421a79 */
    if(mi_signed_less(f->flags))goto mi_00421a25;
    /* 00421a7b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00421a7c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00421a7d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00421a7e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00421a7f */
    f->r[4]+=4;return;
mi_00421a80:;
    /* 00421a80 */
    f->r[3]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00421a84 */
    f->r[5]=mi_read((0x00000014u+f->r[4]),4u);
    /* 00421a88 */
    f->r[6]=0x005be750u;
mi_00421a8d:;
    /* 00421a8d */
    f->r[2]=mi_read((0x00799a38u+f->r[7]),4u);
    /* 00421a93 */
    f->r[0]=mi_read((0xfffffff8u+f->r[6]),4u);
    /* 00421a96 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00421a98 */
    if(!(f->flags&MI_ZF))goto mi_00421ad8;
    /* 00421a9a */
    f->r[0]=mi_read((0x00799a68u+f->r[7]),4u);
    /* 00421aa0 */
    f->r[1]=(0x00000018u+f->r[4]);
    /* 00421aa4 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421aa5 */
    f->r[2]=(0x00000018u+f->r[4]);
    /* 00421aa9 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421aaa */
    f->r[1]=mi_read((0x00000000u+f->r[6]),4u);
    /* 00421aac */
    f->r[0]=(0x00000024u+f->r[4]);
    /* 00421ab0 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421ab1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421ab2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00421ab7u;f->boundary(0x00421ab2u,0x0041fad0u,f);
    /* 00421ab7 */
    f->r[0]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00421abb */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00421abd */
    if((f->flags&MI_ZF))goto mi_00421ad8;
    /* 00421abf */
    f->r[1]=mi_read((0x00000014u+f->r[4]),4u);
    /* 00421ac3 */
    f->r[2]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 00421ac7 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00421ac9 */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00421acb */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421acc */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421acd */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421ace */
    f->r[1]=0x00f55bb0u;
    /* 00421ad3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00421ad8u;f->boundary(0x00421ad3u,0x0053e590u,f);
mi_00421ad8:;
    /* 00421ad8 */
    a=f->r[6];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00421adb */
    a=f->r[6];b=0x005be780u;t=mi_alu(f,a,b,32,1);
    /* 00421ae1 */
    if(mi_signed_less(f->flags))goto mi_00421a8d;
    /* 00421ae3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00421ae4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00421ae5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00421ae6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00421ae7 */
    f->r[4]+=4;return;
mi_00421ae8:;
    /* 00421ae8 */
    f->r[3]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00421aec */
    f->r[5]=mi_read((0x00000014u+f->r[4]),4u);
    /* 00421af0 */
    f->r[6]=0x005be780u;
mi_00421af5:;
    /* 00421af5 */
    f->r[0]=mi_read((0x00799a38u+f->r[7]),4u);
    /* 00421afb */
    f->r[1]=mi_read((0xfffffff8u+f->r[6]),4u);
    /* 00421afe */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 00421b00 */
    if(!(f->flags&MI_ZF))goto mi_00421b40;
    /* 00421b02 */
    f->r[1]=mi_read((0x00799a68u+f->r[7]),4u);
    /* 00421b08 */
    f->r[2]=(0x00000018u+f->r[4]);
    /* 00421b0c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421b0d */
    f->r[0]=(0x00000018u+f->r[4]);
    /* 00421b11 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421b12 */
    f->r[1]=(0x00000024u+f->r[4]);
    /* 00421b16 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421b17 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421b18 */
    f->r[1]=mi_read((0x00000000u+f->r[6]),4u);
    /* 00421b1a */
    f->r[4]-=4;MI_U32(f->r[4])=0x00421b1fu;f->boundary(0x00421b1au,0x0041fad0u,f);
    /* 00421b1f */
    f->r[0]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00421b23 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00421b25 */
    if((f->flags&MI_ZF))goto mi_00421b40;
    /* 00421b27 */
    f->r[2]=mi_read((0x00000014u+f->r[4]),4u);
    /* 00421b2b */
    f->r[1]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 00421b2f */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00421b31 */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00421b33 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421b34 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421b35 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421b36 */
    f->r[1]=0x00f55bb0u;
    /* 00421b3b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00421b40u;f->boundary(0x00421b3bu,0x0053e590u,f);
mi_00421b40:;
    /* 00421b40 */
    a=f->r[6];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00421b43 */
    a=f->r[6];b=0x005be7b0u;t=mi_alu(f,a,b,32,1);
    /* 00421b49 */
    if(mi_signed_less(f->flags))goto mi_00421af5;
    /* 00421b4b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00421b4c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00421b4d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00421b4e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00421b4f */
    f->r[4]+=4;return;
mi_00421b50:;
    /* 00421b50 */
    f->r[3]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00421b54 */
    f->r[5]=mi_read((0x00000014u+f->r[4]),4u);
    /* 00421b58 */
    f->r[6]=0x005be5a0u;
mi_00421b5d:;
    /* 00421b5d */
    f->r[2]=mi_read((0x00799a38u+f->r[7]),4u);
    /* 00421b63 */
    f->r[0]=mi_read((0xfffffff8u+f->r[6]),4u);
    /* 00421b66 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00421b68 */
    if(!(f->flags&MI_ZF))goto mi_00421ba8;
    /* 00421b6a */
    f->r[0]=mi_read((0x00799a68u+f->r[7]),4u);
    /* 00421b70 */
    f->r[1]=(0x00000018u+f->r[4]);
    /* 00421b74 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421b75 */
    f->r[2]=(0x00000018u+f->r[4]);
    /* 00421b79 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421b7a */
    f->r[1]=mi_read((0x00000000u+f->r[6]),4u);
    /* 00421b7c */
    f->r[0]=(0x00000024u+f->r[4]);
    /* 00421b80 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421b81 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421b82 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00421b87u;f->boundary(0x00421b82u,0x0041fad0u,f);
    /* 00421b87 */
    f->r[0]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00421b8b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00421b8d */
    if((f->flags&MI_ZF))goto mi_00421ba8;
    /* 00421b8f */
    f->r[1]=mi_read((0x00000014u+f->r[4]),4u);
    /* 00421b93 */
    f->r[2]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 00421b97 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00421b99 */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00421b9b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421b9c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421b9d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421b9e */
    f->r[1]=0x00f55bb0u;
    /* 00421ba3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00421ba8u;f->boundary(0x00421ba3u,0x0053e590u,f);
mi_00421ba8:;
    /* 00421ba8 */
    a=f->r[6];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00421bab */
    a=f->r[6];b=0x005be5d0u;t=mi_alu(f,a,b,32,1);
    /* 00421bb1 */
    if(mi_signed_less(f->flags))goto mi_00421b5d;
    /* 00421bb3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00421bb4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00421bb5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00421bb6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00421bb7 */
    f->r[4]+=4;return;
mi_00421bb8:;
    /* 00421bb8 */
    f->r[3]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00421bbc */
    f->r[5]=mi_read((0x00000014u+f->r[4]),4u);
    /* 00421bc0 */
    f->r[6]=0x005be5d0u;
mi_00421bc5:;
    /* 00421bc5 */
    f->r[0]=mi_read((0x00799a38u+f->r[7]),4u);
    /* 00421bcb */
    f->r[1]=mi_read((0xfffffff8u+f->r[6]),4u);
    /* 00421bce */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 00421bd0 */
    if(!(f->flags&MI_ZF))goto mi_00421c10;
    /* 00421bd2 */
    f->r[1]=mi_read((0x00799a68u+f->r[7]),4u);
    /* 00421bd8 */
    f->r[2]=(0x00000018u+f->r[4]);
    /* 00421bdc */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421bdd */
    f->r[0]=(0x00000018u+f->r[4]);
    /* 00421be1 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421be2 */
    f->r[1]=(0x00000024u+f->r[4]);
    /* 00421be6 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421be7 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421be8 */
    f->r[1]=mi_read((0x00000000u+f->r[6]),4u);
    /* 00421bea */
    f->r[4]-=4;MI_U32(f->r[4])=0x00421befu;f->boundary(0x00421beau,0x0041fad0u,f);
    /* 00421bef */
    f->r[0]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00421bf3 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00421bf5 */
    if((f->flags&MI_ZF))goto mi_00421c10;
    /* 00421bf7 */
    f->r[2]=mi_read((0x00000014u+f->r[4]),4u);
    /* 00421bfb */
    f->r[1]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 00421bff */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00421c01 */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00421c03 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421c04 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421c05 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421c06 */
    f->r[1]=0x00f55bb0u;
    /* 00421c0b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00421c10u;f->boundary(0x00421c0bu,0x0053e590u,f);
mi_00421c10:;
    /* 00421c10 */
    a=f->r[6];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00421c13 */
    a=f->r[6];b=0x005be600u;t=mi_alu(f,a,b,32,1);
    /* 00421c19 */
    if(mi_signed_less(f->flags))goto mi_00421bc5;
    /* 00421c1b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00421c1c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00421c1d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00421c1e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00421c1f */
    f->r[4]+=4;return;
mi_00421c20:;
    /* 00421c20 */
    f->r[3]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00421c24 */
    f->r[5]=mi_read((0x00000014u+f->r[4]),4u);
    /* 00421c28 */
    f->r[6]=0x005be600u;
mi_00421c2d:;
    /* 00421c2d */
    f->r[2]=mi_read((0x00799a38u+f->r[7]),4u);
    /* 00421c33 */
    f->r[0]=mi_read((0xfffffff8u+f->r[6]),4u);
    /* 00421c36 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00421c38 */
    if(!(f->flags&MI_ZF))goto mi_00421c78;
    /* 00421c3a */
    f->r[0]=mi_read((0x00799a68u+f->r[7]),4u);
    /* 00421c40 */
    f->r[1]=(0x00000018u+f->r[4]);
    /* 00421c44 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421c45 */
    f->r[2]=(0x00000018u+f->r[4]);
    /* 00421c49 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421c4a */
    f->r[1]=mi_read((0x00000000u+f->r[6]),4u);
    /* 00421c4c */
    f->r[0]=(0x00000024u+f->r[4]);
    /* 00421c50 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421c51 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421c52 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00421c57u;f->boundary(0x00421c52u,0x0041fad0u,f);
    /* 00421c57 */
    f->r[0]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00421c5b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00421c5d */
    if((f->flags&MI_ZF))goto mi_00421c78;
    /* 00421c5f */
    f->r[1]=mi_read((0x00000014u+f->r[4]),4u);
    /* 00421c63 */
    f->r[2]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 00421c67 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00421c69 */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00421c6b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421c6c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421c6d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421c6e */
    f->r[1]=0x00f55bb0u;
    /* 00421c73 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00421c78u;f->boundary(0x00421c73u,0x0053e590u,f);
mi_00421c78:;
    /* 00421c78 */
    a=f->r[6];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00421c7b */
    a=f->r[6];b=0x005be630u;t=mi_alu(f,a,b,32,1);
    /* 00421c81 */
    if(mi_signed_less(f->flags))goto mi_00421c2d;
    /* 00421c83 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00421c84 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00421c85 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00421c86 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00421c87 */
    f->r[4]+=4;return;
mi_00421c88:;
    /* 00421c88 */
    f->r[3]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00421c8c */
    f->r[5]=mi_read((0x00000014u+f->r[4]),4u);
    /* 00421c90 */
    f->r[6]=0x005be630u;
mi_00421c95:;
    /* 00421c95 */
    f->r[0]=mi_read((0x00799a38u+f->r[7]),4u);
    /* 00421c9b */
    f->r[1]=mi_read((0xfffffff8u+f->r[6]),4u);
    /* 00421c9e */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 00421ca0 */
    if(!(f->flags&MI_ZF))goto mi_00421ce0;
    /* 00421ca2 */
    f->r[1]=mi_read((0x00799a68u+f->r[7]),4u);
    /* 00421ca8 */
    f->r[2]=(0x00000018u+f->r[4]);
    /* 00421cac */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421cad */
    f->r[0]=(0x00000018u+f->r[4]);
    /* 00421cb1 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421cb2 */
    f->r[1]=(0x00000024u+f->r[4]);
    /* 00421cb6 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421cb7 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421cb8 */
    f->r[1]=mi_read((0x00000000u+f->r[6]),4u);
    /* 00421cba */
    f->r[4]-=4;MI_U32(f->r[4])=0x00421cbfu;f->boundary(0x00421cbau,0x0041fad0u,f);
    /* 00421cbf */
    f->r[0]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00421cc3 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00421cc5 */
    if((f->flags&MI_ZF))goto mi_00421ce0;
    /* 00421cc7 */
    f->r[2]=mi_read((0x00000014u+f->r[4]),4u);
    /* 00421ccb */
    f->r[1]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 00421ccf */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00421cd1 */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00421cd3 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421cd4 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421cd5 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421cd6 */
    f->r[1]=0x00f55bb0u;
    /* 00421cdb */
    f->r[4]-=4;MI_U32(f->r[4])=0x00421ce0u;f->boundary(0x00421cdbu,0x0053e590u,f);
mi_00421ce0:;
    /* 00421ce0 */
    a=f->r[6];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00421ce3 */
    a=f->r[6];b=0x005be660u;t=mi_alu(f,a,b,32,1);
    /* 00421ce9 */
    if(mi_signed_less(f->flags))goto mi_00421c95;
    /* 00421ceb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00421cec */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00421ced */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00421cee */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00421cef */
    f->r[4]+=4;return;
mi_00421cf0:;
    /* 00421cf0 */
    f->r[3]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00421cf4 */
    f->r[5]=mi_read((0x00000014u+f->r[4]),4u);
    /* 00421cf8 */
    f->r[6]=0x005be7b0u;
mi_00421cfd:;
    /* 00421cfd */
    f->r[2]=mi_read((0x00799a38u+f->r[7]),4u);
    /* 00421d03 */
    f->r[0]=mi_read((0xfffffff8u+f->r[6]),4u);
    /* 00421d06 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00421d08 */
    if(!(f->flags&MI_ZF))goto mi_00421d48;
    /* 00421d0a */
    f->r[0]=mi_read((0x00799a68u+f->r[7]),4u);
    /* 00421d10 */
    f->r[1]=(0x00000018u+f->r[4]);
    /* 00421d14 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421d15 */
    f->r[2]=(0x00000018u+f->r[4]);
    /* 00421d19 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421d1a */
    f->r[1]=mi_read((0x00000000u+f->r[6]),4u);
    /* 00421d1c */
    f->r[0]=(0x00000024u+f->r[4]);
    /* 00421d20 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421d21 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421d22 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00421d27u;f->boundary(0x00421d22u,0x0041fad0u,f);
    /* 00421d27 */
    f->r[0]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00421d2b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00421d2d */
    if((f->flags&MI_ZF))goto mi_00421d48;
    /* 00421d2f */
    f->r[1]=mi_read((0x00000014u+f->r[4]),4u);
    /* 00421d33 */
    f->r[2]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 00421d37 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00421d39 */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00421d3b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421d3c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421d3d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421d3e */
    f->r[1]=0x00f55bb0u;
    /* 00421d43 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00421d48u;f->boundary(0x00421d43u,0x0053e590u,f);
mi_00421d48:;
    /* 00421d48 */
    a=f->r[6];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00421d4b */
    a=f->r[6];b=0x005be7e0u;t=mi_alu(f,a,b,32,1);
    /* 00421d51 */
    if(mi_signed_less(f->flags))goto mi_00421cfd;
    /* 00421d53 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00421d54 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00421d55 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00421d56 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00421d57 */
    f->r[4]+=4;return;
mi_00421d58:;
    /* 00421d58 */
    f->r[3]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00421d5c */
    f->r[5]=mi_read((0x00000014u+f->r[4]),4u);
    /* 00421d60 */
    f->r[6]=0x005be7e0u;
mi_00421d65:;
    /* 00421d65 */
    f->r[0]=mi_read((0x00799a38u+f->r[7]),4u);
    /* 00421d6b */
    f->r[1]=mi_read((0xfffffff8u+f->r[6]),4u);
    /* 00421d6e */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 00421d70 */
    if(!(f->flags&MI_ZF))goto mi_00421db0;
    /* 00421d72 */
    f->r[1]=mi_read((0x00799a68u+f->r[7]),4u);
    /* 00421d78 */
    f->r[2]=(0x00000018u+f->r[4]);
    /* 00421d7c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421d7d */
    f->r[0]=(0x00000018u+f->r[4]);
    /* 00421d81 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421d82 */
    f->r[1]=(0x00000024u+f->r[4]);
    /* 00421d86 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421d87 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421d88 */
    f->r[1]=mi_read((0x00000000u+f->r[6]),4u);
    /* 00421d8a */
    f->r[4]-=4;MI_U32(f->r[4])=0x00421d8fu;f->boundary(0x00421d8au,0x0041fad0u,f);
    /* 00421d8f */
    f->r[0]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00421d93 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00421d95 */
    if((f->flags&MI_ZF))goto mi_00421db0;
    /* 00421d97 */
    f->r[2]=mi_read((0x00000014u+f->r[4]),4u);
    /* 00421d9b */
    f->r[1]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 00421d9f */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00421da1 */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00421da3 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421da4 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421da5 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421da6 */
    f->r[1]=0x00f55bb0u;
    /* 00421dab */
    f->r[4]-=4;MI_U32(f->r[4])=0x00421db0u;f->boundary(0x00421dabu,0x0053e590u,f);
mi_00421db0:;
    /* 00421db0 */
    a=f->r[6];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00421db3 */
    a=f->r[6];b=0x005be810u;t=mi_alu(f,a,b,32,1);
    /* 00421db9 */
    if(mi_signed_less(f->flags))goto mi_00421d65;
    /* 00421dbb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00421dbc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00421dbd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00421dbe */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00421dbf */
    f->r[4]+=4;return;
mi_00421dc0:;
    /* 00421dc0 */
    f->r[3]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00421dc4 */
    f->r[5]=mi_read((0x00000014u+f->r[4]),4u);
    /* 00421dc8 */
    f->r[6]=0x005be660u;
mi_00421dcd:;
    /* 00421dcd */
    f->r[2]=mi_read((0x00799a38u+f->r[7]),4u);
    /* 00421dd3 */
    f->r[0]=mi_read((0xfffffff8u+f->r[6]),4u);
    /* 00421dd6 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00421dd8 */
    if(!(f->flags&MI_ZF))goto mi_00421e18;
    /* 00421dda */
    f->r[0]=mi_read((0x00799a68u+f->r[7]),4u);
    /* 00421de0 */
    f->r[1]=(0x00000018u+f->r[4]);
    /* 00421de4 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421de5 */
    f->r[2]=(0x00000018u+f->r[4]);
    /* 00421de9 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421dea */
    f->r[1]=mi_read((0x00000000u+f->r[6]),4u);
    /* 00421dec */
    f->r[0]=(0x00000024u+f->r[4]);
    /* 00421df0 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421df1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421df2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00421df7u;f->boundary(0x00421df2u,0x0041fad0u,f);
    /* 00421df7 */
    f->r[0]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00421dfb */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00421dfd */
    if((f->flags&MI_ZF))goto mi_00421e18;
    /* 00421dff */
    f->r[1]=mi_read((0x00000014u+f->r[4]),4u);
    /* 00421e03 */
    f->r[2]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 00421e07 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00421e09 */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00421e0b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421e0c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421e0d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421e0e */
    f->r[1]=0x00f55bb0u;
    /* 00421e13 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00421e18u;f->boundary(0x00421e13u,0x0053e590u,f);
mi_00421e18:;
    /* 00421e18 */
    a=f->r[6];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00421e1b */
    a=f->r[6];b=0x005be690u;t=mi_alu(f,a,b,32,1);
    /* 00421e21 */
    if(mi_signed_less(f->flags))goto mi_00421dcd;
    /* 00421e23 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00421e24 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00421e25 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00421e26 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00421e27 */
    f->r[4]+=4;return;
mi_00421e28:;
    /* 00421e28 */
    f->r[3]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00421e2c */
    f->r[5]=mi_read((0x00000014u+f->r[4]),4u);
    /* 00421e30 */
    f->r[6]=0x005be690u;
mi_00421e35:;
    /* 00421e35 */
    f->r[0]=mi_read((0x00799a38u+f->r[7]),4u);
    /* 00421e3b */
    f->r[1]=mi_read((0xfffffff8u+f->r[6]),4u);
    /* 00421e3e */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 00421e40 */
    if(!(f->flags&MI_ZF))goto mi_00421e80;
    /* 00421e42 */
    f->r[1]=mi_read((0x00799a68u+f->r[7]),4u);
    /* 00421e48 */
    f->r[2]=(0x00000018u+f->r[4]);
    /* 00421e4c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421e4d */
    f->r[0]=(0x00000018u+f->r[4]);
    /* 00421e51 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421e52 */
    f->r[1]=(0x00000024u+f->r[4]);
    /* 00421e56 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421e57 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421e58 */
    f->r[1]=mi_read((0x00000000u+f->r[6]),4u);
    /* 00421e5a */
    f->r[4]-=4;MI_U32(f->r[4])=0x00421e5fu;f->boundary(0x00421e5au,0x0041fad0u,f);
    /* 00421e5f */
    f->r[0]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00421e63 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00421e65 */
    if((f->flags&MI_ZF))goto mi_00421e80;
    /* 00421e67 */
    f->r[2]=mi_read((0x00000014u+f->r[4]),4u);
    /* 00421e6b */
    f->r[1]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 00421e6f */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00421e71 */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00421e73 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421e74 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421e75 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421e76 */
    f->r[1]=0x00f55bb0u;
    /* 00421e7b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00421e80u;f->boundary(0x00421e7bu,0x0053e590u,f);
mi_00421e80:;
    /* 00421e80 */
    a=f->r[6];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00421e83 */
    a=f->r[6];b=0x005be6c0u;t=mi_alu(f,a,b,32,1);
    /* 00421e89 */
    if(mi_signed_less(f->flags))goto mi_00421e35;
    /* 00421e8b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00421e8c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00421e8d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00421e8e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00421e8f */
    f->r[4]+=4;return;
mi_00421e90:;
    /* 00421e90 */
    f->r[3]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00421e94 */
    f->r[5]=mi_read((0x00000014u+f->r[4]),4u);
    /* 00421e98 */
    f->r[6]=0x005be8a0u;
mi_00421e9d:;
    /* 00421e9d */
    f->r[2]=mi_read((0x00799a38u+f->r[7]),4u);
    /* 00421ea3 */
    f->r[0]=mi_read((0xfffffff8u+f->r[6]),4u);
    /* 00421ea6 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00421ea8 */
    if(!(f->flags&MI_ZF))goto mi_00421ee8;
    /* 00421eaa */
    f->r[0]=mi_read((0x00799a68u+f->r[7]),4u);
    /* 00421eb0 */
    f->r[1]=(0x00000018u+f->r[4]);
    /* 00421eb4 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421eb5 */
    f->r[2]=(0x00000018u+f->r[4]);
    /* 00421eb9 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421eba */
    f->r[1]=mi_read((0x00000000u+f->r[6]),4u);
    /* 00421ebc */
    f->r[0]=(0x00000024u+f->r[4]);
    /* 00421ec0 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421ec1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421ec2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00421ec7u;f->boundary(0x00421ec2u,0x0041fad0u,f);
    /* 00421ec7 */
    f->r[0]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00421ecb */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00421ecd */
    if((f->flags&MI_ZF))goto mi_00421ee8;
    /* 00421ecf */
    f->r[1]=mi_read((0x00000014u+f->r[4]),4u);
    /* 00421ed3 */
    f->r[2]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 00421ed7 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00421ed9 */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00421edb */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421edc */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421edd */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421ede */
    f->r[1]=0x00f55bb0u;
    /* 00421ee3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00421ee8u;f->boundary(0x00421ee3u,0x0053e590u,f);
mi_00421ee8:;
    /* 00421ee8 */
    a=f->r[6];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00421eeb */
    a=f->r[6];b=0x005be8d0u;t=mi_alu(f,a,b,32,1);
    /* 00421ef1 */
    if(mi_signed_less(f->flags))goto mi_00421e9d;
    /* 00421ef3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00421ef4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00421ef5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00421ef6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00421ef7 */
    f->r[4]+=4;return;
mi_00421ef8:;
    /* 00421ef8 */
    f->r[3]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00421efc */
    f->r[5]=mi_read((0x00000014u+f->r[4]),4u);
    /* 00421f00 */
    f->r[6]=0x005be810u;
mi_00421f05:;
    /* 00421f05 */
    f->r[0]=mi_read((0x00799a38u+f->r[7]),4u);
    /* 00421f0b */
    f->r[1]=mi_read((0xfffffff8u+f->r[6]),4u);
    /* 00421f0e */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 00421f10 */
    if(!(f->flags&MI_ZF))goto mi_00421f50;
    /* 00421f12 */
    f->r[1]=mi_read((0x00799a68u+f->r[7]),4u);
    /* 00421f18 */
    f->r[2]=(0x00000018u+f->r[4]);
    /* 00421f1c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421f1d */
    f->r[0]=(0x00000018u+f->r[4]);
    /* 00421f21 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421f22 */
    f->r[1]=(0x00000024u+f->r[4]);
    /* 00421f26 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421f27 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421f28 */
    f->r[1]=mi_read((0x00000000u+f->r[6]),4u);
    /* 00421f2a */
    f->r[4]-=4;MI_U32(f->r[4])=0x00421f2fu;f->boundary(0x00421f2au,0x0041fad0u,f);
    /* 00421f2f */
    f->r[0]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00421f33 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00421f35 */
    if((f->flags&MI_ZF))goto mi_00421f50;
    /* 00421f37 */
    f->r[2]=mi_read((0x00000014u+f->r[4]),4u);
    /* 00421f3b */
    f->r[1]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 00421f3f */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00421f41 */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00421f43 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421f44 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421f45 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421f46 */
    f->r[1]=0x00f55bb0u;
    /* 00421f4b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00421f50u;f->boundary(0x00421f4bu,0x0053e590u,f);
mi_00421f50:;
    /* 00421f50 */
    a=f->r[6];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00421f53 */
    a=f->r[6];b=0x005be840u;t=mi_alu(f,a,b,32,1);
    /* 00421f59 */
    if(mi_signed_less(f->flags))goto mi_00421f05;
    /* 00421f5b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00421f5c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00421f5d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00421f5e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00421f5f */
    f->r[4]+=4;return;
mi_00421f60:;
    /* 00421f60 */
    f->r[3]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00421f64 */
    f->r[5]=mi_read((0x00000014u+f->r[4]),4u);
    /* 00421f68 */
    f->r[6]=0x005be8d0u;
mi_00421f6d:;
    /* 00421f6d */
    f->r[2]=mi_read((0x00799a38u+f->r[7]),4u);
    /* 00421f73 */
    f->r[0]=mi_read((0xfffffff8u+f->r[6]),4u);
    /* 00421f76 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00421f78 */
    if(!(f->flags&MI_ZF))goto mi_00421fb8;
    /* 00421f7a */
    f->r[0]=mi_read((0x00799a68u+f->r[7]),4u);
    /* 00421f80 */
    f->r[1]=(0x00000018u+f->r[4]);
    /* 00421f84 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421f85 */
    f->r[2]=(0x00000018u+f->r[4]);
    /* 00421f89 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421f8a */
    f->r[1]=mi_read((0x00000000u+f->r[6]),4u);
    /* 00421f8c */
    f->r[0]=(0x00000024u+f->r[4]);
    /* 00421f90 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421f91 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421f92 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00421f97u;f->boundary(0x00421f92u,0x0041fad0u,f);
    /* 00421f97 */
    f->r[0]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00421f9b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00421f9d */
    if((f->flags&MI_ZF))goto mi_00421fb8;
    /* 00421f9f */
    f->r[1]=mi_read((0x00000014u+f->r[4]),4u);
    /* 00421fa3 */
    f->r[2]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 00421fa7 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00421fa9 */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00421fab */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421fac */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421fad */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421fae */
    f->r[1]=0x00f55bb0u;
    /* 00421fb3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00421fb8u;f->boundary(0x00421fb3u,0x0053e590u,f);
mi_00421fb8:;
    /* 00421fb8 */
    a=f->r[6];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00421fbb */
    a=f->r[6];b=0x005be900u;t=mi_alu(f,a,b,32,1);
    /* 00421fc1 */
    if(mi_signed_less(f->flags))goto mi_00421f6d;
    /* 00421fc3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00421fc4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00421fc5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00421fc6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00421fc7 */
    f->r[4]+=4;return;
mi_00421fc8:;
    /* 00421fc8 */
    f->r[3]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00421fcc */
    f->r[5]=mi_read((0x00000014u+f->r[4]),4u);
    /* 00421fd0 */
    f->r[6]=0x005be900u;
mi_00421fd5:;
    /* 00421fd5 */
    f->r[0]=mi_read((0x00799a38u+f->r[7]),4u);
    /* 00421fdb */
    f->r[1]=mi_read((0xfffffff8u+f->r[6]),4u);
    /* 00421fde */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 00421fe0 */
    if(!(f->flags&MI_ZF))goto mi_00422020;
    /* 00421fe2 */
    f->r[1]=mi_read((0x00799a68u+f->r[7]),4u);
    /* 00421fe8 */
    f->r[2]=(0x00000018u+f->r[4]);
    /* 00421fec */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421fed */
    f->r[0]=(0x00000018u+f->r[4]);
    /* 00421ff1 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421ff2 */
    f->r[1]=(0x00000024u+f->r[4]);
    /* 00421ff6 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421ff7 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00421ff8 */
    f->r[1]=mi_read((0x00000000u+f->r[6]),4u);
    /* 00421ffa */
    f->r[4]-=4;MI_U32(f->r[4])=0x00421fffu;f->boundary(0x00421ffau,0x0041fad0u,f);
    /* 00421fff */
    f->r[0]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00422003 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00422005 */
    if((f->flags&MI_ZF))goto mi_00422020;
    /* 00422007 */
    f->r[2]=mi_read((0x00000014u+f->r[4]),4u);
    /* 0042200b */
    f->r[1]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 0042200f */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00422011 */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00422013 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00422014 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00422015 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00422016 */
    f->r[1]=0x00f55bb0u;
    /* 0042201b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00422020u;f->boundary(0x0042201bu,0x0053e590u,f);
mi_00422020:;
    /* 00422020 */
    a=f->r[6];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00422023 */
    a=f->r[6];b=0x005be930u;t=mi_alu(f,a,b,32,1);
    /* 00422029 */
    if(mi_signed_less(f->flags))goto mi_00421fd5;
    /* 0042202b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0042202c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0042202d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0042202e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0042202f */
    f->r[4]+=4;return;
mi_00422030:;
    /* 00422030 */
    f->r[3]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00422034 */
    f->r[5]=mi_read((0x00000014u+f->r[4]),4u);
    /* 00422038 */
    f->r[6]=0x005be6c0u;
mi_0042203d:;
    /* 0042203d */
    f->r[2]=mi_read((0x00799a38u+f->r[7]),4u);
    /* 00422043 */
    f->r[0]=mi_read((0xfffffff8u+f->r[6]),4u);
    /* 00422046 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00422048 */
    if(!(f->flags&MI_ZF))goto mi_00422088;
    /* 0042204a */
    f->r[0]=mi_read((0x00799a68u+f->r[7]),4u);
    /* 00422050 */
    f->r[1]=(0x00000018u+f->r[4]);
    /* 00422054 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00422055 */
    f->r[2]=(0x00000018u+f->r[4]);
    /* 00422059 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0042205a */
    f->r[1]=mi_read((0x00000000u+f->r[6]),4u);
    /* 0042205c */
    f->r[0]=(0x00000024u+f->r[4]);
    /* 00422060 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00422061 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00422062 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00422067u;f->boundary(0x00422062u,0x0041fad0u,f);
    /* 00422067 */
    f->r[0]=mi_read((0x00000018u+f->r[4]),4u);
    /* 0042206b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0042206d */
    if((f->flags&MI_ZF))goto mi_00422088;
    /* 0042206f */
    f->r[1]=mi_read((0x00000014u+f->r[4]),4u);
    /* 00422073 */
    f->r[2]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 00422077 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00422079 */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0042207b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0042207c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0042207d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0042207e */
    f->r[1]=0x00f55bb0u;
    /* 00422083 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00422088u;f->boundary(0x00422083u,0x0053e590u,f);
mi_00422088:;
    /* 00422088 */
    a=f->r[6];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0042208b */
    a=f->r[6];b=0x005be6f0u;t=mi_alu(f,a,b,32,1);
    /* 00422091 */
    if(mi_signed_less(f->flags))goto mi_0042203d;
    /* 00422093 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00422094 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00422095 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00422096 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00422097 */
    f->r[4]+=4;return;
mi_00422098:;
    /* 00422098 */
    f->r[3]=mi_read((0x00000018u+f->r[4]),4u);
    /* 0042209c */
    f->r[5]=mi_read((0x00000014u+f->r[4]),4u);
    /* 004220a0 */
    f->r[6]=0x005be6f0u;
mi_004220a5:;
    /* 004220a5 */
    f->r[0]=mi_read((0x00799a38u+f->r[7]),4u);
    /* 004220ab */
    f->r[1]=mi_read((0xfffffff8u+f->r[6]),4u);
    /* 004220ae */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004220b0 */
    if(!(f->flags&MI_ZF))goto mi_004220f0;
    /* 004220b2 */
    f->r[1]=mi_read((0x00799a68u+f->r[7]),4u);
    /* 004220b8 */
    f->r[2]=(0x00000018u+f->r[4]);
    /* 004220bc */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004220bd */
    f->r[0]=(0x00000018u+f->r[4]);
    /* 004220c1 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004220c2 */
    f->r[1]=(0x00000024u+f->r[4]);
    /* 004220c6 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004220c7 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004220c8 */
    f->r[1]=mi_read((0x00000000u+f->r[6]),4u);
    /* 004220ca */
    f->r[4]-=4;MI_U32(f->r[4])=0x004220cfu;f->boundary(0x004220cau,0x0041fad0u,f);
    /* 004220cf */
    f->r[0]=mi_read((0x00000018u+f->r[4]),4u);
    /* 004220d3 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004220d5 */
    if((f->flags&MI_ZF))goto mi_004220f0;
    /* 004220d7 */
    f->r[2]=mi_read((0x00000014u+f->r[4]),4u);
    /* 004220db */
    f->r[1]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 004220df */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004220e1 */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004220e3 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004220e4 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004220e5 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004220e6 */
    f->r[1]=0x00f55bb0u;
    /* 004220eb */
    f->r[4]-=4;MI_U32(f->r[4])=0x004220f0u;f->boundary(0x004220ebu,0x0053e590u,f);
mi_004220f0:;
    /* 004220f0 */
    a=f->r[6];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004220f3 */
    a=f->r[6];b=0x005be720u;t=mi_alu(f,a,b,32,1);
    /* 004220f9 */
    if(mi_signed_less(f->flags))goto mi_004220a5;
    /* 004220fb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004220fc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004220fd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004220fe */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004220ff */
    f->r[4]+=4;return;
mi_00422100:;
    /* 00422100 */
    f->r[3]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00422104 */
    f->r[5]=mi_read((0x00000014u+f->r[4]),4u);
    /* 00422108 */
    f->r[6]=0x005be720u;
mi_0042210d:;
    /* 0042210d */
    f->r[2]=mi_read((0x00799a38u+f->r[7]),4u);
    /* 00422113 */
    f->r[0]=mi_read((0xfffffff8u+f->r[6]),4u);
    /* 00422116 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00422118 */
    if(!(f->flags&MI_ZF))goto mi_00422158;
    /* 0042211a */
    f->r[0]=mi_read((0x00799a68u+f->r[7]),4u);
    /* 00422120 */
    f->r[1]=(0x00000018u+f->r[4]);
    /* 00422124 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00422125 */
    f->r[2]=(0x00000018u+f->r[4]);
    /* 00422129 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0042212a */
    f->r[1]=mi_read((0x00000000u+f->r[6]),4u);
    /* 0042212c */
    f->r[0]=(0x00000024u+f->r[4]);
    /* 00422130 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00422131 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00422132 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00422137u;f->boundary(0x00422132u,0x0041fad0u,f);
    /* 00422137 */
    f->r[0]=mi_read((0x00000018u+f->r[4]),4u);
    /* 0042213b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0042213d */
    if((f->flags&MI_ZF))goto mi_00422158;
    /* 0042213f */
    f->r[1]=mi_read((0x00000014u+f->r[4]),4u);
    /* 00422143 */
    f->r[2]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 00422147 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00422149 */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0042214b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0042214c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0042214d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0042214e */
    f->r[1]=0x00f55bb0u;
    /* 00422153 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00422158u;f->boundary(0x00422153u,0x0053e590u,f);
mi_00422158:;
    /* 00422158 */
    a=f->r[6];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0042215b */
    a=f->r[6];b=0x005be750u;t=mi_alu(f,a,b,32,1);
    /* 00422161 */
    if(mi_signed_less(f->flags))goto mi_0042210d;
    /* 00422163 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00422164 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00422165 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00422166 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00422167 */
    f->r[4]+=4;return;
mi_00422168:;
    /* 00422168 */
    f->r[3]=mi_read((0x00000018u+f->r[4]),4u);
    /* 0042216c */
    f->r[5]=mi_read((0x00000014u+f->r[4]),4u);
    /* 00422170 */
    f->r[6]=0x005be840u;
mi_00422175:;
    /* 00422175 */
    f->r[0]=mi_read((0x00799a38u+f->r[7]),4u);
    /* 0042217b */
    f->r[1]=mi_read((0xfffffff8u+f->r[6]),4u);
    /* 0042217e */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 00422180 */
    if(!(f->flags&MI_ZF))goto mi_004221c0;
    /* 00422182 */
    f->r[1]=mi_read((0x00799a68u+f->r[7]),4u);
    /* 00422188 */
    f->r[2]=(0x00000018u+f->r[4]);
    /* 0042218c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0042218d */
    f->r[0]=(0x00000018u+f->r[4]);
    /* 00422191 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00422192 */
    f->r[1]=(0x00000024u+f->r[4]);
    /* 00422196 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00422197 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00422198 */
    f->r[1]=mi_read((0x00000000u+f->r[6]),4u);
    /* 0042219a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0042219fu;f->boundary(0x0042219au,0x0041fad0u,f);
    /* 0042219f */
    f->r[0]=mi_read((0x00000018u+f->r[4]),4u);
    /* 004221a3 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004221a5 */
    if((f->flags&MI_ZF))goto mi_004221c0;
    /* 004221a7 */
    f->r[2]=mi_read((0x00000014u+f->r[4]),4u);
    /* 004221ab */
    f->r[1]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 004221af */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004221b1 */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004221b3 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004221b4 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004221b5 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004221b6 */
    f->r[1]=0x00f55bb0u;
    /* 004221bb */
    f->r[4]-=4;MI_U32(f->r[4])=0x004221c0u;f->boundary(0x004221bbu,0x0053e590u,f);
mi_004221c0:;
    /* 004221c0 */
    a=f->r[6];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004221c3 */
    a=f->r[6];b=0x005be870u;t=mi_alu(f,a,b,32,1);
    /* 004221c9 */
    if(mi_signed_less(f->flags))goto mi_00422175;
    /* 004221cb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004221cc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004221cd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004221ce */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004221cf */
    f->r[4]+=4;return;
mi_004221d0:;
    /* 004221d0 */
    f->r[3]=mi_read((0x00000018u+f->r[4]),4u);
    /* 004221d4 */
    f->r[5]=mi_read((0x00000014u+f->r[4]),4u);
    /* 004221d8 */
    f->r[6]=0x005be870u;
mi_004221dd:;
    /* 004221dd */
    f->r[2]=mi_read((0x00799a38u+f->r[7]),4u);
    /* 004221e3 */
    f->r[0]=mi_read((0xfffffff8u+f->r[6]),4u);
    /* 004221e6 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004221e8 */
    if(!(f->flags&MI_ZF))goto mi_00422228;
    /* 004221ea */
    f->r[0]=mi_read((0x00799a68u+f->r[7]),4u);
    /* 004221f0 */
    f->r[1]=(0x00000018u+f->r[4]);
    /* 004221f4 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004221f5 */
    f->r[2]=(0x00000018u+f->r[4]);
    /* 004221f9 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004221fa */
    f->r[1]=mi_read((0x00000000u+f->r[6]),4u);
    /* 004221fc */
    f->r[0]=(0x00000024u+f->r[4]);
    /* 00422200 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00422201 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00422202 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00422207u;f->boundary(0x00422202u,0x0041fad0u,f);
    /* 00422207 */
    f->r[0]=mi_read((0x00000018u+f->r[4]),4u);
    /* 0042220b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0042220d */
    if((f->flags&MI_ZF))goto mi_00422228;
    /* 0042220f */
    f->r[1]=mi_read((0x00000014u+f->r[4]),4u);
    /* 00422213 */
    f->r[2]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 00422217 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00422219 */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0042221b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0042221c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0042221d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0042221e */
    f->r[1]=0x00f55bb0u;
    /* 00422223 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00422228u;f->boundary(0x00422223u,0x0053e590u,f);
mi_00422228:;
    /* 00422228 */
    a=f->r[6];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0042222b */
    a=f->r[6];b=0x005be8a0u;t=mi_alu(f,a,b,32,1);
    /* 00422231 */
    if(mi_signed_less(f->flags))goto mi_004221dd;
    /* 00422233 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00422234 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00422235 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00422236 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00422237 */
    f->r[4]+=4;return;
mi_00422238:;
    /* 00422238 */
    f->r[3]=mi_read((0x00000018u+f->r[4]),4u);
    /* 0042223c */
    f->r[5]=mi_read((0x00000014u+f->r[4]),4u);
    /* 00422240 */
    f->r[6]=0x005be930u;
mi_00422245:;
    /* 00422245 */
    f->r[0]=mi_read((0x00799a38u+f->r[7]),4u);
    /* 0042224b */
    f->r[1]=mi_read((0xfffffff8u+f->r[6]),4u);
    /* 0042224e */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 00422250 */
    if(!(f->flags&MI_ZF))goto mi_00422290;
    /* 00422252 */
    f->r[1]=mi_read((0x00799a68u+f->r[7]),4u);
    /* 00422258 */
    f->r[2]=(0x00000018u+f->r[4]);
    /* 0042225c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0042225d */
    f->r[0]=(0x00000018u+f->r[4]);
    /* 00422261 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00422262 */
    f->r[1]=(0x00000024u+f->r[4]);
    /* 00422266 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00422267 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00422268 */
    f->r[1]=mi_read((0x00000000u+f->r[6]),4u);
    /* 0042226a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0042226fu;f->boundary(0x0042226au,0x0041fad0u,f);
    /* 0042226f */
    f->r[0]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00422273 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00422275 */
    if((f->flags&MI_ZF))goto mi_00422290;
    /* 00422277 */
    f->r[2]=mi_read((0x00000014u+f->r[4]),4u);
    /* 0042227b */
    f->r[1]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 0042227f */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00422281 */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00422283 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00422284 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00422285 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00422286 */
    f->r[1]=0x00f55bb0u;
    /* 0042228b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00422290u;f->boundary(0x0042228bu,0x0053e590u,f);
mi_00422290:;
    /* 00422290 */
    a=f->r[6];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00422293 */
    a=f->r[6];b=0x005be960u;t=mi_alu(f,a,b,32,1);
    /* 00422299 */
    if(mi_signed_less(f->flags))goto mi_00422245;
    /* 0042229b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0042229c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0042229d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0042229e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0042229f */
    f->r[4]+=4;return;
mi_004222a0:;
    /* 004222a0 */
    f->r[3]=mi_read((0x00000018u+f->r[4]),4u);
    /* 004222a4 */
    f->r[5]=mi_read((0x00000014u+f->r[4]),4u);
    /* 004222a8 */
    f->r[6]=0x005be960u;
mi_004222ad:;
    /* 004222ad */
    f->r[2]=mi_read((0x00799a38u+f->r[7]),4u);
    /* 004222b3 */
    f->r[0]=mi_read((0xfffffff8u+f->r[6]),4u);
    /* 004222b6 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004222b8 */
    if(!(f->flags&MI_ZF))goto mi_004222f8;
    /* 004222ba */
    f->r[0]=mi_read((0x00799a68u+f->r[7]),4u);
    /* 004222c0 */
    f->r[1]=(0x00000018u+f->r[4]);
    /* 004222c4 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004222c5 */
    f->r[2]=(0x00000018u+f->r[4]);
    /* 004222c9 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004222ca */
    f->r[1]=mi_read((0x00000000u+f->r[6]),4u);
    /* 004222cc */
    f->r[0]=(0x00000024u+f->r[4]);
    /* 004222d0 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004222d1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004222d2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004222d7u;f->boundary(0x004222d2u,0x0041fad0u,f);
    /* 004222d7 */
    f->r[0]=mi_read((0x00000018u+f->r[4]),4u);
    /* 004222db */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004222dd */
    if((f->flags&MI_ZF))goto mi_004222f8;
    /* 004222df */
    f->r[1]=mi_read((0x00000014u+f->r[4]),4u);
    /* 004222e3 */
    f->r[2]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 004222e7 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004222e9 */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004222eb */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004222ec */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004222ed */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004222ee */
    f->r[1]=0x00f55bb0u;
    /* 004222f3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004222f8u;f->boundary(0x004222f3u,0x0053e590u,f);
mi_004222f8:;
    /* 004222f8 */
    a=f->r[6];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004222fb */
    a=f->r[6];b=0x005be990u;t=mi_alu(f,a,b,32,1);
    /* 00422301 */
    if(mi_signed_less(f->flags))goto mi_004222ad;
    /* 00422303 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00422304 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00422305 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00422306 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00422307 */
    f->r[4]+=4;return;
mi_00422308:;
    /* 00422308 */
    f->r[3]=mi_read((0x00000018u+f->r[4]),4u);
    /* 0042230c */
    f->r[5]=mi_read((0x00000014u+f->r[4]),4u);
    /* 00422310 */
    f->r[6]=0x005be990u;
mi_00422315:;
    /* 00422315 */
    f->r[0]=mi_read((0x00799a38u+f->r[7]),4u);
    /* 0042231b */
    f->r[1]=mi_read((0xfffffff8u+f->r[6]),4u);
    /* 0042231e */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 00422320 */
    if(!(f->flags&MI_ZF))goto mi_00422360;
    /* 00422322 */
    f->r[1]=mi_read((0x00799a68u+f->r[7]),4u);
    /* 00422328 */
    f->r[2]=(0x00000018u+f->r[4]);
    /* 0042232c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0042232d */
    f->r[0]=(0x00000018u+f->r[4]);
    /* 00422331 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00422332 */
    f->r[1]=(0x00000024u+f->r[4]);
    /* 00422336 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00422337 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00422338 */
    f->r[1]=mi_read((0x00000000u+f->r[6]),4u);
    /* 0042233a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0042233fu;f->boundary(0x0042233au,0x0041fad0u,f);
    /* 0042233f */
    f->r[0]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00422343 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00422345 */
    if((f->flags&MI_ZF))goto mi_00422360;
    /* 00422347 */
    f->r[2]=mi_read((0x00000014u+f->r[4]),4u);
    /* 0042234b */
    f->r[1]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 0042234f */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00422351 */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00422353 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00422354 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00422355 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00422356 */
    f->r[1]=0x00f55bb0u;
    /* 0042235b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00422360u;f->boundary(0x0042235bu,0x0053e590u,f);
mi_00422360:;
    /* 00422360 */
    a=f->r[6];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00422363 */
    a=f->r[6];b=0x005be9c0u;t=mi_alu(f,a,b,32,1);
    /* 00422369 */
    if(mi_signed_less(f->flags))goto mi_00422315;
mi_0042236b:;
    /* 0042236b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0042236c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0042236d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0042236e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0042236f */
    f->r[4]+=4;return;
}
