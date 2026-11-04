#include "building_removal.h"
/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "invasion_model.h"
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
static __inline uint32_t mi_alu(struct restore_invasion_frame *f,uint32_t a,uint32_t b,uint32_t width,uint32_t operation)
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
static __inline uint32_t mi_shift(struct restore_invasion_frame *f,uint32_t a,uint32_t count,uint32_t width,uint32_t arithmetic)
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
static __inline uint32_t mi_imul(struct restore_invasion_frame *f,uint32_t a,uint32_t b)
{
 int64_t product=(int64_t)(int32_t)a*(int32_t)b;uint32_t result=(uint32_t)product;
 f->flags&=~(MI_CF|MI_OF);if(product!=(int64_t)(int32_t)result)f->flags|=MI_CF|MI_OF;
 return result;
}
static __inline int mi_signed_less(uint32_t flags){return ((flags&MI_SF)!=0)^((flags&MI_OF)!=0);}

static __inline void mi_idiv32(struct restore_invasion_frame *f,uint32_t divisor_bits)
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

static const uint8_t mi_rom_00469548[]={0x58,0x94,0x46,0x00,0x6b,0x94,0x46,0x00,0x7e,0x94,0x46,0x00,0xab,0x94,0x46,0x00};
static const uint8_t mi_rom_00469558[]={0x00,0x00,0x00,0x00,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x01,0x03,0x03,0x02,0x02,0x02};
static __inline uint32_t mi_read(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x00469548u&&width<=16u&&(offset=address-0x00469548u)<=16u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00469548[offset+k]<<(k*8u);return result;
 }
 if(address>=0x00469558u&&width<=106u&&(offset=address-0x00469558u)<=106u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00469558[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}

void restore_invasion_C_00446a30(struct restore_invasion_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00446a30 */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 00446a33 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00446a34 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00446a35 */
    f->r[5]=MI_U32((0x00000088u+f->r[4]));
    /* 00446a3c */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00446a3d */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00446a3e */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 00446a40 */
    a=f->r[5];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 00446a42 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[7]);
    /* 00446a46 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 00446a4a */
    if(mi_signed_less(f->flags))goto mi_00446b6e;
    /* 00446a50 */
    a=f->r[5];b=MI_U32((0x00785d70u));t=mi_alu(f,a,b,32,1);
    /* 00446a56 */
    if(!mi_signed_less(f->flags))goto mi_00446b6e;
    /* 00446a5c */
    f->r[0]=f->r[5];
    /* 00446a5e */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00446a61 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00446a63 */
    f->r[0]=(0x00785d74u+f->r[0]*4u);
    /* 00446a6a */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 00446a6c */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00446a70 */
    if((f->flags&MI_ZF))goto mi_00446a76;
    /* 00446a72 */
    MI_U8((0x00000032u+f->r[0]))=(uint8_t)(0x00000001u);
mi_00446a76:;
    /* 00446a76 */
    f->r[6]=MI_U32((0x00000088u+f->r[4]));
    /* 00446a7d */
    a=f->r[6];b=0x00000009u;t=mi_alu(f,a,b,32,1);
    /* 00446a80 */
    if(mi_signed_less(f->flags))goto mi_00446ef9;
    /* 00446a86 */
    a=f->r[6];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 00446a89 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00446ef9;
    /* 00446a8f */
    a=MI_U32((0x00000080u+f->r[4]));b=0x0000000du;t=mi_alu(f,a,b,32,1);
    /* 00446a97 */
    if((f->flags&MI_ZF))goto mi_00446ef9;
    /* 00446a9d */
    a=MI_U32((0x00000084u+f->r[4]));b=0x00000096u;t=mi_alu(f,a,b,32,1);
    /* 00446aa8 */
    if(mi_signed_less(f->flags))goto mi_00446ab5;
    /* 00446aaa */
    MI_U32((0x00000084u+f->r[4]))=(uint32_t)(0x00000096u);
mi_00446ab5:;
    /* 00446ab5 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00775900u)))&0xffffu)<<0);
    /* 00446abb */
    a=(f->r[0]&0xffffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,16,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=(f->r[0]&0xffff0000u)|(((t)&0xffffu)<<0);
    /* 00446abd */
    a=(f->r[0]&0xffffu);b=0x00007d00u;t=mi_alu(f,a,b,16,1);
    /* 00446ac1 */
    MI_U16((0x00775900u))=(uint16_t)((f->r[0]&0xffffu));
    /* 00446ac7 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00446ad2;
    /* 00446ac9 */
    MI_U16((0x00775900u))=(uint16_t)(0x00000001u);
mi_00446ad2:;
    /* 00446ad2 */
    f->r[0]=0x00784bd0u;
mi_00446ad7:;
    /* 00446ad7 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00446ada */
    a=f->r[0];b=0x00784be0u;t=mi_alu(f,a,b,32,1);
    /* 00446adf */
    if(mi_signed_less(f->flags))goto mi_00446ad7;
    /* 00446ae1 */
    f->r[1]=0x00784dc4u;
    /* 00446ae6 */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[0]=t;
mi_00446ae9:;
    /* 00446ae9 */
    f->r[2]=MI_U32((0x00000000u+f->r[1]));
    /* 00446aeb */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00446aed */
    if((f->flags&MI_ZF))goto mi_00446afb;
    /* 00446aef */
    MI_U32((0x00000034u+f->r[4]+f->r[7]*4u))=(uint32_t)(f->r[2]);
    /* 00446af3 */
    f->r[2]=MI_U32((0x0000000cu+f->r[1]));
    /* 00446af6 */
    MI_U32((0x00000040u+f->r[4]+f->r[7]*4u))=(uint32_t)(f->r[2]);
    /* 00446afa */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
mi_00446afb:;
    /* 00446afb */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00446afe */
    a=f->r[1];b=0x00784dd0u;t=mi_alu(f,a,b,32,1);
    /* 00446b04 */
    if(mi_signed_less(f->flags))goto mi_00446ae9;
    /* 00446b06 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 00446b0a */
    f->r[2]=(0xffffffffu+f->r[6]);
    /* 00446b0d */
    f->r[1]=0x00000008u;
mi_00446b12:;
    /* 00446b12 */
    a=MI_U16((0x00784bc0u+f->r[2]*2u));b=(f->r[0]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00446b1a */
    if(!(f->flags&MI_ZF))goto mi_00446b2d;
    /* 00446b1c */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00446b1d */
    a=f->r[2];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 00446b20 */
    if(mi_signed_less(f->flags))goto mi_00446b27;
    /* 00446b22 */
    f->r[2]=0x00000008u;
mi_00446b27:;
    /* 00446b27 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00446b28 */
    a=f->r[1];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 00446b2b */
    if(mi_signed_less(f->flags))goto mi_00446b12;
mi_00446b2d:;
    /* 00446b2d */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00784bc0u+f->r[2]*2u));
    /* 00446b35 */
    f->r[3]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00784be0u+f->r[2]*2u));
    /* 00446b3d */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00446b3f */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00446b43 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 00446b47 */
    if((f->flags&MI_ZF))goto mi_00446b4d;
    /* 00446b49 */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00446b4b */
    if(!(f->flags&MI_ZF))goto mi_00446b63;
mi_00446b4d:;
    /* 00446b4d */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00784c68u));
    /* 00446b54 */
    f->r[3]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00784c6au));
    /* 00446b5b */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00446b5f */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[3]);
mi_00446b63:;
    /* 00446b63 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 00446b65 */
    if(!(f->flags&MI_ZF))goto mi_00446b77;
    /* 00446b67 */
    f->r[6]=0x00000004u;
    /* 00446b6c */
    goto mi_00446ba7;
mi_00446b6e:;
    /* 00446b6e */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 00446b72 */
    goto mi_00446a76;
mi_00446b77:;
    /* 00446b77 */
    f->r[0]=MI_U32((0x00784908u));
    /* 00446b7c */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00446b7d */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00446b7f */
    if(mi_signed_less(f->flags))goto mi_00446b85;
    /* 00446b81 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 00446b83 */
    goto mi_00446ba7;
mi_00446b85:;
    /* 00446b85 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00446b87 */
    if(!(f->flags&MI_ZF))goto mi_00446b90;
    /* 00446b89 */
    f->r[6]=0x00000002u;
    /* 00446b8e */
    goto mi_00446ba7;
mi_00446b90:;
    /* 00446b90 */
    f->r[2]=MI_U32((0x00784904u));
    /* 00446b96 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00446b98 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00446b99 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 00446b9b */
    t=(mi_signed_less(f->flags))?1u:0u;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00446b9e */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00446b9f */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 00446ba2 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00446ba5 */
    f->r[6]=f->r[0];
