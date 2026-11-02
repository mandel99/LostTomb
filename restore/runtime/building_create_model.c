/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "building_create_model.h"
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

static const uint8_t mi_rom_004724f4[]={0xec,0x09,0x47,0x00,0x1b,0x0a,0x47,0x00,0x5c,0x0a,0x47,0x00,0xd1,0x0a,0x47,0x00,0xa1,0x0b,0x47,0x00,0x3b,0x0b,0x47,0x00,0xf7,0x0b,0x47,0x00,0x05,0x0c,0x47,0x00,0x13,0x0c,0x47,0x00,0xa0,0x14,0x47,0x00,0x21,0x0c,0x47,0x00,0xaf,0x0c,0x47,0x00,0xbd,0x0c,0x47,0x00,0xd9,0x0c,0x47,0x00,0xab,0x17,0x47,0x00,0xe7,0x0c,0x47,0x00,0xf5,0x0c,0x47,0x00,0x03,0x0d,0x47,0x00,0x5a,0x10,0x47,0x00,0x68,0x10,0x47,0x00,0x76,0x10,0x47,0x00,0x84,0x10,0x47,0x00,0x92,0x10,0x47,0x00,0xa0,0x10,0x47,0x00,0xeb,0x0e,0x47,0x00,0xb6,0x0e,0x47,0x00,0x5c,0x1e,0x47,0x00,0x52,0x11,0x47,0x00,0x96,0x21,0x47,0x00,0xc2,0x12,0x47,0x00,0x53,0x0f,0x47,0x00,0x75,0x0f,0x47,0x00,0x97,0x0f,0x47,0x00,0xbb,0x0f,0x47,0x00,0xc9,0x0f,0x47,0x00,0x57,0x20,0x47,0x00,0xd7,0x0f,0x47,0x00,0x01,0x10,0x47,0x00,0x19,0x10,0x47,0x00,0x27,0x22,0x47,0x00,0xe5,0x0f,0x47,0x00,0x29,0x10,0x47,0x00,0x36,0x10,0x47,0x00,0x32,0x17,0x47,0x00,0x4c,0x24,0x47,0x00,0x11,0x0d,0x47,0x00,0x34,0x0d,0x47,0x00,0x50,0x0d,0x47,0x00,0xea,0x0d,0x47,0x00,0xf6,0x0d,0x47,0x00,0x02,0x0e,0x47,0x00,0x0e,0x0e,0x47,0x00,0x56,0x0e,0x47,0x00,0x62,0x0e,0x47,0x00,0x6e,0x0e,0x47,0x00,0x7a,0x0e,0x47,0x00,0xce,0x11,0x47,0x00,0x72,0x22,0x47,0x00,0xc8,0x22,0x47,0x00,0xc6,0x0d,0x47,0x00,0xde,0x0d,0x47,0x00,0xe1,0x22,0x47,0x00,0x8f,0x14,0x47,0x00,0x40,0x14,0x47,0x00,0x86,0x0e,0x47,0x00,0x92,0x0e,0x47,0x00,0x9e,0x0e,0x47,0x00,0xf3,0x0f,0x47,0x00,0x3f,0x13,0x47,0x00,0xbc,0x13,0x47,0x00,0xfd,0x22,0x47,0x00,0xef,0x22,0x47,0x00,0x4c,0x10,0x47,0x00,0x7a,0x20,0x47,0x00,0x08,0x21,0x47,0x00,0x1e,0x0f,0x47,0x00,0x26,0x0d,0x47,0x00,0x1a,0x0e,0x47,0x00,0x3e,0x0e,0x47,0x00,0x4a,0x0e,0x47,0x00,0xcb,0x0c,0x47,0x00,0xf6,0x1a,0x47,0x00,0xbd,0x0b,0x47,0x00,0x3a,0x18,0x47,0x00,0xef,0x10,0x47,0x00,0x2b,0x1b,0x47,0x00,0x19,0x24,0x47,0x00,0x8b,0x0d,0x47,0x00,0xd2,0x0d,0x47,0x00,0x61,0x23,0x47,0x00,0xa3,0x23,0x47,0x00,0xe5,0x23,0x47,0x00,0x42,0x0d,0x47,0x00,0xf1,0x1d,0x47,0x00,0xb8,0x1b,0x47,0x00,0xaf,0x0b,0x47,0x00,0x05,0x1c,0x47,0x00,0xa7,0x1d,0x47,0x00,0x52,0x1c,0x47,0x00,0x6b,0x1b,0x47,0x00,0xaa,0x0e,0x47,0x00,0x26,0x0e,0x47,0x00,0x32,0x0e,0x47,0x00,0xaa,0x1c,0x47,0x00,0x02,0x1d,0x47,0x00,0x5a,0x1d,0x47,0x00,0xea,0x24,0x47,0x00};
static const uint8_t mi_rom_004726a0[]={0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x04,0x6a,0x6a,0x09,0x0a,0x0a,0x0a,0x09,0x09,0x0b,0x0c,0x6a,0x0d,0x0e,0x0f,0x6a,0x10,0x6a,0x11,0x6a,0x6a,0x6a,0x6a,0x12,0x13,0x14,0x15,0x16,0x17,0x17,0x17,0x17,0x17,0x18,0x19,0x1a,0x6a,0x1b,0x1c,0x1d,0x1e,0x1f,0x20,0x21,0x22,0x6a,0x6a,0x23,0x23,0x24,0x6a,0x25,0x26,0x27,0x6a,0x28,0x29,0x2a,0x2b,0x6a,0x6a,0x6a,0x2c,0x2d,0x2d,0x2d,0x2d,0x2d,0x2d,0x2e,0x2f,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x6a,0x6a,0x6a,0x6a,0x6a,0x6a,0x6a,0x6a,0x6a,0x6a,0x6a,0x6a,0x6a,0x6a,0x6a,0x6a,0x6a,0x6a,0x6a,0x6a,0x38,0x6a,0x39,0x6a,0x3a,0x3a,0x3a,0x3a,0x3a,0x3a,0x3a,0x3a,0x3a,0x3a,0x6a,0x12,0x12,0x12,0x12,0x12,0x17,0x17,0x17,0x17,0x17,0x3b,0x3c,0x6a,0x6a,0x6a,0x6a,0x3d,0x6a,0x6a,0x6a,0x3e,0x6a,0x3f,0x6a,0x6a,0x6a,0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x48,0x23,0x49,0x4a,0x4b,0x19,0x1c,0x1a,0x2d,0x2d,0x2d,0x6a,0x6a,0x4c,0x6a,0x6a,0x3e,0x4d,0x4e,0x4f,0x50,0x51,0x51,0x52,0x53,0x54,0x54,0x55,0x6a,0x56,0x57,0x58,0x59,0x5a,0x5b,0x5c,0x5d,0x6a,0x2d,0x5e,0x5f,0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69};
static __inline uint32_t mi_read(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x004724f4u&&width<=428u&&(offset=address-0x004724f4u)<=428u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004724f4[offset+k]<<(k*8u);return result;
 }
 if(address>=0x004726a0u&&width<=227u&&(offset=address-0x004726a0u)<=227u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004726a0[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}

void restore_C_00470990(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00470990 */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 00470993 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470994 */
    f->r[3]=mi_read((0x00000078u+f->r[4]),4u);
    /* 00470998 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470999 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047099a */
    f->r[6]=mi_read((0x00000084u+f->r[4]),4u);
    /* 004709a1 */
    a=f->r[3];b=0x00000039u;t=mi_alu(f,a,b,32,1);
    /* 004709a4 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004709a5 */
    if(!(f->flags&MI_ZF))goto mi_004709cf;
    /* 004709a7 */
    f->r[0]=f->r[6];
    /* 004709a9 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004709ac */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004709ae */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x007999eau+f->r[0]*8u),2u))&0xffffu)<<0);
    /* 004709b6 */
    a=(f->r[0]&0xffffu);b=0x0000000du;t=mi_alu(f,a,b,16,1);
    /* 004709ba */
    if(!(f->flags&MI_ZF))goto mi_004709c3;
    /* 004709bc */
    f->r[3]=0x00000028u;
    /* 004709c1 */
    goto mi_004709cf;
mi_004709c3:;
    /* 004709c3 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004709c5 */
    a=(f->r[0]&0xffffu);b=0x0000000bu;t=mi_alu(f,a,b,16,1);
    /* 004709c9 */
    t=(!(f->flags&MI_ZF))?1u:0u;f->r[3]=(f->r[3]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004709cc */
    a=f->r[3];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[3]=t;
mi_004709cf:;
    /* 004709cf */
    f->r[0]=(0xfffffff6u+f->r[3]);
    /* 004709d2 */
    a=f->r[0];b=0x000000e2u;t=mi_alu(f,a,b,32,1);
    /* 004709d7 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004724ea;
    /* 004709dd */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004709df */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x004726a0u+f->r[0]),1u))&0xffu)<<0);
    /* 004709e5 */
    switch(mi_read((0x004724f4u+f->r[1]*4u),4u)){case 0x004709ecu:goto mi_004709ec;case 0x00470a1bu:goto mi_00470a1b;case 0x00470a5cu:goto mi_00470a5c;case 0x00470ad1u:goto mi_00470ad1;case 0x00470b3bu:goto mi_00470b3b;case 0x00470ba1u:goto mi_00470ba1;case 0x00470bafu:goto mi_00470baf;case 0x00470bbdu:goto mi_00470bbd;case 0x00470bf7u:goto mi_00470bf7;case 0x00470c05u:goto mi_00470c05;case 0x00470c13u:goto mi_00470c13;case 0x00470c21u:goto mi_00470c21;case 0x00470cafu:goto mi_00470caf;case 0x00470cbdu:goto mi_00470cbd;case 0x00470ccbu:goto mi_00470ccb;case 0x00470cd9u:goto mi_00470cd9;case 0x00470ce7u:goto mi_00470ce7;case 0x00470cf5u:goto mi_00470cf5;case 0x00470d03u:goto mi_00470d03;case 0x00470d11u:goto mi_00470d11;case 0x00470d26u:goto mi_00470d26;case 0x00470d34u:goto mi_00470d34;case 0x00470d42u:goto mi_00470d42;case 0x00470d50u:goto mi_00470d50;case 0x00470d8bu:goto mi_00470d8b;case 0x00470dc6u:goto mi_00470dc6;case 0x00470dd2u:goto mi_00470dd2;case 0x00470ddeu:goto mi_00470dde;case 0x00470deau:goto mi_00470dea;case 0x00470df6u:goto mi_00470df6;case 0x00470e02u:goto mi_00470e02;case 0x00470e0eu:goto mi_00470e0e;case 0x00470e1au:goto mi_00470e1a;case 0x00470e26u:goto mi_00470e26;case 0x00470e32u:goto mi_00470e32;case 0x00470e3eu:goto mi_00470e3e;case 0x00470e4au:goto mi_00470e4a;case 0x00470e56u:goto mi_00470e56;case 0x00470e62u:goto mi_00470e62;case 0x00470e6eu:goto mi_00470e6e;case 0x00470e7au:goto mi_00470e7a;case 0x00470e86u:goto mi_00470e86;case 0x00470e92u:goto mi_00470e92;case 0x00470e9eu:goto mi_00470e9e;case 0x00470eaau:goto mi_00470eaa;case 0x00470eb6u:goto mi_00470eb6;case 0x00470eebu:goto mi_00470eeb;case 0x00470f1eu:goto mi_00470f1e;case 0x00470f53u:goto mi_00470f53;case 0x00470f75u:goto mi_00470f75;case 0x00470f97u:goto mi_00470f97;case 0x00470fbbu:goto mi_00470fbb;case 0x00470fc9u:goto mi_00470fc9;case 0x00470fd7u:goto mi_00470fd7;case 0x00470fe5u:goto mi_00470fe5;case 0x00470ff3u:goto mi_00470ff3;case 0x00471001u:goto mi_00471001;case 0x00471019u:goto mi_00471019;case 0x00471029u:goto mi_00471029;case 0x00471036u:goto mi_00471036;case 0x0047104cu:goto mi_0047104c;case 0x0047105au:goto mi_0047105a;case 0x00471068u:goto mi_00471068;case 0x00471076u:goto mi_00471076;case 0x00471084u:goto mi_00471084;case 0x00471092u:goto mi_00471092;case 0x004710a0u:goto mi_004710a0;case 0x004710efu:goto mi_004710ef;case 0x00471152u:goto mi_00471152;case 0x004711ceu:goto mi_004711ce;case 0x004712c2u:goto mi_004712c2;case 0x0047133fu:goto mi_0047133f;case 0x004713bcu:goto mi_004713bc;case 0x00471440u:goto mi_00471440;case 0x0047148fu:goto mi_0047148f;case 0x004714a0u:goto mi_004714a0;case 0x00471732u:goto mi_00471732;case 0x004717abu:goto mi_004717ab;case 0x0047183au:goto mi_0047183a;case 0x00471af6u:goto mi_00471af6;case 0x00471b2bu:goto mi_00471b2b;case 0x00471b6bu:goto mi_00471b6b;case 0x00471bb8u:goto mi_00471bb8;case 0x00471c05u:goto mi_00471c05;case 0x00471c52u:goto mi_00471c52;case 0x00471caau:goto mi_00471caa;case 0x00471d02u:goto mi_00471d02;case 0x00471d5au:goto mi_00471d5a;case 0x00471da7u:goto mi_00471da7;case 0x00471df1u:goto mi_00471df1;case 0x00471e5cu:goto mi_00471e5c;case 0x00472057u:goto mi_00472057;case 0x0047207au:goto mi_0047207a;case 0x00472108u:goto mi_00472108;case 0x00472196u:goto mi_00472196;case 0x00472227u:goto mi_00472227;case 0x00472272u:goto mi_00472272;case 0x004722c8u:goto mi_004722c8;case 0x004722e1u:goto mi_004722e1;case 0x004722efu:goto mi_004722ef;case 0x004722fdu:goto mi_004722fd;case 0x00472361u:goto mi_00472361;case 0x004723a3u:goto mi_004723a3;case 0x004723e5u:goto mi_004723e5;case 0x00472419u:goto mi_00472419;case 0x0047244cu:goto mi_0047244c;case 0x004724eau:goto mi_004724ea;default:abort();}
mi_004709ec:;
    /* 004709ec */
    f->r[0]=f->r[6];
    /* 004709ee */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004709f1 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004709f3 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004709f6 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00799a5fu+f->r[0]),1u))&0xffu)<<0);
    /* 004709fc */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004709fe */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00470a4a;
    /* 00470a00 */
    a=mi_read((0x007999f4u+f->r[0]),2u);b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 00470a08 */
    if(!(f->flags&MI_ZF))goto mi_00470a4a;
    /* 00470a0a */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470a0b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00470a10u;f->boundary(0x00470a0bu,0x0046cf40u,f);
    /* 00470a10 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00470a13 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00470a14 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00470a15 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00470a16 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00470a17 */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00470a1a */
    f->r[4]+=4;return;
mi_00470a1b:;
    /* 00470a1b */
    f->r[0]=f->r[6];
    /* 00470a1d */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00470a20 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00470a22 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00470a25 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00799a5fu+f->r[0]),1u))&0xffu)<<0);
    /* 00470a2b */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00470a2d */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00470a4a;
    /* 00470a2f */
    a=mi_read((0x007999f4u+f->r[0]),2u);b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 00470a37 */
    if(!(f->flags&MI_ZF))goto mi_00470a4a;
    /* 00470a39 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470a3a */
    f->r[4]-=4;MI_U32(f->r[4])=0x00470a3fu;f->boundary(0x00470a3au,0x0046cf40u,f);
    /* 00470a3f */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00470a42 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00470a43 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00470a44 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00470a45 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00470a46 */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00470a49 */
    f->r[4]+=4;return;
