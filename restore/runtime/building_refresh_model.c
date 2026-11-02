/* Whole pinned 16-body/1909-instruction closure. Native bytes authenticated
 * against fde7cd65...d33c8d before translation. Tables have exact guarded sizes;
 * native record, index, divide-fault domains are retained without new clamps. */
#include "building_refresh_helpers.h"
#include "map_refresh_arithmetic.h"
#include "map_x87_conversion.h"
#include <stdlib.h>
#define MI_U64(a) (*(volatile uint64_t *)(uintptr_t)(uint32_t)(a))
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
static __inline uint32_t mi_alu(struct restore_building_refresh_frame *f,uint32_t a,uint32_t b,uint32_t width,uint32_t operation)
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
static __inline uint32_t mi_shift(struct restore_building_refresh_frame *f,uint32_t a,uint32_t count,uint32_t width,uint32_t arithmetic)
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
static __inline uint32_t mi_imul(struct restore_building_refresh_frame *f,uint32_t a,uint32_t b)
{
 int64_t product=(int64_t)(int32_t)a*(int32_t)b;uint32_t result=(uint32_t)product;
 f->flags&=~(MI_CF|MI_OF);if(product!=(int64_t)(int32_t)result)f->flags|=MI_CF|MI_OF;
 return result;
}
static __inline int mi_signed_less(uint32_t flags){return ((flags&MI_SF)!=0)^((flags&MI_OF)!=0);}
static __inline void mi_idiv32(struct restore_building_refresh_frame *f,uint32_t divisor_bits)
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
void restore_building_refresh_C_00488690(struct restore_building_refresh_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00488690 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000018u+f->r[4])))&0xffu)<<0);
    /* 00488694 */
    a=f->r[4];b=0x00000064u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 00488697 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00488698 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 0048869a */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048869b */
    a=(f->r[0]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 0048869d */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048869e */
    if(!(f->flags&MI_ZF))goto mi_004886dd;
    /* 004886a0 */
    f->r[7]=MI_U32((0x00000074u+f->r[4]));
    /* 004886a4 */
    f->r[6]=MI_U32((0x00f56b24u));
    /* 004886aa */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004886ab */
    f->r[4]-=4;MI_U32(f->r[4])=0x004886b0u;f->boundary(0x004886abu,0x004890f0u,f);
    /* 004886b0 */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,0,a,8,1);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004886b2 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,2);f->r[0]=t;
    /* 004886b4 */
    f->r[1]=MI_U32((0x00000078u+f->r[4]));
    /* 004886b8 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004886b9 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004886bb */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004886bd */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004886be */
    f->r[0]=MI_U32((0x00000084u+f->r[4]));
    /* 004886c5 */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004886c7 */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004886c9 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004886ca */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004886cb */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004886cc */
    f->r[4]-=4;MI_U32(f->r[4])=0x004886d1u;f->boundary(0x004886ccu,0x0047bf50u,f);
    /* 004886d1 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004886d4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004886d5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004886d6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004886d7 */
    a=f->r[4];b=0x00000064u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004886da */
    f->r[4]+=28;return;
