/* Complete integer bodies from pinned Pharaoh SHA256 fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d.
 * Native inclusive bounds:454190..454ae7,45d570..45d5cc,475b50..475b78,4b7590..4b75bb,50f0f0..50f319.
 * The only embedded code-section data is the reviewed 19-DWORD table454ae8. */
#include "menu_prepare_integer.h"
#include <stdio.h>
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
static __inline uint32_t mi_alu(struct restore_prepare_integer_frame *f,uint32_t a,uint32_t b,uint32_t width,uint32_t operation)
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
static __inline uint32_t mi_shift(struct restore_prepare_integer_frame *f,uint32_t a,uint32_t count,uint32_t width,uint32_t arithmetic)
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
static __inline uint32_t mi_imul(struct restore_prepare_integer_frame *f,uint32_t a,uint32_t b)
{
 int64_t product=(int64_t)(int32_t)a*(int32_t)b;uint32_t result=(uint32_t)product;
 f->flags&=~(MI_CF|MI_OF);if(product!=(int64_t)(int32_t)result)f->flags|=MI_CF|MI_OF;
 return result;
}
static __inline int mi_signed_less(uint32_t flags){return ((flags&MI_SF)!=0)^((flags&MI_OF)!=0);}

static const uint8_t mi_rom_00454ae8[]={0x17,0x43,0x45,0x00,0x22,0x43,0x45,0x00,0x29,0x43,0x45,0x00,0x30,0x43,0x45,0x00,0x37,0x43,0x45,0x00,0x1b,0x43,0x45,0x00,0x45,0x43,0x45,0x00,0x3e,0x43,0x45,0x00,0x68,0x43,0x45,0x00,0x68,0x43,0x45,0x00,0x68,0x43,0x45,0x00,0x68,0x43,0x45,0x00,0x61,0x43,0x45,0x00,0x68,0x43,0x45,0x00,0x5a,0x43,0x45,0x00,0x68,0x43,0x45,0x00,0x4c,0x43,0x45,0x00,0x68,0x43,0x45,0x00,0x53,0x43,0x45,0x00};
static __inline uint32_t mi_read(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x00454ae8u&&width<=76u&&(offset=address-0x00454ae8u)<=76u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00454ae8[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}


void restore_prepare_C_00454190(struct restore_prepare_integer_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00454190 */
    f->r[0]=mi_read((0x00000004u+f->r[4]),4u);
    /* 00454194 */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 00454197 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00454198 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00454199 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045419a */
    f->r[6]=f->r[0];
    /* 0045419c */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0045419f */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004541a1 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004541a2 */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004541a5 */
    f->r[5]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999eau+f->r[6]),2u);
    /* 004541ac */
    f->r[0]=(uint32_t)(int32_t)(int8_t)mi_read((0x00799a96u+f->r[6]),1u);
    /* 004541b3 */
    f->r[1]=(0x00000000u+f->r[5]+f->r[5]*4u);
    /* 004541b7 */
    f->r[1]=(0x00000000u+f->r[5]+f->r[1]*2u);
    /* 004541bb */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004541be */
    a=f->r[0];b=mi_read((0x0070cdccu+f->r[1]),4u);t=mi_alu(f,a,b,32,1);
    /* 004541c4 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004541d0;
    /* 004541c6 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(0xffffffffu);
    /* 004541ce */
    goto mi_004541e1;
mi_004541d0:;
    /* 004541d0 */
    f->r[7]=mi_read((0x0070cdd0u+f->r[1]),4u);
    /* 004541d6 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004541d8 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004541da */
    t=(!mi_signed_less(f->flags))?1u:0u;f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004541dd */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[2]);