mi_00470a4a:;
    /* 00470a4a */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470a4b */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470a4c */
    f->r[4]-=4;MI_U32(f->r[4])=0x00470a51u;f->boundary(0x00470a4cu,0x0046cea0u,f);
    /* 00470a51 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00470a54 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00470a55 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00470a56 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00470a57 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00470a58 */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00470a5b */
    f->r[4]+=4;return;
mi_00470a5c:;
    /* 00470a5c */
    f->r[7]=f->r[6];
    /* 00470a5e */
    a=f->r[7];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 00470a61 */
    a=f->r[7];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 00470a63 */
    a=f->r[7];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 00470a66 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00799a12u+f->r[7]),2u);
    /* 00470a6d */
    f->r[2]=f->r[0];
    /* 00470a6f */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 00470a72 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00470a74 */
    a=mi_read((0x007999e8u+f->r[2]*8u),2u);b=0x0000001eu;t=mi_alu(f,a,b,16,1);
    /* 00470a7d */
    if(!(f->flags&MI_ZF))goto mi_00470a98;
    /* 00470a7f */
    f->r[0]=mi_read((0x00000090u+f->r[4]),4u);
    /* 00470a86 */
    f->r[1]=mi_read((0x0000008cu+f->r[4]),4u);
    /* 00470a8d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470a8e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470a8f */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470a90 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00470a95u;f->boundary(0x00470a90u,0x00440540u,f);
    /* 00470a95 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00470a98:;
    /* 00470a98 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470a99 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00470a9eu;f->boundary(0x00470a99u,0x00475b50u,f);
    /* 00470a9e */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00470aa1 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 00470aa3 */
    if(!(f->flags&MI_ZF))goto mi_004724ea;
    /* 00470aa9 */
    f->r[0]=mi_read((0x00799ab0u+f->r[7]),4u);
    /* 00470aaf */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00470ab1 */
    if((f->flags&MI_ZF))goto mi_004724ea;
    /* 00470ab7 */
    f->r[2]=mi_read((0x00799aa8u+f->r[7]),4u);
    /* 00470abd */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470abe */
    a=0x0000001eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470ac0 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470ac1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00470ac6u;f->boundary(0x00470ac1u,0x00440060u,f);
    /* 00470ac6 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00470ac9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00470aca */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00470acb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00470acc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00470acd */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00470ad0 */
    f->r[4]+=4;return;
mi_00470ad1:;
    /* 00470ad1 */
    f->r[0]=mi_read((0x00f567acu),4u);
    /* 00470ad6 */
    f->r[1]=mi_read((0x00000090u+f->r[4]),4u);
    /* 00470add */
    f->r[2]=mi_read((0x0000008cu+f->r[4]),4u);
    /* 00470ae4 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470ae6 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470ae7 */
    f->r[0]=mi_read((0x0000009cu+f->r[4]),4u);
    /* 00470aee */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470aef */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470af0 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470af1 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470af2 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470af3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00470af8u;f->boundary(0x00470af3u,0x0047bf50u,f);
    /* 00470af8 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470af9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00470afeu;f->boundary(0x00470af9u,0x00475b50u,f);
    /* 00470afe */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00470b01 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 00470b03 */
    if(!(f->flags&MI_ZF))goto mi_004724ea;
    /* 00470b09 */
    f->r[0]=f->r[6];
    /* 00470b0b */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00470b0e */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00470b10 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00470b13 */
    f->r[1]=mi_read((0x00799ab0u+f->r[0]),4u);
    /* 00470b19 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00470b1b */
    if((f->flags&MI_ZF))goto mi_004724ea;
    /* 00470b21 */
    f->r[0]=mi_read((0x00799aa8u+f->r[0]),4u);
    /* 00470b27 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470b28 */
    a=0x0000001fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470b2a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470b2b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00470b30u;f->boundary(0x00470b2bu,0x00440060u,f);
    /* 00470b30 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00470b33 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00470b34 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00470b35 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00470b36 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00470b37 */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00470b3a */
    f->r[4]+=4;return;
mi_00470b3b:;
    /* 00470b3b */
    f->r[1]=mi_read((0x00f5659cu),4u);
    /* 00470b41 */
    f->r[2]=mi_read((0x00000090u+f->r[4]),4u);
    /* 00470b48 */
    f->r[0]=mi_read((0x0000008cu+f->r[4]),4u);
    /* 00470b4f */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470b51 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470b52 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470b54 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470b56 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470b57 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470b58 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470b59 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00470b5eu;f->boundary(0x00470b59u,0x0047bf50u,f);
    /* 00470b5e */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470b5f */
    f->r[4]-=4;MI_U32(f->r[4])=0x00470b64u;f->boundary(0x00470b5fu,0x00475b50u,f);
    /* 00470b64 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00470b67 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 00470b69 */
    if(!(f->flags&MI_ZF))goto mi_004724ea;
    /* 00470b6f */
    f->r[0]=f->r[6];
    /* 00470b71 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00470b74 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00470b76 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00470b79 */
    f->r[1]=mi_read((0x00799ab0u+f->r[0]),4u);
    /* 00470b7f */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00470b81 */
    if((f->flags&MI_ZF))goto mi_004724ea;
    /* 00470b87 */
    f->r[1]=mi_read((0x00799aa8u+f->r[0]),4u);
    /* 00470b8d */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470b8e */
    a=0x00000021u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470b90 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470b91 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00470b96u;f->boundary(0x00470b91u,0x00440060u,f);
    /* 00470b96 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00470b99 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00470b9a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00470b9b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00470b9c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00470b9d */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00470ba0 */
    f->r[4]+=4;return;
mi_00470ba1:;
    /* 00470ba1 */
    f->r[2]=mi_read((0x00f564a4u),4u);
    /* 00470ba7 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470ba9 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470baa */
    goto mi_004723bb;
mi_00470baf:;
    /* 00470baf */
    f->r[2]=mi_read((0x00f57214u),4u);
    /* 00470bb5 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470bb7 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470bb8 */
    goto mi_004723bb;
mi_00470bbd:;
    /* 00470bbd */
    f->r[2]=f->r[6];
    /* 00470bbf */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470bc0 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 00470bc3 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00470bc5 */
    a=0x000000d1u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470bca */
    f->r[0]=mi_read((0x00799aa8u+f->r[2]*8u),4u);
    /* 00470bd1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470bd2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00470bd7u;f->boundary(0x00470bd2u,0x00440060u,f);
    /* 00470bd7 */
    f->r[1]=mi_read((0x00eaac28u),4u);
    /* 00470bdd */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00470be0 */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 00470be6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00470be7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00470be8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00470be9 */
    MI_U16((0x00ea6084u+f->r[1]))=(uint16_t)(0x00000001u);
    /* 00470bf2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00470bf3 */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00470bf6 */
    f->r[4]+=4;return;
mi_00470bf7:;
    /* 00470bf7 */
    f->r[2]=mi_read((0x00f565b4u),4u);
    /* 00470bfd */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470bff */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470c00 */
    goto mi_004723bb;
mi_00470c05:;
    /* 00470c05 */
    f->r[2]=mi_read((0x00f565bcu),4u);
    /* 00470c0b */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470c0d */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470c0e */
    goto mi_004723bb;
mi_00470c13:;
    /* 00470c13 */
    f->r[2]=mi_read((0x00f5658cu),4u);
    /* 00470c19 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470c1b */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470c1c */
    goto mi_004723bb;
mi_00470c21:;
    /* 00470c21 */
    f->r[0]=mi_read((0x00bca2e0u),4u);
    /* 00470c26 */
    f->r[2]=f->r[6];
    /* 00470c28 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 00470c2b */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00470c2d */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 00470c2f */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00799a82u+f->r[2]*8u),1u))&0xffu)<<0);
    /* 00470c36 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 00470c37 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00470c39 */
    f->r[7]=f->r[1];
    /* 00470c3b */
    a=f->r[1];b=0x0000000fu;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 00470c3e */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 00470c40 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00470c42 */
    a=f->r[7];t=a>>4;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3)&1u)|mi_szp(t,32);f->r[7]=t;
    /* 00470c45 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00470c46 */
    if(!(f->flags&MI_SF))goto mi_00470c4b;
    /* 00470c48 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
mi_00470c4b:;
    /* 00470c4b */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 00470c4e */
    if(!mi_signed_less(f->flags))goto mi_00470c6d;
    /* 00470c50 */
    f->r[0]=(0x00000000u+f->r[3]+f->r[3]*4u);
    /* 00470c53 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470c55 */
    f->r[2]=mi_read((0x005d06e4u+f->r[0]*4u),4u);
    /* 00470c5c */
    f->r[0]=mi_read((0x00f55bb4u+f->r[2]*8u),4u);
    /* 00470c63 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[7]*4u);
    /* 00470c66 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00470c68 */
    goto mi_004723ba;
mi_00470c6d:;
    /* 00470c6d */
    a=f->r[3];b=0x00000029u;t=mi_alu(f,a,b,32,1);
    /* 00470c70 */
    if(!(f->flags&MI_ZF))goto mi_00470c85;
    /* 00470c72 */
    f->r[2]=mi_read((0x00f5730cu),4u);
    /* 00470c78 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470c7a */
    f->r[0]=(0xfffffff8u+f->r[2]+f->r[7]*4u);
    /* 00470c7e */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00470c80 */
    goto mi_004723ba;
mi_00470c85:;
    /* 00470c85 */
    a=f->r[3];b=0x0000002au;t=mi_alu(f,a,b,32,1);
    /* 00470c88 */
    if(!(f->flags&MI_ZF))goto mi_00470c9c;
    /* 00470c8a */
    f->r[0]=mi_read((0x00f57304u),4u);
    /* 00470c8f */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470c91 */
    f->r[0]=(0xfffffff8u+f->r[0]+f->r[7]*4u);
    /* 00470c95 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00470c97 */
    goto mi_004723ba;
mi_00470c9c:;
    /* 00470c9c */
    f->r[2]=mi_read((0x00f572fcu),4u);
    /* 00470ca2 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470ca4 */
    f->r[0]=(0xfffffff8u+f->r[2]+f->r[7]*4u);
    /* 00470ca8 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00470caa */
    goto mi_004723ba;
mi_00470caf:;
    /* 00470caf */
    f->r[2]=mi_read((0x00f5663cu),4u);
    /* 00470cb5 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470cb7 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470cb8 */
    goto mi_004723bb;
mi_00470cbd:;
    /* 00470cbd */
    f->r[2]=mi_read((0x00f56994u),4u);
    /* 00470cc3 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470cc5 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470cc6 */
    goto mi_004723bb;
mi_00470ccb:;
    /* 00470ccb */
    f->r[2]=mi_read((0x00f5664cu),4u);
    /* 00470cd1 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470cd3 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470cd4 */
    goto mi_004723bb;
mi_00470cd9:;
    /* 00470cd9 */
    f->r[2]=mi_read((0x00f56634u),4u);
    /* 00470cdf */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470ce1 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470ce2 */
    goto mi_004723bb;
mi_00470ce7:;
    /* 00470ce7 */
    f->r[2]=mi_read((0x00f5656cu),4u);
    /* 00470ced */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470cef */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470cf0 */
    goto mi_004723bb;
mi_00470cf5:;
    /* 00470cf5 */
    f->r[2]=mi_read((0x00f56574u),4u);
    /* 00470cfb */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470cfd */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470cfe */
    goto mi_004723bb;
mi_00470d03:;
    /* 00470d03 */
    f->r[2]=mi_read((0x00f5661cu),4u);
    /* 00470d09 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470d0b */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470d0c */
    goto mi_004723bb;
mi_00470d11:;
    /* 00470d11 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470d13 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470d14 */
    f->r[1]=0x00bca350u;
    /* 00470d19 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00470d1eu;f->boundary(0x00470d19u,0x00488e60u,f);
    /* 00470d1e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00470d1f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00470d20 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00470d21 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00470d22 */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00470d25 */
    f->r[4]+=4;return;
mi_00470d26:;
    /* 00470d26 */
    f->r[2]=mi_read((0x00f56684u),4u);
    /* 00470d2c */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470d2e */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470d2f */
    goto mi_004723bb;
mi_00470d34:;
    /* 00470d34 */
    f->r[2]=mi_read((0x00f569f4u),4u);
    /* 00470d3a */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470d3c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470d3d */
    goto mi_004723bb;
mi_00470d42:;
    /* 00470d42 */
    f->r[2]=mi_read((0x00f56a44u),4u);
    /* 00470d48 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470d4a */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470d4b */
    goto mi_004723bb;
mi_00470d50:;
    /* 00470d50 */
    a=0x00000019u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470d52 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00470d57u;f->boundary(0x00470d52u,0x00451bc0u,f);
    /* 00470d57 */
    f->r[0]=mi_read((0x00f55bb4u+f->r[0]*8u),4u);
    /* 00470d5e */
    f->r[2]=mi_read((0x00000094u+f->r[4]),4u);
    /* 00470d65 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470d67 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470d68 */
    f->r[0]=mi_read((0x000000a0u+f->r[4]),4u);
    /* 00470d6f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470d70 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470d71 */
    f->r[0]=mi_read((0x000000a0u+f->r[4]),4u);
    /* 00470d78 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470d79 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470d7a */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470d7b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00470d80u;f->boundary(0x00470d7bu,0x0047bf50u,f);
    /* 00470d80 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00470d83 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00470d84 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00470d85 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00470d86 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00470d87 */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00470d8a */
    f->r[4]+=4;return;
mi_00470d8b:;
    /* 00470d8b */
    a=0x0000001au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470d8d */
    f->r[4]-=4;MI_U32(f->r[4])=0x00470d92u;f->boundary(0x00470d8du,0x00451bc0u,f);
    /* 00470d92 */
    f->r[0]=mi_read((0x00f55bb4u+f->r[0]*8u),4u);
    /* 00470d99 */
    f->r[1]=mi_read((0x00000094u+f->r[4]),4u);
    /* 00470da0 */
    f->r[2]=mi_read((0x00000090u+f->r[4]),4u);
    /* 00470da7 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470da9 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470daa */
    f->r[0]=mi_read((0x000000a0u+f->r[4]),4u);
    /* 00470db1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470db2 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470db3 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470db4 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470db5 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470db6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00470dbbu;f->boundary(0x00470db6u,0x0047bf50u,f);
    /* 00470dbb */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00470dbe */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00470dbf */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00470dc0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00470dc1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00470dc2 */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00470dc5 */
    f->r[4]+=4;return;
mi_00470dc6:;
    /* 00470dc6 */
    f->r[0]=mi_read((0x00f569e4u),4u);
    /* 00470dcb */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470dcd */
    goto mi_00472378;
mi_00470dd2:;
    /* 00470dd2 */
    f->r[0]=mi_read((0x00f56a3cu),4u);
    /* 00470dd7 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470dd9 */
    goto mi_00472378;
mi_00470dde:;
    /* 00470dde */
    f->r[0]=mi_read((0x00f569fcu),4u);
    /* 00470de3 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470de5 */
    goto mi_00472378;
mi_00470dea:;
    /* 00470dea */
    f->r[0]=mi_read((0x00f56624u),4u);
    /* 00470def */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470df1 */
    goto mi_00472378;
mi_00470df6:;
    /* 00470df6 */
    f->r[0]=mi_read((0x00f5655cu),4u);
    /* 00470dfb */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470dfd */
    goto mi_00472378;
