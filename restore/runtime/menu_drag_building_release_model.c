/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "building_release_model.h"
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

static const uint8_t mi_rom_0046afc8[]={0xc1,0xad,0x46,0x00,0x2b,0xae,0x46,0x00,0x10,0xaf,0x46,0x00,0x57,0xad,0x46,0x00,0xaa,0xad,0x46,0x00,0x6e,0xad,0x46,0x00,0xfd,0xad,0x46,0x00,0x1e,0xaf,0x46,0x00,0x54,0xaf,0x46,0x00,0x5f,0xae,0x46,0x00,0x1a,0xad,0x46,0x00,0xee,0xac,0x46,0x00,0x40,0xae,0x46,0x00,0x80,0xae,0x46,0x00,0xab,0xae,0x46,0x00,0x6d,0xae,0x46,0x00,0xbf,0xae,0x46,0x00,0xd3,0xae,0x46,0x00,0xe7,0xae,0x46,0x00,0x64,0xaf,0x46,0x00};
static const uint8_t mi_rom_0046b018[]={0x00,0x13,0x13,0x13,0x13,0x13,0x13,0x01,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x02,0x13,0x13,0x03,0x03,0x03,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x04,0x05,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x06,0x13,0x07,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x08,0x09,0x13,0x13,0x13,0x0a,0x0a,0x0a,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x0b,0x0c,0x13,0x13,0x09,0x13,0x07,0x13,0x13,0x13,0x13,0x13,0x13,0x09,0x13,0x13,0x09,0x13,0x09,0x0d,0x0e,0x0f,0x13,0x13,0x13,0x10,0x11,0x12};
static __inline uint32_t mi_read(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x0046afc8u&&width<=80u&&(offset=address-0x0046afc8u)<=80u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0046afc8[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0046b018u&&width<=187u&&(offset=address-0x0046b018u)<=187u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0046b018[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}

void restore_C_0046ac50(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0046ac50 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ac51 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ac52 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ac53 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ac54 */
    f->r[7]=mi_read((0x00000014u+f->r[4]),4u);
    /* 0046ac58 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 0046ac5a */
    f->r[6]=f->r[7];
    /* 0046ac5c */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0046ac5f */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0046ac61 */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0046ac64 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00799a99u+f->r[6]),1u))&0xffu)<<0);
    /* 0046ac6a */
    a=(f->r[0]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 0046ac6c */
    if((f->flags&MI_ZF))goto mi_0046ac88;
    /* 0046ac6e */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 0046ac73 */
    MI_U8((0x00799a99u+f->r[6]))=(uint8_t)((f->r[3]&0xffu));
    /* 0046ac79 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0046ac7c */
    a=f->r[1];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0046ac7f */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046ac81 */
    MI_U8((0x007900c0u+f->r[1]*4u))=(uint8_t)((f->r[3]&0xffu));
mi_0046ac88:;
    /* 0046ac88 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00799aa2u+f->r[6]),1u))&0xffu)<<0);
    /* 0046ac8e */
    a=(f->r[0]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 0046ac90 */
    if((f->flags&MI_ZF))goto mi_0046acaf;
    /* 0046ac92 */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 0046ac97 */
    MI_U8((0x00799aa2u+f->r[6]))=(uint8_t)((f->r[3]&0xffu));
    /* 0046ac9d */
    f->r[2]=(0x00000000u+f->r[0]*8u);
    /* 0046aca4 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0046aca6 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0046aca9 */
    MI_U32((0x00eaaf68u+f->r[2]))=(uint32_t)(f->r[3]);
mi_0046acaf:;
    /* 0046acaf */
    a=mi_read((0x00799aa3u+f->r[6]),1u);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 0046acb5 */
    if((f->flags&MI_ZF))goto mi_0046acc6;
    /* 0046acb7 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046acb8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046acbdu;f->boundary(0x0046acb8u,0x0046a2e0u,f);
    /* 0046acbd */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046acc0 */
    MI_U8((0x00799aa3u+f->r[6]))=(uint8_t)((f->r[3]&0xffu));
mi_0046acc6:;
    /* 0046acc6 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x007999e8u+f->r[6]),2u))&0xffffu)<<0);
    /* 0046accd */
    f->r[1]=(uint32_t)(int32_t)(int16_t)(f->r[0]&0xffffu);
    /* 0046acd0 */
    f->r[5]=(0xffffffceu+f->r[1]);
    /* 0046acd3 */
    a=f->r[5];b=0x000000bau;t=mi_alu(f,a,b,32,1);
    /* 0046acd9 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0046af64;
    /* 0046acdf */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0046ace1 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x0046b018u+f->r[5]),1u))&0xffu)<<0);
    /* 0046ace7 */
    switch(mi_read((0x0046afc8u+f->r[2]*4u),4u)){case 0x0046aceeu:goto mi_0046acee;case 0x0046ad1au:goto mi_0046ad1a;case 0x0046ad57u:goto mi_0046ad57;case 0x0046ad6eu:goto mi_0046ad6e;case 0x0046adaau:goto mi_0046adaa;case 0x0046adc1u:goto mi_0046adc1;case 0x0046adfdu:goto mi_0046adfd;case 0x0046ae2bu:goto mi_0046ae2b;case 0x0046ae40u:goto mi_0046ae40;case 0x0046ae5fu:goto mi_0046ae5f;case 0x0046ae6du:goto mi_0046ae6d;case 0x0046ae80u:goto mi_0046ae80;case 0x0046aeabu:goto mi_0046aeab;case 0x0046aebfu:goto mi_0046aebf;case 0x0046aed3u:goto mi_0046aed3;case 0x0046aee7u:goto mi_0046aee7;case 0x0046af10u:goto mi_0046af10;case 0x0046af1eu:goto mi_0046af1e;case 0x0046af54u:goto mi_0046af54;case 0x0046af64u:goto mi_0046af64;default:abort();}