mi_004541e1:;
    /* 004541e1 */
    f->r[0]=mi_read((0x0070cdd8u+f->r[1]),4u);
    /* 004541e7 */
    f->r[2]=mi_read((0x0070cdd4u+f->r[1]),4u);
    /* 004541ed */
    MI_U32((0x00000040u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004541f1 */
    f->r[0]=mi_read((0x0070cde0u+f->r[1]),4u);
    /* 004541f7 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((mi_read((0x007999dbu+f->r[6]),1u))&0xffu)<<0);
    /* 004541fd */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00454201 */
    f->r[2]=mi_read((0x0070cddcu+f->r[1]),4u);
    /* 00454207 */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0045420b */
    f->r[0]=mi_read((0x0070cde8u+f->r[1]),4u);
    /* 00454211 */
    MI_U32((0x00000030u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00454215 */
    f->r[2]=mi_read((0x0070cdecu+f->r[1]),4u);
    /* 0045421b */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0045421f */
    f->r[0]=mi_read((0x0070cdf0u+f->r[1]),4u);
    /* 00454225 */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00454229 */
    f->r[2]=mi_read((0x0070cdf4u+f->r[1]),4u);
    /* 0045422f */
    MI_U32((0x00000038u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00454233 */
    f->r[0]=mi_read((0x0070cdf8u+f->r[1]),4u);
    /* 00454239 */
    a=(f->r[3]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 0045423c */
    MI_U32((0x00000044u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00454240 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00454244 */
    f->r[7]=0x00000001u;
    /* 00454249 */
    if(!(f->flags&MI_ZF))goto mi_00454267;
    /* 0045424b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0045424d */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00454267;
    /* 0045424f */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00454252 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 00454253 */
    a=f->r[2];b=0x00000003u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 00454256 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00454258 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 0045425b */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 0045425d */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00454261 */
    if(!mi_signed_less(f->flags))goto mi_00454267;
    /* 00454263 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[7]);
mi_00454267:;
    /* 00454267 */
    f->r[0]=mi_read((0x0070cdfcu+f->r[1]),4u);
    /* 0045426d */
    a=(f->r[3]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 00454270 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00454274 */
    if(!(f->flags&MI_ZF))goto mi_00454292;
    /* 00454276 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00454278 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00454292;
    /* 0045427a */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0045427d */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0045427e */
    a=f->r[2];b=0x00000003u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 00454281 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00454283 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 00454286 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 00454288 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0045428c */
    if(!mi_signed_less(f->flags))goto mi_00454292;
    /* 0045428e */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[7]);
mi_00454292:;
    /* 00454292 */
    f->r[0]=mi_read((0x0070ce00u+f->r[1]),4u);
    /* 00454298 */
    a=(f->r[3]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 0045429b */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0045429f */
    if(!(f->flags&MI_ZF))goto mi_004542bd;
    /* 004542a1 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004542a3 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004542bd;
    /* 004542a5 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004542a8 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004542a9 */
    a=f->r[2];b=0x00000003u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004542ac */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004542ae */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004542b1 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004542b3 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004542b7 */
    if(!mi_signed_less(f->flags))goto mi_004542bd;
    /* 004542b9 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[7]);
mi_004542bd:;
    /* 004542bd */
    f->r[0]=mi_read((0x0070ce04u+f->r[1]),4u);
    /* 004542c3 */
    a=(f->r[3]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004542c6 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004542ca */
    if(!(f->flags&MI_ZF))goto mi_004542ea;
    /* 004542cc */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004542ce */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004542ea;
    /* 004542d0 */
    f->r[1]=f->r[0];
    /* 004542d2 */
    f->r[0]=(0x00000002u+f->r[1]);
    /* 004542d5 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004542d6 */
    a=f->r[2];b=0x00000003u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004542d9 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004542db */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004542de */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004542e0 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004542e4 */
    if(!mi_signed_less(f->flags))goto mi_004542ea;
    /* 004542e6 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[7]);
mi_004542ea:;
    /* 004542ea */
    f->r[0]=mi_read((0x00f53dd8u),4u);
    /* 004542ef */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004542f1 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004542f3 */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(f->r[3]);
    /* 004542f7 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00454393;
    /* 004542fd */
    f->r[2]=0x00f53ddcu;
    /* 00454302 */
    f->r[7]=f->r[0];
mi_00454304:;
    /* 00454304 */
    f->r[0]=mi_read((0x00000000u+f->r[2]),4u);
    /* 00454306 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00454308 */
    if((f->flags&MI_ZF))goto mi_00454385;
    /* 0045430a */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0045430b */
    a=f->r[0];b=0x00000012u;t=mi_alu(f,a,b,32,1);
    /* 0045430e */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00454368;
    /* 00454310 */
    switch(mi_read((0x00454ae8u+f->r[0]*4u),4u)){case 0x00454317u:goto mi_00454317;case 0x0045431bu:goto mi_0045431b;case 0x00454322u:goto mi_00454322;case 0x00454329u:goto mi_00454329;case 0x00454330u:goto mi_00454330;case 0x00454337u:goto mi_00454337;case 0x0045433eu:goto mi_0045433e;case 0x00454345u:goto mi_00454345;case 0x0045434cu:goto mi_0045434c;case 0x00454353u:goto mi_00454353;case 0x0045435au:goto mi_0045435a;case 0x00454361u:goto mi_00454361;case 0x00454368u:goto mi_00454368;default:abort();}
mi_00454317:;
    /* 00454317 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00454319 */
    goto mi_0045436b;
mi_0045431b:;
    /* 0045431b */
    f->r[0]=0x00000005u;
    /* 00454320 */
    goto mi_0045436b;
mi_00454322:;
    /* 00454322 */
    f->r[0]=0x00000001u;
    /* 00454327 */
    goto mi_0045436b;
mi_00454329:;
    /* 00454329 */
    f->r[0]=0x00000002u;
    /* 0045432e */
    goto mi_0045436b;
mi_00454330:;
    /* 00454330 */
    f->r[0]=0x00000003u;
    /* 00454335 */
    goto mi_0045436b;
mi_00454337:;
    /* 00454337 */
    f->r[0]=0x00000004u;
    /* 0045433c */
    goto mi_0045436b;
mi_0045433e:;
    /* 0045433e */
    f->r[0]=0x00000007u;
    /* 00454343 */
    goto mi_0045436b;
mi_00454345:;
    /* 00454345 */
    f->r[0]=0x00000006u;
    /* 0045434a */
    goto mi_0045436b;
mi_0045434c:;
    /* 0045434c */
    f->r[0]=0x0000000au;
    /* 00454351 */
    goto mi_0045436b;
mi_00454353:;
    /* 00454353 */
    f->r[0]=0x0000000bu;
    /* 00454358 */
    goto mi_0045436b;
mi_0045435a:;
    /* 0045435a */
    f->r[0]=0x00000008u;
    /* 0045435f */
    goto mi_0045436b;
mi_00454361:;
    /* 00454361 */
    f->r[0]=0x00000009u;
    /* 00454366 */
    goto mi_0045436b;
mi_00454368:;
    /* 00454368 */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[0]=t;
mi_0045436b:;
    /* 0045436b */
    f->r[1]=mi_read((0x0000004cu+f->r[4]),4u);
    /* 0045436f */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00454372 */
    a=f->r[1];b=mi_read((0x0000004cu+f->r[4]),4u);t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00454376 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[1]*4u);
    /* 00454379 */
    a=mi_read((0x00799a2cu+f->r[0]*2u),2u);b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 00454382 */
    if((f->flags&MI_ZF))goto mi_00454385;
    /* 00454384 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
mi_00454385:;
    /* 00454385 */
    a=f->r[2];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00454388 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 00454389 */
    if(!(f->flags&MI_ZF))goto mi_00454304;
    /* 0045438f */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(f->r[3]);
mi_00454393:;
    /* 00454393 */
    a=mi_read((0x00000020u+f->r[4]),4u);b=0xffffffffu;t=mi_alu(f,a,b,32,1);
    /* 00454398 */
    if(!(f->flags&MI_ZF))goto mi_004543a9;
    /* 0045439a */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x00000000u);
    /* 004543a1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004543a2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004543a3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004543a4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004543a5 */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004543a8 */
    f->r[4]+=4;return;
mi_004543a9:;
    /* 004543a9 */
    f->r[0]=mi_read((0x00000040u+f->r[4]),4u);
    /* 004543ad */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004543b0 */
    if(!(f->flags&MI_ZF))goto mi_004543d5;
    /* 004543b2 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00799a0du+f->r[6]),1u))&0xffu)<<0);
    /* 004543b8 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004543ba */
    if(!(f->flags&MI_ZF))goto mi_004543f3;
    /* 004543bc */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00799a17u+f->r[6]),1u))&0xffu)<<0);
    /* 004543c2 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004543c4 */
    if(!(f->flags&MI_ZF))goto mi_004543f3;
    /* 004543c6 */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x00000001u);
    /* 004543cd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004543ce */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004543cf */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004543d0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004543d1 */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004543d4 */
    f->r[4]+=4;return;
mi_004543d5:;
    /* 004543d5 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 004543d8 */
    if(!(f->flags&MI_ZF))goto mi_004543f3;
    /* 004543da */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00799a0du+f->r[6]),1u))&0xffu)<<0);
    /* 004543e0 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004543e2 */
    if(!(f->flags&MI_ZF))goto mi_004543f3;
    /* 004543e4 */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x00000002u);
    /* 004543eb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004543ec */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004543ed */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004543ee */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004543ef */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004543f2 */
    f->r[4]+=4;return;
mi_004543f3:;
    /* 004543f3 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00799a58u+f->r[6]),1u))&0xffu)<<0);
    /* 004543f9 */
    f->r[1]=mi_read((0x00000024u+f->r[4]),4u);
    /* 004543fd */
    f->r[2]=(uint32_t)(int32_t)(int8_t)(f->r[0]&0xffu);
    /* 00454400 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 00454402 */
    MI_U32((0x00000040u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00454406 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0045447a;
    /* 00454408 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0045440a */
    if(!(f->flags&MI_ZF))goto mi_0045441b;
    /* 0045440c */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x00000003u);
    /* 00454413 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00454414 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00454415 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00454416 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00454417 */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0045441a */
    f->r[4]+=4;return;
mi_0045441b:;
    /* 0045441b */
    a=f->r[1];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 0045441e */
    if(!mi_signed_less(f->flags))goto mi_0045442f;
    /* 00454420 */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x00000004u);
    /* 00454427 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00454428 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00454429 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0045442a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0045442b */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0045442e */
    f->r[4]+=4;return;
mi_0045442f:;
    /* 0045442f */
    a=f->r[1];b=0x00000019u;t=mi_alu(f,a,b,32,1);
    /* 00454432 */
    if(!mi_signed_less(f->flags))goto mi_00454443;
    /* 00454434 */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x00000005u);
    /* 0045443b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0045443c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0045443d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0045443e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0045443f */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00454442 */
    f->r[4]+=4;return;