mi_00470e02:;
    /* 00470e02 */
    f->r[0]=mi_read((0x00f567bcu),4u);
    /* 00470e07 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470e09 */
    goto mi_00472378;
mi_00470e0e:;
    /* 00470e0e */
    f->r[0]=mi_read((0x00f567ecu),4u);
    /* 00470e13 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470e15 */
    goto mi_00472378;
mi_00470e1a:;
    /* 00470e1a */
    f->r[0]=mi_read((0x00f5657cu),4u);
    /* 00470e1f */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470e21 */
    goto mi_00472378;
mi_00470e26:;
    /* 00470e26 */
    f->r[0]=mi_read((0x00f572b4u),4u);
    /* 00470e2b */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470e2d */
    goto mi_00472378;
mi_00470e32:;
    /* 00470e32 */
    f->r[0]=mi_read((0x00f572bcu),4u);
    /* 00470e37 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470e39 */
    goto mi_00472378;
mi_00470e3e:;
    /* 00470e3e */
    f->r[0]=mi_read((0x00f567fcu),4u);
    /* 00470e43 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470e45 */
    goto mi_00472378;
mi_00470e4a:;
    /* 00470e4a */
    f->r[0]=mi_read((0x00f567dcu),4u);
    /* 00470e4f */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470e51 */
    goto mi_00472378;
mi_00470e56:;
    /* 00470e56 */
    f->r[0]=mi_read((0x00f567f4u),4u);
    /* 00470e5b */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470e5d */
    goto mi_00472378;
mi_00470e62:;
    /* 00470e62 */
    f->r[0]=mi_read((0x00f567d4u),4u);
    /* 00470e67 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470e69 */
    goto mi_00472378;
mi_00470e6e:;
    /* 00470e6e */
    f->r[0]=mi_read((0x00f56804u),4u);
    /* 00470e73 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470e75 */
    goto mi_00472378;
mi_00470e7a:;
    /* 00470e7a */
    f->r[0]=mi_read((0x00f5699cu),4u);
    /* 00470e7f */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470e81 */
    goto mi_00472378;
mi_00470e86:;
    /* 00470e86 */
    f->r[0]=mi_read((0x00f566f4u),4u);
    /* 00470e8b */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470e8d */
    goto mi_00472378;
mi_00470e92:;
    /* 00470e92 */
    f->r[0]=mi_read((0x00f565e4u),4u);
    /* 00470e97 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470e99 */
    goto mi_00472378;
mi_00470e9e:;
    /* 00470e9e */
    f->r[0]=mi_read((0x00f566dcu),4u);
    /* 00470ea3 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470ea5 */
    goto mi_00472378;
mi_00470eaa:;
    /* 00470eaa */
    f->r[0]=mi_read((0x00f572dcu),4u);
    /* 00470eaf */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470eb1 */
    goto mi_00472378;
mi_00470eb6:;
    /* 00470eb6 */
    f->r[0]=mi_read((0x00000098u+f->r[4]),4u);
    /* 00470ebd */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00470ebf */
    if((f->flags&MI_ZF))goto mi_00470edd;
    /* 00470ec1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00470ec6u;f->boundary(0x00470ec1u,0x005596d0u,f);
    /* 00470ec6 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470ec7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00470eccu;f->boundary(0x00470ec7u,0x0046b100u,f);
    /* 00470ecc */
    f->r[1]=f->r[6];
    /* 00470ece */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00470ed1 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00470ed4 */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00470ed6 */
    MI_U8((0x00799a99u+f->r[1]*8u))=(uint8_t)((f->r[0]&0xffu));
mi_00470edd:;
    /* 00470edd */
    f->r[2]=mi_read((0x00f56734u),4u);
    /* 00470ee3 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470ee5 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470ee6 */
    goto mi_004723bb;
mi_00470eeb:;
    /* 00470eeb */
    f->r[0]=mi_read((0x00000098u+f->r[4]),4u);
    /* 00470ef2 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00470ef4 */
    if((f->flags&MI_ZF))goto mi_00470f12;
    /* 00470ef6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00470efbu;f->boundary(0x00470ef6u,0x005596d0u,f);
    /* 00470efb */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470efc */
    f->r[4]-=4;MI_U32(f->r[4])=0x00470f01u;f->boundary(0x00470efcu,0x0046b100u,f);
    /* 00470f01 */
    f->r[2]=f->r[6];
    /* 00470f03 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00470f06 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 00470f09 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00470f0b */
    MI_U8((0x00799a99u+f->r[2]*8u))=(uint8_t)((f->r[0]&0xffu));
mi_00470f12:;
    /* 00470f12 */
    f->r[0]=mi_read((0x00f564ccu),4u);
    /* 00470f17 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470f19 */
    goto mi_00472378;
mi_00470f1e:;
    /* 00470f1e */
    f->r[0]=mi_read((0x00000098u+f->r[4]),4u);
    /* 00470f25 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00470f27 */
    if((f->flags&MI_ZF))goto mi_00470f45;
    /* 00470f29 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00470f2eu;f->boundary(0x00470f29u,0x005596d0u,f);
    /* 00470f2e */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470f2f */
    f->r[4]-=4;MI_U32(f->r[4])=0x00470f34u;f->boundary(0x00470f2fu,0x0046b100u,f);
    /* 00470f34 */
    f->r[1]=f->r[6];
    /* 00470f36 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00470f39 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00470f3c */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00470f3e */
    MI_U8((0x00799a99u+f->r[1]*8u))=(uint8_t)((f->r[0]&0xffu));
mi_00470f45:;
    /* 00470f45 */
    f->r[2]=mi_read((0x00f564ccu),4u);
    /* 00470f4b */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470f4d */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470f4e */
    goto mi_004723bb;
mi_00470f53:;
    /* 00470f53 */
    f->r[2]=mi_read((0x00eaac28u),4u);
    /* 00470f59 */
    f->r[0]=mi_read((0x00f566c4u),4u);
    /* 00470f5e */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 00470f64 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470f66 */
    MI_U32((0x00ea620cu+f->r[2]))=(uint32_t)(0x00000001u);
    /* 00470f70 */
    goto mi_00472378;
mi_00470f75:;
    /* 00470f75 */
    f->r[0]=mi_read((0x00eaac28u),4u);
    /* 00470f7a */
    f->r[1]=mi_read((0x00f566ccu),4u);
    /* 00470f80 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 00470f86 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470f88 */
    MI_U32((0x00ea620cu+f->r[0]))=(uint32_t)(0x00000001u);
    /* 00470f92 */
    goto mi_00472421;
mi_00470f97:;
    /* 00470f97 */
    f->r[1]=mi_read((0x00eaac28u),4u);
    /* 00470f9d */
    f->r[2]=mi_read((0x00f566d4u),4u);
    /* 00470fa3 */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 00470fa9 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470fab */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470fac */
    MI_U32((0x00ea620cu+f->r[1]))=(uint32_t)(0x00000001u);
    /* 00470fb6 */
    goto mi_004723bb;
mi_00470fbb:;
    /* 00470fbb */
    f->r[2]=mi_read((0x00f569dcu),4u);
    /* 00470fc1 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470fc3 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470fc4 */
    goto mi_004723bb;
mi_00470fc9:;
    /* 00470fc9 */
    f->r[2]=mi_read((0x00f566a4u),4u);
    /* 00470fcf */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470fd1 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470fd2 */
    goto mi_004723bb;
mi_00470fd7:;
    /* 00470fd7 */
    f->r[2]=mi_read((0x00f56614u),4u);
    /* 00470fdd */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470fdf */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470fe0 */
    goto mi_004723bb;
mi_00470fe5:;
    /* 00470fe5 */
    f->r[2]=mi_read((0x00f564d4u),4u);
    /* 00470feb */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470fed */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470fee */
    goto mi_004723bb;
mi_00470ff3:;
    /* 00470ff3 */
    f->r[2]=mi_read((0x00f56644u),4u);
    /* 00470ff9 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470ffb */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470ffc */
    goto mi_004723bb;
mi_00471001:;
    /* 00471001 */
    f->r[7]=mi_read((0x00f569d4u),4u);
    /* 00471007 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471009 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047100eu;f->boundary(0x00471009u,0x00562348u,f);
    /* 0047100e */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 00471011 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00471013 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471014 */
    goto mi_00472422;
mi_00471019:;
    /* 00471019 */
    f->r[1]=mi_read((0x00f569d4u),4u);
    /* 0047101f */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471021 */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00471024 */
    goto mi_00472421;
mi_00471029:;
    /* 00471029 */
    f->r[1]=mi_read((0x00f565dcu),4u);
    /* 0047102f */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471031 */
    goto mi_00472421;
mi_00471036:;
    /* 00471036 */
    f->r[1]=mi_read((0x00eaac28u),4u);
    /* 0047103c */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 00471042 */
    MI_U32((0x00ea6240u+f->r[1]))=(uint32_t)(0x00000001u);
mi_0047104c:;
    /* 0047104c */
    f->r[2]=mi_read((0x00f56984u),4u);
    /* 00471052 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471054 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471055 */
    goto mi_004723bb;
mi_0047105a:;
    /* 0047105a */
    f->r[2]=mi_read((0x00f564e4u),4u);
    /* 00471060 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471062 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471063 */
    goto mi_004723bb;
mi_00471068:;
    /* 00471068 */
    f->r[2]=mi_read((0x00f564b4u),4u);
    /* 0047106e */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471070 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471071 */
    goto mi_004723bb;
mi_00471076:;
    /* 00471076 */
    f->r[2]=mi_read((0x00f564bcu),4u);
    /* 0047107c */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047107e */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047107f */
    goto mi_004723bb;
mi_00471084:;
    /* 00471084 */
    f->r[2]=mi_read((0x00f564c4u),4u);
    /* 0047108a */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047108c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047108d */
    goto mi_004723bb;
mi_00471092:;
    /* 00471092 */
    f->r[2]=mi_read((0x00f5667cu),4u);
    /* 00471098 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047109a */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047109b */
    goto mi_004723bb;
mi_004710a0:;
    /* 004710a0 */
    a=0x0000000au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004710a2 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004710a3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004710a8u;f->boundary(0x004710a3u,0x004c5120u,f);
    /* 004710a8 */
    f->r[2]=mi_read((0x005e018cu+f->r[0]*4u),4u);
    /* 004710af */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004710b2 */
    f->r[1]=0x00f55bb0u;
    /* 004710b7 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004710b8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004710bdu;f->boundary(0x004710b8u,0x0053dd90u,f);
    /* 004710bd */
    f->r[0]=mi_read((0x00000098u+f->r[4]),4u);
    /* 004710c4 */
    f->r[1]=mi_read((0x00000080u+f->r[4]),4u);
    /* 004710cb */
    f->r[2]=mi_read((0x00000090u+f->r[4]),4u);
    /* 004710d2 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004710d3 */
    f->r[0]=mi_read((0x00000090u+f->r[4]),4u);
    /* 004710da */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004710db */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004710dc */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004710dd */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004710de */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004710df */
    f->r[4]-=4;MI_U32(f->r[4])=0x004710e4u;f->boundary(0x004710dfu,0x004c3420u,f);
    /* 004710e4 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004710e7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004710e8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004710e9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004710ea */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004710eb */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004710ee */
    f->r[4]+=4;return;
mi_004710ef:;
    /* 004710ef */
    f->r[0]=mi_read((0x00000098u+f->r[4]),4u);
    /* 004710f6 */
    f->r[2]=mi_read((0x00eaac28u),4u);
    /* 004710fc */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004710fe */
    if((f->flags&MI_ZF))goto mi_00471133;
    /* 00471100 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00471102 */
    a=f->r[3];b=0x000000d3u;t=mi_alu(f,a,b,32,1);
    /* 00471108 */
    f->r[1]=f->r[2];
    /* 0047110a */
    t=(!(f->flags&MI_ZF))?1u:0u;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0047110d */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 00471113 */
    f->r[1]=mi_read((0x00ea5f9cu+f->r[1]),4u);
    /* 00471119 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0047111a */
    f->r[6]=f->r[1];
    /* 0047111c */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0047111f */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00471121 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((mi_read((0x00799a81u+f->r[6]*8u),1u))&0xffu)<<0);
    /* 00471128 */
    f->r[1]=(0x00799a81u+f->r[6]*8u);
    /* 0047112f */
    a=(f->r[3]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,0);f->r[3]=(f->r[3]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00471131 */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)((f->r[3]&0xffu));
mi_00471133:;
    /* 00471133 */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 00471139 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047113b */
    f->r[2]=mi_read((0x00ea5f9cu+f->r[2]),4u);
    /* 00471141 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471142 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471147u;f->boundary(0x00471142u,0x004c0d50u,f);
    /* 00471147 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047114a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0047114b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0047114c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0047114d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0047114e */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00471151 */
    f->r[4]+=4;return;
mi_00471152:;
    /* 00471152 */
    f->r[7]=mi_read((0x00000090u+f->r[4]),4u);
    /* 00471159 */
    f->r[3]=mi_read((0x0000008cu+f->r[4]),4u);
    /* 00471160 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471162 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471163 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471164 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471169u;f->boundary(0x00471164u,0x0047d060u,f);
    /* 00471169 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bc96d8u),1u))&0xffu)<<0);
    /* 0047116f */
    f->r[0]=f->r[6];
    /* 00471171 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00471174 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00471176 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471177 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471178 */
    MI_U8((0x00799a82u+f->r[0]*8u))=(uint8_t)((f->r[1]&0xffu));
    /* 0047117f */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471184u;f->boundary(0x0047117fu,0x0047a1c0u,f);
    /* 00471184 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00471187 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00471189 */
    if((f->flags&MI_ZF))goto mi_004711ac;
    /* 0047118b */
    f->r[2]=mi_read((0x00bc96dcu),4u);
    /* 00471191 */
    f->r[0]=mi_read((0x00f5724cu),4u);
    /* 00471196 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00471198 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471199 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047119a */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047119b */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047119c */
    f->r[4]-=4;MI_U32(f->r[4])=0x004711a1u;f->boundary(0x0047119cu,0x0047d1f0u,f);
    /* 004711a1 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004711a4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004711a5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004711a6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004711a7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004711a8 */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004711ab */
    f->r[4]+=4;return;
mi_004711ac:;
    /* 004711ac */
    f->r[1]=mi_read((0x00bc96dcu),4u);
    /* 004711b2 */
    f->r[2]=mi_read((0x00f56c0cu),4u);
    /* 004711b8 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004711ba */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004711bb */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004711bc */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004711bd */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004711be */
    f->r[4]-=4;MI_U32(f->r[4])=0x004711c3u;f->boundary(0x004711beu,0x0047d1f0u,f);
    /* 004711c3 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004711c6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004711c7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004711c8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004711c9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004711ca */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004711cd */
    f->r[4]+=4;return;
mi_004711ce:;
    /* 004711ce */
    f->r[0]=mi_read((0x00000098u+f->r[4]),4u);
    /* 004711d5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004711d7 */
    if((f->flags&MI_ZF))goto mi_00471221;
    /* 004711d9 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004711da */
    f->r[4]-=4;MI_U32(f->r[4])=0x004711dfu;f->boundary(0x004711dau,0x0046efd0u,f);
    /* 004711df */
    f->r[7]=f->r[6];
    /* 004711e1 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004711e4 */
    a=f->r[7];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 004711e7 */
    a=f->r[7];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004711e9 */
    a=f->r[7];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 004711ec */
    MI_U8((0x00799aa2u+f->r[7]))=(uint8_t)((f->r[0]&0xffu));
    /* 004711f2 */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004711f7 */
    f->r[1]=(0x00000000u+f->r[0]*8u);
    /* 004711fe */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00471200 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00471203 */
    a=f->r[1];b=0x00eaaf68u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00471209 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047120eu;f->boundary(0x00471209u,0x004f6520u,f);
    /* 0047120e */
    f->r[0]=mi_read((0x00bca310u),4u);
    /* 00471213 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00471215 */
    if(!(f->flags&MI_ZF))goto mi_00471221;
    /* 00471217 */
    MI_U32((0x00799a9cu+f->r[7]))=(uint32_t)(0x00000000u);