mi_00446ba7:;
    /* 00446ba7 */
    f->r[2]=(0x00000000u+f->r[3]+f->r[3]*8u);
    /* 00446baa */
    MI_U32((0x00000088u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 00446bb1 */
    f->r[0]=(0x00000000u+f->r[3]+f->r[2]*2u);
    /* 00446bb4 */
    f->r[2]=MI_U32((0x00bca2d8u));
    /* 00446bba */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 00446bbd */
    f->r[0]=(0x00000000u+f->r[2]+f->r[0]*4u);
    /* 00446bc0 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00446bc2 */
    a=f->r[5];b=0x0000008cu;t=mi_alu(f,a,b,32,1);
    /* 00446bc8 */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00446bcc */
    if(!(f->flags&MI_ZF))goto mi_00446be0;
    /* 00446bce */
    f->r[0]=MI_U32((0x00000084u+f->r[4]));
    /* 00446bd5 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 00446bd6 */
    a=f->r[2];b=0x00000003u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 00446bd9 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00446bdb */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 00446bde */
    goto mi_00446be9;
mi_00446be0:;
    /* 00446be0 */
    f->r[2]=MI_U32((0x00000024u+f->r[4]));
    /* 00446be4 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00446be6 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000036u+f->r[2])))&0xffu)<<0);
mi_00446be9:;
    /* 00446be9 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 00446beb */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00446bef */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00446bf1 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00446cc3;
    /* 00446bf7 */
    goto mi_00446c04;
mi_00446bf9:;
    /* 00446bf9 */
    f->r[6]=MI_U32((0x00000088u+f->r[4]));
    /* 00446c00 */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
mi_00446c04:;
    /* 00446c04 */
    a=MI_U32((0x00000080u+f->r[4]));b=0x0000000cu;t=mi_alu(f,a,b,32,1);
    /* 00446c0c */
    if(!(f->flags&MI_ZF))goto mi_00446c1f;
    /* 00446c0e */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00446c0f */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00446c10 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00446c11 */
    a=0x00000064u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00446c13 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00446c15 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00446c1au;f->boundary(0x00446c15u,0x004b4800u,f);
    /* 00446c1a */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00446c1d */
    goto mi_00446c4d;
mi_00446c1f:;
    /* 00446c1f */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00446c23 */
    f->r[0]=(0x00000024u+f->r[4]);
    /* 00446c27 */
    f->r[1]=(0x00000014u+f->r[4]);
    /* 00446c2b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00446c2c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00446c2d */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[3]);
    /* 00446c31 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00446c36u;f->boundary(0x00446c31u,0x004467a0u,f);
    /* 00446c36 */
    f->r[2]=MI_U32((0x0000002cu+f->r[4]));
    /* 00446c3a */
    f->r[0]=MI_U32((0x0000001cu+f->r[4]));
    /* 00446c3e */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00446c3f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00446c40 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00446c41 */
    a=0x0000005du;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00446c43 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00446c45 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00446c4au;f->boundary(0x00446c45u,0x004b4800u,f);
    /* 00446c4a */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00446c4d:;
    /* 00446c4d */
    f->r[7]=f->r[0];
    /* 00446c4f */
    f->r[1]=(0x00000000u+f->r[5]+f->r[5]*2u);
    /* 00446c53 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00446c54 */
    f->r[6]=(0x00000000u+f->r[7]+f->r[7]*2u);
    /* 00446c57 */
    f->r[2]=(0x0000000au+f->r[5]+f->r[1]*8u);
    /* 00446c5b */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 00446c5e */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00446c60 */
    a=f->r[6];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 00446c63 */
    MI_U8((0x00bcd605u+f->r[6]))=(uint8_t)(0x00000000u);
    /* 00446c6a */
    MI_U8((0x00bcd630u+f->r[6]))=(uint8_t)(0x00000008u);
    /* 00446c71 */
    MI_U16((0x00bcd62eu+f->r[6]))=(uint16_t)((f->r[2]&0xffffu));
    /* 00446c78 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00446c7du;f->boundary(0x00446c78u,0x004b5af0u,f);
    /* 00446c7d */
    f->r[1]=MI_U32((0x00000020u+f->r[4]));
    /* 00446c81 */
    MI_U8((0x00bcd65du+f->r[6]))=(uint8_t)(0x00000001u);
    /* 00446c88 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00446c89 */
    MI_U8((0x00bcd66eu+f->r[6]))=(uint8_t)(0x00000001u);
    /* 00446c90 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00446c91 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00446c95 */
    MI_U32((0x00bcd778u+f->r[6]))=(uint32_t)(0x00000000u);
    /* 00446c9f */
    f->r[4]-=4;MI_U32(f->r[4])=0x00446ca4u;f->boundary(0x00446c9fu,0x00446650u,f);
    /* 00446ca4 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x0078d7c0u)))&0xffu)<<0);
    /* 00446ca9 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00446cac */
    MI_U8((0x00bcd678u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 00446cb2 */
    f->r[0]=MI_U32((0x0000002cu+f->r[4]));
    /* 00446cb6 */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 00446cb7 */
    a=f->r[5];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00446cb9 */
    if(mi_signed_less(f->flags))goto mi_00446bf9;
    /* 00446cbf */
    f->r[7]=MI_U32((0x00000020u+f->r[4]));
mi_00446cc3:;
    /* 00446cc3 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 00446cc5 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 00446cc7 */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 00446cc9 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00446d8f;
    /* 00446ccf */
    f->r[0]=MI_U32((0x00d3b1c4u));
    /* 00446cd4 */
    f->r[1]=MI_U32((0x00d3b374u));
    /* 00446cda */
    MI_U32((0x00d3b1c8u+f->r[0]*4u))=(uint32_t)(f->r[1]);
    /* 00446ce1 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00446ce2 */
    a=f->r[0];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 00446ce5 */
    MI_U32((0x00d3b1c4u))=(uint32_t)(f->r[0]);
    /* 00446cea */
    if(mi_signed_less(f->flags))goto mi_00446cf2;
    /* 00446cec */
    MI_U32((0x00d3b1c4u))=(uint32_t)(f->r[2]);
mi_00446cf2:;
    /* 00446cf2 */
    f->r[2]=MI_U32((0x00d3b384u));
    /* 00446cf8 */
    f->r[1]=MI_U32((0x00d3b380u));
    /* 00446cfe */
    f->r[7]=0x0000001fu;
mi_00446d03:;
    /* 00446d03 */
    f->r[0]=f->r[2];
    /* 00446d05 */
    f->r[6]=f->r[2];
    /* 00446d07 */
    a=f->r[0];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 00446d0a */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 00446d0d */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 00446d10 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00446d12 */
    f->r[6]=f->r[1];
    /* 00446d14 */
    a=f->r[6];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[6]=t;
    /* 00446d17 */
    f->r[3]=f->r[1];
    /* 00446d19 */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 00446d1c */
    a=f->r[3];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[3]=t;
    /* 00446d1f */
    a=f->r[6];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 00446d21 */
    a=f->r[2];t=a>>1u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|((a>>0u)&1u)|mi_szp(t,32)|((a&0x80000000u)?MI_OF:0);f->r[2]=t;
    /* 00446d23 */
    a=f->r[1];t=a>>1u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|((a>>0u)&1u)|mi_szp(t,32)|((a&0x80000000u)?MI_OF:0);f->r[1]=t;
    /* 00446d25 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00446d27 */
    if((f->flags&MI_ZF))goto mi_00446d2f;
    /* 00446d29 */
    a=f->r[2];b=0x40000000u;t=mi_alu(f,a,b,32,4);f->r[2]=t;
mi_00446d2f:;
    /* 00446d2f */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 00446d31 */
    if((f->flags&MI_ZF))goto mi_00446d39;
    /* 00446d33 */
    a=f->r[1];b=0x40000000u;t=mi_alu(f,a,b,32,4);f->r[1]=t;
mi_00446d39:;
    /* 00446d39 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 00446d3a */
    if(!(f->flags&MI_ZF))goto mi_00446d03;
    /* 00446d3c */
    MI_U32((0x00d3b384u))=(uint32_t)(f->r[2]);
    /* 00446d42 */
    f->r[0]=f->r[2];
    /* 00446d44 */
    f->r[6]=f->r[2];
    /* 00446d46 */
    a=f->r[2];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 00446d49 */
    MI_U32((0x00d3b378u))=(uint32_t)(f->r[2]);
    /* 00446d4f */
    f->r[2]=f->r[1];
    /* 00446d51 */
    a=f->r[2];b=0x00007fffu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 00446d57 */
    a=f->r[0];b=0x00007fffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 00446d5c */
    MI_U32((0x00d3b364u))=(uint32_t)(f->r[2]);
    /* 00446d62 */
    f->r[2]=f->r[1];
    /* 00446d64 */
    a=f->r[2];b=0x0000007fu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 00446d67 */
    MI_U32((0x00d3b370u))=(uint32_t)(f->r[0]);
    /* 00446d6c */
    MI_U32((0x00d3b368u))=(uint32_t)(f->r[2]);
    /* 00446d72 */
    MI_U32((0x00d3b380u))=(uint32_t)(f->r[1]);
    /* 00446d78 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 00446d79 */
    a=f->r[6];b=0x0000007fu;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 00446d7c */
    a=f->r[1];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 00446d7f */
    a=MI_U32((0x00000020u+f->r[4]));mi_idiv32(f,a);
    /* 00446d83 */
    MI_U32((0x00d3b374u))=(uint32_t)(f->r[6]);
    /* 00446d89 */
    MI_U32((0x00d3b36cu))=(uint32_t)(f->r[1]);
mi_00446d8f:;
    /* 00446d8f */
    f->r[0]=MI_U32((0x00000084u+f->r[4]));
    /* 00446d96 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00446d98 */
    if((f->flags&MI_ZF))goto mi_0044741d;
    /* 00446d9e */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00000034u+f->r[4]+f->r[2]*4u)))&0xffffu)<<0);
    /* 00446da3 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x00000040u+f->r[4]+f->r[2]*4u)))&0xffffu)<<0);
    /* 00446da8 */
    MI_U16((0x0000002cu+f->r[4]))=(uint16_t)((f->r[0]&0xffffu));
    /* 00446dad */
    MI_U16((0x00000024u+f->r[4]))=(uint16_t)((f->r[1]&0xffffu));