mi_00454443:;
    /* 00454443 */
    a=f->r[1];b=0x00000032u;t=mi_alu(f,a,b,32,1);
    /* 00454446 */
    if(!mi_signed_less(f->flags))goto mi_00454457;
    /* 00454448 */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x00000006u);
    /* 0045444f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00454450 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00454451 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00454452 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00454453 */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00454456 */
    f->r[4]+=4;return;
mi_00454457:;
    /* 00454457 */
    a=f->r[1];b=0x00000050u;t=mi_alu(f,a,b,32,1);
    /* 0045445a */
    if(!mi_signed_less(f->flags))goto mi_0045446b;
    /* 0045445c */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x00000007u);
    /* 00454463 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00454464 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00454465 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00454466 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00454467 */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0045446a */
    f->r[4]+=4;return;
mi_0045446b:;
    /* 0045446b */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x00000008u);
    /* 00454472 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00454473 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00454474 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00454475 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00454476 */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00454479 */
    f->r[4]+=4;return;
mi_0045447a:;
    /* 0045447a */
    f->r[0]=mi_read((0x00000044u+f->r[4]),4u);
    /* 0045447e */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 00454480 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004544be;
    /* 00454482 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 00454485 */
    if(!(f->flags&MI_ZF))goto mi_00454496;
    /* 00454487 */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x00000009u);
    /* 0045448e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0045448f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00454490 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00454491 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00454492 */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00454495 */
    f->r[4]+=4;return;
mi_00454496:;
    /* 00454496 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 00454499 */
    if(!(f->flags&MI_ZF))goto mi_004544aa;
    /* 0045449b */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x0000000au);
    /* 004544a2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004544a3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004544a4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004544a5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004544a6 */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004544a9 */
    f->r[4]+=4;return;
mi_004544aa:;
    /* 004544aa */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 004544ad */
    if(!(f->flags&MI_ZF))goto mi_004544be;
    /* 004544af */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x0000000bu);
    /* 004544b6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004544b7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004544b8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004544b9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004544ba */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004544bd */
    f->r[4]+=4;return;
mi_004544be:;
    /* 004544be */
    f->r[0]=(uint32_t)(int32_t)(int8_t)mi_read((0x00799a59u+f->r[6]),1u);
    /* 004544c5 */
    f->r[1]=mi_read((0x00000028u+f->r[4]),4u);
    /* 004544c9 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004544cb */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0045452c;
    /* 004544cd */
    a=f->r[1];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004544d0 */
    if(!(f->flags&MI_ZF))goto mi_004544e1;
    /* 004544d2 */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x0000000eu);
    /* 004544d9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004544da */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004544db */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004544dc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004544dd */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004544e0 */
    f->r[4]+=4;return;
mi_004544e1:;
    /* 004544e1 */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 004544e4 */
    if(!(f->flags&MI_ZF))goto mi_00454518;
    /* 004544e6 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00799a4au+f->r[6]),1u))&0xffu)<<0);
    /* 004544ec */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004544ee */
    if((f->flags&MI_ZF))goto mi_004544ff;
    /* 004544f0 */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x0000000fu);
    /* 004544f7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004544f8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004544f9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004544fa */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004544fb */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004544fe */
    f->r[4]+=4;return;
mi_004544ff:;
    /* 004544ff */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00799a4bu+f->r[6]),1u))&0xffu)<<0);
    /* 00454505 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00454507 */
    if((f->flags&MI_ZF))goto mi_0045452c;
    /* 00454509 */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x00000010u);
    /* 00454510 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00454511 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00454512 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00454513 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00454514 */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00454517 */
    f->r[4]+=4;return;
mi_00454518:;
    /* 00454518 */
    a=f->r[1];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 0045451b */
    if(!(f->flags&MI_ZF))goto mi_0045452c;
    /* 0045451d */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x00000011u);
    /* 00454524 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00454525 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00454526 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00454527 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00454528 */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0045452b */
    f->r[4]+=4;return;
mi_0045452c:;
    /* 0045452c */
    f->r[1]=mi_read((0x0000002cu+f->r[4]),4u);
    /* 00454530 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00454532 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00799a48u+f->r[6]),1u))&0xffu)<<0);
    /* 00454538 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0045453a */
    MI_U32((0x00000044u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0045453e */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0045454f;
    /* 00454540 */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x00000012u);
    /* 00454547 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00454548 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00454549 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0045454a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0045454b */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0045454e */
    f->r[4]+=4;return;
mi_0045454f:;
    /* 0045454f */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00799a3eu+f->r[6]),2u);
    /* 00454556 */
    a=mi_read((0x00000014u+f->r[4]),4u);b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0045455a */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0045456b;
    /* 0045455c */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x00000013u);
    /* 00454563 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00454564 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00454565 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00454566 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00454567 */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0045456a */
    f->r[4]+=4;return;
mi_0045456b:;
    /* 0045456b */
    f->r[0]=(uint32_t)(int32_t)(int8_t)mi_read((0x00799a5bu+f->r[6]),1u);
    /* 00454572 */
    f->r[1]=mi_read((0x00000030u+f->r[4]),4u);
    /* 00454576 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00454578 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004545b6;
    /* 0045457a */
    a=f->r[1];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0045457d */
    if(!(f->flags&MI_ZF))goto mi_0045458e;
    /* 0045457f */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x00000014u);
    /* 00454586 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00454587 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00454588 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00454589 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0045458a */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0045458d */
    f->r[4]+=4;return;
mi_0045458e:;
    /* 0045458e */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 00454591 */
    if(!(f->flags&MI_ZF))goto mi_004545a2;
    /* 00454593 */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x00000015u);
    /* 0045459a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0045459b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0045459c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0045459d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0045459e */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004545a1 */
    f->r[4]+=4;return;
mi_004545a2:;
    /* 004545a2 */
    a=f->r[1];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 004545a5 */
    if(!(f->flags&MI_ZF))goto mi_004545b6;
    /* 004545a7 */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x00000016u);
    /* 004545ae */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004545af */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004545b0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004545b1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004545b2 */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004545b5 */
    f->r[4]+=4;return;
mi_004545b6:;
    /* 004545b6 */
    f->r[1]=mi_read((0x00000034u+f->r[4]),4u);
    /* 004545ba */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004545bc */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00799a4du+f->r[6]),1u))&0xffu)<<0);
    /* 004545c2 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004545c4 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004545c8 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004545d9;
    /* 004545ca */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x00000017u);
    /* 004545d1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004545d2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004545d3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004545d4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004545d5 */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004545d8 */
    f->r[4]+=4;return;
mi_004545d9:;
    /* 004545d9 */
    f->r[0]=(uint32_t)(int32_t)(int8_t)mi_read((0x00799a5au+f->r[6]),1u);
    /* 004545e0 */
    f->r[1]=mi_read((0x00000038u+f->r[4]),4u);
    /* 004545e4 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004545e6 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00454624;
    /* 004545e8 */
    a=f->r[1];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004545eb */
    if(!(f->flags&MI_ZF))goto mi_004545fc;
    /* 004545ed */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x00000018u);
    /* 004545f4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004545f5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004545f6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004545f7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004545f8 */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004545fb */
    f->r[4]+=4;return;
mi_004545fc:;
    /* 004545fc */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00799a46u+f->r[6]),1u))&0xffu)<<0);
    /* 00454602 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00454604 */
    if((f->flags&MI_ZF))goto mi_00454615;
    /* 00454606 */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x00000019u);
    /* 0045460d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0045460e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0045460f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00454610 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00454611 */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00454614 */
    f->r[4]+=4;return;