mi_004886dd:;
    /* 004886dd */
    f->r[1]=MI_U32((0x00000084u+f->r[4]));
    /* 004886e4 */
    f->r[0]=0x66666667u;
    /* 004886e9 */
    a=f->r[0];b=f->r[1];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 004886eb */
    a=f->r[2];b=0x00000002u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 004886ee */
    f->r[0]=f->r[2];
    /* 004886f0 */
    f->r[6]=MI_U32((0x00000080u+f->r[4]));
    /* 004886f7 */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 004886fa */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004886fc */
    f->r[1]=0x00000002u;
    /* 00488701 */
    f->r[0]=0x00000001u;
    /* 00488706 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0048870a */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0048870e */
    MI_U32((0x00000038u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00488712 */
    MI_U32((0x00000040u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00488716 */
    MI_U32((0x00000048u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0048871a */
    MI_U32((0x0000004cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0048871e */
    MI_U32((0x0000005cu+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00488722 */
    MI_U32((0x00000068u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00488726 */
    MI_U32((0x0000000cu+f->r[4]))=(uint32_t)(0xfffffffeu);
    /* 0048872e */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 00488732 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 00488736 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[3]);
    /* 0048873a */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(0xffffffffu);
    /* 00488742 */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(0x000000e6u);
    /* 0048874a */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(0x00000004u);
    /* 00488752 */
    MI_U32((0x00000030u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 00488756 */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 0048875a */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(0x000001cau);
    /* 00488762 */
    MI_U32((0x00000044u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 00488766 */
    MI_U32((0x00000050u+f->r[4]))=(uint32_t)(0x000001c9u);
    /* 0048876e */
    MI_U32((0x00000054u+f->r[4]))=(uint32_t)(0x00000003u);
    /* 00488776 */
    MI_U32((0x00000058u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 0048877a */
    MI_U32((0x00000060u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 0048877e */
    MI_U32((0x00000064u+f->r[4]))=(uint32_t)(0x000001c8u);
    /* 00488786 */
    MI_U32((0x0000006cu+f->r[4]))=(uint32_t)(f->r[3]);
    /* 0048878a */
    f->r[0]=(0x0000001cu+f->r[4]);
    /* 0048878e */
    f->r[1]=0x00000005u;
mi_00488793:;
    /* 00488793 */
    a=f->r[2];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 00488796 */
    f->r[7]=(0x00000005u+f->r[6]);
    /* 00488799 */
    if(!mi_signed_less(f->flags))goto mi_0048879e;
    /* 0048879b */
    f->r[7]=(0x00000000u+f->r[6]+f->r[2]*1u);
mi_0048879e:;
    /* 0048879e */
    a=f->r[2];b=0x00000004u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004887a1 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[7]);
    /* 004887a3 */
    if(!(f->flags&MI_SF))goto mi_004887a7;
    /* 004887a5 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
mi_004887a7:;
    /* 004887a7 */
    a=f->r[0];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004887aa */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004887ab */
    if(!(f->flags&MI_ZF))goto mi_00488793;
    /* 004887ad */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004887ae */
    f->r[7]=(0x0000001cu+f->r[4]);
    /* 004887b2 */
    MI_U32((0x0000008cu+f->r[4]))=(uint32_t)(0x00000005u);
mi_004887bd:;
    /* 004887bd */
    f->r[0]=MI_U32((0x00000000u+f->r[7]));
    /* 004887bf */
    f->r[5]=MI_U32((0x0000007cu+f->r[4]));
    /* 004887c3 */
    f->r[6]=MI_U32((0x00000080u+f->r[4]));
    /* 004887ca */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 004887cd */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004887d0 */
    f->r[1]=MI_U32((0x00000010u+f->r[4]+f->r[0]*1u));
    /* 004887d4 */
    f->r[2]=MI_U32((0x00000014u+f->r[4]+f->r[0]*1u));
    /* 004887d8 */
    a=f->r[1];b=MI_U32((0x0070e45cu));t=mi_imul(f,a,b);f->r[1]=t;
    /* 004887df */
    a=f->r[2];b=MI_U32((0x0070e44cu));t=mi_imul(f,a,b);f->r[2]=t;
    /* 004887e6 */
    f->r[0]=MI_U32((0x00000020u+f->r[4]+f->r[0]*1u));
    /* 004887ea */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004887ec */
    f->r[5]=MI_U32((0x0126517cu));
    /* 004887f2 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004887f4 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004887f6 */
    f->r[6]=f->r[0];
    /* 004887f8 */
    if(mi_signed_less(f->flags))goto mi_004887fc;
    /* 004887fa */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
mi_004887fc:;
    /* 004887fc */
    f->r[5]=MI_U32((0x0070e448u));
    /* 00488802 */
    a=f->r[6];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 00488805 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 00488807 */
    f->r[6]=(uint32_t)(int32_t)(int16_t)MI_U16((0x010d6a0eu+f->r[6]));
    /* 0048880e */
    a=f->r[5];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[5]=t;
    /* 00488810 */
    a=f->r[5];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 00488812 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00488813 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00488814 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00488815 */
    f->r[1]=0x00f55bb0u;
    /* 0048881a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048881fu;f->boundary(0x0048881au,0x0053e590u,f);
    /* 0048881f */
    f->r[0]=MI_U32((0x0000008cu+f->r[4]));
    /* 00488826 */
    a=f->r[7];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 00488829 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0048882a */
    MI_U32((0x0000008cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00488831 */
    if(!(f->flags&MI_ZF))goto mi_004887bd;
    /* 00488833 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00488834 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00488835 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00488836 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00488837 */
    a=f->r[4];b=0x00000064u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048883a */
    f->r[4]+=28;return;
}

static const uint8_t mi_rom_00488840_00488bfc[]={0xea,0x88,0x48,0x00,0x36,0x89,0x48,0x00,0x00,0x89,0x48,0x00,0x36,0x89,0x48,0x00,0x0e,0x89,0x48,0x00,0x36,0x89,0x48,0x00,0x1f,0x89,0x48,0x00};
static __inline uint32_t mi_read_00488840(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x00488bfcu&&width<=28u&&(offset=address-0x00488bfcu)<=28u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00488840_00488bfc[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}
void restore_building_refresh_C_00488840(struct restore_building_refresh_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    MRFloat fp[8];uint32_t fpn=0;uint16_t cw=mr_fp_control_word();(void)cw;
    /* 00488840 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read_00488840((0x00000018u+f->r[4]),1u))&0xffu)<<0);
    /* 00488844 */
    a=f->r[4];b=0x00000058u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 00488847 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00488849 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048884a */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048884b */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048884c */
    if(!(f->flags&MI_ZF))goto mi_004888a2;
    /* 0048884e */
    f->r[7]=mi_read_00488840((0x00000070u+f->r[4]),4u);
    /* 00488852 */
    f->r[3]=mi_read_00488840((0x0000006cu+f->r[4]),4u);
    /* 00488856 */
    f->r[6]=mi_read_00488840((0x00f56544u),4u);
    /* 0048885c */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048885e */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00488860 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00488861 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00488862 */
    f->r[1]=0x00bca350u;
    /* 00488867 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048886cu;f->boundary(0x00488867u,0x00489050u,f);
    /* 0048886c */
    f->r[1]=f->r[0];
    /* 0048886e */
    f->r[0]=0x92492493u;
    /* 00488873 */
    a=f->r[0];b=f->r[1];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 00488875 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00488877 */
    f->r[1]=mi_read_00488840((0x00000068u+f->r[4]),4u);
    /* 0048887b */
    a=f->r[2];b=0x00000003u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 0048887e */
    f->r[0]=f->r[2];
    /* 00488880 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00488882 */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 00488885 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00488887 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00488889 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048888a */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048888c */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048888e */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048888f */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00488890 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00488891 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00488896u;f->boundary(0x00488891u,0x0047bf50u,f);
    /* 00488896 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00488899 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0048889a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0048889b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0048889c */
    a=f->r[4];b=0x00000058u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048889f */
    f->r[4]+=28;return;
mi_004888a2:;
    /* 004888a2 */
    f->r[0]=mi_read_00488840((0x00000068u+f->r[4]),4u);
    /* 004888a6 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 004888a8 */
    f->r[6]=f->r[0];
    /* 004888aa */
    MI_U32((0x0000007cu+f->r[4]))=(uint32_t)(f->r[7]);
    /* 004888ae */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004888b1 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004888b3 */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004888b6 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read_00488840((0x007999e0u+f->r[6]),2u);
    /* 004888bd */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read_00488840((0x007999deu+f->r[6]),2u);
    /* 004888c4 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 004888c7 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*2u);
    /* 004888ca */
    f->r[2]=mi_read_00488840((0x00bca2d8u),4u);
    /* 004888d0 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004888d2 */
    f->r[1]=mi_read_00488840((0x00bca2e0u),4u);
    /* 004888d8 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004888db */
    a=f->r[1];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 004888de */
    f->r[0]=(0x00000000u+f->r[2]+f->r[0]*4u);
    /* 004888e1 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00488936;
    /* 004888e3 */
    switch(mi_read_00488840((0x00488bfcu+f->r[1]*4u),4u)){case 0x004888eau:goto mi_004888ea;case 0x00488900u:goto mi_00488900;case 0x0048890eu:goto mi_0048890e;case 0x0048891fu:goto mi_0048891f;case 0x00488936u:goto mi_00488936;default:abort();}
mi_004888ea:;
    /* 004888ea */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004888ec */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read_00488840((0x007999dbu+f->r[6]),1u))&0xffu)<<0);
    /* 004888f2 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 004888f5 */
    a=f->r[1];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004888f8 */
    f->r[1]=mi_read_00488840((0x005cc890u+f->r[1]),4u);
    /* 004888fe */
    goto mi_00488930;
mi_00488900:;
    /* 00488900 */
    f->r[2]=mi_read_00488840((0x005cc8c0u),4u);
    /* 00488906 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00488908 */
    MI_U32((0x0000007cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0048890c */
    goto mi_00488936;
mi_0048890e:;
    /* 0048890e */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 00488910 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read_00488840((0x007999dbu+f->r[6]),1u))&0xffu)<<0);
    /* 00488916 */
    f->r[1]=mi_read_00488840((0x005cc8b8u+f->r[1]*8u),4u);
    /* 0048891d */
    goto mi_00488930;
mi_0048891f:;
    /* 0048891f */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 00488921 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read_00488840((0x007999dbu+f->r[6]),1u))&0xffu)<<0);
    /* 00488927 */
    a=f->r[2];b=0x00000038u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 0048892a */
    f->r[1]=mi_read_00488840((0x005cc888u+f->r[2]),4u);
mi_00488930:;
    /* 00488930 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00488932 */
    MI_U32((0x0000007cu+f->r[4]))=(uint32_t)(f->r[1]);
mi_00488936:;
    /* 00488936 */
    f->r[1]=mi_read_00488840((0x0000007cu+f->r[4]),4u);
    /* 0048893a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0048893c */
    fp[fpn++]=mr_fp_binary64(MI_U64((0x0056f5a8u)));
    /* 00488942 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read_00488840((0x00adc36cu+f->r[1]),1u))&0xffu)<<0);
    /* 00488948 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00488949 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0048894d */
    f->r[4]-=4;MI_U32(f->r[4])=0x00488952u;{uint8_t raw[10];uint64_t result;mr_fp_raw80(fp[--fpn],raw);result=restore_map_trunc80(raw);f->r[0]=(uint32_t)result;f->r[2]=(uint32_t)(result>>32);}f->r[4]+=4;
    /* 00488952 */
    f->r[5]=mi_read_00488840((0x0000007cu+f->r[4]),4u);
    /* 00488956 */
    f->r[1]=f->r[0];
    /* 00488958 */
    a=f->r[1];b=f->r[5];t=mi_imul(f,a,b);f->r[1]=t;
    /* 0048895b */
    f->r[0]=mi_read_00488840((0x00000018u+f->r[4]),4u);
    /* 0048895f */
    a=f->r[3];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[3]=t;
    /* 00488962 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 00488966 */
    MI_U32((0x0000007cu+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0048896a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0048896c */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 00488970 */
    f->r[7]=0x00000001u;
    /* 00488975 */
    if((f->flags&MI_ZF))goto mi_00488a0b;
    /* 0048897b */
    a=mi_read_00488840((0x00799a18u+f->r[6]),2u);b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 00488983 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00488a0b;
    /* 00488989 */
    f->r[6]=mi_read_00488840((0x00bca34cu),4u);
    /* 0048898f */
    f->r[0]=0x66666667u;
    /* 00488994 */
    a=f->r[0];b=f->r[6];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 00488996 */
    a=f->r[2];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 00488998 */
    f->r[0]=f->r[2];
    /* 0048899a */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 0048899d */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0048899f */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004889a1 */
    if(!mi_signed_less(f->flags))goto mi_004889b1;
    /* 004889a3 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 004889a7 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(0x00000004u);
    /* 004889af */
    goto mi_00488a0b;
mi_004889b1:;
    /* 004889b1 */
    a=mi_read_00488840((0x00d386d4u),4u);b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004889b7 */
    if((f->flags&MI_ZF))goto mi_004889e1;
    /* 004889b9 */
    fp[fpn++]=mr_fp_i32((int32_t)MI_U32((0x0000007cu+f->r[4])));
    /* 004889bd */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 004889be */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00000002u);
    /* 004889c6 */
    MI_U32((0x0000007cu+f->r[4]))=(uint32_t)(f->r[6]);
    /* 004889ca */
    fp[fpn++]=mr_fp_i32((int32_t)MI_U32((0x0000007cu+f->r[4])));
    /* 004889ce */
    fp[fpn-2]=mr_fp_div(fp[fpn-2],fp[fpn-1],cw);--fpn;
    /* 004889d0 */
    fp[fpn-1]=mr_fp_mul(fp[fpn-1],mr_fp_binary64(MI_U64((0x0056f658u))),cw);
    /* 004889d6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004889dbu;{uint8_t raw[10];uint64_t result;mr_fp_raw80(fp[--fpn],raw);result=restore_map_trunc80(raw);f->r[0]=(uint32_t)result;f->r[2]=(uint32_t)(result>>32);}f->r[4]+=4;
    /* 004889db */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004889df */
    goto mi_00488a0b;
mi_004889e1:;
    /* 004889e1 */
    f->r[1]=mi_read_00488840((0x0078d7c4u),4u);
    /* 004889e7 */
    f->r[2]=mi_read_00488840((0x00000080u+f->r[4]),4u);
    /* 004889ee */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004889f0 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00000003u);
    /* 004889f8 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read_00488840((0x00acf85cu+f->r[1]+f->r[2]*1u),1u))&0xffu)<<0);
    /* 004889ff */
    f->r[1]=0x00000007u;
    /* 00488a04 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 00488a05 */
    a=f->r[1];mi_idiv32(f,a);
    /* 00488a07 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[2]);
mi_00488a0b:;
    /* 00488a0b */
    f->r[0]=0x66666667u;
    /* 00488a10 */
    f->r[1]=0x00000002u;
    /* 00488a15 */
    a=f->r[0];b=f->r[5];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 00488a17 */
    a=f->r[2];b=0x00000002u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 00488a1a */
    f->r[0]=f->r[2];
    /* 00488a1c */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[3]);
    /* 00488a20 */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 00488a23 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00488a25 */
    f->r[0]=0xfffffffeu;
    /* 00488a2a */
    f->r[5]=f->r[2];
    /* 00488a2c */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 00488a2e */
    MI_U32((0x00000038u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 00488a32 */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(f->r[3]);
    /* 00488a36 */
    MI_U32((0x00000058u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 00488a3a */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00488a3e */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00488a42 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00488a46 */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 00488a4a */
    MI_U32((0x00000030u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00488a4e */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00488a52 */
    MI_U32((0x00000040u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00488a56 */
    MI_U32((0x00000044u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00488a5a */
    MI_U32((0x00000048u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 00488a5e */
    MI_U32((0x0000004cu+f->r[4]))=(uint32_t)(f->r[7]);
    /* 00488a62 */
    MI_U32((0x00000050u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00488a66 */
    MI_U32((0x00000054u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00488a6a */
    MI_U32((0x0000005cu+f->r[4]))=(uint32_t)(f->r[7]);
    /* 00488a6e */
    MI_U32((0x00000060u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00488a72 */
    MI_U32((0x00000064u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00488a76 */
    f->r[3]=(0x00000024u+f->r[4]);
    /* 00488a7a */
    goto mi_00488a7e;
mi_00488a7c:;
    /* 00488a7c */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
mi_00488a7e:;
    /* 00488a7e */
    f->r[7]=mi_read_00488840((0xfffffffcu+f->r[3]),4u);
    /* 00488a81 */
    f->r[6]=mi_read_00488840((0x00000070u+f->r[4]),4u);
    /* 00488a85 */
    a=f->r[7];b=mi_read_00488840((0x0070e45cu),4u);t=mi_imul(f,a,b);f->r[7]=t;
    /* 00488a8c */
    a=f->r[7];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 00488a8e */
    f->r[6]=mi_read_00488840((0x00000000u+f->r[3]),4u);
    /* 00488a90 */
    a=f->r[6];b=mi_read_00488840((0x0070e44cu),4u);t=mi_imul(f,a,b);f->r[6]=t;
    /* 00488a97 */
    f->r[1]=mi_read_00488840((0x00000074u+f->r[4]),4u);
    /* 00488a9b */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00488a9d */
    a=f->r[5];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 00488aa0 */
    if(mi_signed_less(f->flags))goto mi_00488aab;
    /* 00488aa2 */
    f->r[1]=mi_read_00488840((0x00000078u+f->r[4]),4u);
    /* 00488aa6 */
    a=f->r[1];b=0x00000005u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00488aa9 */
    goto mi_00488ab2;
mi_00488aab:;
    /* 00488aab */
    f->r[0]=mi_read_00488840((0x00000078u+f->r[4]),4u);
    /* 00488aaf */
    f->r[1]=(0x00000000u+f->r[0]+f->r[5]*1u);
mi_00488ab2:;
    /* 00488ab2 */
    a=f->r[1];b=mi_read_00488840((0x0126517cu),4u);t=mi_alu(f,a,b,32,1);
    /* 00488ab8 */
    f->r[0]=f->r[1];
    /* 00488aba */
    if(mi_signed_less(f->flags))goto mi_00488abe;
    /* 00488abc */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_00488abe:;
    /* 00488abe */
    a=f->r[0];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00488ac1 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read_00488840((0x010d6a0eu+f->r[0]),2u);
    /* 00488ac8 */
    f->r[0]=mi_read_00488840((0x0070e448u),4u);
    /* 00488acd */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00488acf */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00488ad1 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00488ad3 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00488ad4 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00488ad5 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00488ad6 */
    f->r[1]=0x00f55bb0u;
    /* 00488adb */
    f->r[4]-=4;MI_U32(f->r[4])=0x00488ae0u;f->boundary(0x00488adbu,0x0053e590u,f);
    /* 00488ae0 */
    a=f->r[5];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[5]=t;
    /* 00488ae3 */
    MI_U32((0x0000007cu+f->r[4]))=(uint32_t)(f->r[5]);
    /* 00488ae7 */
    if(!(f->flags&MI_SF))goto mi_00488af5;
    /* 00488ae9 */
    MI_U32((0x0000007cu+f->r[4]))=(uint32_t)(0x00000000u);
    /* 00488af1 */
    f->r[5]=mi_read_00488840((0x0000007cu+f->r[4]),4u);
mi_00488af5:;
    /* 00488af5 */
    f->r[1]=mi_read_00488840((0x00000010u+f->r[4]),4u);
    /* 00488af9 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00488afb */
    if((f->flags&MI_ZF))goto mi_00488bda;
    /* 00488b01 */
    f->r[2]=mi_read_00488840((0x0000001cu+f->r[4]),4u);
    /* 00488b05 */
    f->r[0]=mi_read_00488840((0x00000014u+f->r[4]),4u);
    /* 00488b09 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 00488b0b */
    if(!(f->flags&MI_ZF))goto mi_00488bda;
    /* 00488b11 */
    f->r[0]=mi_read_00488840((0x0078d79cu),4u);
    /* 00488b16 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00488b18 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00488bda;
    /* 00488b1e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00488b20 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00488b21 */
    if((f->flags&MI_ZF))goto mi_00488b65;
    /* 00488b23 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00488b24 */
    if((f->flags&MI_ZF))goto mi_00488b45;
    /* 00488b26 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00488b27 */
    if(!(f->flags&MI_ZF))goto mi_00488b86;
    /* 00488b29 */
    f->r[1]=mi_read_00488840((0x00000080u+f->r[4]),4u);
    /* 00488b30 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read_00488840((0x00acf85cu+f->r[1]),1u))&0xffu)<<0);
    /* 00488b36 */
    f->r[1]=0x00000007u;
    /* 00488b3b */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 00488b3c */
    a=f->r[1];mi_idiv32(f,a);
    /* 00488b3e */
    f->r[0]=mi_read_00488840((0x00f560acu),4u);
    /* 00488b43 */
    goto mi_00488b7c;
mi_00488b45:;
    /* 00488b45 */
    f->r[1]=mi_read_00488840((0x00000080u+f->r[4]),4u);
    /* 00488b4c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00488b4e */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read_00488840((0x00acf85cu+f->r[1]),1u))&0xffu)<<0);
    /* 00488b54 */
    f->r[1]=0x00000007u;
    /* 00488b59 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00488b5b */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 00488b5c */
    a=f->r[1];mi_idiv32(f,a);
    /* 00488b5e */
    f->r[0]=mi_read_00488840((0x00f5609cu),4u);
    /* 00488b63 */
    goto mi_00488b7c;
mi_00488b65:;
    /* 00488b65 */
    f->r[2]=mi_read_00488840((0x0078d7ccu),4u);
    /* 00488b6b */
    f->r[0]=mi_read_00488840((0x0000006cu+f->r[4]),4u);
    /* 00488b6f */
    a=f->r[2];b=0x00000003u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 00488b72 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00488b74 */
    f->r[0]=mi_read_00488840((0x00f560a4u),4u);
    /* 00488b79 */
    a=f->r[2];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
mi_00488b7c:;
    /* 00488b7c */
    f->r[1]=mi_read_00488840((0x00000018u+f->r[4]),4u);
    /* 00488b80 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00488b82 */
    f->r[0]=(0xfffffff8u+f->r[0]+f->r[1]*8u);
mi_00488b86:;
    /* 00488b86 */
    f->r[5]=mi_read_00488840((0x0126517cu),4u);
    /* 00488b8c */
    f->r[1]=f->r[0];
    /* 00488b8e */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 00488b90 */
    if(mi_signed_less(f->flags))goto mi_00488b94;
    /* 00488b92 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_00488b94:;
    /* 00488b94 */
    f->r[2]=mi_read_00488840((0x0070e45cu),4u);
    /* 00488b9a */
    a=f->r[1];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00488b9d */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read_00488840((0x010d6a1au+f->r[1]),2u);
    /* 00488ba4 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00488ba6 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 00488ba8 */
    f->r[1]=f->r[0];
    /* 00488baa */
    if(mi_signed_less(f->flags))goto mi_00488bae;
    /* 00488bac */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_00488bae:;
    /* 00488bae */
    a=f->r[1];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00488bb1 */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00488bb3 */
    f->r[5]=(uint32_t)(int32_t)(int16_t)mi_read_00488840((0x010d6a1cu+f->r[1]),2u);
    /* 00488bba */
    f->r[1]=mi_read_00488840((0x0070e44cu),4u);
    /* 00488bc0 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 00488bc3 */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00488bc5 */
    f->r[1]=(0xfffffffbu+f->r[1]+f->r[6]*1u);
    /* 00488bc9 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00488bca */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00488bcb */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00488bcc */
    f->r[1]=0x00f55bb0u;
    /* 00488bd1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00488bd6u;f->boundary(0x00488bd1u,0x0053e590u,f);
    /* 00488bd6 */
    f->r[5]=mi_read_00488840((0x0000007cu+f->r[4]),4u);
mi_00488bda:;
    /* 00488bda */
    f->r[0]=mi_read_00488840((0x0000001cu+f->r[4]),4u);
    /* 00488bde */
    a=f->r[3];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 00488be1 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00488be2 */
    a=f->r[0];b=0x00000009u;t=mi_alu(f,a,b,32,1);
    /* 00488be5 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00488be9 */
    if(mi_signed_less(f->flags))goto mi_00488a7c;
    /* 00488bef */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00488bf0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00488bf1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00488bf2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00488bf3 */
    a=f->r[4];b=0x00000058u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00488bf6 */
    f->r[4]+=28;return;
}

static const uint8_t mi_rom_00488c20_00488db8[]={0xf7,0x8c,0x48,0x00,0xfb,0x8c,0x48,0x00,0x09,0x8d,0x48,0x00,0x10,0x8d,0x48,0x00,0x17,0x8d,0x48,0x00,0x1e,0x8d,0x48,0x00,0x2c,0x8d,0x48,0x00,0x25,0x8d,0x48,0x00,0x02,0x8d,0x48,0x00,0x31,0x8d,0x48,0x00};
static const uint8_t mi_rom_00488c20_00488de0[]={0x00,0x01,0x02,0x03,0x04,0x05,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x06,0x07,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x09,0x08};
static __inline uint32_t mi_read_00488c20(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x00488db8u&&width<=40u&&(offset=address-0x00488db8u)<=40u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00488c20_00488db8[offset+k]<<(k*8u);return result;
 }
 if(address>=0x00488de0u&&width<=125u&&(offset=address-0x00488de0u)<=125u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00488c20_00488de0[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}
void restore_building_refresh_C_00488c20(struct restore_building_refresh_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    MRFloat fp[8];uint32_t fpn=0;uint16_t cw=mr_fp_control_word();(void)cw;
    /* 00488c20 */
    a=f->r[4];b=0x00000028u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 00488c23 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00488c24 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00488c25 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00488c26 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00488c27 */
    f->r[7]=mi_read_00488c20((0x0000003cu+f->r[4]),4u);
    /* 00488c2b */
    f->r[0]=0x00000180u;
    /* 00488c30 */
    f->r[6]=f->r[7];
    /* 00488c32 */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00488c36 */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 00488c39 */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00488c3b */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00488c3f */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 00488c42 */
    MI_U32((0x00000030u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00488c46 */
    f->r[3]=f->r[1];
    /* 00488c48 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read_00488c20((0x007999e8u+f->r[6]),2u))&0xffffu)<<0);
    /* 00488c4f */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00000171u);
    /* 00488c57 */
    a=(f->r[0]&0xffffu);b=0x000000c3u;t=mi_alu(f,a,b,16,1);
    /* 00488c5b */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(0x000001cfu);
    /* 00488c63 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(0x00000145u);
    /* 00488c6b */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(0x0000011au);
    /* 00488c73 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(0x00000142u);
    /* 00488c7b */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(0x00000144u);
    /* 00488c83 */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(0x000002cbu);
    /* 00488c8b */
    if(!(f->flags&MI_ZF))goto mi_00488cb6;
    /* 00488c8d */
    f->r[0]=mi_read_00488c20((0x00f564dcu),4u);
    /* 00488c92 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00488c94 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00488c95 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00488c97 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
mi_00488c99:;
    /* 00488c99 */
    f->r[1]=mi_read_00488c20((0x00000054u+f->r[4]),4u);
    /* 00488c9d */
    f->r[2]=mi_read_00488c20((0x00000050u+f->r[4]),4u);
    /* 00488ca1 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00488ca2 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00488ca3 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00488ca4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00488ca9u;f->boundary(0x00488ca4u,0x0047bf50u,f);
    /* 00488ca9 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00488cac */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00488cad */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00488cae */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00488caf */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00488cb0 */
    a=f->r[4];b=0x00000028u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00488cb3 */
    f->r[4]+=20;return;
mi_00488cb6:;
    /* 00488cb6 */
    a=(f->r[0]&0xffffu);b=0x000000c2u;t=mi_alu(f,a,b,16,1);
    /* 00488cba */
    if(!(f->flags&MI_ZF))goto mi_00488cca;
    /* 00488cbc */
    f->r[0]=mi_read_00488c20((0x00f56764u),4u);
    /* 00488cc1 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00488cc3 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00488cc4 */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00488cc6 */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00488cc8 */
    goto mi_00488c99;
mi_00488cca:;
    /* 00488cca */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00488ccb */
    f->r[1]=f->r[3];
    /* 00488ccd */
    a=f->r[5];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[5]=t;
    /* 00488cd0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00488cd5u;f->boundary(0x00488cd0u,0x00488030u,f);
    /* 00488cd5 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00488cd7 */
    if((f->flags&MI_ZF))goto mi_00488d31;
    /* 00488cd9 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read_00488c20((0x007999e8u+f->r[6]),2u);
    /* 00488ce0 */
    a=f->r[0];b=0xffffff9cu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00488ce3 */
    a=f->r[0];b=0x0000007cu;t=mi_alu(f,a,b,32,1);
    /* 00488ce6 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00488d31;
    /* 00488ce8 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 00488cea */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read_00488c20((0x00488de0u+f->r[0]),1u))&0xffu)<<0);
    /* 00488cf0 */
    switch(mi_read_00488c20((0x00488db8u+f->r[1]*4u),4u)){case 0x00488cf7u:goto mi_00488cf7;case 0x00488cfbu:goto mi_00488cfb;case 0x00488d02u:goto mi_00488d02;case 0x00488d09u:goto mi_00488d09;case 0x00488d10u:goto mi_00488d10;case 0x00488d17u:goto mi_00488d17;case 0x00488d1eu:goto mi_00488d1e;case 0x00488d25u:goto mi_00488d25;case 0x00488d2cu:goto mi_00488d2c;case 0x00488d31u:goto mi_00488d31;default:abort();}
mi_00488cf7:;
    /* 00488cf7 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 00488cf9 */
    goto mi_00488d31;
mi_00488cfb:;
    /* 00488cfb */
    f->r[5]=0x00000001u;
    /* 00488d00 */
    goto mi_00488d31;
mi_00488d02:;
    /* 00488d02 */
    f->r[5]=0x00000009u;
    /* 00488d07 */
    goto mi_00488d31;
mi_00488d09:;
    /* 00488d09 */
    f->r[5]=0x00000002u;
    /* 00488d0e */
    goto mi_00488d31;
mi_00488d10:;
    /* 00488d10 */
    f->r[5]=0x00000003u;
    /* 00488d15 */
    goto mi_00488d31;
mi_00488d17:;
    /* 00488d17 */
    f->r[5]=0x00000004u;
    /* 00488d1c */
    goto mi_00488d31;
mi_00488d1e:;
    /* 00488d1e */
    f->r[5]=0x00000005u;
    /* 00488d23 */
    goto mi_00488d31;
mi_00488d25:;
    /* 00488d25 */
    f->r[5]=0x00000007u;
    /* 00488d2a */
    goto mi_00488d31;
mi_00488d2c:;
    /* 00488d2c */
    f->r[5]=0x00000008u;
mi_00488d31:;
    /* 00488d31 */
    f->r[2]=mi_read_00488c20((0x00000010u+f->r[4]+f->r[5]*4u),4u);
    /* 00488d35 */
    fp[fpn++]=mr_fp_binary64(MI_U64((0x0056f5a8u)));
    /* 00488d3b */
    f->r[5]=mi_read_00488c20((0x00f55bb4u+f->r[2]*8u),4u);
    /* 00488d42 */
    f->r[6]=(uint32_t)(int32_t)(int16_t)mi_read_00488c20((0x00799a2cu+f->r[6]),2u);
    /* 00488d49 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00488d4eu;{uint8_t raw[10];uint64_t result;mr_fp_raw80(fp[--fpn],raw);result=restore_map_trunc80(raw);f->r[0]=(uint32_t)result;f->r[2]=(uint32_t)(result>>32);}f->r[4]+=4;
    /* 00488d4e */
    f->r[1]=f->r[0];
    /* 00488d50 */
    f->r[0]=f->r[6];
    /* 00488d52 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 00488d53 */
    a=f->r[1];mi_idiv32(f,a);
    /* 00488d55 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00488d56 */
    f->r[1]=f->r[3];
    /* 00488d58 */
    f->r[6]=f->r[0];
    /* 00488d5a */
    f->r[4]-=4;MI_U32(f->r[4])=0x00488d5fu;f->boundary(0x00488d5au,0x004894a0u,f);
    /* 00488d5f */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00488d61 */
    if((f->flags&MI_ZF))goto mi_00488d92;
    /* 00488d63 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00488d64 */
    f->r[1]=f->r[3];
    /* 00488d66 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00488d6bu;f->boundary(0x00488d66u,0x004894d0u,f);
    /* 00488d6b */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00488d6d */
    if(!(f->flags&MI_ZF))goto mi_00488dab;
    /* 00488d6f */
    f->r[2]=mi_read_00488c20((0x00000048u+f->r[4]),4u);
    /* 00488d73 */
    f->r[0]=mi_read_00488c20((0x00000044u+f->r[4]),4u);
    /* 00488d77 */
    f->r[1]=mi_read_00488c20((0x00000040u+f->r[4]),4u);
    /* 00488d7b */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00488d7c */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00488d7d */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00488d7e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00488d7f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00488d80 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00488d81 */
    f->r[1]=f->r[3];
    /* 00488d83 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00488d88u;f->boundary(0x00488d83u,0x00488840u,f);
    /* 00488d88 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00488d89 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00488d8a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00488d8b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00488d8c */
    a=f->r[4];b=0x00000028u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00488d8f */
    f->r[4]+=20;return;
mi_00488d92:;
    /* 00488d92 */
    f->r[2]=mi_read_00488c20((0x00000048u+f->r[4]),4u);
    /* 00488d96 */
    f->r[0]=mi_read_00488c20((0x00000044u+f->r[4]),4u);
    /* 00488d9a */
    f->r[1]=mi_read_00488c20((0x00000040u+f->r[4]),4u);
    /* 00488d9e */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00488d9f */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00488da0 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00488da1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00488da2 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00488da3 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00488da4 */
    f->r[1]=f->r[3];
    /* 00488da6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00488dabu;f->boundary(0x00488da6u,0x00488690u,f);
mi_00488dab:;
    /* 00488dab */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00488dac */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00488dad */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00488dae */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00488daf */
    a=f->r[4];b=0x00000028u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00488db2 */
    f->r[4]+=20;return;
}

void restore_building_refresh_C_00488e60(struct restore_building_refresh_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00488e60 */
    f->r[2]=MI_U32((0x00000004u+f->r[4]));
    /* 00488e64 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00488e65 */
    f->r[6]=MI_U32((0x0000000cu+f->r[4]));
    /* 00488e69 */
    f->r[0]=f->r[2];
    /* 00488e6b */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00488e6e */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00488e70 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00488e71 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00488e74 */
    f->r[6]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[0]));
    /* 00488e7b */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[0]));
    /* 00488e82 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00488e83 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00488e84 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00488e85 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00488e8au;f->boundary(0x00488e85u,0x00488c20u,f);
    /* 00488e8a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00488e8b */
    f->r[4]+=12;return;
}

void restore_building_refresh_C_004894d0(struct restore_building_refresh_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004894d0 */
    f->r[1]=MI_U32((0x00000004u+f->r[4]));
    /* 004894d4 */
    a=0x00000004u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004894d6 */
    f->r[0]=f->r[1];
    /* 004894d8 */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004894da */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004894dd */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004894df */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004894e1 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004894e4 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[0]));
    /* 004894eb */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[0]));
    /* 004894f2 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004894f3 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004894f4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004894f9u;f->boundary(0x004894f4u,0x004893c0u,f);
    /* 004894f9 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004894fc */
    f->r[4]+=8;return;
}

void restore_building_refresh_C_004df9d0(struct restore_building_refresh_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004df9d0 */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 004df9d3 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000048u+f->r[4])))&0xffu)<<0);
    /* 004df9d7 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004df9d8 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004df9d9 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004df9da */
    f->r[6]=MI_U32((0x00000050u+f->r[4]));
    /* 004df9de */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004df9df */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004df9e1 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 004df9e9 */
    f->r[5]=f->r[6];
    /* 004df9eb */
    if(!(f->flags&MI_ZF))goto mi_004dfa01;
    /* 004df9ed */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004df9ee */
    f->r[4]-=4;MI_U32(f->r[4])=0x004df9f3u;f->boundary(0x004df9eeu,0x004f4f80u,f);
    /* 004df9f3 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004df9f7 */
    f->r[0]=MI_U32((0x00bca2e0u));
    /* 004df9fc */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004df9ff */
    goto mi_004dfa0e;
mi_004dfa01:;
    /* 004dfa01 */
    f->r[0]=MI_U32((0x00702798u));
    /* 004dfa06 */
    a=f->r[0];b=0x0000ffffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004dfa0b */
    a=f->r[0];t=a>>13u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>12u)&1u)|mi_szp(t,32);f->r[0]=t;
mi_004dfa0e:;
    /* 004dfa0e */
    f->r[2]=MI_U32((0x00bca2e0u));
    /* 004dfa14 */
    f->r[1]=f->r[0];
    /* 004dfa16 */
    MI_U32((0x00000058u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004dfa1a */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfa1b */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfa1c */
    f->r[0]=(0x0000001cu+f->r[4]);
    /* 004dfa20 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfa22 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfa23 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dfa28u;f->boundary(0x004dfa23u,0x004ded50u,f);
    /* 004dfa28 */
    f->r[3]=MI_U32((0x0000005cu+f->r[4]));
    /* 004dfa2c */
    f->r[1]=MI_U32((0x00000024u+f->r[4]));
    /* 004dfa30 */
    f->r[0]=f->r[6];
    /* 004dfa32 */
    f->r[7]=MI_U32((0x00000060u+f->r[4]));
    /* 004dfa36 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004dfa39 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004dfa3b */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004dfa3d */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004dfa40 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfa41 */
    MI_U16((0x007999deu+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004dfa48 */
    f->r[1]=MI_U32((0x0000002cu+f->r[4]));
    /* 004dfa4c */
    f->r[2]=(0x00000000u+f->r[1]+f->r[7]*1u);
    /* 004dfa4f */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004dfa51 */
    MI_U16((0x007999e0u+f->r[0]))=(uint16_t)((f->r[2]&0xffffu));
    /* 004dfa58 */
    f->r[2]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 004dfa5b */
    f->r[1]=(0x00000000u+f->r[1]+f->r[2]*2u);
    /* 004dfa5e */
    f->r[2]=MI_U32((0x00bca2d8u));
    /* 004dfa64 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 004dfa67 */
    f->r[1]=(0x00000000u+f->r[2]+f->r[1]*4u);
    /* 004dfa6a */
    f->r[2]=MI_U32((0x00000028u+f->r[4]));
    /* 004dfa6e */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004dfa70 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00000030u+f->r[4])))&0xffu)<<0);
    /* 004dfa74 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004dfa76 */
    MI_U32((0x007999e4u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 004dfa7c */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004dfa7e */
    MI_U16((0x007999eau+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004dfa85 */
    MI_U8((0x007999dbu+f->r[0]))=(uint8_t)((f->r[2]&0xffu));
    /* 004dfa8b */
    f->r[2]=MI_U32((0x00000024u+f->r[4]));
    /* 004dfa8f */
    MI_U16((0x00799a10u+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004dfa96 */
    MI_U32((0x00799a2cu+f->r[0]))=(uint32_t)(f->r[2]);
    /* 004dfa9c */
    MI_U32((0x00799a30u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 004dfaa2 */
    MI_U32((0x00799a38u+f->r[0]))=(uint32_t)(0x00000001u);
    /* 004dfaac */
    MI_U32((0x00799a34u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 004dfab2 */
    f->r[1]=MI_U32((0x0000006cu+f->r[4]));
    /* 004dfab6 */
    MI_U32((0x00799a40u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 004dfabc */
    MI_U32((0x00799a44u+f->r[0]))=(uint32_t)(f->r[7]);
    /* 004dfac2 */
    MI_U32((0x00799a48u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 004dfac8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dfacdu;f->boundary(0x004dfac8u,0x004dfbb0u,f);
    /* 004dfacd */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dface */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dfad3u;f->boundary(0x004dfaceu,0x00485100u,f);
    /* 004dfad3 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dfad6 */
    f->r[7]=0x00000001u;
mi_004dfadb:;
    /* 004dfadb */
    f->r[2]=MI_U32((0x00000058u+f->r[4]));
    /* 004dfadf */
    f->r[0]=MI_U32((0x00bca2e0u));
    /* 004dfae4 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfae5 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfae6 */
    f->r[1]=(0x0000001cu+f->r[4]);
    /* 004dfaea */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfaeb */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfaec */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dfaf1u;f->boundary(0x004dfaecu,0x004ded50u,f);
    /* 004dfaf1 */
    f->r[2]=MI_U32((0x00000028u+f->r[4]));
    /* 004dfaf5 */
    f->r[0]=MI_U32((0x00000060u+f->r[4]));
    /* 004dfaf9 */
    f->r[1]=MI_U32((0x00000024u+f->r[4]));
    /* 004dfafd */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 004dfafe */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfaff */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004dfb01 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfb03 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004dfb05 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfb06 */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 004dfb0c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfb0d */
    a=0x000000deu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfb12 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfb13 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dfb18u;f->boundary(0x004dfb13u,0x0046a400u,f);
    /* 004dfb18 */
    f->r[6]=f->r[0];
    /* 004dfb1a */
    f->r[2]=f->r[5];
    /* 004dfb1c */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004dfb1f */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004dfb21 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004dfb23 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004dfb26 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004dfb29 */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004dfb2b */
    MI_U16((0x00799a10u+f->r[0]))=(uint16_t)((f->r[5]&0xffffu));
    /* 004dfb32 */
    MI_U16((0x00799a12u+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004dfb39 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfb3a */
    MI_U16((0x00799a12u+f->r[2]*8u))=(uint16_t)((f->r[6]&0xffffu));
    /* 004dfb42 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00000048u+f->r[4])))&0xffu)<<0);
    /* 004dfb46 */
    MI_U16((0x007999eau+f->r[0]))=(uint16_t)((f->r[7]&0xffffu));
    /* 004dfb4d */
    MI_U8((0x007999dbu+f->r[0]))=(uint8_t)((f->r[2]&0xffu));
    /* 004dfb53 */
    f->r[2]=MI_U32((0x0000003cu+f->r[4]));
    /* 004dfb57 */
    f->r[5]=f->r[6];
    /* 004dfb59 */
    MI_U32((0x00799a2cu+f->r[0]))=(uint32_t)(f->r[2]);
    /* 004dfb5f */
    f->r[2]=MI_U32((0x00000084u+f->r[4]));
    /* 004dfb66 */
    MI_U32((0x00799a30u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 004dfb6c */
    MI_U32((0x00799a38u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 004dfb72 */
    MI_U32((0x00799a34u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 004dfb78 */
    f->r[1]=MI_U32((0x0000007cu+f->r[4]));
    /* 004dfb7c */
    MI_U32((0x00799a40u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 004dfb82 */
    MI_U32((0x00799a44u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 004dfb88 */
    MI_U32((0x00799a48u+f->r[0]))=(uint32_t)(f->r[2]);
    /* 004dfb8e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dfb93u;f->boundary(0x004dfb8eu,0x004dfbb0u,f);
    /* 004dfb93 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfb94 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dfb99u;f->boundary(0x004dfb94u,0x00485100u,f);
    /* 004dfb99 */
    a=f->r[4];b=0x00000030u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dfb9c */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 004dfb9d */
    a=f->r[7];b=0x00000074u;t=mi_alu(f,a,b,32,1);
    /* 004dfba0 */
    if(mi_signed_less(f->flags))goto mi_004dfadb;
    /* 004dfba6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dfba7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dfba8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dfba9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dfbaa */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dfbad */
    f->r[4]+=4;return;
}

void restore_building_refresh_C_004dfbb0(struct restore_building_refresh_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004dfbb0 */
    a=f->r[4];b=0x0000004cu;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 004dfbb3 */
    f->r[1]=MI_U32((0x00bca2e0u));
    /* 004dfbb9 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfbba */
    f->r[3]=MI_U32((0x00000054u+f->r[4]));
    /* 004dfbbe */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfbbf */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfbc0 */
    f->r[6]=f->r[3];
    /* 004dfbc2 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfbc3 */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004dfbc6 */
    a=f->r[6];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004dfbc8 */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004dfbcb */
    f->r[0]=MI_U32((0x00799a48u+f->r[6]));
    /* 004dfbd1 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999eau+f->r[6]));
    /* 004dfbd8 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfbd9 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfbda */
    f->r[0]=(0x00000030u+f->r[4]);
    /* 004dfbde */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfbdf */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfbe0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dfbe5u;f->boundary(0x004dfbe0u,0x004ded50u,f);
    /* 004dfbe5 */
    f->r[1]=(0x0000002cu+f->r[4]);
    /* 004dfbe9 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfbea */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfbeb */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dfbf0u;f->boundary(0x004dfbebu,0x004df360u,f);
    /* 004dfbf0 */
    f->r[7]=f->r[0];
    /* 004dfbf2 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000034u+f->r[4])))&0xffu)<<0);
    /* 004dfbf6 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dfbf9 */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,3);
    /* 004dfbfb */
    if((f->flags&MI_ZF))goto mi_004dfd5d;
    /* 004dfc01 */
    f->r[3]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[6]));
    /* 004dfc08 */
    f->r[1]=MI_U32((0x00bca2d8u));
    /* 004dfc0e */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 004dfc12 */
    f->r[5]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[6]));
    /* 004dfc19 */
    f->r[2]=(0x00000000u+f->r[3]+f->r[3]*8u);
    /* 004dfc1c */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 004dfc20 */
    f->r[0]=(0x00000000u+f->r[3]+f->r[2]*2u);
    /* 004dfc23 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004dfc26 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[0]*4u);
    /* 004dfc29 */
    f->r[1]=MI_U32((0x00000030u+f->r[4]));
    /* 004dfc2d */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004dfc2f */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004dfc31 */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[0]);
    /* 004dfc36 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004dfd27;
    /* 004dfc3c */
    f->r[5]=0x005cc8c0u;
    /* 004dfc41 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004dfc45 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 004dfc49 */
    goto mi_004dfc4f;
mi_004dfc4b:;
    /* 004dfc4b */
    f->r[1]=MI_U32((0x00000030u+f->r[4]));
mi_004dfc4f:;
    /* 004dfc4f */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[1]);
mi_004dfc53:;
    /* 004dfc53 */
    f->r[1]=MI_U32((0x00000000u+f->r[5]));
    /* 004dfc56 */
    f->r[3]=(f->r[3]&0xffff0000u)|(((MI_U16((0x00000060u+f->r[4])))&0xffffu)<<0);
    /* 004dfc5b */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004dfc5d */
    f->r[6]=MI_U32((0x00af598cu+f->r[1]*4u));
    /* 004dfc64 */
    MI_U16((0x00b4e6fcu+f->r[1]*2u))=(uint16_t)((f->r[3]&0xffffu));
    /* 004dfc6c */
    f->r[3]=(f->r[3]&0xffffff00u)|(((MI_U8((0x00b285ccu+f->r[1])))&0xffu)<<0);
    /* 004dfc72 */
    f->r[2]=(0x00af598cu+f->r[1]*4u);
    /* 004dfc79 */
    a=(f->r[3]&0xffu);b=0x000000e0u;t=mi_alu(f,a,b,8,3);f->r[3]=(f->r[3]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004dfc7c */
    a=f->r[6];b=0x10000000u;t=mi_alu(f,a,b,32,4);f->r[6]=t;
    /* 004dfc82 */
    MI_U8((0x00b285ccu+f->r[1]))=(uint8_t)((f->r[3]&0xffu));
    /* 004dfc88 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((MI_U8((0x00b67d1cu+f->r[1])))&0xffu)<<0);
    /* 004dfc8e */
    a=(f->r[3]&0xffu);b=0x00000040u;t=mi_alu(f,a,b,8,4);f->r[3]=(f->r[3]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004dfc91 */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[6]);
    /* 004dfc93 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 004dfc95 */
    MI_U8((0x00b67d1cu+f->r[1]))=(uint8_t)((f->r[3]&0xffu));
    /* 004dfc9b */
    if((f->flags&MI_ZF))goto mi_004dfce4;
    /* 004dfc9d */
    a=f->r[7];b=MI_U32((0x00f56b54u));t=mi_alu(f,a,b,32,1);
    /* 004dfca3 */
    if(!(f->flags&MI_ZF))goto mi_004dfcb9;
    /* 004dfca5 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((MI_U8((0x00acf85cu+f->r[1])))&0xffu)<<0);
    /* 004dfcab */
    a=f->r[3];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[3]=t;
    /* 004dfcae */
    a=f->r[3];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 004dfcb0 */
    MI_U32((0x00b7482cu+f->r[1]*4u))=(uint32_t)(f->r[3]);
    /* 004dfcb7 */
    goto mi_004dfcc0;
mi_004dfcb9:;
    /* 004dfcb9 */
    MI_U32((0x00b7482cu+f->r[1]*4u))=(uint32_t)(f->r[7]);
mi_004dfcc0:;
    /* 004dfcc0 */
    a=f->r[7];b=MI_U32((0x00f57464u));t=mi_alu(f,a,b,32,1);
    /* 004dfcc6 */
    if((f->flags&MI_ZF))goto mi_004dfcda;
    /* 004dfcc8 */
    a=f->r[7];b=MI_U32((0x00f5736cu));t=mi_alu(f,a,b,32,1);
    /* 004dfcce */
    if((f->flags&MI_ZF))goto mi_004dfcda;
    /* 004dfcd0 */
    a=f->r[6];b=0xfefed77eu;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 004dfcd6 */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[6]);
    /* 004dfcd8 */
    goto mi_004dfcef;
mi_004dfcda:;
    /* 004dfcda */
    a=f->r[6];b=0xfefed7feu;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 004dfce0 */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[6]);
    /* 004dfce2 */
    goto mi_004dfcef;
mi_004dfce4:;
    /* 004dfce4 */
    MI_U32((0x00b7482cu+f->r[1]*4u))=(uint32_t)(0x00000000u);
mi_004dfcef:;
    /* 004dfcef */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
    /* 004dfcf3 */
    a=f->r[5];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 004dfcf6 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004dfcf7 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004dfcfb */
    if(!(f->flags&MI_ZF))goto mi_004dfc53;
    /* 004dfd01 */
    f->r[5]=MI_U32((0x00000014u+f->r[4]));
    /* 004dfd05 */
    f->r[1]=MI_U32((0x00000018u+f->r[4]));
    /* 004dfd09 */
    a=f->r[5];b=0x00000030u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 004dfd0c */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004dfd0d */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 004dfd11 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004dfd15 */
    if(!(f->flags&MI_ZF))goto mi_004dfc4b;
    /* 004dfd1b */
    f->r[3]=MI_U32((0x00000020u+f->r[4]));
    /* 004dfd1f */
    f->r[5]=MI_U32((0x00000024u+f->r[4]));
    /* 004dfd23 */
    f->r[1]=MI_U32((0x00000030u+f->r[4]));
mi_004dfd27:;
    /* 004dfd27 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 004dfd29 */
    if(!(f->flags&MI_ZF))goto mi_004dfd7f;
    /* 004dfd2b */
    f->r[6]=(0xffffffffu+f->r[3]+f->r[1]*1u);
    /* 004dfd2f */
    f->r[7]=(0xffffffffu+f->r[1]+f->r[5]*1u);
    /* 004dfd33 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfd34 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfd35 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfd36 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfd37 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dfd3cu;f->boundary(0x004dfd37u,0x00475bf0u,f);
    /* 004dfd3c */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfd3e */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfd40 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfd41 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfd42 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfd43 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfd44 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dfd49u;f->boundary(0x004dfd44u,0x004767d0u,f);
    /* 004dfd49 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfd4a */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfd4b */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfd4c */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfd4d */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dfd52u;f->boundary(0x004dfd4du,0x00477500u,f);
    /* 004dfd52 */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dfd55 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dfd56 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dfd57 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dfd58 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dfd59 */
    a=f->r[4];b=0x0000004cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dfd5c */
    f->r[4]+=4;return;
mi_004dfd5d:;
    /* 004dfd5d */
    f->r[0]=MI_U32((0x00000030u+f->r[4]));
    /* 004dfd61 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfd63 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[6]));
    /* 004dfd6a */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfd6b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfd6c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfd6d */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfd6e */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[6]));
    /* 004dfd75 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfd76 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfd77 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dfd7cu;f->boundary(0x004dfd77u,0x0047bf50u,f);
    /* 004dfd7c */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004dfd7f:;
    /* 004dfd7f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dfd80 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dfd81 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dfd82 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dfd83 */
    a=f->r[4];b=0x0000004cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dfd86 */
    f->r[4]+=4;return;
}

void restore_building_refresh_C_004e57f0(struct restore_building_refresh_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004e57f0 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e57f1 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e57f2 */
    f->r[7]=MI_U32((0x0000000cu+f->r[4]));
    /* 004e57f6 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e57f7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e57fcu;f->boundary(0x004e57f7u,0x004f4f80u,f);
    /* 004e57fc */
    f->r[6]=f->r[7];
    /* 004e57fe */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e57ff */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004e5802 */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004e5804 */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004e5807 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e580cu;f->boundary(0x004e5807u,0x004f4aa0u,f);
    /* 004e580c */
    MI_U16((0x007999eau+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 004e5813 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e5814 */
    MI_U8((0x00799a2du+f->r[6]))=(uint8_t)(0x00000021u);
    /* 004e581b */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e5820u;f->boundary(0x004e581bu,0x004e5b90u,f);
    /* 004e5820 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e5823 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 004e5825 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004e5826 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e5827 */
    f->r[4]+=4;return;
}

void restore_building_refresh_C_004e5840(struct restore_building_refresh_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004e5840 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e5841 */
    f->r[0]=MI_U32((0x00000008u+f->r[4]));
    /* 004e5845 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e5846 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e5847 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e5848 */
    f->r[6]=f->r[0];
    /* 004e584a */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e584b */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004e584e */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004e5850 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004e5852 */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004e5855 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00799a2du+f->r[6])))&0xffu)<<0);
    /* 004e585b */
    f->r[7]=(f->r[7]&0xffff0000u)|(((MI_U16((0x007999e8u+f->r[6])))&0xffffu)<<0);
    /* 004e5862 */
    f->r[5]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999eau+f->r[6]));
    /* 004e5869 */
    a=f->r[0];b=0x00000042u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004e586c */
    a=f->r[5];b=0x00000005u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 004e586f */
    f->r[1]=(f->r[1]&0xffff0000u)|((((f->r[7]&0xffffu))&0xffffu)<<0);
    /* 004e5872 */
    f->r[3]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004e5875 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004e5877 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 004e5879 */
    t=((f->flags&MI_ZF)||mi_signed_less(f->flags))?1u:0u;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004e587c */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004e587d */
    a=(f->r[1]&0xffffu);b=0x000000cfu;t=mi_alu(f,a,b,16,1);f->r[1]=(f->r[1]&0xffff0000u)|(((t)&0xffffu)<<0);
    /* 004e5882 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004e5884 */
    a=(f->r[1]&0xffffu);b=f->flags&MI_CF;t=mi_alu(f,0,a,16,1);f->r[1]=(f->r[1]&0xffff0000u)|(((t)&0xffffu)<<0);
    /* 004e5887 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,2);f->r[1]=t;
    /* 004e5889 */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 004e588c */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004e588f */
    f->r[2]=(0xffffffffu+f->r[1]);
    /* 004e5892 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004e5896 */
    f->r[2]=f->r[1];
    /* 004e5898 */
    a=f->r[2];b=f->r[0];t=mi_imul(f,a,b);f->r[2]=t;
    /* 004e589b */
    f->r[0]=0x51eb851fu;
    /* 004e58a0 */
    a=f->r[0];b=f->r[2];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 004e58a2 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 004e58a5 */
    f->r[0]=f->r[2];
    /* 004e58a7 */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 004e58aa */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004e58ac */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 004e58b0 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004e58b2 */
    if(!mi_signed_less(f->flags))goto mi_004e58d4;
    /* 004e58b4 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004e58b6 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 004e58b8 */
    t=((f->flags&MI_ZF)||mi_signed_less(f->flags))?1u:0u;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004e58bb */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004e58bc */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004e58be */
    a=f->r[1];b=f->r[0];t=mi_imul(f,a,b);f->r[1]=t;
    /* 004e58c1 */
    f->r[0]=0x51eb851fu;
    /* 004e58c6 */
    a=f->r[0];b=f->r[1];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 004e58c8 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 004e58cb */
    f->r[1]=f->r[2];
    /* 004e58cd */
    a=f->r[1];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[1]=t;
    /* 004e58d0 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004e58d2 */
    goto mi_004e58d6;
mi_004e58d4:;
    /* 004e58d4 */
    f->r[2]=f->r[0];
mi_004e58d6:;
    /* 004e58d6 */
    a=(f->r[7]&0xffffu);b=0x000000cfu;t=mi_alu(f,a,b,16,1);f->r[7]=(f->r[7]&0xffff0000u)|(((t)&0xffffu)<<0);
    /* 004e58db */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e58dc */
    a=(f->r[7]&0xffffu);b=f->flags&MI_CF;t=mi_alu(f,0,a,16,1);f->r[7]=(f->r[7]&0xffff0000u)|(((t)&0xffffu)<<0);
    /* 004e58df */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,2);f->r[7]=t;
    /* 004e58e1 */
    f->r[1]=0x00f55bb0u;
    /* 004e58e6 */
    a=f->r[7];b=0x00000004u;t=mi_alu(f,a,b,32,3);f->r[7]=t;
    /* 004e58e9 */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004e58eb */
    f->r[2]=MI_U32((0x005e4588u+f->r[7]*4u));
    /* 004e58f2 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e58f3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e58f8u;f->boundary(0x004e58f3u,0x0053dd90u,f);
    /* 004e58f8 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999eau+f->r[6]));
    /* 004e58ff */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004e5901 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004e5903 */
    if((f->flags&MI_ZF))goto mi_004e591b;
    /* 004e5905 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004e5906 */
    if((f->flags&MI_ZF))goto mi_004e5913;
    /* 004e5908 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004e5909 */
    if(!(f->flags&MI_ZF))goto mi_004e5921;
    /* 004e590b */
    f->r[1]=MI_U32((0x00f56d64u));
    /* 004e5911 */
    goto mi_004e5921;
mi_004e5913:;
    /* 004e5913 */
    f->r[1]=MI_U32((0x00f56d5cu));
    /* 004e5919 */
    goto mi_004e5921;
mi_004e591b:;
    /* 004e591b */
    f->r[1]=MI_U32((0x00f56d54u));
mi_004e5921:;
    /* 004e5921 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004e5923 */
    f->r[2]=MI_U32((0x00000018u+f->r[4]));
    /* 004e5927 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x007999dbu+f->r[6])))&0xffu)<<0);
    /* 004e592d */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e592f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e5930 */
    f->r[1]=MI_U32((0x00000024u+f->r[4]));
    /* 004e5934 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e5935 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e5936 */
    f->r[0]=MI_U32((0x00000030u+f->r[4]));
    /* 004e593a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e593b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e593c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e593d */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e5942u;f->boundary(0x004e593du,0x0047bf50u,f);
    /* 004e5942 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e5945 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004e5946 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e5947 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004e5948 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004e5949 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004e594a */
    f->r[4]+=4;return;
}

void restore_building_refresh_C_004edf50(struct restore_building_refresh_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004edf50 */
    f->r[0]=MI_U32((0x00000004u+f->r[4]));
    /* 004edf54 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 004edf57 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004edf59 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004edf5a */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004edf5b */
    f->r[6]=f->r[0];
    /* 004edf5d */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004edf5e */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004edf61 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004edf63 */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004edf66 */
    MI_U32((0x00799a44u+f->r[6]))=(uint32_t)(f->r[1]);
    /* 004edf6c */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00799a2du+f->r[6])))&0xffu)<<0);
    /* 004edf72 */
    a=(f->r[2]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004edf75 */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_004edf91;
    /* 004edf77 */
    f->r[1]=MI_U32((0x00000020u+f->r[4]));
    /* 004edf7b */
    f->r[2]=MI_U32((0x0000001cu+f->r[4]));
    /* 004edf7f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004edf80 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004edf81 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004edf82 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004edf87u;f->boundary(0x004edf82u,0x004f4160u,f);
    /* 004edf87 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004edf8a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004edf8b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004edf8c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004edf8d */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004edf90 */
    f->r[4]+=4;return;
mi_004edf91:;
    /* 004edf91 */
    f->r[2]=MI_U32((0x005e4ef4u));
    /* 004edf97 */
    f->r[0]=MI_U32((0x00000020u+f->r[4]));
    /* 004edf9b */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004edf9c */
    f->r[5]=(f->r[5]&0xffff0000u)|(((MI_U16((0x007999eau+f->r[6])))&0xffffu)<<0);
    /* 004edfa3 */
    a=f->r[2];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 004edfa6 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 004edfa9 */
    f->r[3]=(uint32_t)(int32_t)(int16_t)(f->r[5]&0xffffu);
    /* 004edfac */
    if(!(f->flags&MI_ZF))goto mi_004edfba;
    /* 004edfae */
    f->r[0]=(0x00000000u+f->r[3]+f->r[0]*4u);
    /* 004edfb1 */
    f->r[0]=MI_U32((0x005e6578u+f->r[0]*8u));
    /* 004edfb8 */
    goto mi_004edfc6;
mi_004edfba:;
    /* 004edfba */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004edfbd */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004edfbf */
    f->r[0]=MI_U32((0x005e4ef8u+f->r[0]*8u));
mi_004edfc6:;
    /* 004edfc6 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 004edfc9 */
    if(!mi_signed_less(f->flags))goto mi_004edfd9;
    /* 004edfcb */
    a=f->r[0];b=MI_U32((0x00000020u+f->r[4]));t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004edfcf */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 004edfd2 */
    if(mi_signed_less(f->flags))goto mi_004edff8;
    /* 004edfd4 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004edfd7 */
    goto mi_004edff8;
mi_004edfd9:;
    /* 004edfd9 */
    a=f->r[0];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 004edfdc */
    if(!mi_signed_less(f->flags))goto mi_004edfec;
    /* 004edfde */
    a=f->r[0];b=MI_U32((0x00000020u+f->r[4]));t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004edfe2 */
    a=f->r[0];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 004edfe5 */
    if(mi_signed_less(f->flags))goto mi_004edff8;
    /* 004edfe7 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004edfea */
    goto mi_004edff8;
mi_004edfec:;
    /* 004edfec */
    a=f->r[0];b=0x00000009u;t=mi_alu(f,a,b,32,1);
    /* 004edfef */
    if(!(f->flags&MI_ZF))goto mi_004edff8;
    /* 004edff1 */
    f->r[0]=MI_U32((0x00000020u+f->r[4]));
    /* 004edff5 */
    a=f->r[0];b=0x00000009u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
mi_004edff8:;
    /* 004edff8 */
    f->r[7]=MI_U32((0x00f57334u));
    /* 004edffe */
    a=f->r[2];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 004ee001 */
    if(!(f->flags&MI_ZF))goto mi_004ee009;
    /* 004ee003 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004ee007 */
    goto mi_004ee017;
mi_004ee009:;
    /* 004ee009 */
    a=f->r[0];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 004ee00c */
    if(!(f->flags&MI_ZF))goto mi_004ee07c;
    /* 004ee00e */
    f->r[1]=(0x0000006du+f->r[7]);
mi_004ee011:;
    /* 004ee011 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[1]);
mi_004ee015:;
    /* 004ee015 */
    f->r[1]=f->r[0];
mi_004ee017:;
    /* 004ee017 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00799a2du+f->r[6])))&0xffu)<<0);
    /* 004ee01d */
    MI_U8((0x00000013u+f->r[4]))=(uint8_t)(0x00000000u);
    /* 004ee022 */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004ee024 */
    if((f->flags&MI_ZF))goto mi_004ee0ec;
    /* 004ee02a */
    f->r[2]=MI_U32((0x005e4ef4u));
    /* 004ee030 */
    a=f->r[2];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 004ee033 */
    if((f->flags&MI_ZF))goto mi_004ee0ec;
    /* 004ee039 */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004ee03c */
    if(!(f->flags&MI_ZF))goto mi_004ee0d3;
    /* 004ee042 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004ee044 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00799a2fu+f->r[6])))&0xffu)<<0);
    /* 004ee04a */
    f->r[3]=f->r[2];
    /* 004ee04c */
    f->r[2]=MI_U32((0x00000024u+f->r[4]));
    /* 004ee050 */
    f->r[2]=(0x00000000u+f->r[2]+f->r[2]*2u);
    /* 004ee053 */
    a=MI_U8((0x005e68d8u+f->r[3]+f->r[2]*2u));b=0x00000000u;t=mi_alu(f,a,b,8,1);
    /* 004ee05b */
    if((f->flags&MI_ZF))goto mi_004ee0d3;
    /* 004ee05d */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004ee05f */
    f->r[7]=0x00000005u;
    /* 004ee064 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00799a2eu+f->r[6])))&0xffu)<<0);
    /* 004ee06a */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 004ee06c */
    f->r[2]=MI_U32((0x00f5738cu));
    /* 004ee072 */
    f->r[7]=(0x00000000u+f->r[2]+f->r[7]*8u);
    /* 004ee075 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004ee077 */
    goto mi_004ee1af;
mi_004ee07c:;
    /* 004ee07c */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 004ee07f */
    if(!(f->flags&MI_ZF))goto mi_004ee097;
    /* 004ee081 */
    a=f->r[2];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 004ee084 */
    if(!(f->flags&MI_ZF))goto mi_004ee08f;
    /* 004ee086 */
    f->r[2]=(0x00000070u+f->r[7]);
    /* 004ee089 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004ee08d */
    goto mi_004ee015;
mi_004ee08f:;
    /* 004ee08f */
    f->r[1]=(0x0000006eu+f->r[7]);
    /* 004ee092 */
    goto mi_004ee011;
mi_004ee097:;
    /* 004ee097 */
    a=f->r[2];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 004ee09a */
    if(!(f->flags&MI_ZF))goto mi_004ee011;
    /* 004ee0a0 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004ee0a3 */
    if(!(f->flags&MI_ZF))goto mi_004ee0b1;
    /* 004ee0a5 */
    f->r[2]=(0x00000073u+f->r[7]);
    /* 004ee0a8 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004ee0ac */
    goto mi_004ee015;
mi_004ee0b1:;
    /* 004ee0b1 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 004ee0b4 */
    if(!(f->flags&MI_ZF))goto mi_004ee0be;
    /* 004ee0b6 */
    f->r[1]=(0x00000077u+f->r[7]);
    /* 004ee0b9 */
    goto mi_004ee011;
mi_004ee0be:;
    /* 004ee0be */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 004ee0c1 */
    if(!(f->flags&MI_ZF))goto mi_004ee011;
    /* 004ee0c7 */
    f->r[2]=(0x0000007bu+f->r[7]);
    /* 004ee0ca */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004ee0ce */
    goto mi_004ee015;
mi_004ee0d3:;
    /* 004ee0d3 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004ee0d5 */
    f->r[3]=0x0000000bu;
    /* 004ee0da */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00799a2eu+f->r[6])))&0xffu)<<0);
    /* 004ee0e0 */
    a=f->r[3];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 004ee0e2 */
    f->r[7]=(0x00000000u+f->r[7]+f->r[3]*8u);
    /* 004ee0e5 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004ee0e7 */
    goto mi_004ee1af;
mi_004ee0ec:;
    /* 004ee0ec */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00799a2eu+f->r[6])))&0xffu)<<0);
    /* 004ee0f2 */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004ee0f4 */
    if(!(f->flags&MI_ZF))goto mi_004ee12b;
    /* 004ee0f6 */
    a=(f->r[5]&0xffffu);b=0x00000004u;t=mi_alu(f,a,b,16,1);
    /* 004ee0fa */
    if(!mi_signed_less(f->flags))goto mi_004ee11f;
    /* 004ee0fc */
    a=MI_U32((0x005e4ef4u));b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 004ee103 */
    if((f->flags&MI_ZF))goto mi_004ee11f;
    /* 004ee105 */
    f->r[0]=MI_U32((0x00000020u+f->r[4]));
    /* 004ee109 */
    f->r[2]=(0x00000000u+f->r[3]+f->r[0]*4u);
    /* 004ee10c */
    f->r[0]=MI_U32((0x00f57364u));
    /* 004ee111 */
    f->r[7]=MI_U32((0x0056f840u+f->r[2]*4u));
    /* 004ee118 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004ee11a */
    goto mi_004ee1af;
mi_004ee11f:;
    /* 004ee11f */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 004ee121 */
    MI_U8((0x00000013u+f->r[4]))=(uint8_t)(0x00000001u);
    /* 004ee126 */
    goto mi_004ee1af;
mi_004ee12b:;
    /* 004ee12b */
    f->r[3]=(f->r[3]&0xffffff00u)|(((MI_U8((0x00799a2fu+f->r[6])))&0xffu)<<0);
    /* 004ee131 */
    a=(f->r[3]&0xffu);b=MI_U8((0x00799a30u+f->r[6]));t=mi_alu(f,a,b,8,1);
    /* 004ee137 */
    if((f->flags&MI_CF))goto mi_004ee191;
    /* 004ee139 */
    a=f->r[0];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 004ee13c */
    if((f->flags&MI_ZF))goto mi_004ee191;
    /* 004ee13e */
    a=MI_U32((0x005e4ef4u));b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004ee145 */
    if(!(f->flags&MI_ZF))goto mi_004ee174;
    /* 004ee147 */
    f->r[5]=MI_U32((0x00000024u+f->r[4]));
    /* 004ee14b */
    a=f->r[3];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[3]=t;
    /* 004ee151 */
    f->r[5]=(0x00000000u+f->r[5]+f->r[5]*2u);
    /* 004ee155 */
    a=MI_U8((0x005e68d8u+f->r[3]+f->r[5]*2u));b=0x00000000u;t=mi_alu(f,a,b,8,1);
    /* 004ee15d */
    if((f->flags&MI_ZF))goto mi_004ee179;
    /* 004ee15f */
    f->r[7]=MI_U32((0x00f5738cu));
    /* 004ee165 */
    a=f->r[2];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004ee16b */
    f->r[2]=(0x00000000u+f->r[7]+f->r[2]*8u);
    /* 004ee16e */
    f->r[7]=(0xfffffff8u+f->r[2]+f->r[0]*1u);
    /* 004ee172 */
    goto mi_004ee1af;
mi_004ee174:;
    /* 004ee174 */
    a=f->r[0];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 004ee177 */
    if(!mi_signed_less(f->flags))goto mi_004ee188;
mi_004ee179:;
    /* 004ee179 */
    a=f->r[2];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004ee17f */
    f->r[2]=(0x00000000u+f->r[7]+f->r[2]*8u);
    /* 004ee182 */
    f->r[7]=(0xfffffff8u+f->r[2]+f->r[0]*1u);
    /* 004ee186 */
    goto mi_004ee1af;
mi_004ee188:;
    /* 004ee188 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004ee18b */
    f->r[7]=(0x0000006du+f->r[0]+f->r[7]*1u);
    /* 004ee18f */
    goto mi_004ee1af;
mi_004ee191:;
    /* 004ee191 */
    f->r[5]=MI_U32((0x007999e4u+f->r[6]));
    /* 004ee197 */
    a=f->r[2];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004ee19d */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00acf85cu+f->r[5])))&0xffu)<<0);
    /* 004ee1a3 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004ee1a6 */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004ee1a8 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004ee1ab */
    f->r[7]=(0x0000005fu+f->r[7]+f->r[0]*2u);
mi_004ee1af:;
    /* 004ee1af */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00799a31u+f->r[6])))&0xffu)<<0);
    /* 004ee1b5 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004ee1b7 */
    if(!(f->flags&MI_ZF))goto mi_004ee1c7;
    /* 004ee1b9 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00799a32u+f->r[6])))&0xffu)<<0);
    /* 004ee1bf */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004ee1c1 */
    if((f->flags&MI_ZF))goto mi_004ee3f6;