mi_00446db2:;
    /* 00446db2 */
    f->r[0]=MI_U32((0x00000084u+f->r[4]));
    /* 00446db9 */
    a=f->r[0];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 00446dbc */
    if(mi_signed_less(f->flags))goto mi_00446dce;
    /* 00446dbe */
    f->r[3]=0x00000010u;
    /* 00446dc3 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00446dc5 */
    MI_U32((0x00000084u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00446dcc */
    goto mi_00446de0;
mi_00446dce:;
    /* 00446dce */
    f->r[3]=MI_U32((0x00000084u+f->r[4]));
    /* 00446dd5 */
    MI_U32((0x00000084u+f->r[4]))=(uint32_t)(0x00000000u);
mi_00446de0:;
    /* 00446de0 */
    f->r[2]=MI_U32((0x00000010u+f->r[4]));
    /* 00446de4 */
    f->r[0]=MI_U32((0x00000018u+f->r[4]));
    /* 00446de8 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00446dec */
    f->r[1]=(0x00000020u+f->r[4]);
    /* 00446df0 */
    f->r[2]=(0x0000001cu+f->r[4]);
    /* 00446df4 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00446df5 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00446df6 */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00446dfa */
    f->r[4]-=4;MI_U32(f->r[4])=0x00446dffu;f->boundary(0x00446dfau,0x004467a0u,f);
    /* 00446dff */
    f->r[0]=MI_U32((0x00000088u+f->r[4]));
    /* 00446e06 */
    f->r[1]=MI_U32((0x00000028u+f->r[4]));
    /* 00446e0a */
    f->r[2]=MI_U32((0x00000024u+f->r[4]));
    /* 00446e0e */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00446e11 */
    a=f->r[0];b=0x0000000cu;t=mi_alu(f,a,b,32,1);
    /* 00446e14 */
    f->r[0]=MI_U32((0x00000088u+f->r[4]));
    /* 00446e1b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00446e1c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00446e1d */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00446e1e */
    if(!(f->flags&MI_ZF))goto mi_00446e24;
    /* 00446e20 */
    a=0x00000065u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00446e22 */
    goto mi_00446e26;
mi_00446e24:;
    /* 00446e24 */
    a=0x0000005cu;f->r[4]-=4;MI_U32(f->r[4])=a;
mi_00446e26:;
    /* 00446e26 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00446e28 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00446e2du;f->boundary(0x00446e28u,0x004b4800u,f);
    /* 00446e2d */
    f->r[7]=f->r[0];
    /* 00446e2f */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00000040u+f->r[4])))&0xffffu)<<0);
    /* 00446e34 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x00000038u+f->r[4])))&0xffffu)<<0);
    /* 00446e39 */
    f->r[2]=(0x00000000u+f->r[5]+f->r[5]*4u);
    /* 00446e3d */
    f->r[6]=(0x00000000u+f->r[7]+f->r[7]*2u);
    /* 00446e40 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00446e43 */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 00446e46 */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00446e48 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00446e49 */
    a=f->r[6];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 00446e4c */
    MI_U16((0x00bcd628u+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 00446e53 */
    MI_U16((0x00bcd62au+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
    /* 00446e5a */
    MI_U8((0x00bcd605u+f->r[6]))=(uint8_t)(0x00000000u);
    /* 00446e61 */
    f->r[0]=(0x0000000au+f->r[2]*8u);
    /* 00446e68 */
    MI_U8((0x00bcd630u+f->r[6]))=(uint8_t)(0x00000008u);
    /* 00446e6f */
    MI_U16((0x00bcd62eu+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 00446e76 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00446e7bu;f->boundary(0x00446e76u,0x004b5af0u,f);
    /* 00446e7b */
    MI_U8((0x00bcd65du+f->r[6]))=(uint8_t)(0x00000001u);
    /* 00446e82 */
    MI_U8((0x00bcd66eu+f->r[6]))=(uint8_t)(0x00000001u);
    /* 00446e89 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00446e8a */
    MI_U32((0x00bcd778u+f->r[6]))=(uint32_t)(0x00000000u);
    /* 00446e94 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00446e99u;f->boundary(0x00446e94u,0x004620c0u,f);
    /* 00446e99 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00775900u));
    /* 00446ea0 */
    f->r[2]=MI_U32((0x00000088u+f->r[4]));
    /* 00446ea7 */
    f->r[0]=MI_U32((0x00000094u+f->r[4]));
    /* 00446eae */
    MI_U8((0x00bcd728u+f->r[6]))=(uint8_t)(0x00000001u);
    /* 00446eb5 */
    MI_U32((0x00bcd764u+f->r[6]))=(uint32_t)(f->r[3]);
    /* 00446ebb */
    MI_U32((0x00bcd768u+f->r[6]))=(uint32_t)(f->r[1]);
    /* 00446ec1 */
    f->r[1]=MI_U32((0x00000098u+f->r[4]));
    /* 00446ec8 */
    MI_U32((0x00bcd76cu+f->r[6]))=(uint32_t)(f->r[2]);
    /* 00446ece */
    MI_U32((0x00bcd770u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 00446ed4 */
    f->r[0]=MI_U32((0x0000008cu+f->r[4]));
    /* 00446edb */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00446ede */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 00446edf */
    MI_U32((0x00bcd774u+f->r[6]))=(uint32_t)(f->r[1]);
    /* 00446ee5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00446ee7 */
    if(!(f->flags&MI_ZF))goto mi_00446db2;
    /* 00446eed */
    f->r[0]=MI_U32((0x00000028u+f->r[4]));
    /* 00446ef1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00446ef2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00446ef3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00446ef4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00446ef5 */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00446ef8 */
    f->r[4]+=4;return;
mi_00446ef9:;
    /* 00446ef9 */
    f->r[2]=MI_U32((0x00e39080u));
    /* 00446eff */
    f->r[1]=MI_U32((0x00000084u+f->r[4]));
    /* 00446f06 */
    f->r[0]=MI_U32((0x005d19b0u+f->r[2]*4u));
    /* 00446f0d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00446f0e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00446f0f */
    f->r[4]-=4;MI_U32(f->r[4])=0x00446f14u;f->boundary(0x00446f0fu,0x004cde40u,f);
    /* 00446f14 */
    f->r[6]=f->r[0];
    /* 00446f16 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00446f19 */
    a=f->r[6];b=0x00000096u;t=mi_alu(f,a,b,32,1);
    /* 00446f1f */
    if(mi_signed_less(f->flags))goto mi_00446f26;
    /* 00446f21 */
    f->r[6]=0x00000096u;
mi_00446f26:;
    /* 00446f26 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00775900u)))&0xffffu)<<0);
    /* 00446f2c */
    a=(f->r[0]&0xffffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,16,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=(f->r[0]&0xffff0000u)|(((t)&0xffffu)<<0);
    /* 00446f2e */
    a=(f->r[0]&0xffffu);b=0x00007d00u;t=mi_alu(f,a,b,16,1);
    /* 00446f32 */
    MI_U16((0x00775900u))=(uint16_t)((f->r[0]&0xffffu));
    /* 00446f38 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00446f43;
    /* 00446f3a */
    MI_U16((0x00775900u))=(uint16_t)(0x00000001u);
mi_00446f43:;
    /* 00446f43 */
    f->r[0]=MI_U32((0x00000080u+f->r[4]));
    /* 00446f4a */
    a=f->r[0];b=0x0000000cu;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00446f4d */
    if((f->flags&MI_ZF))goto mi_00446f70;
    /* 00446f4f */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00446f50 */
    if((f->flags&MI_ZF))goto mi_00446f65;
    /* 00446f52 */
    f->r[0]=MI_U32((0x00709a90u));
    /* 00446f57 */
    f->r[7]=MI_U32((0x00709a58u));
    /* 00446f5d */
    f->r[3]=MI_U32((0x00709ac8u));
    /* 00446f63 */
    goto mi_00446f81;
mi_00446f65:;
    /* 00446f65 */
    f->r[0]=MI_U32((0x0070a698u));
    /* 00446f6a */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 00446f6c */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 00446f6e */
    goto mi_00446f81;
mi_00446f70:;
    /* 00446f70 */
    f->r[0]=MI_U32((0x00709cc0u));
    /* 00446f75 */
    f->r[7]=MI_U32((0x00709cf8u));
    /* 00446f7b */
    f->r[3]=MI_U32((0x00709d30u));
mi_00446f81:;
    /* 00446f81 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00446f82 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00446f83 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00446f88u;f->boundary(0x00446f83u,0x004cde40u,f);
    /* 00446f88 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00446f89 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00446f8a */
    f->r[4]-=4;MI_U32(f->r[4])=0x00446f8fu;f->boundary(0x00446f8au,0x004cde40u,f);
    /* 00446f8f */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00446f90 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00446f91 */
    f->r[5]=f->r[0];
    /* 00446f93 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00446f98u;f->boundary(0x00446f93u,0x004cde40u,f);
    /* 00446f98 */
    f->r[3]=f->r[0];
    /* 00446f9a */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 00446f9c */
    a=f->r[6];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 00446f9e */
    MI_U32((0x0000004cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00446fa2 */
    a=f->r[6];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 00446fa4 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00446fa7 */
    f->r[1]=0x0000000cu;
    /* 00446fac */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00446fae */
    f->r[7]=(0x0000004cu+f->r[4]);
    /* 00446fb2 */
    MI_U32((0x00000038u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00446fb6 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 00446fb8 */
    while(f->r[1]){MI_U32(f->r[7])=f->r[0];f->r[7]+=(f->flags&0x400u)?0xfffffffcu:4u;--f->r[1];}
    /* 00446fba */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00446fbe */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00447014;
    /* 00446fc0 */
    a=f->r[6];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 00446fc3 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00446fcf;
    /* 00446fc5 */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(0x00000001u);
    /* 00446fcd */
    goto mi_00447010;
mi_00446fcf:;
    /* 00446fcf */
    a=f->r[6];b=0x00000020u;t=mi_alu(f,a,b,32,1);
    /* 00446fd2 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00446feb;
    /* 00446fd4 */
    f->r[0]=f->r[6];
    /* 00446fd6 */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(0x00000002u);
    /* 00446fde */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 00446fdf */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00446fe1 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 00446fe3 */
    MI_U32((0x00000050u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00446fe7 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 00446fe9 */
    goto mi_00447010;
mi_00446feb:;
    /* 00446feb */
    f->r[0]=0x55555556u;
    /* 00446ff0 */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(0x00000003u);
    /* 00446ff8 */
    a=f->r[0];b=f->r[6];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 00446ffa */
    f->r[0]=f->r[2];
    /* 00446ffc */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 00446fff */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00447001 */
    f->r[0]=f->r[2];
    /* 00447003 */
    f->r[1]=(0x00000000u+f->r[2]+f->r[2]*1u);
    /* 00447006 */
    MI_U32((0x00000054u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0044700a */
    MI_U32((0x00000050u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0044700e */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[6]=t;
mi_00447010:;
    /* 00447010 */
    MI_U32((0x0000004cu+f->r[4]))=(uint32_t)(f->r[6]);
mi_00447014:;
    /* 00447014 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 00447016 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0044706c;
    /* 00447018 */
    a=f->r[5];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 0044701b */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00447027;
    /* 0044701d */
    MI_U32((0x00000038u+f->r[4]))=(uint32_t)(0x00000001u);
    /* 00447025 */
    goto mi_00447068;
mi_00447027:;
    /* 00447027 */
    a=f->r[5];b=0x00000020u;t=mi_alu(f,a,b,32,1);
    /* 0044702a */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00447043;
    /* 0044702c */
    f->r[0]=f->r[5];
    /* 0044702e */
    MI_U32((0x00000038u+f->r[4]))=(uint32_t)(0x00000002u);
    /* 00447036 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 00447037 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00447039 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 0044703b */
    MI_U32((0x00000060u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0044703f */
    a=f->r[5];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[5]=t;
    /* 00447041 */
    goto mi_00447068;
mi_00447043:;
    /* 00447043 */
    f->r[0]=0x55555556u;
    /* 00447048 */
    MI_U32((0x00000038u+f->r[4]))=(uint32_t)(0x00000003u);
    /* 00447050 */
    a=f->r[0];b=f->r[5];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 00447052 */
    f->r[0]=f->r[2];
    /* 00447054 */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 00447057 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00447059 */
    f->r[0]=f->r[2];
    /* 0044705b */
    f->r[1]=(0x00000000u+f->r[2]+f->r[2]*1u);
    /* 0044705e */
    MI_U32((0x00000064u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00447062 */
    MI_U32((0x00000060u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00447066 */
    a=f->r[5];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[5]=t;
mi_00447068:;
    /* 00447068 */
    MI_U32((0x0000005cu+f->r[4]))=(uint32_t)(f->r[5]);
mi_0044706c:;
    /* 0044706c */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 0044706e */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004470c4;
    /* 00447070 */
    a=f->r[3];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 00447073 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0044707f;
    /* 00447075 */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(0x00000001u);
    /* 0044707d */
    goto mi_004470c0;
mi_0044707f:;
    /* 0044707f */
    a=f->r[3];b=0x00000020u;t=mi_alu(f,a,b,32,1);
    /* 00447082 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0044709b;
    /* 00447084 */
    f->r[0]=f->r[3];
    /* 00447086 */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(0x00000002u);
    /* 0044708e */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0044708f */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00447091 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 00447093 */
    MI_U32((0x00000070u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00447097 */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 00447099 */
    goto mi_004470c0;
mi_0044709b:;
    /* 0044709b */
    f->r[0]=0x55555556u;
    /* 004470a0 */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(0x00000003u);
    /* 004470a8 */
    a=f->r[0];b=f->r[3];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 004470aa */
    f->r[0]=f->r[2];
    /* 004470ac */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 004470af */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004470b1 */
    f->r[0]=f->r[2];
    /* 004470b3 */
    f->r[1]=(0x00000000u+f->r[2]+f->r[2]*1u);
    /* 004470b6 */
    MI_U32((0x00000074u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004470ba */
    MI_U32((0x00000070u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004470be */
    a=f->r[3];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[3]=t;
mi_004470c0:;
    /* 004470c0 */
    MI_U32((0x0000006cu+f->r[4]))=(uint32_t)(f->r[3]);
mi_004470c4:;
    /* 004470c4 */
    f->r[1]=MI_U32((0x00000080u+f->r[4]));
    /* 004470cb */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[0]=t;
    /* 004470ce */
    a=f->r[1];b=0x0000000bu;t=mi_alu(f,a,b,32,1);
    /* 004470d1 */
    if(!(f->flags&MI_ZF))goto mi_004470e7;
    /* 004470d3 */
    f->r[7]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00784c64u));
    /* 004470da */
    f->r[6]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00784c66u));
    /* 004470e1 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 004470e5 */
    goto mi_0044715f;
mi_004470e7:;
    /* 004470e7 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004470e9 */
    f->r[1]=0x00784bc0u;
mi_004470ee:;
    /* 004470ee */
    a=MI_U16((0x00000000u+f->r[1]));b=(f->r[0]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 004470f1 */
    if((f->flags&MI_ZF))goto mi_004470f4;
    /* 004470f3 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
mi_004470f4:;
    /* 004470f4 */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004470f7 */
    a=f->r[1];b=0x00784bd0u;t=mi_alu(f,a,b,32,1);
    /* 004470fd */
    if(mi_signed_less(f->flags))goto mi_004470ee;
    /* 004470ff */
    f->r[1]=MI_U32((0x00000088u+f->r[4]));
    /* 00447106 */
    a=f->r[1];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 00447109 */
    if(!(f->flags&MI_ZF))goto mi_00447130;
    /* 0044710b */
    a=f->r[2];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0044710e */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0044711b;
    /* 00447110 */
    f->r[1]=MI_U32((0x00d3b374u));
    /* 00447116 */
    a=f->r[1];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 00447119 */
    goto mi_00447131;
mi_0044711b:;
    /* 0044711b */
    f->r[1]=MI_U32((0x00d3b374u));
    /* 00447121 */
    a=f->r[2];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 00447124 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0044712b;
    /* 00447126 */
    a=f->r[1];b=0x00000003u;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 00447129 */
    goto mi_00447131;
mi_0044712b:;
    /* 0044712b */
    a=f->r[1];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 0044712e */
    goto mi_00447131;
mi_00447130:;
    /* 00447130 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
mi_00447131:;
    /* 00447131 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
mi_00447133:;
    /* 00447133 */
    a=MI_U16((0x00784bc0u+f->r[1]*2u));b=(f->r[0]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 0044713b */
    if(!(f->flags&MI_ZF))goto mi_0044714b;
    /* 0044713d */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0044713e */
    a=f->r[1];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 00447141 */
    if(mi_signed_less(f->flags))goto mi_00447145;
    /* 00447143 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_00447145:;
    /* 00447145 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00447146 */
    a=f->r[2];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 00447149 */
    if(mi_signed_less(f->flags))goto mi_00447133;
mi_0044714b:;
    /* 0044714b */
    f->r[7]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00784bc0u+f->r[1]*2u));
    /* 00447153 */
    f->r[6]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00784be0u+f->r[1]*2u));
    /* 0044715b */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[7]);
mi_0044715f:;
    /* 0044715f */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00447161 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 00447165 */
    if((f->flags&MI_ZF))goto mi_0044716b;
    /* 00447167 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00447169 */
    if(!(f->flags&MI_ZF))goto mi_00447181;
mi_0044716b:;
    /* 0044716b */
    f->r[7]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00784c68u));
    /* 00447172 */
    f->r[6]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00784c6au));
    /* 00447179 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 0044717d */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[6]);
mi_00447181:;
    /* 00447181 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 00447183 */
    if(!(f->flags&MI_ZF))goto mi_00447193;
    /* 00447185 */
    f->r[3]=0x00000004u;
    /* 0044718a */
    MI_U32((0x00000088u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 00447191 */
    goto mi_004471e1;
mi_00447193:;
    /* 00447193 */
    f->r[2]=MI_U32((0x00784908u));
    /* 00447199 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0044719a */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0044719c */
    if(mi_signed_less(f->flags))goto mi_004471ab;
    /* 0044719e */
    MI_U32((0x00000088u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 004471a9 */
    goto mi_004471da;
mi_004471ab:;
    /* 004471ab */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 004471ad */
    if(!(f->flags&MI_ZF))goto mi_004471bc;
    /* 004471af */
    MI_U32((0x00000088u+f->r[4]))=(uint32_t)(0x00000002u);
    /* 004471ba */
    goto mi_004471da;
mi_004471bc:;
    /* 004471bc */
    f->r[1]=MI_U32((0x00784904u));
    /* 004471c2 */
    f->r[2]=(0xffffffffu+f->r[1]);
    /* 004471c5 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004471c7 */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004471c9 */
    t=(mi_signed_less(f->flags))?1u:0u;f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004471cc */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004471cd */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 004471d0 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004471d3 */
    MI_U32((0x00000088u+f->r[4]))=(uint32_t)(f->r[1]);
mi_004471da:;
    /* 004471da */
    f->r[3]=MI_U32((0x00000088u+f->r[4]));
mi_004471e1:;
    /* 004471e1 */
    f->r[1]=(0x00000000u+f->r[6]+f->r[6]*8u);
    /* 004471e4 */
    f->r[1]=(0x00000000u+f->r[6]+f->r[1]*2u);
    /* 004471e7 */
    f->r[2]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 004471ea */
    f->r[1]=MI_U32((0x00bca2d8u));
    /* 004471f0 */
    f->r[2]=(0x00000000u+f->r[1]+f->r[2]*4u);
    /* 004471f3 */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004471f5 */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004471f9 */
    f->r[1]=MI_U32((0x00af598cu+f->r[2]*4u));
    /* 00447200 */
    a=f->r[1];b=0x00000203u;t=mi_alu(f,a,b,32,3);
    /* 00447206 */
    if(!(f->flags&MI_ZF))goto mi_00447421;
    /* 0044720c */
    a=(f->r[1]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,3);
    /* 0044720f */
    if((f->flags&MI_ZF))goto mi_0044721e;
    /* 00447211 */
    a=(f->r[1]&0xffu);b=0x00000040u;t=mi_alu(f,a,b,8,3);
    /* 00447214 */
    if(!(f->flags&MI_ZF))goto mi_00447231;
    /* 00447216 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00447217 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00447218 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00447219 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0044721a */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044721d */
    f->r[4]+=4;return;
mi_0044721e:;
    /* 0044721e */
    a=f->r[1];b=0x0000c108u;t=mi_alu(f,a,b,32,3);
    /* 00447224 */
    if((f->flags&MI_ZF))goto mi_00447231;
    /* 00447226 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00447227 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00447228 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00447229 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044722eu;f->boundary(0x00447229u,0x00469140u,f);
    /* 0044722e */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00447231:;
    /* 00447231 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00447233 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00447237 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[0]);
mi_0044723b:;
    /* 0044723b */
    f->r[2]=MI_U32((0x00000020u+f->r[4]));
    /* 0044723f */
    f->r[1]=MI_U32((0x00000034u+f->r[4]+f->r[2]*4u));
    /* 00447243 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00447245 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0044740b;
    /* 0044724b */
    f->r[0]=MI_U32((0x00000080u+f->r[4]));
    /* 00447252 */
    a=f->r[0];b=0x0000000cu;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00447255 */
    if((f->flags&MI_ZF))goto mi_0044729e;
    /* 00447257 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00447258 */
    if((f->flags&MI_ZF))goto mi_00447289;
    /* 0044725a */
    f->r[0]=f->r[2];
    /* 0044725c */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0044725d */
    if((f->flags&MI_ZF))goto mi_0044727c;
    /* 0044725f */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00447260 */
    if((f->flags&MI_ZF))goto mi_0044726f;
    /* 00447262 */
    MI_U32((0x00000084u+f->r[4]))=(uint32_t)(0x0000002cu);
    /* 0044726d */
    goto mi_004472cd;
mi_0044726f:;
    /* 0044726f */
    MI_U32((0x00000084u+f->r[4]))=(uint32_t)(0x0000002du);
    /* 0044727a */
    goto mi_004472cd;
mi_0044727c:;
    /* 0044727c */
    MI_U32((0x00000084u+f->r[4]))=(uint32_t)(0x0000002bu);
    /* 00447287 */
    goto mi_004472cd;
mi_00447289:;
    /* 00447289 */
    MI_U32((0x00000084u+f->r[4]))=(uint32_t)(0x00000063u);
    /* 00447294 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(0x00000008u);
    /* 0044729c */
    goto mi_004472d5;
mi_0044729e:;
    /* 0044729e */
    f->r[0]=MI_U32((0x00000020u+f->r[4]));
    /* 004472a2 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004472a3 */
    if((f->flags&MI_ZF))goto mi_004472c2;
    /* 004472a5 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004472a6 */
    if((f->flags&MI_ZF))goto mi_004472b5;
    /* 004472a8 */
    MI_U32((0x00000084u+f->r[4]))=(uint32_t)(0x00000036u);
    /* 004472b3 */
    goto mi_004472cd;
mi_004472b5:;
    /* 004472b5 */
    MI_U32((0x00000084u+f->r[4]))=(uint32_t)(0x00000038u);
    /* 004472c0 */
    goto mi_004472cd;
mi_004472c2:;
    /* 004472c2 */
    MI_U32((0x00000084u+f->r[4]))=(uint32_t)(0x00000037u);
mi_004472cd:;
    /* 004472cd */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(0x0000000au);
mi_004472d5:;
    /* 004472d5 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004472d7 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0044740b;
    /* 004472dd */
    f->r[0]=MI_U32((0x00000020u+f->r[4]));
    /* 004472e1 */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004472e5 */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004472e8 */
    f->r[2]=(0x0000004cu+f->r[4]+f->r[0]*1u);
    /* 004472ec */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[2]);
mi_004472f0:;
    /* 004472f0 */
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
    /* 004472f4 */
    f->r[1]=MI_U32((0x00000084u+f->r[4]));
    /* 004472fb */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004472fc */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004472fd */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004472fe */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004472ff */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00447300 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00447305u;f->boundary(0x00447300u,0x004b6ff0u,f);
    /* 00447305 */
    f->r[5]=f->r[0];
    /* 00447307 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044730a */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 0044730c */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004473f1;
    /* 00447312 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x0000008cu+f->r[4])))&0xffffu)<<0);
    /* 0044731a */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00000080u+f->r[4])))&0xffu)<<0);
    /* 00447321 */
    f->r[0]=(0x00000000u+f->r[5]+f->r[5]*8u);
    /* 00447325 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 00447327 */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0044732a */
    MI_U16((0x00bcba0cu+f->r[0]))=(uint16_t)((f->r[2]&0xffffu));
    /* 00447331 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x00000090u+f->r[4])))&0xffffu)<<0);
    /* 00447339 */
    MI_U8((0x00bcba39u+f->r[0]))=(uint8_t)((f->r[3]&0xffu));
    /* 0044733f */
    MI_U8((0x00bcba32u+f->r[0]))=(uint8_t)((f->r[1]&0xffu));
    /* 00447345 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x00775900u)))&0xffffu)<<0);
    /* 0044734c */
    MI_U16((0x00bcba40u+f->r[0]))=(uint16_t)((f->r[2]&0xffffu));
    /* 00447353 */
    f->r[2]=MI_U32((0x00000024u+f->r[4]));
    /* 00447357 */
    MI_U16((0x00bcba50u+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 0044735e */
    f->r[3]=MI_U32((0x00000000u+f->r[2]));
    /* 00447360 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 00447362 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004473d9;
    /* 00447364 */
    f->r[0]=MI_U32((0x0000001cu+f->r[4]));
    /* 00447368 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 0044736b */
    MI_U32((0x00000030u+f->r[4]))=(uint32_t)(f->r[0]);
mi_0044736f:;
    /* 0044736f */
    f->r[1]=MI_U32((0x00000088u+f->r[4]));
    /* 00447376 */
    f->r[2]=MI_U32((0x00000018u+f->r[4]));
    /* 0044737a */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 0044737e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044737f */
    f->r[1]=MI_U32((0x00000088u+f->r[4]));
    /* 00447386 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00447387 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00447388 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00447389 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044738b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00447390u;f->boundary(0x0044738bu,0x004b4800u,f);
    /* 00447390 */
    f->r[2]=MI_U32((0x00000044u+f->r[4]));
    /* 00447394 */
    f->r[6]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 00447397 */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0044739a */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0044739c */
    f->r[1]=(0x00000001u+f->r[7]+f->r[2]*4u);
    /* 004473a0 */
    a=f->r[6];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004473a3 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*4u);
    /* 004473a6 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004473a7 */
    MI_U8((0x00bcd605u+f->r[6]))=(uint8_t)(0x00000000u);
    /* 004473ae */
    MI_U8((0x00bcd630u+f->r[6]))=(uint8_t)(0x00000009u);
    /* 004473b5 */
    a=f->r[1];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004473b7 */
    MI_U16((0x00bcd62eu+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004473be */
    MI_U16((0x00bcd658u+f->r[6]))=(uint16_t)((f->r[5]&0xffffu));
    /* 004473c5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004473cau;f->boundary(0x004473c5u,0x004b5af0u,f);
    /* 004473ca */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004473cd */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 004473ce */
    a=f->r[7];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004473d0 */
    MI_U8((0x00bcd65du+f->r[6]))=(uint8_t)(0x00000001u);
    /* 004473d7 */
    if(mi_signed_less(f->flags))goto mi_0044736f;
mi_004473d9:;
    /* 004473d9 */
    f->r[0]=MI_U32((0x0000001cu+f->r[4]));
    /* 004473dd */
    f->r[3]=MI_U32((0x00000088u+f->r[4]));
    /* 004473e4 */
    f->r[6]=MI_U32((0x00000018u+f->r[4]));
    /* 004473e8 */
    f->r[7]=MI_U32((0x00000010u+f->r[4]));
    /* 004473ec */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004473ed */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[0]);
mi_004473f1:;
    /* 004473f1 */
    f->r[1]=MI_U32((0x00000024u+f->r[4]));
    /* 004473f5 */
    f->r[0]=MI_U32((0x0000002cu+f->r[4]));
    /* 004473f9 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004473fc */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004473fd */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00447401 */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00447405 */
    if(!(f->flags&MI_ZF))goto mi_004472f0;
mi_0044740b:;
    /* 0044740b */
    f->r[0]=MI_U32((0x00000020u+f->r[4]));
    /* 0044740f */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00447410 */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 00447413 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00447417 */
    if(mi_signed_less(f->flags))goto mi_0044723b;
mi_0044741d:;
    /* 0044741d */
    f->r[0]=MI_U32((0x00000028u+f->r[4]));
mi_00447421:;
    /* 00447421 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00447422 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00447423 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00447424 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00447425 */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00447428 */
    f->r[4]+=4;return;
}

void restore_invasion_C_004467a0(struct restore_invasion_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004467a0 */
    f->r[0]=MI_U32((0x00000008u+f->r[4]));
    /* 004467a4 */
    f->r[1]=MI_U32((0x00000004u+f->r[4]));
    /* 004467a8 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 004467ab */
    f->r[0]=MI_U32((0x00000000u+f->r[0]));
    /* 004467ad */
    f->r[1]=MI_U32((0x00000000u+f->r[1]));
    /* 004467af */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004467b0 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004467b1 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 004467b4 */
    f->r[5]=MI_U32((0x00bca2d8u));
    /* 004467ba */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004467bb */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004467bc */
    f->r[2]=(0x00000000u+f->r[0]+f->r[2]*2u);
    /* 004467bf */
    f->r[2]=(0x00000000u+f->r[2]+f->r[2]*2u);
    /* 004467c2 */
    f->r[2]=(0x00000000u+f->r[1]+f->r[2]*4u);
    /* 004467c5 */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004467c7 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((MI_U8((0x00ed4140u+f->r[2])))&0xffu)<<0);
    /* 004467cd */
    a=(f->r[3]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004467cf */
    if(mi_signed_less(f->flags))goto mi_004467e6;
    /* 004467d1 */
    a=MI_U16((0x00b350dcu+f->r[2]*2u));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 004467da */
    if(!(f->flags&MI_ZF))goto mi_004467e6;
    /* 004467dc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004467dd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004467de */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004467df */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 004467e1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004467e2 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004467e5 */
    f->r[4]+=4;return;
mi_004467e6:;
    /* 004467e6 */
    f->r[6]=MI_U32((0x00bca2ecu));
    /* 004467ec */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(0x00000001u);
    /* 004467f4 */
    f->r[2]=(0x00000001u+f->r[0]+f->r[6]*1u);
    /* 004467f8 */
    f->r[0]=(0xffffffffu+f->r[0]+f->r[6]*1u);
    /* 004467fc */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00446800 */
    f->r[2]=MI_U32((0x00bca2e8u));
    /* 00446806 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0044680a */
    f->r[3]=(0x00000001u+f->r[1]+f->r[2]*1u);
    /* 0044680e */
    f->r[1]=(0xffffffffu+f->r[1]+f->r[2]*1u);
    /* 00446812 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 00446816 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[1]);
mi_0044681a:;
    /* 0044681a */
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
    /* 0044681e */
    f->r[7]=MI_U32((0x00000018u+f->r[4]));
    /* 00446822 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 00446824 */
    f->r[6]=f->r[1];
    /* 00446826 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[3]);
    /* 0044682a */
    if(!mi_signed_less(f->flags))goto mi_00446835;
    /* 0044682c */
    MI_U8((0x00000012u+f->r[4]))=(uint8_t)(0x00000001u);
    /* 00446831 */
    f->r[6]=f->r[2];
    /* 00446833 */
    goto mi_0044683a;
mi_00446835:;
    /* 00446835 */
    MI_U8((0x00000012u+f->r[4]))=(uint8_t)(0x00000000u);
mi_0044683a:;
    /* 0044683a */
    f->r[1]=MI_U32((0x00bca2f0u));
    /* 00446840 */
    a=f->r[3];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 00446842 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00446851;
    /* 00446844 */
    f->r[3]=f->r[1];
    /* 00446846 */
    MI_U8((0x00000013u+f->r[4]))=(uint8_t)(0x00000001u);
    /* 0044684b */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[3]);
    /* 0044684f */
    goto mi_00446856;
mi_00446851:;
    /* 00446851 */
    MI_U8((0x00000013u+f->r[4]))=(uint8_t)(0x00000000u);
mi_00446856:;
    /* 00446856 */
    f->r[2]=MI_U32((0x00bca2ecu));
    /* 0044685c */
    f->r[1]=MI_U32((0x00000014u+f->r[4]));
    /* 00446860 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 00446862 */
    if(!mi_signed_less(f->flags))goto mi_0044686a;
    /* 00446864 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 00446866 */
    f->r[0]=f->r[2];
    /* 00446868 */
    goto mi_0044686c;
mi_0044686a:;
    /* 0044686a */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,5);f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
mi_0044686c:;
    /* 0044686c */
    f->r[2]=MI_U32((0x00000018u+f->r[4]));
    /* 00446870 */
    f->r[5]=MI_U32((0x00bca2f4u));
    /* 00446876 */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 00446878 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00446883;
    /* 0044687a */
    MI_U8((0x00000011u+f->r[4]))=(uint8_t)(0x00000001u);
    /* 0044687f */
    f->r[7]=f->r[5];
    /* 00446881 */
    goto mi_00446888;
mi_00446883:;
    /* 00446883 */
    MI_U8((0x00000011u+f->r[4]))=(uint8_t)(0x00000000u);
mi_00446888:;
    /* 00446888 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044688a */
    if(!(f->flags&MI_ZF))goto mi_004468d4;
    /* 0044688c */
    a=f->r[6];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 0044688e */
    f->r[1]=f->r[0];
    /* 00446890 */
    f->r[2]=f->r[6];
    /* 00446892 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004468d4;
    /* 00446894 */
    f->r[3]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 00446897 */
    f->r[3]=(0x00000000u+f->r[0]+f->r[3]*2u);
    /* 0044689a */
    f->r[3]=(0x00000000u+f->r[3]+f->r[3]*2u);
    /* 0044689d */
    f->r[5]=(0x00000000u+f->r[6]+f->r[3]*4u);
    /* 004468a0 */
    f->r[3]=(0x00ed4140u+f->r[5]);
    /* 004468a6 */
    f->r[5]=(0x00b350dcu+f->r[5]*2u);
    /* 004468ad */
    goto mi_004468b3;
mi_004468af:;
    /* 004468af */
    f->r[3]=MI_U32((0x0000002cu+f->r[4]));
mi_004468b3:;
    /* 004468b3 */
    a=MI_U8((0x00000000u+f->r[3]));b=0x00000000u;t=mi_alu(f,a,b,8,1);
    /* 004468b6 */
    if(mi_signed_less(f->flags))goto mi_004468c3;
    /* 004468b8 */
    a=MI_U16((0x00000000u+f->r[5]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 004468bd */
    if((f->flags&MI_ZF))goto mi_004469f3;
mi_004468c3:;
    /* 004468c3 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004468c4 */
    a=f->r[5];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 004468c7 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 004468c8 */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[3]);
    /* 004468cc */
    f->r[3]=MI_U32((0x0000001cu+f->r[4]));
    /* 004468d0 */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004468d2 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004468af;
mi_004468d4:;
    /* 004468d4 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00000011u+f->r[4])))&0xffu)<<0);
    /* 004468d8 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004468da */
    if(!(f->flags&MI_ZF))goto mi_00446924;
    /* 004468dc */
    a=f->r[6];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004468de */
    f->r[1]=f->r[7];
    /* 004468e0 */
    f->r[2]=f->r[6];
    /* 004468e2 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00446924;
    /* 004468e4 */
    f->r[3]=(0x00000000u+f->r[7]+f->r[7]*8u);
    /* 004468e7 */
    f->r[3]=(0x00000000u+f->r[7]+f->r[3]*2u);
    /* 004468ea */
    f->r[3]=(0x00000000u+f->r[3]+f->r[3]*2u);
    /* 004468ed */
    f->r[5]=(0x00000000u+f->r[6]+f->r[3]*4u);
    /* 004468f0 */
    f->r[3]=(0x00ed4140u+f->r[5]);
    /* 004468f6 */
    f->r[5]=(0x00b350dcu+f->r[5]*2u);
    /* 004468fd */
    goto mi_00446903;
mi_004468ff:;
    /* 004468ff */
    f->r[3]=MI_U32((0x0000002cu+f->r[4]));
mi_00446903:;
    /* 00446903 */
    a=MI_U8((0x00000000u+f->r[3]));b=0x00000000u;t=mi_alu(f,a,b,8,1);
    /* 00446906 */
    if(mi_signed_less(f->flags))goto mi_00446913;
    /* 00446908 */
    a=MI_U16((0x00000000u+f->r[5]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 0044690d */
    if((f->flags&MI_ZF))goto mi_004469f3;
mi_00446913:;
    /* 00446913 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00446914 */
    a=f->r[5];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 00446917 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 00446918 */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[3]);
    /* 0044691c */
    f->r[3]=MI_U32((0x0000001cu+f->r[4]));
    /* 00446920 */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 00446922 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004468ff;
mi_00446924:;
    /* 00446924 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00000012u+f->r[4])))&0xffu)<<0);
    /* 00446928 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044692a */
    if(!(f->flags&MI_ZF))goto mi_0044696e;
    /* 0044692c */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 0044692f */
    f->r[2]=f->r[6];
    /* 00446931 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 00446933 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[1]*2u);
    /* 00446936 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 00446939 */
    f->r[6]=(0x00000000u+f->r[6]+f->r[1]*4u);
    /* 0044693c */
    f->r[1]=f->r[0];
    /* 0044693e */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0044696e;
    /* 00446940 */
    f->r[5]=(0x00ed4140u+f->r[6]);
    /* 00446946 */
    f->r[6]=(0x00b350dcu+f->r[6]*2u);
mi_0044694d:;
    /* 0044694d */
    a=MI_U8((0x00000000u+f->r[5]));b=0x00000000u;t=mi_alu(f,a,b,8,1);
    /* 00446951 */
    if(mi_signed_less(f->flags))goto mi_0044695d;
    /* 00446953 */
    a=MI_U16((0x00000000u+f->r[6]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 00446957 */
    if((f->flags&MI_ZF))goto mi_004469f3;
mi_0044695d:;
    /* 0044695d */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0044695e */
    a=f->r[5];b=0x000000e4u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 00446964 */
    a=f->r[6];b=0x000001c8u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0044696a */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 0044696c */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0044694d;
mi_0044696e:;
    /* 0044696e */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00000013u+f->r[4])))&0xffu)<<0);
    /* 00446972 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00446974 */
    if(!(f->flags&MI_ZF))goto mi_004469b2;
    /* 00446976 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 00446979 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 0044697b */
    f->r[2]=f->r[3];
    /* 0044697d */
    f->r[1]=(0x00000000u+f->r[0]+f->r[1]*2u);
    /* 00446980 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 00446983 */
    f->r[6]=(0x00000000u+f->r[3]+f->r[1]*4u);
    /* 00446986 */
    f->r[1]=f->r[0];
    /* 00446988 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004469b2;
    /* 0044698a */
    f->r[0]=(0x00ed4140u+f->r[6]);
    /* 00446990 */
    f->r[6]=(0x00b350dcu+f->r[6]*2u);
mi_00446997:;
    /* 00446997 */
    a=MI_U8((0x00000000u+f->r[0]));b=0x00000000u;t=mi_alu(f,a,b,8,1);
    /* 0044699a */
    if(mi_signed_less(f->flags))goto mi_004469a2;
    /* 0044699c */
    a=MI_U16((0x00000000u+f->r[6]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 004469a0 */
    if((f->flags&MI_ZF))goto mi_004469f3;
mi_004469a2:;
    /* 004469a2 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004469a3 */
    a=f->r[0];b=0x000000e4u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004469a8 */
    a=f->r[6];b=0x000001c8u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004469ae */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004469b0 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00446997;
mi_004469b2:;
    /* 004469b2 */
    f->r[0]=MI_U32((0x00000028u+f->r[4]));
    /* 004469b6 */
    f->r[3]=MI_U32((0x00000020u+f->r[4]));
    /* 004469ba */
    f->r[7]=MI_U32((0x00000024u+f->r[4]));
    /* 004469be */
    f->r[6]=MI_U32((0x00000014u+f->r[4]));
    /* 004469c2 */
    f->r[2]=MI_U32((0x00000018u+f->r[4]));
    /* 004469c6 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004469c7 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 004469c8 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 004469c9 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 004469ca */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004469cb */
    a=f->r[0];b=0x000000e4u;t=mi_alu(f,a,b,32,1);
    /* 004469d0 */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004469d4 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 004469d8 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 004469dc */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 004469e0 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004469e4 */
    if(!mi_signed_less(f->flags))goto mi_00446a18;
    /* 004469e6 */
    f->r[2]=MI_U32((0x00bca2e8u));
    /* 004469ec */
    f->r[1]=f->r[7];
    /* 004469ee */
    goto mi_0044681a;
mi_004469f3:;
    /* 004469f3 */
    f->r[6]=MI_U32((0x00bca2e8u));
    /* 004469f9 */
    f->r[0]=MI_U32((0x00000034u+f->r[4]));
    /* 004469fd */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004469ff */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00446a00 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[2]);
    /* 00446a02 */
    f->r[0]=MI_U32((0x00bca2ecu));
    /* 00446a07 */
    f->r[2]=MI_U32((0x00000034u+f->r[4]));
    /* 00446a0b */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00446a0d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00446a0e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00446a0f */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[1]);
    /* 00446a11 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 00446a13 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00446a14 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00446a17 */
    f->r[4]+=4;return;
mi_00446a18:;
    /* 00446a18 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00446a19 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00446a1a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00446a1b */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,5);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00446a1d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00446a1e */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00446a21 */
    f->r[4]+=4;return;
}

void restore_invasion_C_00446650(struct restore_invasion_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00446650 */
    f->r[0]=MI_U32((0x00000004u+f->r[4]));
    /* 00446654 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00446655 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 00446657 */
    a=0x00000014u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00446659 */
    f->r[6]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0044665c */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0044665f */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00446661 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00446663 */
    a=f->r[6];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 00446666 */
    MI_U32((0x00bcd71cu+f->r[6]))=(uint32_t)(f->r[1]);
    /* 0044666c */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00bcd602u+f->r[6])))&0xffu)<<0);
    /* 00446672 */
    f->r[2]=(0x00000000u+f->r[0]*8u);
    /* 00446679 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0044667b */
    f->r[0]=MI_U32((0x007090d0u+f->r[2]*8u));
    /* 00446682 */
    MI_U32((0x00bcd720u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 00446688 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044668a */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00bcd602u+f->r[6])))&0xffu)<<0);
    /* 00446690 */
    f->r[2]=(0x00000000u+f->r[0]*8u);
    /* 00446697 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00446699 */
    f->r[0]=MI_U32((0x007090d0u+f->r[2]*8u));
    /* 004466a0 */
    MI_U32((0x00bcd724u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 004466a6 */
    MI_U8((0x00bcd728u+f->r[6]))=(uint8_t)((f->r[1]&0xffu));
    /* 004466ac */
    MI_U32((0x00bcd730u+f->r[6]))=(uint32_t)(f->r[1]);
    /* 004466b2 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004466b4 */
    MI_U32((0x00bcd72cu+f->r[6]))=(uint32_t)(f->r[1]);
    /* 004466ba */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00bcd602u+f->r[6])))&0xffu)<<0);
    /* 004466c0 */
    f->r[2]=(0x00000000u+f->r[0]*8u);
    /* 004466c7 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004466c9 */
    f->r[0]=MI_U32((0x007090ecu+f->r[2]*8u));
    /* 004466d0 */
    MI_U32((0x00bcd734u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 004466d6 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004466d8 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00bcd602u+f->r[6])))&0xffu)<<0);
    /* 004466de */
    f->r[2]=(0x00000000u+f->r[0]*8u);
    /* 004466e5 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004466e7 */
    f->r[0]=MI_U32((0x007090f4u+f->r[2]*8u));
    /* 004466ee */
    MI_U32((0x00bcd738u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 004466f4 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004466f6 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00bcd602u+f->r[6])))&0xffu)<<0);
    /* 004466fc */
    f->r[2]=(0x00000000u+f->r[0]*8u);
    /* 00446703 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00446705 */
    f->r[0]=MI_U32((0x007090fcu+f->r[2]*8u));
    /* 0044670c */
    MI_U32((0x00bcd73cu+f->r[6]))=(uint32_t)(f->r[0]);
    /* 00446712 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00446714 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00bcd602u+f->r[6])))&0xffu)<<0);
    /* 0044671a */
    f->r[2]=(0x00000000u+f->r[0]*8u);
    /* 00446721 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00446723 */
    f->r[0]=MI_U32((0x00709100u+f->r[2]*8u));
    /* 0044672a */
    MI_U32((0x00bcd740u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 00446730 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00446732 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00bcd602u+f->r[6])))&0xffu)<<0);
    /* 00446738 */
    f->r[2]=(0x00000000u+f->r[0]*8u);
    /* 0044673f */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00446741 */
    f->r[0]=MI_U32((0x007090d4u+f->r[2]*8u));
    /* 00446748 */
    MI_U32((0x00bcd748u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 0044674e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00446750 */
    MI_U8((0x00bcd74cu+f->r[6]))=(uint8_t)((f->r[1]&0xffu));
    /* 00446756 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00bcd602u+f->r[6])))&0xffu)<<0);
    /* 0044675c */
    f->r[1]=(0x00000000u+f->r[0]*8u);
    /* 00446763 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00446765 */
    f->r[2]=MI_U32((0x007090d4u+f->r[1]*8u));
    /* 0044676c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044676d */
    f->r[4]-=4;MI_U32(f->r[4])=0x00446772u;f->boundary(0x0044676du,0x004cde40u,f);
    /* 00446772 */
    MI_U32((0x00bcd750u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 00446778 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 0044677d */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 00446783 */
    f->r[1]=MI_U32((0x00ea6210u+f->r[0]));
    /* 00446789 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044678c */
    f->r[0]=(0x00ea6210u+f->r[0]);
    /* 00446792 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00446793 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00446794 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 00446796 */
    f->r[4]+=4;return;
}

void restore_invasion_C_004620c0(struct restore_invasion_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004620c0 */
    f->r[1]=MI_U32((0x00000004u+f->r[4]));
    /* 004620c4 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004620c6 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004620c7 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 004620ca */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004620cd */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004620cf */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004620d1 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004620d4 */
    MI_U32((0x00bcd71cu+f->r[0]))=(uint32_t)(f->r[2]);
    /* 004620da */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00bcd602u+f->r[0])))&0xffu)<<0);
    /* 004620e0 */
    f->r[6]=(0x00000000u+f->r[1]*8u);
    /* 004620e7 */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 004620e9 */
    f->r[1]=MI_U32((0x007090d0u+f->r[6]*8u));
    /* 004620f0 */
    MI_U32((0x00bcd720u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 004620f6 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004620f8 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00bcd602u+f->r[0])))&0xffu)<<0);
    /* 004620fe */
    f->r[6]=(0x00000000u+f->r[1]*8u);
    /* 00462105 */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 00462107 */
    f->r[1]=MI_U32((0x007090d0u+f->r[6]*8u));
    /* 0046210e */
    MI_U32((0x00bcd724u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 00462114 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 00462116 */
    MI_U8((0x00bcd728u+f->r[0]))=(uint8_t)((f->r[2]&0xffu));
    /* 0046211c */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00bcd602u+f->r[0])))&0xffu)<<0);
    /* 00462122 */
    f->r[6]=(0x00000000u+f->r[1]*8u);
    /* 00462129 */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 0046212b */
    f->r[1]=MI_U32((0x007090ecu+f->r[6]*8u));
    /* 00462132 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00462133 */
    MI_U32((0x00bcd734u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 00462139 */
    MI_U8((0x00bcd74cu+f->r[0]))=(uint8_t)((f->r[2]&0xffu));
    /* 0046213f */
    f->r[4]+=4;return;
}

void restore_invasion_C_00469140(struct restore_invasion_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00469140 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 00469143 */
    f->r[1]=mi_read((0x00000020u+f->r[4]),4u);
    /* 00469147 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00469148 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00469149 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046914a */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00b4e6fcu+f->r[1]*2u),2u);
    /* 00469152 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00469154 */
    MI_U32((0x0000000cu+f->r[4]))=(uint32_t)(0x00000000u);
    /* 0046915c */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x0000000fu);
    /* 00469164 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(0x0000001eu);
    /* 0046916c */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(0x0000003cu);
    /* 00469174 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(0x00000064u);
    /* 0046917c */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004691b7;
    /* 0046917e */
    f->r[0]=mi_read((0x00af598cu+f->r[1]*4u),4u);
    /* 00469185 */
    f->r[6]=mi_read((0x00000028u+f->r[4]),4u);
    /* 00469189 */
    a=((f->r[0]>>8)&0xffu);b=0x00000040u;t=mi_alu(f,a,b,8,3);
    /* 0046918c */
    if((f->flags&MI_ZF))goto mi_0046919e;
    /* 0046918e */
    f->r[7]=mi_read((0x00000024u+f->r[4]),4u);
    /* 00469192 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00469193 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00469194 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00469199u;f->boundary(0x00469194u,0x004b7510u,f);
    /* 00469199 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046919c */
    goto mi_004691a2;