mi_00454615:;
    /* 00454615 */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x0000001au);
    /* 0045461c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0045461d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0045461e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0045461f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00454620 */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00454623 */
    f->r[4]+=4;return;
mi_00454624:;
    /* 00454624 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00799a3cu+f->r[6]),2u);
    /* 0045462b */
    a=mi_read((0x00000010u+f->r[4]),4u);b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0045462f */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00454640;
    /* 00454631 */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x0000001du);
    /* 00454638 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00454639 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0045463a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0045463b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0045463c */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0045463f */
    f->r[4]+=4;return;
mi_00454640:;
    /* 00454640 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00799a40u+f->r[6]),2u);
    /* 00454647 */
    a=mi_read((0x00000018u+f->r[4]),4u);b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0045464b */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0045465c;
    /* 0045464d */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x0000001bu);
    /* 00454654 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00454655 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00454656 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00454657 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00454658 */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0045465b */
    f->r[4]+=4;return;
mi_0045465c:;
    /* 0045465c */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00799a42u+f->r[6]),2u);
    /* 00454663 */
    f->r[1]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 00454667 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00454669 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0045467a;
    /* 0045466b */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x00000048u);
    /* 00454672 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00454673 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00454674 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00454675 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00454676 */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00454679 */
    f->r[4]+=4;return;
mi_0045467a:;
    /* 0045467a */
    a=f->r[1];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0045467d */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004546a3;
    /* 0045467f */
    f->r[1]=mi_read((0x00eaac28u),4u);
    /* 00454685 */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 0045468b */
    a=mi_read((0x00ea5f14u+f->r[1]),4u);b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 00454692 */
    if(!mi_signed_less(f->flags))goto mi_004546a3;
    /* 00454694 */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x00000046u);
    /* 0045469b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0045469c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0045469d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0045469e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0045469f */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004546a2 */
    f->r[4]+=4;return;
mi_004546a3:;
    /* 004546a3 */
    a=f->r[5];b=0x00000013u;t=mi_alu(f,a,b,32,1);
    /* 004546a6 */
    if(mi_signed_less(f->flags))goto mi_004546b7;
    /* 004546a8 */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x0000003cu);
    /* 004546af */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004546b0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004546b1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004546b2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004546b3 */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004546b6 */
    f->r[4]+=4;return;
mi_004546b7:;
    /* 004546b7 */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 004546b8 */
    f->r[2]=(0x00000000u+f->r[5]+f->r[5]*4u);
    /* 004546bc */
    f->r[1]=(0x00000000u+f->r[5]+f->r[2]*2u);
    /* 004546c0 */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004546c3 */
    f->r[0]=mi_read((0x0070cdd4u+f->r[1]),4u);
    /* 004546c9 */
    f->r[2]=mi_read((0x0070cde0u+f->r[1]),4u);
    /* 004546cf */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004546d3 */
    f->r[0]=mi_read((0x0070cddcu+f->r[1]),4u);
    /* 004546d9 */
    f->r[7]=mi_read((0x0070cdd8u+f->r[1]),4u);
    /* 004546df */
    f->r[3]=mi_read((0x0070cdf4u+f->r[1]),4u);
    /* 004546e5 */
    f->r[5]=mi_read((0x0070cdf8u+f->r[1]),4u);
    /* 004546eb */
    MI_U32((0x00000030u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004546ef */
    f->r[0]=mi_read((0x0070cdecu+f->r[1]),4u);
    /* 004546f5 */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004546f9 */
    f->r[2]=mi_read((0x0070cde8u+f->r[1]),4u);
    /* 004546ff */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00454703 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x007999dbu+f->r[6]),1u))&0xffu)<<0);
    /* 00454709 */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0045470d */
    f->r[2]=mi_read((0x0070cdf0u+f->r[1]),4u);
    /* 00454713 */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 00454715 */
    MI_U32((0x00000038u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00454719 */
    if(!(f->flags&MI_ZF))goto mi_00454737;
    /* 0045471b */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 0045471d */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00454737;
    /* 0045471f */
    f->r[0]=(0x00000002u+f->r[5]);
    /* 00454722 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 00454723 */
    a=f->r[2];b=0x00000003u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 00454726 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00454728 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 0045472b */
    f->r[5]=f->r[0];
    /* 0045472d */
    a=f->r[5];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 00454730 */
    if(!mi_signed_less(f->flags))goto mi_00454737;
    /* 00454732 */
    f->r[5]=0x00000001u;
mi_00454737:;
    /* 00454737 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x007999dbu+f->r[6]),1u))&0xffu)<<0);
    /* 0045473d */
    f->r[0]=mi_read((0x0070cdfcu+f->r[1]),4u);
    /* 00454743 */
    a=(f->r[2]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 00454746 */
    MI_U32((0x0000004cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0045474a */
    if(!(f->flags&MI_ZF))goto mi_0045476d;
    /* 0045474c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0045474e */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0045476d;
    /* 00454750 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00454753 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 00454754 */
    a=f->r[2];b=0x00000003u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 00454757 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00454759 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 0045475c */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0045475f */
    MI_U32((0x0000004cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00454763 */
    if(!mi_signed_less(f->flags))goto mi_0045476d;
    /* 00454765 */
    MI_U32((0x0000004cu+f->r[4]))=(uint32_t)(0x00000001u);
mi_0045476d:;
    /* 0045476d */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x007999dbu+f->r[6]),1u))&0xffu)<<0);
    /* 00454773 */
    f->r[0]=mi_read((0x0070ce00u+f->r[1]),4u);
    /* 00454779 */
    a=(f->r[2]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 0045477c */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00454780 */
    if(!(f->flags&MI_ZF))goto mi_004547a5;
    /* 00454782 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00454784 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004547a5;
    /* 00454786 */
    f->r[2]=f->r[0];
    /* 00454788 */
    f->r[0]=(0x00000002u+f->r[2]);
    /* 0045478b */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0045478c */
    a=f->r[2];b=0x00000003u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0045478f */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00454791 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 00454794 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 00454797 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0045479b */
    if(!mi_signed_less(f->flags))goto mi_004547a5;
    /* 0045479d */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00000001u);
mi_004547a5:;
    /* 004547a5 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x007999dbu+f->r[6]),1u))&0xffu)<<0);
    /* 004547ab */
    f->r[1]=mi_read((0x0070ce04u+f->r[1]),4u);
    /* 004547b1 */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004547b3 */
    if(!(f->flags&MI_ZF))goto mi_004547d1;
    /* 004547b5 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004547b7 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004547d1;
    /* 004547b9 */
    f->r[0]=(0x00000002u+f->r[1]);
    /* 004547bc */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004547bd */
    a=f->r[2];b=0x00000003u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004547c0 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004547c2 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004547c5 */
    f->r[1]=f->r[0];
    /* 004547c7 */
    a=f->r[1];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004547ca */
    if(!mi_signed_less(f->flags))goto mi_004547d1;
    /* 004547cc */
    f->r[1]=0x00000001u;
mi_004547d1:;
    /* 004547d1 */
    f->r[0]=mi_read((0x00000020u+f->r[4]),4u);
    /* 004547d5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004547d7 */
    if(!(f->flags&MI_ZF))goto mi_00454800;
    /* 004547d9 */
    f->r[0]=mi_read((0x00eadcd4u),4u);
    /* 004547de */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004547e0 */
    if((f->flags&MI_ZF))goto mi_004547f1;
    /* 004547e2 */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x0000003eu);
    /* 004547e9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004547ea */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004547eb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004547ec */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004547ed */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004547f0 */
    f->r[4]+=4;return;
mi_004547f1:;
    /* 004547f1 */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x0000001eu);
    /* 004547f8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004547f9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004547fa */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004547fb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004547fc */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004547ff */
    f->r[4]+=4;return;