mi_004ee1c7:;
    /* 004ee1c7 */
    f->r[5]=MI_U32((0x00000014u+f->r[4]));
    /* 004ee1cb */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 004ee1cd */
    if((f->flags&MI_ZF))goto mi_004ee3f6;
    /* 004ee1d3 */
    f->r[2]=MI_U32((0x005e4ef4u));
    /* 004ee1d9 */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004ee1dc */
    if(!(f->flags&MI_ZF))goto mi_004ee2b2;
    /* 004ee1e2 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004ee1e4 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00799a2fu+f->r[6])))&0xffu)<<0);
    /* 004ee1ea */
    f->r[3]=f->r[2];
    /* 004ee1ec */
    f->r[2]=MI_U32((0x00000024u+f->r[4]));
    /* 004ee1f0 */
    f->r[2]=(0x00000000u+f->r[2]+f->r[2]*2u);
    /* 004ee1f3 */
    a=MI_U8((0x005e68d8u+f->r[3]+f->r[2]*2u));b=0x00000000u;t=mi_alu(f,a,b,8,1);
    /* 004ee1fb */
    if((f->flags&MI_ZF))goto mi_004ee35a;
    /* 004ee201 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004ee203 */
    if((f->flags&MI_ZF))goto mi_004ee24f;
    /* 004ee205 */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004ee20a */
    f->r[3]=0x00000006u;
    /* 004ee20f */
    f->r[0]=(0xfffffffeu+f->r[0]+f->r[0]*1u);
    /* 004ee213 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004ee214 */
    a=f->r[3];mi_idiv32(f,a);
    /* 004ee216 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 004ee219 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[0]*2u);
    /* 004ee21c */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x005f5b28u+f->r[1]*8u)))&0xffffu)<<0);
    /* 004ee224 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004ee227 */
    a=(f->r[2]&0xffffu);b=MI_U16((0x005f5618u+f->r[0]));t=mi_alu(f,a,b,16,0);f->r[2]=(f->r[2]&0xffff0000u)|(((t)&0xffffu)<<0);
    /* 004ee22e */
    f->r[0]=MI_U32((0x005f561cu+f->r[0]));
    /* 004ee234 */
    MI_U16((0x00799a34u+f->r[6]))=(uint16_t)((f->r[2]&0xffffu));
    /* 004ee23b */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004ee23c */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004ee23e */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004ee240 */
    a=(f->r[0]&0xffffu);b=MI_U16((0x005f5b2cu+f->r[1]*8u));t=mi_alu(f,a,b,16,0);f->r[0]=(f->r[0]&0xffff0000u)|(((t)&0xffffu)<<0);
    /* 004ee248 */
    MI_U16((0x00799a36u+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
mi_004ee24f:;
    /* 004ee24f */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00799a32u+f->r[6])))&0xffu)<<0);
    /* 004ee255 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004ee257 */
    if((f->flags&MI_ZF))goto mi_004ee3ee;
    /* 004ee25d */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004ee262 */
    f->r[3]=0x00000006u;
    /* 004ee267 */
    f->r[0]=(0xfffffffeu+f->r[0]+f->r[0]*1u);
    /* 004ee26b */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004ee26c */
    a=f->r[3];mi_idiv32(f,a);
    /* 004ee26e */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 004ee271 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[0]*2u);
    /* 004ee274 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x005f5b28u+f->r[1]*8u)))&0xffffu)<<0);
    /* 004ee27c */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004ee27f */
    a=(f->r[2]&0xffffu);b=MI_U16((0x005f5618u+f->r[0]));t=mi_alu(f,a,b,16,0);f->r[2]=(f->r[2]&0xffff0000u)|(((t)&0xffffu)<<0);
    /* 004ee286 */
    f->r[0]=MI_U32((0x005f561cu+f->r[0]));
    /* 004ee28c */
    MI_U16((0x00799a38u+f->r[6]))=(uint16_t)((f->r[2]&0xffffu));
    /* 004ee293 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004ee294 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004ee296 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004ee298 */
    a=(f->r[0]&0xffffu);b=MI_U16((0x005f5b2cu+f->r[1]*8u));t=mi_alu(f,a,b,16,0);f->r[0]=(f->r[0]&0xffff0000u)|(((t)&0xffffu)<<0);
    /* 004ee2a0 */
    MI_U16((0x00799a3au+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 004ee2a7 */
    MI_U32((0x00799a40u+f->r[6]))=(uint32_t)(f->r[5]);
    /* 004ee2ad */
    goto mi_004ee400;
mi_004ee2b2:;
    /* 004ee2b2 */
    a=f->r[2];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 004ee2b5 */
    if(!(f->flags&MI_ZF))goto mi_004ee35a;
    /* 004ee2bb */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004ee2bd */
    if((f->flags&MI_ZF))goto mi_004ee300;
    /* 004ee2bf */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004ee2c4 */
    f->r[3]=0x00000006u;
    /* 004ee2c9 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004ee2ca */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004ee2cb */
    a=f->r[3];mi_idiv32(f,a);
    /* 004ee2cd */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004ee2cf */
    a=f->r[2];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 004ee2d2 */
    f->r[2]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 004ee2d5 */
    t=(!mi_signed_less(f->flags))?1u:0u;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004ee2d8 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004ee2d9 */
    a=(f->r[0]&0xffu);b=0x000000fdu;t=mi_alu(f,a,b,8,3);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004ee2db */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004ee2de */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*2u);
    /* 004ee2e1 */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004ee2e4 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x005f57c8u+f->r[0])))&0xffffu)<<0);
    /* 004ee2eb */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x005f57ccu+f->r[0])))&0xffffu)<<0);
    /* 004ee2f2 */
    MI_U16((0x00799a34u+f->r[6]))=(uint16_t)((f->r[2]&0xffffu));
    /* 004ee2f9 */
    MI_U16((0x00799a36u+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
mi_004ee300:;
    /* 004ee300 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00799a32u+f->r[6])))&0xffu)<<0);
    /* 004ee306 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004ee308 */
    if((f->flags&MI_ZF))goto mi_004ee3ee;
    /* 004ee30e */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004ee313 */
    f->r[3]=0x00000006u;
    /* 004ee318 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004ee319 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 004ee31c */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004ee31d */
    a=f->r[3];mi_idiv32(f,a);
    /* 004ee31f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004ee321 */
    a=f->r[2];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 004ee324 */
    t=(!mi_signed_less(f->flags))?1u:0u;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004ee327 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004ee328 */
    a=(f->r[0]&0xffu);b=0x000000fdu;t=mi_alu(f,a,b,8,3);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004ee32a */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004ee32d */
    f->r[0]=(0x00000000u+f->r[0]+f->r[1]*2u);
    /* 004ee330 */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004ee333 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x005f57c8u+f->r[0])))&0xffffu)<<0);
    /* 004ee33a */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x005f57ccu+f->r[0])))&0xffffu)<<0);
    /* 004ee341 */
    MI_U16((0x00799a38u+f->r[6]))=(uint16_t)((f->r[2]&0xffffu));
    /* 004ee348 */
    MI_U16((0x00799a3au+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 004ee34f */
    MI_U32((0x00799a40u+f->r[6]))=(uint32_t)(f->r[5]);
    /* 004ee355 */
    goto mi_004ee400;
mi_004ee35a:;
    /* 004ee35a */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004ee35c */
    if((f->flags&MI_ZF))goto mi_004ee3a1;
    /* 004ee35e */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004ee363 */
    f->r[3]=0x00000006u;
    /* 004ee368 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004ee369 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004ee36a */
    a=f->r[3];mi_idiv32(f,a);
    /* 004ee36c */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 004ee36f */
    f->r[0]=(0x00000000u+f->r[2]+f->r[0]*2u);
    /* 004ee372 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x005f5b28u+f->r[1]*8u)))&0xffffu)<<0);
    /* 004ee37a */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004ee37d */
    a=(f->r[2]&0xffffu);b=MI_U16((0x005f5618u+f->r[0]));t=mi_alu(f,a,b,16,0);f->r[2]=(f->r[2]&0xffff0000u)|(((t)&0xffffu)<<0);
    /* 004ee384 */
    MI_U16((0x00799a34u+f->r[6]))=(uint16_t)((f->r[2]&0xffffu));
    /* 004ee38b */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x005f5b2cu+f->r[1]*8u)))&0xffffu)<<0);
    /* 004ee393 */
    a=(f->r[2]&0xffffu);b=MI_U16((0x005f561cu+f->r[0]));t=mi_alu(f,a,b,16,0);f->r[2]=(f->r[2]&0xffff0000u)|(((t)&0xffffu)<<0);
    /* 004ee39a */
    MI_U16((0x00799a36u+f->r[6]))=(uint16_t)((f->r[2]&0xffffu));
