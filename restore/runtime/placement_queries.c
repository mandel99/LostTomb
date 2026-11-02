#include "placement_queries.h"
#include <stdio.h>
#include <stdlib.h>
#include "monument_progress.h"
#include "sprite_libraries.h"
extern uint32_t FUN_004ea930(void);
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
/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */

#include <stdlib.h>



static const uint8_t mi_rom_00401a48[]={0x73,0x19,0x40,0x00,0x92,0x19,0x40,0x00,0xc0,0x19,0x40,0x00,0xdb,0x19,0x40,0x00,0xe7,0x19,0x40,0x00,0xf3,0x19,0x40,0x00,0x2c,0x1a,0x40,0x00,0xce,0x18,0x40,0x00,0x07,0x19,0x40,0x00,0x3e,0x19,0x40,0x00,0x75,0x18,0x40,0x00,0x96,0x18,0x40,0x00};
static const uint8_t mi_rom_00401a78[]={0x83,0x19,0x40,0x00,0xa2,0x19,0x40,0x00,0xb1,0x19,0x40,0x00,0xcc,0x19,0x40,0x00};
static const uint8_t mi_rom_00401a88[]={0xa2,0x19,0x40,0x00,0xb1,0x19,0x40,0x00,0xcc,0x19,0x40,0x00,0x83,0x19,0x40,0x00};
static const uint8_t mi_rom_00401a98[]={0xcc,0x19,0x40,0x00,0x83,0x19,0x40,0x00,0xa2,0x19,0x40,0x00,0xb1,0x19,0x40,0x00};
static const uint8_t mi_rom_00401aa8[]={0x1d,0x1a,0x40,0x00,0x38,0x1a,0x40,0x00,0xff,0x19,0x40,0x00,0x0e,0x1a,0x40,0x00};
static const uint8_t mi_rom_00401ab8[]={0x0e,0x1a,0x40,0x00,0x1d,0x1a,0x40,0x00,0x38,0x1a,0x40,0x00,0xff,0x19,0x40,0x00};
static const uint8_t mi_rom_00401ac8[]={0x38,0x1a,0x40,0x00,0xff,0x19,0x40,0x00,0x0e,0x1a,0x40,0x00,0x1d,0x1a,0x40,0x00};
static const uint8_t mi_rom_00401ad8[]={0xff,0x19,0x40,0x00,0x0e,0x1a,0x40,0x00,0x1d,0x1a,0x40,0x00,0x38,0x1a,0x40,0x00};
static __inline uint32_t pq_read_00401800(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x00401a48u&&width<=48u&&(offset=address-0x00401a48u)<=48u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00401a48[offset+k]<<(k*8u);return result;
 }
 if(address>=0x00401a78u&&width<=16u&&(offset=address-0x00401a78u)<=16u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00401a78[offset+k]<<(k*8u);return result;
 }
 if(address>=0x00401a88u&&width<=16u&&(offset=address-0x00401a88u)<=16u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00401a88[offset+k]<<(k*8u);return result;
 }
 if(address>=0x00401a98u&&width<=16u&&(offset=address-0x00401a98u)<=16u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00401a98[offset+k]<<(k*8u);return result;
 }
 if(address>=0x00401aa8u&&width<=16u&&(offset=address-0x00401aa8u)<=16u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00401aa8[offset+k]<<(k*8u);return result;
 }
 if(address>=0x00401ab8u&&width<=16u&&(offset=address-0x00401ab8u)<=16u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00401ab8[offset+k]<<(k*8u);return result;
 }
 if(address>=0x00401ac8u&&width<=16u&&(offset=address-0x00401ac8u)<=16u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00401ac8[offset+k]<<(k*8u);return result;
 }
 if(address>=0x00401ad8u&&width<=16u&&(offset=address-0x00401ad8u)<=16u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00401ad8[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}

void restore_placement_query_C_00401800(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00401800 */
    f->r[0]=pq_read_00401800((0x00000004u+f->r[4]),4u);
    /* 00401804 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00401805 */
    f->r[3]=f->r[0];
    /* 00401807 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00401808 */
    a=f->r[3];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[3]=t;
    /* 0040180b */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 0040180d */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040180e */
    a=f->r[3];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[3]=t;
    /* 00401811 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00401812 */
    f->r[5]=0x00000000u;
    /* 00401817 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)pq_read_00401800((0x007999eau+f->r[3]),2u);
    /* 0040181e */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 00401821 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*4u);
    /* 00401824 */
    f->r[1]=pq_read_00401800((0x00000058u+f->r[1]+f->r[0]*4u),4u);
    /* 00401828 */
    f->r[0]=pq_read_00401800((0x00bca2e0u),4u);
    /* 0040182d */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0040182e */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00401830 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00401834 */
    f->r[6]=f->r[0];
    /* 00401836 */
    a=f->r[6];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[6]=t;
    /* 00401838 */
    if((f->flags&MI_ZF))goto mi_0040183f;
    /* 0040183a */
    a=f->r[6];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 0040183d */
    if(!(f->flags&MI_ZF))goto mi_00401844;
mi_0040183f:;
    /* 0040183f */
    f->r[5]=0x00000001u;
mi_00401844:;
    /* 00401844 */
    f->r[2]=pq_read_00401800((0x00000018u+f->r[4]),4u);
    /* 00401848 */
    a=0x0000001au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040184a */
    a=0x005760b0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040184f */
    f->r[1]=0x00f55bb0u;
    /* 00401854 */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(0x00000000u);
    /* 0040185a */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 0040185c */
    f->r[4]-=4;MI_U32(f->r[4])=0x00401861u;f->boundary(0x0040185cu,0x0053dd90u,f);
    /* 00401861 */
    f->r[0]=pq_read_00401800((0x00000014u+f->r[4]),4u);
    /* 00401865 */
    a=f->r[0];b=0x0000000bu;t=mi_alu(f,a,b,32,1);
    /* 00401868 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00401a3e;
    /* 0040186e */
    switch(pq_read_00401800((0x00401a48u+f->r[0]*4u),4u)){case 0x00401875u:goto mi_00401875;case 0x00401896u:goto mi_00401896;case 0x004018ceu:goto mi_004018ce;case 0x00401907u:goto mi_00401907;case 0x0040193eu:goto mi_0040193e;case 0x00401973u:goto mi_00401973;case 0x00401992u:goto mi_00401992;case 0x004019c0u:goto mi_004019c0;case 0x004019dbu:goto mi_004019db;case 0x004019e7u:goto mi_004019e7;case 0x004019f3u:goto mi_004019f3;case 0x00401a2cu:goto mi_00401a2c;default:abort();}
mi_00401875:;
    /* 00401875 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 00401877 */
    if((f->flags&MI_ZF))goto mi_004018b6;
    /* 00401879 */
    f->r[1]=pq_read_00401800((0x00f5751cu),4u);
    /* 0040187f */
    f->r[0]=pq_read_00401800((0x00799a30u+f->r[3]),4u);
    /* 00401885 */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 00401888 */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0040188a */
    f->r[7]=(0x00000000u+f->r[1]+f->r[0]*2u);
    /* 0040188d */
    f->r[0]=f->r[7];
    /* 0040188f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00401890 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00401891 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00401892 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00401893 */
    f->r[4]+=12;return;
mi_00401896:;
    /* 00401896 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 00401898 */
    if((f->flags&MI_ZF))goto mi_004018b6;
    /* 0040189a */
    f->r[0]=pq_read_00401800((0x00f5751cu),4u);
    /* 0040189f */
    f->r[2]=pq_read_00401800((0x00799a30u+f->r[3]),4u);
    /* 004018a5 */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 004018a8 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004018aa */
    f->r[7]=(0x00000000u+f->r[0]+f->r[2]*2u);
    /* 004018ad */
    f->r[0]=f->r[7];
    /* 004018af */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004018b0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004018b1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004018b2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004018b3 */
    f->r[4]+=12;return;
mi_004018b6:;
    /* 004018b6 */
    f->r[0]=pq_read_00401800((0x00f57514u),4u);
    /* 004018bb */
    a=f->r[7];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[7]=t;
    /* 004018be */
    a=f->r[7];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 004018c0 */
    a=f->r[7];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[7]=t;
    /* 004018c3 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004018c5 */
    f->r[0]=f->r[7];
    /* 004018c7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004018c8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004018c9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004018ca */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004018cb */
    f->r[4]+=12;return;
mi_004018ce:;
    /* 004018ce */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 004018d0 */
    if((f->flags&MI_ZF))goto mi_004018ef;
    /* 004018d2 */
    f->r[2]=pq_read_00401800((0x00f5752cu),4u);
    /* 004018d8 */
    f->r[1]=pq_read_00401800((0x00799a30u+f->r[3]),4u);
    /* 004018de */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 004018e1 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004018e3 */
    f->r[7]=(0x00000000u+f->r[2]+f->r[1]*2u);
    /* 004018e6 */
    f->r[0]=f->r[7];
    /* 004018e8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004018e9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004018ea */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004018eb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004018ec */
    f->r[4]+=12;return;
mi_004018ef:;
    /* 004018ef */
    f->r[0]=pq_read_00401800((0x00f57524u),4u);
    /* 004018f4 */
    a=f->r[7];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[7]=t;
    /* 004018f7 */
    a=f->r[7];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 004018f9 */
    a=f->r[7];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[7]=t;
    /* 004018fc */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004018fe */
    f->r[0]=f->r[7];
    /* 00401900 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00401901 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00401902 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00401903 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00401904 */
    f->r[4]+=12;return;
mi_00401907:;
    /* 00401907 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 00401909 */
    if((f->flags&MI_ZF))goto mi_00401928;
    /* 0040190b */
    f->r[1]=pq_read_00401800((0x00f57534u),4u);
    /* 00401911 */
    f->r[0]=pq_read_00401800((0x00799a30u+f->r[3]),4u);
    /* 00401917 */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 0040191a */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0040191c */
    f->r[7]=(0x00000000u+f->r[1]+f->r[0]*2u);
    /* 0040191f */
    f->r[0]=f->r[7];
    /* 00401921 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00401922 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00401923 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00401924 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00401925 */
    f->r[4]+=12;return;
mi_00401928:;
    /* 00401928 */
    f->r[2]=pq_read_00401800((0x00f57534u),4u);
    /* 0040192e */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 00401931 */
    f->r[7]=(0x0000000eu+f->r[6]+f->r[2]*1u);
    /* 00401935 */
    f->r[0]=f->r[7];
    /* 00401937 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00401938 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00401939 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0040193a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040193b */
    f->r[4]+=12;return;
mi_0040193e:;
    /* 0040193e */
    f->r[3]=pq_read_00401800((0x00799a30u+f->r[3]),4u);
    /* 00401944 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 00401946 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0040195e;
    /* 00401948 */
    f->r[0]=pq_read_00401800((0x00f5753cu),4u);
    /* 0040194d */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 00401950 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00401952 */
    f->r[7]=(0x00000000u+f->r[0]+f->r[3]*2u);
    /* 00401955 */
    f->r[0]=f->r[7];
    /* 00401957 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00401958 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00401959 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0040195a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040195b */
    f->r[4]+=12;return;
mi_0040195e:;
    /* 0040195e */
    f->r[0]=pq_read_00401800((0x00f5753cu),4u);
    /* 00401963 */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 00401966 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00401968 */
    f->r[7]=f->r[6];
    /* 0040196a */
    f->r[0]=f->r[7];
    /* 0040196c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040196d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040196e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0040196f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00401970 */
    f->r[4]+=12;return;
mi_00401973:;
    /* 00401973 */
    a=f->r[6];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 00401976 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00401a3e;
    /* 0040197c */
    switch(pq_read_00401800((0x00401a78u+f->r[6]*4u),4u)){case 0x00401983u:goto mi_00401983;case 0x004019a2u:goto mi_004019a2;case 0x004019b1u:goto mi_004019b1;case 0x004019ccu:goto mi_004019cc;default:abort();}
mi_00401983:;
    /* 00401983 */
    f->r[7]=pq_read_00401800((0x00f57564u),4u);
    /* 00401989 */
    f->r[0]=f->r[7];
    /* 0040198b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040198c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040198d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0040198e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040198f */
    f->r[4]+=12;return;
mi_00401992:;
    /* 00401992 */
    a=f->r[6];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 00401995 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00401a3e;
    /* 0040199b */
    switch(pq_read_00401800((0x00401a88u+f->r[6]*4u),4u)){case 0x00401983u:goto mi_00401983;case 0x004019a2u:goto mi_004019a2;case 0x004019b1u:goto mi_004019b1;case 0x004019ccu:goto mi_004019cc;default:abort();}
mi_004019a2:;
    /* 004019a2 */
    f->r[7]=pq_read_00401800((0x00f57574u),4u);
    /* 004019a8 */
    f->r[0]=f->r[7];
    /* 004019aa */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004019ab */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004019ac */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004019ad */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004019ae */
    f->r[4]+=12;return;
mi_004019b1:;
    /* 004019b1 */
    f->r[7]=pq_read_00401800((0x00f5757cu),4u);
    /* 004019b7 */
    f->r[0]=f->r[7];
    /* 004019b9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004019ba */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004019bb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004019bc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004019bd */
    f->r[4]+=12;return;
mi_004019c0:;
    /* 004019c0 */
    a=f->r[6];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 004019c3 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00401a3e;
    /* 004019c5 */
    switch(pq_read_00401800((0x00401a98u+f->r[6]*4u),4u)){case 0x00401983u:goto mi_00401983;case 0x004019a2u:goto mi_004019a2;case 0x004019b1u:goto mi_004019b1;case 0x004019ccu:goto mi_004019cc;default:abort();}
mi_004019cc:;
    /* 004019cc */
    f->r[7]=pq_read_00401800((0x00f5756cu),4u);
    /* 004019d2 */
    f->r[0]=f->r[7];
    /* 004019d4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004019d5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004019d6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004019d7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004019d8 */
    f->r[4]+=12;return;
mi_004019db:;
    /* 004019db */
    a=f->r[6];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 004019de */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00401a3e;
    /* 004019e0 */
    switch(pq_read_00401800((0x00401aa8u+f->r[6]*4u),4u)){case 0x004019ffu:goto mi_004019ff;case 0x00401a0eu:goto mi_00401a0e;case 0x00401a1du:goto mi_00401a1d;case 0x00401a38u:goto mi_00401a38;default:abort();}
mi_004019e7:;
    /* 004019e7 */
    a=f->r[6];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 004019ea */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00401a3e;
    /* 004019ec */
    switch(pq_read_00401800((0x00401ab8u+f->r[6]*4u),4u)){case 0x004019ffu:goto mi_004019ff;case 0x00401a0eu:goto mi_00401a0e;case 0x00401a1du:goto mi_00401a1d;case 0x00401a38u:goto mi_00401a38;default:abort();}
mi_004019f3:;
    /* 004019f3 */
    a=f->r[6];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 004019f6 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00401a3e;
    /* 004019f8 */
    switch(pq_read_00401800((0x00401ac8u+f->r[6]*4u),4u)){case 0x004019ffu:goto mi_004019ff;case 0x00401a0eu:goto mi_00401a0e;case 0x00401a1du:goto mi_00401a1d;case 0x00401a38u:goto mi_00401a38;default:abort();}
mi_004019ff:;
    /* 004019ff */
    f->r[7]=pq_read_00401800((0x00f5755cu),4u);
    /* 00401a05 */
    f->r[0]=f->r[7];
    /* 00401a07 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00401a08 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00401a09 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00401a0a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00401a0b */
    f->r[4]+=12;return;
mi_00401a0e:;
    /* 00401a0e */
    f->r[7]=pq_read_00401800((0x00f57554u),4u);
    /* 00401a14 */
    f->r[0]=f->r[7];
    /* 00401a16 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00401a17 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00401a18 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00401a19 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00401a1a */
    f->r[4]+=12;return;
mi_00401a1d:;
    /* 00401a1d */
    f->r[7]=pq_read_00401800((0x00f5754cu),4u);
    /* 00401a23 */
    f->r[0]=f->r[7];
    /* 00401a25 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00401a26 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00401a27 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00401a28 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00401a29 */
    f->r[4]+=12;return;
mi_00401a2c:;
    /* 00401a2c */
    a=f->r[6];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 00401a2f */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00401a3e;
    /* 00401a31 */
    switch(pq_read_00401800((0x00401ad8u+f->r[6]*4u),4u)){case 0x004019ffu:goto mi_004019ff;case 0x00401a0eu:goto mi_00401a0e;case 0x00401a1du:goto mi_00401a1d;case 0x00401a38u:goto mi_00401a38;default:abort();}
mi_00401a38:;
    /* 00401a38 */
    f->r[7]=pq_read_00401800((0x00f57544u),4u);
mi_00401a3e:;
    /* 00401a3e */
    f->r[0]=f->r[7];
    /* 00401a40 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00401a41 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00401a42 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00401a43 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00401a44 */
    f->r[4]+=12;return;
}

/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */

#include <stdlib.h>