mi_00454800:;
    /* 00454800 */
    a=f->r[7];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 00454803 */
    if(!(f->flags&MI_ZF))goto mi_00454828;
    /* 00454805 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00799a0du+f->r[6]),1u))&0xffu)<<0);
    /* 0045480b */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0045480d */
    if(!(f->flags&MI_ZF))goto mi_00454846;
    /* 0045480f */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00799a17u+f->r[6]),1u))&0xffu)<<0);
    /* 00454815 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00454817 */
    if(!(f->flags&MI_ZF))goto mi_00454846;
    /* 00454819 */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x0000001fu);
    /* 00454820 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00454821 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00454822 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00454823 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00454824 */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00454827 */
    f->r[4]+=4;return;
mi_00454828:;
    /* 00454828 */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0045482b */
    if(!(f->flags&MI_ZF))goto mi_00454846;
    /* 0045482d */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00799a0du+f->r[6]),1u))&0xffu)<<0);
    /* 00454833 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00454835 */
    if(!(f->flags&MI_ZF))goto mi_00454846;
    /* 00454837 */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x00000020u);
    /* 0045483e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0045483f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00454840 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00454841 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00454842 */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00454845 */
    f->r[4]+=4;return;
mi_00454846:;
    /* 00454846 */
    f->r[0]=mi_read((0x00000024u+f->r[4]),4u);
    /* 0045484a */
    f->r[2]=mi_read((0x00000040u+f->r[4]),4u);
    /* 0045484e */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 00454850 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004548ca;
    /* 00454852 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00799a58u+f->r[6]),1u))&0xffu)<<0);
    /* 00454858 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0045485a */
    if(!(f->flags&MI_ZF))goto mi_0045486b;
    /* 0045485c */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x00000021u);
    /* 00454863 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00454864 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00454865 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00454866 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00454867 */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0045486a */
    f->r[4]+=4;return;
mi_0045486b:;
    /* 0045486b */
    a=f->r[0];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 0045486e */
    if(!mi_signed_less(f->flags))goto mi_0045487f;
    /* 00454870 */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x00000022u);
    /* 00454877 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00454878 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00454879 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0045487a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0045487b */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0045487e */
    f->r[4]+=4;return;
mi_0045487f:;
    /* 0045487f */
    a=f->r[0];b=0x00000019u;t=mi_alu(f,a,b,32,1);
    /* 00454882 */
    if(!mi_signed_less(f->flags))goto mi_00454893;
    /* 00454884 */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x00000023u);
    /* 0045488b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0045488c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0045488d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0045488e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0045488f */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00454892 */
    f->r[4]+=4;return;
mi_00454893:;
    /* 00454893 */
    a=f->r[0];b=0x00000032u;t=mi_alu(f,a,b,32,1);
    /* 00454896 */
    if(!mi_signed_less(f->flags))goto mi_004548a7;
    /* 00454898 */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x00000024u);
    /* 0045489f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004548a0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004548a1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004548a2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004548a3 */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004548a6 */
    f->r[4]+=4;return;
mi_004548a7:;
    /* 004548a7 */
    a=f->r[0];b=0x00000050u;t=mi_alu(f,a,b,32,1);
    /* 004548aa */
    if(!mi_signed_less(f->flags))goto mi_004548bb;
    /* 004548ac */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x00000025u);
    /* 004548b3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004548b4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004548b5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004548b6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004548b7 */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004548ba */
    f->r[4]+=4;return;
mi_004548bb:;
    /* 004548bb */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x00000026u);
    /* 004548c2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004548c3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004548c4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004548c5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004548c6 */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004548c9 */
    f->r[4]+=4;return;
mi_004548ca:;
    /* 004548ca */
    a=f->r[3];b=mi_read((0x0000003cu+f->r[4]),4u);t=mi_alu(f,a,b,32,1);
    /* 004548ce */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0045490c;
    /* 004548d0 */
    a=f->r[3];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004548d3 */
    if(!(f->flags&MI_ZF))goto mi_004548e4;
    /* 004548d5 */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x00000027u);
    /* 004548dc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004548dd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004548de */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004548df */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004548e0 */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004548e3 */
    f->r[4]+=4;return;
mi_004548e4:;
    /* 004548e4 */
    a=f->r[3];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 004548e7 */
    if(!(f->flags&MI_ZF))goto mi_004548f8;
    /* 004548e9 */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x00000028u);
    /* 004548f0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004548f1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004548f2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004548f3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004548f4 */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004548f7 */
    f->r[4]+=4;return;
mi_004548f8:;
    /* 004548f8 */
    a=f->r[3];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 004548fb */
    if(!(f->flags&MI_ZF))goto mi_0045490c;
    /* 004548fd */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x00000029u);
    /* 00454904 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00454905 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00454906 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00454907 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00454908 */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0045490b */
    f->r[4]+=4;return;
mi_0045490c:;
    /* 0045490c */
    f->r[0]=(uint32_t)(int32_t)(int8_t)mi_read((0x00799a59u+f->r[6]),1u);
    /* 00454913 */
    f->r[2]=mi_read((0x00000028u+f->r[4]),4u);
    /* 00454917 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00454919 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00454966;
    /* 0045491b */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0045491e */
    if(!(f->flags&MI_ZF))goto mi_0045492f;
    /* 00454920 */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x0000002cu);
    /* 00454927 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00454928 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00454929 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0045492a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0045492b */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0045492e */
    f->r[4]+=4;return;
mi_0045492f:;
    /* 0045492f */
    a=f->r[2];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 00454932 */
    if(!(f->flags&MI_ZF))goto mi_00454966;
    /* 00454934 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00799a4au+f->r[6]),1u))&0xffu)<<0);
    /* 0045493a */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0045493c */
    if((f->flags&MI_ZF))goto mi_0045494d;
    /* 0045493e */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x0000002du);
    /* 00454945 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00454946 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00454947 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00454948 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00454949 */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0045494c */
    f->r[4]+=4;return;
mi_0045494d:;
    /* 0045494d */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00799a4bu+f->r[6]),1u))&0xffu)<<0);
    /* 00454953 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00454955 */
    if((f->flags&MI_ZF))goto mi_00454966;
    /* 00454957 */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x0000002eu);
    /* 0045495e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0045495f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00454960 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00454961 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00454962 */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00454965 */
    f->r[4]+=4;return;
mi_00454966:;
    /* 00454966 */
    f->r[0]=mi_read((0x0000002cu+f->r[4]),4u);
    /* 0045496a */
    f->r[2]=mi_read((0x00000044u+f->r[4]),4u);
    /* 0045496e */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 00454970 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00454981;
    /* 00454972 */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x00000030u);
    /* 00454979 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0045497a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0045497b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0045497c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0045497d */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00454980 */
    f->r[4]+=4;return;