mi_00471221:;
    /* 00471221 */
    f->r[7]=mi_read((0x00000090u+f->r[4]),4u);
    /* 00471228 */
    f->r[3]=mi_read((0x0000008cu+f->r[4]),4u);
    /* 0047122f */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471231 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471233 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471234 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471235 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047123au;f->boundary(0x00471235u,0x0047cc70u,f);
    /* 0047123a */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bc96d8u),1u))&0xffu)<<0);
    /* 00471240 */
    f->r[0]=f->r[6];
    /* 00471242 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00471245 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00471247 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471248 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471249 */
    MI_U8((0x00799a82u+f->r[0]*8u))=(uint8_t)((f->r[1]&0xffu));
    /* 00471250 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471255u;f->boundary(0x00471250u,0x0047a1c0u,f);
    /* 00471255 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00471258 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0047125a */
    if((f->flags&MI_ZF))goto mi_0047126c;
    /* 0047125c */
    f->r[2]=mi_read((0x00bc96dcu),4u);
    /* 00471262 */
    f->r[0]=mi_read((0x00f57274u),4u);
    /* 00471267 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00471269 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047126a */
    goto mi_0047127b;
mi_0047126c:;
    /* 0047126c */
    f->r[1]=mi_read((0x00bc96dcu),4u);
    /* 00471272 */
    f->r[2]=mi_read((0x00f56bf4u),4u);
    /* 00471278 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0047127a */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
mi_0047127b:;
    /* 0047127b */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047127c */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047127d */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047127e */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471283u;f->boundary(0x0047127eu,0x0047cf00u,f);
    /* 00471283 */
    f->r[0]=mi_read((0x00bca310u),4u);
    /* 00471288 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047128b */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0047128e */
    if(!(f->flags&MI_ZF))goto mi_004712a4;
    /* 00471290 */
    f->r[0]=mi_read((0x00bc9800u),4u);
    /* 00471295 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471296 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047129bu;f->boundary(0x00471296u,0x00485100u,f);
    /* 0047129b */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047129c */
    f->r[4]-=4;MI_U32(f->r[4])=0x004712a1u;f->boundary(0x0047129cu,0x00485100u,f);
    /* 004712a1 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004712a4:;
    /* 004712a4 */
    a=mi_read((0x00bca310u),4u);b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 004712ab */
    if(!(f->flags&MI_ZF))goto mi_004724ea;
    /* 004712b1 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004712b2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004712b7u;f->boundary(0x004712b2u,0x00485100u,f);
    /* 004712b7 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004712ba */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004712bb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004712bc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004712bd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004712be */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004712c1 */
    f->r[4]+=4;return;
mi_004712c2:;
    /* 004712c2 */
    f->r[7]=mi_read((0x00000090u+f->r[4]),4u);
    /* 004712c9 */
    f->r[3]=mi_read((0x0000008cu+f->r[4]),4u);
    /* 004712d0 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004712d2 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004712d4 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004712d5 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004712d6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004712dbu;f->boundary(0x004712d6u,0x0047cc70u,f);
    /* 004712db */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bc96d8u),1u))&0xffu)<<0);
    /* 004712e1 */
    f->r[1]=f->r[6];
    /* 004712e3 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004712e6 */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004712e8 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004712e9 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004712ea */
    MI_U8((0x00799a82u+f->r[1]*8u))=(uint8_t)((f->r[2]&0xffu));
    /* 004712f1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004712f6u;f->boundary(0x004712f1u,0x0047a1c0u,f);
    /* 004712f6 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004712f9 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004712fb */
    if((f->flags&MI_ZF))goto mi_0047131e;
    /* 004712fd */
    f->r[0]=mi_read((0x00bc96dcu),4u);
    /* 00471302 */
    f->r[1]=mi_read((0x00f5726cu),4u);
    /* 00471308 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0047130a */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047130b */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047130c */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047130d */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047130e */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471313u;f->boundary(0x0047130eu,0x0047cf00u,f);
    /* 00471313 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00471316 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00471317 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00471318 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00471319 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0047131a */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047131d */
    f->r[4]+=4;return;
mi_0047131e:;
    /* 0047131e */
    f->r[2]=mi_read((0x00bc96dcu),4u);
    /* 00471324 */
    f->r[0]=mi_read((0x00f56bccu),4u);
    /* 00471329 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0047132b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047132c */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047132d */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047132e */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047132f */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471334u;f->boundary(0x0047132fu,0x0047cf00u,f);
    /* 00471334 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00471337 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00471338 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00471339 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0047133a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0047133b */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047133e */
    f->r[4]+=4;return;
mi_0047133f:;
    /* 0047133f */
    f->r[7]=mi_read((0x00000090u+f->r[4]),4u);
    /* 00471346 */
    f->r[3]=mi_read((0x0000008cu+f->r[4]),4u);
    /* 0047134d */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047134f */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471351 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471352 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471353 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471358u;f->boundary(0x00471353u,0x0047cc70u,f);
    /* 00471358 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bc96d8u),1u))&0xffu)<<0);
    /* 0047135e */
    f->r[1]=f->r[6];
    /* 00471360 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00471363 */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00471365 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471366 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471367 */
    MI_U8((0x00799a82u+f->r[1]*8u))=(uint8_t)((f->r[2]&0xffu));
    /* 0047136e */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471373u;f->boundary(0x0047136eu,0x0047a1c0u,f);
    /* 00471373 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00471376 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00471378 */
    if((f->flags&MI_ZF))goto mi_0047139b;
    /* 0047137a */
    f->r[0]=mi_read((0x00bc96dcu),4u);
    /* 0047137f */
    f->r[1]=mi_read((0x00f5725cu),4u);
    /* 00471385 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00471387 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471388 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471389 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047138a */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047138b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471390u;f->boundary(0x0047138bu,0x0047cf00u,f);
    /* 00471390 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00471393 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00471394 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00471395 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00471396 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00471397 */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047139a */
    f->r[4]+=4;return;
mi_0047139b:;
    /* 0047139b */
    f->r[2]=mi_read((0x00bc96dcu),4u);
    /* 004713a1 */
    f->r[0]=mi_read((0x00f56bc4u),4u);
    /* 004713a6 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004713a8 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004713a9 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004713aa */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004713ab */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004713ac */
    f->r[4]-=4;MI_U32(f->r[4])=0x004713b1u;f->boundary(0x004713acu,0x0047cf00u,f);
    /* 004713b1 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004713b4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004713b5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004713b6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004713b7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004713b8 */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004713bb */
    f->r[4]+=4;return;
mi_004713bc:;
    /* 004713bc */
    f->r[7]=mi_read((0x00000090u+f->r[4]),4u);
    /* 004713c3 */
    f->r[3]=mi_read((0x0000008cu+f->r[4]),4u);
    /* 004713ca */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004713cc */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004713cd */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004713ce */
    f->r[4]-=4;MI_U32(f->r[4])=0x004713d3u;f->boundary(0x004713ceu,0x0047d060u,f);
    /* 004713d3 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bc96d8u),1u))&0xffu)<<0);
    /* 004713d9 */
    f->r[1]=f->r[6];
    /* 004713db */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004713de */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004713e0 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004713e1 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004713e2 */
    MI_U8((0x00799a82u+f->r[1]*8u))=(uint8_t)((f->r[2]&0xffu));
    /* 004713e9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004713eeu;f->boundary(0x004713e9u,0x0047a1c0u,f);
    /* 004713ee */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004713f1 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004713f3 */
    if((f->flags&MI_ZF))goto mi_00471405;
    /* 004713f5 */
    f->r[0]=mi_read((0x00bc96dcu),4u);
    /* 004713fa */
    f->r[1]=mi_read((0x00f57254u),4u);
    /* 00471400 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00471402 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471403 */
    goto mi_00471413;
mi_00471405:;
    /* 00471405 */
    f->r[2]=mi_read((0x00bc96dcu),4u);
    /* 0047140b */
    f->r[0]=mi_read((0x00f56c1cu),4u);
    /* 00471410 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00471412 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
mi_00471413:;
    /* 00471413 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471414 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471415 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471416 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047141bu;f->boundary(0x00471416u,0x0047d1f0u,f);
    /* 0047141b */
    f->r[0]=mi_read((0x000000a8u+f->r[4]),4u);
    /* 00471422 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00471425 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00471427 */
    if((f->flags&MI_ZF))goto mi_004724ea;
    /* 0047142d */
    f->r[0]=mi_read((0x00bcb9b0u),4u);
    /* 00471432 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00471433 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00471434 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00471435 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00471436 */
    MI_U32((0x00bcb9b0u))=(uint32_t)(f->r[0]);
    /* 0047143b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0047143c */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047143f */
    f->r[4]+=4;return;
mi_00471440:;
    /* 00471440 */
    f->r[7]=mi_read((0x00000090u+f->r[4]),4u);
    /* 00471447 */
    f->r[3]=mi_read((0x0000008cu+f->r[4]),4u);
    /* 0047144e */
    a=0xff3fbfffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471453 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471455 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471457 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471458 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471459 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047145eu;f->boundary(0x00471459u,0x00482580u,f);
    /* 0047145e */
    f->r[1]=mi_read((0x00f56854u),4u);
    /* 00471464 */
    f->r[0]=mi_read((0x000000a8u+f->r[4]),4u);
    /* 0047146b */
    a=0x00008008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471470 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471471 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471472 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471473 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471474 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471475 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471476 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047147bu;f->boundary(0x00471476u,0x0047bf50u,f);
    /* 0047147b */
    a=0x0000000au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047147d */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047147e */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047147f */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471484u;f->boundary(0x0047147fu,0x0047afd0u,f);
    /* 00471484 */
    a=f->r[4];b=0x0000003cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00471487 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00471488 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00471489 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0047148a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0047148b */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047148e */
    f->r[4]+=4;return;
mi_0047148f:;
    /* 0047148f */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471490 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471495u;f->boundary(0x00471490u,0x004bfb30u,f);
    /* 00471495 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00471498 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00471499 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0047149a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0047149b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0047149c */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047149f */
    f->r[4]+=4;return;
mi_004714a0:;
    /* 004714a0 */
    f->r[2]=mi_read((0x00f5662cu),4u);
    /* 004714a6 */
    f->r[0]=mi_read((0x00000094u+f->r[4]),4u);
    /* 004714ad */
    f->r[5]=mi_read((0x00000090u+f->r[4]),4u);
    /* 004714b4 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004714b6 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004714b7 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004714b8 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004714b9 */
    f->r[0]=mi_read((0x0000009cu+f->r[4]),4u);
    /* 004714c0 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004714c1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004714c2 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004714c3 */
    MI_U32((0x000000b8u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 004714ca */
    f->r[4]-=4;MI_U32(f->r[4])=0x004714cfu;f->boundary(0x004714cau,0x0047bf50u,f);
    /* 004714cf */
    f->r[0]=mi_read((0x000000b4u+f->r[4]),4u);
    /* 004714d6 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004714d9 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004714db */
    if((f->flags&MI_ZF))goto mi_004716f7;
    /* 004714e1 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004714e2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004714e7u;f->boundary(0x004714e2u,0x004b6e10u,f);
    /* 004714e7 */
    f->r[7]=f->r[0];
    /* 004714e9 */
    f->r[0]=f->r[6];
    /* 004714eb */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004714ee */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004714f0 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004714f3 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004714f6 */
    a=f->r[3];b=0x00000028u;t=mi_alu(f,a,b,32,1);
    /* 004714f9 */
    MI_U32((0x00000090u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00471500 */
    MI_U16((0x00799a10u+f->r[0]))=(uint16_t)(0x00000000u);
    /* 00471509 */
    MI_U16((0x00799a28u+f->r[0]))=(uint16_t)((f->r[7]&0xffffu));
    /* 00471510 */
    if(!(f->flags&MI_ZF))goto mi_00471598;
    /* 00471516 */
    f->r[1]=0x0000000du;
    /* 0047151b */
    f->r[6]=(0x00000000u+f->r[7]+f->r[7]*8u);
    /* 0047151e */
    MI_U16((0x007999eau+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 00471525 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x006fdd04u),1u))&0xffu)<<0);
    /* 0047152a */
    a=f->r[6];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0047152d */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0047152f */
    MI_U16((0x00bcb9ccu+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
    /* 00471536 */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_00471691;
    /* 0047153c */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x006fdcfcu),1u))&0xffu)<<0);
    /* 00471542 */
    f->r[0]=mi_read((0x0000008cu+f->r[4]),4u);
    /* 00471549 */
    MI_U8((0x00bcb9cau+f->r[6]))=(uint8_t)((f->r[1]&0xffu));
    /* 0047154f */
    f->r[1]=mi_read((0x006fdd04u),4u);
    /* 00471555 */
    f->r[2]=(0x00000001u+f->r[5]);
    /* 00471558 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0047155b */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047155c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047155d */
    a=f->r[1];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 00471563 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471564 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471565 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047156au;f->boundary(0x00471565u,0x00472790u,f);
    /* 0047156a */
    f->r[2]=(f->r[2]&0xffff0000u)|(((mi_read((0x006fdd00u),2u))&0xffffu)<<0);
    /* 00471571 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00471574 */
    MI_U16((0x00bcba42u+f->r[6]))=(uint16_t)((f->r[2]&0xffffu));
    /* 0047157b */
    MI_U32((0x006fdd00u))=(uint32_t)(0xffffffffu);
    /* 00471585 */
    MI_U8((0x006fdd04u))=(uint8_t)(0x00000000u);
    /* 0047158c */
    MI_U8((0x006fdcfcu))=(uint8_t)(0x000000ffu);
    /* 00471593 */
    goto mi_00471691;