static const uint8_t mi_rom_0040eb20[]={0xec,0xde,0x40,0x00,0x61,0xdf,0x40,0x00,0x7c,0xe0,0x40,0x00,0x46,0xe3,0x40,0x00,0xb9,0xe3,0x40,0x00,0xc3,0xe6,0x40,0x00,0x2e,0xe6,0x40,0x00,0x46,0xe7,0x40,0x00,0xc9,0xe7,0x40,0x00,0x4c,0xe8,0x40,0x00,0xd1,0xe8,0x40,0x00,0x0e,0xe9,0x40,0x00,0x37,0xe9,0x40,0x00,0xa4,0xe1,0x40,0x00,0xf4,0xe1,0x40,0x00,0x30,0xe5,0x40,0x00,0xae,0xe5,0x40,0x00,0xbc,0xe9,0x40,0x00,0xf9,0xe9,0x40,0x00,0x34,0xe4,0x40,0x00,0xb2,0xe4,0x40,0x00,0x44,0xe2,0x40,0x00,0xc5,0xe2,0x40,0x00,0x22,0xea,0x40,0x00,0xa2,0xea,0x40,0x00};
static __inline uint32_t pq_read_0040de80(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x0040eb20u&&width<=100u&&(offset=address-0x0040eb20u)<=100u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0040eb20[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}

void restore_placement_query_C_0040de80(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0040de80 */
    f->r[2]=pq_read_0040de80((0x00000004u+f->r[4]),4u);
    /* 0040de84 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040de85 */
    f->r[0]=f->r[2];
    /* 0040de87 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040de88 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0040de8b */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0040de8d */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040de8e */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0040de91 */
    f->r[7]=pq_read_0040de80((0x00799a30u+f->r[0]),4u);
    /* 0040de97 */
    a=f->r[7];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 0040de9a */
    if(mi_signed_less(f->flags))goto mi_0040deb3;
    /* 0040de9c */
    f->r[6]=(uint32_t)(int32_t)(int16_t)pq_read_0040de80((0x007999eau+f->r[0]),2u);
    /* 0040dea3 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0040dea5 */
    f->r[6]=f->r[2];
    /* 0040dea7 */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0040deaa */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0040deac */
    f->r[7]=pq_read_0040de80((0x00799a30u+f->r[6]*8u),4u);
mi_0040deb3:;
    /* 0040deb3 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)pq_read_0040de80((0x007999eau+f->r[0]),2u);
    /* 0040deba */
    f->r[3]=pq_read_0040de80((0x00000014u+f->r[4]),4u);
    /* 0040debe */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0040dec1 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*4u);
    /* 0040dec4 */
    f->r[1]=pq_read_0040de80((0x00000058u+f->r[1]+f->r[0]*4u),4u);
    /* 0040dec8 */
    f->r[0]=pq_read_0040de80((0x00bca2e0u),4u);
    /* 0040decd */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0040dece */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0040ded0 */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(0x00000000u);
    /* 0040ded6 */
    f->r[6]=f->r[0];
    /* 0040ded8 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0040deda */
    a=f->r[6];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[6]=t;
    /* 0040dedc */
    a=f->r[1];b=0x00000018u;t=mi_alu(f,a,b,32,1);
    /* 0040dedf */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0040eb1a;
    /* 0040dee5 */
    switch(pq_read_0040de80((0x0040eb20u+f->r[1]*4u),4u)){case 0x0040deecu:goto mi_0040deec;case 0x0040df61u:goto mi_0040df61;case 0x0040e07cu:goto mi_0040e07c;case 0x0040e1a4u:goto mi_0040e1a4;case 0x0040e1f4u:goto mi_0040e1f4;case 0x0040e244u:goto mi_0040e244;case 0x0040e2c5u:goto mi_0040e2c5;case 0x0040e346u:goto mi_0040e346;case 0x0040e3b9u:goto mi_0040e3b9;case 0x0040e434u:goto mi_0040e434;case 0x0040e4b2u:goto mi_0040e4b2;case 0x0040e530u:goto mi_0040e530;case 0x0040e5aeu:goto mi_0040e5ae;case 0x0040e62eu:goto mi_0040e62e;case 0x0040e6c3u:goto mi_0040e6c3;case 0x0040e746u:goto mi_0040e746;case 0x0040e7c9u:goto mi_0040e7c9;case 0x0040e84cu:goto mi_0040e84c;case 0x0040e8d1u:goto mi_0040e8d1;case 0x0040e90eu:goto mi_0040e90e;case 0x0040e937u:goto mi_0040e937;case 0x0040e9bcu:goto mi_0040e9bc;case 0x0040e9f9u:goto mi_0040e9f9;case 0x0040ea22u:goto mi_0040ea22;case 0x0040eaa2u:goto mi_0040eaa2;default:abort();}
mi_0040deec:;
    /* 0040deec */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040deef */
    if(!mi_signed_less(f->flags))goto mi_0040deff;
    /* 0040def1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040def2 */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(0x00000001u);
    /* 0040def8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040def9 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0040defb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040defc */
    f->r[4]+=12;return;
mi_0040deff:;
    /* 0040deff */
    a=f->r[7];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 0040df02 */
    if(mi_signed_less(f->flags))goto mi_0040e70b;
    /* 0040df08 */
    a=f->r[7];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 0040df0b */
    if(!mi_signed_less(f->flags))goto mi_0040df16;
    /* 0040df0d */
    a=0x00000015u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040df0f */
    a=0x00576338u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040df14 */
    goto mi_0040df3e;
mi_0040df16:;
    /* 0040df16 */
    a=f->r[7];b=0x0000000bu;t=mi_alu(f,a,b,32,1);
    /* 0040df19 */
    if(!mi_signed_less(f->flags))goto mi_0040df24;
    /* 0040df1b */
    a=0x00000015u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040df1d */
    a=0x0057632cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040df22 */
    goto mi_0040df3e;
mi_0040df24:;
    /* 0040df24 */
    a=f->r[7];b=0x0000000eu;t=mi_alu(f,a,b,32,1);
    /* 0040df27 */
    if(!mi_signed_less(f->flags))goto mi_0040df32;
    /* 0040df29 */
    a=0x00000015u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040df2b */
    a=0x00576320u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040df30 */
    goto mi_0040df3e;
mi_0040df32:;
    /* 0040df32 */
    a=f->r[7];b=0x00000012u;t=mi_alu(f,a,b,32,1);
    /* 0040df35 */
    a=0x00000015u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040df37 */
    if(!mi_signed_less(f->flags))goto mi_0040df5a;
    /* 0040df39 */
    a=0x00576314u;f->r[4]-=4;MI_U32(f->r[4])=a;
mi_0040df3e:;
    /* 0040df3e */
    f->r[1]=0x00f55bb0u;
    /* 0040df43 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040df48u;f->boundary(0x0040df43u,0x0053dd90u,f);
    /* 0040df48 */
    f->r[0]=pq_read_0040de80((0x00f573acu),4u);
    /* 0040df4d */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 0040df50 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0040df52 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040df53 */
    f->r[0]=f->r[6];
    /* 0040df55 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040df56 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040df57 */
    f->r[4]+=12;return;
mi_0040df5a:;
    /* 0040df5a */
    a=0x00576308u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040df5f */
    goto mi_0040df3e;
mi_0040df61:;
    /* 0040df61 */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040df64 */
    if(!mi_signed_less(f->flags))goto mi_0040df74;
    /* 0040df66 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040df67 */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(0x00000001u);
    /* 0040df6d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040df6e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0040df70 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040df71 */
    f->r[4]+=12;return;
mi_0040df74:;
    /* 0040df74 */
    a=f->r[7];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 0040df77 */
    if(mi_signed_less(f->flags))goto mi_0040e70b;
    /* 0040df7d */
    a=f->r[7];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 0040df80 */
    if(!mi_signed_less(f->flags))goto mi_0040df97;
    /* 0040df82 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040df85 */
    if(!mi_signed_less(f->flags))goto mi_0040ea3a;
    /* 0040df8b */
    a=0x00000014u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040df8d */
    a=0x005762fcu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040df92 */
    goto mi_0040e060;
mi_0040df97:;
    /* 0040df97 */
    a=f->r[7];b=0x00000007u;t=mi_alu(f,a,b,32,1);
    /* 0040df9a */
    if(!mi_signed_less(f->flags))goto mi_0040dfb1;
    /* 0040df9c */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040df9f */
    if(!mi_signed_less(f->flags))goto mi_0040e0df;
mi_0040dfa5:;
    /* 0040dfa5 */
    a=0x00000014u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040dfa7 */
    a=0x005762fcu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040dfac */
    goto mi_0040e060;
mi_0040dfb1:;
    /* 0040dfb1 */
    a=f->r[7];b=0x00000009u;t=mi_alu(f,a,b,32,1);
    /* 0040dfb4 */
    if(!mi_signed_less(f->flags))goto mi_0040dfcb;
    /* 0040dfb6 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040dfb9 */
    if(!mi_signed_less(f->flags))goto mi_0040e0df;
    /* 0040dfbf */
    a=0x00000014u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040dfc1 */
    a=0x005762f0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040dfc6 */
    goto mi_0040e060;
mi_0040dfcb:;
    /* 0040dfcb */
    a=f->r[7];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 0040dfce */
    if(!mi_signed_less(f->flags))goto mi_0040dfe2;
    /* 0040dfd0 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040dfd3 */
    if(!mi_signed_less(f->flags))goto mi_0040e113;
mi_0040dfd9:;
    /* 0040dfd9 */
    a=0x00000014u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040dfdb */
    a=0x005762f0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040dfe0 */
    goto mi_0040e060;
mi_0040dfe2:;
    /* 0040dfe2 */
    a=f->r[7];b=0x0000000cu;t=mi_alu(f,a,b,32,1);
    /* 0040dfe5 */
    if(!mi_signed_less(f->flags))goto mi_0040dff9;
    /* 0040dfe7 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040dfea */
    if(!mi_signed_less(f->flags))goto mi_0040e113;
    /* 0040dff0 */
    a=0x00000014u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040dff2 */
    a=0x005762e4u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040dff7 */
    goto mi_0040e060;
mi_0040dff9:;
    /* 0040dff9 */
    a=f->r[7];b=0x0000000du;t=mi_alu(f,a,b,32,1);
    /* 0040dffc */
    if(!mi_signed_less(f->flags))goto mi_0040e010;
    /* 0040dffe */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e001 */
    if(!mi_signed_less(f->flags))goto mi_0040e141;
mi_0040e007:;
    /* 0040e007 */
    a=0x00000014u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e009 */
    a=0x005762e4u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e00e */
    goto mi_0040e060;
mi_0040e010:;
    /* 0040e010 */
    a=f->r[7];b=0x0000000fu;t=mi_alu(f,a,b,32,1);
    /* 0040e013 */
    if(!mi_signed_less(f->flags))goto mi_0040e027;
    /* 0040e015 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e018 */
    if(!mi_signed_less(f->flags))goto mi_0040e141;
    /* 0040e01e */
    a=0x00000014u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e020 */
    a=0x005762d8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e025 */
    goto mi_0040e060;
mi_0040e027:;
    /* 0040e027 */
    a=f->r[7];b=0x00000011u;t=mi_alu(f,a,b,32,1);
    /* 0040e02a */
    if(!mi_signed_less(f->flags))goto mi_0040e03a;
    /* 0040e02c */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e02f */
    if(!mi_signed_less(f->flags))goto mi_0040e044;
mi_0040e031:;
    /* 0040e031 */
    a=0x00000014u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e033 */
    a=0x005762d8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e038 */
    goto mi_0040e060;
mi_0040e03a:;
    /* 0040e03a */
    a=f->r[7];b=0x00000013u;t=mi_alu(f,a,b,32,1);
    /* 0040e03d */
    if(!mi_signed_less(f->flags))goto mi_0040e050;
    /* 0040e03f */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e042 */
    if(mi_signed_less(f->flags))goto mi_0040e059;
mi_0040e044:;
    /* 0040e044 */
    a=0x00000016u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e046 */
    a=0x005762ccu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e04b */
    goto mi_0040e188;
mi_0040e050:;
    /* 0040e050 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e053 */
    if(!mi_signed_less(f->flags))goto mi_0040e181;
mi_0040e059:;
    /* 0040e059 */
    a=0x00000014u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e05b */
    a=0x005762c0u;f->r[4]-=4;MI_U32(f->r[4])=a;
mi_0040e060:;
    /* 0040e060 */
    f->r[1]=0x00f55bb0u;
    /* 0040e065 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040e06au;f->boundary(0x0040e065u,0x0053dd90u,f);
    /* 0040e06a */
    f->r[0]=pq_read_0040de80((0x00f573a4u),4u);
    /* 0040e06f */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 0040e072 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0040e074 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e075 */
    f->r[0]=f->r[6];
    /* 0040e077 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e078 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e079 */
    f->r[4]+=12;return;
mi_0040e07c:;
    /* 0040e07c */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e07f */
    if(!mi_signed_less(f->flags))goto mi_0040e08f;
    /* 0040e081 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e082 */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(0x00000001u);
    /* 0040e088 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e089 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0040e08b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e08c */
    f->r[4]+=12;return;
mi_0040e08f:;
    /* 0040e08f */
    a=f->r[7];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 0040e092 */
    if(mi_signed_less(f->flags))goto mi_0040e70b;
    /* 0040e098 */
    a=f->r[7];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 0040e09b */
    if(!mi_signed_less(f->flags))goto mi_0040e0b7;
    /* 0040e09d */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e0a0 */
    if(!mi_signed_less(f->flags))goto mi_0040dfa5;
    /* 0040e0a6 */
    f->r[0]=pq_read_0040de80((0x00f56b54u),4u);
    /* 0040e0ab */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e0ac */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(0x00000001u);
    /* 0040e0b2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e0b3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e0b4 */
    f->r[4]+=12;return;
mi_0040e0b7:;
    /* 0040e0b7 */
    a=f->r[7];b=0x00000007u;t=mi_alu(f,a,b,32,1);
    /* 0040e0ba */
    if(!mi_signed_less(f->flags))goto mi_0040e0d1;
    /* 0040e0bc */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e0bf */
    if(!mi_signed_less(f->flags))goto mi_0040dfa5;
    /* 0040e0c5 */
    a=0x00000016u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e0c7 */
    a=0x005762b4u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e0cc */
    goto mi_0040e188;
mi_0040e0d1:;
    /* 0040e0d1 */
    a=f->r[7];b=0x00000009u;t=mi_alu(f,a,b,32,1);
    /* 0040e0d4 */
    if(!mi_signed_less(f->flags))goto mi_0040e0eb;
    /* 0040e0d6 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e0d9 */
    if(!mi_signed_less(f->flags))goto mi_0040dfd9;
mi_0040e0df:;
    /* 0040e0df */
    a=0x00000016u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e0e1 */
    a=0x005762b4u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e0e6 */
    goto mi_0040e188;
mi_0040e0eb:;
    /* 0040e0eb */
    a=f->r[7];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 0040e0ee */
    if(!mi_signed_less(f->flags))goto mi_0040e105;
    /* 0040e0f0 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e0f3 */
    if(!mi_signed_less(f->flags))goto mi_0040dfd9;
    /* 0040e0f9 */
    a=0x00000016u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e0fb */
    a=0x005762a8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e100 */
    goto mi_0040e188;
mi_0040e105:;
    /* 0040e105 */
    a=f->r[7];b=0x0000000cu;t=mi_alu(f,a,b,32,1);
    /* 0040e108 */
    if(!mi_signed_less(f->flags))goto mi_0040e11c;
    /* 0040e10a */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e10d */
    if(!mi_signed_less(f->flags))goto mi_0040e007;
mi_0040e113:;
    /* 0040e113 */
    a=0x00000016u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e115 */
    a=0x005762a8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e11a */
    goto mi_0040e188;
mi_0040e11c:;
    /* 0040e11c */
    a=f->r[7];b=0x0000000du;t=mi_alu(f,a,b,32,1);
    /* 0040e11f */
    if(!mi_signed_less(f->flags))goto mi_0040e133;
    /* 0040e121 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e124 */
    if(!mi_signed_less(f->flags))goto mi_0040e007;
    /* 0040e12a */
    a=0x00000016u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e12c */
    a=0x0057629cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e131 */
    goto mi_0040e188;
mi_0040e133:;
    /* 0040e133 */
    a=f->r[7];b=0x0000000fu;t=mi_alu(f,a,b,32,1);
    /* 0040e136 */
    if(!mi_signed_less(f->flags))goto mi_0040e14a;
    /* 0040e138 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e13b */
    if(!mi_signed_less(f->flags))goto mi_0040e031;
mi_0040e141:;
    /* 0040e141 */
    a=0x00000016u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e143 */
    a=0x0057629cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e148 */
    goto mi_0040e188;
mi_0040e14a:;
    /* 0040e14a */
    a=f->r[7];b=0x00000011u;t=mi_alu(f,a,b,32,1);
    /* 0040e14d */
    if(!mi_signed_less(f->flags))goto mi_0040e161;
    /* 0040e14f */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e152 */
    if(!mi_signed_less(f->flags))goto mi_0040e031;
    /* 0040e158 */
    a=0x00000016u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e15a */
    a=0x005762ccu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e15f */
    goto mi_0040e188;
mi_0040e161:;
    /* 0040e161 */
    a=f->r[7];b=0x00000013u;t=mi_alu(f,a,b,32,1);
    /* 0040e164 */
    if(!mi_signed_less(f->flags))goto mi_0040e178;
    /* 0040e166 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e169 */
    if(!mi_signed_less(f->flags))goto mi_0040e059;
    /* 0040e16f */
    a=0x00000016u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e171 */
    a=0x005762ccu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e176 */
    goto mi_0040e188;
mi_0040e178:;
    /* 0040e178 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e17b */
    if(!mi_signed_less(f->flags))goto mi_0040e059;
mi_0040e181:;
    /* 0040e181 */
    a=0x00000016u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e183 */
    a=0x00576290u;f->r[4]-=4;MI_U32(f->r[4])=a;
mi_0040e188:;
    /* 0040e188 */
    f->r[1]=0x00f55bb0u;
    /* 0040e18d */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040e192u;f->boundary(0x0040e18du,0x0053dd90u,f);
    /* 0040e192 */
    f->r[0]=pq_read_0040de80((0x00f573b4u),4u);
    /* 0040e197 */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 0040e19a */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0040e19c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e19d */
    f->r[0]=f->r[6];
    /* 0040e19f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e1a0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e1a1 */
    f->r[4]+=12;return;
mi_0040e1a4:;
    /* 0040e1a4 */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e1a7 */
    if(!mi_signed_less(f->flags))goto mi_0040e1b7;
    /* 0040e1a9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e1aa */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(0x00000001u);
    /* 0040e1b0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e1b1 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0040e1b3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e1b4 */
    f->r[4]+=12;return;
mi_0040e1b7:;
    /* 0040e1b7 */
    a=f->r[7];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 0040e1ba */
    if(mi_signed_less(f->flags))goto mi_0040e70b;
    /* 0040e1c0 */
    a=f->r[7];b=0x00000014u;t=mi_alu(f,a,b,32,1);
    /* 0040e1c3 */
    a=0x00000017u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e1c5 */
    if(!mi_signed_less(f->flags))goto mi_0040e1ce;
    /* 0040e1c7 */
    a=0x00576284u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e1cc */
    goto mi_0040e1d3;
mi_0040e1ce:;
    /* 0040e1ce */
    a=0x00576278u;f->r[4]-=4;MI_U32(f->r[4])=a;
mi_0040e1d3:;
    /* 0040e1d3 */
    f->r[1]=0x00f55bb0u;
    /* 0040e1d8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040e1ddu;f->boundary(0x0040e1d8u,0x0053dd90u,f);
    /* 0040e1dd */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e1e0 */
    if(!mi_signed_less(f->flags))goto mi_0040e232;
mi_0040e1e2:;
    /* 0040e1e2 */
    f->r[0]=pq_read_0040de80((0x00f57414u),4u);
    /* 0040e1e7 */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 0040e1ea */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0040e1ec */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e1ed */
    f->r[0]=f->r[6];
    /* 0040e1ef */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e1f0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e1f1 */
    f->r[4]+=12;return;
mi_0040e1f4:;
    /* 0040e1f4 */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e1f7 */
    if(!mi_signed_less(f->flags))goto mi_0040e207;
    /* 0040e1f9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e1fa */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(0x00000001u);
    /* 0040e200 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e201 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0040e203 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e204 */
    f->r[4]+=12;return;
mi_0040e207:;
    /* 0040e207 */
    a=f->r[7];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 0040e20a */
    if(mi_signed_less(f->flags))goto mi_0040e70b;
    /* 0040e210 */
    a=f->r[7];b=0x00000014u;t=mi_alu(f,a,b,32,1);
    /* 0040e213 */
    a=0x00000017u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e215 */
    if(!mi_signed_less(f->flags))goto mi_0040e21e;
    /* 0040e217 */
    a=0x00576284u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e21c */
    goto mi_0040e223;
mi_0040e21e:;
    /* 0040e21e */
    a=0x00576278u;f->r[4]-=4;MI_U32(f->r[4])=a;
mi_0040e223:;
    /* 0040e223 */
    f->r[1]=0x00f55bb0u;
    /* 0040e228 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040e22du;f->boundary(0x0040e228u,0x0053dd90u,f);
    /* 0040e22d */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e230 */
    if(!mi_signed_less(f->flags))goto mi_0040e1e2;
mi_0040e232:;
    /* 0040e232 */
    f->r[0]=pq_read_0040de80((0x00f5741cu),4u);
    /* 0040e237 */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 0040e23a */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0040e23c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e23d */
    f->r[0]=f->r[6];
    /* 0040e23f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e240 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e241 */
    f->r[4]+=12;return;
mi_0040e244:;
    /* 0040e244 */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e247 */
    if(!mi_signed_less(f->flags))goto mi_0040e25a;
    /* 0040e249 */
    f->r[0]=pq_read_0040de80((0x00f573bcu),4u);
    /* 0040e24e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e24f */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(0x00000007u);
    /* 0040e255 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e256 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e257 */
    f->r[4]+=12;return;
mi_0040e25a:;
    /* 0040e25a */
    a=f->r[7];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 0040e25d */
    if(mi_signed_less(f->flags))goto mi_0040e70b;
    /* 0040e263 */
    a=f->r[7];b=0x00000014u;t=mi_alu(f,a,b,32,1);
    /* 0040e266 */
    if(!mi_signed_less(f->flags))goto mi_0040e294;
    /* 0040e268 */
    a=0x00000017u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e26a */
    a=0x00576284u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e26f */
    f->r[1]=0x00f55bb0u;
mi_0040e274:;
    /* 0040e274 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040e279u;f->boundary(0x0040e274u,0x0053dd90u,f);
    /* 0040e279 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e27c */
    if(!mi_signed_less(f->flags))goto mi_0040ea3a;
    /* 0040e282 */
    f->r[0]=pq_read_0040de80((0x00f57424u),4u);
    /* 0040e287 */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 0040e28a */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0040e28c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e28d */
    f->r[0]=f->r[6];
    /* 0040e28f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e290 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e291 */
    f->r[4]+=12;return;
mi_0040e294:;
    /* 0040e294 */
    a=f->r[7];b=0x00000015u;t=mi_alu(f,a,b,32,1);
    /* 0040e297 */
    a=0x00000017u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e299 */
    a=0x00576278u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e29e */
    f->r[1]=0x00f55bb0u;
    /* 0040e2a3 */
    if(mi_signed_less(f->flags))goto mi_0040e274;
    /* 0040e2a5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040e2aau;f->boundary(0x0040e2a5u,0x0053dd90u,f);
    /* 0040e2aa */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e2ad */
    if(!mi_signed_less(f->flags))goto mi_0040e429;
mi_0040e2b3:;
    /* 0040e2b3 */
    f->r[0]=pq_read_0040de80((0x00f57424u),4u);
    /* 0040e2b8 */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 0040e2bb */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0040e2bd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e2be */
    f->r[0]=f->r[6];
    /* 0040e2c0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e2c1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e2c2 */
    f->r[4]+=12;return;
mi_0040e2c5:;
    /* 0040e2c5 */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e2c8 */
    if(!mi_signed_less(f->flags))goto mi_0040e2db;
    /* 0040e2ca */
    f->r[0]=pq_read_0040de80((0x00f573bcu),4u);
    /* 0040e2cf */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e2d0 */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(0x00000007u);
    /* 0040e2d6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e2d7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e2d8 */
    f->r[4]+=12;return;
mi_0040e2db:;
    /* 0040e2db */
    a=f->r[7];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 0040e2de */
    if(mi_signed_less(f->flags))goto mi_0040e70b;
    /* 0040e2e4 */
    a=f->r[7];b=0x00000014u;t=mi_alu(f,a,b,32,1);
    /* 0040e2e7 */
    if(!mi_signed_less(f->flags))goto mi_0040e315;
    /* 0040e2e9 */
    a=0x00000017u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e2eb */
    a=0x00576284u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e2f0 */
    f->r[1]=0x00f55bb0u;
mi_0040e2f5:;
    /* 0040e2f5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040e2fau;f->boundary(0x0040e2f5u,0x0053dd90u,f);
    /* 0040e2fa */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e2fd */
    if(!mi_signed_less(f->flags))goto mi_0040ea3a;
    /* 0040e303 */
    f->r[0]=pq_read_0040de80((0x00f5742cu),4u);
    /* 0040e308 */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 0040e30b */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0040e30d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e30e */
    f->r[0]=f->r[6];
    /* 0040e310 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e311 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e312 */
    f->r[4]+=12;return;
mi_0040e315:;
    /* 0040e315 */
    a=f->r[7];b=0x00000015u;t=mi_alu(f,a,b,32,1);
    /* 0040e318 */
    a=0x00000017u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e31a */
    a=0x00576278u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e31f */
    f->r[1]=0x00f55bb0u;
    /* 0040e324 */
    if(mi_signed_less(f->flags))goto mi_0040e2f5;
    /* 0040e326 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040e32bu;f->boundary(0x0040e326u,0x0053dd90u,f);
    /* 0040e32b */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e32e */
    if(!mi_signed_less(f->flags))goto mi_0040e429;
mi_0040e334:;
    /* 0040e334 */
    f->r[0]=pq_read_0040de80((0x00f5742cu),4u);
    /* 0040e339 */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 0040e33c */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0040e33e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e33f */
    f->r[0]=f->r[6];
    /* 0040e341 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e342 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e343 */
    f->r[4]+=12;return;
mi_0040e346:;
    /* 0040e346 */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e349 */
    if(!mi_signed_less(f->flags))goto mi_0040e35c;
    /* 0040e34b */
    f->r[0]=pq_read_0040de80((0x00f573bcu),4u);
    /* 0040e350 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e351 */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(0x00000007u);
    /* 0040e357 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e358 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e359 */
    f->r[4]+=12;return;
mi_0040e35c:;
    /* 0040e35c */
    a=f->r[7];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 0040e35f */
    if(mi_signed_less(f->flags))goto mi_0040e70b;
    /* 0040e365 */
    a=f->r[7];b=0x00000014u;t=mi_alu(f,a,b,32,1);
    /* 0040e368 */
    if(!mi_signed_less(f->flags))goto mi_0040e378;
    /* 0040e36a */
    a=0x00000017u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e36c */
    a=0x00576284u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e371 */
    f->r[1]=0x00f55bb0u;
    /* 0040e376 */
    goto mi_0040e389;
mi_0040e378:;
    /* 0040e378 */
    a=f->r[7];b=0x00000015u;t=mi_alu(f,a,b,32,1);
    /* 0040e37b */
    a=0x00000017u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e37d */
    a=0x00576278u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e382 */
    f->r[1]=0x00f55bb0u;
    /* 0040e387 */
    if(!mi_signed_less(f->flags))goto mi_0040e3a4;
mi_0040e389:;
    /* 0040e389 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040e38eu;f->boundary(0x0040e389u,0x0053dd90u,f);
    /* 0040e38e */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e391 */
    if(!mi_signed_less(f->flags))goto mi_0040e334;
    /* 0040e393 */
    f->r[0]=pq_read_0040de80((0x00f56b54u),4u);
    /* 0040e398 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e399 */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(0x00000001u);
    /* 0040e39f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e3a0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e3a1 */
    f->r[4]+=12;return;
mi_0040e3a4:;
    /* 0040e3a4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040e3a9u;f->boundary(0x0040e3a4u,0x0053dd90u,f);
    /* 0040e3a9 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e3ac */
    if(!mi_signed_less(f->flags))goto mi_0040e334;
    /* 0040e3ae */
    f->r[0]=pq_read_0040de80((0x00f573c4u),4u);
    /* 0040e3b3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e3b4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e3b5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e3b6 */
    f->r[4]+=12;return;
mi_0040e3b9:;
    /* 0040e3b9 */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e3bc */
    if(!mi_signed_less(f->flags))goto mi_0040e3cf;
    /* 0040e3be */
    f->r[0]=pq_read_0040de80((0x00f573bcu),4u);
    /* 0040e3c3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e3c4 */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(0x00000007u);
    /* 0040e3ca */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e3cb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e3cc */
    f->r[4]+=12;return;
mi_0040e3cf:;
    /* 0040e3cf */
    a=f->r[7];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 0040e3d2 */
    if(mi_signed_less(f->flags))goto mi_0040e70b;
    /* 0040e3d8 */
    a=f->r[7];b=0x00000014u;t=mi_alu(f,a,b,32,1);
    /* 0040e3db */
    if(!mi_signed_less(f->flags))goto mi_0040e3eb;
    /* 0040e3dd */
    a=0x00000017u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e3df */
    a=0x00576284u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e3e4 */
    f->r[1]=0x00f55bb0u;
    /* 0040e3e9 */
    goto mi_0040e3fc;
mi_0040e3eb:;
    /* 0040e3eb */
    a=f->r[7];b=0x00000015u;t=mi_alu(f,a,b,32,1);
    /* 0040e3ee */
    a=0x00000017u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e3f0 */
    a=0x00576278u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e3f5 */
    f->r[1]=0x00f55bb0u;
    /* 0040e3fa */
    if(!mi_signed_less(f->flags))goto mi_0040e41b;
mi_0040e3fc:;
    /* 0040e3fc */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040e401u;f->boundary(0x0040e3fcu,0x0053dd90u,f);
    /* 0040e401 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e404 */
    if(!mi_signed_less(f->flags))goto mi_0040e2b3;
    /* 0040e40a */
    f->r[0]=pq_read_0040de80((0x00f56b54u),4u);
    /* 0040e40f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e410 */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(0x00000001u);
    /* 0040e416 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e417 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e418 */
    f->r[4]+=12;return;
mi_0040e41b:;
    /* 0040e41b */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040e420u;f->boundary(0x0040e41bu,0x0053dd90u,f);
    /* 0040e420 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e423 */
    if(!mi_signed_less(f->flags))goto mi_0040e2b3;
mi_0040e429:;
    /* 0040e429 */
    f->r[0]=pq_read_0040de80((0x00f573c4u),4u);
    /* 0040e42e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e42f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e430 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e431 */
    f->r[4]+=12;return;
mi_0040e434:;
    /* 0040e434 */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e437 */
    if(!mi_signed_less(f->flags))goto mi_0040e447;
    /* 0040e439 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e43a */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(0x00000001u);
    /* 0040e440 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e441 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0040e443 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e444 */
    f->r[4]+=12;return;
mi_0040e447:;
    /* 0040e447 */
    a=f->r[7];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 0040e44a */
    if(mi_signed_less(f->flags))goto mi_0040e70b;
    /* 0040e450 */
    a=f->r[7];b=0x00000014u;t=mi_alu(f,a,b,32,1);
    /* 0040e453 */
    if(!mi_signed_less(f->flags))goto mi_0040e481;
    /* 0040e455 */
    a=0x00000017u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e457 */
    a=0x00576284u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e45c */
    f->r[1]=0x00f55bb0u;
mi_0040e461:;
    /* 0040e461 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040e466u;f->boundary(0x0040e461u,0x0053dd90u,f);
    /* 0040e466 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e469 */
    if(!mi_signed_less(f->flags))goto mi_0040ea3a;
    /* 0040e46f */
    f->r[0]=pq_read_0040de80((0x00f57404u),4u);
    /* 0040e474 */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 0040e477 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0040e479 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e47a */
    f->r[0]=f->r[6];
    /* 0040e47c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e47d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e47e */
    f->r[4]+=12;return;
mi_0040e481:;
    /* 0040e481 */
    a=f->r[7];b=0x00000015u;t=mi_alu(f,a,b,32,1);
    /* 0040e484 */
    a=0x00000017u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e486 */
    a=0x00576278u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e48b */
    f->r[1]=0x00f55bb0u;
    /* 0040e490 */
    if(mi_signed_less(f->flags))goto mi_0040e461;
    /* 0040e492 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040e497u;f->boundary(0x0040e492u,0x0053dd90u,f);
    /* 0040e497 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e49a */
    if(!mi_signed_less(f->flags))goto mi_0040e73b;
mi_0040e4a0:;
    /* 0040e4a0 */
    f->r[0]=pq_read_0040de80((0x00f57404u),4u);
    /* 0040e4a5 */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 0040e4a8 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0040e4aa */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e4ab */
    f->r[0]=f->r[6];
    /* 0040e4ad */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e4ae */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e4af */
    f->r[4]+=12;return;
mi_0040e4b2:;
    /* 0040e4b2 */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e4b5 */
    if(!mi_signed_less(f->flags))goto mi_0040e4c5;
    /* 0040e4b7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e4b8 */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(0x00000001u);
    /* 0040e4be */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e4bf */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0040e4c1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e4c2 */
    f->r[4]+=12;return;
mi_0040e4c5:;
    /* 0040e4c5 */
    a=f->r[7];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 0040e4c8 */
    if(mi_signed_less(f->flags))goto mi_0040e70b;
    /* 0040e4ce */
    a=f->r[7];b=0x00000014u;t=mi_alu(f,a,b,32,1);
    /* 0040e4d1 */
    if(!mi_signed_less(f->flags))goto mi_0040e4ff;
    /* 0040e4d3 */
    a=0x00000017u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e4d5 */
    a=0x00576284u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e4da */
    f->r[1]=0x00f55bb0u;
mi_0040e4df:;
    /* 0040e4df */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040e4e4u;f->boundary(0x0040e4dfu,0x0053dd90u,f);
    /* 0040e4e4 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e4e7 */
    if(!mi_signed_less(f->flags))goto mi_0040ea3a;
    /* 0040e4ed */
    f->r[0]=pq_read_0040de80((0x00f5740cu),4u);
    /* 0040e4f2 */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 0040e4f5 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0040e4f7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e4f8 */
    f->r[0]=f->r[6];
    /* 0040e4fa */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e4fb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e4fc */
    f->r[4]+=12;return;
mi_0040e4ff:;
    /* 0040e4ff */
    a=f->r[7];b=0x00000015u;t=mi_alu(f,a,b,32,1);
    /* 0040e502 */
    a=0x00000017u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e504 */
    a=0x00576278u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e509 */
    f->r[1]=0x00f55bb0u;
    /* 0040e50e */
    if(mi_signed_less(f->flags))goto mi_0040e4df;
    /* 0040e510 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040e515u;f->boundary(0x0040e510u,0x0053dd90u,f);
    /* 0040e515 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e518 */
    if(!mi_signed_less(f->flags))goto mi_0040e73b;
mi_0040e51e:;
    /* 0040e51e */
    f->r[0]=pq_read_0040de80((0x00f5740cu),4u);
    /* 0040e523 */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 0040e526 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0040e528 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e529 */
    f->r[0]=f->r[6];
    /* 0040e52b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e52c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e52d */
    f->r[4]+=12;return;
mi_0040e530:;
    /* 0040e530 */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e533 */
    if(!mi_signed_less(f->flags))goto mi_0040e543;
    /* 0040e535 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e536 */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(0x00000001u);
    /* 0040e53c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e53d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0040e53f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e540 */
    f->r[4]+=12;return;
mi_0040e543:;
    /* 0040e543 */
    a=f->r[7];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 0040e546 */
    if(mi_signed_less(f->flags))goto mi_0040e70b;
    /* 0040e54c */
    a=f->r[7];b=0x00000014u;t=mi_alu(f,a,b,32,1);
    /* 0040e54f */
    if(!mi_signed_less(f->flags))goto mi_0040e57d;
    /* 0040e551 */
    a=0x00000017u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e553 */
    a=0x00576284u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e558 */
    f->r[1]=0x00f55bb0u;
mi_0040e55d:;
    /* 0040e55d */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040e562u;f->boundary(0x0040e55du,0x0053dd90u,f);
    /* 0040e562 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e565 */
    if(!mi_signed_less(f->flags))goto mi_0040ea3a;
    /* 0040e56b */
    f->r[0]=pq_read_0040de80((0x00f573f4u),4u);
    /* 0040e570 */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 0040e573 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0040e575 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e576 */
    f->r[0]=f->r[6];
    /* 0040e578 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e579 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e57a */
    f->r[4]+=12;return;
mi_0040e57d:;
    /* 0040e57d */
    a=f->r[7];b=0x00000015u;t=mi_alu(f,a,b,32,1);
    /* 0040e580 */
    a=0x00000017u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e582 */
    a=0x00576278u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e587 */
    f->r[1]=0x00f55bb0u;
    /* 0040e58c */
    if(mi_signed_less(f->flags))goto mi_0040e55d;
    /* 0040e58e */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040e593u;f->boundary(0x0040e58eu,0x0053dd90u,f);
    /* 0040e593 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e596 */
    if(!mi_signed_less(f->flags))goto mi_0040e9b1;
mi_0040e59c:;
    /* 0040e59c */
    f->r[0]=pq_read_0040de80((0x00f573f4u),4u);
    /* 0040e5a1 */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 0040e5a4 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0040e5a6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e5a7 */
    f->r[0]=f->r[6];
    /* 0040e5a9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e5aa */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e5ab */
    f->r[4]+=12;return;
mi_0040e5ae:;
    /* 0040e5ae */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e5b1 */
    if(!mi_signed_less(f->flags))goto mi_0040e5c1;
    /* 0040e5b3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e5b4 */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(0x00000001u);
    /* 0040e5ba */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e5bb */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0040e5bd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e5be */
    f->r[4]+=12;return;
mi_0040e5c1:;
    /* 0040e5c1 */
    a=f->r[7];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 0040e5c4 */
    if(mi_signed_less(f->flags))goto mi_0040e70b;
    /* 0040e5ca */
    a=f->r[7];b=0x00000014u;t=mi_alu(f,a,b,32,1);
    /* 0040e5cd */
    if(!mi_signed_less(f->flags))goto mi_0040e5dd;
    /* 0040e5cf */
    a=0x00000017u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e5d1 */
    a=0x00576284u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e5d6 */
    f->r[1]=0x00f55bb0u;
    /* 0040e5db */
    goto mi_0040e5ee;
mi_0040e5dd:;
    /* 0040e5dd */
    a=f->r[7];b=0x00000015u;t=mi_alu(f,a,b,32,1);
    /* 0040e5e0 */
    a=0x00000017u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e5e2 */
    a=0x00576278u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e5e7 */
    f->r[1]=0x00f55bb0u;
    /* 0040e5ec */
    if(!mi_signed_less(f->flags))goto mi_0040e60e;
mi_0040e5ee:;
    /* 0040e5ee */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040e5f3u;f->boundary(0x0040e5eeu,0x0053dd90u,f);
    /* 0040e5f3 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e5f6 */
    if(!mi_signed_less(f->flags))goto mi_0040ea3a;
mi_0040e5fc:;
    /* 0040e5fc */
    f->r[0]=pq_read_0040de80((0x00f573fcu),4u);
    /* 0040e601 */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 0040e604 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0040e606 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e607 */
    f->r[0]=f->r[6];
    /* 0040e609 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e60a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e60b */
    f->r[4]+=12;return;
mi_0040e60e:;
    /* 0040e60e */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040e613u;f->boundary(0x0040e60eu,0x0053dd90u,f);
    /* 0040e613 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e616 */
    if(!mi_signed_less(f->flags))goto mi_0040e9b1;
    /* 0040e61c */
    f->r[0]=pq_read_0040de80((0x00f573fcu),4u);
    /* 0040e621 */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 0040e624 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0040e626 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e627 */
    f->r[0]=f->r[6];
    /* 0040e629 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e62a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e62b */
    f->r[4]+=12;return;
mi_0040e62e:;
    /* 0040e62e */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e631 */
    if(!mi_signed_less(f->flags))goto mi_0040e641;
    /* 0040e633 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e634 */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(0x00000001u);
    /* 0040e63a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e63b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0040e63d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e63e */
    f->r[4]+=12;return;
mi_0040e641:;
    /* 0040e641 */
    a=f->r[7];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 0040e644 */
    if(mi_signed_less(f->flags))goto mi_0040e70b;
    /* 0040e64a */
    a=f->r[7];b=0x00000014u;t=mi_alu(f,a,b,32,1);
    /* 0040e64d */
    if(!mi_signed_less(f->flags))goto mi_0040e67a;
    /* 0040e64f */
    a=0x00000017u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e651 */
    a=0x00576284u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e656 */
    f->r[1]=0x00f55bb0u;
    /* 0040e65b */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040e660u;f->boundary(0x0040e65bu,0x0053dd90u,f);
    /* 0040e660 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e663 */
    if(!mi_signed_less(f->flags))goto mi_0040e4a0;
    /* 0040e669 */
    f->r[0]=pq_read_0040de80((0x00f56b54u),4u);
    /* 0040e66e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e66f */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(0x00000001u);
    /* 0040e675 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e676 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e677 */
    f->r[4]+=12;return;
mi_0040e67a:;
    /* 0040e67a */
    a=f->r[7];b=0x00000015u;t=mi_alu(f,a,b,32,1);
    /* 0040e67d */
    a=0x00000017u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e67f */
    a=0x00576278u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e684 */
    f->r[1]=0x00f55bb0u;
    /* 0040e689 */
    if(!mi_signed_less(f->flags))goto mi_0040e6aa;
    /* 0040e68b */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040e690u;f->boundary(0x0040e68bu,0x0053dd90u,f);
    /* 0040e690 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e693 */
    if(!mi_signed_less(f->flags))goto mi_0040e4a0;
    /* 0040e699 */
    f->r[0]=pq_read_0040de80((0x00f56b54u),4u);
    /* 0040e69e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e69f */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(0x00000001u);
    /* 0040e6a5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e6a6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e6a7 */
    f->r[4]+=12;return;
mi_0040e6aa:;
    /* 0040e6aa */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040e6afu;f->boundary(0x0040e6aau,0x0053dd90u,f);
    /* 0040e6af */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e6b2 */
    if(!mi_signed_less(f->flags))goto mi_0040e4a0;
    /* 0040e6b8 */
    f->r[0]=pq_read_0040de80((0x00f573ccu),4u);
    /* 0040e6bd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e6be */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e6bf */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e6c0 */
    f->r[4]+=12;return;
mi_0040e6c3:;
    /* 0040e6c3 */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e6c6 */
    if(!mi_signed_less(f->flags))goto mi_0040e6d6;
    /* 0040e6c8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e6c9 */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(0x00000001u);
    /* 0040e6cf */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e6d0 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0040e6d2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e6d3 */
    f->r[4]+=12;return;
mi_0040e6d6:;
    /* 0040e6d6 */
    a=f->r[7];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 0040e6d9 */
    if(!mi_signed_less(f->flags))goto mi_0040e6ec;
    /* 0040e6db */
    f->r[0]=pq_read_0040de80((0x00f56b54u),4u);
    /* 0040e6e0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e6e1 */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(0x00000001u);
    /* 0040e6e7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e6e8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e6e9 */
    f->r[4]+=12;return;
mi_0040e6ec:;
    /* 0040e6ec */
    a=f->r[7];b=0x00000014u;t=mi_alu(f,a,b,32,1);
    /* 0040e6ef */
    if(!mi_signed_less(f->flags))goto mi_0040e71c;
    /* 0040e6f1 */
    a=0x00000017u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e6f3 */
    a=0x00576284u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e6f8 */
    f->r[1]=0x00f55bb0u;
mi_0040e6fd:;
    /* 0040e6fd */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040e702u;f->boundary(0x0040e6fdu,0x0053dd90u,f);
    /* 0040e702 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e705 */
    if(!mi_signed_less(f->flags))goto mi_0040e51e;
mi_0040e70b:;
    /* 0040e70b */
    f->r[0]=pq_read_0040de80((0x00f56b54u),4u);
    /* 0040e710 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e711 */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(0x00000001u);
    /* 0040e717 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e718 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e719 */
    f->r[4]+=12;return;
mi_0040e71c:;
    /* 0040e71c */
    a=f->r[7];b=0x00000015u;t=mi_alu(f,a,b,32,1);
    /* 0040e71f */
    a=0x00000017u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e721 */
    a=0x00576278u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e726 */
    f->r[1]=0x00f55bb0u;
    /* 0040e72b */
    if(mi_signed_less(f->flags))goto mi_0040e6fd;
    /* 0040e72d */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040e732u;f->boundary(0x0040e72du,0x0053dd90u,f);
    /* 0040e732 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e735 */
    if(!mi_signed_less(f->flags))goto mi_0040e51e;
mi_0040e73b:;
    /* 0040e73b */
    f->r[0]=pq_read_0040de80((0x00f573ccu),4u);
    /* 0040e740 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e741 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e742 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e743 */
    f->r[4]+=12;return;
mi_0040e746:;
    /* 0040e746 */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e749 */
    if(!mi_signed_less(f->flags))goto mi_0040e759;
    /* 0040e74b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e74c */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(0x00000001u);
    /* 0040e752 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e753 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0040e755 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e756 */
    f->r[4]+=12;return;
mi_0040e759:;
    /* 0040e759 */
    a=f->r[7];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 0040e75c */
    if(!mi_signed_less(f->flags))goto mi_0040e76f;
    /* 0040e75e */
    f->r[0]=pq_read_0040de80((0x00f56b54u),4u);
    /* 0040e763 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e764 */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(0x00000001u);
    /* 0040e76a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e76b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e76c */
    f->r[4]+=12;return;
mi_0040e76f:;
    /* 0040e76f */
    a=f->r[7];b=0x00000014u;t=mi_alu(f,a,b,32,1);
    /* 0040e772 */
    a=0x00000017u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e774 */
    if(!mi_signed_less(f->flags))goto mi_0040e79f;
    /* 0040e776 */
    a=0x00576284u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e77b */
    f->r[1]=0x00f55bb0u;
    /* 0040e780 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040e785u;f->boundary(0x0040e780u,0x0053dd90u,f);
    /* 0040e785 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e788 */
    if(!mi_signed_less(f->flags))goto mi_0040eadd;
    /* 0040e78e */
    f->r[0]=pq_read_0040de80((0x00f56b54u),4u);
    /* 0040e793 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e794 */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(0x00000001u);
    /* 0040e79a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e79b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e79c */
    f->r[4]+=12;return;
mi_0040e79f:;
    /* 0040e79f */
    a=0x00576278u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e7a4 */
    f->r[1]=0x00f55bb0u;
    /* 0040e7a9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040e7aeu;f->boundary(0x0040e7a9u,0x0053dd90u,f);
    /* 0040e7ae */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e7b1 */
    if(mi_signed_less(f->flags))goto mi_0040eb15;
    /* 0040e7b7 */
    f->r[0]=pq_read_0040de80((0x00f5743cu),4u);
    /* 0040e7bc */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 0040e7bf */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0040e7c1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e7c2 */
    f->r[0]=f->r[6];
    /* 0040e7c4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e7c5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e7c6 */
    f->r[4]+=12;return;
mi_0040e7c9:;
    /* 0040e7c9 */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e7cc */
    if(!mi_signed_less(f->flags))goto mi_0040e7dc;
    /* 0040e7ce */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e7cf */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(0x00000001u);
    /* 0040e7d5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e7d6 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0040e7d8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e7d9 */
    f->r[4]+=12;return;
mi_0040e7dc:;
    /* 0040e7dc */
    a=f->r[7];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 0040e7df */
    if(!mi_signed_less(f->flags))goto mi_0040e7f2;
    /* 0040e7e1 */
    f->r[0]=pq_read_0040de80((0x00f56b54u),4u);
    /* 0040e7e6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e7e7 */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(0x00000001u);
    /* 0040e7ed */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e7ee */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e7ef */
    f->r[4]+=12;return;
mi_0040e7f2:;
    /* 0040e7f2 */
    a=f->r[7];b=0x00000014u;t=mi_alu(f,a,b,32,1);
    /* 0040e7f5 */
    a=0x00000017u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e7f7 */
    if(!mi_signed_less(f->flags))goto mi_0040e822;
    /* 0040e7f9 */
    a=0x00576284u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e7fe */
    f->r[1]=0x00f55bb0u;
    /* 0040e803 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040e808u;f->boundary(0x0040e803u,0x0053dd90u,f);
    /* 0040e808 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e80b */
    if(!mi_signed_less(f->flags))goto mi_0040ea66;
    /* 0040e811 */
    f->r[0]=pq_read_0040de80((0x00f56b54u),4u);
    /* 0040e816 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e817 */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(0x00000001u);
    /* 0040e81d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e81e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e81f */
    f->r[4]+=12;return;
mi_0040e822:;
    /* 0040e822 */
    a=0x00576278u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e827 */
    f->r[1]=0x00f55bb0u;
    /* 0040e82c */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040e831u;f->boundary(0x0040e82cu,0x0053dd90u,f);
    /* 0040e831 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e834 */
    if(mi_signed_less(f->flags))goto mi_0040eb15;
    /* 0040e83a */
    f->r[0]=pq_read_0040de80((0x00f57434u),4u);
    /* 0040e83f */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 0040e842 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0040e844 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e845 */
    f->r[0]=f->r[6];
    /* 0040e847 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e848 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e849 */
    f->r[4]+=12;return;
mi_0040e84c:;
    /* 0040e84c */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e84f */
    if(!mi_signed_less(f->flags))goto mi_0040e85f;
    /* 0040e851 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e852 */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(0x00000001u);
    /* 0040e858 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e859 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0040e85b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e85c */
    f->r[4]+=12;return;
mi_0040e85f:;
    /* 0040e85f */
    a=f->r[7];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 0040e862 */
    if(!mi_signed_less(f->flags))goto mi_0040e875;
    /* 0040e864 */
    f->r[0]=pq_read_0040de80((0x00f56b54u),4u);
    /* 0040e869 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e86a */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(0x00000001u);
    /* 0040e870 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e871 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e872 */
    f->r[4]+=12;return;
mi_0040e875:;
    /* 0040e875 */
    a=f->r[7];b=0x00000014u;t=mi_alu(f,a,b,32,1);
    /* 0040e878 */
    if(!mi_signed_less(f->flags))goto mi_0040e888;
    /* 0040e87a */
    a=0x00000017u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e87c */
    a=0x00576284u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e881 */
    f->r[1]=0x00f55bb0u;
    /* 0040e886 */
    goto mi_0040e899;
mi_0040e888:;
    /* 0040e888 */
    a=f->r[7];b=0x00000015u;t=mi_alu(f,a,b,32,1);
    /* 0040e88b */
    a=0x00000017u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e88d */
    a=0x00576278u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e892 */
    f->r[1]=0x00f55bb0u;
    /* 0040e897 */
    if(!mi_signed_less(f->flags))goto mi_0040e8b8;
mi_0040e899:;
    /* 0040e899 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040e89eu;f->boundary(0x0040e899u,0x0053dd90u,f);
    /* 0040e89e */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e8a1 */
    if(!mi_signed_less(f->flags))goto mi_0040e5fc;
    /* 0040e8a7 */
    f->r[0]=pq_read_0040de80((0x00f56b54u),4u);
    /* 0040e8ac */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e8ad */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(0x00000001u);
    /* 0040e8b3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e8b4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e8b5 */
    f->r[4]+=12;return;
mi_0040e8b8:;
    /* 0040e8b8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040e8bdu;f->boundary(0x0040e8b8u,0x0053dd90u,f);
    /* 0040e8bd */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e8c0 */
    if(!mi_signed_less(f->flags))goto mi_0040e5fc;
    /* 0040e8c6 */
    f->r[0]=pq_read_0040de80((0x00f573e4u),4u);
    /* 0040e8cb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e8cc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e8cd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e8ce */
    f->r[4]+=12;return;
mi_0040e8d1:;
    /* 0040e8d1 */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e8d4 */
    if(!mi_signed_less(f->flags))goto mi_0040e8e4;
    /* 0040e8d6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e8d7 */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(0x00000001u);
    /* 0040e8dd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e8de */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0040e8e0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e8e1 */
    f->r[4]+=12;return;
mi_0040e8e4:;
    /* 0040e8e4 */
    a=f->r[7];b=0x00000015u;t=mi_alu(f,a,b,32,1);
    /* 0040e8e7 */
    if(!mi_signed_less(f->flags))goto mi_0040e8fa;
    /* 0040e8e9 */
    f->r[0]=pq_read_0040de80((0x00f56b54u),4u);
    /* 0040e8ee */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e8ef */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(0x00000001u);
    /* 0040e8f5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e8f6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e8f7 */
    f->r[4]+=12;return;
mi_0040e8fa:;
    /* 0040e8fa */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e8fd */
    if(!mi_signed_less(f->flags))goto mi_0040eb15;
    /* 0040e903 */
    f->r[0]=pq_read_0040de80((0x00f573ecu),4u);
    /* 0040e908 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e909 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e90a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e90b */
    f->r[4]+=12;return;
mi_0040e90e:;
    /* 0040e90e */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e911 */
    if(!mi_signed_less(f->flags))goto mi_0040e921;
    /* 0040e913 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e914 */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(0x00000001u);
    /* 0040e91a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e91b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0040e91d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e91e */
    f->r[4]+=12;return;
mi_0040e921:;
    /* 0040e921 */
    a=f->r[7];b=0x00000015u;t=mi_alu(f,a,b,32,1);
    /* 0040e924 */
    if(!mi_signed_less(f->flags))goto mi_0040e8fa;
    /* 0040e926 */
    f->r[0]=pq_read_0040de80((0x00f56b54u),4u);
    /* 0040e92b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e92c */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(0x00000001u);
    /* 0040e932 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e933 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e934 */
    f->r[4]+=12;return;
mi_0040e937:;
    /* 0040e937 */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e93a */
    if(!mi_signed_less(f->flags))goto mi_0040e94a;
    /* 0040e93c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e93d */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(0x00000001u);
    /* 0040e943 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e944 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0040e946 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e947 */
    f->r[4]+=12;return;
mi_0040e94a:;
    /* 0040e94a */
    a=f->r[7];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 0040e94d */
    if(!mi_signed_less(f->flags))goto mi_0040e960;
    /* 0040e94f */
    f->r[0]=pq_read_0040de80((0x00f56b54u),4u);
    /* 0040e954 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e955 */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(0x00000001u);
    /* 0040e95b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e95c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e95d */
    f->r[4]+=12;return;
mi_0040e960:;
    /* 0040e960 */
    a=f->r[7];b=0x00000014u;t=mi_alu(f,a,b,32,1);
    /* 0040e963 */
    if(!mi_signed_less(f->flags))goto mi_0040e973;
    /* 0040e965 */
    a=0x00000017u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e967 */
    a=0x00576284u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e96c */
    f->r[1]=0x00f55bb0u;
    /* 0040e971 */
    goto mi_0040e984;
mi_0040e973:;
    /* 0040e973 */
    a=f->r[7];b=0x00000015u;t=mi_alu(f,a,b,32,1);
    /* 0040e976 */
    a=0x00000017u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e978 */
    a=0x00576278u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040e97d */
    f->r[1]=0x00f55bb0u;
    /* 0040e982 */
    if(!mi_signed_less(f->flags))goto mi_0040e9a3;
mi_0040e984:;
    /* 0040e984 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040e989u;f->boundary(0x0040e984u,0x0053dd90u,f);
    /* 0040e989 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e98c */
    if(!mi_signed_less(f->flags))goto mi_0040e59c;
    /* 0040e992 */
    f->r[0]=pq_read_0040de80((0x00f56b54u),4u);
    /* 0040e997 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e998 */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(0x00000001u);
    /* 0040e99e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e99f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e9a0 */
    f->r[4]+=12;return;
mi_0040e9a3:;
    /* 0040e9a3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040e9a8u;f->boundary(0x0040e9a3u,0x0053dd90u,f);
    /* 0040e9a8 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e9ab */
    if(!mi_signed_less(f->flags))goto mi_0040e59c;
mi_0040e9b1:;
    /* 0040e9b1 */
    f->r[0]=pq_read_0040de80((0x00f573e4u),4u);
    /* 0040e9b6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e9b7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e9b8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e9b9 */
    f->r[4]+=12;return;
mi_0040e9bc:;
    /* 0040e9bc */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e9bf */
    if(!mi_signed_less(f->flags))goto mi_0040e9cf;
    /* 0040e9c1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e9c2 */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(0x00000001u);
    /* 0040e9c8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e9c9 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0040e9cb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e9cc */
    f->r[4]+=12;return;
mi_0040e9cf:;
    /* 0040e9cf */
    a=f->r[7];b=0x00000015u;t=mi_alu(f,a,b,32,1);
    /* 0040e9d2 */
    if(!mi_signed_less(f->flags))goto mi_0040e9e5;
    /* 0040e9d4 */
    f->r[0]=pq_read_0040de80((0x00f56b54u),4u);
    /* 0040e9d9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e9da */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(0x00000001u);
    /* 0040e9e0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e9e1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e9e2 */
    f->r[4]+=12;return;
mi_0040e9e5:;
    /* 0040e9e5 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e9e8 */
    if(mi_signed_less(f->flags))goto mi_0040eb15;
    /* 0040e9ee */
    f->r[0]=pq_read_0040de80((0x00f573ecu),4u);
    /* 0040e9f3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e9f4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040e9f5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040e9f6 */
    f->r[4]+=12;return;
mi_0040e9f9:;
    /* 0040e9f9 */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040e9fc */
    if(!mi_signed_less(f->flags))goto mi_0040ea0c;
    /* 0040e9fe */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040e9ff */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(0x00000001u);
    /* 0040ea05 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040ea06 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0040ea08 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040ea09 */
    f->r[4]+=12;return;
mi_0040ea0c:;
    /* 0040ea0c */
    a=f->r[7];b=0x00000015u;t=mi_alu(f,a,b,32,1);
    /* 0040ea0f */
    if(!mi_signed_less(f->flags))goto mi_0040e9e5;
    /* 0040ea11 */
    f->r[0]=pq_read_0040de80((0x00f56b54u),4u);
    /* 0040ea16 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040ea17 */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(0x00000001u);
    /* 0040ea1d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040ea1e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040ea1f */
    f->r[4]+=12;return;
mi_0040ea22:;
    /* 0040ea22 */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040ea25 */
    if(!mi_signed_less(f->flags))goto mi_0040ea35;
    /* 0040ea27 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040ea28 */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(0x00000001u);
    /* 0040ea2e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040ea2f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0040ea31 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040ea32 */
    f->r[4]+=12;return;
mi_0040ea35:;
    /* 0040ea35 */
    a=f->r[7];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 0040ea38 */
    if(!mi_signed_less(f->flags))goto mi_0040ea4b;
mi_0040ea3a:;
    /* 0040ea3a */
    f->r[0]=pq_read_0040de80((0x00f56b54u),4u);
    /* 0040ea3f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040ea40 */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(0x00000001u);
    /* 0040ea46 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040ea47 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040ea48 */
    f->r[4]+=12;return;
mi_0040ea4b:;
    /* 0040ea4b */
    a=f->r[7];b=0x00000014u;t=mi_alu(f,a,b,32,1);
    /* 0040ea4e */
    a=0x00000017u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040ea50 */
    if(!mi_signed_less(f->flags))goto mi_0040ea78;
    /* 0040ea52 */
    a=0x00576284u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040ea57 */
    f->r[1]=0x00f55bb0u;
    /* 0040ea5c */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040ea61u;f->boundary(0x0040ea5cu,0x0053dd90u,f);
    /* 0040ea61 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040ea64 */
    if(!mi_signed_less(f->flags))goto mi_0040ea3a;
mi_0040ea66:;
    /* 0040ea66 */
    f->r[0]=pq_read_0040de80((0x00f57434u),4u);
    /* 0040ea6b */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 0040ea6e */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0040ea70 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040ea71 */
    f->r[0]=f->r[6];
    /* 0040ea73 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040ea74 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040ea75 */
    f->r[4]+=12;return;
mi_0040ea78:;
    /* 0040ea78 */
    a=0x00576278u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040ea7d */
    f->r[1]=0x00f55bb0u;
    /* 0040ea82 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040ea87u;f->boundary(0x0040ea82u,0x0053dd90u,f);
    /* 0040ea87 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040ea8a */
    if(!mi_signed_less(f->flags))goto mi_0040eb15;
    /* 0040ea90 */
    f->r[0]=pq_read_0040de80((0x00f57434u),4u);
    /* 0040ea95 */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 0040ea98 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0040ea9a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040ea9b */
    f->r[0]=f->r[6];
    /* 0040ea9d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040ea9e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040ea9f */
    f->r[4]+=12;return;
mi_0040eaa2:;
    /* 0040eaa2 */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040eaa5 */
    if(!mi_signed_less(f->flags))goto mi_0040eab5;
    /* 0040eaa7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040eaa8 */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(0x00000001u);
    /* 0040eaae */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040eaaf */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0040eab1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040eab2 */
    f->r[4]+=12;return;
mi_0040eab5:;
    /* 0040eab5 */
    a=f->r[7];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 0040eab8 */
    if(mi_signed_less(f->flags))goto mi_0040e70b;
    /* 0040eabe */
    a=f->r[7];b=0x00000014u;t=mi_alu(f,a,b,32,1);
    /* 0040eac1 */
    a=0x00000017u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040eac3 */
    if(!mi_signed_less(f->flags))goto mi_0040eaef;
    /* 0040eac5 */
    a=0x00576284u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040eaca */
    f->r[1]=0x00f55bb0u;
    /* 0040eacf */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040ead4u;f->boundary(0x0040eacfu,0x0053dd90u,f);
    /* 0040ead4 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040ead7 */
    if(!mi_signed_less(f->flags))goto mi_0040ea3a;
mi_0040eadd:;
    /* 0040eadd */
    f->r[0]=pq_read_0040de80((0x00f5743cu),4u);
    /* 0040eae2 */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 0040eae5 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0040eae7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040eae8 */
    f->r[0]=f->r[6];
    /* 0040eaea */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040eaeb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040eaec */
    f->r[4]+=12;return;
mi_0040eaef:;
    /* 0040eaef */
    a=0x00576278u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040eaf4 */
    f->r[1]=0x00f55bb0u;
    /* 0040eaf9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040eafeu;f->boundary(0x0040eaf9u,0x0053dd90u,f);
    /* 0040eafe */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040eb01 */
    if(!mi_signed_less(f->flags))goto mi_0040eb15;
    /* 0040eb03 */
    f->r[0]=pq_read_0040de80((0x00f5743cu),4u);
    /* 0040eb08 */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 0040eb0b */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0040eb0d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040eb0e */
    f->r[0]=f->r[6];
    /* 0040eb10 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040eb11 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040eb12 */
    f->r[4]+=12;return;
mi_0040eb15:;
    /* 0040eb15 */
    f->r[0]=pq_read_0040de80((0x00f573dcu),4u);
mi_0040eb1a:;
    /* 0040eb1a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040eb1b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040eb1c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040eb1d */
    f->r[4]+=12;return;
}