mi_00454981:;
    /* 00454981 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00799a3eu+f->r[6]),2u);
    /* 00454988 */
    a=f->r[5];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0045498a */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0045499b;
    /* 0045498c */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x00000031u);
    /* 00454993 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00454994 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00454995 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00454996 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00454997 */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0045499a */
    f->r[4]+=4;return;
mi_0045499b:;
    /* 0045499b */
    f->r[0]=(uint32_t)(int32_t)(int8_t)mi_read((0x00799a5bu+f->r[6]),1u);
    /* 004549a2 */
    f->r[2]=mi_read((0x00000030u+f->r[4]),4u);
    /* 004549a6 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004549a8 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004549e6;
    /* 004549aa */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004549ad */
    if(!(f->flags&MI_ZF))goto mi_004549be;
    /* 004549af */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x00000032u);
    /* 004549b6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004549b7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004549b8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004549b9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004549ba */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004549bd */
    f->r[4]+=4;return;
mi_004549be:;
    /* 004549be */
    a=f->r[2];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 004549c1 */
    if(!(f->flags&MI_ZF))goto mi_004549d2;
    /* 004549c3 */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x00000033u);
    /* 004549ca */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004549cb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004549cc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004549cd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004549ce */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004549d1 */
    f->r[4]+=4;return;
mi_004549d2:;
    /* 004549d2 */
    a=f->r[2];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 004549d5 */
    if(!(f->flags&MI_ZF))goto mi_004549e6;
    /* 004549d7 */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x00000034u);
    /* 004549de */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004549df */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004549e0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004549e1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004549e2 */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004549e5 */
    f->r[4]+=4;return;
mi_004549e6:;
    /* 004549e6 */
    f->r[2]=mi_read((0x00000034u+f->r[4]),4u);
    /* 004549ea */
    f->r[0]=mi_read((0x00000014u+f->r[4]),4u);
    /* 004549ee */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004549f0 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00454a01;
    /* 004549f2 */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x00000035u);
    /* 004549f9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004549fa */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004549fb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004549fc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004549fd */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00454a00 */
    f->r[4]+=4;return;
mi_00454a01:;
    /* 00454a01 */
    f->r[0]=(uint32_t)(int32_t)(int8_t)mi_read((0x00799a5au+f->r[6]),1u);
    /* 00454a08 */
    f->r[2]=mi_read((0x00000038u+f->r[4]),4u);
    /* 00454a0c */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00454a0e */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00454a4c;
    /* 00454a10 */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 00454a13 */
    if(!(f->flags&MI_ZF))goto mi_00454a24;
    /* 00454a15 */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x00000036u);
    /* 00454a1c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00454a1d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00454a1e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00454a1f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00454a20 */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00454a23 */
    f->r[4]+=4;return;
mi_00454a24:;
    /* 00454a24 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00799a46u+f->r[6]),1u))&0xffu)<<0);
    /* 00454a2a */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00454a2c */
    if((f->flags&MI_ZF))goto mi_00454a3d;
    /* 00454a2e */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x00000037u);
    /* 00454a35 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00454a36 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00454a37 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00454a38 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00454a39 */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00454a3c */
    f->r[4]+=4;return;
mi_00454a3d:;
    /* 00454a3d */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x00000038u);
    /* 00454a44 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00454a45 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00454a46 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00454a47 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00454a48 */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00454a4b */
    f->r[4]+=4;return;
mi_00454a4c:;
    /* 00454a4c */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00799a3cu+f->r[6]),2u);
    /* 00454a53 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00454a55 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00454a66;
    /* 00454a57 */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x0000003bu);
    /* 00454a5e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00454a5f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00454a60 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00454a61 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00454a62 */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00454a65 */
    f->r[4]+=4;return;
mi_00454a66:;
    /* 00454a66 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00799a40u+f->r[6]),2u);
    /* 00454a6d */
    a=mi_read((0x0000004cu+f->r[4]),4u);b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00454a71 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00454a82;
    /* 00454a73 */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x00000039u);
    /* 00454a7a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00454a7b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00454a7c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00454a7d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00454a7e */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00454a81 */
    f->r[4]+=4;return;
mi_00454a82:;
    /* 00454a82 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00799a42u+f->r[6]),2u);
    /* 00454a89 */
    f->r[1]=mi_read((0x00000010u+f->r[4]),4u);
    /* 00454a8d */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00454a8f */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00454aa0;
    /* 00454a91 */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x00000044u);
    /* 00454a98 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00454a99 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00454a9a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00454a9b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00454a9c */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00454a9f */
    f->r[4]+=4;return;
mi_00454aa0:;
    /* 00454aa0 */
    a=f->r[1];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 00454aa3 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00454ac8;
    /* 00454aa5 */
    f->r[0]=mi_read((0x00eaac28u),4u);
    /* 00454aaa */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 00454ab0 */
    a=mi_read((0x00ea5f14u+f->r[0]),4u);b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 00454ab7 */
    if(!mi_signed_less(f->flags))goto mi_00454ac8;
    /* 00454ab9 */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x0000004au);
    /* 00454ac0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00454ac1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00454ac2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00454ac3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00454ac4 */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00454ac7 */
    f->r[4]+=4;return;
mi_00454ac8:;
    /* 00454ac8 */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x0000003du);
    /* 00454acf */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00799a56u+f->r[6]),1u))&0xffu)<<0);
    /* 00454ad5 */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 00454ad7 */
    if(!(f->flags&MI_ZF))goto mi_00454ae0;
    /* 00454ad9 */
    MI_U8((0x00799a5du+f->r[6]))=(uint8_t)(0x00000040u);
mi_00454ae0:;
    /* 00454ae0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00454ae1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00454ae2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00454ae3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00454ae4 */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00454ae7 */
    f->r[4]+=4;return;
}

void restore_prepare_C_0045d570(struct restore_prepare_integer_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0045d570 */
    f->r[1]=MI_U32((0x00000004u+f->r[4]));
    /* 0045d574 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045d575 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045d576 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0045d578 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 0045d57a */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
mi_0045d57c:;
    /* 0045d57c */
    f->r[0]=f->r[1];
    /* 0045d57e */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0045d581 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0045d583 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00799a12u+f->r[0]*8u));
    /* 0045d58b */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0045d58d */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0045d5b9;
    /* 0045d58f */
    f->r[0]=f->r[1];
    /* 0045d591 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0045d594 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0045d596 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0045d599 */
    a=MI_U16((0x007999eau+f->r[0]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 0045d5a1 */
    if(!(f->flags&MI_ZF))goto mi_0045d5a6;
    /* 0045d5a3 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 0045d5a4 */
    goto mi_0045d5af;
mi_0045d5a6:;
    /* 0045d5a6 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00799a14u+f->r[0]));
    /* 0045d5ad */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
mi_0045d5af:;
    /* 0045d5af */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0045d5b0 */
    a=f->r[2];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 0045d5b3 */
    if(mi_signed_less(f->flags))goto mi_0045d57c;
    /* 0045d5b5 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 0045d5b7 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0045d5be;
mi_0045d5b9:;
    /* 0045d5b9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0045d5ba */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0045d5bc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0045d5bd */
    f->r[4]+=4;return;
mi_0045d5be:;
    /* 0045d5be */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0045d5c0 */
    a=f->r[6];b=0x00000c80u;t=mi_alu(f,a,b,32,1);
    /* 0045d5c6 */
    t=(mi_signed_less(f->flags))?1u:0u;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0045d5c9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0045d5ca */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0045d5cb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0045d5cc */
    f->r[4]+=4;return;
}

void restore_prepare_C_00475b50(struct restore_prepare_integer_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00475b50 */
    f->r[0]=MI_U32((0x00000004u+f->r[4]));
    /* 00475b54 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
mi_00475b56:;
    /* 00475b56 */
    f->r[1]=f->r[0];
    /* 00475b58 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00475b5b */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00475b5d */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x00799a10u+f->r[1]*8u)))&0xffffu)<<0);
    /* 00475b65 */
    a=(f->r[1]&0xffffu);b=(f->r[1]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 00475b68 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00475b78;
    /* 00475b6a */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00475b6b */
    a=f->r[2];b=0x0000020du;t=mi_alu(f,a,b,32,1);
    /* 00475b71 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)(f->r[1]&0xffffu);
    /* 00475b74 */
    if(mi_signed_less(f->flags))goto mi_00475b56;
    /* 00475b76 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_00475b78:;
    /* 00475b78 */
    f->r[4]+=4;return;
}