mi_004ee3a1:;
    /* 004ee3a1 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00799a32u+f->r[6])))&0xffu)<<0);
    /* 004ee3a7 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004ee3a9 */
    if((f->flags&MI_ZF))goto mi_004ee3ee;
    /* 004ee3ab */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004ee3b0 */
    f->r[3]=0x00000006u;
    /* 004ee3b5 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004ee3b6 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004ee3b7 */
    a=f->r[3];mi_idiv32(f,a);
    /* 004ee3b9 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 004ee3bc */
    f->r[0]=(0x00000000u+f->r[2]+f->r[0]*2u);
    /* 004ee3bf */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x005f5b28u+f->r[1]*8u)))&0xffffu)<<0);
    /* 004ee3c7 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x005f5b2cu+f->r[1]*8u)))&0xffffu)<<0);
    /* 004ee3cf */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004ee3d2 */
    a=(f->r[2]&0xffffu);b=MI_U16((0x005f5618u+f->r[0]));t=mi_alu(f,a,b,16,0);f->r[2]=(f->r[2]&0xffff0000u)|(((t)&0xffffu)<<0);
    /* 004ee3d9 */
    a=(f->r[1]&0xffffu);b=MI_U16((0x005f561cu+f->r[0]));t=mi_alu(f,a,b,16,0);f->r[1]=(f->r[1]&0xffff0000u)|(((t)&0xffffu)<<0);
    /* 004ee3e0 */
    MI_U16((0x00799a38u+f->r[6]))=(uint16_t)((f->r[2]&0xffffu));
    /* 004ee3e7 */
    MI_U16((0x00799a3au+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
mi_004ee3ee:;
    /* 004ee3ee */
    MI_U32((0x00799a40u+f->r[6]))=(uint32_t)(f->r[5]);
    /* 004ee3f4 */
    goto mi_004ee400;
mi_004ee3f6:;
    /* 004ee3f6 */
    MI_U32((0x00799a40u+f->r[6]))=(uint32_t)(0x00000000u);
mi_004ee400:;
    /* 004ee400 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000013u+f->r[4])))&0xffu)<<0);
    /* 004ee404 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004ee405 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004ee407 */
    if((f->flags&MI_ZF))goto mi_004ee46d;
    /* 004ee409 */
    f->r[7]=0x0056f880u;
    /* 004ee40e */
    f->r[3]=0x00000005u;
mi_004ee413:;
    /* 004ee413 */
    f->r[0]=MI_U32((0x007999e4u+f->r[6]));
    /* 004ee419 */
    f->r[2]=MI_U32((0x00000000u+f->r[7]));
    /* 004ee41b */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004ee41d */
    f->r[1]=MI_U32((0x00000018u+f->r[4]));
    /* 004ee421 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ee423 */
    f->r[2]=MI_U32((0x00af598cu+f->r[0]*4u));
    /* 004ee42a */
    MI_U16((0x00b4e6fcu+f->r[0]*2u))=(uint16_t)((f->r[1]&0xffffu));
    /* 004ee432 */
    a=f->r[2];b=0xefffffffu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004ee438 */
    a=f->r[2];b=0x00000008u;t=mi_alu(f,a,b,32,4);f->r[2]=t;
    /* 004ee43b */
    MI_U32((0x00af598cu+f->r[0]*4u))=(uint32_t)(f->r[2]);
    /* 004ee442 */
    f->r[2]=MI_U32((0x005e4ef4u));
    /* 004ee448 */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004ee44a */
    if(!(f->flags&MI_ZF))goto mi_004ee44f;
    /* 004ee44c */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ee44d */
    goto mi_004ee451;
mi_004ee44f:;
    /* 004ee44f */
    a=0x0000000au;f->r[4]-=4;MI_U32(f->r[4])=a;
mi_004ee451:;
    /* 004ee451 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ee452 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ee453 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004ee458u;f->boundary(0x004ee453u,0x004f3f70u,f);
    /* 004ee458 */
    a=f->r[7];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004ee45b */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004ee45e */
    a=f->r[7];b=0x0056f890u;t=mi_alu(f,a,b,32,1);
    /* 004ee464 */
    if(mi_signed_less(f->flags))goto mi_004ee413;
    /* 004ee466 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004ee467 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004ee468 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004ee469 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004ee46c */
    f->r[4]+=4;return;
mi_004ee46d:;
    /* 004ee46d */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[6]));
    /* 004ee474 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004ee476 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ee478 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x007999dbu+f->r[6])))&0xffu)<<0);
    /* 004ee47e */
    f->r[2]=MI_U32((0x0000001cu+f->r[4]));
    /* 004ee482 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ee483 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ee484 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ee485 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[6]));
    /* 004ee48c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ee48d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ee48e */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ee48f */
    f->r[4]-=4;MI_U32(f->r[4])=0x004ee494u;f->boundary(0x004ee48fu,0x0047bf50u,f);
    /* 004ee494 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004ee497 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004ee498 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004ee499 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004ee49a */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004ee49d */
    f->r[4]+=4;return;
}