mi_0046acee:;
    /* 0046acee */
    f->r[0]=mi_read((0x00eaac28u),4u);
    /* 0046acf3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0046acf4 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0046acfa */
    a=f->r[1];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[1]=t;
    /* 0046acfd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046acfe */
    MI_U32((0x00ea6010u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 0046ad04 */
    MI_U32((0x00ea6014u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 0046ad0a */
    MI_U16((0x00ea6084u+f->r[0]))=(uint16_t)((f->r[3]&0xffffu));
    /* 0046ad11 */
    MI_U32((0x00775678u))=(uint32_t)(f->r[3]);
    /* 0046ad17 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0046ad18 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0046ad19 */
    f->r[4]+=4;return;
mi_0046ad1a:;
    /* 0046ad1a */
    f->r[0]=mi_read((0x00eaac28u),4u);
    /* 0046ad1f */
    f->r[1]=mi_read((0x007999e4u+f->r[6]),4u);
    /* 0046ad25 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0046ad2b */
    a=f->r[1];b=mi_read((0x00ea3ffcu+f->r[0]),4u);t=mi_alu(f,a,b,32,1);
    /* 0046ad31 */
    if(!(f->flags&MI_ZF))goto mi_0046afc2;
    /* 0046ad37 */
    MI_U32((0x00ea3ffcu+f->r[0]))=(uint32_t)(f->r[3]);
    /* 0046ad3d */
    MI_U16((0x00ea3ff8u+f->r[0]))=(uint16_t)((f->r[3]&0xffffu));
    /* 0046ad44 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0046ad45 */
    MI_U16((0x00ea3ffau+f->r[0]))=(uint16_t)((f->r[3]&0xffffu));
    /* 0046ad4c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046ad4d */
    MI_U16((0x00ea5cf6u+f->r[0]))=(uint16_t)((f->r[3]&0xffffu));
    /* 0046ad54 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0046ad55 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0046ad56 */
    f->r[4]+=4;return;
mi_0046ad57:;
    /* 0046ad57 */
    f->r[2]=mi_read((0x00eaac28u),4u);
    /* 0046ad5d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0046ad5e */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 0046ad64 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046ad65 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0046ad66 */
    MI_U32((0x00ea620cu+f->r[2]))=(uint32_t)(f->r[3]);
    /* 0046ad6c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0046ad6d */
    f->r[4]+=4;return;
mi_0046ad6e:;
    /* 0046ad6e */
    f->r[0]=mi_read((0x00eaac28u),4u);
    /* 0046ad73 */
    f->r[1]=mi_read((0x007999e4u+f->r[6]),4u);
    /* 0046ad79 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0046ad7f */
    a=f->r[1];b=mi_read((0x00ea5d18u+f->r[0]),4u);t=mi_alu(f,a,b,32,1);
    /* 0046ad85 */
    if(!(f->flags&MI_ZF))goto mi_0046afc2;
    /* 0046ad8b */
    MI_U32((0x00ea5d18u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 0046ad91 */
    MI_U16((0x00ea5d14u+f->r[0]))=(uint16_t)((f->r[3]&0xffffu));
    /* 0046ad98 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0046ad99 */
    MI_U16((0x00ea5d16u+f->r[0]))=(uint16_t)((f->r[3]&0xffffu));
    /* 0046ada0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046ada1 */
    MI_U32((0x00ea5d20u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 0046ada7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0046ada8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0046ada9 */
    f->r[4]+=4;return;
mi_0046adaa:;
    /* 0046adaa */
    f->r[2]=mi_read((0x00eaac28u),4u);
    /* 0046adb0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0046adb1 */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 0046adb7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046adb8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0046adb9 */
    MI_U32((0x00ea6240u+f->r[2]))=(uint32_t)(f->r[3]);
    /* 0046adbf */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0046adc0 */
    f->r[4]+=4;return;
mi_0046adc1:;
    /* 0046adc1 */
    f->r[0]=mi_read((0x00eaac28u),4u);
    /* 0046adc6 */
    f->r[1]=mi_read((0x007999e4u+f->r[6]),4u);
    /* 0046adcc */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0046add2 */
    a=f->r[1];b=mi_read((0x00ea5e68u+f->r[0]),4u);t=mi_alu(f,a,b,32,1);
    /* 0046add8 */
    if(!(f->flags&MI_ZF))goto mi_0046afc2;
    /* 0046adde */
    MI_U32((0x00ea5e68u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 0046ade4 */
    MI_U16((0x00ea5e64u+f->r[0]))=(uint16_t)((f->r[3]&0xffffu));
    /* 0046adeb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0046adec */
    MI_U16((0x00ea5e66u+f->r[0]))=(uint16_t)((f->r[3]&0xffffu));
    /* 0046adf3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046adf4 */
    MI_U32((0x00ea5e70u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 0046adfa */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0046adfb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0046adfc */
    f->r[4]+=4;return;
mi_0046adfd:;
    /* 0046adfd */
    f->r[6]=mi_read((0x00799a9cu+f->r[6]),4u);
    /* 0046ae03 */
    a=f->r[6];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 0046ae05 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0046afc2;
    /* 0046ae0b */
    f->r[2]=f->r[6];
    /* 0046ae0d */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ae0e */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0046ae11 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0046ae13 */
    MI_U32((0x00799a9cu+f->r[2]*8u))=(uint32_t)(0xffffffffu);
    /* 0046ae1e */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046ae23u;f->boundary(0x0046ae1eu,0x004a36b0u,f);
    /* 0046ae23 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046ae26 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0046ae27 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046ae28 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0046ae29 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0046ae2a */
    f->r[4]+=4;return;
mi_0046ae2b:;
    /* 0046ae2b */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00799a28u+f->r[6]),2u);
    /* 0046ae32 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ae33 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046ae38u;f->boundary(0x0046ae33u,0x004b82e0u,f);
    /* 0046ae38 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046ae3b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0046ae3c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046ae3d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0046ae3e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0046ae3f */
    f->r[4]+=4;return;
mi_0046ae40:;
    /* 0046ae40 */
    f->r[0]=mi_read((0x00eaac28u),4u);
    /* 0046ae45 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0046ae4b */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00ea5baau+f->r[0]),2u);
    /* 0046ae52 */
    MI_U32((0x00ea6244u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 0046ae58 */
    MI_U16((0x00ea5baau+f->r[0]))=(uint16_t)((f->r[3]&0xffffu));
mi_0046ae5f:;
    /* 0046ae5f */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ae60 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046ae65u;f->boundary(0x0046ae60u,0x004f4ae0u,f);
    /* 0046ae65 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046ae68 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0046ae69 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046ae6a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0046ae6b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0046ae6c */
    f->r[4]+=4;return;
mi_0046ae6d:;
    /* 0046ae6d */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ae6e */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046ae73u;f->boundary(0x0046ae6eu,0x004f4ae0u,f);
    /* 0046ae73 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046ae76 */
    a=mi_read((0x007999eau+f->r[6]),2u);b=0x0000000fu;t=mi_alu(f,a,b,16,1);
    /* 0046ae7e */
    goto mi_0046aef9;
mi_0046ae80:;
    /* 0046ae80 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ae81 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046ae86u;f->boundary(0x0046ae81u,0x004f4ae0u,f);
    /* 0046ae86 */
    f->r[3]=0x00000001u;
    /* 0046ae8b */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046ae8e */
    a=mi_read((0x007999eau+f->r[6]),2u);b=(f->r[3]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 0046ae95 */
    if(!(f->flags&MI_ZF))goto mi_0046afc2;
    /* 0046ae9b */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046aea0u;f->boundary(0x0046ae9bu,0x004dea50u,f);
    /* 0046aea0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0046aea1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046aea2 */
    MI_U8((0x00e38dcau))=(uint8_t)((f->r[3]&0xffu));
    /* 0046aea8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0046aea9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0046aeaa */
    f->r[4]+=4;return;
mi_0046aeab:;
    /* 0046aeab */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046aeac */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046aeb1u;f->boundary(0x0046aeacu,0x004f4ae0u,f);
    /* 0046aeb1 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046aeb4 */
    a=mi_read((0x007999eau+f->r[6]),2u);b=0x000000a6u;t=mi_alu(f,a,b,16,1);
    /* 0046aebd */
    goto mi_0046aef9;
mi_0046aebf:;
    /* 0046aebf */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046aec0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046aec5u;f->boundary(0x0046aec0u,0x004f4ae0u,f);
    /* 0046aec5 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046aec8 */
    a=mi_read((0x007999eau+f->r[6]),2u);b=0x0000009du;t=mi_alu(f,a,b,16,1);
    /* 0046aed1 */
    goto mi_0046aef9;
mi_0046aed3:;
    /* 0046aed3 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046aed4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046aed9u;f->boundary(0x0046aed4u,0x004f4ae0u,f);
    /* 0046aed9 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046aedc */
    a=mi_read((0x007999eau+f->r[6]),2u);b=0x000001b4u;t=mi_alu(f,a,b,16,1);
    /* 0046aee5 */
    goto mi_0046aef9;
mi_0046aee7:;
    /* 0046aee7 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046aee8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046aeedu;f->boundary(0x0046aee8u,0x004f4ae0u,f);
    /* 0046aeed */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046aef0 */
    a=mi_read((0x007999eau+f->r[6]),2u);b=0x00000200u;t=mi_alu(f,a,b,16,1);
mi_0046aef9:;
    /* 0046aef9 */
    if(!(f->flags&MI_ZF))goto mi_0046afc2;
    /* 0046aeff */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046af04u;f->boundary(0x0046aeffu,0x004dea50u,f);
    /* 0046af04 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0046af05 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046af06 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0046af07 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 0046af0e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0046af0f */
    f->r[4]+=4;return;
mi_0046af10:;
    /* 0046af10 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046af11 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046af16u;f->boundary(0x0046af11u,0x0046a2e0u,f);
    /* 0046af16 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046af19 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0046af1a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046af1b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0046af1c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0046af1d */
    f->r[4]+=4;return;
mi_0046af1e:;
    /* 0046af1e */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e0u+f->r[6]),2u);
    /* 0046af25 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999deu+f->r[6]),2u);
    /* 0046af2c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046af2d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046af2e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046af2f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046af30 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046af35u;f->boundary(0x0046af30u,0x0047b590u,f);
    /* 0046af35 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e0u+f->r[6]),2u);
    /* 0046af3c */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999deu+f->r[6]),2u);
    /* 0046af43 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046af44 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046af45 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046af46 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046af47 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046af4cu;f->boundary(0x0046af47u,0x0047bd50u,f);
    /* 0046af4c */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046af4f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0046af50 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046af51 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0046af52 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0046af53 */
    f->r[4]+=4;return;
mi_0046af54:;
    /* 0046af54 */
    f->r[0]=mi_read((0x00bcb9b0u),4u);
    /* 0046af59 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0046af5a */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0046af5b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046af5c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0046af5d */
    MI_U32((0x00bcb9b0u))=(uint32_t)(f->r[0]);
    /* 0046af62 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0046af63 */
    f->r[4]+=4;return;
mi_0046af64:;
    /* 0046af64 */
    a=(f->r[0]&0xffffu);b=0x000000cfu;t=mi_alu(f,a,b,16,1);
    /* 0046af68 */
    if((f->flags&MI_ZF))goto mi_0046af70;
    /* 0046af6a */
    a=(f->r[0]&0xffffu);b=0x000000d0u;t=mi_alu(f,a,b,16,1);
    /* 0046af6e */
    if(!(f->flags&MI_ZF))goto mi_0046af7e;
mi_0046af70:;
    /* 0046af70 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046af71 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046af76u;f->boundary(0x0046af71u,0x004e5830u,f);
    /* 0046af76 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046af79 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0046af7a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046af7b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0046af7c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0046af7d */
    f->r[4]+=4;return;
mi_0046af7e:;
    /* 0046af7e */
    a=f->r[1];b=0x00000041u;t=mi_alu(f,a,b,32,1);
    /* 0046af81 */
    if(mi_signed_less(f->flags))goto mi_0046afc2;
    /* 0046af83 */
    a=f->r[1];b=0x00000045u;t=mi_alu(f,a,b,32,1);
    /* 0046af86 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0046afc2;
    /* 0046af88 */
    f->r[0]=mi_read((0x00eaac28u),4u);
    /* 0046af8d */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046af8e */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0046af94 */
    MI_U16((0x00ea5ce8u+f->r[0]))=(uint16_t)((f->r[3]&0xffffu));
    /* 0046af9b */
    MI_U16((0x00ea5cfau+f->r[0]))=(uint16_t)((f->r[3]&0xffffu));
    /* 0046afa2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046afa7u;f->boundary(0x0046afa2u,0x00475b50u,f);
    /* 0046afa7 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046afa8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046afadu;f->boundary(0x0046afa8u,0x004c0920u,f);
    /* 0046afad */
    f->r[2]=mi_read((0x00eaac28u),4u);
    /* 0046afb3 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046afb6 */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 0046afbc */
    MI_U32((0x00ea5f9cu+f->r[2]))=(uint32_t)(f->r[3]);
mi_0046afc2:;
    /* 0046afc2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0046afc3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046afc4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0046afc5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0046afc6 */
    f->r[4]+=4;return;
}