mi_0046919e:;
    /* 0046919e */
    f->r[7]=mi_read((0x00000024u+f->r[4]),4u);
mi_004691a2:;
    /* 004691a2 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004691a4 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004691a6 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004691a7 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004691a8 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004691aa */
    f->r[4]-=4;MI_U32(f->r[4])=0x004691afu;f->boundary(0x004691aau,0x0047c2b0u,f);
    /* 004691af */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004691b2 */
    goto mi_00469507;
mi_004691b7:;
    /* 004691b7 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004691b8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004691bdu;f->boundary(0x004691b8u,0x00475b50u,f);
    /* 004691bd */
    f->r[5]=f->r[0];
    /* 004691bf */
    f->r[6]=f->r[5];
    /* 004691c1 */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004691c4 */
    a=f->r[6];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004691c6 */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004691c9 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e8u+f->r[6]),2u);
    /* 004691d0 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004691d1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004691d6u;f->boundary(0x004691d1u,0x00474380u,f);
    /* 004691d6 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004691d9 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004691db */
    if(!(f->flags&MI_ZF))goto mi_0046953e;
    /* 004691e1 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x007999e8u+f->r[6]),2u))&0xffffu)<<0);
    /* 004691e8 */
    a=(f->r[0]&0xffffu);b=0x000000cau;t=mi_alu(f,a,b,16,1);
    /* 004691ec */
    if(!(f->flags&MI_ZF))goto mi_004691f9;
    /* 004691ee */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004691ef */
    f->r[4]-=4;MI_U32(f->r[4])=0x004691f4u;f->boundary(0x004691efu,0x004c03d0u,f);
    /* 004691f4 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004691f7 */
    goto mi_00469266;