/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */




void restore_placement_query_C_00417310(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00417310 */
    a=f->r[4];b=0x00000090u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 00417316 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00417317 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00417318 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00417319 */
    f->r[6]=0x00000006u;
    /* 0041731e */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 00417322 */
    f->r[0]=0x00000008u;
    /* 00417327 */
    MI_U32((0x00000050u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 0041732b */
    MI_U32((0x0000005cu+f->r[4]))=(uint32_t)(f->r[6]);
    /* 0041732f */
    MI_U32((0x00000078u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 00417333 */
    f->r[6]=MI_U32((0x000000a0u+f->r[4]));
    /* 0041733a */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0041733e */
    MI_U32((0x00000038u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00417342 */
    MI_U32((0x00000054u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00417346 */
    MI_U32((0x00000090u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0041734d */
    f->r[0]=f->r[6];
    /* 0041734f */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00417350 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00417353 */
    f->r[7]=0x00000005u;
    /* 00417358 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0041735a */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 0041735e */
    MI_U32((0x00000038u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 00417362 */
    MI_U32((0x00000064u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 00417366 */
    MI_U32((0x00000098u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 0041736d */
    f->r[2]=0x00000007u;
    /* 00417372 */
    f->r[7]=(0x007999eau+f->r[0]*8u);
    /* 00417379 */
    f->r[0]=MI_U32((0x00bca2e0u));
    /* 0041737e */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00417382 */
    MI_U32((0x00000048u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00417386 */
    MI_U32((0x0000005cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0041738a */
    MI_U32((0x00000088u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00417391 */
    f->r[5]=0x00000003u;
    /* 00417396 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 00417397 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00417399 */
    f->r[3]=0x00000004u;
    /* 0041739e */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004173a0 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 004173a8 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(0x00000001u);
    /* 004173b0 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 004173b3 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(0x00000002u);
    /* 004173bb */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[7]));
    /* 004173be */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004173c0 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[5]);
    /* 004173c4 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 004173c8 */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(0x00000002u);
    /* 004173d0 */
    MI_U32((0x00000040u+f->r[4]))=(uint32_t)(0x00000001u);
    /* 004173d8 */
    MI_U32((0x00000044u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 004173dc */
    MI_U32((0x0000004cu+f->r[4]))=(uint32_t)(0x00000000u);
    /* 004173e4 */
    MI_U32((0x00000050u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 004173e8 */
    MI_U32((0x00000068u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 004173ec */
    MI_U32((0x0000006cu+f->r[4]))=(uint32_t)(f->r[5]);
    /* 004173f0 */
    MI_U32((0x00000070u+f->r[4]))=(uint32_t)(0x00000002u);
    /* 004173f8 */
    MI_U32((0x00000074u+f->r[4]))=(uint32_t)(0x00000001u);
    /* 00417400 */
    MI_U32((0x00000078u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 00417408 */
    MI_U32((0x00000080u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 0041740f */
    MI_U32((0x00000084u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 0041741a */
    MI_U32((0x0000008cu+f->r[4]))=(uint32_t)(f->r[3]);
    /* 00417421 */
    MI_U32((0x00000090u+f->r[4]))=(uint32_t)(0x00000001u);
    /* 0041742c */
    MI_U32((0x0000009cu+f->r[4]))=(uint32_t)(0x00000002u);
    /* 00417437 */
    f->r[3]=MI_U32((0x00000010u+f->r[4]+f->r[2]*4u));
    /* 0041743b */
    f->r[2]=MI_U32((0x000000a8u+f->r[4]));
    /* 00417442 */
    a=0x00000012u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00417444 */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(0x00000000u);
    /* 0041744a */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[7]));
    /* 0041744d */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 0041744f */
    f->r[2]=f->r[6];
    /* 00417451 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 00417454 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00417456 */
    f->r[0]=MI_U32((0x00799a30u+f->r[2]*8u));
    /* 0041745d */
    f->r[1]=MI_U32((0x00000220u+f->r[1]+f->r[0]*4u));
    /* 00417464 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00417465 */
    f->r[1]=0x00f55bb0u;
    /* 0041746a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0041746fu;f->boundary(0x0041746au,0x0053dd90u,f);
    /* 0041746f */
    f->r[2]=MI_U32((0x00f57394u));
    /* 00417475 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00417476 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00417477 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00417478 */
    f->r[0]=(0x00000000u+f->r[3]+f->r[2]*1u);
    /* 0041747b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0041747c */
    a=f->r[4];b=0x00000090u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00417482 */
    f->r[4]+=12;return;
}

/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */

#include <stdlib.h>



static const uint8_t mi_rom_005459d4[]={0xf5,0x58,0x54,0x00,0x7e,0x58,0x54,0x00,0xce,0x58,0x54,0x00,0x1c,0x59,0x54,0x00,0x45,0x59,0x54,0x00,0x6c,0x59,0x54,0x00,0x93,0x59,0x54,0x00,0xbc,0x59,0x54,0x00,0x50,0x58,0x54,0x00,0x60,0x58,0x54,0x00,0x6e,0x58,0x54,0x00,0xa5,0x58,0x54,0x00,0xca,0x59,0x54,0x00};
static const uint8_t mi_rom_00545a08[]={0x00,0x01,0x02,0x03,0x0c,0x0c,0x04,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x05,0x06,0x07,0x08,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x09,0x0a,0x0b};
static __inline uint32_t pq_read_00545760(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x005459d4u&&width<=52u&&(offset=address-0x005459d4u)<=52u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_005459d4[offset+k]<<(k*8u);return result;
 }
 if(address>=0x00545a08u&&width<=30u&&(offset=address-0x00545a08u)<=30u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00545a08[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}

void restore_placement_query_C_00545760(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00545760 */
    f->r[0]=pq_read_00545760((0x00000004u+f->r[4]),4u);
    /* 00545764 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00545765 */
    f->r[2]=pq_read_00545760((0x0000000cu+f->r[4]),4u);
    /* 00545769 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0054576a */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0054576b */
    f->r[6]=f->r[0];
    /* 0054576d */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 00545770 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00545772 */
    f->r[5]=f->r[1];
    /* 00545774 */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 00545777 */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(0x00000000u);
    /* 0054577d */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0054577e */
    f->r[1]=pq_read_00545760((0x00799a38u+f->r[6]),4u);
    /* 00545784 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00545786 */
    if(!(f->flags&MI_ZF))goto mi_005457ba;
    /* 00545788 */
    f->r[0]=pq_read_00545760((0x00799a40u+f->r[6]),4u);
    /* 0054578e */
    a=f->r[0];b=0x00000012u;t=mi_alu(f,a,b,32,1);
    /* 00545791 */
    if((f->flags&MI_ZF))goto mi_005457ba;
    /* 00545793 */
    a=f->r[0];b=0x0000001bu;t=mi_alu(f,a,b,32,1);
    /* 00545796 */
    if((f->flags&MI_ZF))goto mi_005457ba;
    /* 00545798 */
    a=f->r[0];b=0x0000001cu;t=mi_alu(f,a,b,32,1);
    /* 0054579b */
    if((f->flags&MI_ZF))goto mi_005457ba;
    /* 0054579d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0054579f */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(0x00000001u);
    /* 005457a5 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((pq_read_00545760((0x00799a74u+f->r[6]),1u))&0xffu)<<0);
    /* 005457ab */
    f->r[1]=pq_read_00545760((0x00f574dcu),4u);
    /* 005457b1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 005457b2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 005457b3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 005457b4 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 005457b6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 005457b7 */
    f->r[4]+=12;return;
mi_005457ba:;
    /* 005457ba */
    f->r[0]=pq_read_00545760((0x00bca2e0u),4u);
    /* 005457bf */
    f->r[7]=pq_read_00545760((0x00799a68u+f->r[6]),4u);
    /* 005457c5 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 005457c6 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 005457c8 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 005457ca */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 005457cc */
    if(!(f->flags&MI_SF))goto mi_005457d1;
    /* 005457ce */
    a=f->r[7];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
mi_005457d1:;
    /* 005457d1 */
    a=f->r[1];b=0x00000003u;t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 005457d4 */
    if((f->flags&MI_ZF))goto mi_005457ee;
    /* 005457d6 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 005457d7 */
    if((f->flags&MI_ZF))goto mi_005457e7;
    /* 005457d9 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 005457da */
    if((f->flags&MI_ZF))goto mi_005457e0;
    /* 005457dc */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 005457de */
    goto mi_005457f3;
mi_005457e0:;
    /* 005457e0 */
    f->r[3]=0x0000000cu;
    /* 005457e5 */
    goto mi_005457f3;
mi_005457e7:;
    /* 005457e7 */
    f->r[3]=0x00000008u;
    /* 005457ec */
    goto mi_005457f3;
mi_005457ee:;
    /* 005457ee */
    f->r[3]=0x00000004u;
mi_005457f3:;
    /* 005457f3 */
    f->r[1]=f->r[5];
    /* 005457f5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x005457fau;f->boundary(0x005457f5u,0x00545fb0u,f);
    /* 005457fa */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 005457fc */
    if((f->flags&MI_ZF))goto mi_0054581b;
    /* 005457fe */
    f->r[0]=(f->r[0]&0xffffff00u)|(((pq_read_00545760((0x00799a6cu+f->r[6]),1u))&0xffu)<<0);
    /* 00545804 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00545806 */
    if((f->flags&MI_ZF))goto mi_0054581b;
    /* 00545808 */
    f->r[0]=pq_read_00545760((0x00000014u+f->r[4]),4u);
    /* 0054580c */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0054580e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0054580f */
    f->r[1]=f->r[5];
    /* 00545811 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00545816u;f->boundary(0x00545811u,0x00545740u,f);
    /* 00545816 */
    a=f->r[0];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 00545819 */
    if((f->flags&MI_ZF))goto mi_0054582f;
mi_0054581b:;
    /* 0054581b */
    a=pq_read_00545760((0x007999eeu+f->r[6]),2u);b=0xffffffffu;t=mi_alu(f,a,b,16,1);
    /* 00545823 */
    if(!(f->flags&MI_ZF))goto mi_00545832;
    /* 00545825 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((pq_read_00545760((0x00799a6cu+f->r[6]),1u))&0xffu)<<0);
    /* 0054582b */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0054582d */
    if((f->flags&MI_ZF))goto mi_00545832;
mi_0054582f:;
    /* 0054582f */
    a=f->r[3];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
mi_00545832:;
    /* 00545832 */
    f->r[6]=pq_read_00545760((0x00799a40u+f->r[6]),4u);
    /* 00545838 */
    a=f->r[6];b=0x0000001du;t=mi_alu(f,a,b,32,1);
    /* 0054583b */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_005459ca;
    /* 00545841 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 00545843 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((pq_read_00545760((0x00545a08u+f->r[6]),1u))&0xffu)<<0);
    /* 00545849 */
    switch(pq_read_00545760((0x005459d4u+f->r[1]*4u),4u)){case 0x00545850u:goto mi_00545850;case 0x00545860u:goto mi_00545860;case 0x0054586eu:goto mi_0054586e;case 0x0054587eu:goto mi_0054587e;case 0x005458a5u:goto mi_005458a5;case 0x005458ceu:goto mi_005458ce;case 0x005458f5u:goto mi_005458f5;case 0x0054591cu:goto mi_0054591c;case 0x00545945u:goto mi_00545945;case 0x0054596cu:goto mi_0054596c;case 0x00545993u:goto mi_00545993;case 0x005459bcu:goto mi_005459bc;case 0x005459cau:goto mi_005459ca;default:abort();}
mi_00545850:;
    /* 00545850 */
    f->r[2]=pq_read_00545760((0x00f574ecu),4u);
    /* 00545856 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[7]*1u);
    /* 00545859 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0054585a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0054585b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0054585c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0054585d */
    f->r[4]+=12;return;
mi_00545860:;
    /* 00545860 */
    f->r[0]=pq_read_00545760((0x00f574f4u),4u);
    /* 00545865 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00545867 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00545868 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00545869 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0054586a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0054586b */
    f->r[4]+=12;return;
mi_0054586e:;
    /* 0054586e */
    f->r[1]=pq_read_00545760((0x00f574fcu),4u);
    /* 00545874 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[7]*1u);
    /* 00545877 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00545878 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00545879 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0054587a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0054587b */
    f->r[4]+=12;return;
mi_0054587e:;
    /* 0054587e */
    f->r[0]=pq_read_00545760((0x00000054u+f->r[5]),4u);
    /* 00545881 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00545883 */
    if((f->flags&MI_ZF))goto mi_00545895;
    /* 00545885 */
    f->r[2]=pq_read_00545760((0x00f57484u),4u);
    /* 0054588b */
    f->r[0]=(0x00000000u+f->r[2]+f->r[7]*1u);
    /* 0054588e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0054588f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00545890 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00545891 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00545892 */
    f->r[4]+=12;return;
mi_00545895:;
    /* 00545895 */
    f->r[0]=pq_read_00545760((0x00f5747cu),4u);
    /* 0054589a */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0054589c */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0054589e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0054589f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 005458a0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 005458a1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 005458a2 */
    f->r[4]+=12;return;
mi_005458a5:;
    /* 005458a5 */
    f->r[0]=pq_read_00545760((0x00000054u+f->r[5]),4u);
    /* 005458a8 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 005458aa */
    if((f->flags&MI_ZF))goto mi_005458bc;
    /* 005458ac */
    f->r[1]=pq_read_00545760((0x00f5750cu),4u);
    /* 005458b2 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[7]*1u);
    /* 005458b5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 005458b6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 005458b7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 005458b8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 005458b9 */
    f->r[4]+=12;return;
mi_005458bc:;
    /* 005458bc */
    f->r[2]=pq_read_00545760((0x00f57504u),4u);
    /* 005458c2 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[3]*1u);
    /* 005458c5 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 005458c7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 005458c8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 005458c9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 005458ca */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 005458cb */
    f->r[4]+=12;return;
mi_005458ce:;
    /* 005458ce */
    f->r[0]=pq_read_00545760((0x00000054u+f->r[5]),4u);
    /* 005458d1 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 005458d3 */
    if((f->flags&MI_ZF))goto mi_005458e3;
    /* 005458d5 */
    f->r[0]=pq_read_00545760((0x00f57494u),4u);
    /* 005458da */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 005458dc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 005458dd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 005458de */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 005458df */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 005458e0 */
    f->r[4]+=12;return;
mi_005458e3:;
    /* 005458e3 */
    f->r[1]=pq_read_00545760((0x00f5748cu),4u);
    /* 005458e9 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[3]*1u);
    /* 005458ec */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 005458ee */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 005458ef */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 005458f0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 005458f1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 005458f2 */
    f->r[4]+=12;return;
mi_005458f5:;
    /* 005458f5 */
    f->r[0]=pq_read_00545760((0x00000054u+f->r[5]),4u);
    /* 005458f8 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 005458fa */
    if((f->flags&MI_ZF))goto mi_0054590c;
    /* 005458fc */
    f->r[2]=pq_read_00545760((0x00f57474u),4u);
    /* 00545902 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[7]*1u);
    /* 00545905 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00545906 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00545907 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00545908 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00545909 */
    f->r[4]+=12;return;
mi_0054590c:;
    /* 0054590c */
    f->r[0]=pq_read_00545760((0x00f5746cu),4u);
    /* 00545911 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00545913 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00545915 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00545916 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00545917 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00545918 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00545919 */
    f->r[4]+=12;return;
mi_0054591c:;
    /* 0054591c */
    f->r[0]=pq_read_00545760((0x00000054u+f->r[5]),4u);
    /* 0054591f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00545921 */
    if((f->flags&MI_ZF))goto mi_00545933;
    /* 00545923 */
    f->r[1]=pq_read_00545760((0x00f574a4u),4u);
    /* 00545929 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[7]*1u);
    /* 0054592c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0054592d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0054592e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0054592f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00545930 */
    f->r[4]+=12;return;
mi_00545933:;
    /* 00545933 */
    f->r[2]=pq_read_00545760((0x00f5749cu),4u);
    /* 00545939 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[3]*1u);
    /* 0054593c */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0054593e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0054593f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00545940 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00545941 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00545942 */
    f->r[4]+=12;return;
mi_00545945:;
    /* 00545945 */
    f->r[0]=pq_read_00545760((0x00000054u+f->r[5]),4u);
    /* 00545948 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0054594a */
    if((f->flags&MI_ZF))goto mi_0054595a;
    /* 0054594c */
    f->r[0]=pq_read_00545760((0x00f574b4u),4u);
    /* 00545951 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00545953 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00545954 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00545955 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00545956 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00545957 */
    f->r[4]+=12;return;
mi_0054595a:;
    /* 0054595a */
    f->r[1]=pq_read_00545760((0x00f574acu),4u);
    /* 00545960 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[3]*1u);
    /* 00545963 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00545965 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00545966 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00545967 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00545968 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00545969 */
    f->r[4]+=12;return;
mi_0054596c:;
    /* 0054596c */
    f->r[0]=pq_read_00545760((0x00000054u+f->r[5]),4u);
    /* 0054596f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00545971 */
    if((f->flags&MI_ZF))goto mi_00545983;
    /* 00545973 */
    f->r[2]=pq_read_00545760((0x00f574c4u),4u);
    /* 00545979 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[7]*1u);
    /* 0054597c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0054597d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0054597e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0054597f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00545980 */
    f->r[4]+=12;return;
mi_00545983:;
    /* 00545983 */
    f->r[0]=pq_read_00545760((0x00f574bcu),4u);
    /* 00545988 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0054598a */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0054598c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0054598d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0054598e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0054598f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00545990 */
    f->r[4]+=12;return;
mi_00545993:;
    /* 00545993 */
    f->r[0]=pq_read_00545760((0x00000054u+f->r[5]),4u);
    /* 00545996 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00545998 */
    if((f->flags&MI_ZF))goto mi_005459aa;
    /* 0054599a */
    f->r[1]=pq_read_00545760((0x00f574d4u),4u);
    /* 005459a0 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[7]*1u);
    /* 005459a3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 005459a4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 005459a5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 005459a6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 005459a7 */
    f->r[4]+=12;return;
mi_005459aa:;
    /* 005459aa */
    f->r[2]=pq_read_00545760((0x00f574ccu),4u);
    /* 005459b0 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[3]*1u);
    /* 005459b3 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 005459b5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 005459b6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 005459b7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 005459b8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 005459b9 */
    f->r[4]+=12;return;
mi_005459bc:;
    /* 005459bc */
    f->r[0]=pq_read_00545760((0x00f574dcu),4u);
    /* 005459c1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 005459c2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 005459c3 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 005459c5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 005459c6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 005459c7 */
    f->r[4]+=12;return;
mi_005459ca:;
    /* 005459ca */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 005459cb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 005459cc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 005459cd */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 005459cf */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 005459d0 */
    f->r[4]+=12;return;
}

/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */

#include <stdlib.h>



static const uint8_t mi_rom_00558ea8[]={0xa3,0x8d,0x55,0x00,0x2c,0x8d,0x55,0x00,0x7a,0x8d,0x55,0x00,0xca,0x8d,0x55,0x00,0xf1,0x8d,0x55,0x00,0x91,0x8e,0x55,0x00,0x1c,0x8d,0x55,0x00,0x1a,0x8e,0x55,0x00,0x41,0x8e,0x55,0x00,0x68,0x8e,0x55,0x00,0xfe,0x8c,0x55,0x00,0x0c,0x8d,0x55,0x00,0x53,0x8d,0x55,0x00,0x9d,0x8e,0x55,0x00};
static const uint8_t mi_rom_00558ee0[]={0x00,0x01,0x02,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x03,0x04,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x05,0x06,0x07,0x08,0x09,0x0d,0x0d,0x0d,0x0d,0x0d,0x0a,0x0b,0x0c};
static __inline uint32_t pq_read_00558c00(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x00558ea8u&&width<=56u&&(offset=address-0x00558ea8u)<=56u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00558ea8[offset+k]<<(k*8u);return result;
 }
 if(address>=0x00558ee0u&&width<=30u&&(offset=address-0x00558ee0u)<=30u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00558ee0[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}

void restore_placement_query_C_00558c00(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00558c00 */
    f->r[0]=pq_read_00558c00((0x00000004u+f->r[4]),4u);
    /* 00558c04 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00558c05 */
    f->r[2]=pq_read_00558c00((0x0000000cu+f->r[4]),4u);
    /* 00558c09 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00558c0a */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00558c0b */
    f->r[6]=f->r[0];
    /* 00558c0d */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 00558c10 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00558c12 */
    f->r[5]=f->r[1];
    /* 00558c14 */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 00558c17 */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(0x00000000u);
    /* 00558c1d */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00558c1e */
    f->r[1]=pq_read_00558c00((0x00799a38u+f->r[6]),4u);
    /* 00558c24 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00558c26 */
    if(!(f->flags&MI_ZF))goto mi_00558c5a;
    /* 00558c28 */
    f->r[0]=pq_read_00558c00((0x00799a40u+f->r[6]),4u);
    /* 00558c2e */
    a=f->r[0];b=0x00000012u;t=mi_alu(f,a,b,32,1);
    /* 00558c31 */
    if((f->flags&MI_ZF))goto mi_00558c5a;
    /* 00558c33 */
    a=f->r[0];b=0x0000001bu;t=mi_alu(f,a,b,32,1);
    /* 00558c36 */
    if((f->flags&MI_ZF))goto mi_00558c5a;
    /* 00558c38 */
    a=f->r[0];b=0x0000001cu;t=mi_alu(f,a,b,32,1);
    /* 00558c3b */
    if((f->flags&MI_ZF))goto mi_00558c5a;
    /* 00558c3d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00558c3f */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(0x00000001u);
    /* 00558c45 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((pq_read_00558c00((0x00799a74u+f->r[6]),1u))&0xffu)<<0);
    /* 00558c4b */
    f->r[1]=pq_read_00558c00((0x00f574dcu),4u);
    /* 00558c51 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00558c52 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00558c53 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00558c54 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00558c56 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00558c57 */
    f->r[4]+=12;return;
mi_00558c5a:;
    /* 00558c5a */
    f->r[0]=pq_read_00558c00((0x00bca2e0u),4u);
    /* 00558c5f */
    f->r[7]=pq_read_00558c00((0x00799a68u+f->r[6]),4u);
    /* 00558c65 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 00558c66 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00558c68 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 00558c6a */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 00558c6c */
    if(!(f->flags&MI_SF))goto mi_00558c71;
    /* 00558c6e */
    a=f->r[7];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
mi_00558c71:;
    /* 00558c71 */
    a=f->r[1];b=0x00000003u;t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00558c74 */
    if((f->flags&MI_ZF))goto mi_00558c8e;
    /* 00558c76 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00558c77 */
    if((f->flags&MI_ZF))goto mi_00558c87;
    /* 00558c79 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00558c7a */
    if((f->flags&MI_ZF))goto mi_00558c80;
    /* 00558c7c */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 00558c7e */
    goto mi_00558c93;
mi_00558c80:;
    /* 00558c80 */
    f->r[3]=0x0000000cu;
    /* 00558c85 */
    goto mi_00558c93;
mi_00558c87:;
    /* 00558c87 */
    f->r[3]=0x00000008u;
    /* 00558c8c */
    goto mi_00558c93;
mi_00558c8e:;
    /* 00558c8e */
    f->r[3]=0x00000004u;
mi_00558c93:;
    /* 00558c93 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)pq_read_00558c00((0x007999eau+f->r[6]),2u);
    /* 00558c9a */
    a=f->r[0];b=pq_read_00558c00((0x00000050u+f->r[5]),4u);t=mi_alu(f,a,b,32,1);
    /* 00558c9d */
    if(!(f->flags&MI_ZF))goto mi_00558ca1;
    /* 00558c9f */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
mi_00558ca1:;
    /* 00558ca1 */
    f->r[1]=f->r[5];
    /* 00558ca3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00558ca8u;f->boundary(0x00558ca3u,0x00545fb0u,f);
    /* 00558ca8 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00558caa */
    if((f->flags&MI_ZF))goto mi_00558cc9;
    /* 00558cac */
    f->r[0]=(f->r[0]&0xffffff00u)|(((pq_read_00558c00((0x00799a6cu+f->r[6]),1u))&0xffu)<<0);
    /* 00558cb2 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00558cb4 */
    if((f->flags&MI_ZF))goto mi_00558cc9;
    /* 00558cb6 */
    f->r[1]=pq_read_00558c00((0x00000014u+f->r[4]),4u);
    /* 00558cba */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00558cbc */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00558cbd */
    f->r[1]=f->r[5];
    /* 00558cbf */
    f->r[4]-=4;MI_U32(f->r[4])=0x00558cc4u;f->boundary(0x00558cbfu,0x00558be0u,f);
    /* 00558cc4 */
    a=f->r[0];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 00558cc7 */
    if((f->flags&MI_ZF))goto mi_00558cdd;
mi_00558cc9:;
    /* 00558cc9 */
    a=pq_read_00558c00((0x007999eeu+f->r[6]),2u);b=0xffffffffu;t=mi_alu(f,a,b,16,1);
    /* 00558cd1 */
    if(!(f->flags&MI_ZF))goto mi_00558ce0;
    /* 00558cd3 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((pq_read_00558c00((0x00799a6cu+f->r[6]),1u))&0xffu)<<0);
    /* 00558cd9 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00558cdb */
    if((f->flags&MI_ZF))goto mi_00558ce0;
mi_00558cdd:;
    /* 00558cdd */
    a=f->r[3];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
mi_00558ce0:;
    /* 00558ce0 */
    f->r[6]=pq_read_00558c00((0x00799a40u+f->r[6]),4u);
    /* 00558ce6 */
    a=f->r[6];b=0x0000001du;t=mi_alu(f,a,b,32,1);
    /* 00558ce9 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00558e9d;
    /* 00558cef */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 00558cf1 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((pq_read_00558c00((0x00558ee0u+f->r[6]),1u))&0xffu)<<0);
    /* 00558cf7 */
    switch(pq_read_00558c00((0x00558ea8u+f->r[2]*4u),4u)){case 0x00558cfeu:goto mi_00558cfe;case 0x00558d0cu:goto mi_00558d0c;case 0x00558d1cu:goto mi_00558d1c;case 0x00558d2cu:goto mi_00558d2c;case 0x00558d53u:goto mi_00558d53;case 0x00558d7au:goto mi_00558d7a;case 0x00558da3u:goto mi_00558da3;case 0x00558dcau:goto mi_00558dca;case 0x00558df1u:goto mi_00558df1;case 0x00558e1au:goto mi_00558e1a;case 0x00558e41u:goto mi_00558e41;case 0x00558e68u:goto mi_00558e68;case 0x00558e91u:goto mi_00558e91;case 0x00558e9du:goto mi_00558e9d;default:abort();}
mi_00558cfe:;
    /* 00558cfe */
    f->r[0]=pq_read_00558c00((0x00f5762cu),4u);
    /* 00558d03 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00558d05 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00558d06 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00558d07 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00558d08 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00558d09 */
    f->r[4]+=12;return;
mi_00558d0c:;
    /* 00558d0c */
    f->r[1]=pq_read_00558c00((0x00f57634u),4u);
    /* 00558d12 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[7]*1u);
    /* 00558d15 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00558d16 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00558d17 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00558d18 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00558d19 */
    f->r[4]+=12;return;
mi_00558d1c:;
    /* 00558d1c */
    f->r[2]=pq_read_00558c00((0x00f57624u),4u);
    /* 00558d22 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[7]*1u);
    /* 00558d25 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00558d26 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00558d27 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00558d28 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00558d29 */
    f->r[4]+=12;return;
mi_00558d2c:;
    /* 00558d2c */
    f->r[0]=pq_read_00558c00((0x00000054u+f->r[5]),4u);
    /* 00558d2f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00558d31 */
    if((f->flags&MI_ZF))goto mi_00558d41;
    /* 00558d33 */
    f->r[0]=pq_read_00558c00((0x00f575acu),4u);
    /* 00558d38 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00558d3a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00558d3b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00558d3c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00558d3d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00558d3e */
    f->r[4]+=12;return;
mi_00558d41:;
    /* 00558d41 */
    f->r[1]=pq_read_00558c00((0x00f575a4u),4u);
    /* 00558d47 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[3]*1u);
    /* 00558d4a */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00558d4c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00558d4d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00558d4e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00558d4f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00558d50 */
    f->r[4]+=12;return;
mi_00558d53:;
    /* 00558d53 */
    f->r[0]=pq_read_00558c00((0x00000054u+f->r[5]),4u);
    /* 00558d56 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00558d58 */
    if((f->flags&MI_ZF))goto mi_00558d6a;
    /* 00558d5a */
    f->r[2]=pq_read_00558c00((0x00f57644u),4u);
    /* 00558d60 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[7]*1u);
    /* 00558d63 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00558d64 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00558d65 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00558d66 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00558d67 */
    f->r[4]+=12;return;
mi_00558d6a:;
    /* 00558d6a */
    f->r[0]=pq_read_00558c00((0x00f5763cu),4u);
    /* 00558d6f */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00558d71 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00558d73 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00558d74 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00558d75 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00558d76 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00558d77 */
    f->r[4]+=12;return;
mi_00558d7a:;
    /* 00558d7a */
    f->r[0]=pq_read_00558c00((0x00000054u+f->r[5]),4u);
    /* 00558d7d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00558d7f */
    if((f->flags&MI_ZF))goto mi_00558d91;
    /* 00558d81 */
    f->r[1]=pq_read_00558c00((0x00f575bcu),4u);
    /* 00558d87 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[7]*1u);
    /* 00558d8a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00558d8b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00558d8c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00558d8d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00558d8e */
    f->r[4]+=12;return;
mi_00558d91:;
    /* 00558d91 */
    f->r[2]=pq_read_00558c00((0x00f575b4u),4u);
    /* 00558d97 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[3]*1u);
    /* 00558d9a */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00558d9c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00558d9d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00558d9e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00558d9f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00558da0 */
    f->r[4]+=12;return;
mi_00558da3:;
    /* 00558da3 */
    f->r[0]=pq_read_00558c00((0x00000054u+f->r[5]),4u);
    /* 00558da6 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00558da8 */
    if((f->flags&MI_ZF))goto mi_00558db8;
    /* 00558daa */
    f->r[0]=pq_read_00558c00((0x00f5759cu),4u);
    /* 00558daf */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00558db1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00558db2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00558db3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00558db4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00558db5 */
    f->r[4]+=12;return;
mi_00558db8:;
    /* 00558db8 */
    f->r[1]=pq_read_00558c00((0x00f57594u),4u);
    /* 00558dbe */
    f->r[0]=(0x00000000u+f->r[1]+f->r[3]*1u);
    /* 00558dc1 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00558dc3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00558dc4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00558dc5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00558dc6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00558dc7 */
    f->r[4]+=12;return;
mi_00558dca:;
    /* 00558dca */
    f->r[0]=pq_read_00558c00((0x00000054u+f->r[5]),4u);
    /* 00558dcd */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00558dcf */
    if((f->flags&MI_ZF))goto mi_00558de1;
    /* 00558dd1 */
    f->r[2]=pq_read_00558c00((0x00f575ccu),4u);
    /* 00558dd7 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[7]*1u);
    /* 00558dda */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00558ddb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00558ddc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00558ddd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00558dde */
    f->r[4]+=12;return;
mi_00558de1:;
    /* 00558de1 */
    f->r[0]=pq_read_00558c00((0x00f575c4u),4u);
    /* 00558de6 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00558de8 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00558dea */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00558deb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00558dec */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00558ded */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00558dee */
    f->r[4]+=12;return;
mi_00558df1:;
    /* 00558df1 */
    f->r[0]=pq_read_00558c00((0x00000054u+f->r[5]),4u);
    /* 00558df4 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00558df6 */
    if((f->flags&MI_ZF))goto mi_00558e08;
    /* 00558df8 */
    f->r[1]=pq_read_00558c00((0x00f575dcu),4u);
    /* 00558dfe */
    f->r[0]=(0x00000000u+f->r[1]+f->r[7]*1u);
    /* 00558e01 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00558e02 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00558e03 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00558e04 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00558e05 */
    f->r[4]+=12;return;
mi_00558e08:;
    /* 00558e08 */
    f->r[2]=pq_read_00558c00((0x00f575d4u),4u);
    /* 00558e0e */
    f->r[0]=(0x00000000u+f->r[2]+f->r[3]*1u);
    /* 00558e11 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00558e13 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00558e14 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00558e15 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00558e16 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00558e17 */
    f->r[4]+=12;return;
mi_00558e1a:;
    /* 00558e1a */
    f->r[0]=pq_read_00558c00((0x00000054u+f->r[5]),4u);
    /* 00558e1d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00558e1f */
    if((f->flags&MI_ZF))goto mi_00558e2f;
    /* 00558e21 */
    f->r[0]=pq_read_00558c00((0x00f575ecu),4u);
    /* 00558e26 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00558e28 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00558e29 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00558e2a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00558e2b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00558e2c */
    f->r[4]+=12;return;
mi_00558e2f:;
    /* 00558e2f */
    f->r[1]=pq_read_00558c00((0x00f575e4u),4u);
    /* 00558e35 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[3]*1u);
    /* 00558e38 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00558e3a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00558e3b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00558e3c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00558e3d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00558e3e */
    f->r[4]+=12;return;
mi_00558e41:;
    /* 00558e41 */
    f->r[0]=pq_read_00558c00((0x00000054u+f->r[5]),4u);
    /* 00558e44 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00558e46 */
    if((f->flags&MI_ZF))goto mi_00558e58;
    /* 00558e48 */
    f->r[2]=pq_read_00558c00((0x00f575fcu),4u);
    /* 00558e4e */
    f->r[0]=(0x00000000u+f->r[2]+f->r[7]*1u);
    /* 00558e51 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00558e52 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00558e53 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00558e54 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00558e55 */
    f->r[4]+=12;return;
mi_00558e58:;
    /* 00558e58 */
    f->r[0]=pq_read_00558c00((0x00f575f4u),4u);
    /* 00558e5d */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00558e5f */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00558e61 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00558e62 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00558e63 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00558e64 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00558e65 */
    f->r[4]+=12;return;
mi_00558e68:;
    /* 00558e68 */
    f->r[0]=pq_read_00558c00((0x00000054u+f->r[5]),4u);
    /* 00558e6b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00558e6d */
    if((f->flags&MI_ZF))goto mi_00558e7f;
    /* 00558e6f */
    f->r[1]=pq_read_00558c00((0x00f5760cu),4u);
    /* 00558e75 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[7]*1u);
    /* 00558e78 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00558e79 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00558e7a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00558e7b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00558e7c */
    f->r[4]+=12;return;
mi_00558e7f:;
    /* 00558e7f */
    f->r[2]=pq_read_00558c00((0x00f57604u),4u);
    /* 00558e85 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[3]*1u);
    /* 00558e88 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00558e8a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00558e8b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00558e8c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00558e8d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00558e8e */
    f->r[4]+=12;return;
mi_00558e91:;
    /* 00558e91 */
    f->r[0]=pq_read_00558c00((0x00f57614u),4u);
    /* 00558e96 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00558e97 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00558e98 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00558e99 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00558e9a */
    f->r[4]+=12;return;
mi_00558e9d:;
    /* 00558e9d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00558e9e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00558e9f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00558ea0 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00558ea2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00558ea3 */
    f->r[4]+=12;return;
}

/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */

#include <stdlib.h>



static const uint8_t mi_rom_005561c0[]={0x43,0x60,0x55,0x00,0xcc,0x5f,0x55,0x00,0x1a,0x60,0x55,0x00,0x6a,0x60,0x55,0x00,0x91,0x60,0x55,0x00,0xba,0x60,0x55,0x00,0xe1,0x60,0x55,0x00,0x08,0x61,0x55,0x00,0x31,0x61,0x55,0x00,0xa8,0x61,0x55,0x00,0x9e,0x5f,0x55,0x00,0x58,0x61,0x55,0x00,0x7f,0x61,0x55,0x00,0xac,0x5f,0x55,0x00,0xbc,0x5f,0x55,0x00,0xf3,0x5f,0x55,0x00,0xb4,0x61,0x55,0x00};
static const uint8_t mi_rom_00556204[]={0x00,0x01,0x02,0x10,0x03,0x04,0x05,0x06,0x07,0x10,0x10,0x10,0x08,0x10,0x10,0x10,0x10,0x09,0x0a,0x10,0x10,0x10,0x0b,0x0c,0x10,0x10,0x10,0x0d,0x0e,0x0f};
static __inline uint32_t pq_read_00555ea0(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x005561c0u&&width<=68u&&(offset=address-0x005561c0u)<=68u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_005561c0[offset+k]<<(k*8u);return result;
 }
 if(address>=0x00556204u&&width<=30u&&(offset=address-0x00556204u)<=30u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00556204[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}

void restore_placement_query_C_00555ea0(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00555ea0 */
    f->r[0]=pq_read_00555ea0((0x00000004u+f->r[4]),4u);
    /* 00555ea4 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00555ea5 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00555ea6 */
    f->r[2]=pq_read_00555ea0((0x00000010u+f->r[4]),4u);
    /* 00555eaa */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00555eab */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00555eac */
    f->r[7]=f->r[0];
    /* 00555eae */
    f->r[5]=f->r[1];
    /* 00555eb0 */
    a=f->r[7];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 00555eb3 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 00555eb5 */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(0x00000000u);
    /* 00555ebb */
    a=f->r[7];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 00555ebe */
    f->r[1]=pq_read_00555ea0((0x00799a38u+f->r[7]),4u);
    /* 00555ec4 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00555ec6 */
    if(!(f->flags&MI_ZF))goto mi_00555efa;
    /* 00555ec8 */
    f->r[0]=pq_read_00555ea0((0x00799a40u+f->r[7]),4u);
    /* 00555ece */
    a=f->r[0];b=0x00000012u;t=mi_alu(f,a,b,32,1);
    /* 00555ed1 */
    if((f->flags&MI_ZF))goto mi_00555efa;
    /* 00555ed3 */
    a=f->r[0];b=0x0000001bu;t=mi_alu(f,a,b,32,1);
    /* 00555ed6 */
    if((f->flags&MI_ZF))goto mi_00555efa;
    /* 00555ed8 */
    a=f->r[0];b=0x0000001cu;t=mi_alu(f,a,b,32,1);
    /* 00555edb */
    if((f->flags&MI_ZF))goto mi_00555efa;
    /* 00555edd */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00555edf */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(0x00000001u);
    /* 00555ee5 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((pq_read_00555ea0((0x00799a74u+f->r[7]),1u))&0xffu)<<0);
    /* 00555eeb */
    f->r[1]=pq_read_00555ea0((0x00f574dcu),4u);
    /* 00555ef1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00555ef2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00555ef3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00555ef4 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00555ef6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00555ef7 */
    f->r[4]+=12;return;
mi_00555efa:;
    /* 00555efa */
    f->r[0]=pq_read_00555ea0((0x00bca2e0u),4u);
    /* 00555eff */
    f->r[6]=pq_read_00555ea0((0x00799a68u+f->r[7]),4u);
    /* 00555f05 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 00555f06 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00555f08 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 00555f0a */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 00555f0c */
    if(!(f->flags&MI_SF))goto mi_00555f11;
    /* 00555f0e */
    a=f->r[6];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
mi_00555f11:;
    /* 00555f11 */
    a=f->r[1];b=0x00000003u;t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00555f14 */
    if((f->flags&MI_ZF))goto mi_00555f2e;
    /* 00555f16 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00555f17 */
    if((f->flags&MI_ZF))goto mi_00555f27;
    /* 00555f19 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00555f1a */
    if((f->flags&MI_ZF))goto mi_00555f20;
    /* 00555f1c */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 00555f1e */
    goto mi_00555f33;
mi_00555f20:;
    /* 00555f20 */
    f->r[3]=0x0000000cu;
    /* 00555f25 */
    goto mi_00555f33;
mi_00555f27:;
    /* 00555f27 */
    f->r[3]=0x00000008u;
    /* 00555f2c */
    goto mi_00555f33;
mi_00555f2e:;
    /* 00555f2e */
    f->r[3]=0x00000004u;
mi_00555f33:;
    /* 00555f33 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)pq_read_00555ea0((0x007999eau+f->r[7]),2u);
    /* 00555f3a */
    a=f->r[0];b=pq_read_00555ea0((0x00000050u+f->r[5]),4u);t=mi_alu(f,a,b,32,1);
    /* 00555f3d */
    if(!(f->flags&MI_ZF))goto mi_00555f41;
    /* 00555f3f */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
mi_00555f41:;
    /* 00555f41 */
    f->r[1]=f->r[5];
    /* 00555f43 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00555f48u;f->boundary(0x00555f43u,0x00545fb0u,f);
    /* 00555f48 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00555f4a */
    if((f->flags&MI_ZF))goto mi_00555f69;
    /* 00555f4c */
    f->r[0]=(f->r[0]&0xffffff00u)|(((pq_read_00555ea0((0x00799a6cu+f->r[7]),1u))&0xffu)<<0);
    /* 00555f52 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00555f54 */
    if((f->flags&MI_ZF))goto mi_00555f69;
    /* 00555f56 */
    f->r[1]=pq_read_00555ea0((0x00000014u+f->r[4]),4u);
    /* 00555f5a */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00555f5c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00555f5d */
    f->r[1]=f->r[5];
    /* 00555f5f */
    f->r[4]-=4;MI_U32(f->r[4])=0x00555f64u;f->boundary(0x00555f5fu,0x00555e80u,f);
    /* 00555f64 */
    a=f->r[0];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 00555f67 */
    if((f->flags&MI_ZF))goto mi_00555f7d;
mi_00555f69:;
    /* 00555f69 */
    a=pq_read_00555ea0((0x007999eeu+f->r[7]),2u);b=0xffffffffu;t=mi_alu(f,a,b,16,1);
    /* 00555f71 */
    if(!(f->flags&MI_ZF))goto mi_00555f80;
    /* 00555f73 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((pq_read_00555ea0((0x00799a6cu+f->r[7]),1u))&0xffu)<<0);
    /* 00555f79 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00555f7b */
    if((f->flags&MI_ZF))goto mi_00555f80;
mi_00555f7d:;
    /* 00555f7d */
    a=f->r[3];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
mi_00555f80:;
    /* 00555f80 */
    f->r[7]=pq_read_00555ea0((0x00799a40u+f->r[7]),4u);
    /* 00555f86 */
    a=f->r[7];b=0x0000001du;t=mi_alu(f,a,b,32,1);
    /* 00555f89 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_005561b4;
    /* 00555f8f */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 00555f91 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((pq_read_00555ea0((0x00556204u+f->r[7]),1u))&0xffu)<<0);
    /* 00555f97 */
    switch(pq_read_00555ea0((0x005561c0u+f->r[2]*4u),4u)){case 0x00555f9eu:goto mi_00555f9e;case 0x00555facu:goto mi_00555fac;case 0x00555fbcu:goto mi_00555fbc;case 0x00555fccu:goto mi_00555fcc;case 0x00555ff3u:goto mi_00555ff3;case 0x0055601au:goto mi_0055601a;case 0x00556043u:goto mi_00556043;case 0x0055606au:goto mi_0055606a;case 0x00556091u:goto mi_00556091;case 0x005560bau:goto mi_005560ba;case 0x005560e1u:goto mi_005560e1;case 0x00556108u:goto mi_00556108;case 0x00556131u:goto mi_00556131;case 0x00556158u:goto mi_00556158;case 0x0055617fu:goto mi_0055617f;case 0x005561a8u:goto mi_005561a8;case 0x005561b4u:goto mi_005561b4;default:abort();}
mi_00555f9e:;
    /* 00555f9e */
    f->r[0]=pq_read_00555ea0((0x00f5770cu),4u);
    /* 00555fa3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00555fa4 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00555fa6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00555fa7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00555fa8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00555fa9 */
    f->r[4]+=12;return;
mi_00555fac:;
    /* 00555fac */
    f->r[1]=pq_read_00555ea0((0x00f57714u),4u);
    /* 00555fb2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00555fb3 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[6]*1u);
    /* 00555fb6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00555fb7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00555fb8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00555fb9 */
    f->r[4]+=12;return;
mi_00555fbc:;
    /* 00555fbc */
    f->r[2]=pq_read_00555ea0((0x00f5771cu),4u);
    /* 00555fc2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00555fc3 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[6]*1u);
    /* 00555fc6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00555fc7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00555fc8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00555fc9 */
    f->r[4]+=12;return;
mi_00555fcc:;
    /* 00555fcc */
    f->r[0]=pq_read_00555ea0((0x00000054u+f->r[5]),4u);
    /* 00555fcf */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00555fd1 */
    if((f->flags&MI_ZF))goto mi_00555fe1;
    /* 00555fd3 */
    f->r[0]=pq_read_00555ea0((0x00f57664u),4u);
    /* 00555fd8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00555fd9 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00555fdb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00555fdc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00555fdd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00555fde */
    f->r[4]+=12;return;
mi_00555fe1:;
    /* 00555fe1 */
    f->r[1]=pq_read_00555ea0((0x00f5765cu),4u);
    /* 00555fe7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00555fe8 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[3]*1u);
    /* 00555feb */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00555fed */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00555fee */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00555fef */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00555ff0 */
    f->r[4]+=12;return;
mi_00555ff3:;
    /* 00555ff3 */
    f->r[0]=pq_read_00555ea0((0x00000054u+f->r[5]),4u);
    /* 00555ff6 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00555ff8 */
    if((f->flags&MI_ZF))goto mi_0055600a;
    /* 00555ffa */
    f->r[2]=pq_read_00555ea0((0x00f5772cu),4u);
    /* 00556000 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00556001 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[6]*1u);
    /* 00556004 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00556005 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00556006 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00556007 */
    f->r[4]+=12;return;
mi_0055600a:;
    /* 0055600a */
    f->r[0]=pq_read_00555ea0((0x00f57724u),4u);
    /* 0055600f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00556010 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00556012 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00556014 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00556015 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00556016 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00556017 */
    f->r[4]+=12;return;
mi_0055601a:;
    /* 0055601a */
    f->r[0]=pq_read_00555ea0((0x00000054u+f->r[5]),4u);
    /* 0055601d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0055601f */
    if((f->flags&MI_ZF))goto mi_00556031;
    /* 00556021 */
    f->r[1]=pq_read_00555ea0((0x00f57674u),4u);
    /* 00556027 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00556028 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[6]*1u);
    /* 0055602b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0055602c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0055602d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0055602e */
    f->r[4]+=12;return;
mi_00556031:;
    /* 00556031 */
    f->r[2]=pq_read_00555ea0((0x00f5766cu),4u);
    /* 00556037 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00556038 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[3]*1u);
    /* 0055603b */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0055603d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0055603e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0055603f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00556040 */
    f->r[4]+=12;return;
mi_00556043:;
    /* 00556043 */
    f->r[0]=pq_read_00555ea0((0x00000054u+f->r[5]),4u);
    /* 00556046 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00556048 */
    if((f->flags&MI_ZF))goto mi_00556058;
    /* 0055604a */
    f->r[0]=pq_read_00555ea0((0x00f57654u),4u);
    /* 0055604f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00556050 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00556052 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00556053 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00556054 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00556055 */
    f->r[4]+=12;return;
mi_00556058:;
    /* 00556058 */
    f->r[1]=pq_read_00555ea0((0x00f5764cu),4u);
    /* 0055605e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0055605f */
    f->r[0]=(0x00000000u+f->r[1]+f->r[3]*1u);
    /* 00556062 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00556064 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00556065 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00556066 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00556067 */
    f->r[4]+=12;return;
mi_0055606a:;
    /* 0055606a */
    f->r[0]=pq_read_00555ea0((0x00000054u+f->r[5]),4u);
    /* 0055606d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0055606f */
    if((f->flags&MI_ZF))goto mi_00556081;
    /* 00556071 */
    f->r[2]=pq_read_00555ea0((0x00f57684u),4u);
    /* 00556077 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00556078 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[6]*1u);
    /* 0055607b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0055607c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0055607d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0055607e */
    f->r[4]+=12;return;
mi_00556081:;
    /* 00556081 */
    f->r[0]=pq_read_00555ea0((0x00f5767cu),4u);
    /* 00556086 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00556087 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00556089 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0055608b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0055608c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0055608d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0055608e */
    f->r[4]+=12;return;
mi_00556091:;
    /* 00556091 */
    f->r[0]=pq_read_00555ea0((0x00000054u+f->r[5]),4u);
    /* 00556094 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00556096 */
    if((f->flags&MI_ZF))goto mi_005560a8;
    /* 00556098 */
    f->r[1]=pq_read_00555ea0((0x00f57694u),4u);
    /* 0055609e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0055609f */
    f->r[0]=(0x00000000u+f->r[1]+f->r[6]*1u);
    /* 005560a2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 005560a3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 005560a4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 005560a5 */
    f->r[4]+=12;return;
mi_005560a8:;
    /* 005560a8 */
    f->r[2]=pq_read_00555ea0((0x00f5768cu),4u);
    /* 005560ae */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 005560af */
    f->r[0]=(0x00000000u+f->r[2]+f->r[3]*1u);
    /* 005560b2 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 005560b4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 005560b5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 005560b6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 005560b7 */
    f->r[4]+=12;return;
mi_005560ba:;
    /* 005560ba */
    f->r[0]=pq_read_00555ea0((0x00000054u+f->r[5]),4u);
    /* 005560bd */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 005560bf */
    if((f->flags&MI_ZF))goto mi_005560cf;
    /* 005560c1 */
    f->r[0]=pq_read_00555ea0((0x00f576a4u),4u);
    /* 005560c6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 005560c7 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 005560c9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 005560ca */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 005560cb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 005560cc */
    f->r[4]+=12;return;
mi_005560cf:;
    /* 005560cf */
    f->r[1]=pq_read_00555ea0((0x00f5769cu),4u);
    /* 005560d5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 005560d6 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[3]*1u);
    /* 005560d9 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 005560db */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 005560dc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 005560dd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 005560de */
    f->r[4]+=12;return;
mi_005560e1:;
    /* 005560e1 */
    f->r[0]=pq_read_00555ea0((0x00000054u+f->r[5]),4u);
    /* 005560e4 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 005560e6 */
    if((f->flags&MI_ZF))goto mi_005560f8;
    /* 005560e8 */
    f->r[2]=pq_read_00555ea0((0x00f576b4u),4u);
    /* 005560ee */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 005560ef */
    f->r[0]=(0x00000000u+f->r[2]+f->r[6]*1u);
    /* 005560f2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 005560f3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 005560f4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 005560f5 */
    f->r[4]+=12;return;
mi_005560f8:;
    /* 005560f8 */
    f->r[0]=pq_read_00555ea0((0x00f576acu),4u);
    /* 005560fd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 005560fe */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00556100 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00556102 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00556103 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00556104 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00556105 */
    f->r[4]+=12;return;
mi_00556108:;
    /* 00556108 */
    f->r[0]=pq_read_00555ea0((0x00000054u+f->r[5]),4u);
    /* 0055610b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0055610d */
    if((f->flags&MI_ZF))goto mi_0055611f;
    /* 0055610f */
    f->r[1]=pq_read_00555ea0((0x00f576c4u),4u);
    /* 00556115 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00556116 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[6]*1u);
    /* 00556119 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0055611a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0055611b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0055611c */
    f->r[4]+=12;return;
mi_0055611f:;
    /* 0055611f */
    f->r[2]=pq_read_00555ea0((0x00f576bcu),4u);
    /* 00556125 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00556126 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[3]*1u);
    /* 00556129 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0055612b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0055612c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0055612d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0055612e */
    f->r[4]+=12;return;
mi_00556131:;
    /* 00556131 */
    f->r[0]=pq_read_00555ea0((0x00000054u+f->r[5]),4u);
    /* 00556134 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00556136 */
    if((f->flags&MI_ZF))goto mi_00556146;
    /* 00556138 */
    f->r[0]=pq_read_00555ea0((0x00f576d4u),4u);
    /* 0055613d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0055613e */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00556140 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00556141 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00556142 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00556143 */
    f->r[4]+=12;return;
mi_00556146:;
    /* 00556146 */
    f->r[1]=pq_read_00555ea0((0x00f576ccu),4u);
    /* 0055614c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0055614d */
    f->r[0]=(0x00000000u+f->r[1]+f->r[3]*1u);
    /* 00556150 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00556152 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00556153 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00556154 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00556155 */
    f->r[4]+=12;return;
mi_00556158:;
    /* 00556158 */
    f->r[0]=pq_read_00555ea0((0x00000054u+f->r[5]),4u);
    /* 0055615b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0055615d */
    if((f->flags&MI_ZF))goto mi_0055616f;
    /* 0055615f */
    f->r[2]=pq_read_00555ea0((0x00f576e4u),4u);
    /* 00556165 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00556166 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[6]*1u);
    /* 00556169 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0055616a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0055616b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0055616c */
    f->r[4]+=12;return;
mi_0055616f:;
    /* 0055616f */
    f->r[0]=pq_read_00555ea0((0x00f576dcu),4u);
    /* 00556174 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00556175 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00556177 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00556179 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0055617a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0055617b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0055617c */
    f->r[4]+=12;return;
mi_0055617f:;
    /* 0055617f */
    f->r[0]=pq_read_00555ea0((0x00000054u+f->r[5]),4u);
    /* 00556182 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00556184 */
    if((f->flags&MI_ZF))goto mi_00556196;
    /* 00556186 */
    f->r[1]=pq_read_00555ea0((0x00f576f4u),4u);
    /* 0055618c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0055618d */
    f->r[0]=(0x00000000u+f->r[1]+f->r[6]*1u);
    /* 00556190 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00556191 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00556192 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00556193 */
    f->r[4]+=12;return;
mi_00556196:;
    /* 00556196 */
    f->r[2]=pq_read_00555ea0((0x00f576ecu),4u);
    /* 0055619c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0055619d */
    f->r[0]=(0x00000000u+f->r[2]+f->r[3]*1u);
    /* 005561a0 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 005561a2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 005561a3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 005561a4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 005561a5 */
    f->r[4]+=12;return;
mi_005561a8:;
    /* 005561a8 */
    f->r[0]=pq_read_00555ea0((0x00f576fcu),4u);
    /* 005561ad */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 005561ae */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 005561af */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 005561b0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 005561b1 */
    f->r[4]+=12;return;
mi_005561b4:;
    /* 005561b4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 005561b5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 005561b6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 005561b7 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 005561b9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 005561ba */
    f->r[4]+=12;return;
}

/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */

#include <stdlib.h>



static const uint8_t mi_rom_0054f824[]={0x33,0xf6,0x54,0x00,0xbc,0xf5,0x54,0x00,0x0a,0xf6,0x54,0x00,0x5a,0xf6,0x54,0x00,0x81,0xf6,0x54,0x00,0xaa,0xf6,0x54,0x00,0xd1,0xf6,0x54,0x00,0xf8,0xf6,0x54,0x00,0x21,0xf7,0x54,0x00,0x48,0xf7,0x54,0x00,0x6f,0xf7,0x54,0x00,0x0f,0xf8,0x54,0x00,0x8e,0xf5,0x54,0x00,0x98,0xf7,0x54,0x00,0xbf,0xf7,0x54,0x00,0xe6,0xf7,0x54,0x00,0x9c,0xf5,0x54,0x00,0xac,0xf5,0x54,0x00,0xe3,0xf5,0x54,0x00,0x1b,0xf8,0x54,0x00};
static const uint8_t mi_rom_0054f874[]={0x00,0x01,0x02,0x13,0x03,0x13,0x04,0x05,0x06,0x13,0x13,0x07,0x08,0x09,0x0a,0x13,0x13,0x0b,0x0c,0x13,0x13,0x13,0x13,0x13,0x0d,0x0e,0x0f,0x10,0x11,0x12};
static __inline uint32_t pq_read_0054f490(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x0054f824u&&width<=80u&&(offset=address-0x0054f824u)<=80u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0054f824[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0054f874u&&width<=30u&&(offset=address-0x0054f874u)<=30u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0054f874[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}

void restore_placement_query_C_0054f490(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0054f490 */
    f->r[0]=pq_read_0054f490((0x00000004u+f->r[4]),4u);
    /* 0054f494 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0054f495 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0054f496 */
    f->r[2]=pq_read_0054f490((0x00000010u+f->r[4]),4u);
    /* 0054f49a */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0054f49b */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0054f49c */
    f->r[7]=f->r[0];
    /* 0054f49e */
    f->r[5]=f->r[1];
    /* 0054f4a0 */
    a=f->r[7];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 0054f4a3 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0054f4a5 */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(0x00000000u);
    /* 0054f4ab */
    a=f->r[7];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 0054f4ae */
    f->r[1]=pq_read_0054f490((0x00799a38u+f->r[7]),4u);
    /* 0054f4b4 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0054f4b6 */
    if(!(f->flags&MI_ZF))goto mi_0054f4ea;
    /* 0054f4b8 */
    f->r[0]=pq_read_0054f490((0x00799a40u+f->r[7]),4u);
    /* 0054f4be */
    a=f->r[0];b=0x00000012u;t=mi_alu(f,a,b,32,1);
    /* 0054f4c1 */
    if((f->flags&MI_ZF))goto mi_0054f4ea;
    /* 0054f4c3 */
    a=f->r[0];b=0x0000001bu;t=mi_alu(f,a,b,32,1);
    /* 0054f4c6 */
    if((f->flags&MI_ZF))goto mi_0054f4ea;
    /* 0054f4c8 */
    a=f->r[0];b=0x0000001cu;t=mi_alu(f,a,b,32,1);
    /* 0054f4cb */
    if((f->flags&MI_ZF))goto mi_0054f4ea;
    /* 0054f4cd */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0054f4cf */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(0x00000001u);
    /* 0054f4d5 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((pq_read_0054f490((0x00799a74u+f->r[7]),1u))&0xffu)<<0);
    /* 0054f4db */
    f->r[1]=pq_read_0054f490((0x00f574dcu),4u);
    /* 0054f4e1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0054f4e2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0054f4e3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0054f4e4 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0054f4e6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0054f4e7 */
    f->r[4]+=12;return;
mi_0054f4ea:;
    /* 0054f4ea */
    f->r[0]=pq_read_0054f490((0x00bca2e0u),4u);
    /* 0054f4ef */
    f->r[6]=pq_read_0054f490((0x00799a68u+f->r[7]),4u);
    /* 0054f4f5 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0054f4f6 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0054f4f8 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 0054f4fa */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 0054f4fc */
    if(!(f->flags&MI_SF))goto mi_0054f501;
    /* 0054f4fe */
    a=f->r[6];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
mi_0054f501:;
    /* 0054f501 */
    a=f->r[1];b=0x00000003u;t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0054f504 */
    if((f->flags&MI_ZF))goto mi_0054f51e;
    /* 0054f506 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0054f507 */
    if((f->flags&MI_ZF))goto mi_0054f517;
    /* 0054f509 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0054f50a */
    if((f->flags&MI_ZF))goto mi_0054f510;
    /* 0054f50c */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 0054f50e */
    goto mi_0054f523;
mi_0054f510:;
    /* 0054f510 */
    f->r[3]=0x0000000cu;
    /* 0054f515 */
    goto mi_0054f523;
mi_0054f517:;
    /* 0054f517 */
    f->r[3]=0x00000008u;
    /* 0054f51c */
    goto mi_0054f523;
mi_0054f51e:;
    /* 0054f51e */
    f->r[3]=0x00000004u;
mi_0054f523:;
    /* 0054f523 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)pq_read_0054f490((0x007999eau+f->r[7]),2u);
    /* 0054f52a */
    a=f->r[0];b=pq_read_0054f490((0x00000050u+f->r[5]),4u);t=mi_alu(f,a,b,32,1);
    /* 0054f52d */
    if(!(f->flags&MI_ZF))goto mi_0054f531;
    /* 0054f52f */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
mi_0054f531:;
    /* 0054f531 */
    f->r[1]=f->r[5];
    /* 0054f533 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0054f538u;f->boundary(0x0054f533u,0x00545fb0u,f);
    /* 0054f538 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0054f53a */
    if((f->flags&MI_ZF))goto mi_0054f559;
    /* 0054f53c */
    f->r[0]=(f->r[0]&0xffffff00u)|(((pq_read_0054f490((0x00799a6cu+f->r[7]),1u))&0xffu)<<0);
    /* 0054f542 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0054f544 */
    if((f->flags&MI_ZF))goto mi_0054f559;
    /* 0054f546 */
    f->r[1]=pq_read_0054f490((0x00000014u+f->r[4]),4u);
    /* 0054f54a */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0054f54c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0054f54d */
    f->r[1]=f->r[5];
    /* 0054f54f */
    f->r[4]-=4;MI_U32(f->r[4])=0x0054f554u;f->boundary(0x0054f54fu,0x0054f470u,f);
    /* 0054f554 */
    a=f->r[0];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 0054f557 */
    if((f->flags&MI_ZF))goto mi_0054f56d;
mi_0054f559:;
    /* 0054f559 */
    a=pq_read_0054f490((0x007999eeu+f->r[7]),2u);b=0xffffffffu;t=mi_alu(f,a,b,16,1);
    /* 0054f561 */
    if(!(f->flags&MI_ZF))goto mi_0054f570;
    /* 0054f563 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((pq_read_0054f490((0x00799a6cu+f->r[7]),1u))&0xffu)<<0);
    /* 0054f569 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0054f56b */
    if((f->flags&MI_ZF))goto mi_0054f570;
mi_0054f56d:;
    /* 0054f56d */
    a=f->r[3];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
mi_0054f570:;
    /* 0054f570 */
    f->r[7]=pq_read_0054f490((0x00799a40u+f->r[7]),4u);
    /* 0054f576 */
    a=f->r[7];b=0x0000001du;t=mi_alu(f,a,b,32,1);
    /* 0054f579 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0054f81b;
    /* 0054f57f */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0054f581 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((pq_read_0054f490((0x0054f874u+f->r[7]),1u))&0xffu)<<0);
    /* 0054f587 */
    switch(pq_read_0054f490((0x0054f824u+f->r[2]*4u),4u)){case 0x0054f58eu:goto mi_0054f58e;case 0x0054f59cu:goto mi_0054f59c;case 0x0054f5acu:goto mi_0054f5ac;case 0x0054f5bcu:goto mi_0054f5bc;case 0x0054f5e3u:goto mi_0054f5e3;case 0x0054f60au:goto mi_0054f60a;case 0x0054f633u:goto mi_0054f633;case 0x0054f65au:goto mi_0054f65a;case 0x0054f681u:goto mi_0054f681;case 0x0054f6aau:goto mi_0054f6aa;case 0x0054f6d1u:goto mi_0054f6d1;case 0x0054f6f8u:goto mi_0054f6f8;case 0x0054f721u:goto mi_0054f721;case 0x0054f748u:goto mi_0054f748;case 0x0054f76fu:goto mi_0054f76f;case 0x0054f798u:goto mi_0054f798;case 0x0054f7bfu:goto mi_0054f7bf;case 0x0054f7e6u:goto mi_0054f7e6;case 0x0054f80fu:goto mi_0054f80f;case 0x0054f81bu:goto mi_0054f81b;default:abort();}
mi_0054f58e:;
    /* 0054f58e */
    f->r[0]=pq_read_0054f490((0x00f57824u),4u);
    /* 0054f593 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0054f594 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0054f596 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0054f597 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0054f598 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0054f599 */
    f->r[4]+=12;return;
mi_0054f59c:;
    /* 0054f59c */
    f->r[1]=pq_read_0054f490((0x00f5782cu),4u);
    /* 0054f5a2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0054f5a3 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[6]*1u);
    /* 0054f5a6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0054f5a7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0054f5a8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0054f5a9 */
    f->r[4]+=12;return;
mi_0054f5ac:;
    /* 0054f5ac */
    f->r[2]=pq_read_0054f490((0x00f57834u),4u);
    /* 0054f5b2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0054f5b3 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[6]*1u);
    /* 0054f5b6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0054f5b7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0054f5b8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0054f5b9 */
    f->r[4]+=12;return;
mi_0054f5bc:;
    /* 0054f5bc */
    f->r[0]=pq_read_0054f490((0x00000054u+f->r[5]),4u);
    /* 0054f5bf */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0054f5c1 */
    if((f->flags&MI_ZF))goto mi_0054f5d1;
    /* 0054f5c3 */
    f->r[0]=pq_read_0054f490((0x00f5774cu),4u);
    /* 0054f5c8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0054f5c9 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0054f5cb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0054f5cc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0054f5cd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0054f5ce */
    f->r[4]+=12;return;
mi_0054f5d1:;
    /* 0054f5d1 */
    f->r[1]=pq_read_0054f490((0x00f57744u),4u);
    /* 0054f5d7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0054f5d8 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[3]*1u);
    /* 0054f5db */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0054f5dd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0054f5de */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0054f5df */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0054f5e0 */
    f->r[4]+=12;return;
mi_0054f5e3:;
    /* 0054f5e3 */
    f->r[0]=pq_read_0054f490((0x00000054u+f->r[5]),4u);
    /* 0054f5e6 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0054f5e8 */
    if((f->flags&MI_ZF))goto mi_0054f5fa;
    /* 0054f5ea */
    f->r[2]=pq_read_0054f490((0x00f57844u),4u);
    /* 0054f5f0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0054f5f1 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[6]*1u);
    /* 0054f5f4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0054f5f5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0054f5f6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0054f5f7 */
    f->r[4]+=12;return;
mi_0054f5fa:;
    /* 0054f5fa */
    f->r[0]=pq_read_0054f490((0x00f5783cu),4u);
    /* 0054f5ff */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0054f600 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0054f602 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0054f604 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0054f605 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0054f606 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0054f607 */
    f->r[4]+=12;return;
mi_0054f60a:;
    /* 0054f60a */
    f->r[0]=pq_read_0054f490((0x00000054u+f->r[5]),4u);
    /* 0054f60d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0054f60f */
    if((f->flags&MI_ZF))goto mi_0054f621;
    /* 0054f611 */
    f->r[1]=pq_read_0054f490((0x00f5775cu),4u);
    /* 0054f617 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0054f618 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[6]*1u);
    /* 0054f61b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0054f61c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0054f61d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0054f61e */
    f->r[4]+=12;return;
mi_0054f621:;
    /* 0054f621 */
    f->r[2]=pq_read_0054f490((0x00f57754u),4u);
    /* 0054f627 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0054f628 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[3]*1u);
    /* 0054f62b */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0054f62d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0054f62e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0054f62f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0054f630 */
    f->r[4]+=12;return;
mi_0054f633:;
    /* 0054f633 */
    f->r[0]=pq_read_0054f490((0x00000054u+f->r[5]),4u);
    /* 0054f636 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0054f638 */
    if((f->flags&MI_ZF))goto mi_0054f648;
    /* 0054f63a */
    f->r[0]=pq_read_0054f490((0x00f5773cu),4u);
    /* 0054f63f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0054f640 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0054f642 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0054f643 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0054f644 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0054f645 */
    f->r[4]+=12;return;
mi_0054f648:;
    /* 0054f648 */
    f->r[1]=pq_read_0054f490((0x00f57734u),4u);
    /* 0054f64e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0054f64f */
    f->r[0]=(0x00000000u+f->r[1]+f->r[3]*1u);
    /* 0054f652 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0054f654 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0054f655 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0054f656 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0054f657 */
    f->r[4]+=12;return;
mi_0054f65a:;
    /* 0054f65a */
    f->r[0]=pq_read_0054f490((0x00000054u+f->r[5]),4u);
    /* 0054f65d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0054f65f */
    if((f->flags&MI_ZF))goto mi_0054f671;
    /* 0054f661 */
    f->r[2]=pq_read_0054f490((0x00f5776cu),4u);
    /* 0054f667 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0054f668 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[6]*1u);
    /* 0054f66b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0054f66c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0054f66d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0054f66e */
    f->r[4]+=12;return;
mi_0054f671:;
    /* 0054f671 */
    f->r[0]=pq_read_0054f490((0x00f57764u),4u);
    /* 0054f676 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0054f677 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0054f679 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0054f67b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0054f67c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0054f67d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0054f67e */
    f->r[4]+=12;return;
mi_0054f681:;
    /* 0054f681 */
    f->r[0]=pq_read_0054f490((0x00000054u+f->r[5]),4u);
    /* 0054f684 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0054f686 */
    if((f->flags&MI_ZF))goto mi_0054f698;
    /* 0054f688 */
    f->r[1]=pq_read_0054f490((0x00f5777cu),4u);
    /* 0054f68e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0054f68f */
    f->r[0]=(0x00000000u+f->r[1]+f->r[6]*1u);
    /* 0054f692 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0054f693 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0054f694 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0054f695 */
    f->r[4]+=12;return;
mi_0054f698:;
    /* 0054f698 */
    f->r[2]=pq_read_0054f490((0x00f57774u),4u);
    /* 0054f69e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0054f69f */
    f->r[0]=(0x00000000u+f->r[2]+f->r[3]*1u);
    /* 0054f6a2 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0054f6a4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0054f6a5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0054f6a6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0054f6a7 */
    f->r[4]+=12;return;
mi_0054f6aa:;
    /* 0054f6aa */
    f->r[0]=pq_read_0054f490((0x00000054u+f->r[5]),4u);
    /* 0054f6ad */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0054f6af */
    if((f->flags&MI_ZF))goto mi_0054f6bf;
    /* 0054f6b1 */
    f->r[0]=pq_read_0054f490((0x00f5778cu),4u);
    /* 0054f6b6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0054f6b7 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0054f6b9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0054f6ba */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0054f6bb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0054f6bc */
    f->r[4]+=12;return;
mi_0054f6bf:;
    /* 0054f6bf */
    f->r[1]=pq_read_0054f490((0x00f57784u),4u);
    /* 0054f6c5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0054f6c6 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[3]*1u);
    /* 0054f6c9 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0054f6cb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0054f6cc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0054f6cd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0054f6ce */
    f->r[4]+=12;return;
mi_0054f6d1:;
    /* 0054f6d1 */
    f->r[0]=pq_read_0054f490((0x00000054u+f->r[5]),4u);
    /* 0054f6d4 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0054f6d6 */
    if((f->flags&MI_ZF))goto mi_0054f6e8;
    /* 0054f6d8 */
    f->r[2]=pq_read_0054f490((0x00f5779cu),4u);
    /* 0054f6de */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0054f6df */
    f->r[0]=(0x00000000u+f->r[2]+f->r[6]*1u);
    /* 0054f6e2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0054f6e3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0054f6e4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0054f6e5 */
    f->r[4]+=12;return;
mi_0054f6e8:;
    /* 0054f6e8 */
    f->r[0]=pq_read_0054f490((0x00f57794u),4u);
    /* 0054f6ed */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0054f6ee */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0054f6f0 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0054f6f2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0054f6f3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0054f6f4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0054f6f5 */
    f->r[4]+=12;return;
mi_0054f6f8:;
    /* 0054f6f8 */
    f->r[0]=pq_read_0054f490((0x00000054u+f->r[5]),4u);
    /* 0054f6fb */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0054f6fd */
    if((f->flags&MI_ZF))goto mi_0054f70f;
    /* 0054f6ff */
    f->r[1]=pq_read_0054f490((0x00f577acu),4u);
    /* 0054f705 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0054f706 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[6]*1u);
    /* 0054f709 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0054f70a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0054f70b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0054f70c */
    f->r[4]+=12;return;
mi_0054f70f:;
    /* 0054f70f */
    f->r[2]=pq_read_0054f490((0x00f577a4u),4u);
    /* 0054f715 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0054f716 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[3]*1u);
    /* 0054f719 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0054f71b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0054f71c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0054f71d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0054f71e */
    f->r[4]+=12;return;
mi_0054f721:;
    /* 0054f721 */
    f->r[0]=pq_read_0054f490((0x00000054u+f->r[5]),4u);
    /* 0054f724 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0054f726 */
    if((f->flags&MI_ZF))goto mi_0054f736;
    /* 0054f728 */
    f->r[0]=pq_read_0054f490((0x00f577bcu),4u);
    /* 0054f72d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0054f72e */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0054f730 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0054f731 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0054f732 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0054f733 */
    f->r[4]+=12;return;
mi_0054f736:;
    /* 0054f736 */
    f->r[1]=pq_read_0054f490((0x00f577b4u),4u);
    /* 0054f73c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0054f73d */
    f->r[0]=(0x00000000u+f->r[1]+f->r[3]*1u);
    /* 0054f740 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0054f742 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0054f743 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0054f744 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0054f745 */
    f->r[4]+=12;return;
mi_0054f748:;
    /* 0054f748 */
    f->r[0]=pq_read_0054f490((0x00000054u+f->r[5]),4u);
    /* 0054f74b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0054f74d */
    if((f->flags&MI_ZF))goto mi_0054f75f;
    /* 0054f74f */
    f->r[2]=pq_read_0054f490((0x00f577ccu),4u);
    /* 0054f755 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0054f756 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[6]*1u);
    /* 0054f759 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0054f75a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0054f75b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0054f75c */
    f->r[4]+=12;return;
mi_0054f75f:;
    /* 0054f75f */
    f->r[0]=pq_read_0054f490((0x00f577c4u),4u);
    /* 0054f764 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0054f765 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0054f767 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0054f769 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0054f76a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0054f76b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0054f76c */
    f->r[4]+=12;return;
mi_0054f76f:;
    /* 0054f76f */
    f->r[0]=pq_read_0054f490((0x00000054u+f->r[5]),4u);
    /* 0054f772 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0054f774 */
    if((f->flags&MI_ZF))goto mi_0054f786;
    /* 0054f776 */
    f->r[1]=pq_read_0054f490((0x00f577dcu),4u);
    /* 0054f77c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0054f77d */
    f->r[0]=(0x00000000u+f->r[1]+f->r[6]*1u);
    /* 0054f780 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0054f781 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0054f782 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0054f783 */
    f->r[4]+=12;return;
mi_0054f786:;
    /* 0054f786 */
    f->r[2]=pq_read_0054f490((0x00f577d4u),4u);
    /* 0054f78c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0054f78d */
    f->r[0]=(0x00000000u+f->r[2]+f->r[3]*1u);
    /* 0054f790 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0054f792 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0054f793 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0054f794 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0054f795 */
    f->r[4]+=12;return;
mi_0054f798:;
    /* 0054f798 */
    f->r[0]=pq_read_0054f490((0x00000054u+f->r[5]),4u);
    /* 0054f79b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0054f79d */
    if((f->flags&MI_ZF))goto mi_0054f7ad;
    /* 0054f79f */
    f->r[0]=pq_read_0054f490((0x00f577ecu),4u);
    /* 0054f7a4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0054f7a5 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0054f7a7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0054f7a8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0054f7a9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0054f7aa */
    f->r[4]+=12;return;
mi_0054f7ad:;
    /* 0054f7ad */
    f->r[1]=pq_read_0054f490((0x00f577e4u),4u);
    /* 0054f7b3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0054f7b4 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[3]*1u);
    /* 0054f7b7 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0054f7b9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0054f7ba */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0054f7bb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0054f7bc */
    f->r[4]+=12;return;
mi_0054f7bf:;
    /* 0054f7bf */
    f->r[0]=pq_read_0054f490((0x00000054u+f->r[5]),4u);
    /* 0054f7c2 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0054f7c4 */
    if((f->flags&MI_ZF))goto mi_0054f7d6;
    /* 0054f7c6 */
    f->r[2]=pq_read_0054f490((0x00f577fcu),4u);
    /* 0054f7cc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0054f7cd */
    f->r[0]=(0x00000000u+f->r[2]+f->r[6]*1u);
    /* 0054f7d0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0054f7d1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0054f7d2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0054f7d3 */
    f->r[4]+=12;return;
mi_0054f7d6:;
    /* 0054f7d6 */
    f->r[0]=pq_read_0054f490((0x00f577f4u),4u);
    /* 0054f7db */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0054f7dc */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0054f7de */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0054f7e0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0054f7e1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0054f7e2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0054f7e3 */
    f->r[4]+=12;return;
mi_0054f7e6:;
    /* 0054f7e6 */
    f->r[0]=pq_read_0054f490((0x00000054u+f->r[5]),4u);
    /* 0054f7e9 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0054f7eb */
    if((f->flags&MI_ZF))goto mi_0054f7fd;
    /* 0054f7ed */
    f->r[1]=pq_read_0054f490((0x00f5780cu),4u);
    /* 0054f7f3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0054f7f4 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[6]*1u);
    /* 0054f7f7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0054f7f8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0054f7f9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0054f7fa */
    f->r[4]+=12;return;
mi_0054f7fd:;
    /* 0054f7fd */
    f->r[2]=pq_read_0054f490((0x00f57804u),4u);
    /* 0054f803 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0054f804 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[3]*1u);
    /* 0054f807 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0054f809 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0054f80a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0054f80b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0054f80c */
    f->r[4]+=12;return;
mi_0054f80f:;
    /* 0054f80f */
    f->r[0]=pq_read_0054f490((0x00f57814u),4u);
    /* 0054f814 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0054f815 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0054f816 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0054f817 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0054f818 */
    f->r[4]+=12;return;
mi_0054f81b:;
    /* 0054f81b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0054f81c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0054f81d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0054f81e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0054f820 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0054f821 */
    f->r[4]+=12;return;
}

/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */




void restore_placement_query_C_004e6a70(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004e6a70 */
    f->r[2]=MI_U32((0x00000004u+f->r[4]));
    /* 004e6a74 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e6a75 */
    f->r[0]=f->r[2];
    /* 004e6a77 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004e6a7a */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004e6a7c */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004e6a7f */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999eau+f->r[0]));
    /* 004e6a86 */
    f->r[6]=(0x00000000u+f->r[2]+f->r[2]*2u);
    /* 004e6a89 */
    f->r[2]=(0x00000000u+f->r[2]+f->r[6]*4u);
    /* 004e6a8c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e6a8d */
    f->r[2]=MI_U32((0x00000058u+f->r[1]+f->r[2]*4u));
    /* 004e6a91 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004e6a94 */
    if((f->flags&MI_ZF))goto mi_004e6aa6;
    /* 004e6a96 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004e6a97 */
    if((f->flags&MI_ZF))goto mi_004e6a9e;
    /* 004e6a99 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004e6a9b */
    f->r[4]+=12;return;
mi_004e6a9e:;
    /* 004e6a9e */
    f->r[0]=MI_U32((0x00f55bb4u));
    /* 004e6aa3 */
    f->r[4]+=12;return;
mi_004e6aa6:;
    /* 004e6aa6 */
    f->r[2]=MI_U32((0x00000008u+f->r[4]));
    /* 004e6aaa */
    a=0x00000013u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e6aac */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(0x00000000u);
    /* 004e6ab2 */
    f->r[0]=MI_U32((0x00799a30u+f->r[0]));
    /* 004e6ab8 */
    f->r[1]=MI_U32((0x000000b4u+f->r[1]+f->r[0]*4u));
    /* 004e6abf */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e6ac0 */
    f->r[1]=0x00f55bb0u;
    /* 004e6ac5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e6acau;f->boundary(0x004e6ac5u,0x0053dd90u,f);
    /* 004e6aca */
    f->r[0]=MI_U32((0x00f5739cu));
    /* 004e6acf */
    f->r[4]+=12;return;
}

/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */




void restore_placement_query_C_00401af0(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00401af0 */
    f->r[0]=MI_U32((0x00000004u+f->r[4]));
    /* 00401af4 */
    f->r[1]=f->r[0];
    /* 00401af6 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00401af9 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00401afb */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999eau+f->r[1]*8u));
    /* 00401b03 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00401b05 */
    f->r[2]=MI_U32((0x00000008u+f->r[4]));
    /* 00401b09 */
    f->r[1]=f->r[0];
    /* 00401b0b */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00401b0e */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00401b10 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00401b13 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[0]));
    /* 00401b1a */
    a=f->r[1];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00401b1d */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[1]);
    /* 00401b1f */
    f->r[1]=MI_U32((0x0000000cu+f->r[4]));
    /* 00401b23 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[0]));
    /* 00401b2a */
    a=f->r[0];b=0x00000008u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00401b2d */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 00401b2f */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 00401b31 */
    f->r[4]+=16;return;
}

/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */




void restore_placement_query_C_0040eb90(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0040eb90 */
    f->r[0]=MI_U32((0x00000004u+f->r[4]));
    /* 0040eb94 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040eb95 */
    f->r[6]=f->r[0];
    /* 0040eb97 */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0040eb9a */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0040eb9c */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0040eb9f */
    a=MI_U32((0x00799a30u+f->r[6]));b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 0040eba6 */
    if(!mi_signed_less(f->flags))goto mi_0040ebe2;
    /* 0040eba8 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999eau+f->r[6]));
    /* 0040ebaf */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0040ebb2 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*4u);
    /* 0040ebb5 */
    f->r[0]=MI_U32((0x00000054u+f->r[1]+f->r[0]*4u));
    /* 0040ebb9 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[6]));
    /* 0040ebc0 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0040ebc1 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0040ebc3 */
    f->r[2]=MI_U32((0x0000000cu+f->r[4]));
    /* 0040ebc7 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 0040ebc9 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0040ebcb */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[1]);
    /* 0040ebcd */
    f->r[2]=MI_U32((0x00000010u+f->r[4]));
    /* 0040ebd1 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[6]));
    /* 0040ebd8 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0040ebda */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 0040ebdc */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[1]);
    /* 0040ebde */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040ebdf */
    f->r[4]+=16;return;
mi_0040ebe2:;
    /* 0040ebe2 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999eau+f->r[6]));
    /* 0040ebe9 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0040ebeb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040ebec */
    f->r[1]=f->r[0];
    /* 0040ebee */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0040ebf1 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0040ebf3 */
    f->r[1]=MI_U32((0x00000008u+f->r[4]));
    /* 0040ebf7 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0040ebfa */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[0]));
    /* 0040ec01 */
    a=f->r[2];b=0x00000003u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0040ec04 */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[2]);
    /* 0040ec06 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[0]));
    /* 0040ec0d */
    f->r[0]=MI_U32((0x0000000cu+f->r[4]));
    /* 0040ec11 */
    a=f->r[2];b=0x0000000bu;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0040ec14 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[2]);
    /* 0040ec16 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 0040ec18 */
    f->r[4]+=16;return;
}

/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */

#include <stdlib.h>



static const uint8_t mi_rom_00417578[]={0xbf,0x74,0x41,0x00,0xee,0x74,0x41,0x00,0x1b,0x75,0x41,0x00,0x48,0x75,0x41,0x00};
static __inline uint32_t pq_read_00417490(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x00417578u&&width<=16u&&(offset=address-0x00417578u)<=16u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00417578[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}

void restore_placement_query_C_00417490(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00417490 */
    f->r[1]=pq_read_00417490((0x00000004u+f->r[4]),4u);
    /* 00417494 */
    f->r[0]=f->r[1];
    /* 00417496 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00417499 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0041749b */
    f->r[2]=(uint32_t)(int32_t)(int16_t)pq_read_00417490((0x007999eau+f->r[0]*8u),2u);
    /* 004174a3 */
    f->r[0]=pq_read_00417490((0x00bca2e0u),4u);
    /* 004174a8 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004174aa */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004174ab */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004174ad */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004174af */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 004174b2 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00417572;
    /* 004174b8 */
    switch(pq_read_00417490((0x00417578u+f->r[0]*4u),4u)){case 0x004174bfu:goto mi_004174bf;case 0x004174eeu:goto mi_004174ee;case 0x0041751bu:goto mi_0041751b;case 0x00417548u:goto mi_00417548;default:abort();}
mi_004174bf:;
    /* 004174bf */
    f->r[0]=f->r[1];
    /* 004174c1 */
    f->r[2]=pq_read_00417490((0x00000008u+f->r[4]),4u);
    /* 004174c5 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004174c8 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004174ca */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004174cd */
    f->r[1]=(uint32_t)(int32_t)(int16_t)pq_read_00417490((0x007999deu+f->r[0]),2u);
    /* 004174d4 */
    a=f->r[1];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004174d7 */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[1]);
    /* 004174d9 */
    f->r[1]=pq_read_00417490((0x0000000cu+f->r[4]),4u);
    /* 004174dd */
    f->r[0]=(uint32_t)(int32_t)(int16_t)pq_read_00417490((0x007999e0u+f->r[0]),2u);
    /* 004174e4 */
    a=f->r[0];b=0x0000000fu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004174e7 */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 004174e9 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 004174eb */
    f->r[4]+=16;return;
mi_004174ee:;
    /* 004174ee */
    f->r[0]=f->r[1];
    /* 004174f0 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004174f3 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004174f5 */
    f->r[1]=pq_read_00417490((0x00000008u+f->r[4]),4u);
    /* 004174f9 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004174fc */
    f->r[2]=(uint32_t)(int32_t)(int16_t)pq_read_00417490((0x007999deu+f->r[0]),2u);
    /* 00417503 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00417504 */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[2]);
    /* 00417506 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)pq_read_00417490((0x007999e0u+f->r[0]),2u);
    /* 0041750d */
    f->r[0]=pq_read_00417490((0x0000000cu+f->r[4]),4u);
    /* 00417511 */
    a=f->r[2];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00417514 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[2]);
    /* 00417516 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 00417518 */
    f->r[4]+=16;return;
mi_0041751b:;
    /* 0041751b */
    f->r[0]=f->r[1];
    /* 0041751d */
    f->r[2]=pq_read_00417490((0x00000008u+f->r[4]),4u);
    /* 00417521 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00417524 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00417526 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00417529 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)pq_read_00417490((0x007999deu+f->r[0]),2u);
    /* 00417530 */
    a=f->r[1];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00417533 */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[1]);
    /* 00417535 */
    f->r[1]=pq_read_00417490((0x0000000cu+f->r[4]),4u);
    /* 00417539 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)pq_read_00417490((0x007999e0u+f->r[0]),2u);
    /* 00417540 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00417541 */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 00417543 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 00417545 */
    f->r[4]+=16;return;
mi_00417548:;
    /* 00417548 */
    f->r[0]=f->r[1];
    /* 0041754a */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0041754d */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0041754f */
    f->r[1]=pq_read_00417490((0x00000008u+f->r[4]),4u);
    /* 00417553 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00417556 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)pq_read_00417490((0x007999deu+f->r[0]),2u);
    /* 0041755d */
    a=f->r[2];b=0x0000000fu;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00417560 */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[2]);
    /* 00417562 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)pq_read_00417490((0x007999e0u+f->r[0]),2u);
    /* 00417569 */
    f->r[0]=pq_read_00417490((0x0000000cu+f->r[4]),4u);
    /* 0041756d */
    a=f->r[2];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00417570 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[2]);
mi_00417572:;
    /* 00417572 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 00417574 */
    f->r[4]+=16;return;
}

/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */

#include <stdlib.h>



static const uint8_t mi_rom_00545b94[]={0x55,0x5a,0x54,0x00,0x78,0x5a,0x54,0x00,0x9d,0x5a,0x54,0x00,0xc0,0x5a,0x54,0x00,0xe3,0x5a,0x54,0x00,0x06,0x5b,0x54,0x00,0x27,0x5b,0x54,0x00,0x48,0x5b,0x54,0x00,0x52,0x5b,0x54,0x00};
static __inline uint32_t pq_read_00545a30(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x00545b94u&&width<=36u&&(offset=address-0x00545b94u)<=36u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00545b94[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}

void restore_placement_query_C_00545a30(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00545a30 */
    f->r[1]=pq_read_00545a30((0x00000004u+f->r[4]),4u);
    /* 00545a34 */
    f->r[0]=f->r[1];
    /* 00545a36 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00545a39 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00545a3b */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00545a3e */
    f->r[1]=(uint32_t)(int32_t)(int16_t)pq_read_00545a30((0x007999eau+f->r[0]),2u);
    /* 00545a45 */
    a=f->r[1];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 00545a48 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00545b72;
    /* 00545a4e */
    switch(pq_read_00545a30((0x00545b94u+f->r[1]*4u),4u)){case 0x00545a55u:goto mi_00545a55;case 0x00545a78u:goto mi_00545a78;case 0x00545a9du:goto mi_00545a9d;case 0x00545ac0u:goto mi_00545ac0;case 0x00545ae3u:goto mi_00545ae3;case 0x00545b06u:goto mi_00545b06;case 0x00545b27u:goto mi_00545b27;case 0x00545b48u:goto mi_00545b48;case 0x00545b52u:goto mi_00545b52;default:abort();}
mi_00545a55:;
    /* 00545a55 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)pq_read_00545a30((0x007999deu+f->r[0]),2u);
    /* 00545a5c */
    f->r[2]=pq_read_00545a30((0x00000008u+f->r[4]),4u);
    /* 00545a60 */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00545a63 */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[1]);
    /* 00545a65 */
    f->r[1]=pq_read_00545a30((0x0000000cu+f->r[4]),4u);
    /* 00545a69 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)pq_read_00545a30((0x007999e0u+f->r[0]),2u);
    /* 00545a70 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00545a71 */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 00545a73 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 00545a75 */
    f->r[4]+=16;return;
mi_00545a78:;
    /* 00545a78 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)pq_read_00545a30((0x007999deu+f->r[0]),2u);
    /* 00545a7f */
    f->r[1]=pq_read_00545a30((0x00000008u+f->r[4]),4u);
    /* 00545a83 */
    a=f->r[2];b=0x00000003u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00545a86 */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[2]);
    /* 00545a88 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)pq_read_00545a30((0x007999e0u+f->r[0]),2u);
    /* 00545a8f */
    f->r[0]=pq_read_00545a30((0x0000000cu+f->r[4]),4u);
    /* 00545a93 */
    a=f->r[2];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00545a96 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[2]);
    /* 00545a98 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 00545a9a */
    f->r[4]+=16;return;
mi_00545a9d:;
    /* 00545a9d */
    f->r[1]=(uint32_t)(int32_t)(int16_t)pq_read_00545a30((0x007999deu+f->r[0]),2u);
    /* 00545aa4 */
    f->r[2]=pq_read_00545a30((0x00000008u+f->r[4]),4u);
    /* 00545aa8 */
    a=f->r[1];b=0x00000003u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00545aab */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[1]);
    /* 00545aad */
    f->r[1]=pq_read_00545a30((0x0000000cu+f->r[4]),4u);
    /* 00545ab1 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)pq_read_00545a30((0x007999e0u+f->r[0]),2u);
    /* 00545ab8 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00545ab9 */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 00545abb */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 00545abd */
    f->r[4]+=16;return;
mi_00545ac0:;
    /* 00545ac0 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)pq_read_00545a30((0x007999deu+f->r[0]),2u);
    /* 00545ac7 */
    f->r[1]=pq_read_00545a30((0x00000008u+f->r[4]),4u);
    /* 00545acb */
    a=f->r[2];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00545ace */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[2]);
    /* 00545ad0 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)pq_read_00545a30((0x007999e0u+f->r[0]),2u);
    /* 00545ad7 */
    f->r[0]=pq_read_00545a30((0x0000000cu+f->r[4]),4u);
    /* 00545adb */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00545adc */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[2]);
    /* 00545ade */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 00545ae0 */
    f->r[4]+=16;return;
mi_00545ae3:;
    /* 00545ae3 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)pq_read_00545a30((0x007999deu+f->r[0]),2u);
    /* 00545aea */
    f->r[2]=pq_read_00545a30((0x00000008u+f->r[4]),4u);
    /* 00545aee */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00545af1 */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[1]);
    /* 00545af3 */
    f->r[1]=pq_read_00545a30((0x0000000cu+f->r[4]),4u);
    /* 00545af7 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)pq_read_00545a30((0x007999e0u+f->r[0]),2u);
    /* 00545afe */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00545aff */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 00545b01 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 00545b03 */
    f->r[4]+=16;return;
mi_00545b06:;
    /* 00545b06 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)pq_read_00545a30((0x007999deu+f->r[0]),2u);
    /* 00545b0d */
    f->r[1]=pq_read_00545a30((0x00000008u+f->r[4]),4u);
    /* 00545b11 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00545b12 */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[2]);
    /* 00545b14 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)pq_read_00545a30((0x007999e0u+f->r[0]),2u);
    /* 00545b1b */
    f->r[0]=pq_read_00545a30((0x0000000cu+f->r[4]),4u);
    /* 00545b1f */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00545b20 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[2]);
    /* 00545b22 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 00545b24 */
    f->r[4]+=16;return;
mi_00545b27:;
    /* 00545b27 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)pq_read_00545a30((0x007999deu+f->r[0]),2u);
    /* 00545b2e */
    f->r[2]=pq_read_00545a30((0x00000008u+f->r[4]),4u);
    /* 00545b32 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00545b33 */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[1]);
    /* 00545b35 */
    f->r[1]=pq_read_00545a30((0x0000000cu+f->r[4]),4u);
    /* 00545b39 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)pq_read_00545a30((0x007999e0u+f->r[0]),2u);
    /* 00545b40 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00545b41 */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 00545b43 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 00545b45 */
    f->r[4]+=16;return;
mi_00545b48:;
    /* 00545b48 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)pq_read_00545a30((0x007999deu+f->r[0]),2u);
    /* 00545b4f */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00545b50 */
    goto mi_00545b79;
mi_00545b52:;
    /* 00545b52 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)pq_read_00545a30((0x007999deu+f->r[0]),2u);
    /* 00545b59 */
    f->r[2]=pq_read_00545a30((0x00000008u+f->r[4]),4u);
    /* 00545b5d */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00545b5e */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[1]);
    /* 00545b60 */
    f->r[1]=pq_read_00545a30((0x0000000cu+f->r[4]),4u);
    /* 00545b64 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)pq_read_00545a30((0x007999e0u+f->r[0]),2u);
    /* 00545b6b */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 00545b6d */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 00545b6f */
    f->r[4]+=16;return;
mi_00545b72:;
    /* 00545b72 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)pq_read_00545a30((0x007999deu+f->r[0]),2u);
mi_00545b79:;
    /* 00545b79 */
    f->r[1]=pq_read_00545a30((0x00000008u+f->r[4]),4u);
    /* 00545b7d */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[2]);
    /* 00545b7f */
    f->r[2]=(uint32_t)(int32_t)(int16_t)pq_read_00545a30((0x007999e0u+f->r[0]),2u);
    /* 00545b86 */
    f->r[0]=pq_read_00545a30((0x0000000cu+f->r[4]),4u);
    /* 00545b8a */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[2]);
    /* 00545b8c */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 00545b8e */
    f->r[4]+=16;return;
}