mi_00471598:;
    /* 00471598 */
    a=f->r[3];b=0x0000002cu;t=mi_alu(f,a,b,32,1);
    /* 0047159b */
    if(!(f->flags&MI_ZF))goto mi_00471619;
    /* 0047159d */
    f->r[1]=0x0000000bu;
    /* 004715a2 */
    f->r[6]=(0x00000000u+f->r[7]+f->r[7]*8u);
    /* 004715a5 */
    MI_U16((0x007999eau+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004715ac */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x006fdcf8u),1u))&0xffu)<<0);
    /* 004715b1 */
    a=f->r[6];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004715b4 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004715b6 */
    MI_U16((0x00bcb9ccu+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004715bd */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_00471691;
    /* 004715c3 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x006fdcf0u),1u))&0xffu)<<0);
    /* 004715c8 */
    f->r[2]=mi_read((0x0000008cu+f->r[4]),4u);
    /* 004715cf */
    MI_U8((0x00bcb9cau+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 004715d5 */
    f->r[0]=mi_read((0x006fdcf8u),4u);
    /* 004715da */
    f->r[1]=(0x00000001u+f->r[5]);
    /* 004715dd */
    a=f->r[2];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004715e0 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004715e1 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004715e2 */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004715e7 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004715e8 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004715e9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004715eeu;f->boundary(0x004715e9u,0x00472790u,f);
    /* 004715ee */
    f->r[1]=(f->r[1]&0xffff0000u)|(((mi_read((0x006fdcf4u),2u))&0xffffu)<<0);
    /* 004715f5 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004715f8 */
    MI_U16((0x00bcba42u+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004715ff */
    MI_U32((0x006fdcf4u))=(uint32_t)(0xffffffffu);
    /* 00471609 */
    MI_U8((0x006fdcf8u))=(uint8_t)(0x00000000u);
    /* 00471610 */
    MI_U8((0x006fdcf0u))=(uint8_t)(0x000000ffu);
    /* 00471617 */
    goto mi_00471691;
mi_00471619:;
    /* 00471619 */
    f->r[1]=0x0000000cu;
    /* 0047161e */
    f->r[6]=(0x00000000u+f->r[7]+f->r[7]*8u);
    /* 00471621 */
    MI_U16((0x007999eau+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 00471628 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x006fdcecu),1u))&0xffu)<<0);
    /* 0047162d */
    a=f->r[6];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 00471630 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00471632 */
    MI_U16((0x00bcb9ccu+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
    /* 00471639 */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_00471691;
    /* 0047163b */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x006fdce4u),1u))&0xffu)<<0);
    /* 00471641 */
    f->r[1]=mi_read((0x0000008cu+f->r[4]),4u);
    /* 00471648 */
    MI_U8((0x00bcb9cau+f->r[6]))=(uint8_t)((f->r[2]&0xffu));
    /* 0047164e */
    f->r[2]=mi_read((0x006fdcecu),4u);
    /* 00471654 */
    f->r[0]=(0x00000001u+f->r[5]);
    /* 00471657 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0047165a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047165b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047165c */
    a=f->r[2];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 00471662 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471663 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471664 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471669u;f->boundary(0x00471664u,0x00472790u,f);
    /* 00471669 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x006fdce8u),2u))&0xffffu)<<0);
    /* 0047166f */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00471672 */
    MI_U16((0x00bcba42u+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 00471679 */
    MI_U32((0x006fdce8u))=(uint32_t)(0xffffffffu);
    /* 00471683 */
    MI_U8((0x006fdcecu))=(uint8_t)(0x00000000u);
    /* 0047168a */
    MI_U8((0x006fdce4u))=(uint8_t)(0x000000ffu);
mi_00471691:;
    /* 00471691 */
    f->r[2]=mi_read((0x0000008cu+f->r[4]),4u);
    /* 00471698 */
    f->r[0]=mi_read((0x00000080u+f->r[4]),4u);
    /* 0047169f */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004716a1 */
    f->r[1]=(0xffffffffu+f->r[5]);
    /* 004716a4 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004716a6 */
    a=f->r[2];b=0x00000003u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004716a9 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004716aa */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004716ab */
    a=0x00000036u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004716ad */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004716ae */
    f->r[4]-=4;MI_U32(f->r[4])=0x004716b3u;f->boundary(0x004716aeu,0x0046a400u,f);
    /* 004716b3 */
    f->r[6]=f->r[0];
    /* 004716b5 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004716b6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004716bbu;f->boundary(0x004716b6u,0x00485100u,f);
    /* 004716bb */
    f->r[0]=f->r[6];
    /* 004716bd */
    f->r[1]=(f->r[1]&0xffff0000u)|(((mi_read((0x000000b8u+f->r[4]),2u))&0xffffu)<<0);
    /* 004716c5 */
    f->r[2]=mi_read((0x000000acu+f->r[4]),4u);
    /* 004716cc */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004716cf */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004716d2 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004716d4 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004716d7 */
    MI_U16((0x00799a28u+f->r[0]))=(uint16_t)((f->r[7]&0xffffu));
    /* 004716de */
    MI_U16((0x00799a10u+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004716e5 */
    MI_U16((0x00799a12u+f->r[2]))=(uint16_t)((f->r[6]&0xffffu));
    /* 004716ec */
    MI_U16((0x00799a12u+f->r[0]))=(uint16_t)(0x00000000u);
    /* 004716f5 */
    goto mi_00471706;
mi_004716f7:;
    /* 004716f7 */
    f->r[0]=f->r[6];
    /* 004716f9 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004716fc */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004716fe */
    f->r[6]=(uint32_t)(int32_t)(int16_t)mi_read((0x00799a12u+f->r[0]*8u),2u);
mi_00471706:;
    /* 00471706 */
    f->r[1]=mi_read((0x00f5662cu),4u);
    /* 0047170c */
    f->r[2]=mi_read((0x0000008cu+f->r[4]),4u);
    /* 00471713 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00471714 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471716 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471717 */
    a=0x00000004u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471719 */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 0047171a */
    a=0x00000004u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047171c */
    a=f->r[2];b=0x00000003u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0047171f */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471720 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471721 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471722 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471727u;f->boundary(0x00471722u,0x0047bf50u,f);
    /* 00471727 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047172a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0047172b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0047172c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0047172d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0047172e */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00471731 */
    f->r[4]+=4;return;
mi_00471732:;
    /* 00471732 */
    f->r[0]=mi_read((0x00f5694cu),4u);
    /* 00471737 */
    f->r[7]=mi_read((0x00000090u+f->r[4]),4u);
    /* 0047173e */
    f->r[3]=mi_read((0x0000008cu+f->r[4]),4u);
    /* 00471745 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471747 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471748 */
    f->r[0]=mi_read((0x0000009cu+f->r[4]),4u);
    /* 0047174f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471750 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471751 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471752 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471753 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471754 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471759u;f->boundary(0x00471754u,0x0047bf50u,f);
    /* 00471759 */
    f->r[0]=mi_read((0x00eaac28u),4u);
    /* 0047175e */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00471761 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 00471767 */
    f->r[1]=mi_read((0x00ea5d18u+f->r[0]),4u);
    /* 0047176d */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0047176f */
    if(!(f->flags&MI_ZF))goto mi_004724ea;
    /* 00471775 */
    f->r[1]=(0x00000000u+f->r[7]+f->r[7]*8u);
    /* 00471778 */
    MI_U32((0x00ea5d1cu+f->r[0]))=(uint32_t)(f->r[6]);
    /* 0047177e */
    MI_U16((0x00ea5d14u+f->r[0]))=(uint16_t)((f->r[3]&0xffffu));
    /* 00471785 */
    MI_U16((0x00ea5d16u+f->r[0]))=(uint16_t)((f->r[7]&0xffffu));
    /* 0047178c */
    f->r[1]=(0x00000000u+f->r[7]+f->r[1]*2u);
    /* 0047178f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00471790 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00471791 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00471792 */
    f->r[2]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 00471795 */
    f->r[1]=mi_read((0x00bca2d8u),4u);
    /* 0047179b */
    f->r[2]=(0x00000000u+f->r[1]+f->r[2]*4u);
    /* 0047179e */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004717a0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004717a1 */
    MI_U32((0x00ea5d18u+f->r[0]))=(uint32_t)(f->r[2]);
    /* 004717a7 */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004717aa */
    f->r[4]+=4;return;
mi_004717ab:;
    /* 004717ab */
    f->r[0]=mi_read((0x00eaac28u),4u);
    /* 004717b0 */
    f->r[1]=mi_read((0x00f5694cu),4u);
    /* 004717b6 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004717bc */
    f->r[7]=mi_read((0x00000090u+f->r[4]),4u);
    /* 004717c3 */
    f->r[3]=mi_read((0x0000008cu+f->r[4]),4u);
    /* 004717ca */
    MI_U32((0x00ea5e70u+f->r[0]))=(uint32_t)(0x00000001u);
    /* 004717d4 */
    f->r[0]=mi_read((0x00000094u+f->r[4]),4u);
    /* 004717db */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004717dd */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004717de */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004717df */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004717e0 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004717e1 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004717e2 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004717e3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004717e8u;f->boundary(0x004717e3u,0x0047bf50u,f);
    /* 004717e8 */
    f->r[0]=mi_read((0x00eaac28u),4u);
    /* 004717ed */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004717f0 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004717f6 */
    f->r[1]=mi_read((0x00ea5e68u+f->r[0]),4u);
    /* 004717fc */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004717fe */
    if(!(f->flags&MI_ZF))goto mi_004724ea;
    /* 00471804 */
    f->r[2]=(0x00000000u+f->r[7]+f->r[7]*8u);
    /* 00471807 */
    MI_U32((0x00ea5e6cu+f->r[0]))=(uint32_t)(f->r[6]);
    /* 0047180d */
    MI_U16((0x00ea5e64u+f->r[0]))=(uint16_t)((f->r[3]&0xffffu));
    /* 00471814 */
    MI_U16((0x00ea5e66u+f->r[0]))=(uint16_t)((f->r[7]&0xffffu));
    /* 0047181b */
    f->r[1]=(0x00000000u+f->r[7]+f->r[2]*2u);
    /* 0047181e */
    f->r[2]=mi_read((0x00bca2d8u),4u);
    /* 00471824 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00471825 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00471826 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 00471829 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0047182a */
    f->r[1]=(0x00000000u+f->r[2]+f->r[1]*4u);
    /* 0047182d */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0047182f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00471830 */
    MI_U32((0x00ea5e68u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 00471836 */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00471839 */
    f->r[4]+=4;return;
mi_0047183a:;
    /* 0047183a */
    f->r[0]=mi_read((0x00000098u+f->r[4]),4u);
    /* 00471841 */
    f->r[5]=f->r[6];
    /* 00471843 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00471845 */
    if((f->flags&MI_ZF))goto mi_004719b8;
    /* 0047184b */
    f->r[2]=mi_read((0x00eaac28u),4u);
    /* 00471851 */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 00471857 */
    MI_U16((0x00ea5baau+f->r[2]))=(uint16_t)(0x00000001u);
    /* 00471860 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471865u;f->boundary(0x00471860u,0x0041ff90u,f);
    /* 00471865 */
    f->r[0]=mi_read((0x00bca2e0u),4u);
    /* 0047186a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0047186c */
    if((f->flags&MI_ZF))goto mi_00471888;
    /* 0047186e */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 00471871 */
    if((f->flags&MI_ZF))goto mi_00471888;
    /* 00471873 */
    f->r[0]=f->r[6];
    /* 00471875 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00471878 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0047187a */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0047187d */
    MI_U16((0x007999eau+f->r[0]))=(uint16_t)(0x00000003u);
    /* 00471886 */
    goto mi_0047189b;
mi_00471888:;
    /* 00471888 */
    f->r[0]=f->r[6];
    /* 0047188a */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0047188d */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0047188f */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00471892 */
    MI_U16((0x007999eau+f->r[0]))=(uint16_t)(0x00000000u);
mi_0047189b:;
    /* 0047189b */
    f->r[1]=mi_read((0x0000008cu+f->r[4]),4u);
    /* 004718a2 */
    f->r[3]=mi_read((0x00000080u+f->r[4]),4u);
    /* 004718a9 */
    MI_U16((0x00799a10u+f->r[0]))=(uint16_t)(0x00000000u);
    /* 004718b2 */
    f->r[0]=mi_read((0x00000090u+f->r[4]),4u);
    /* 004718b9 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004718bb */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004718bd */
    a=f->r[1];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004718c0 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004718c1 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004718c2 */
    a=0x000000d2u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004718c7 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004718c8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004718cdu;f->boundary(0x004718c8u,0x0046a400u,f);
    /* 004718cd */
    f->r[7]=f->r[0];
    /* 004718cf */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004718d0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004718d5u;f->boundary(0x004718d0u,0x00485100u,f);
    /* 004718d5 */
    f->r[0]=mi_read((0x00bca2e0u),4u);
    /* 004718da */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004718dd */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004718df */
    if((f->flags&MI_ZF))goto mi_004718fb;
    /* 004718e1 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 004718e4 */
    if((f->flags&MI_ZF))goto mi_004718fb;
    /* 004718e6 */
    f->r[0]=f->r[7];
    /* 004718e8 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004718eb */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004718ed */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004718f0 */
    MI_U16((0x007999eau+f->r[0]))=(uint16_t)(0x00000004u);
    /* 004718f9 */
    goto mi_0047190e;
mi_004718fb:;
    /* 004718fb */
    f->r[0]=f->r[7];
    /* 004718fd */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00471900 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00471902 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00471905 */
    MI_U16((0x007999eau+f->r[0]))=(uint16_t)(0x00000001u);
mi_0047190e:;
    /* 0047190e */
    f->r[2]=f->r[6];
    /* 00471910 */
    f->r[1]=mi_read((0x0000008cu+f->r[4]),4u);
    /* 00471917 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0047191a */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0047191c */
    MI_U16((0x00799a10u+f->r[0]))=(uint16_t)((f->r[6]&0xffffu));
    /* 00471923 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471925 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471927 */
    MI_U16((0x00799a12u+f->r[2]*8u))=(uint16_t)((f->r[7]&0xffffu));
    /* 0047192f */
    MI_U16((0x00799a12u+f->r[0]))=(uint16_t)(0x00000000u);
    /* 00471938 */
    f->r[0]=mi_read((0x00000098u+f->r[4]),4u);
    /* 0047193f */
    a=f->r[1];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00471942 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471943 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471944 */
    a=0x000000d2u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471949 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047194a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047194fu;f->boundary(0x0047194au,0x0046a400u,f);
    /* 0047194f */
    f->r[6]=f->r[0];
    /* 00471951 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471952 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471957u;f->boundary(0x00471952u,0x00485100u,f);
    /* 00471957 */
    f->r[0]=mi_read((0x00bca2e0u),4u);
    /* 0047195c */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047195f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00471961 */
    if((f->flags&MI_ZF))goto mi_0047197d;
    /* 00471963 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 00471966 */
    if((f->flags&MI_ZF))goto mi_0047197d;
    /* 00471968 */
    f->r[0]=f->r[6];
    /* 0047196a */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0047196d */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0047196f */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00471972 */
    MI_U16((0x007999eau+f->r[0]))=(uint16_t)(0x00000005u);
    /* 0047197b */
    goto mi_00471990;
mi_0047197d:;
    /* 0047197d */
    f->r[0]=f->r[6];
    /* 0047197f */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00471982 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00471984 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00471987 */
    MI_U16((0x007999eau+f->r[0]))=(uint16_t)(0x00000002u);
mi_00471990:;
    /* 00471990 */
    f->r[2]=f->r[7];
    /* 00471992 */
    MI_U16((0x00799a10u+f->r[0]))=(uint16_t)((f->r[7]&0xffffu));
    /* 00471999 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0047199c */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0047199e */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047199f */
    MI_U16((0x00799a12u+f->r[2]*8u))=(uint16_t)((f->r[6]&0xffffu));
    /* 004719a7 */
    MI_U16((0x00799a12u+f->r[0]))=(uint16_t)(0x00000000u);
    /* 004719b0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004719b5u;f->boundary(0x004719b0u,0x004f4f80u,f);
    /* 004719b5 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004719b8:;
    /* 004719b8 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004719b9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004719beu;f->boundary(0x004719b9u,0x00475b50u,f);
    /* 004719be */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004719c1 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004719c3 */
    if(!(f->flags&MI_ZF))goto mi_00471ae9;
    /* 004719c9 */
    f->r[0]=mi_read((0x00bca2e0u),4u);
    /* 004719ce */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004719d0 */
    if(!(f->flags&MI_ZF))goto mi_004719dc;
    /* 004719d2 */
    f->r[0]=mi_read((0x00f56d54u),4u);
    /* 004719d7 */
    f->r[7]=(0x00000001u+f->r[0]);
    /* 004719da */
    goto mi_004719fd;
mi_004719dc:;
    /* 004719dc */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 004719df */
    if(!(f->flags&MI_ZF))goto mi_004719ec;
    /* 004719e1 */
    f->r[1]=mi_read((0x00f56d64u),4u);
    /* 004719e7 */
    f->r[7]=(0x00000001u+f->r[1]);
    /* 004719ea */
    goto mi_004719fd;
mi_004719ec:;
    /* 004719ec */
    f->r[7]=mi_read((0x00f56d64u),4u);
    /* 004719f2 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 004719f5 */
    if((f->flags&MI_ZF))goto mi_004719fd;
    /* 004719f7 */
    f->r[7]=mi_read((0x00f56d54u),4u);
mi_004719fd:;
    /* 004719fd */
    f->r[6]=mi_read((0x00000094u+f->r[4]),4u);
    /* 00471a04 */
    f->r[2]=mi_read((0x00000090u+f->r[4]),4u);
    /* 00471a0b */
    f->r[0]=mi_read((0x0000008cu+f->r[4]),4u);
    /* 00471a12 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471a14 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471a15 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471a16 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471a17 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471a18 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471a19 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471a1a */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471a1fu;f->boundary(0x00471a1au,0x0047bf50u,f);
    /* 00471a1f */
    f->r[0]=mi_read((0x00bca2e0u),4u);
    /* 00471a24 */
    f->r[1]=f->r[5];
    /* 00471a26 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00471a29 */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00471a2b */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00471a2e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00471a30 */
    f->r[3]=(uint32_t)(int32_t)(int16_t)mi_read((0x00799a12u+f->r[1]*8u),2u);
    /* 00471a38 */
    if(!(f->flags&MI_ZF))goto mi_00471a45;
    /* 00471a3a */
    f->r[2]=mi_read((0x00f56d5cu),4u);
    /* 00471a40 */
    f->r[7]=(0x00000001u+f->r[2]);
    /* 00471a43 */
    goto mi_00471a64;
mi_00471a45:;
    /* 00471a45 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 00471a48 */
    if(!(f->flags&MI_ZF))goto mi_00471a54;
    /* 00471a4a */
    f->r[0]=mi_read((0x00f56d5cu),4u);
    /* 00471a4f */
    f->r[7]=(0x00000001u+f->r[0]);
    /* 00471a52 */
    goto mi_00471a64;
mi_00471a54:;
    /* 00471a54 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 00471a57 */
    if((f->flags&MI_ZF))goto mi_00471a5e;
    /* 00471a59 */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 00471a5c */
    if(!(f->flags&MI_ZF))goto mi_00471a64;
mi_00471a5e:;
    /* 00471a5e */
    f->r[7]=mi_read((0x00f56d5cu),4u);
mi_00471a64:;
    /* 00471a64 */
    f->r[1]=mi_read((0x0000008cu+f->r[4]),4u);
    /* 00471a6b */
    f->r[5]=mi_read((0x00000090u+f->r[4]),4u);
    /* 00471a72 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471a74 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471a75 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471a76 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471a77 */
    a=f->r[1];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00471a7a */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471a7b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471a7c */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471a7d */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471a82u;f->boundary(0x00471a7du,0x0047bf50u,f);
    /* 00471a82 */
    f->r[0]=mi_read((0x00bca2e0u),4u);
    /* 00471a87 */
    f->r[2]=f->r[3];
    /* 00471a89 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 00471a8c */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00471a8e */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00471a91 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00471a93 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00799a12u+f->r[2]*8u),2u);
    /* 00471a9b */
    if(!(f->flags&MI_ZF))goto mi_00471aa7;
    /* 00471a9d */
    f->r[0]=mi_read((0x00f56d64u),4u);
    /* 00471aa2 */
    f->r[7]=(0x00000001u+f->r[0]);
    /* 00471aa5 */
    goto mi_00471acf;
mi_00471aa7:;
    /* 00471aa7 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 00471aaa */
    if(!(f->flags&MI_ZF))goto mi_00471ab7;
    /* 00471aac */
    f->r[2]=mi_read((0x00f56d54u),4u);
    /* 00471ab2 */
    f->r[7]=(0x00000001u+f->r[2]);
    /* 00471ab5 */
    goto mi_00471acf;
mi_00471ab7:;
    /* 00471ab7 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 00471aba */
    if(!(f->flags&MI_ZF))goto mi_00471ac4;
    /* 00471abc */
    f->r[7]=mi_read((0x00f56d54u),4u);
    /* 00471ac2 */
    goto mi_00471acf;
mi_00471ac4:;
    /* 00471ac4 */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 00471ac7 */
    if(!(f->flags&MI_ZF))goto mi_00471acf;
    /* 00471ac9 */
    f->r[7]=mi_read((0x00f56d64u),4u);
mi_00471acf:;
    /* 00471acf */
    f->r[0]=mi_read((0x0000008cu+f->r[4]),4u);
    /* 00471ad6 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471ad8 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471ad9 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471ada */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471adb */
    a=f->r[0];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00471ade */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471adf */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471ae0 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471ae1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471ae6u;f->boundary(0x00471ae1u,0x0047bf50u,f);
    /* 00471ae6 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00471ae9:;
    /* 00471ae9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471aeeu;f->boundary(0x00471ae9u,0x0053d850u,f);
    /* 00471aee */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00471aef */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00471af0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00471af1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00471af2 */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00471af5 */
    f->r[4]+=4;return;
mi_00471af6:;
    /* 00471af6 */
    f->r[0]=mi_read((0x00000098u+f->r[4]),4u);
    /* 00471afd */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00471aff */
    if((f->flags&MI_ZF))goto mi_00471b0a;
    /* 00471b01 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471b02 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471b07u;f->boundary(0x00471b02u,0x004e57f0u,f);
    /* 00471b07 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00471b0a:;
    /* 00471b0a */
    f->r[1]=mi_read((0x00000090u+f->r[4]),4u);
    /* 00471b11 */
    f->r[2]=mi_read((0x0000008cu+f->r[4]),4u);
    /* 00471b18 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471b19 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471b1a */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471b1b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471b20u;f->boundary(0x00471b1bu,0x004e5840u,f);
    /* 00471b20 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00471b23 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00471b24 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00471b25 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00471b26 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00471b27 */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00471b2a */
    f->r[4]+=4;return;
mi_00471b2b:;
    /* 00471b2b */
    f->r[0]=mi_read((0x00000098u+f->r[4]),4u);
    /* 00471b32 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 00471b34 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 00471b36 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471b37 */
    if((f->flags&MI_ZF))goto mi_00471b5b;
    /* 00471b39 */
    f->r[0]=mi_read((0x00000094u+f->r[4]),4u);
    /* 00471b40 */
    f->r[1]=mi_read((0x00000090u+f->r[4]),4u);
    /* 00471b47 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471b48 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471b49 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471b4eu;f->boundary(0x00471b49u,0x005417b0u,f);
    /* 00471b4e */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00471b51 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471b56u;f->boundary(0x00471b51u,0x0041ffc0u,f);
    /* 00471b56 */
    goto mi_00471e1f;
mi_00471b5b:;
    /* 00471b5b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471b60u;f->boundary(0x00471b5bu,0x00541920u,f);
    /* 00471b60 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00471b63 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00471b64 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00471b65 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00471b66 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00471b67 */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00471b6a */
    f->r[4]+=4;return;
mi_00471b6b:;
    /* 00471b6b */
    f->r[0]=mi_read((0x00000098u+f->r[4]),4u);
    /* 00471b72 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 00471b74 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 00471b76 */
    if((f->flags&MI_ZF))goto mi_00471ba5;
    /* 00471b78 */
    f->r[2]=mi_read((0x0000009cu+f->r[4]),4u);
    /* 00471b7f */
    f->r[0]=mi_read((0x00000090u+f->r[4]),4u);
    /* 00471b86 */
    f->r[1]=mi_read((0x0000008cu+f->r[4]),4u);
    /* 00471b8d */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471b8e */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471b8f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471b90 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471b91 */
    f->r[1]=0x006de650u;
    /* 00471b96 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471b9bu;f->boundary(0x00471b96u,0x004015a0u,f);
    /* 00471b9b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471ba0u;f->boundary(0x00471b9bu,0x004209e0u,f);
    /* 00471ba0 */
    goto mi_00471e1f;
mi_00471ba5:;
    /* 00471ba5 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471ba6 */
    f->r[1]=0x006de650u;
    /* 00471bab */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471bb0u;f->boundary(0x00471babu,0x004017a0u,f);
    /* 00471bb0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00471bb1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00471bb2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00471bb3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00471bb4 */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00471bb7 */
    f->r[4]+=4;return;
mi_00471bb8:;
    /* 00471bb8 */
    f->r[0]=mi_read((0x00000098u+f->r[4]),4u);
    /* 00471bbf */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 00471bc1 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 00471bc3 */
    if((f->flags&MI_ZF))goto mi_00471bf2;
    /* 00471bc5 */
    f->r[2]=mi_read((0x0000009cu+f->r[4]),4u);
    /* 00471bcc */
    f->r[0]=mi_read((0x00000090u+f->r[4]),4u);
    /* 00471bd3 */
    f->r[1]=mi_read((0x0000008cu+f->r[4]),4u);
    /* 00471bda */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471bdb */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471bdc */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471bdd */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471bde */
    f->r[1]=0x006fd5d8u;
    /* 00471be3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471be8u;f->boundary(0x00471be3u,0x0040da90u,f);
    /* 00471be8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471bedu;f->boundary(0x00471be8u,0x004206a0u,f);
    /* 00471bed */
    goto mi_00471e1f;
mi_00471bf2:;
    /* 00471bf2 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471bf3 */
    f->r[1]=0x006fd5d8u;
    /* 00471bf8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471bfdu;f->boundary(0x00471bf8u,0x0040dca0u,f);
    /* 00471bfd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00471bfe */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00471bff */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00471c00 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00471c01 */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00471c04 */
    f->r[4]+=4;return;
mi_00471c05:;
    /* 00471c05 */
    f->r[0]=mi_read((0x00000098u+f->r[4]),4u);
    /* 00471c0c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00471c0e */
    if((f->flags&MI_ZF))goto mi_00471c3f;
    /* 00471c10 */
    f->r[2]=mi_read((0x00000090u+f->r[4]),4u);
    /* 00471c17 */
    f->r[0]=mi_read((0x0000008cu+f->r[4]),4u);
    /* 00471c1e */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471c20 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471c21 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471c22 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471c23 */
    f->r[1]=0x006fd368u;
    /* 00471c28 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471c2du;f->boundary(0x00471c28u,0x00417180u,f);
    /* 00471c2d */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471c32u;f->boundary(0x00471c2du,0x00420d10u,f);
    /* 00471c32 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471c37u;f->boundary(0x00471c32u,0x00405ba0u,f);
    /* 00471c37 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00471c38 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00471c39 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00471c3a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00471c3b */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00471c3e */
    f->r[4]+=4;return;
mi_00471c3f:;
    /* 00471c3f */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471c40 */
    f->r[1]=0x006fd368u;
    /* 00471c45 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471c4au;f->boundary(0x00471c45u,0x004017a0u,f);
    /* 00471c4a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00471c4b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00471c4c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00471c4d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00471c4e */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00471c51 */
    f->r[4]+=4;return;
mi_00471c52:;
    /* 00471c52 */
    f->r[0]=mi_read((0x00000098u+f->r[4]),4u);
    /* 00471c59 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00471c5b */
    if((f->flags&MI_ZF))goto mi_00471c97;
    /* 00471c5d */
    f->r[1]=mi_read((0x0000009cu+f->r[4]),4u);
    /* 00471c64 */
    f->r[2]=mi_read((0x00000090u+f->r[4]),4u);
    /* 00471c6b */
    f->r[0]=mi_read((0x0000008cu+f->r[4]),4u);
    /* 00471c72 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471c73 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471c74 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471c75 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471c76 */
    f->r[1]=0x006f9320u;
    /* 00471c7b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471c80u;f->boundary(0x00471c7bu,0x00545c30u,f);
    /* 00471c80 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471c85u;f->boundary(0x00471c80u,0x004212f0u,f);
    /* 00471c85 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00471c86 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00471c87 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00471c88 */
    MI_U32((0x006059e8u))=(uint32_t)(0x00000000u);
    /* 00471c92 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00471c93 */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00471c96 */
    f->r[4]+=4;return;
mi_00471c97:;
    /* 00471c97 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471c98 */
    f->r[1]=0x006f9320u;
    /* 00471c9d */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471ca2u;f->boundary(0x00471c9du,0x005468d0u,f);
    /* 00471ca2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00471ca3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00471ca4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00471ca5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00471ca6 */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00471ca9 */
    f->r[4]+=4;return;
mi_00471caa:;
    /* 00471caa */
    f->r[0]=mi_read((0x00000098u+f->r[4]),4u);
    /* 00471cb1 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00471cb3 */
    if((f->flags&MI_ZF))goto mi_00471cef;
    /* 00471cb5 */
    f->r[1]=mi_read((0x0000009cu+f->r[4]),4u);
    /* 00471cbc */
    f->r[2]=mi_read((0x00000090u+f->r[4]),4u);
    /* 00471cc3 */
    f->r[0]=mi_read((0x0000008cu+f->r[4]),4u);
    /* 00471cca */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471ccb */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471ccc */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471ccd */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471cce */
    f->r[1]=0x006f5648u;
    /* 00471cd3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471cd8u;f->boundary(0x00471cd3u,0x00559130u,f);
    /* 00471cd8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471cddu;f->boundary(0x00471cd8u,0x004212f0u,f);
    /* 00471cdd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00471cde */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00471cdf */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00471ce0 */
    MI_U32((0x006059e8u))=(uint32_t)(0x00000000u);
    /* 00471cea */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00471ceb */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00471cee */
    f->r[4]+=4;return;
mi_00471cef:;
    /* 00471cef */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471cf0 */
    f->r[1]=0x006f5648u;
    /* 00471cf5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471cfau;f->boundary(0x00471cf5u,0x005468d0u,f);
    /* 00471cfa */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00471cfb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00471cfc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00471cfd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00471cfe */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00471d01 */
    f->r[4]+=4;return;
mi_00471d02:;
    /* 00471d02 */
    f->r[0]=mi_read((0x00000098u+f->r[4]),4u);
    /* 00471d09 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00471d0b */
    if((f->flags&MI_ZF))goto mi_00471d47;
    /* 00471d0d */
    f->r[1]=mi_read((0x0000009cu+f->r[4]),4u);
    /* 00471d14 */
    f->r[2]=mi_read((0x00000090u+f->r[4]),4u);
    /* 00471d1b */
    f->r[0]=mi_read((0x0000008cu+f->r[4]),4u);
    /* 00471d22 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471d23 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471d24 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471d25 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471d26 */
    f->r[1]=0x006de9e0u;
    /* 00471d2b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471d30u;f->boundary(0x00471d2bu,0x00556480u,f);
    /* 00471d30 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471d35u;f->boundary(0x00471d30u,0x004212f0u,f);
    /* 00471d35 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00471d36 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00471d37 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00471d38 */
    MI_U32((0x006059e8u))=(uint32_t)(0x00000000u);
    /* 00471d42 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00471d43 */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00471d46 */
    f->r[4]+=4;return;
mi_00471d47:;
    /* 00471d47 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471d48 */
    f->r[1]=0x006de9e0u;
    /* 00471d4d */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471d52u;f->boundary(0x00471d4du,0x005468d0u,f);
    /* 00471d52 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00471d53 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00471d54 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00471d55 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00471d56 */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00471d59 */
    f->r[4]+=4;return;
mi_00471d5a:;
    /* 00471d5a */
    f->r[0]=mi_read((0x00000098u+f->r[4]),4u);
    /* 00471d61 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00471d63 */
    if((f->flags&MI_ZF))goto mi_00471d94;
    /* 00471d65 */
    f->r[1]=mi_read((0x00000090u+f->r[4]),4u);
    /* 00471d6c */
    f->r[2]=mi_read((0x0000008cu+f->r[4]),4u);
    /* 00471d73 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471d75 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471d76 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471d77 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471d78 */
    f->r[1]=0x006e9188u;
    /* 00471d7d */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471d82u;f->boundary(0x00471d7du,0x0054fba0u,f);
    /* 00471d82 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471d87u;f->boundary(0x00471d82u,0x004212f0u,f);
    /* 00471d87 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471d8cu;f->boundary(0x00471d87u,0x00405ba0u,f);
    /* 00471d8c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00471d8d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00471d8e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00471d8f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00471d90 */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00471d93 */
    f->r[4]+=4;return;
mi_00471d94:;
    /* 00471d94 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471d95 */
    f->r[1]=0x006e9188u;
    /* 00471d9a */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471d9fu;f->boundary(0x00471d9au,0x005468d0u,f);
    /* 00471d9f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00471da0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00471da1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00471da2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00471da3 */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00471da6 */
    f->r[4]+=4;return;
mi_00471da7:;
    /* 00471da7 */
    f->r[0]=mi_read((0x00000098u+f->r[4]),4u);
    /* 00471dae */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 00471db0 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 00471db2 */
    if((f->flags&MI_ZF))goto mi_00471dde;
    /* 00471db4 */
    f->r[0]=mi_read((0x0000009cu+f->r[4]),4u);
    /* 00471dbb */
    f->r[1]=mi_read((0x00000090u+f->r[4]),4u);
    /* 00471dc2 */
    f->r[2]=mi_read((0x0000008cu+f->r[4]),4u);
    /* 00471dc9 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471dca */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471dcb */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471dcc */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471dcd */
    f->r[1]=0x006de548u;
    /* 00471dd2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471dd7u;f->boundary(0x00471dd2u,0x004e6840u,f);
    /* 00471dd7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471ddcu;f->boundary(0x00471dd7u,0x00421010u,f);
    /* 00471ddc */
    goto mi_00471e1f;
mi_00471dde:;
    /* 00471dde */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471ddf */
    f->r[1]=0x006de548u;
    /* 00471de4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471de9u;f->boundary(0x00471de4u,0x004e69c0u,f);
    /* 00471de9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00471dea */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00471deb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00471dec */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00471ded */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00471df0 */
    f->r[4]+=4;return;
mi_00471df1:;
    /* 00471df1 */
    f->r[0]=mi_read((0x00000098u+f->r[4]),4u);
    /* 00471df8 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 00471dfa */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 00471dfc */
    if((f->flags&MI_ZF))goto mi_00471e4b;
    /* 00471dfe */
    f->r[0]=mi_read((0x0000009cu+f->r[4]),4u);
    /* 00471e05 */
    f->r[1]=mi_read((0x00000090u+f->r[4]),4u);
    /* 00471e0c */
    f->r[2]=mi_read((0x0000008cu+f->r[4]),4u);
    /* 00471e13 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471e14 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471e15 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471e16 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471e17 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471e1cu;f->boundary(0x00471e17u,0x004df9d0u,f);
    /* 00471e1c */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00471e1f:;
    /* 00471e1f */
    MI_U32((0x006059e8u))=(uint32_t)(f->r[7]);
    /* 00471e25 */
    MI_U32((0x006059e4u))=(uint32_t)(f->r[7]);
    /* 00471e2b */
    MI_U32((0x006059dcu))=(uint32_t)(f->r[7]);
    /* 00471e31 */
    MI_U32((0x006059e0u))=(uint32_t)(f->r[7]);
    /* 00471e37 */
    MI_U32((0x006059d4u))=(uint32_t)(f->r[7]);
    /* 00471e3d */
    MI_U32((0x006059d8u))=(uint32_t)(f->r[7]);
    /* 00471e43 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00471e44 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00471e45 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00471e46 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00471e47 */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00471e4a */
    f->r[4]+=4;return;
mi_00471e4b:;
    /* 00471e4b */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471e4c */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471e51u;f->boundary(0x00471e4cu,0x004dfbb0u,f);
    /* 00471e51 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00471e54 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00471e55 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00471e56 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00471e57 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00471e58 */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00471e5b */
    f->r[4]+=4;return;
mi_00471e5c:;
    /* 00471e5c */
    f->r[0]=0x00000160u;
    /* 00471e61 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 00471e63 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00471e67 */
    MI_U32((0x00000030u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00471e6b */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00471e6f */
    MI_U32((0x00000048u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00471e73 */
    MI_U32((0x00000054u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00471e77 */
    MI_U32((0x00000060u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00471e7b */
    MI_U32((0x0000006cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00471e7f */
    MI_U32((0x00000078u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00471e83 */
    f->r[0]=mi_read((0x00000098u+f->r[4]),4u);
    /* 00471e8a */
    f->r[1]=0x00000001u;
    /* 00471e8f */
    f->r[2]=0x00000002u;
    /* 00471e94 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 00471e96 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 00471e9a */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 00471e9e */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(0x0000015fu);
    /* 00471ea6 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00471eaa */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 00471eae */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00471eb2 */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[5]);
    /* 00471eb6 */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 00471eba */
    MI_U32((0x00000038u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00471ebe */
    MI_U32((0x00000040u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00471ec2 */
    MI_U32((0x00000044u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00471ec6 */
    MI_U32((0x0000004cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00471eca */
    MI_U32((0x00000050u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00471ece */
    MI_U32((0x00000058u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 00471ed2 */
    MI_U32((0x0000005cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00471ed6 */
    MI_U32((0x00000064u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00471eda */
    MI_U32((0x00000068u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00471ede */
    MI_U32((0x00000070u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00471ee2 */
    MI_U32((0x00000074u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00471ee6 */
    MI_U32((0x0000009cu+f->r[4]))=(uint32_t)(f->r[6]);
    /* 00471eed */
    if((f->flags&MI_ZF))goto mi_00471f0f;
    /* 00471eef */
    f->r[7]=f->r[6];
    /* 00471ef1 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471ef2 */
    a=f->r[7];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 00471ef5 */
    a=f->r[7];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 00471ef7 */
    a=f->r[7];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 00471efa */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471effu;f->boundary(0x00471efau,0x0046b100u,f);
    /* 00471eff */
    MI_U8((0x00799a99u+f->r[7]))=(uint8_t)((f->r[0]&0xffu));
    /* 00471f05 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00471f08 */
    MI_U16((0x00799a10u+f->r[7]))=(uint16_t)((f->r[5]&0xffffu));
mi_00471f0f:;
    /* 00471f0f */
    MI_U32((0x00000084u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 00471f16 */
    f->r[7]=(0x00000018u+f->r[4]);
    /* 00471f1a */
    goto mi_00471f1e;
mi_00471f1c:;
    /* 00471f1c */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
mi_00471f1e:;
    /* 00471f1e */
    f->r[0]=mi_read((0x00000000u+f->r[7]),4u);
    /* 00471f20 */
    f->r[3]=mi_read((0x00f55bb4u+f->r[0]*8u),4u);
    /* 00471f27 */
    f->r[0]=mi_read((0x00000084u+f->r[4]),4u);
    /* 00471f2e */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 00471f30 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00471f87;
    /* 00471f32 */
    a=mi_read((0x00000098u+f->r[4]),4u);b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 00471f39 */
    if(!(f->flags&MI_ZF))goto mi_00471f87;
    /* 00471f3b */
    f->r[5]=f->r[6];
    /* 00471f3d */
    a=f->r[5];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[5]=t;
    /* 00471f40 */
    a=f->r[5];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 00471f42 */
    a=f->r[5];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[5]=t;
    /* 00471f45 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999eau+f->r[5]),2u);
    /* 00471f4c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00471f4e */
    if((f->flags&MI_ZF))goto mi_00471f87;
    /* 00471f50 */
    a=f->r[0];b=0x0000001fu;t=mi_alu(f,a,b,32,1);
    /* 00471f53 */
    if(!mi_signed_less(f->flags))goto mi_00471f61;
    /* 00471f55 */
    f->r[1]=mi_read((0x00f566bcu),4u);
    /* 00471f5b */
    f->r[3]=(0xfffffffcu+f->r[1]+f->r[0]*4u);
    /* 00471f5f */
    goto mi_00471f6b;
mi_00471f61:;
    /* 00471f61 */
    f->r[2]=mi_read((0x00f57204u),4u);
    /* 00471f67 */
    f->r[3]=(0xffffff84u+f->r[2]+f->r[0]*4u);
mi_00471f6b:;
    /* 00471f6b */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471f6d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471f6e */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471f73u;f->boundary(0x00471f6eu,0x0047d350u,f);
    /* 00471f73 */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 00471f75 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00799a14u+f->r[5]),2u);
    /* 00471f7c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471f7d */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471f82u;f->boundary(0x00471f7du,0x0045b490u,f);
    /* 00471f82 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00471f85 */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[3]=t;
mi_00471f87:;
    /* 00471f87 */
    f->r[0]=mi_read((0x00000094u+f->r[4]),4u);
    /* 00471f8e */
    f->r[1]=mi_read((0xfffffffcu+f->r[7]),4u);
    /* 00471f91 */
    f->r[2]=mi_read((0xfffffff8u+f->r[7]),4u);
    /* 00471f94 */
    f->r[5]=mi_read((0x0000008cu+f->r[4]),4u);
    /* 00471f9b */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471f9d */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471f9e */
    f->r[3]=mi_read((0x00000098u+f->r[4]),4u);
    /* 00471fa5 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471fa6 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00471fa8 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471fa9 */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00471fab */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471fac */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471fad */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471fae */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471fb3u;f->boundary(0x00471faeu,0x0047bf50u,f);
    /* 00471fb3 */
    f->r[0]=mi_read((0x000000a0u+f->r[4]),4u);
    /* 00471fba */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00471fbd */
    a=f->r[0];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 00471fc0 */
    if(!mi_signed_less(f->flags))goto mi_00472034;
    /* 00471fc2 */
    f->r[0]=mi_read((0x00000098u+f->r[4]),4u);
    /* 00471fc9 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00471fcb */
    if((f->flags&MI_ZF))goto mi_0047201e;
    /* 00471fcd */
    f->r[0]=mi_read((0x00000008u+f->r[7]),4u);
    /* 00471fd0 */
    f->r[1]=mi_read((0x00000004u+f->r[7]),4u);
    /* 00471fd3 */
    f->r[2]=mi_read((0x00000080u+f->r[4]),4u);
    /* 00471fda */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471fdc */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00471fde */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471fe0 */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00471fe2 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471fe3 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471fe4 */
    a=0x00000049u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471fe6 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471fe7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471fecu;f->boundary(0x00471fe7u,0x0046a400u,f);
    /* 00471fec */
    f->r[6]=f->r[0];
    /* 00471fee */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00471fef */
    f->r[4]-=4;MI_U32(f->r[4])=0x00471ff4u;f->boundary(0x00471fefu,0x00485100u,f);
    /* 00471ff4 */
    f->r[0]=mi_read((0x000000b8u+f->r[4]),4u);
    /* 00471ffb */
    f->r[1]=f->r[6];
    /* 00471ffd */
    f->r[2]=f->r[0];
    /* 00471fff */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00472002 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00472005 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 00472008 */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0047200a */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0047200c */
    MI_U16((0x00799a10u+f->r[1]*8u))=(uint16_t)((f->r[0]&0xffffu));
    /* 00472014 */
    MI_U16((0x00799a12u+f->r[2]*8u))=(uint16_t)((f->r[6]&0xffffu));
    /* 0047201c */
    goto mi_0047202d;
mi_0047201e:;
    /* 0047201e */
    f->r[0]=f->r[6];
    /* 00472020 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00472023 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00472025 */
    f->r[6]=(uint32_t)(int32_t)(int16_t)mi_read((0x00799a12u+f->r[0]*8u),2u);
mi_0047202d:;
    /* 0047202d */
    MI_U32((0x0000009cu+f->r[4]))=(uint32_t)(f->r[6]);
mi_00472034:;
    /* 00472034 */
    f->r[0]=mi_read((0x00000084u+f->r[4]),4u);
    /* 0047203b */
    a=f->r[7];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0047203e */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0047203f */
    a=f->r[0];b=0x00000009u;t=mi_alu(f,a,b,32,1);
    /* 00472042 */
    MI_U32((0x00000084u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00472049 */
    if(mi_signed_less(f->flags))goto mi_00471f1c;
    /* 0047204f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00472050 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00472051 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00472052 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00472053 */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00472056 */
    f->r[4]+=4;return;
mi_00472057:;
    /* 00472057 */
    f->r[1]=mi_read((0x00eaac28u),4u);
    /* 0047205d */
    f->r[2]=mi_read((0x00f56594u),4u);
    /* 00472063 */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 00472069 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047206b */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047206c */
    MI_U16((0x00ea5cf6u+f->r[1]))=(uint16_t)(0x00000001u);
    /* 00472075 */
    goto mi_00472125;
mi_0047207a:;
    /* 0047207a */
    f->r[0]=mi_read((0x00eaac28u),4u);
    /* 0047207f */
    f->r[1]=mi_read((0x00f56554u),4u);
    /* 00472085 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0047208b */
    f->r[7]=mi_read((0x00000090u+f->r[4]),4u);
    /* 00472092 */
    f->r[3]=mi_read((0x0000008cu+f->r[4]),4u);
    /* 00472099 */
    MI_U16((0x00ea5cf6u+f->r[0]))=(uint16_t)(0x00000001u);
    /* 004720a2 */
    f->r[0]=mi_read((0x00000094u+f->r[4]),4u);
    /* 004720a9 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004720ab */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004720ac */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004720ad */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004720ae */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004720af */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004720b0 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004720b1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004720b6u;f->boundary(0x004720b1u,0x0047bf50u,f);
    /* 004720b6 */
    f->r[0]=mi_read((0x00eaac28u),4u);
    /* 004720bb */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004720be */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004720c4 */
    f->r[1]=mi_read((0x00ea3ffcu+f->r[0]),4u);
    /* 004720ca */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004720cc */
    if(!(f->flags&MI_ZF))goto mi_004724ea;
    /* 004720d2 */
    f->r[2]=(0x00000000u+f->r[7]+f->r[7]*8u);
    /* 004720d5 */
    MI_U32((0x00ea4000u+f->r[0]))=(uint32_t)(f->r[6]);
    /* 004720db */
    MI_U16((0x00ea3ff8u+f->r[0]))=(uint16_t)((f->r[3]&0xffffu));
    /* 004720e2 */
    MI_U16((0x00ea3ffau+f->r[0]))=(uint16_t)((f->r[7]&0xffffu));
    /* 004720e9 */
    f->r[1]=(0x00000000u+f->r[7]+f->r[2]*2u);
    /* 004720ec */
    f->r[2]=mi_read((0x00bca2d8u),4u);
    /* 004720f2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004720f3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004720f4 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 004720f7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004720f8 */
    f->r[1]=(0x00000000u+f->r[2]+f->r[1]*4u);
    /* 004720fb */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004720fd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004720fe */
    MI_U32((0x00ea3ffcu+f->r[0]))=(uint32_t)(f->r[1]);
    /* 00472104 */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00472107 */
    f->r[4]+=4;return;
mi_00472108:;
    /* 00472108 */
    f->r[2]=mi_read((0x00eaac28u),4u);
    /* 0047210e */
    f->r[0]=mi_read((0x00f564acu),4u);
    /* 00472113 */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 00472119 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047211b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047211c */
    MI_U16((0x00ea5cf6u+f->r[2]))=(uint16_t)(0x00000001u);
mi_00472125:;
    /* 00472125 */
    f->r[0]=mi_read((0x0000009cu+f->r[4]),4u);
    /* 0047212c */
    f->r[7]=mi_read((0x00000098u+f->r[4]),4u);
    /* 00472133 */
    f->r[3]=mi_read((0x00000094u+f->r[4]),4u);
    /* 0047213a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047213b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047213c */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047213d */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047213e */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047213f */
    f->r[4]-=4;MI_U32(f->r[4])=0x00472144u;f->boundary(0x0047213fu,0x0047bf50u,f);
    /* 00472144 */
    f->r[0]=mi_read((0x00eaac28u),4u);
    /* 00472149 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047214c */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 00472152 */
    f->r[1]=mi_read((0x00ea3ffcu+f->r[0]),4u);
    /* 00472158 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0047215a */
    if(!(f->flags&MI_ZF))goto mi_004724ea;
    /* 00472160 */
    f->r[1]=(0x00000000u+f->r[7]+f->r[7]*8u);
    /* 00472163 */
    MI_U32((0x00ea4000u+f->r[0]))=(uint32_t)(f->r[6]);
    /* 00472169 */
    MI_U16((0x00ea3ff8u+f->r[0]))=(uint16_t)((f->r[3]&0xffffu));
    /* 00472170 */
    MI_U16((0x00ea3ffau+f->r[0]))=(uint16_t)((f->r[7]&0xffffu));
    /* 00472177 */
    f->r[1]=(0x00000000u+f->r[7]+f->r[1]*2u);
    /* 0047217a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0047217b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0047217c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0047217d */
    f->r[2]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 00472180 */
    f->r[1]=mi_read((0x00bca2d8u),4u);
    /* 00472186 */
    f->r[2]=(0x00000000u+f->r[1]+f->r[2]*4u);
    /* 00472189 */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0047218b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0047218c */
    MI_U32((0x00ea3ffcu+f->r[0]))=(uint32_t)(f->r[2]);
    /* 00472192 */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00472195 */
    f->r[4]+=4;return;
mi_00472196:;
    /* 00472196 */
    f->r[7]=mi_read((0x00000090u+f->r[4]),4u);
    /* 0047219d */
    f->r[3]=mi_read((0x0000008cu+f->r[4]),4u);
    /* 004721a4 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004721a6 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004721a7 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004721a8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004721adu;f->boundary(0x004721a8u,0x0047d060u,f);
    /* 004721ad */
    f->r[0]=mi_read((0x000000a4u+f->r[4]),4u);
    /* 004721b4 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004721b7 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004721b9 */
    if((f->flags&MI_ZF))goto mi_004721c1;
    /* 004721bb */
    a=mi_read((0x00705c1cu),4u);b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;MI_U32((0x00705c1cu))=(uint32_t)(t);
mi_004721c1:;
    /* 004721c1 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bc96d8u),1u))&0xffu)<<0);
    /* 004721c7 */
    f->r[0]=f->r[6];
    /* 004721c9 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004721cc */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004721ce */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004721cf */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004721d0 */
    MI_U8((0x00799a82u+f->r[0]*8u))=(uint8_t)((f->r[1]&0xffu));
    /* 004721d7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004721dcu;f->boundary(0x004721d7u,0x0047a1c0u,f);
    /* 004721dc */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004721df */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004721e1 */
    if((f->flags&MI_ZF))goto mi_00472204;
    /* 004721e3 */
    f->r[2]=mi_read((0x00bc96dcu),4u);
    /* 004721e9 */
    f->r[0]=mi_read((0x00f5727cu),4u);
    /* 004721ee */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004721f0 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004721f1 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004721f2 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004721f3 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004721f4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004721f9u;f->boundary(0x004721f4u,0x0047d1f0u,f);
    /* 004721f9 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004721fc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004721fd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004721fe */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004721ff */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00472200 */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00472203 */
    f->r[4]+=4;return;
mi_00472204:;
    /* 00472204 */
    f->r[1]=mi_read((0x00bc96dcu),4u);
    /* 0047220a */
    f->r[2]=mi_read((0x00f56cc4u),4u);
    /* 00472210 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[1]*1u);
    /* 00472213 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472214 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472215 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472216 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472217 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047221cu;f->boundary(0x00472217u,0x0047d1f0u,f);
    /* 0047221c */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047221f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00472220 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00472221 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00472222 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00472223 */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00472226 */
    f->r[4]+=4;return;
mi_00472227:;
    /* 00472227 */
    f->r[0]=mi_read((0x00000098u+f->r[4]),4u);
    /* 0047222e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00472230 */
    if((f->flags&MI_ZF))goto mi_00472261;
    /* 00472232 */
    f->r[0]=mi_read((0x00000090u+f->r[4]),4u);
    /* 00472239 */
    f->r[1]=mi_read((0x0000008cu+f->r[4]),4u);
    /* 00472240 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472242 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472244 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472245 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472246 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047224bu;f->boundary(0x00472246u,0x0047cc70u,f);
    /* 0047224b */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bc96d8u),1u))&0xffu)<<0);
    /* 00472250 */
    f->r[2]=f->r[6];
    /* 00472252 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 00472255 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00472258 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0047225a */
    MI_U8((0x00799a82u+f->r[2]*8u))=(uint8_t)((f->r[0]&0xffu));
mi_00472261:;
    /* 00472261 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472262 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00472267u;f->boundary(0x00472262u,0x00487ec0u,f);
    /* 00472267 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047226a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0047226b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0047226c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0047226d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0047226e */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00472271 */
    f->r[4]+=4;return;
mi_00472272:;
    /* 00472272 */
    f->r[0]=mi_read((0x00000090u+f->r[4]),4u);
    /* 00472279 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 0047227c */
    f->r[1]=(0x00000000u+f->r[0]+f->r[1]*2u);
    /* 0047227f */
    f->r[2]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 00472282 */
    f->r[1]=mi_read((0x00bca2d8u),4u);
    /* 00472288 */
    f->r[2]=(0x00000000u+f->r[1]+f->r[2]*4u);
    /* 0047228b */
    f->r[1]=mi_read((0x0000008cu+f->r[4]),4u);
    /* 00472292 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00472294 */
    f->r[2]=mi_read((0x00af598cu+f->r[2]*4u),4u);
    /* 0047229b */
    a=f->r[2];b=0x00000100u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004722a1 */
    a=f->r[2];b=0x00000048u;t=mi_alu(f,a,b,32,4);f->r[2]=t;
    /* 004722a4 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004722a5 */
    f->r[2]=mi_read((0x00f5672cu),4u);
    /* 004722ab */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004722ac */
    f->r[2]=mi_read((0x0000009cu+f->r[4]),4u);
    /* 004722b3 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004722b4 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004722b5 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004722b6 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004722b7 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004722b8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004722bdu;f->boundary(0x004722b8u,0x0047bf50u,f);
    /* 004722bd */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004722c0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004722c1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004722c2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004722c3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004722c4 */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004722c7 */
    f->r[4]+=4;return;
mi_004722c8:;
    /* 004722c8 */
    f->r[0]=(0x00000000u+f->r[3]+f->r[3]*4u);
    /* 004722cb */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004722cd */
    f->r[1]=mi_read((0x005d06e4u+f->r[0]*4u),4u);
    /* 004722d4 */
    f->r[2]=mi_read((0x00f55bb4u+f->r[1]*8u),4u);
    /* 004722db */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004722dc */
    goto mi_004723bb;
mi_004722e1:;
    /* 004722e1 */
    f->r[2]=mi_read((0x00f5668cu),4u);
    /* 004722e7 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004722e9 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004722ea */
    goto mi_004723bb;
mi_004722ef:;
    /* 004722ef */
    f->r[2]=mi_read((0x00f5660cu),4u);
    /* 004722f5 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004722f7 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004722f8 */
    goto mi_004723bb;
mi_004722fd:;
    /* 004722fd */
    f->r[7]=mi_read((0x00000098u+f->r[4]),4u);
    /* 00472304 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 00472306 */
    if((f->flags&MI_ZF))goto mi_00472331;
    /* 00472308 */
    f->r[2]=mi_read((0x00000094u+f->r[4]),4u);
    /* 0047230f */
    f->r[0]=mi_read((0x00000090u+f->r[4]),4u);
    /* 00472316 */
    f->r[1]=mi_read((0x0000008cu+f->r[4]),4u);
    /* 0047231d */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047231e */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047231f */
    f->r[2]=mi_read((0x00000088u+f->r[4]),4u);
    /* 00472326 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472327 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472328 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472329 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047232eu;f->boundary(0x00472329u,0x004ee780u,f);
    /* 0047232e */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00472331:;
    /* 00472331 */
    f->r[0]=f->r[6];
    /* 00472333 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00472336 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00472338 */
    a=mi_read((0x00799a10u+f->r[0]*8u),2u);b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 00472341 */
    if(!(f->flags&MI_ZF))goto mi_0047234c;
    /* 00472343 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472344 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00472349u;f->boundary(0x00472344u,0x004ee4a0u,f);
    /* 00472349 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0047234c:;
    /* 0047234c */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 0047234e */
    if((f->flags&MI_ZF))goto mi_004724ea;
    /* 00472354 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00472359u;f->boundary(0x00472354u,0x00405ba0u,f);
    /* 00472359 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0047235a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0047235b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0047235c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0047235d */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00472360 */
    f->r[4]+=4;return;
mi_00472361:;
    /* 00472361 */
    f->r[0]=mi_read((0x00bca2e0u),4u);
    /* 00472366 */
    f->r[7]=mi_read((0x00f57374u),4u);
    /* 0047236c */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0047236d */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0047236f */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472371 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 00472373 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 00472376 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
mi_00472378:;
    /* 00472378 */
    f->r[1]=mi_read((0x00000094u+f->r[4]),4u);
    /* 0047237f */
    f->r[2]=mi_read((0x00000090u+f->r[4]),4u);
    /* 00472386 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472387 */
    f->r[0]=mi_read((0x0000009cu+f->r[4]),4u);
    /* 0047238e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047238f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472390 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472391 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472392 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472393 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00472398u;f->boundary(0x00472393u,0x0047bf50u,f);
    /* 00472398 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047239b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0047239c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0047239d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0047239e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0047239f */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004723a2 */
    f->r[4]+=4;return;
mi_004723a3:;
    /* 004723a3 */
    f->r[0]=mi_read((0x00bca2e0u),4u);
    /* 004723a8 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004723aa */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004723ab */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004723ad */
    f->r[2]=mi_read((0x00f5737cu),4u);
    /* 004723b3 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004723b5 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004723b8 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
mi_004723ba:;
    /* 004723ba */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
mi_004723bb:;
    /* 004723bb */
    f->r[0]=mi_read((0x0000009cu+f->r[4]),4u);
    /* 004723c2 */
    f->r[1]=mi_read((0x00000094u+f->r[4]),4u);
    /* 004723c9 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004723ca */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004723cb */
    f->r[0]=mi_read((0x000000a0u+f->r[4]),4u);
    /* 004723d2 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004723d3 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004723d4 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004723d5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004723dau;f->boundary(0x004723d5u,0x0047bf50u,f);
    /* 004723da */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004723dd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004723de */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004723df */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004723e0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004723e1 */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004723e4 */
    f->r[4]+=4;return;
mi_004723e5:;
    /* 004723e5 */
    f->r[0]=mi_read((0x00bca2e0u),4u);
    /* 004723ea */
    f->r[5]=mi_read((0x00f57384u),4u);
    /* 004723f0 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004723f1 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004723f3 */
    f->r[2]=mi_read((0x00000090u+f->r[4]),4u);
    /* 004723fa */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004723fc */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004723fe */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004723ff */
    f->r[0]=mi_read((0x00000090u+f->r[4]),4u);
    /* 00472406 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472407 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472408 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472409 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047240eu;f->boundary(0x00472409u,0x0047cf00u,f);
    /* 0047240e */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00472411 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00472412 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00472413 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00472414 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00472415 */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00472418 */
    f->r[4]+=4;return;
mi_00472419:;
    /* 00472419 */
    f->r[1]=mi_read((0x00f56b3cu),4u);
    /* 0047241f */
    a=0x00000048u;f->r[4]-=4;MI_U32(f->r[4])=a;
mi_00472421:;
    /* 00472421 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
mi_00472422:;
    /* 00472422 */
    f->r[0]=mi_read((0x0000009cu+f->r[4]),4u);
    /* 00472429 */
    f->r[2]=mi_read((0x00000098u+f->r[4]),4u);
    /* 00472430 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472431 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472432 */
    f->r[0]=mi_read((0x0000009cu+f->r[4]),4u);
    /* 00472439 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047243a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047243b */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047243c */
    f->r[4]-=4;MI_U32(f->r[4])=0x00472441u;f->boundary(0x0047243cu,0x0047bf50u,f);
    /* 00472441 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00472444 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00472445 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00472446 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00472447 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00472448 */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047244b */
    f->r[4]+=4;return;
mi_0047244c:;
    /* 0047244c */
    f->r[0]=f->r[6];
    /* 0047244e */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00472451 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00472453 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00472456 */
    a=mi_read((0x007999d8u+f->r[0]),1u);b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 0047245d */
    if(!(f->flags&MI_ZF))goto mi_0047248a;
    /* 0047245f */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999deu+f->r[0]),2u);
    /* 00472466 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 00472468 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x007999dbu+f->r[0]),1u))&0xffu)<<0);
    /* 0047246e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047246f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472470 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e0u+f->r[0]),2u);
    /* 00472477 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472478 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472479 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047247a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047247fu;f->boundary(0x0047247au,0x0047c2b0u,f);
    /* 0047247f */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00472482 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00472483 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00472484 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00472485 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00472486 */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00472489 */
    f->r[4]+=4;return;
mi_0047248a:;
    /* 0047248a */
    f->r[1]=mi_read((0x007999e4u+f->r[0]),4u);
    /* 00472490 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x007999ddu+f->r[0]),1u))&0xffu)<<0);
    /* 00472496 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00acf85cu+f->r[1]),1u))&0xffu)<<0);
    /* 0047249c */
    a=f->r[1];b=0x00000003u;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 0047249f */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004724a1 */
    if((f->flags&MI_ZF))goto mi_004724b5;
    /* 004724a3 */
    a=mi_read((0x007999eau+f->r[0]),2u);b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 004724ab */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004724b5;
    /* 004724ad */
    f->r[1]=mi_read((0x00f56c54u),4u);
    /* 004724b3 */
    goto mi_004724ca;
mi_004724b5:;
    /* 004724b5 */
    a=f->r[1];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 004724b8 */
    if(mi_signed_less(f->flags))goto mi_004724bf;
    /* 004724ba */
    f->r[1]=0x00000003u;
mi_004724bf:;
    /* 004724bf */
    f->r[2]=mi_read((0x00f56c5cu),4u);
    /* 004724c5 */
    f->r[2]=(0x00000000u+f->r[2]+f->r[1]*8u);
    /* 004724c8 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
mi_004724ca:;
    /* 004724ca */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999deu+f->r[0]),2u);
    /* 004724d1 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004724d3 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004724d4 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e0u+f->r[0]),2u);
    /* 004724db */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004724dd */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004724df */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004724e0 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004724e1 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004724e2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004724e7u;f->boundary(0x004724e2u,0x0047bf50u,f);
    /* 004724e7 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004724ea:;
    /* 004724ea */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004724eb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004724ec */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004724ed */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004724ee */
    a=f->r[4];b=0x0000006cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004724f1 */
    f->r[4]+=4;return;
}