mi_004691f9:;
    /* 004691f9 */
    a=(f->r[0]&0xffffu);b=0x0000001fu;t=mi_alu(f,a,b,16,1);
    /* 004691fd */
    if((f->flags&MI_ZF))goto mi_00469236;
    /* 004691ff */
    a=(f->r[0]&0xffffu);b=0x0000001eu;t=mi_alu(f,a,b,16,1);
    /* 00469203 */
    if((f->flags&MI_ZF))goto mi_00469236;
    /* 00469205 */
    a=(f->r[0]&0xffffu);b=0x00000021u;t=mi_alu(f,a,b,16,1);
    /* 00469209 */
    if((f->flags&MI_ZF))goto mi_00469236;
    /* 0046920b */
    a=(f->r[0]&0xffffu);b=0x000000d1u;t=mi_alu(f,a,b,16,1);
    /* 0046920f */
    if((f->flags&MI_ZF))goto mi_00469236;
    /* 00469211 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e0u+f->r[6]),2u);
    /* 00469218 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999deu+f->r[6]),2u);
    /* 0046921f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00469221 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x007999dbu+f->r[6]),1u))&0xffu)<<0);
    /* 00469227 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00469228 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00469229 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046922a */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046922b */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046922c */
    f->r[4]-=4;MI_U32(f->r[4])=0x00469231u;f->boundary(0x0046922cu,0x0047c2b0u,f);
    /* 00469231 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00469234 */
    goto mi_00469266;