/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */

#include <stdlib.h>



static const uint8_t mi_rom_0055906c[]={0x25,0x8f,0x55,0x00,0x48,0x8f,0x55,0x00,0x6d,0x8f,0x55,0x00,0x7c,0x8f,0x55,0x00,0x9c,0x8f,0x55,0x00,0xc1,0x8f,0x55,0x00,0xe4,0x8f,0x55,0x00,0x04,0x90,0x55,0x00,0x29,0x90,0x55,0x00,0x49,0x90,0x55,0x00};
static __inline uint32_t pq_read_00558f00(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x0055906cu&&width<=40u&&(offset=address-0x0055906cu)<=40u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0055906c[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}

void restore_placement_query_C_00558f00(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00558f00 */
    f->r[1]=pq_read_00558f00((0x00000004u+f->r[4]),4u);
    /* 00558f04 */
    f->r[0]=f->r[1];
    /* 00558f06 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00558f09 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00558f0b */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00558f0e */
    f->r[1]=(uint32_t)(int32_t)(int16_t)pq_read_00558f00((0x007999eau+f->r[0]),2u);
    /* 00558f15 */
    a=f->r[1];b=0x00000009u;t=mi_alu(f,a,b,32,1);
    /* 00558f18 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00559065;
    /* 00558f1e */
    switch(pq_read_00558f00((0x0055906cu+f->r[1]*4u),4u)){case 0x00558f25u:goto mi_00558f25;case 0x00558f48u:goto mi_00558f48;case 0x00558f6du:goto mi_00558f6d;case 0x00558f7cu:goto mi_00558f7c;case 0x00558f9cu:goto mi_00558f9c;case 0x00558fc1u:goto mi_00558fc1;case 0x00558fe4u:goto mi_00558fe4;case 0x00559004u:goto mi_00559004;case 0x00559029u:goto mi_00559029;case 0x00559049u:goto mi_00559049;default:abort();}
mi_00558f25:;
    /* 00558f25 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)pq_read_00558f00((0x007999deu+f->r[0]),2u);
    /* 00558f2c */
    f->r[2]=pq_read_00558f00((0x00000008u+f->r[4]),4u);
    /* 00558f30 */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00558f33 */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[1]);
    /* 00558f35 */
    f->r[1]=pq_read_00558f00((0x0000000cu+f->r[4]),4u);
    /* 00558f39 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)pq_read_00558f00((0x007999e0u+f->r[0]),2u);
    /* 00558f40 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00558f41 */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 00558f43 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 00558f45 */
    f->r[4]+=16;return;
mi_00558f48:;
    /* 00558f48 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)pq_read_00558f00((0x007999deu+f->r[0]),2u);
    /* 00558f4f */
    f->r[1]=pq_read_00558f00((0x00000008u+f->r[4]),4u);
    /* 00558f53 */
    a=f->r[2];b=0x00000003u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00558f56 */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[2]);
    /* 00558f58 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)pq_read_00558f00((0x007999e0u+f->r[0]),2u);
    /* 00558f5f */
    f->r[0]=pq_read_00558f00((0x0000000cu+f->r[4]),4u);
    /* 00558f63 */
    a=f->r[2];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00558f66 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[2]);
    /* 00558f68 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 00558f6a */
    f->r[4]+=16;return;