void restore_building_refresh_C_004ee4a0(struct restore_building_refresh_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004ee4a0 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ee4a1 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ee4a2 */
    f->r[5]=MI_U32((0x0000000cu+f->r[4]));
    /* 004ee4a6 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ee4a7 */
    f->r[6]=f->r[5];
    /* 004ee4a9 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004ee4ab */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004ee4ae */
    a=f->r[6];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004ee4b0 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ee4b1 */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004ee4b4 */
    f->r[3]=f->r[5];
    /* 004ee4b6 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00799a2cu+f->r[6])))&0xffu)<<0);
    /* 004ee4bc */
    f->r[7]=f->r[0];
    /* 004ee4be */
    f->r[0]=MI_U32((0x00bca2e0u));
    /* 004ee4c3 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004ee4c4 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004ee4c6 */
    f->r[1]=f->r[0];
    /* 004ee4c8 */
    a=f->r[1];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[1]=t;
    /* 004ee4ca */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[1]);
mi_004ee4ce:;
    /* 004ee4ce */
    a=MI_U32((0x005e4ef4u));b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 004ee4d5 */
    if(!(f->flags&MI_ZF))goto mi_004ee4e0;
    /* 004ee4d7 */
    f->r[0]=MI_U32((0x0056f7ecu+f->r[7]*4u));
    /* 004ee4de */
    goto mi_004ee4e7;