mi_00469236:;
    /* 00469236 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00469237 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046923cu;f->boundary(0x00469237u,0x0046d0f0u,f);
    /* 0046923c */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046923f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00469241 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00469266;
    /* 00469243 */
    f->r[1]=f->r[0];
    /* 00469245 */
    MI_U32((0x0077567cu))=(uint32_t)(0x00000001u);
    /* 0046924f */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00469252 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00469254 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e8u+f->r[1]*8u),2u);
    /* 0046925c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046925d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046925e */
    f->r[4]-=4;MI_U32(f->r[4])=0x00469263u;f->boundary(0x0046925eu,0x0043fd00u,f);
    /* 00469263 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00469266:;
    /* 00469266 */
    a=mi_read((0x007999d8u+f->r[6]),1u);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 0046926d */
    if(!(f->flags&MI_ZF))goto mi_004694ff;
    /* 00469273 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x007999e8u+f->r[6]),2u))&0xffffu)<<0);
    /* 0046927a */
    a=(f->r[0]&0xffffu);b=0x0000000au;t=mi_alu(f,a,b,16,1);
    /* 0046927e */
    if((f->flags&MI_ZF))goto mi_004694ab;
    /* 00469284 */
    a=(f->r[0]&0xffffu);b=0x0000000bu;t=mi_alu(f,a,b,16,1);
    /* 00469288 */
    if((f->flags&MI_ZF))goto mi_004694ab;
    /* 0046928e */
    a=(f->r[0]&0xffffu);b=0x00000037u;t=mi_alu(f,a,b,16,1);
    /* 00469292 */
    if((f->flags&MI_ZF))goto mi_004694ab;
    /* 00469298 */
    a=(f->r[0]&0xffffu);b=0x00000051u;t=mi_alu(f,a,b,16,1);
    /* 0046929c */
    if((f->flags&MI_ZF))goto mi_004694ab;
    /* 004692a2 */
    a=(f->r[0]&0xffffu);b=0x0000005cu;t=mi_alu(f,a,b,16,1);
    /* 004692a6 */
    if((f->flags&MI_ZF))goto mi_004694ab;
    /* 004692ac */
    a=(f->r[0]&0xffffu);b=0x00000039u;t=mi_alu(f,a,b,16,1);
    /* 004692b0 */
    if((f->flags&MI_ZF))goto mi_004694ab;
    /* 004692b6 */
    a=(f->r[0]&0xffffu);b=0x00000036u;t=mi_alu(f,a,b,16,1);
    /* 004692ba */
    if((f->flags&MI_ZF))goto mi_004694ab;
    /* 004692c0 */
    a=(f->r[0]&0xffffu);b=0x0000003au;t=mi_alu(f,a,b,16,1);
    /* 004692c4 */
    if((f->flags&MI_ZF))goto mi_004694ab;
    /* 004692ca */
    a=(f->r[0]&0xffffu);b=0x000000aau;t=mi_alu(f,a,b,16,1);
    /* 004692ce */
    if((f->flags&MI_ZF))goto mi_004694ab;
    /* 004692d4 */
    a=(f->r[0]&0xffffu);b=0x000000abu;t=mi_alu(f,a,b,16,1);
    /* 004692d8 */
    if((f->flags&MI_ZF))goto mi_004694ab;
    /* 004692de */
    a=(f->r[0]&0xffffu);b=0x000000c8u;t=mi_alu(f,a,b,16,1);
    /* 004692e2 */
    if((f->flags&MI_ZF))goto mi_004694ab;
    /* 004692e8 */
    a=(f->r[0]&0xffffu);b=0x000000c9u;t=mi_alu(f,a,b,16,1);
    /* 004692ec */
    if((f->flags&MI_ZF))goto mi_004694ab;
    /* 004692f2 */
    a=(f->r[0]&0xffffu);b=0x000000cau;t=mi_alu(f,a,b,16,1);
    /* 004692f6 */
    if((f->flags&MI_ZF))goto mi_004694ab;
    /* 004692fc */
    a=(f->r[0]&0xffffu);b=0x0000003bu;t=mi_alu(f,a,b,16,1);
    /* 00469300 */
    if((f->flags&MI_ZF))goto mi_004694ab;
    /* 00469306 */
    a=(f->r[0]&0xffffu);b=0x000000acu;t=mi_alu(f,a,b,16,1);
    /* 0046930a */
    if((f->flags&MI_ZF))goto mi_004694ab;
    /* 00469310 */
    a=(f->r[0]&0xffffu);b=0x000000adu;t=mi_alu(f,a,b,16,1);
    /* 00469314 */
    if((f->flags&MI_ZF))goto mi_004694ab;
    /* 0046931a */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046931b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00469320u;f->boundary(0x0046931bu,0x00459ec0u,f);
    /* 00469320 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00469323 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00469325 */
    if((f->flags&MI_ZF))goto mi_0046939f;
    /* 00469327 */
    f->r[0]=mi_read((0x00eaac28u),4u);
    /* 0046932c */
    f->r[1]=mi_read((0x00e39080u),4u);
    /* 00469332 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 00469338 */
    f->r[2]=mi_read((0x00ea5ec8u+f->r[0]),4u);
    /* 0046933e */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0046933f */
    MI_U32((0x00ea5ec8u+f->r[0]))=(uint32_t)(f->r[2]);
    /* 00469345 */
    f->r[2]=mi_read((0x0000000cu+f->r[4]+f->r[1]*4u),4u);
    /* 00469349 */
    f->r[0]=mi_read((0x00ea5cc0u+f->r[0]),4u);
    /* 0046934f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00469350 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00469351 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00469356u;f->boundary(0x00469351u,0x004cde40u,f);
    /* 00469356 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00469359 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0046935b */
    if((f->flags&MI_ZF))goto mi_004694ab;
    /* 00469361 */
    f->r[1]=mi_read((0x00eaac28u),4u);
    /* 00469367 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00469369 */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 0046936f */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00469371 */
    a=0x000000c7u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00469376 */
    f->r[7]=mi_read((0x00ea5cc0u+f->r[1]),4u);
    /* 0046937c */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 0046937e */
    MI_U32((0x00ea5cc0u+f->r[1]))=(uint32_t)(f->r[7]);
    /* 00469384 */
    f->r[2]=mi_read((0x00ea5ff4u+f->r[1]),4u);
    /* 0046938a */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0046938c */
    MI_U32((0x00ea5ff4u+f->r[1]))=(uint32_t)(f->r[2]);
    /* 00469392 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00469397u;f->boundary(0x00469392u,0x004e2970u,f);
    /* 00469397 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046939a */
    goto mi_004694ab;