mi_00558f6d:;
    /* 00558f6d */
    f->r[1]=(uint32_t)(int32_t)(int16_t)pq_read_00558f00((0x007999deu+f->r[0]),2u);
    /* 00558f74 */
    a=f->r[1];b=0x00000003u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00558f77 */
    goto mi_00559031;
mi_00558f7c:;
    /* 00558f7c */
    f->r[2]=(uint32_t)(int32_t)(int16_t)pq_read_00558f00((0x007999deu+f->r[0]),2u);
    /* 00558f83 */
    f->r[1]=pq_read_00558f00((0x00000008u+f->r[4]),4u);
    /* 00558f87 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00558f88 */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[2]);
    /* 00558f8a */
    f->r[2]=(uint32_t)(int32_t)(int16_t)pq_read_00558f00((0x007999e0u+f->r[0]),2u);
    /* 00558f91 */
    f->r[0]=pq_read_00558f00((0x0000000cu+f->r[4]),4u);
    /* 00558f95 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[2]);
    /* 00558f97 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 00558f99 */
    f->r[4]+=16;return;
mi_00558f9c:;
    /* 00558f9c */
    f->r[1]=(uint32_t)(int32_t)(int16_t)pq_read_00558f00((0x007999deu+f->r[0]),2u);
    /* 00558fa3 */
    f->r[2]=pq_read_00558f00((0x00000008u+f->r[4]),4u);
    /* 00558fa7 */
    a=f->r[1];b=0x00000003u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00558faa */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[1]);
    /* 00558fac */
    f->r[1]=pq_read_00558f00((0x0000000cu+f->r[4]),4u);
    /* 00558fb0 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)pq_read_00558f00((0x007999e0u+f->r[0]),2u);
    /* 00558fb7 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00558fba */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 00558fbc */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 00558fbe */
    f->r[4]+=16;return;