void restore_prepare_C_004b7590(struct restore_prepare_integer_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004b7590 */
    f->r[1]=0x00bcba9au;
    /* 004b7595 */
    f->r[0]=0x00000001u;
mi_004b759a:;
    /* 004b759a */
    a=MI_U8((0xffffffbeu+f->r[1]));b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 004b759d */
    if(!(f->flags&MI_ZF))goto mi_004b75ab;
    /* 004b759f */
    a=MI_U8((0x00000000u+f->r[1]));b=0x00000000u;t=mi_alu(f,a,b,8,1);
    /* 004b75a2 */
    if((f->flags&MI_ZF))goto mi_004b75ab;
    /* 004b75a4 */
    a=MI_U16((0x00000004u+f->r[1]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 004b75a9 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004b75bb;
mi_004b75ab:;
    /* 004b75ab */
    a=f->r[1];b=0x00000090u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004b75b1 */
    a=f->r[1];b=0x00bcd62au;t=mi_alu(f,a,b,32,1);
    /* 004b75b7 */
    if(mi_signed_less(f->flags))goto mi_004b759a;
    /* 004b75b9 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_004b75bb:;
    /* 004b75bb */
    f->r[4]+=4;return;
}

void restore_prepare_C_0050f0f0(struct restore_prepare_integer_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0050f0f0 */
    a=f->r[4];b=0x00000024u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 0050f0f3 */
    f->r[2]=MI_U32((0x00eadd6cu));
    /* 0050f0f9 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0050f0fa */
    f->r[0]=f->r[2];
    /* 0050f0fc */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0050f0fd */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0050f100 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0050f102 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0050f103 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0050f106 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0050f107 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0050f109 */
    f->r[6]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[0]));
    /* 0050f110 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[0]));
    /* 0050f117 */
    f->r[3]=(0x00000006u+f->r[6]);
    /* 0050f11a */
    a=f->r[6];b=0x00000006u;t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 0050f11d */
    f->r[7]=(0x00000006u+f->r[0]);
    /* 0050f120 */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0050f123 */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0050f125 */
    MI_U32((0x00eadcd8u))=(uint32_t)(f->r[1]);
    /* 0050f12b */
    MI_U32((0x00eadcd4u))=(uint32_t)(f->r[1]);
    /* 0050f131 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 0050f135 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 0050f139 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 0050f13d */
    if(!mi_signed_less(f->flags))goto mi_0050f145;
    /* 0050f13f */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0050f143 */
    f->r[6]=f->r[1];
mi_0050f145:;
    /* 0050f145 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0050f147 */
    if(!mi_signed_less(f->flags))goto mi_0050f14b;
    /* 0050f149 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_0050f14b:;
    /* 0050f14b */
    f->r[1]=MI_U32((0x00bca2d0u));
    /* 0050f151 */
    a=f->r[3];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0050f153 */
    if(mi_signed_less(f->flags))goto mi_0050f15c;
    /* 0050f155 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0050f156 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0050f15a */
    f->r[3]=f->r[1];
mi_0050f15c:;
    /* 0050f15c */
    f->r[1]=MI_U32((0x00bca2d4u));
    /* 0050f162 */
    a=f->r[7];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0050f164 */
    if(mi_signed_less(f->flags))goto mi_0050f16b;
    /* 0050f166 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0050f167 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[1]);
mi_0050f16b:;
    /* 0050f16b */
    f->r[7]=MI_U32((0x00bca2d8u));
    /* 0050f171 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 0050f174 */
    f->r[5]=MI_U32((0x00000020u+f->r[4]));
    /* 0050f178 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0050f17c */
    f->r[1]=(0x00000000u+f->r[0]+f->r[1]*2u);
    /* 0050f17f */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 0050f182 */
    f->r[1]=(0x00000000u+f->r[7]+f->r[1]*4u);
    /* 0050f185 */
    f->r[7]=f->r[6];
    /* 0050f187 */
    a=f->r[7];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 0050f189 */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0050f18b */
    a=f->r[7];b=0x000000e3u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0050f191 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 0050f193 */
    MI_U32((0x00000030u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 0050f197 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0050f312;
mi_0050f19d:;
    /* 0050f19d */
    a=f->r[6];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 0050f19f */
    f->r[5]=f->r[6];
    /* 0050f1a1 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0050f2fb;
    /* 0050f1a7 */
    f->r[0]=(0x00b4e6fcu+f->r[1]*2u);
    /* 0050f1ae */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0050f1b2 */
    f->r[0]=f->r[3];
    /* 0050f1b4 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0050f1b6 */
    f->r[1]=(0x00000001u+f->r[1]+f->r[0]*1u);
    /* 0050f1ba */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[1]);
mi_0050f1be:;
    /* 0050f1be */
    f->r[0]=MI_U32((0x00000018u+f->r[4]));
    /* 0050f1c2 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[0]));
    /* 0050f1c5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0050f1c7 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0050f1cb */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0050f2df;
    /* 0050f1d1 */
    f->r[1]=f->r[0];
    /* 0050f1d3 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0050f1d6 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0050f1d8 */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0050f1db */
    a=MI_U8((0x007999d8u+f->r[1]));b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 0050f1e2 */
    if(!(f->flags&MI_ZF))goto mi_0050f2df;
    /* 0050f1e8 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0050f1ea */
    if((f->flags&MI_ZF))goto mi_0050f2df;
    /* 0050f1f0 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x007999ddu+f->r[1])))&0xffu)<<0);
    /* 0050f1f6 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0050f1f8 */
    if((f->flags&MI_ZF))goto mi_0050f216;
    /* 0050f1fa */
    f->r[7]=(f->r[7]&0xffff0000u)|(((MI_U16((0x007999e8u+f->r[1])))&0xffffu)<<0);
    /* 0050f201 */
    f->r[0]=f->r[2];
    /* 0050f203 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0050f206 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0050f208 */
    a=(f->r[7]&0xffffu);b=MI_U16((0x007999e8u+f->r[0]*8u));t=mi_alu(f,a,b,16,1);
    /* 0050f210 */
    if(!mi_signed_less(f->flags))goto mi_0050f2df;
mi_0050f216:;
    /* 0050f216 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x007999e8u+f->r[1])))&0xffffu)<<0);
    /* 0050f21d */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0050f221 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)(f->r[1]&0xffffu);
    /* 0050f224 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*4u);
    /* 0050f227 */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0050f22a */
    f->r[7]=MI_U32((0x00705ea8u+f->r[1]));
    /* 0050f230 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 0050f232 */
    if(!mi_signed_less(f->flags))goto mi_0050f2df;
    /* 0050f238 */
    f->r[3]=MI_U32((0x00705eb0u+f->r[1]));
    /* 0050f23e */
    f->r[0]=MI_U32((0x00705eb4u+f->r[1]));
    /* 0050f244 */
    f->r[1]=f->r[2];
    /* 0050f246 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0050f249 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0050f24b */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0050f24e */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[1]));
    /* 0050f255 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[1]));
    /* 0050f25c */
    a=f->r[5];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0050f25e */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0050f266;
    /* 0050f260 */
    f->r[6]=f->r[5];
    /* 0050f262 */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 0050f264 */
    goto mi_0050f270;