mi_004ee4e0:;
    /* 004ee4e0 */
    f->r[0]=MI_U32((0x0056f7d8u+f->r[7]*4u));
mi_004ee4e7:;
    /* 004ee4e7 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004ee4e9 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ee4ea */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004ee4ec */
    if(!mi_signed_less(f->flags))goto mi_004ee4ff;
    /* 004ee4ee */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ee4ef */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ee4f0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004ee4f5u;f->boundary(0x004ee4f0u,0x004edf50u,f);
    /* 004ee4f5 */
    f->r[1]=MI_U32((0x00000020u+f->r[4]));
    /* 004ee4f9 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004ee4fc */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 004ee4fd */
    goto mi_004ee4ce;
mi_004ee4ff:;
    /* 004ee4ff */
    f->r[4]-=4;MI_U32(f->r[4])=0x004ee504u;f->boundary(0x004ee4ffu,0x004ecea0u,f);
    /* 004ee504 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004ee507 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004ee509 */
    if((f->flags&MI_ZF))goto mi_004ee54d;
    /* 004ee50b */
    f->r[1]=MI_U32((0x005f5d08u+f->r[7]*4u));
    /* 004ee512 */
    f->r[3]=MI_U32((0x007999e4u+f->r[6]));
    /* 004ee518 */
    f->r[7]=MI_U32((0x00bca2d8u));
    /* 004ee51e */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004ee520 */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004ee522 */
    f->r[0]=0x8fb823efu;
    /* 004ee527 */
    a=f->r[0];b=f->r[1];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 004ee529 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004ee52b */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ee52d */
    a=f->r[2];b=0x00000007u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 004ee530 */
    f->r[0]=f->r[2];
    /* 004ee532 */
    a=0x00000004u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ee534 */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 004ee537 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004ee539 */
    f->r[0]=f->r[1];
    /* 004ee53b */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ee53c */
    f->r[1]=0x000000e4u;
    /* 004ee541 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004ee542 */
    a=f->r[1];mi_idiv32(f,a);
    /* 004ee544 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ee545 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004ee54au;f->boundary(0x004ee545u,0x004ed360u,f);
    /* 004ee54a */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004ee54d:;
    /* 004ee54d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004ee54e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004ee54f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004ee550 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004ee551 */
    f->r[4]+=4;return;
}

void restore_building_refresh_C_004ee6c0(struct restore_building_refresh_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004ee6c0 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ee6c1 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ee6c2 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ee6c3 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ee6c4 */
    f->r[7]=MI_U32((0x00000014u+f->r[4]));
    /* 004ee6c8 */
    f->r[0]=f->r[7];
    /* 004ee6ca */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004ee6cc */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004ee6cf */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004ee6d1 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004ee6d4 */
    f->r[6]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[0]));
    /* 004ee6db */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[0]));
    /* 004ee6e2 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((MI_U8((0x007999dbu+f->r[0])))&0xffu)<<0);
    /* 004ee6e8 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004ee6ea */
    if(mi_signed_less(f->flags))goto mi_004ee77a;
    /* 004ee6f0 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004ee6f2 */
    if(mi_signed_less(f->flags))goto mi_004ee77a;
    /* 004ee6f8 */
    f->r[2]=MI_U32((0x00bca2d0u));
    /* 004ee6fe */
    f->r[0]=(0xffffffffu+f->r[3]+f->r[6]*1u);
    /* 004ee702 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004ee704 */
    if(!mi_signed_less(f->flags))goto mi_004ee77a;
    /* 004ee706 */
    f->r[0]=MI_U32((0x00bca2d4u));
    /* 004ee70b */
    f->r[2]=(0xffffffffu+f->r[3]+f->r[1]*1u);
    /* 004ee70f */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004ee711 */
    if(!mi_signed_less(f->flags))goto mi_004ee77a;
    /* 004ee713 */
    f->r[2]=MI_U32((0x00bca2d8u));
    /* 004ee719 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 004ee71c */
    f->r[0]=(0x00000000u+f->r[1]+f->r[0]*2u);
    /* 004ee71f */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004ee722 */
    f->r[2]=(0x00000000u+f->r[2]+f->r[1]*4u);
    /* 004ee725 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004ee727 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 004ee729 */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[2]);
    /* 004ee72f */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004ee77a;
    /* 004ee731 */
    f->r[1]=0x005cc8c0u;
    /* 004ee736 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ee737 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004ee73b */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 004ee73f */
    f->r[5]=0x10000008u;
mi_004ee744:;
    /* 004ee744 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 004ee746 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004ee763;
    /* 004ee748 */
    f->r[6]=f->r[3];
mi_004ee74a:;
    /* 004ee74a */
    f->r[0]=MI_U32((0x00000000u+f->r[1]));
    /* 004ee74c */
    a=f->r[1];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004ee74f */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004ee751 */
    a=MI_U32((0x00af598cu+f->r[0]*4u));b=f->r[5];t=mi_alu(f,a,b,32,4);MI_U32((0x00af598cu+f->r[0]*4u))=(uint32_t)(t);
    /* 004ee758 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 004ee759 */
    MI_U16((0x00b4e6fcu+f->r[0]*2u))=(uint16_t)((f->r[7]&0xffffu));
    /* 004ee761 */
    if(!(f->flags&MI_ZF))goto mi_004ee74a;