mi_00558fc1:;
    /* 00558fc1 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)pq_read_00558f00((0x007999deu+f->r[0]),2u);
    /* 00558fc8 */
    f->r[1]=pq_read_00558f00((0x00000008u+f->r[4]),4u);
    /* 00558fcc */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00558fcd */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[2]);
    /* 00558fcf */
    f->r[2]=(uint32_t)(int32_t)(int16_t)pq_read_00558f00((0x007999e0u+f->r[0]),2u);
    /* 00558fd6 */
    f->r[0]=pq_read_00558f00((0x0000000cu+f->r[4]),4u);
    /* 00558fda */
    a=f->r[2];b=0x00000003u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00558fdd */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[2]);
    /* 00558fdf */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 00558fe1 */
    f->r[4]+=16;return;
mi_00558fe4:;
    /* 00558fe4 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)pq_read_00558f00((0x007999deu+f->r[0]),2u);
    /* 00558feb */
    f->r[2]=pq_read_00558f00((0x00000008u+f->r[4]),4u);
    /* 00558fef */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[1]);
    /* 00558ff1 */
    f->r[1]=pq_read_00558f00((0x0000000cu+f->r[4]),4u);
    /* 00558ff5 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)pq_read_00558f00((0x007999e0u+f->r[0]),2u);
    /* 00558ffc */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00558ffd */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 00558fff */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 00559001 */
    f->r[4]+=16;return;