mi_0046939f:;
    /* 0046939f */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004693a0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004693a5u;f->boundary(0x004693a0u,0x00459ef0u,f);
    /* 004693a5 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004693a8 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004693aa */
    if((f->flags&MI_ZF))goto mi_00469491;
    /* 004693b0 */
    f->r[1]=mi_read((0x00eaac28u),4u);
    /* 004693b6 */
    f->r[2]=mi_read((0x00e39080u),4u);
    /* 004693bc */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 004693c2 */
    f->r[0]=(0x00ea5ec8u+f->r[1]);
    /* 004693c8 */
    f->r[1]=mi_read((0x00ea5ec8u+f->r[1]),4u);
    /* 004693ce */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004693cf */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 004693d1 */
    f->r[0]=mi_read((0x0000000cu+f->r[4]+f->r[2]*4u),4u);
    /* 004693d5 */
    f->r[1]=mi_read((0x00799a90u+f->r[6]),4u);
    /* 004693db */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004693dc */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004693dd */
    f->r[4]-=4;MI_U32(f->r[4])=0x004693e2u;f->boundary(0x004693ddu,0x004cde40u,f);
    /* 004693e2 */
    f->r[7]=f->r[0];
    /* 004693e4 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004693e7 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 004693e9 */
    if((f->flags&MI_ZF))goto mi_004694ab;
    /* 004693ef */
    f->r[4]-=4;MI_U32(f->r[4])=0x004693f4u;f->boundary(0x004693efu,0x004684e0u,f);
    /* 004693f4 */
    f->r[0]=mi_read((0x00eaac28u),4u);
    /* 004693f9 */
    MI_U32((0x00e950dcu))=(uint32_t)(0x00000001u);
    /* 00469403 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 00469409 */
    a=mi_read((0x00ea5cfcu+f->r[0]),2u);b=f->flags&MI_CF;t=mi_alu(f,a,1,16,0);f->flags=(f->flags&~MI_CF)|b;MI_U16((0x00ea5cfcu+f->r[0]))=(uint16_t)(t);
    /* 00469410 */
    f->r[2]=mi_read((0x00ea44f4u+f->r[0]),4u);
    /* 00469416 */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00469418 */
    MI_U32((0x00ea44f4u+f->r[0]))=(uint32_t)(f->r[2]);
    /* 0046941e */
    f->r[1]=mi_read((0x00ea5fd4u+f->r[0]),4u);
    /* 00469424 */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00469426 */
    MI_U32((0x00ea5fd4u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 0046942c */
    f->r[2]=mi_read((0x00ea187cu+f->r[0]),4u);
    /* 00469432 */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00469434 */
    MI_U32((0x00ea187cu+f->r[0]))=(uint32_t)(f->r[2]);
    /* 0046943a */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e8u+f->r[6]),2u);
    /* 00469441 */
    a=f->r[0];b=0xffffffacu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00469444 */
    a=f->r[0];b=0x00000069u;t=mi_alu(f,a,b,32,1);
    /* 00469447 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004694ab;
    /* 00469449 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0046944b */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00469558u+f->r[0]),1u))&0xffu)<<0);
    /* 00469451 */
    switch(mi_read((0x00469548u+f->r[2]*4u),4u)){case 0x00469458u:goto mi_00469458;case 0x0046946bu:goto mi_0046946b;case 0x0046947eu:goto mi_0046947e;case 0x004694abu:goto mi_004694ab;default:abort();}
mi_00469458:;
    /* 00469458 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046945a */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046945c */
    a=0x000000c9u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00469461 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00469466u;f->boundary(0x00469461u,0x004e2970u,f);
    /* 00469466 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00469469 */
    goto mi_004694ab;
mi_0046946b:;
    /* 0046946b */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046946d */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046946f */
    a=0x000000cau;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00469474 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00469479u;f->boundary(0x00469474u,0x004e2970u,f);
    /* 00469479 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046947c */
    goto mi_004694ab;
mi_0046947e:;
    /* 0046947e */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00469480 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00469482 */
    a=0x000000c8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00469487 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046948cu;f->boundary(0x00469487u,0x004e2970u,f);
    /* 0046948c */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046948f */
    goto mi_004694ab;
mi_00469491:;
    /* 00469491 */
    f->r[0]=mi_read((0x00eaac28u),4u);
    /* 00469496 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0046949c */
    f->r[1]=mi_read((0x00ea5ec8u+f->r[0]),4u);
    /* 004694a2 */
    f->r[0]=(0x00ea5ec8u+f->r[0]);
    /* 004694a8 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004694a9 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[1]);
mi_004694ab:;
    /* 004694ab */
    f->r[0]=mi_read((0x00eaac28u),4u);
    /* 004694b0 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004694b6 */
    f->r[1]=mi_read((0x00ea5ec8u+f->r[0]),4u);
    /* 004694bc */
    f->r[0]=(0x00ea5ec8u+f->r[0]);
    /* 004694c2 */
    a=f->r[1];b=0x0000000cu;t=mi_alu(f,a,b,32,1);
    /* 004694c5 */
    if(mi_signed_less(f->flags))goto mi_004694cd;
    /* 004694c7 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(0x0000000cu);
mi_004694cd:;
    /* 004694cd */
    MI_U8((0x007999d8u+f->r[6]))=(uint8_t)(0x00000004u);
    /* 004694d4 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004694d6 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e0u+f->r[6]),2u);
    /* 004694dd */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x007999dbu+f->r[6]),1u))&0xffu)<<0);
    /* 004694e3 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004694e5 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999deu+f->r[6]),2u);
    /* 004694ec */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004694ed */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004694ee */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004694ef */
    f->r[4]-=4;MI_U32(f->r[4])=0x004694f4u;f->boundary(0x004694efu,0x0048c0e0u,f);
    /* 004694f4 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004694f6 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004694f7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004694fcu;f->boundary(0x004694f7u,0x004695d0u,f);
    /* 004694fc */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004694ff:;
    /* 004694ff */
    f->r[6]=mi_read((0x00000028u+f->r[4]),4u);
    /* 00469503 */
    f->r[7]=mi_read((0x00000024u+f->r[4]),4u);
mi_00469507:;
    /* 00469507 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046950cu;f->boundary(0x00469507u,0x00497b90u,f);
    /* 0046950c */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046950e */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046950f */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00469510 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00469515u;f->boundary(0x00469510u,0x0047afd0u,f);
    /* 00469515 */
    f->r[1]=(0x00000002u+f->r[6]);
    /* 00469518 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046951a */
    f->r[2]=(0x00000002u+f->r[7]);
    /* 0046951d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046951e */
    a=f->r[6];b=0xfffffffeu;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00469521 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00469522 */
    a=f->r[7];b=0xfffffffeu;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 00469525 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00469526 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00469527 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046952cu;f->boundary(0x00469527u,0x0047bae0u,f);
    /* 0046952c */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046952f */
    f->r[4]-=4;MI_U32(f->r[4])=0x00469534u;f->boundary(0x0046952fu,0x00517370u,f);
    /* 00469534 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00469539u;f->boundary(0x00469534u,0x00517660u,f);
    /* 00469539 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046953eu;f->boundary(0x00469539u,0x005178e0u,f);
mi_0046953e:;
    /* 0046953e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0046953f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00469540 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00469541 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00469544 */
    f->r[4]+=4;return;
}

void restore_invasion_C_004b7510(struct restore_invasion_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004b7510 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b7511 */
    f->r[5]=MI_U32((0x0000000cu+f->r[4]));
    /* 004b7515 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b7516 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b7517 */
    f->r[7]=MI_U32((0x00000010u+f->r[4]));
    /* 004b751b */
    f->r[6]=0x00bcd630u;
mi_004b7520:;
    /* 004b7520 */
    a=MI_U8((0xffffffd6u+f->r[6]));b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004b7524 */
    if(!(f->flags&MI_ZF))goto mi_004b757b;
    /* 004b7526 */
    a=MI_U8((0xffffffd2u+f->r[6]));b=0x0000002au;t=mi_alu(f,a,b,8,1);
    /* 004b752a */
    if(!(f->flags&MI_ZF))goto mi_004b757b;
    /* 004b752c */
    a=MI_U8((0x00000000u+f->r[6]));b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 004b752f */
    if((f->flags&MI_ZF))goto mi_004b757b;
    /* 004b7531 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0xffffffdcu+f->r[6]));
    /* 004b7535 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0xffffffdeu+f->r[6]));
    /* 004b7539 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004b753b */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004b7543;
    /* 004b753d */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004b753f */
    f->r[2]=f->r[0];
    /* 004b7541 */
    goto mi_004b754d;
mi_004b7543:;
    /* 004b7543 */
    if(!mi_signed_less(f->flags))goto mi_004b754b;
    /* 004b7545 */
    f->r[2]=f->r[7];
    /* 004b7547 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004b7549 */
    goto mi_004b754d;
mi_004b754b:;
    /* 004b754b */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
mi_004b754d:;
    /* 004b754d */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004b754f */
    MI_U32((0x00d3b4fcu))=(uint32_t)(f->r[2]);
    /* 004b7555 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004b755d;
    /* 004b7557 */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004b7559 */
    f->r[0]=f->r[1];
    /* 004b755b */
    goto mi_004b7567;
mi_004b755d:;
    /* 004b755d */
    if(!mi_signed_less(f->flags))goto mi_004b7565;
    /* 004b755f */
    f->r[0]=f->r[5];
    /* 004b7561 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004b7563 */
    goto mi_004b7567;
mi_004b7565:;
    /* 004b7565 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_004b7567:;
    /* 004b7567 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004b7569 */
    MI_U32((0x00d3b4f8u))=(uint32_t)(f->r[0]);
    /* 004b756e */
    if(mi_signed_less(f->flags))goto mi_004b7572;
    /* 004b7570 */
    f->r[0]=f->r[2];
mi_004b7572:;
    /* 004b7572 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004b7575 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004b757b;
    /* 004b7577 */
    MI_U8((0xffffffd6u+f->r[6]))=(uint8_t)(0x00000002u);
mi_004b757b:;
    /* 004b757b */
    a=f->r[6];b=0x00000184u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004b7581 */
    a=f->r[6];b=0x00c8ad70u;t=mi_alu(f,a,b,32,1);
    /* 004b7587 */
    if(mi_signed_less(f->flags))goto mi_004b7520;
    /* 004b7589 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004b758a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b758b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004b758c */
    f->r[4]+=4;return;
}

void restore_invasion_C_00459ec0(struct restore_invasion_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00459ec0 */
    f->r[0]=MI_U32((0x00000004u+f->r[4]));
    /* 00459ec4 */
    f->r[1]=f->r[0];
    /* 00459ec6 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00459ec9 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00459ecb */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e8u+f->r[1]*8u));
    /* 00459ed3 */
    a=f->r[0];b=0x0000004du;t=mi_alu(f,a,b,32,1);
    /* 00459ed6 */
    if(mi_signed_less(f->flags))goto mi_00459ee3;
    /* 00459ed8 */
    a=f->r[0];b=0x0000004fu;t=mi_alu(f,a,b,32,1);
    /* 00459edb */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00459ee3;
    /* 00459edd */
    f->r[0]=0x00000001u;
    /* 00459ee2 */
    f->r[4]+=4;return;
mi_00459ee3:;
    /* 00459ee3 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00459ee5 */
    f->r[4]+=4;return;
}

void restore_invasion_C_004c03d0(struct restore_invasion_frame *f)
{
 uint32_t sp=f->r[4];restore_building_clear_four_parts((int32_t)MI_U32(sp+4u));f->r[4]=sp+4u;
}