mi_0050f266:;
    /* 0050f266 */
    if(!mi_signed_less(f->flags))goto mi_0050f26e;
    /* 0050f268 */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0050f26a */
    f->r[6]=f->r[1];
    /* 0050f26c */
    goto mi_0050f270;
mi_0050f26e:;
    /* 0050f26e */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
mi_0050f270:;
    /* 0050f270 */
    f->r[1]=MI_U32((0x0000001cu+f->r[4]));
    /* 0050f274 */
    MI_U32((0x00d3b4fcu))=(uint32_t)(f->r[6]);
    /* 0050f27a */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0050f27c */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0050f282;
    /* 0050f27e */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0050f280 */
    goto mi_0050f28c;
mi_0050f282:;
    /* 0050f282 */
    if(!mi_signed_less(f->flags))goto mi_0050f28a;
    /* 0050f284 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0050f286 */
    f->r[1]=f->r[2];
    /* 0050f288 */
    goto mi_0050f28c;
mi_0050f28a:;
    /* 0050f28a */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_0050f28c:;
    /* 0050f28c */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0050f28e */
    MI_U32((0x00d3b4f8u))=(uint32_t)(f->r[1]);
    /* 0050f294 */
    if(mi_signed_less(f->flags))goto mi_0050f298;
    /* 0050f296 */
    f->r[1]=f->r[6];
mi_0050f298:;
    /* 0050f298 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0050f29a */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0050f2d1;
    /* 0050f29c */
    f->r[2]=f->r[1];
    /* 0050f29e */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0050f29f */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0050f2a2 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0050f2a9;
    /* 0050f2a4 */
    a=f->r[1];b=f->r[3];t=mi_imul(f,a,b);f->r[1]=t;
    /* 0050f2a7 */
    a=f->r[7];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[7]=t;
mi_0050f2a9:;
    /* 0050f2a9 */
    a=f->r[7];b=MI_U32((0x00eadcd8u));t=mi_alu(f,a,b,32,1);
    /* 0050f2af */
    if(!mi_signed_less(f->flags))goto mi_0050f2d1;
    /* 0050f2b1 */
    a=MI_U16((0x00000028u+f->r[4]));b=0x00000036u;t=mi_alu(f,a,b,16,1);
    /* 0050f2b7 */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 0050f2bb */
    MI_U32((0x00eadcd8u))=(uint32_t)(f->r[7]);
    /* 0050f2c1 */
    if(!(f->flags&MI_ZF))goto mi_0050f2cc;
    /* 0050f2c3 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0050f2c4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0050f2c9u;f->boundary(0x0050f2c4u,0x00475b50u,f);
    /* 0050f2c9 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0050f2cc:;
    /* 0050f2cc */
    MI_U32((0x00eadcd4u))=(uint32_t)(f->r[0]);
mi_0050f2d1:;
    /* 0050f2d1 */
    f->r[3]=MI_U32((0x00000010u+f->r[4]));
    /* 0050f2d5 */
    f->r[6]=MI_U32((0x00000014u+f->r[4]));
    /* 0050f2d9 */
    f->r[2]=MI_U32((0x00eadd6cu));
mi_0050f2df:;
    /* 0050f2df */
    f->r[1]=MI_U32((0x00000018u+f->r[4]));
    /* 0050f2e3 */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 0050f2e4 */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0050f2e7 */
    a=f->r[5];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 0050f2e9 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0050f2ed */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0050f1be;
    /* 0050f2f3 */
    f->r[1]=MI_U32((0x0000002cu+f->r[4]));
    /* 0050f2f7 */
    f->r[7]=MI_U32((0x00000030u+f->r[4]));
mi_0050f2fb:;
    /* 0050f2fb */
    f->r[0]=MI_U32((0x0000001cu+f->r[4]));
    /* 0050f2ff */
    f->r[5]=MI_U32((0x00000020u+f->r[4]));
    /* 0050f303 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0050f304 */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0050f306 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 0050f308 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0050f30c */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0050f19d;
mi_0050f312:;
    /* 0050f312 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0050f313 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0050f314 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0050f315 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0050f316 */
    a=f->r[4];b=0x00000024u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0050f319 */
    f->r[4]+=4;return;
}

__declspec(noinline) __declspec(noreturn) void restore_prepare_integer_failure(uint32_t target)
{fprintf(stderr,"Unsupported menu integer boundary %08lx\n",(unsigned long)target);abort();}
void restore_prepare_integer_dispatch(uint32_t site,uint32_t target,struct restore_prepare_integer_frame *frame)
{
 (void)site;
 switch(target){case 0x475b50u:restore_prepare_C_00475b50(frame);return;default:restore_prepare_integer_failure(target);}
}
static uint32_t prepare_run(uint32_t target,uint32_t argument,uint32_t has_argument)
{
 uint32_t stack[128],i,entry;struct restore_prepare_integer_frame frame={{0},2,restore_prepare_integer_dispatch};
 typedef char pointer_is_32[sizeof(void*)==4?1:-1];(void)sizeof(pointer_is_32);
 for(i=0;i<128;++i)stack[i]=0xa5a55a5au;
 entry=(uint32_t)(uintptr_t)&stack[112];frame.r[4]=entry;stack[112]=0;if(has_argument)stack[113]=argument;
 switch(target){
 case 0x454190u:restore_prepare_C_00454190(&frame);break;
 case 0x45d570u:restore_prepare_C_0045d570(&frame);break;
 case 0x475b50u:restore_prepare_C_00475b50(&frame);break;
 case 0x4b7590u:restore_prepare_C_004b7590(&frame);break;
 case 0x50f0f0u:restore_prepare_C_0050f0f0(&frame);break;
 default:restore_prepare_integer_failure(target);
 }
 if(frame.r[4]!=entry+4u)restore_prepare_integer_failure(frame.r[4]);
 for(i=0;i<16;++i)if(stack[i]!=0xa5a55a5au)restore_prepare_integer_failure(target);
 for(i=113+has_argument;i<128;++i)if(stack[i]!=0xa5a55a5au)restore_prepare_integer_failure(target);
 return frame.r[0];
}
uint32_t FUN_00454190(uint32_t building){return prepare_run(0x454190u,building,1);}
uint32_t FUN_0045d570(uint32_t building){return prepare_run(0x45d570u,building,1);}
uint32_t FUN_00475b50(uint32_t building){return prepare_run(0x475b50u,building,1);}
uint32_t FUN_004b7590(void){return prepare_run(0x4b7590u,0,0);}
uint32_t FUN_0050f0f0(void){return prepare_run(0x50f0f0u,0,0);}