mi_004ee763:;
    /* 004ee763 */
    f->r[1]=MI_U32((0x00000018u+f->r[4]));
    /* 004ee767 */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 004ee76b */
    a=f->r[1];b=0x00000030u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004ee76e */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004ee76f */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004ee773 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004ee777 */
    if(!(f->flags&MI_ZF))goto mi_004ee744;
    /* 004ee779 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
mi_004ee77a:;
    /* 004ee77a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004ee77b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004ee77c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004ee77d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004ee77e */
    f->r[4]+=4;return;
}

static const uint8_t mi_rom_004ee780_004ee9fc[]={0x41,0xe8,0x4e,0x00,0x38,0xe8,0x4e,0x00,0x2f,0xe8,0x4e,0x00,0x38,0xe8,0x4e,0x00,0x4a,0xe8,0x4e,0x00};
static __inline uint32_t mi_read_004ee780(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x004ee9fcu&&width<=20u&&(offset=address-0x004ee9fcu)<=20u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004ee780_004ee9fc[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}
void restore_building_refresh_C_004ee780(struct restore_building_refresh_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004ee780 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ee781 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ee782 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ee783 */
    f->r[6]=mi_read_004ee780((0x00000020u+f->r[4]),4u);
    /* 004ee787 */
    f->r[0]=f->r[6];
    /* 004ee789 */
    f->r[1]=mi_read_004ee780((0x005e6970u),4u);
    /* 004ee78f */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004ee792 */
    f->r[5]=mi_read_004ee780((0x005e4ef4u),4u);
    /* 004ee798 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004ee79a */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004ee79d */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004ee79f */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ee7a0 */
    MI_U16((0x00799a10u+f->r[0]))=(uint16_t)((f->r[3]&0xffffu));
    /* 004ee7a7 */
    MI_U8((0x00799a2cu+f->r[0]))=(uint8_t)((f->r[1]&0xffu));
    /* 004ee7ad */
    MI_U8((0x00799a2du+f->r[0]))=(uint8_t)(0x00000002u);
    /* 004ee7b4 */
    MI_U8((0x00799a2eu+f->r[0]))=(uint8_t)((f->r[3]&0xffu));
    /* 004ee7ba */
    MI_U16((0x00799a3cu+f->r[0]))=(uint16_t)((f->r[3]&0xffffu));
    /* 004ee7c1 */
    a=f->r[5];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 004ee7c4 */
    f->r[7]=f->r[6];
    /* 004ee7c6 */
    MI_U8((0x00799a2fu+f->r[0]))=(uint8_t)((f->r[3]&0xffu));
    /* 004ee7cc */
    f->r[2]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 004ee7cf */
    if(!(f->flags&MI_ZF))goto mi_004ee7dc;
    /* 004ee7d1 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004ee7d4 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read_004ee780((0x005e657cu+f->r[2]),1u))&0xffu)<<0);
    /* 004ee7da */
    goto mi_004ee7e5;
mi_004ee7dc:;
    /* 004ee7dc */
    a=f->r[2];b=0x00000007u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004ee7df */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read_004ee780((0x005e4efcu+f->r[2]),1u))&0xffu)<<0);
mi_004ee7e5:;
    /* 004ee7e5 */
    MI_U8((0x00799a30u+f->r[0]))=(uint8_t)((f->r[2]&0xffu));
    /* 004ee7eb */
    MI_U8((0x00799a31u+f->r[0]))=(uint8_t)((f->r[3]&0xffu));
    /* 004ee7f1 */
    MI_U8((0x00799a32u+f->r[0]))=(uint8_t)((f->r[3]&0xffu));
    /* 004ee7f7 */
    MI_U8((0x00799a33u+f->r[0]))=(uint8_t)(0x0000000fu);
    /* 004ee7fe */
    MI_U16((0x00799a34u+f->r[0]))=(uint16_t)((f->r[3]&0xffffu));
    /* 004ee805 */
    MI_U16((0x00799a36u+f->r[0]))=(uint16_t)((f->r[3]&0xffffu));
    /* 004ee80c */
    f->r[2]=(0xffffffffu+f->r[5]);
    /* 004ee80f */
    MI_U16((0x00799a38u+f->r[0]))=(uint16_t)((f->r[3]&0xffffu));
    /* 004ee816 */
    MI_U16((0x00799a3au+f->r[0]))=(uint16_t)((f->r[3]&0xffffu));
    /* 004ee81d */
    a=f->r[2];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 004ee820 */
    MI_U32((0x00799a40u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 004ee826 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004ee853;
    /* 004ee828 */
    switch(mi_read_004ee780((0x004ee9fcu+f->r[2]*4u),4u)){case 0x004ee82fu:goto mi_004ee82f;case 0x004ee838u:goto mi_004ee838;case 0x004ee841u:goto mi_004ee841;case 0x004ee84au:goto mi_004ee84a;default:abort();}
mi_004ee82f:;
    /* 004ee82f */
    f->r[1]=mi_read_004ee780((0x005f5c54u+f->r[1]*4u),4u);
    /* 004ee836 */
    goto mi_004ee855;
mi_004ee838:;
    /* 004ee838 */
    f->r[1]=mi_read_004ee780((0x005f5c40u+f->r[1]*4u),4u);
    /* 004ee83f */
    goto mi_004ee855;
mi_004ee841:;
    /* 004ee841 */
    f->r[1]=mi_read_004ee780((0x005f5c68u+f->r[1]*4u),4u);
    /* 004ee848 */
    goto mi_004ee855;
mi_004ee84a:;
    /* 004ee84a */
    f->r[1]=mi_read_004ee780((0x005f5c7cu+f->r[1]*4u),4u);
    /* 004ee851 */
    goto mi_004ee855;
mi_004ee853:;
    /* 004ee853 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_004ee855:;
    /* 004ee855 */
    MI_U16((0x00799aa0u+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004ee85c */
    MI_U16((0x007999eau+f->r[0]))=(uint16_t)((f->r[3]&0xffffu));
    /* 004ee863 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ee864 */
    MI_U8((0x007999dbu+f->r[0]))=(uint8_t)(0x00000002u);
    /* 004ee86b */
    f->r[4]-=4;MI_U32(f->r[4])=0x004ee870u;f->boundary(0x004ee86bu,0x004ee6c0u,f);
    /* 004ee870 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ee871 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004ee876u;f->boundary(0x004ee871u,0x004f4f80u,f);
    /* 004ee876 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004ee879 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004ee87b */
    f->r[5]=0x00000001u;
    /* 004ee880 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[1]);
mi_004ee884:;
    /* 004ee884 */
    f->r[2]=mi_read_004ee780((0x005e4ef4u),4u);
    /* 004ee88a */
    f->r[0]=mi_read_004ee780((0x005e6970u),4u);
    /* 004ee88f */
    a=f->r[2];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 004ee892 */
    if(!(f->flags&MI_ZF))goto mi_004ee89d;
    /* 004ee894 */
    f->r[0]=mi_read_004ee780((0x0056f7ecu+f->r[0]*4u),4u);
    /* 004ee89b */
    goto mi_004ee8a4;
mi_004ee89d:;
    /* 004ee89d */
    f->r[0]=mi_read_004ee780((0x0056f7d8u+f->r[0]*4u),4u);
mi_004ee8a4:;
    /* 004ee8a4 */
    a=f->r[5];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004ee8a6 */
    if(!mi_signed_less(f->flags))goto mi_004ee9d4;
    /* 004ee8ac */
    a=f->r[2];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 004ee8af */
    if(!(f->flags&MI_ZF))goto mi_004ee8bf;
    /* 004ee8b1 */
    f->r[0]=mi_read_004ee780((0x0056fd18u+f->r[1]),4u);
    /* 004ee8b7 */
    f->r[1]=mi_read_004ee780((0x0056fd1cu+f->r[1]),4u);
    /* 004ee8bd */
    goto mi_004ee8cb;
mi_004ee8bf:;
    /* 004ee8bf */
    f->r[0]=mi_read_004ee780((0x0056f898u+f->r[1]),4u);
    /* 004ee8c5 */
    f->r[1]=mi_read_004ee780((0x0056f89cu+f->r[1]),4u);
mi_004ee8cb:;
    /* 004ee8cb */
    f->r[2]=mi_read_004ee780((0x0000001cu+f->r[4]),4u);
    /* 004ee8cf */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 004ee8d0 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ee8d1 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004ee8d3 */
    f->r[2]=mi_read_004ee780((0x00000018u+f->r[4]),4u);
    /* 004ee8d7 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ee8d8 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ee8d9 */
    f->r[1]=mi_read_004ee780((0x00000024u+f->r[4]),4u);
    /* 004ee8dd */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004ee8df */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ee8e0 */
    a=0x000000b7u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ee8e5 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ee8e6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004ee8ebu;f->boundary(0x004ee8e6u,0x0046a400u,f);
    /* 004ee8eb */
    f->r[6]=f->r[0];
    /* 004ee8ed */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ee8ee */
    f->r[4]-=4;MI_U32(f->r[4])=0x004ee8f3u;f->boundary(0x004ee8eeu,0x00485100u,f);
    /* 004ee8f3 */
    f->r[0]=f->r[6];
    /* 004ee8f5 */
    f->r[1]=f->r[7];
    /* 004ee8f7 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004ee8fa */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004ee8fc */
    f->r[2]=mi_read_004ee780((0x005e4ef4u),4u);
    /* 004ee902 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004ee905 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004ee908 */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004ee90a */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004ee90d */
    MI_U16((0x00799a10u+f->r[0]))=(uint16_t)((f->r[7]&0xffffu));
    /* 004ee914 */
    a=f->r[2];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 004ee917 */
    MI_U16((0x00799a12u+f->r[1]*8u))=(uint16_t)((f->r[6]&0xffffu));
    /* 004ee91f */
    f->r[1]=mi_read_004ee780((0x005e6970u),4u);
    /* 004ee925 */
    MI_U16((0x00799a12u+f->r[0]))=(uint16_t)((f->r[3]&0xffffu));
    /* 004ee92c */
    MI_U16((0x007999eau+f->r[0]))=(uint16_t)((f->r[5]&0xffffu));
    /* 004ee933 */
    MI_U8((0x007999dbu+f->r[0]))=(uint8_t)(0x00000002u);
    /* 004ee93a */
    MI_U8((0x00799a2cu+f->r[0]))=(uint8_t)((f->r[1]&0xffu));
    /* 004ee940 */
    MI_U8((0x00799a2du+f->r[0]))=(uint8_t)(0x00000002u);
    /* 004ee947 */
    MI_U8((0x00799a2eu+f->r[0]))=(uint8_t)((f->r[3]&0xffu));
    /* 004ee94d */
    MI_U16((0x00799a3cu+f->r[0]))=(uint16_t)((f->r[3]&0xffffu));
    /* 004ee954 */
    f->r[7]=f->r[6];
    /* 004ee956 */
    MI_U8((0x00799a2fu+f->r[0]))=(uint8_t)((f->r[3]&0xffu));
    /* 004ee95c */
    if(!(f->flags&MI_ZF))goto mi_004ee96e;
    /* 004ee95e */
    f->r[2]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 004ee961 */
    f->r[1]=(0x00000000u+f->r[5]+f->r[2]*4u);
    /* 004ee965 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read_004ee780((0x005e657cu+f->r[1]*8u),1u))&0xffu)<<0);
    /* 004ee96c */
    goto mi_004ee97d;
mi_004ee96e:;
    /* 004ee96e */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 004ee971 */
    a=f->r[1];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004ee974 */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004ee976 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read_004ee780((0x005e4efcu+f->r[1]*8u),1u))&0xffu)<<0);
mi_004ee97d:;
    /* 004ee97d */
    MI_U8((0x00799a30u+f->r[0]))=(uint8_t)((f->r[2]&0xffu));
    /* 004ee983 */
    MI_U8((0x00799a31u+f->r[0]))=(uint8_t)((f->r[3]&0xffu));
    /* 004ee989 */
    MI_U8((0x00799a32u+f->r[0]))=(uint8_t)((f->r[3]&0xffu));
    /* 004ee98f */
    MI_U8((0x00799a33u+f->r[0]))=(uint8_t)(0x0000000fu);
    /* 004ee996 */
    MI_U16((0x00799a34u+f->r[0]))=(uint16_t)((f->r[3]&0xffffu));
    /* 004ee99d */
    MI_U16((0x00799a36u+f->r[0]))=(uint16_t)((f->r[3]&0xffffu));
    /* 004ee9a4 */
    MI_U16((0x00799a38u+f->r[0]))=(uint16_t)((f->r[3]&0xffffu));
    /* 004ee9ab */
    MI_U16((0x00799a3au+f->r[0]))=(uint16_t)((f->r[3]&0xffffu));
    /* 004ee9b2 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ee9b3 */
    MI_U32((0x00799a40u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 004ee9b9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004ee9beu;f->boundary(0x004ee9b9u,0x004ee6c0u,f);
    /* 004ee9be */
    f->r[0]=mi_read_004ee780((0x00000028u+f->r[4]),4u);
    /* 004ee9c2 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004ee9c5 */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 004ee9c6 */
    a=f->r[0];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004ee9c9 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004ee9cd */
    f->r[1]=f->r[0];
    /* 004ee9cf */
    goto mi_004ee884;
mi_004ee9d4:;
    /* 004ee9d4 */
    f->r[0]=f->r[6];
    /* 004ee9d6 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004ee9d9 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004ee9db */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read_004ee780((0x007999eau+f->r[0]*8u),2u);
    /* 004ee9e3 */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 004ee9e5 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ee9e6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004ee9ebu;f->boundary(0x004ee9e6u,0x004f3c90u,f);
    /* 004ee9eb */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ee9ec */
    f->r[4]-=4;MI_U32(f->r[4])=0x004ee9f1u;f->boundary(0x004ee9ecu,0x004f5f10u,f);
    /* 004ee9f1 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004ee9f4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004ee9f5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004ee9f6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004ee9f7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004ee9f8 */
    f->r[4]+=4;return;
}

void restore_building_refresh_C_004f3c90(struct restore_building_refresh_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004f3c90 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f3c91 */
    f->r[1]=MI_U32((0x00000008u+f->r[4]));
    /* 004f3c95 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f3c96 */
    f->r[0]=f->r[1];
    /* 004f3c98 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f3c99 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004f3c9c */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f3c9e */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004f3ca0 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004f3ca3 */
    MI_U32((0x00000008u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004f3ca7 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f3ca8 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00799a2cu+f->r[0])))&0xffu)<<0);
    /* 004f3cae */
    f->r[0]=MI_U32((0x00bca2e0u));
    /* 004f3cb3 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004f3cb4 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004f3cb6 */
    f->r[7]=f->r[1];
    /* 004f3cb8 */
    f->r[5]=f->r[0];
    /* 004f3cba */
    f->r[0]=MI_U32((0x005e4ef4u));
    /* 004f3cbf */
    a=f->r[5];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[5]=t;
    /* 004f3cc1 */
    a=f->r[0];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 004f3cc4 */
    f->r[3]=0x005f6d58u;
    /* 004f3cc9 */
    if((f->flags&MI_ZF))goto mi_004f3cd0;
    /* 004f3ccb */
    f->r[3]=0x005f6cd0u;
mi_004f3cd0:;
    /* 004f3cd0 */
    f->r[0]=MI_U32((0x00000004u+f->r[3]+f->r[7]*8u));
    /* 004f3cd4 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f3cd5 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 004f3cd7 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004f3cd9 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004f3d13;
mi_004f3cdb:;
    /* 004f3cdb */
    f->r[2]=MI_U32((0x00000000u+f->r[3]+f->r[7]*8u));
    /* 004f3cde */
    f->r[0]=MI_U32((0x00000018u+f->r[4]));
    /* 004f3ce2 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f3ce3 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f3ce4 */
    f->r[1]=MI_U32((0x00000000u+f->r[2]+f->r[6]*8u));
    /* 004f3ce7 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004f3ce9 */
    f->r[0]=f->r[1];
    /* 004f3ceb */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f3cec */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004f3cef */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f3cf1 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004f3cf4 */
    MI_U8((0x00799a2du+f->r[0]))=(uint8_t)(0x00000003u);
    /* 004f3cfb */
    MI_U8((0x00799a33u+f->r[0]))=(uint8_t)(0x00000000u);
    /* 004f3d02 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f3d07u;f->boundary(0x004f3d02u,0x004edf50u,f);
    /* 004f3d07 */
    f->r[0]=MI_U32((0x00000004u+f->r[3]+f->r[7]*8u));
    /* 004f3d0b */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004f3d0e */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 004f3d0f */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004f3d11 */
    if(mi_signed_less(f->flags))goto mi_004f3cdb;