mi_00559004:;
    /* 00559004 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)pq_read_00558f00((0x007999deu+f->r[0]),2u);
    /* 0055900b */
    f->r[1]=pq_read_00558f00((0x00000008u+f->r[4]),4u);
    /* 0055900f */
    a=f->r[2];b=0x00000003u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00559012 */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[2]);
    /* 00559014 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)pq_read_00558f00((0x007999e0u+f->r[0]),2u);
    /* 0055901b */
    f->r[0]=pq_read_00558f00((0x0000000cu+f->r[4]),4u);
    /* 0055901f */
    a=f->r[2];b=0x00000005u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00559022 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[2]);
    /* 00559024 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 00559026 */
    f->r[4]+=16;return;
mi_00559029:;
    /* 00559029 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)pq_read_00558f00((0x007999deu+f->r[0]),2u);
    /* 00559030 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
mi_00559031:;
    /* 00559031 */
    f->r[2]=pq_read_00558f00((0x00000008u+f->r[4]),4u);
    /* 00559035 */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[1]);
    /* 00559037 */
    f->r[1]=pq_read_00558f00((0x0000000cu+f->r[4]),4u);
    /* 0055903b */
    f->r[0]=(uint32_t)(int32_t)(int16_t)pq_read_00558f00((0x007999e0u+f->r[0]),2u);
    /* 00559042 */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 00559044 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 00559046 */
    f->r[4]+=16;return;
mi_00559049:;
    /* 00559049 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)pq_read_00558f00((0x007999deu+f->r[0]),2u);
    /* 00559050 */
    f->r[1]=pq_read_00558f00((0x00000008u+f->r[4]),4u);
    /* 00559054 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00559055 */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[2]);
    /* 00559057 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)pq_read_00558f00((0x007999e0u+f->r[0]),2u);
    /* 0055905e */
    f->r[0]=pq_read_00558f00((0x0000000cu+f->r[4]),4u);
    /* 00559062 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00559063 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[2]);
mi_00559065:;
    /* 00559065 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 00559067 */
    f->r[4]+=16;return;
}

/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */

#include <stdlib.h>



static const uint8_t mi_rom_005563b0[]={0x55,0x62,0x55,0x00,0x92,0x62,0x55,0x00,0x55,0x62,0x55,0x00,0x6a,0x63,0x55,0x00,0x55,0x62,0x55,0x00,0x62,0x62,0x55,0x00,0x6f,0x62,0x55,0x00,0x92,0x62,0x55,0x00,0x55,0x62,0x55,0x00,0x92,0x62,0x55,0x00,0xb5,0x62,0x55,0x00,0x62,0x62,0x55,0x00,0xc4,0x62,0x55,0x00,0x29,0x63,0x55,0x00,0xe6,0x62,0x55,0x00,0x62,0x62,0x55,0x00,0x06,0x63,0x55,0x00,0x29,0x63,0x55,0x00,0x49,0x63,0x55,0x00,0x6a,0x63,0x55,0x00,0x8a,0x63,0x55,0x00};
static __inline uint32_t pq_read_00556230(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x005563b0u&&width<=84u&&(offset=address-0x005563b0u)<=84u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_005563b0[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}

void restore_placement_query_C_00556230(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00556230 */
    f->r[1]=pq_read_00556230((0x00000004u+f->r[4]),4u);
    /* 00556234 */
    f->r[0]=f->r[1];
    /* 00556236 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00556239 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0055623b */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0055623e */
    f->r[1]=(uint32_t)(int32_t)(int16_t)pq_read_00556230((0x007999eau+f->r[0]),2u);
    /* 00556245 */
    a=f->r[1];b=0x00000014u;t=mi_alu(f,a,b,32,1);
    /* 00556248 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_005563a8;
    /* 0055624e */
    switch(pq_read_00556230((0x005563b0u+f->r[1]*4u),4u)){case 0x00556255u:goto mi_00556255;case 0x00556262u:goto mi_00556262;case 0x0055626fu:goto mi_0055626f;case 0x00556292u:goto mi_00556292;case 0x005562b5u:goto mi_005562b5;case 0x005562c4u:goto mi_005562c4;case 0x005562e6u:goto mi_005562e6;case 0x00556306u:goto mi_00556306;case 0x00556329u:goto mi_00556329;case 0x00556349u:goto mi_00556349;case 0x0055636au:goto mi_0055636a;case 0x0055638au:goto mi_0055638a;default:abort();}
mi_00556255:;
    /* 00556255 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)pq_read_00556230((0x007999deu+f->r[0]),2u);
    /* 0055625c */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0055625d */
    goto mi_00556394;
mi_00556262:;
    /* 00556262 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)pq_read_00556230((0x007999deu+f->r[0]),2u);
    /* 00556269 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0055626a */
    goto mi_00556331;
mi_0055626f:;
    /* 0055626f */
    f->r[1]=(uint32_t)(int32_t)(int16_t)pq_read_00556230((0x007999deu+f->r[0]),2u);
    /* 00556276 */
    f->r[2]=pq_read_00556230((0x00000008u+f->r[4]),4u);
    /* 0055627a */
    a=f->r[1];b=0x00000003u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0055627d */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[1]);
    /* 0055627f */
    f->r[1]=pq_read_00556230((0x0000000cu+f->r[4]),4u);
    /* 00556283 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)pq_read_00556230((0x007999e0u+f->r[0]),2u);
    /* 0055628a */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0055628b */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 0055628d */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 0055628f */
    f->r[4]+=16;return;
mi_00556292:;
    /* 00556292 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)pq_read_00556230((0x007999deu+f->r[0]),2u);
    /* 00556299 */
    f->r[1]=pq_read_00556230((0x00000008u+f->r[4]),4u);
    /* 0055629d */
    a=f->r[2];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 005562a0 */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[2]);
    /* 005562a2 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)pq_read_00556230((0x007999e0u+f->r[0]),2u);
    /* 005562a9 */
    f->r[0]=pq_read_00556230((0x0000000cu+f->r[4]),4u);
    /* 005562ad */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 005562ae */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[2]);
    /* 005562b0 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 005562b2 */
    f->r[4]+=16;return;