void restore_invasion_C_004684e0(struct restore_invasion_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004684e0 */
    f->r[0]=MI_U32((0x0078d790u));
    /* 004684e5 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004684e6 */
    f->r[7]=0x00000001u;
    /* 004684eb */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004684ed */
    if(mi_signed_less(f->flags))goto mi_0046852a;
    /* 004684ef */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004684f0 */
    f->r[6]=0x00799b98u;
mi_004684f5:;
    /* 004684f5 */
    a=MI_U8((0xffffff48u+f->r[6]));b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004684fc */
    if(!(f->flags&MI_ZF))goto mi_00468519;
    /* 004684fe */
    MI_U32((0x00000000u+f->r[6]))=(uint32_t)(0x00000000u);
    /* 00468504 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0xffffff4du+f->r[6])))&0xffu)<<0);
    /* 0046850a */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0046850c */
    if(!(f->flags&MI_ZF))goto mi_00468519;
    /* 0046850e */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046850f */
    f->r[4]-=4;MI_U32(f->r[4])=0x00468514u;f->boundary(0x0046850fu,0x0045a140u,f);
    /* 00468514 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00468517 */
    MI_U32((0x00000000u+f->r[6]))=(uint32_t)(f->r[0]);
mi_00468519:;
    /* 00468519 */
    f->r[0]=MI_U32((0x0078d790u));
    /* 0046851e */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 0046851f */
    a=f->r[6];b=0x00000108u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00468525 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00468527 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004684f5;
    /* 00468529 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
mi_0046852a:;
    /* 0046852a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0046852b */
    f->r[4]+=4;return;
}

void restore_invasion_C_0048c0e0(struct restore_invasion_frame *f)
{
 uint32_t sp=f->r[4];restore_building_spawn_removal_effects((int32_t)MI_U32(sp+4u),(int32_t)MI_U32(sp+8u),MI_U32(sp+12u),MI_U32(sp+16u));f->r[4]=sp+4u;
}

void restore_invasion_C_00497b90(struct restore_invasion_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00497b90 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00497b91 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00497b92 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00497b93 */
    f->r[7]=0x00000001u;
    /* 00497b98 */
    f->r[6]=0x00bcd7d6u;
    /* 00497b9d */
    f->r[3]=(f->r[3]&0xffffff00u)|(((0x00000008u)&0xffu)<<0);
mi_00497b9f:;
    /* 00497b9f */
    a=MI_U8((0xffffffb0u+f->r[6]));b=0x0000002au;t=mi_alu(f,a,b,8,1);
    /* 00497ba3 */
    if(!(f->flags&MI_ZF))goto mi_00497cd6;
    /* 00497ba9 */
    f->r[0]=MI_U32((0xffffffc6u+f->r[6]));
    /* 00497bac */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00ebab20u+f->r[0])))&0xffu)<<0);
    /* 00497bb2 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00497bb4 */
    if((f->flags&MI_ZF))goto mi_00497cd6;
    /* 00497bba */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0xffffffbcu+f->r[6]));
    /* 00497bbe */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0xffffffbau+f->r[6]));
    /* 00497bc2 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00497bc4 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00497bc6 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00497bc7 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00497bc8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00497bcdu;f->boundary(0x00497bc8u,0x00480060u,f);
    /* 00497bcd */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00497bd0 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00497bd2 */
    if(!(f->flags&MI_ZF))goto mi_00497c3e;
    /* 00497bd4 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00497bd5 */
    MI_U8((0xffffffdeu+f->r[6]))=(uint8_t)((f->r[3]&0xffu));
    /* 00497bd8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00497bddu;f->boundary(0x00497bd8u,0x004b5420u,f);
    /* 00497bdd */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[6]));
    /* 00497be0 */
    f->r[0]=f->r[1];
    /* 00497be2 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00497be3 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00497be6 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00497be8 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00497beb */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x007999deu+f->r[0])))&0xffffu)<<0);
    /* 00497bf2 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x007999e0u+f->r[0])))&0xffffu)<<0);
    /* 00497bf9 */
    MI_U16((0xffffffbau+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
    /* 00497bfd */
    MI_U16((0xffffffd2u+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
    /* 00497c01 */
    MI_U16((0xffffffbcu+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 00497c05 */
    MI_U16((0xffffffd4u+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 00497c09 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0xffffffbcu+f->r[6]));
    /* 00497c0d */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 00497c10 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[1]*2u);
    /* 00497c13 */
    f->r[1]=MI_U32((0x00bca2d8u));
    /* 00497c19 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 00497c1c */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0xffffffbau+f->r[6]));
    /* 00497c20 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00497c22 */
    f->r[2]=(0x00000000u+f->r[1]+f->r[2]*4u);
    /* 00497c25 */
    MI_U32((0xffffffc6u+f->r[6]))=(uint32_t)(f->r[2]);
    /* 00497c28 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00497c2du;f->boundary(0x00497c28u,0x004b52f0u,f);
    /* 00497c2d */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00497c2e */
    MI_U8((0xffffffdeu+f->r[6]))=(uint8_t)((f->r[3]&0xffu));
    /* 00497c31 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00497c36u;f->boundary(0x00497c31u,0x004b3250u,f);
    /* 00497c36 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00497c39 */
    goto mi_00497cd6;
mi_00497c3e:;
    /* 00497c3e */
    f->r[0]=MI_U32((0x00d056f8u));
    /* 00497c43 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00497c44 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00497c49u;f->boundary(0x00497c44u,0x004b3250u,f);
    /* 00497c49 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00497c4a */
    MI_U8((0xffffffdfu+f->r[6]))=(uint8_t)(0x00000000u);
    /* 00497c4e */
    f->r[4]-=4;MI_U32(f->r[4])=0x00497c53u;f->boundary(0x00497c4eu,0x004b5420u,f);
    /* 00497c53 */
    f->r[1]=MI_U32((0x00d3b4e4u));
    /* 00497c59 */
    f->r[0]=MI_U32((0x00d3b4e0u));
    /* 00497c5e */
    MI_U16((0xffffffbau+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
    /* 00497c62 */
    MI_U16((0xffffffbeu+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
    /* 00497c66 */
    f->r[2]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 00497c69 */
    MI_U16((0xffffffbcu+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 00497c6d */
    MI_U16((0xffffffc0u+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 00497c71 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00497c72 */
    f->r[2]=(0x00000000u+f->r[2]+f->r[2]*4u);
    /* 00497c75 */
    MI_U16((0xfffffff0u+f->r[6]))=(uint16_t)((f->r[2]&0xffffu));
    /* 00497c79 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 00497c7c */
    f->r[2]=(0x00000000u+f->r[2]+f->r[2]*4u);
    /* 00497c7f */
    MI_U16((0xfffffff2u+f->r[6]))=(uint16_t)((f->r[2]&0xffffu));
    /* 00497c83 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 00497c86 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*2u);
    /* 00497c89 */
    f->r[2]=MI_U32((0x00bca2d8u));
    /* 00497c8f */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 00497c92 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[0]*4u);
    /* 00497c95 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00497c97 */
    MI_U32((0xffffffc6u+f->r[6]))=(uint32_t)(f->r[1]);
    /* 00497c9a */
    f->r[4]-=4;MI_U32(f->r[4])=0x00497c9fu;f->boundary(0x00497c9au,0x004b52f0u,f);
    /* 00497c9f */
    f->r[1]=MI_U32((0x00d056f8u));
    /* 00497ca5 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00497ca8 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 00497cab */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00497cae */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00497cb0 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00497cb3 */
    MI_U8((0x00bcd630u+f->r[0]))=(uint8_t)(0x0000000bu);
    /* 00497cba */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x00bcd624u+f->r[0])))&0xffffu)<<0);
    /* 00497cc1 */
    MI_U16((0x00bcd61cu+f->r[0]))=(uint16_t)((f->r[2]&0xffffu));
    /* 00497cc8 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x00bcd626u+f->r[0])))&0xffffu)<<0);
    /* 00497ccf */
    MI_U16((0x00bcd61eu+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
mi_00497cd6:;
    /* 00497cd6 */
    a=f->r[6];b=0x00000184u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00497cdc */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 00497cdd */
    a=f->r[6];b=0x00c8ad92u;t=mi_alu(f,a,b,32,1);
    /* 00497ce3 */
    if(mi_signed_less(f->flags))goto mi_00497b9f;
    /* 00497ce9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00497cea */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00497ceb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00497cec */
    f->r[4]+=4;return;
}

void restore_invasion_C_00480060(struct restore_invasion_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00480060 */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 00480064 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00480065 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00480066 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00480067 */
    f->r[6]=0x00000001u;
    /* 0048006c */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048006d */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 0048006f */
    if(mi_signed_less(f->flags))goto mi_00480096;
    /* 00480071 */
    f->r[7]=MI_U32((0x0000001cu+f->r[4]));
    /* 00480075 */
    f->r[3]=MI_U32((0x00000018u+f->r[4]));
    /* 00480079 */
    f->r[5]=MI_U32((0x00000014u+f->r[4]));
mi_0048007d:;
    /* 0048007d */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048007e */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048007f */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00480080 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00480081 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00480086u;f->boundary(0x00480081u,0x004800b0u,f);
    /* 00480086 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00480089 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0048008b */
    if(!(f->flags&MI_ZF))goto mi_0048009d;
    /* 0048008d */
    f->r[0]=MI_U32((0x00000020u+f->r[4]));
    /* 00480091 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 00480092 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00480094 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0048007d;
mi_00480096:;
    /* 00480096 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00480097 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00480098 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00480099 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0048009b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0048009c */
    f->r[4]+=4;return;
mi_0048009d:;
    /* 0048009d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0048009e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0048009f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004800a0 */
    f->r[0]=0x00000001u;
    /* 004800a5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004800a6 */
    f->r[4]+=4;return;
}

void restore_invasion_C_004800b0(struct restore_invasion_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004800b0 */
    f->r[2]=MI_U32((0x00000004u+f->r[4]));
    /* 004800b4 */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
    /* 004800b8 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004800b9 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004800ba */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004800bb */
    f->r[6]=MI_U32((0x00000018u+f->r[4]));
    /* 004800bf */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004800c0 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[6]*1u);
    /* 004800c3 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004800c5 */
    f->r[7]=(0xffffffffu+f->r[0]+f->r[1]*1u);
    /* 004800c9 */
    f->r[0]=MI_U32((0x00000018u+f->r[4]));
    /* 004800cd */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004800cf */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004800d1 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 004800d3 */
    f->r[5]=(0xffffffffu+f->r[6]+f->r[1]*1u);
    /* 004800d7 */
    if(!mi_signed_less(f->flags))goto mi_004800db;
    /* 004800d9 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
mi_004800db:;
    /* 004800db */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004800dd */
    if(!mi_signed_less(f->flags))goto mi_004800e1;
    /* 004800df */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_004800e1:;
    /* 004800e1 */
    f->r[1]=MI_U32((0x00bca2d0u));
    /* 004800e7 */
    a=f->r[7];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004800e9 */
    if(mi_signed_less(f->flags))goto mi_004800ee;
    /* 004800eb */
    f->r[7]=(0xffffffffu+f->r[1]);
mi_004800ee:;
    /* 004800ee */
    f->r[1]=MI_U32((0x00bca2d4u));
    /* 004800f4 */
    a=f->r[5];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004800f6 */
    if(mi_signed_less(f->flags))goto mi_004800fb;
    /* 004800f8 */
    f->r[5]=(0xffffffffu+f->r[1]);
mi_004800fb:;
    /* 004800fb */
    f->r[6]=MI_U32((0x00bca2d8u));
    /* 00480101 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 00480104 */
    f->r[3]=f->r[0];
    /* 00480106 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[1]*2u);
    /* 00480109 */
    MI_U32((0x00d3b4e0u))=(uint32_t)(f->r[3]);
    /* 0048010f */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 00480112 */
    f->r[1]=(0x00000000u+f->r[6]+f->r[1]*4u);
    /* 00480115 */
    f->r[6]=f->r[2];
    /* 00480117 */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 00480119 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048011b */
    a=f->r[6];b=0x000000e3u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00480121 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 00480123 */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[1]);
    /* 00480129 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00480163;
mi_0048012b:;
    /* 0048012b */
    f->r[0]=f->r[2];
    /* 0048012d */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 0048012f */
    MI_U32((0x00d3b4e4u))=(uint32_t)(f->r[0]);
    /* 00480134 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00480150;
mi_00480136:;
    /* 00480136 */
    a=MI_U8((0x00ebab20u+f->r[1]));b=0x00000000u;t=mi_alu(f,a,b,8,1);
    /* 0048013d */
    if((f->flags&MI_ZF))goto mi_0048016a;
    /* 0048013f */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00480140 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00480141 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 00480143 */
    MI_U32((0x00d3b4e4u))=(uint32_t)(f->r[0]);
    /* 00480148 */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[1]);
    /* 0048014e */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00480136;
mi_00480150:;
    /* 00480150 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 00480151 */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00480153 */
    a=f->r[3];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 00480155 */
    MI_U32((0x00d3b4e0u))=(uint32_t)(f->r[3]);
    /* 0048015b */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[1]);
    /* 00480161 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0048012b;
mi_00480163:;
    /* 00480163 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00480164 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00480165 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00480166 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00480168 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00480169 */
    f->r[4]+=4;return;
mi_0048016a:;
    /* 0048016a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0048016b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0048016c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0048016d */
    f->r[0]=0x00000001u;
    /* 00480172 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00480173 */
    f->r[4]+=4;return;
}