mi_004f3d13:;
    /* 004f3d13 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f3d14 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f3d19u;f->boundary(0x004f3d14u,0x004ecea0u,f);
    /* 004f3d19 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004f3d1c */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004f3d1e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004f3d1f */
    if((f->flags&MI_ZF))goto mi_004f3d67;
    /* 004f3d21 */
    f->r[0]=MI_U32((0x0000000cu+f->r[4]));
    /* 004f3d25 */
    f->r[7]=MI_U32((0x005f5d08u+f->r[7]*4u));
    /* 004f3d2c */
    f->r[3]=MI_U32((0x00bca2d8u));
    /* 004f3d32 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f3d34 */
    f->r[5]=MI_U32((0x007999e4u+f->r[0]));
    /* 004f3d3a */
    f->r[0]=0x8fb823efu;
    /* 004f3d3f */
    a=f->r[7];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004f3d41 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f3d43 */
    a=f->r[7];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 004f3d45 */
    a=f->r[0];b=f->r[7];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 004f3d47 */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004f3d49 */
    f->r[0]=f->r[7];
    /* 004f3d4b */
    a=f->r[2];b=0x00000007u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 004f3d4e */
    f->r[1]=f->r[2];
    /* 004f3d50 */
    a=f->r[1];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[1]=t;
    /* 004f3d53 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004f3d55 */
    f->r[1]=0x000000e4u;
    /* 004f3d5a */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f3d5b */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004f3d5c */
    a=f->r[1];mi_idiv32(f,a);
    /* 004f3d5e */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f3d5f */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f3d64u;f->boundary(0x004f3d5fu,0x004ed360u,f);
    /* 004f3d64 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004f3d67:;
    /* 004f3d67 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004f3d68 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004f3d69 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004f3d6a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004f3d6b */
    f->r[4]+=4;return;
}

static const uint8_t mi_rom_004f4160_004f42f4[]={0xe6,0x41,0x4f,0x00,0xa1,0x41,0x4f,0x00,0x51,0x42,0x4f,0x00,0x51,0x42,0x4f,0x00,0x51,0x42,0x4f,0x00,0x51,0x42,0x4f,0x00,0x51,0x42,0x4f,0x00,0x51,0x42,0x4f,0x00,0xa6,0x42,0x4f,0x00};
static __inline uint32_t mi_read_004f4160(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x004f42f4u&&width<=36u&&(offset=address-0x004f42f4u)<=36u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004f4160_004f42f4[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}
void restore_building_refresh_C_004f4160(struct restore_building_refresh_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004f4160 */
    f->r[0]=mi_read_004f4160((0x005e4ef4u),4u);
    /* 004f4165 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f4166 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f4167 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f4168 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f4169 */
    a=f->r[0];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 004f416c */
    f->r[7]=0x005f6d58u;
    /* 004f4171 */
    if((f->flags&MI_ZF))goto mi_004f4178;
    /* 004f4173 */
    f->r[7]=0x005f6cd0u;
mi_004f4178:;
    /* 004f4178 */
    f->r[3]=mi_read_004f4160((0x00000014u+f->r[4]),4u);
    /* 004f417c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004f417e */
    f->r[6]=f->r[3];
    /* 004f4180 */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004f4183 */
    a=f->r[6];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004f4185 */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004f4188 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read_004f4160((0x00799a2du+f->r[6]),1u))&0xffu)<<0);
    /* 004f418e */
    a=f->r[0];b=0xfffffffdu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f4191 */
    a=f->r[0];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 004f4194 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004f42ef;
    /* 004f419a */
    switch(mi_read_004f4160((0x004f42f4u+f->r[0]*4u),4u)){case 0x004f41a1u:goto mi_004f41a1;case 0x004f41e6u:goto mi_004f41e6;case 0x004f4251u:goto mi_004f4251;case 0x004f42a6u:goto mi_004f42a6;default:abort();}
mi_004f41a1:;
    /* 004f41a1 */
    f->r[7]=mi_read_004f4160((0x007999e4u+f->r[6]),4u);
    /* 004f41a7 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004f41a9 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004f41ab */
    f->r[2]=0x0056f880u;
    /* 004f41b0 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read_004f4160((0x00799a33u+f->r[6]),1u))&0xffu)<<0);
    /* 004f41b6 */
    f->r[3]=0xffffff6eu;
    /* 004f41bb */
    f->r[6]=f->r[0];
mi_004f41bd:;
    /* 004f41bd */
    f->r[5]=mi_read_004f4160((0x00000000u+f->r[2]),4u);
    /* 004f41bf */
    f->r[0]=f->r[7];
    /* 004f41c1 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f41c3 */
    f->r[5]=0x00000001u;
    /* 004f41c8 */
    a=f->r[5];b=(f->r[1]&0xffu);t=mi_shift(f,a,b,32,0);f->r[5]=t;
    /* 004f41ca */
    a=f->r[6];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 004f41cc */
    if((f->flags&MI_ZF))goto mi_004f41d5;
    /* 004f41ce */
    a=mi_read_004f4160((0x00af598cu+f->r[0]*4u),4u);b=f->r[3];t=mi_alu(f,a,b,32,3);MI_U32((0x00af598cu+f->r[0]*4u))=(uint32_t)(t);
mi_004f41d5:;
    /* 004f41d5 */
    a=f->r[2];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004f41d8 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004f41d9 */
    a=f->r[2];b=0x0056f890u;t=mi_alu(f,a,b,32,1);
    /* 004f41df */
    if(mi_signed_less(f->flags))goto mi_004f41bd;
    /* 004f41e1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004f41e2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004f41e3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004f41e4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004f41e5 */
    f->r[4]+=4;return;
mi_004f41e6:;
    /* 004f41e6 */
    f->r[1]=mi_read_004f4160((0x0000001cu+f->r[4]),4u);
    /* 004f41ea */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004f41ec */
    f->r[2]=mi_read_004f4160((0x00000004u+f->r[7]+f->r[1]*8u),4u);
    /* 004f41f0 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 004f41f2 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004f42ef;
    /* 004f41f8 */
    f->r[7]=mi_read_004f4160((0x00000000u+f->r[7]+f->r[1]*8u),4u);
    /* 004f41fb */
    f->r[3]=(uint32_t)(int32_t)(int16_t)mi_read_004f4160((0x007999eau+f->r[6]),2u);
    /* 004f4202 */
    f->r[1]=f->r[7];
mi_004f4204:;
    /* 004f4204 */
    a=f->r[3];b=mi_read_004f4160((0x00000000u+f->r[1]),4u);t=mi_alu(f,a,b,32,1);
    /* 004f4206 */
    if((f->flags&MI_ZF))goto mi_004f4215;
    /* 004f4208 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004f4209 */
    a=f->r[1];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004f420c */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004f420e */
    if(mi_signed_less(f->flags))goto mi_004f4204;
    /* 004f4210 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004f4211 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004f4212 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004f4213 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004f4214 */
    f->r[4]+=4;return;
mi_004f4215:;
    /* 004f4215 */
    f->r[0]=mi_read_004f4160((0x00000004u+f->r[7]+f->r[0]*8u),4u);
    /* 004f4219 */
    f->r[2]=mi_read_004f4160((0x007999e4u+f->r[6]),4u);
    /* 004f421f */
    f->r[1]=mi_read_004f4160((0x00f5736cu),4u);
    /* 004f4225 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f4227 */
    MI_U32((0x00b7482cu+f->r[0]*4u))=(uint32_t)(f->r[1]);
    /* 004f422e */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read_004f4160((0x00acf85cu+f->r[0]),1u))&0xffu)<<0);
    /* 004f4234 */
    a=(f->r[1]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,3);
    /* 004f4237 */
    if((f->flags&MI_ZF))goto mi_004f42ef;
    /* 004f423d */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read_004f4160((0x00b285ccu+f->r[0]),1u))&0xffu)<<0);
    /* 004f4243 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004f4244 */
    a=(f->r[1]&0xffu);b=0x00000020u;t=mi_alu(f,a,b,8,4);f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004f4247 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004f4248 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004f4249 */
    MI_U8((0x00b285ccu+f->r[0]))=(uint8_t)((f->r[1]&0xffu));
    /* 004f424f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004f4250 */
    f->r[4]+=4;return;
mi_004f4251:;
    /* 004f4251 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 004f4253 */
    f->r[7]=0x0056f880u;
mi_004f4258:;
    /* 004f4258 */
    f->r[0]=mi_read_004f4160((0x00000000u+f->r[7]),4u);
    /* 004f425a */
    f->r[1]=mi_read_004f4160((0x007999e4u+f->r[6]),4u);
    /* 004f4260 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f4262 */
    f->r[2]=0x00000001u;
    /* 004f4267 */
    f->r[1]=f->r[5];
    /* 004f4269 */
    a=f->r[2];b=(f->r[1]&0xffu);t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004f426b */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read_004f4160((0x00799a33u+f->r[6]),1u))&0xffu)<<0);
    /* 004f4271 */
    a=(f->r[1]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004f4273 */
    if((f->flags&MI_ZF))goto mi_004f427f;
    /* 004f4275 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004f4277 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read_004f4160((0x00799a2du+f->r[6]),1u))&0xffu)<<0);
    /* 004f427d */
    goto mi_004f4288;
mi_004f427f:;
    /* 004f427f */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004f4281 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read_004f4160((0x00799a2du+f->r[6]),1u))&0xffu)<<0);
    /* 004f4287 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
mi_004f4288:;
    /* 004f4288 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f428a */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f428b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f428c */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f428d */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f4292u;f->boundary(0x004f428du,0x004f3f70u,f);
    /* 004f4292 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004f4295 */
    a=f->r[7];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004f4298 */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 004f4299 */
    a=f->r[7];b=0x0056f890u;t=mi_alu(f,a,b,32,1);
    /* 004f429f */
    if(mi_signed_less(f->flags))goto mi_004f4258;
    /* 004f42a1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004f42a2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004f42a3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004f42a4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004f42a5 */
    f->r[4]+=4;return;
mi_004f42a6:;
    /* 004f42a6 */
    f->r[0]=mi_read_004f4160((0x00bca2e0u),4u);
    /* 004f42ab */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004f42ad */
    f->r[7]=(uint32_t)(int32_t)(int16_t)mi_read_004f4160((0x007999eau+f->r[6]),2u);
    /* 004f42b4 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004f42b5 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004f42b7 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read_004f4160((0x007999dbu+f->r[6]),1u))&0xffu)<<0);
    /* 004f42bd */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004f42bf */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f42c1 */
    f->r[2]=(0x00000000u+f->r[7]+f->r[0]*4u);
    /* 004f42c4 */
    f->r[7]=mi_read_004f4160((0x00f57364u),4u);
    /* 004f42ca */
    f->r[0]=mi_read_004f4160((0x0056f840u+f->r[2]*4u),4u);
    /* 004f42d1 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read_004f4160((0x007999deu+f->r[6]),2u);
    /* 004f42d8 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f42da */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f42db */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f42dc */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f42dd */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read_004f4160((0x007999e0u+f->r[6]),2u);
    /* 004f42e4 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f42e5 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f42e6 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f42e7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f42ecu;f->boundary(0x004f42e7u,0x0047bf50u,f);
    /* 004f42ec */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004f42ef:;
    /* 004f42ef */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004f42f0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004f42f1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004f42f2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004f42f3 */
    f->r[4]+=4;return;
}

void restore_building_refresh_C_004f5f10(struct restore_building_refresh_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004f5f10 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f5f11 */
    f->r[5]=MI_U32((0x00000008u+f->r[4]));
    /* 004f5f15 */
    f->r[0]=f->r[5];
    /* 004f5f17 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004f5f1a */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f5f1c */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004f5f1f */
    a=MI_U16((0x007999e8u+f->r[0]));b=0x000000b7u;t=mi_alu(f,a,b,16,1);
    /* 004f5f28 */
    if(!(f->flags&MI_ZF))goto mi_004f5ff9;
    /* 004f5f2e */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999eau+f->r[0]));
    /* 004f5f35 */
    a=f->r[5];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[5]=t;
    /* 004f5f37 */
    f->r[2]=MI_U32((0x005e4ef4u));
    /* 004f5f3d */
    f->r[0]=f->r[5];
    /* 004f5f3f */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004f5f41 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004f5f44 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f5f46 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004f5f49 */
    a=f->r[2];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 004f5f4c */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00799a2cu+f->r[0])))&0xffu)<<0);
    /* 004f5f52 */
    if(!(f->flags&MI_ZF))goto mi_004f5f5d;
    /* 004f5f54 */
    f->r[2]=MI_U32((0x005700e8u+f->r[1]*4u));
    /* 004f5f5b */
    goto mi_004f5f64;
mi_004f5f5d:;
    /* 004f5f5d */
    f->r[2]=MI_U32((0x005700d4u+f->r[1]*4u));
mi_004f5f64:;
    /* 004f5f64 */
    f->r[1]=MI_U32((0x007999e4u+f->r[0]));
    /* 004f5f6a */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f5f6b */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004f5f6d */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f5f6e */
    f->r[0]=f->r[1];
    /* 004f5f70 */
    f->r[6]=0x000000e4u;
    /* 004f5f75 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004f5f76 */
    a=f->r[6];mi_idiv32(f,a);
    /* 004f5f78 */
    f->r[0]=0x8fb823efu;
    /* 004f5f7d */
    f->r[3]=MI_U32((0x00bca2e8u));
    /* 004f5f83 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f5f84 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f5f86 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f5f88 */
    f->r[6]=f->r[2];
    /* 004f5f8a */
    a=f->r[0];b=f->r[1];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 004f5f8c */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004f5f8e */
    f->r[0]=MI_U32((0x00bca2ecu));
    /* 004f5f93 */
    a=f->r[2];b=0x00000007u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 004f5f96 */
    f->r[1]=f->r[2];
    /* 004f5f98 */
    a=f->r[6];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 004f5f9a */
    a=f->r[1];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[1]=t;
    /* 004f5f9d */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004f5f9f */
    f->r[7]=f->r[2];
    /* 004f5fa1 */
    f->r[2]=MI_U32((0x005d17a8u));
    /* 004f5fa7 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 004f5fa9 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 004f5fae */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f5faf */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f5fb0 */
    a=0x000000d7u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f5fb5 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f5fb6 */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004f5fba */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f5fbfu;f->boundary(0x004f5fbau,0x0046a400u,f);
    /* 004f5fbf */
    f->r[3]=f->r[0];
    /* 004f5fc1 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f5fc2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f5fc7u;f->boundary(0x004f5fc2u,0x00485100u,f);
    /* 004f5fc7 */
    f->r[1]=MI_U32((0x00000030u+f->r[4]));
    /* 004f5fcb */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 004f5fd1 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f5fd3 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f5fd5 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f5fd6 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f5fd7 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f5fd8 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f5fd9 */
    a=0x000000d7u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f5fde */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f5fdf */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f5fe4u;f->boundary(0x004f5fdfu,0x00470990u,f);
    /* 004f5fe4 */
    f->r[0]=f->r[3];
    /* 004f5fe6 */
    a=f->r[4];b=0x0000003cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004f5fe9 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004f5fec */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f5fee */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004f5fef */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004f5ff0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004f5ff1 */
    MI_U16((0x007999eau+f->r[0]*8u))=(uint16_t)((f->r[5]&0xffffu));
mi_004f5ff9:;
    /* 004f5ff9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004f5ffa */
    f->r[4]+=4;return;
}