mi_005562b5:;
    /* 005562b5 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)pq_read_00556230((0x007999deu+f->r[0]),2u);
    /* 005562bc */
    a=f->r[1];b=0x00000003u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 005562bf */
    goto mi_00556394;
mi_005562c4:;
    /* 005562c4 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)pq_read_00556230((0x007999deu+f->r[0]),2u);
    /* 005562cb */
    f->r[2]=pq_read_00556230((0x00000008u+f->r[4]),4u);
    /* 005562cf */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 005562d2 */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[1]);
    /* 005562d4 */
    f->r[1]=pq_read_00556230((0x0000000cu+f->r[4]),4u);
    /* 005562d8 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)pq_read_00556230((0x007999e0u+f->r[0]),2u);
    /* 005562df */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 005562e1 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 005562e3 */
    f->r[4]+=16;return;
mi_005562e6:;
    /* 005562e6 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)pq_read_00556230((0x007999deu+f->r[0]),2u);
    /* 005562ed */
    f->r[2]=pq_read_00556230((0x00000008u+f->r[4]),4u);
    /* 005562f1 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 005562f2 */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[1]);
    /* 005562f4 */
    f->r[1]=pq_read_00556230((0x0000000cu+f->r[4]),4u);
    /* 005562f8 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)pq_read_00556230((0x007999e0u+f->r[0]),2u);
    /* 005562ff */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 00556301 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 00556303 */
    f->r[4]+=16;return;
mi_00556306:;
    /* 00556306 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)pq_read_00556230((0x007999deu+f->r[0]),2u);
    /* 0055630d */
    f->r[2]=pq_read_00556230((0x00000008u+f->r[4]),4u);
    /* 00556311 */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00556314 */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[1]);
    /* 00556316 */
    f->r[1]=pq_read_00556230((0x0000000cu+f->r[4]),4u);
    /* 0055631a */
    f->r[0]=(uint32_t)(int32_t)(int16_t)pq_read_00556230((0x007999e0u+f->r[0]),2u);
    /* 00556321 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00556322 */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 00556324 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 00556326 */
    f->r[4]+=16;return;
mi_00556329:;
    /* 00556329 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)pq_read_00556230((0x007999deu+f->r[0]),2u);
    /* 00556330 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
mi_00556331:;
    /* 00556331 */
    f->r[1]=pq_read_00556230((0x00000008u+f->r[4]),4u);
    /* 00556335 */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[2]);
    /* 00556337 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)pq_read_00556230((0x007999e0u+f->r[0]),2u);
    /* 0055633e */
    f->r[0]=pq_read_00556230((0x0000000cu+f->r[4]),4u);
    /* 00556342 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[2]);
    /* 00556344 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 00556346 */
    f->r[4]+=16;return;
mi_00556349:;
    /* 00556349 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)pq_read_00556230((0x007999deu+f->r[0]),2u);
    /* 00556350 */
    f->r[2]=pq_read_00556230((0x00000008u+f->r[4]),4u);
    /* 00556354 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00556355 */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[1]);
    /* 00556357 */
    f->r[1]=pq_read_00556230((0x0000000cu+f->r[4]),4u);
    /* 0055635b */
    f->r[0]=(uint32_t)(int32_t)(int16_t)pq_read_00556230((0x007999e0u+f->r[0]),2u);
    /* 00556362 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00556363 */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 00556365 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 00556367 */
    f->r[4]+=16;return;
mi_0055636a:;
    /* 0055636a */
    f->r[2]=(uint32_t)(int32_t)(int16_t)pq_read_00556230((0x007999deu+f->r[0]),2u);
    /* 00556371 */
    f->r[1]=pq_read_00556230((0x00000008u+f->r[4]),4u);
    /* 00556375 */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[2]);
    /* 00556377 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)pq_read_00556230((0x007999e0u+f->r[0]),2u);
    /* 0055637e */
    f->r[0]=pq_read_00556230((0x0000000cu+f->r[4]),4u);
    /* 00556382 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00556383 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[2]);
    /* 00556385 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 00556387 */
    f->r[4]+=16;return;
mi_0055638a:;
    /* 0055638a */
    f->r[1]=(uint32_t)(int32_t)(int16_t)pq_read_00556230((0x007999deu+f->r[0]),2u);
    /* 00556391 */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
mi_00556394:;
    /* 00556394 */
    f->r[2]=pq_read_00556230((0x00000008u+f->r[4]),4u);
    /* 00556398 */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[1]);
    /* 0055639a */
    f->r[1]=pq_read_00556230((0x0000000cu+f->r[4]),4u);
    /* 0055639e */
    f->r[0]=(uint32_t)(int32_t)(int16_t)pq_read_00556230((0x007999e0u+f->r[0]),2u);
    /* 005563a5 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 005563a6 */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[0]);
mi_005563a8:;
    /* 005563a8 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 005563aa */
    f->r[4]+=16;return;
}

/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */

#include <stdlib.h>



static const uint8_t mi_rom_0054fa98[]={0xc5,0xf8,0x54,0x00,0xe8,0xf8,0x54,0x00,0xc5,0xf8,0x54,0x00,0x0d,0xf9,0x54,0x00,0xc5,0xf8,0x54,0x00,0x0d,0xf9,0x54,0x00,0x2d,0xf9,0x54,0x00,0x55,0xfa,0x54,0x00,0x75,0xfa,0x54,0x00,0x50,0xf9,0x54,0x00,0x59,0xf9,0x54,0x00,0xe8,0xf8,0x54,0x00,0x7e,0xf9,0x54,0x00,0x55,0xfa,0x54,0x00,0xa3,0xf9,0x54,0x00,0xc8,0xf9,0x54,0x00,0xc5,0xf8,0x54,0x00,0x14,0xfa,0x54,0x00,0xd2,0xf9,0x54,0x00,0x14,0xfa,0x54,0x00,0x34,0xfa,0x54,0x00,0xc8,0xf9,0x54,0x00,0xf2,0xf9,0x54,0x00,0x55,0xfa,0x54,0x00,0x75,0xfa,0x54,0x00,0x55,0xfa,0x54,0x00,0x75,0xfa,0x54,0x00,0x14,0xfa,0x54,0x00,0x34,0xfa,0x54,0x00,0x55,0xfa,0x54,0x00,0x75,0xfa,0x54,0x00};
static __inline uint32_t pq_read_0054f8a0(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x0054fa98u&&width<=124u&&(offset=address-0x0054fa98u)<=124u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0054fa98[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}

void restore_placement_query_C_0054f8a0(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0054f8a0 */
    f->r[1]=pq_read_0054f8a0((0x00000004u+f->r[4]),4u);
    /* 0054f8a4 */
    f->r[0]=f->r[1];
    /* 0054f8a6 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0054f8a9 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0054f8ab */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0054f8ae */
    f->r[1]=(uint32_t)(int32_t)(int16_t)pq_read_0054f8a0((0x007999eau+f->r[0]),2u);
    /* 0054f8b5 */
    a=f->r[1];b=0x0000001eu;t=mi_alu(f,a,b,32,1);
    /* 0054f8b8 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0054fa93;
    /* 0054f8be */
    switch(pq_read_0054f8a0((0x0054fa98u+f->r[1]*4u),4u)){case 0x0054f8c5u:goto mi_0054f8c5;case 0x0054f8e8u:goto mi_0054f8e8;case 0x0054f90du:goto mi_0054f90d;case 0x0054f92du:goto mi_0054f92d;case 0x0054f950u:goto mi_0054f950;case 0x0054f959u:goto mi_0054f959;case 0x0054f97eu:goto mi_0054f97e;case 0x0054f9a3u:goto mi_0054f9a3;case 0x0054f9c8u:goto mi_0054f9c8;case 0x0054f9d2u:goto mi_0054f9d2;case 0x0054f9f2u:goto mi_0054f9f2;case 0x0054fa14u:goto mi_0054fa14;case 0x0054fa34u:goto mi_0054fa34;case 0x0054fa55u:goto mi_0054fa55;case 0x0054fa75u:goto mi_0054fa75;default:abort();}
mi_0054f8c5:;
    /* 0054f8c5 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)pq_read_0054f8a0((0x007999deu+f->r[0]),2u);
    /* 0054f8cc */
    f->r[2]=pq_read_0054f8a0((0x00000008u+f->r[4]),4u);
    /* 0054f8d0 */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0054f8d3 */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[1]);
    /* 0054f8d5 */
    f->r[1]=pq_read_0054f8a0((0x0000000cu+f->r[4]),4u);
    /* 0054f8d9 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)pq_read_0054f8a0((0x007999e0u+f->r[0]),2u);
    /* 0054f8e0 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0054f8e1 */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 0054f8e3 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 0054f8e5 */
    f->r[4]+=16;return;
mi_0054f8e8:;
    /* 0054f8e8 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)pq_read_0054f8a0((0x007999deu+f->r[0]),2u);
    /* 0054f8ef */
    a=f->r[2];b=0x00000003u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
mi_0054f8f2:;
    /* 0054f8f2 */
    f->r[1]=pq_read_0054f8a0((0x00000008u+f->r[4]),4u);
    /* 0054f8f6 */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[2]);
    /* 0054f8f8 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)pq_read_0054f8a0((0x007999e0u+f->r[0]),2u);
    /* 0054f8ff */
    f->r[0]=pq_read_0054f8a0((0x0000000cu+f->r[4]),4u);
    /* 0054f903 */
    a=f->r[2];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0054f906 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[2]);
    /* 0054f908 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 0054f90a */
    f->r[4]+=16;return;
mi_0054f90d:;
    /* 0054f90d */
    f->r[2]=(uint32_t)(int32_t)(int16_t)pq_read_0054f8a0((0x007999deu+f->r[0]),2u);
    /* 0054f914 */
    f->r[1]=pq_read_0054f8a0((0x00000008u+f->r[4]),4u);
    /* 0054f918 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0054f919 */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[2]);
    /* 0054f91b */
    f->r[2]=(uint32_t)(int32_t)(int16_t)pq_read_0054f8a0((0x007999e0u+f->r[0]),2u);
    /* 0054f922 */
    f->r[0]=pq_read_0054f8a0((0x0000000cu+f->r[4]),4u);
    /* 0054f926 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[2]);
    /* 0054f928 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 0054f92a */
    f->r[4]+=16;return;
mi_0054f92d:;
    /* 0054f92d */
    f->r[1]=(uint32_t)(int32_t)(int16_t)pq_read_0054f8a0((0x007999deu+f->r[0]),2u);
    /* 0054f934 */
    f->r[2]=pq_read_0054f8a0((0x00000008u+f->r[4]),4u);
    /* 0054f938 */
    a=f->r[1];b=0x00000003u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0054f93b */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[1]);
    /* 0054f93d */
    f->r[1]=pq_read_0054f8a0((0x0000000cu+f->r[4]),4u);
    /* 0054f941 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)pq_read_0054f8a0((0x007999e0u+f->r[0]),2u);
    /* 0054f948 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0054f949 */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 0054f94b */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 0054f94d */
    f->r[4]+=16;return;
mi_0054f950:;
    /* 0054f950 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)pq_read_0054f8a0((0x007999deu+f->r[0]),2u);
    /* 0054f957 */
    goto mi_0054f8f2;
mi_0054f959:;
    /* 0054f959 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)pq_read_0054f8a0((0x007999deu+f->r[0]),2u);
    /* 0054f960 */
    f->r[2]=pq_read_0054f8a0((0x00000008u+f->r[4]),4u);
    /* 0054f964 */
    a=f->r[1];b=0x00000003u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0054f967 */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[1]);
    /* 0054f969 */
    f->r[1]=pq_read_0054f8a0((0x0000000cu+f->r[4]),4u);
    /* 0054f96d */
    f->r[0]=(uint32_t)(int32_t)(int16_t)pq_read_0054f8a0((0x007999e0u+f->r[0]),2u);
    /* 0054f974 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0054f977 */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 0054f979 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 0054f97b */
    f->r[4]+=16;return;
mi_0054f97e:;
    /* 0054f97e */
    f->r[1]=(uint32_t)(int32_t)(int16_t)pq_read_0054f8a0((0x007999deu+f->r[0]),2u);
    /* 0054f985 */
    f->r[2]=pq_read_0054f8a0((0x00000008u+f->r[4]),4u);
    /* 0054f989 */
    a=f->r[1];b=0x00000005u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0054f98c */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[1]);
    /* 0054f98e */
    f->r[1]=pq_read_0054f8a0((0x0000000cu+f->r[4]),4u);
    /* 0054f992 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)pq_read_0054f8a0((0x007999e0u+f->r[0]),2u);
    /* 0054f999 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0054f99c */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 0054f99e */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 0054f9a0 */
    f->r[4]+=16;return;
mi_0054f9a3:;
    /* 0054f9a3 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)pq_read_0054f8a0((0x007999deu+f->r[0]),2u);
    /* 0054f9aa */
    f->r[2]=pq_read_0054f8a0((0x00000008u+f->r[4]),4u);
    /* 0054f9ae */
    a=f->r[1];b=0x00000003u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0054f9b1 */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[1]);
    /* 0054f9b3 */
    f->r[1]=pq_read_0054f8a0((0x0000000cu+f->r[4]),4u);
    /* 0054f9b7 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)pq_read_0054f8a0((0x007999e0u+f->r[0]),2u);
    /* 0054f9be */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0054f9c1 */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 0054f9c3 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 0054f9c5 */
    f->r[4]+=16;return;
mi_0054f9c8:;
    /* 0054f9c8 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)pq_read_0054f8a0((0x007999deu+f->r[0]),2u);
    /* 0054f9cf */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0054f9d0 */
    goto mi_0054fa1c;
mi_0054f9d2:;
    /* 0054f9d2 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)pq_read_0054f8a0((0x007999deu+f->r[0]),2u);
    /* 0054f9d9 */
    f->r[2]=pq_read_0054f8a0((0x00000008u+f->r[4]),4u);
    /* 0054f9dd */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0054f9de */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[1]);
    /* 0054f9e0 */
    f->r[1]=pq_read_0054f8a0((0x0000000cu+f->r[4]),4u);
    /* 0054f9e4 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)pq_read_0054f8a0((0x007999e0u+f->r[0]),2u);
    /* 0054f9eb */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 0054f9ed */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 0054f9ef */
    f->r[4]+=16;return;
mi_0054f9f2:;
    /* 0054f9f2 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)pq_read_0054f8a0((0x007999deu+f->r[0]),2u);
    /* 0054f9f9 */
    f->r[2]=pq_read_0054f8a0((0x00000008u+f->r[4]),4u);
    /* 0054f9fd */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0054fa00 */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[1]);
    /* 0054fa02 */
    f->r[1]=pq_read_0054f8a0((0x0000000cu+f->r[4]),4u);
    /* 0054fa06 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)pq_read_0054f8a0((0x007999e0u+f->r[0]),2u);
    /* 0054fa0d */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 0054fa0f */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 0054fa11 */
    f->r[4]+=16;return;
mi_0054fa14:;
    /* 0054fa14 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)pq_read_0054f8a0((0x007999deu+f->r[0]),2u);
    /* 0054fa1b */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
mi_0054fa1c:;
    /* 0054fa1c */
    f->r[1]=pq_read_0054f8a0((0x00000008u+f->r[4]),4u);
    /* 0054fa20 */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[2]);
    /* 0054fa22 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)pq_read_0054f8a0((0x007999e0u+f->r[0]),2u);
    /* 0054fa29 */
    f->r[0]=pq_read_0054f8a0((0x0000000cu+f->r[4]),4u);
    /* 0054fa2d */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[2]);
    /* 0054fa2f */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 0054fa31 */
    f->r[4]+=16;return;
mi_0054fa34:;
    /* 0054fa34 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)pq_read_0054f8a0((0x007999deu+f->r[0]),2u);
    /* 0054fa3b */
    f->r[2]=pq_read_0054f8a0((0x00000008u+f->r[4]),4u);
    /* 0054fa3f */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0054fa40 */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[1]);
    /* 0054fa42 */
    f->r[1]=pq_read_0054f8a0((0x0000000cu+f->r[4]),4u);
    /* 0054fa46 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)pq_read_0054f8a0((0x007999e0u+f->r[0]),2u);
    /* 0054fa4d */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0054fa4e */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 0054fa50 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 0054fa52 */
    f->r[4]+=16;return;
mi_0054fa55:;
    /* 0054fa55 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)pq_read_0054f8a0((0x007999deu+f->r[0]),2u);
    /* 0054fa5c */
    f->r[1]=pq_read_0054f8a0((0x00000008u+f->r[4]),4u);
    /* 0054fa60 */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[2]);
    /* 0054fa62 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)pq_read_0054f8a0((0x007999e0u+f->r[0]),2u);
    /* 0054fa69 */
    f->r[0]=pq_read_0054f8a0((0x0000000cu+f->r[4]),4u);
    /* 0054fa6d */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0054fa6e */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[2]);
    /* 0054fa70 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 0054fa72 */
    f->r[4]+=16;return;
mi_0054fa75:;
    /* 0054fa75 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)pq_read_0054f8a0((0x007999deu+f->r[0]),2u);
    /* 0054fa7c */
    f->r[2]=pq_read_0054f8a0((0x00000008u+f->r[4]),4u);
    /* 0054fa80 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0054fa81 */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[1]);
    /* 0054fa83 */
    f->r[1]=pq_read_0054f8a0((0x0000000cu+f->r[4]),4u);
    /* 0054fa87 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)pq_read_0054f8a0((0x007999e0u+f->r[0]),2u);
    /* 0054fa8e */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0054fa91 */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[0]);
mi_0054fa93:;
    /* 0054fa93 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 0054fa95 */
    f->r[4]+=16;return;
}

/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */




void restore_placement_query_C_004e6ae0(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004e6ae0 */
    f->r[1]=MI_U32((0x00000004u+f->r[4]));
    /* 004e6ae4 */
    f->r[2]=MI_U32((0x00000008u+f->r[4]));
    /* 004e6ae8 */
    f->r[0]=f->r[1];
    /* 004e6aea */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004e6aed */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004e6aef */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004e6af2 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[0]));
    /* 004e6af9 */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004e6afc */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[1]);
    /* 004e6afe */
    f->r[1]=MI_U32((0x0000000cu+f->r[4]));
    /* 004e6b02 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[0]));
    /* 004e6b09 */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004e6b0c */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 004e6b0e */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 004e6b10 */
    f->r[4]+=16;return;
}

static __declspec(noreturn) void query_failure(uint32_t target)
{fprintf(stderr,"Unreconstructed placement query %08lx\n",(unsigned long)target);abort();}
/* Genuine547BA0: +38 returns a fixed piece count for these four native
   classes; +24 is the already reconstructed5477E0 worker query. */
uint32_t restore_placement_activity_00547ba0(uint32_t self,int32_t building)
{
 uint32_t original=(uint32_t)building,root=original-(uint32_t)(int32_t)(int16_t)MI_U16(0x7999eau+original*264u),eax,index,count,target;
 eax=MI_U32(0x799a70u+root*264u);if((int32_t)eax>0)return eax&0xffffff00u;
 target=MI_U32(MI_U32(self)+0x38u);
 switch(target){
 case 0x545f60u:count=9;break;case 0x559460u:count=10;break;
 case 0x5567b0u:count=21;break;case 0x54fed0u:count=31;break;
 default:query_failure(target);
 }
 eax=count;
 for(index=0;(int32_t)index<(int32_t)count;++index){
  target=MI_U32(MI_U32(self)+0x24u);if(target!=0x5477e0u)query_failure(target);
  eax=original+index;
  /* Native5477E0 modifies AL only. Its canonical AL facade additionally
     preserves the genuine invalid-worker slot-clear side effect. */
  eax=(eax&0xffffff00u)|restore_monument_worker_active(original+index);
  if((uint8_t)eax)return eax&0xffffff00u;
 }
 return (eax&0xffffff00u)|1u;
}
void restore_placement_queries_dispatch(uint32_t site,uint32_t target,struct restore_menu_input_frame *f)
{
 restore_menu_input_frame_call outer=f->boundary;uint32_t sp=f->r[4];(void)site;
 f->boundary=restore_placement_queries_dispatch;
 switch(target){
 case 0x401800u:restore_placement_query_C_00401800(f);break;
 case 0x40de80u:restore_placement_query_C_0040de80(f);break;
 case 0x417310u:restore_placement_query_C_00417310(f);break;
 case 0x545760u:restore_placement_query_C_00545760(f);break;
 case 0x558c00u:restore_placement_query_C_00558c00(f);break;
 case 0x555ea0u:restore_placement_query_C_00555ea0(f);break;
 case 0x54f490u:restore_placement_query_C_0054f490(f);break;
 case 0x4e6a70u:restore_placement_query_C_004e6a70(f);break;
 case 0x401af0u:restore_placement_query_C_00401af0(f);break;
 case 0x40eb90u:restore_placement_query_C_0040eb90(f);break;
 case 0x417490u:restore_placement_query_C_00417490(f);break;
 case 0x545a30u:restore_placement_query_C_00545a30(f);break;
 case 0x558f00u:restore_placement_query_C_00558f00(f);break;
 case 0x556230u:restore_placement_query_C_00556230(f);break;
 case 0x54f8a0u:restore_placement_query_C_0054f8a0(f);break;
 case 0x4e6ae0u:restore_placement_query_C_004e6ae0(f);break;
 case 0x547ba0u:f->r[0]=restore_placement_activity_00547ba0(f->r[1],(int32_t)MI_U32(sp+4u));f->r[4]=sp+8u;break;
 /* Exact no-argument tail adapter545FB0 -> genuine23-instruction4EA930. */
 case 0x545fb0u:f->r[0]=FUN_004ea930();f->r[4]=sp+4u;break;
 /* Native RET8: manager receiver is genuine incoming ECX, not a guessed
    singleton. These41 callers overwrite EAX before consuming its high bits. */
 case 0x53dd90u:
  f->r[0]=(f->r[0]&0xffffff00u)|FUN_0053dd90((RestoreSpriteLibraries *)(uintptr_t)f->r[1],(const char *)(uintptr_t)MI_U32(sp+4u),(int32_t)MI_U32(sp+8u));f->r[4]=sp+12u;break;
 case 0x545740u:f->r[0]=(uint32_t)restore_monument_progress_00545740((void *)(uintptr_t)f->r[1],MI_U32(sp+4u),(int32_t *)(uintptr_t)MI_U32(sp+8u));f->r[4]=sp+12u;break;
 case 0x558be0u:f->r[0]=(uint32_t)restore_monument_progress_00558be0((void *)(uintptr_t)f->r[1],MI_U32(sp+4u),(int32_t *)(uintptr_t)MI_U32(sp+8u));f->r[4]=sp+12u;break;
 case 0x555e80u:f->r[0]=(uint32_t)restore_monument_progress_00555e80((void *)(uintptr_t)f->r[1],MI_U32(sp+4u),(int32_t *)(uintptr_t)MI_U32(sp+8u));f->r[4]=sp+12u;break;
 case 0x54f470u:f->r[0]=(uint32_t)restore_monument_progress_0054f470((void *)(uintptr_t)f->r[1],MI_U32(sp+4u),(int32_t *)(uintptr_t)MI_U32(sp+8u));f->r[4]=sp+12u;break;
 default:query_failure(target);
 }
 f->boundary=outer;
}
static uint32_t query_run(uint32_t target,uint32_t self,int32_t building,uint32_t first,uint32_t second,uint32_t arguments)
{
 uint32_t stack[192],i,entry;struct restore_menu_input_frame f={{0},2,restore_placement_queries_dispatch};
 typedef char pointer_is32[sizeof(void*)==4?1:-1];(void)sizeof(pointer_is32);
 for(i=0;i<192;++i)stack[i]=0x8e47b123u;
 entry=(uint32_t)(uintptr_t)(stack+160);f.r[1]=self;f.r[4]=entry;
 stack[161]=(uint32_t)building;stack[162]=first;if(arguments==3)stack[163]=second;
 restore_placement_queries_dispatch(0,target,&f);
 if(f.r[4]!=entry+4u+arguments*4u||stack[160]!=0x8e47b123u||f.r[3]||f.r[5]||f.r[6]||f.r[7])query_failure(target);
 for(i=0;i<16;++i)if(stack[i]!=0x8e47b123u)query_failure(target);
 for(i=161u+arguments;i<192;++i)if(stack[i]!=0x8e47b123u)query_failure(target);
 return f.r[0];
}
uint32_t restore_placement_image_query(uint32_t self,int32_t building,uint32_t out_flags)
{return query_run(MI_U32(MI_U32(self)+0x1cu),self,building,out_flags,0,2);}
uint32_t restore_placement_position_query(uint32_t self,int32_t building,uint32_t out_x,uint32_t out_y)
{return query_run(MI_U32(MI_U32(self)+0x20u),self,building,out_x,out_y,3);}
