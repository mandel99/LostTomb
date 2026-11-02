/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "world_support_model.h"
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
static __inline uint32_t mi_alu(struct restore_building_world_frame *f,uint32_t a,uint32_t b,uint32_t width,uint32_t operation)
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
static __inline uint32_t mi_shift(struct restore_building_world_frame *f,uint32_t a,uint32_t count,uint32_t width,uint32_t arithmetic)
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
static __inline uint32_t mi_imul(struct restore_building_world_frame *f,uint32_t a,uint32_t b)
{
 int64_t product=(int64_t)(int32_t)a*(int32_t)b;uint32_t result=(uint32_t)product;
 f->flags&=~(MI_CF|MI_OF);if(product!=(int64_t)(int32_t)result)f->flags|=MI_CF|MI_OF;
 return result;
}
static __inline int mi_signed_less(uint32_t flags){return ((flags&MI_SF)!=0)^((flags&MI_OF)!=0);}

void restore_building_world_00401f70(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00401f70 */
    a=f->r[4];b=0x0000008cu;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 00401f76 */
    f->r[0]=0x00000129u;
    /* 00401f7b */
    f->r[1]=0x0000009eu;
    /* 00401f80 */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00401f84 */
    MI_U32((0x00000054u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00401f88 */
    f->r[0]=MI_U32((0x00000090u+f->r[4]));
    /* 00401f8f */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00401f93 */
    MI_U32((0x0000004cu+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00401f97 */
    f->r[1]=f->r[0];
    /* 00401f99 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00401f9c */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00401f9e */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00401f9f */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00401fa0 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00401fa1 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999eau+f->r[1]*8u));
    /* 00401fa9 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00401fab */
    f->r[5]=MI_U32((0x000000a4u+f->r[4]));
    /* 00401fb2 */
    f->r[1]=f->r[0];
    /* 00401fb4 */
    f->r[0]=MI_U32((0x00bca2e0u));
    /* 00401fb9 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 00401fba */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00401fbc */
    f->r[2]=(0x00000028u+f->r[4]);
    /* 00401fc0 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00401fc4 */
    f->r[2]=f->r[1];
    /* 00401fc6 */
    f->r[3]=MI_U32((0x000000a0u+f->r[4]));
    /* 00401fcd */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00401fce */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 00401fd1 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 00401fd3 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 00401fd5 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 00401fd7 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00401fd9 */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[6]);
    /* 00401fdd */
    MI_U32((0x00000030u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 00401fe1 */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(0x0000001eu);
    /* 00401fe9 */
    f->r[1]=(0x00799a39u+f->r[2]*8u);
    /* 00401ff0 */
    MI_U32((0x00000038u+f->r[4]))=(uint32_t)(0x00000046u);
    /* 00401ff8 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(0x00000030u);
    /* 00402000 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(0x000000cbu);
    /* 00402008 */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(0x00000153u);
    /* 00402010 */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(f->r[6]);
    /* 00402014 */
    MI_U32((0x00000040u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 00402018 */
    MI_U32((0x00000048u+f->r[4]))=(uint32_t)(0xffffffb1u);
    /* 00402020 */
    MI_U32((0x00000050u+f->r[4]))=(uint32_t)(0xffffff6fu);
    /* 00402028 */
    MI_U32((0x00000054u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 0040202c */
    MI_U32((0x00000058u+f->r[4]))=(uint32_t)(0xffffffc4u);
    /* 00402034 */
    MI_U32((0x00000060u+f->r[4]))=(uint32_t)(0xffffff75u);
    /* 0040203c */
    MI_U32((0x00000068u+f->r[4]))=(uint32_t)(0xffffff33u);
    /* 00402044 */
    MI_U32((0x0000006cu+f->r[4]))=(uint32_t)(0xfffffff1u);
    /* 0040204c */
    MI_U32((0x00000070u+f->r[4]))=(uint32_t)(0xffffff56u);
    /* 00402054 */
    MI_U32((0x00000074u+f->r[4]))=(uint32_t)(0x0000008fu);
    /* 0040205c */
    MI_U32((0x00000078u+f->r[4]))=(uint32_t)(0xffffff07u);
    /* 00402064 */
    MI_U32((0x0000007cu+f->r[4]))=(uint32_t)(0x0000011au);
    /* 0040206c */
    MI_U32((0x00000080u+f->r[4]))=(uint32_t)(0xfffffec5u);
    /* 00402077 */
    MI_U32((0x00000084u+f->r[4]))=(uint32_t)(0xffffffddu);
    /* 00402082 */
    MI_U32((0x00000088u+f->r[4]))=(uint32_t)(0xfffffee3u);
    /* 0040208d */
    MI_U32((0x0000008cu+f->r[4]))=(uint32_t)(0x0000007bu);
    /* 00402098 */
    MI_U32((0x00000090u+f->r[4]))=(uint32_t)(0xfffffe94u);
    /* 004020a3 */
    MI_U32((0x00000094u+f->r[4]))=(uint32_t)(0x00000106u);
    /* 004020ae */
    MI_U32((0x00000098u+f->r[4]))=(uint32_t)(0xfffffe52u);
    /* 004020b9 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004020bd */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004020c1 */
    goto mi_004020c5;
mi_004020c3:;
    /* 004020c3 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
mi_004020c5:;
    /* 004020c5 */
    f->r[2]=(0x00000020u+f->r[4]);
    /* 004020c9 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[2]);
mi_004020cd:;
    /* 004020cd */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
    /* 004020d1 */
    a=MI_U8((0x00000000u+f->r[1]+f->r[6]*1u));b=0x00000000u;t=mi_alu(f,a,b,8,1);
    /* 004020d5 */
    if((f->flags&MI_ZF))goto mi_00402160;
    /* 004020db */
    a=f->r[7];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 004020de */
    f->r[1]=f->r[0];
    /* 004020e0 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004020ed;
    /* 004020e2 */
    f->r[2]=MI_U32((0x00f57584u));
    /* 004020e8 */
    a=f->r[1];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 004020eb */
    goto mi_004020f6;
mi_004020ed:;
    /* 004020ed */
    f->r[2]=MI_U32((0x00f5758cu));
    /* 004020f3 */
    a=f->r[1];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[1]=t;
mi_004020f6:;
    /* 004020f6 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004020f8 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004020fa */
    if(!(f->flags&MI_ZF))goto mi_00402119;
    /* 004020fc */
    f->r[0]=(0x00000000u+f->r[7]+f->r[6]*1u);
    /* 004020ff */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00402102 */
    f->r[2]=MI_U32((0x00000040u+f->r[4]+f->r[0]*1u));
    /* 00402106 */
    f->r[0]=MI_U32((0x0000003cu+f->r[4]+f->r[0]*1u));
    /* 0040210a */
    f->r[2]=(0xffffff9du+f->r[2]+f->r[5]*1u);
    /* 0040210e */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040210f */
    f->r[2]=(0x000000f3u+f->r[0]+f->r[3]*1u);
    /* 00402116 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00402117 */
    goto mi_00402151;
mi_00402119:;
    /* 00402119 */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 0040211c */
    if(!(f->flags&MI_ZF))goto mi_00402160;
    /* 0040211e */
    f->r[3]=MI_U32((0x00000014u+f->r[4]));
    /* 00402122 */
    f->r[0]=(0x00000000u+f->r[7]+f->r[6]*1u);
    /* 00402125 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00402128 */
    f->r[2]=MI_U32((0x00000040u+f->r[4]+f->r[0]*1u));
    /* 0040212c */
    f->r[0]=MI_U32((0x0000003cu+f->r[4]+f->r[0]*1u));
    /* 00402130 */
    a=f->r[2];b=MI_U32((0x00000000u+f->r[3]));t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00402132 */
    f->r[2]=(0xfffffed9u+f->r[2]+f->r[5]*1u);
    /* 00402139 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040213a */
    f->r[2]=MI_U32((0x0000001cu+f->r[4]));
    /* 0040213e */
    f->r[3]=MI_U32((0x00000000u+f->r[2]));
    /* 00402140 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00402142 */
    f->r[3]=MI_U32((0x000000a8u+f->r[4]));
    /* 00402149 */
    f->r[0]=(0xfffffe65u+f->r[0]+f->r[3]*1u);
    /* 00402150 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
mi_00402151:;
    /* 00402151 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00402152 */
    f->r[1]=0x00f55bb0u;
    /* 00402157 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040215cu;f->boundary(0x00402157u,0x0053e590u,f);
    /* 0040215c */
    f->r[0]=MI_U32((0x0000001cu+f->r[4]));
mi_00402160:;
    /* 00402160 */
    f->r[2]=MI_U32((0x00000014u+f->r[4]));
    /* 00402164 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 00402165 */
    a=f->r[2];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00402168 */
    a=f->r[6];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 0040216b */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0040216f */
    if(mi_signed_less(f->flags))goto mi_004020cd;
    /* 00402175 */
    f->r[6]=MI_U32((0x00000010u+f->r[4]));
    /* 00402179 */
    f->r[2]=MI_U32((0x00000018u+f->r[4]));
    /* 0040217d */
    a=f->r[7];b=0x00000003u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 00402180 */
    a=f->r[6];b=0x00000003u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00402183 */
    a=f->r[2];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00402186 */
    a=f->r[7];b=0x0000000cu;t=mi_alu(f,a,b,32,1);
    /* 00402189 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 0040218d */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00402191 */
    if(mi_signed_less(f->flags))goto mi_004020c3;
    /* 00402197 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00402198 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00402199 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0040219a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040219b */
    a=f->r[4];b=0x0000008cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004021a1 */
    f->r[4]+=16;return;
}


static const uint8_t mi_rom_0040f74c[]={0x1c,0xf4,0x40,0x00,0xa6,0xf4,0x40,0x00};
static const uint8_t mi_rom_0040f754[]={0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x00,0x01,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x00,0x00,0x00};
static __inline uint32_t mi_read_0040f2c0(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x0040f74cu&&width<=8u&&(offset=address-0x0040f74cu)<=8u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0040f74c[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0040f754u&&width<=202u&&(offset=address-0x0040f754u)<=202u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0040f754[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}

void restore_building_world_0040f2c0(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0040f2c0 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040f2c1 */
    f->r[0]=mi_read_0040f2c0((0x0000000cu+f->r[4]),4u);
    /* 0040f2c5 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040f2c6 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040f2c7 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040f2c8 */
    f->r[6]=(uint32_t)(int32_t)(int16_t)mi_read_0040f2c0((0x00b4e6fcu+f->r[0]*2u),2u);
    /* 0040f2d0 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 0040f2d2 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040f2d3 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((mi_read_0040f2c0((0x00adc36cu+f->r[0]),1u))&0xffu)<<0);
    /* 0040f2d9 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 0040f2e1 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 0040f2e3 */
    if((f->flags&MI_ZF))goto mi_0040f674;
    /* 0040f2e9 */
    f->r[7]=f->r[6];
    /* 0040f2eb */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040f2ec */
    a=f->r[7];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 0040f2ef */
    a=f->r[7];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0040f2f1 */
    f->r[1]=0x00bca350u;
    /* 0040f2f6 */
    a=f->r[7];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 0040f2f9 */
    f->r[5]=(uint32_t)(int32_t)(int16_t)mi_read_0040f2c0((0x00799a18u+f->r[7]),2u);
    /* 0040f300 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040f305u;f->boundary(0x0040f300u,0x00488050u,f);
    /* 0040f305 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0040f307 */
    if((f->flags&MI_ZF))goto mi_0040f319;
    /* 0040f309 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 0040f30b */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0040f674;
    /* 0040f311 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040f312 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040f313 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0040f314 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0040f316 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040f317 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0040f318 */
    f->r[4]+=4;return;
mi_0040f319:;
    /* 0040f319 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read_0040f2c0((0x007999e8u+f->r[7]),2u))&0xffffu)<<0);
    /* 0040f320 */
    a=(f->r[0]&0xffffu);b=0x000000e8u;t=mi_alu(f,a,b,16,1);
    /* 0040f324 */
    if(!(f->flags&MI_ZF))goto mi_0040f35a;
    /* 0040f326 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 0040f328 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0040f332;
    /* 0040f32a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040f32b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040f32c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0040f32d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0040f32f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040f330 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0040f331 */
    f->r[4]+=4;return;
mi_0040f332:;
    /* 0040f332 */
    a=mi_read_0040f2c0((0x00799a14u+f->r[7]),2u);b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 0040f33a */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0040f344;
    /* 0040f33c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040f33d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040f33e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0040f33f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0040f341 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040f342 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0040f343 */
    f->r[4]+=4;return;
mi_0040f344:;
    /* 0040f344 */
    a=mi_read_0040f2c0((0x00799aa4u+f->r[7]),2u);b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 0040f34c */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0040f674;
    /* 0040f352 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040f353 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040f354 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0040f355 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0040f357 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040f358 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0040f359 */
    f->r[4]+=4;return;
mi_0040f35a:;
    /* 0040f35a */
    a=(f->r[0]&0xffffu);b=0x0000005cu;t=mi_alu(f,a,b,16,1);
    /* 0040f35e */
    if(!(f->flags&MI_ZF))goto mi_0040f388;
    /* 0040f360 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040f362 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040f363 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040f368u;f->boundary(0x0040f363u,0x0047fd30u,f);
    /* 0040f368 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040f36b */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0040f36e */
    if((f->flags&MI_ZF))goto mi_0040f66c;
    /* 0040f374 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040f377 */
    if((f->flags&MI_ZF))goto mi_0040f66c;
    /* 0040f37d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040f37e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040f37f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0040f380 */
    f->r[0]=0x00000001u;
    /* 0040f385 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040f386 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0040f387 */
    f->r[4]+=4;return;
mi_0040f388:;
    /* 0040f388 */
    a=(f->r[0]&0xffffu);b=0x000000b4u;t=mi_alu(f,a,b,16,1);
    /* 0040f38c */
    if(!(f->flags&MI_ZF))goto mi_0040f3b0;
    /* 0040f38e */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 0040f390 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0040f39a;
    /* 0040f392 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040f393 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040f394 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0040f395 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0040f397 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040f398 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0040f399 */
    f->r[4]+=4;return;
mi_0040f39a:;
    /* 0040f39a */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read_0040f2c0((0x00799a0du+f->r[7]),1u))&0xffu)<<0);
    /* 0040f3a0 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0040f3a2 */
    if(!(f->flags&MI_ZF))goto mi_0040f674;
    /* 0040f3a8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040f3a9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040f3aa */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0040f3ab */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0040f3ad */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040f3ae */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0040f3af */
    f->r[4]+=4;return;
mi_0040f3b0:;
    /* 0040f3b0 */
    a=(f->r[0]&0xffffu);b=0x0000005au;t=mi_alu(f,a,b,16,1);
    /* 0040f3b4 */
    if(!(f->flags&MI_ZF))goto mi_0040f3d8;
    /* 0040f3b6 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 0040f3b8 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0040f3c2;
    /* 0040f3ba */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040f3bb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040f3bc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0040f3bd */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0040f3bf */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040f3c0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0040f3c1 */
    f->r[4]+=4;return;
mi_0040f3c2:;
    /* 0040f3c2 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read_0040f2c0((0x00799a0du+f->r[7]),1u))&0xffu)<<0);
    /* 0040f3c8 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0040f3ca */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0040f674;
    /* 0040f3d0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040f3d1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040f3d2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0040f3d3 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0040f3d5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040f3d6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0040f3d7 */
    f->r[4]+=4;return;
mi_0040f3d8:;
    /* 0040f3d8 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040f3d9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040f3deu;f->boundary(0x0040f3d9u,0x00459ef0u,f);
    /* 0040f3de */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040f3e1 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0040f3e3 */
    if(!(f->flags&MI_ZF))goto mi_0040f4a6;
    /* 0040f3e9 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((mi_read_0040f2c0((0x007999e8u+f->r[7]),2u))&0xffffu)<<0);
    /* 0040f3f0 */
    a=(f->r[1]&0xffffu);b=0x0000006eu;t=mi_alu(f,a,b,16,1);
    /* 0040f3f4 */
    if(mi_signed_less(f->flags))goto mi_0040f3fc;
    /* 0040f3f6 */
    a=(f->r[1]&0xffffu);b=0x00000072u;t=mi_alu(f,a,b,16,1);
    /* 0040f3fa */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0040f41c;
mi_0040f3fc:;
    /* 0040f3fc */
    f->r[0]=(uint32_t)(int32_t)(int16_t)(f->r[1]&0xffffu);
    /* 0040f3ff */
    a=f->r[0];b=0xffffffe0u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0040f402 */
    a=f->r[0];b=0x000000c9u;t=mi_alu(f,a,b,32,1);
    /* 0040f407 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0040f4a6;
    /* 0040f40d */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0040f40f */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read_0040f2c0((0x0040f754u+f->r[0]),1u))&0xffu)<<0);
    /* 0040f415 */
    switch(mi_read_0040f2c0((0x0040f74cu+f->r[2]*4u),4u)){case 0x0040f41cu:goto mi_0040f41c;case 0x0040f4a6u:goto mi_0040f4a6;default:abort();}
mi_0040f41c:;
    /* 0040f41c */
    f->r[0]=mi_read_0040f2c0((0x00605a0cu),4u);
    /* 0040f421 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0040f423 */
    if(!(f->flags&MI_ZF))goto mi_0040f557;
    /* 0040f429 */
    a=(f->r[1]&0xffffu);b=0x0000004au;t=mi_alu(f,a,b,16,1);
    /* 0040f42d */
    f->r[2]=(f->r[2]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 0040f42f */
    if(!(f->flags&MI_ZF))goto mi_0040f456;
    /* 0040f431 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0040f433 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read_0040f2c0((0x00799a8cu+f->r[7]),2u))&0xffffu)<<0);
    /* 0040f43a */
    f->r[6]=f->r[0];
    /* 0040f43c */
    a=f->r[6];b=0x0000f000u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 0040f442 */
    a=f->r[6];b=0x00001000u;t=mi_alu(f,a,b,32,1);
    /* 0040f448 */
    if(!(f->flags&MI_ZF))goto mi_0040f456;
    /* 0040f44a */
    a=f->r[0];b=0x00000fffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 0040f44f */
    a=f->r[0];b=0x00000019u;t=mi_alu(f,a,b,32,1);
    /* 0040f452 */
    if(!(f->flags&MI_ZF))goto mi_0040f456;
    /* 0040f454 */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,5);f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
mi_0040f456:;
    /* 0040f456 */
    a=mi_read_0040f2c0((0x00799a14u+f->r[7]),2u);b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 0040f45e */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0040f46c;
    /* 0040f460 */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0040f462 */
    if((f->flags&MI_ZF))goto mi_0040f46c;
    /* 0040f464 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040f465 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040f466 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0040f467 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0040f469 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040f46a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0040f46b */
    f->r[4]+=4;return;
mi_0040f46c:;
    /* 0040f46c */
    a=(f->r[1]&0xffffu);b=0x000000ccu;t=mi_alu(f,a,b,16,1);
    /* 0040f471 */
    if(!(f->flags&MI_ZF))goto mi_0040f485;
    /* 0040f473 */
    a=mi_read_0040f2c0((0x00799aa4u+f->r[7]),2u);b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 0040f47b */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0040f485;
    /* 0040f47d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040f47e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040f47f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0040f480 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0040f482 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040f483 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0040f484 */
    f->r[4]+=4;return;
mi_0040f485:;
    /* 0040f485 */
    a=(f->r[1]&0xffffu);b=0x000000e2u;t=mi_alu(f,a,b,16,1);
    /* 0040f48a */
    if(!(f->flags&MI_ZF))goto mi_0040f557;
    /* 0040f490 */
    a=mi_read_0040f2c0((0x00799aa4u+f->r[7]),2u);b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 0040f498 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0040f557;
    /* 0040f49e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040f49f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040f4a0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0040f4a1 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0040f4a3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040f4a4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0040f4a5 */
    f->r[4]+=4;return;
mi_0040f4a6:;
    /* 0040f4a6 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read_0040f2c0((0x007999e8u+f->r[7]),2u))&0xffffu)<<0);
    /* 0040f4ad */
    a=(f->r[0]&0xffffu);b=0x00000037u;t=mi_alu(f,a,b,16,1);
    /* 0040f4b1 */
    if((f->flags&MI_ZF))goto mi_0040f557;
    /* 0040f4b7 */
    a=(f->r[0]&0xffffu);b=0x00000051u;t=mi_alu(f,a,b,16,1);
    /* 0040f4bb */
    if((f->flags&MI_ZF))goto mi_0040f557;
    /* 0040f4c1 */
    a=(f->r[0]&0xffffu);b=0x00000046u;t=mi_alu(f,a,b,16,1);
    /* 0040f4c5 */
    if((f->flags&MI_ZF))goto mi_0040f557;
    /* 0040f4cb */
    a=(f->r[0]&0xffffu);b=0x000000beu;t=mi_alu(f,a,b,16,1);
    /* 0040f4cf */
    if((f->flags&MI_ZF))goto mi_0040f557;
    /* 0040f4d5 */
    a=(f->r[0]&0xffffu);b=0x00000048u;t=mi_alu(f,a,b,16,1);
    /* 0040f4d9 */
    if((f->flags&MI_ZF))goto mi_0040f557;
    /* 0040f4db */
    a=(f->r[0]&0xffffu);b=0x000000c1u;t=mi_alu(f,a,b,16,1);
    /* 0040f4df */
    if((f->flags&MI_ZF))goto mi_0040f557;
    /* 0040f4e1 */
    a=(f->r[0]&0xffffu);b=0x0000004bu;t=mi_alu(f,a,b,16,1);
    /* 0040f4e5 */
    if((f->flags&MI_ZF))goto mi_0040f557;
    /* 0040f4e7 */
    a=(f->r[0]&0xffffu);b=0x000000c0u;t=mi_alu(f,a,b,16,1);
    /* 0040f4eb */
    if((f->flags&MI_ZF))goto mi_0040f557;
    /* 0040f4ed */
    a=(f->r[0]&0xffffu);b=0x000000a1u;t=mi_alu(f,a,b,16,1);
    /* 0040f4f1 */
    if((f->flags&MI_ZF))goto mi_0040f557;
    /* 0040f4f3 */
    a=(f->r[0]&0xffffu);b=0x000000d9u;t=mi_alu(f,a,b,16,1);
    /* 0040f4f7 */
    if((f->flags&MI_ZF))goto mi_0040f557;
    /* 0040f4f9 */
    a=(f->r[0]&0xffffu);b=0x000000a2u;t=mi_alu(f,a,b,16,1);
    /* 0040f4fd */
    if((f->flags&MI_ZF))goto mi_0040f557;
    /* 0040f4ff */
    a=(f->r[0]&0xffffu);b=0x0000006au;t=mi_alu(f,a,b,16,1);
    /* 0040f503 */
    if((f->flags&MI_ZF))goto mi_0040f557;
    /* 0040f505 */
    a=(f->r[0]&0xffffu);b=0x000000ddu;t=mi_alu(f,a,b,16,1);
    /* 0040f509 */
    if((f->flags&MI_ZF))goto mi_0040f557;
    /* 0040f50b */
    a=(f->r[0]&0xffffu);b=0x0000006bu;t=mi_alu(f,a,b,16,1);
    /* 0040f50f */
    if((f->flags&MI_ZF))goto mi_0040f557;
    /* 0040f511 */
    a=(f->r[0]&0xffffu);b=0x000000d8u;t=mi_alu(f,a,b,16,1);
    /* 0040f515 */
    if((f->flags&MI_ZF))goto mi_0040f557;
    /* 0040f517 */
    a=(f->r[0]&0xffffu);b=0x0000006cu;t=mi_alu(f,a,b,16,1);
    /* 0040f51b */
    if((f->flags&MI_ZF))goto mi_0040f557;
    /* 0040f51d */
    a=(f->r[0]&0xffffu);b=0x0000006du;t=mi_alu(f,a,b,16,1);
    /* 0040f521 */
    if((f->flags&MI_ZF))goto mi_0040f557;
    /* 0040f523 */
    a=(f->r[0]&0xffffu);b=0x00000022u;t=mi_alu(f,a,b,16,1);
    /* 0040f527 */
    if((f->flags&MI_ZF))goto mi_0040f557;
    /* 0040f529 */
    a=(f->r[0]&0xffffu);b=0x00000023u;t=mi_alu(f,a,b,16,1);
    /* 0040f52d */
    if((f->flags&MI_ZF))goto mi_0040f557;
    /* 0040f52f */
    a=(f->r[0]&0xffffu);b=0x00000024u;t=mi_alu(f,a,b,16,1);
    /* 0040f533 */
    if((f->flags&MI_ZF))goto mi_0040f557;
    /* 0040f535 */
    a=(f->r[0]&0xffffu);b=0x00000025u;t=mi_alu(f,a,b,16,1);
    /* 0040f539 */
    if((f->flags&MI_ZF))goto mi_0040f557;
    /* 0040f53b */
    a=(f->r[0]&0xffffu);b=0x00000020u;t=mi_alu(f,a,b,16,1);
    /* 0040f53f */
    if(!(f->flags&MI_ZF))goto mi_0040f551;
    /* 0040f541 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 0040f543 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0040f674;
    /* 0040f549 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040f54a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040f54b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0040f54c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0040f54e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040f54f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0040f550 */
    f->r[4]+=4;return;
mi_0040f551:;
    /* 0040f551 */
    a=(f->r[0]&0xffffu);b=0x000000e2u;t=mi_alu(f,a,b,16,1);
    /* 0040f555 */
    if(!(f->flags&MI_ZF))goto mi_0040f567;
mi_0040f557:;
    /* 0040f557 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 0040f559 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0040f674;
    /* 0040f55f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040f560 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040f561 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0040f562 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0040f564 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040f565 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0040f566 */
    f->r[4]+=4;return;
mi_0040f567:;
    /* 0040f567 */
    a=(f->r[0]&0xffffu);b=0x0000001fu;t=mi_alu(f,a,b,16,1);
    /* 0040f56b */
    if(!(f->flags&MI_ZF))goto mi_0040f5a8;
    /* 0040f56d */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040f56e */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040f573u;f->boundary(0x0040f56eu,0x00475b50u,f);
    /* 0040f573 */
    f->r[1]=f->r[0];
    /* 0040f575 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040f578 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0040f57b */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0040f57d */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0040f580 */
    a=mi_read_0040f2c0((0x00799a18u+f->r[1]),2u);b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 0040f588 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0040f592;
    /* 0040f58a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040f58b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040f58c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0040f58d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0040f58f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040f590 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0040f591 */
    f->r[4]+=4;return;
mi_0040f592:;
    /* 0040f592 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read_0040f2c0((0x00799a7fu+f->r[1]),1u))&0xffu)<<0);
    /* 0040f598 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0040f59a */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0040f674;
    /* 0040f5a0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040f5a1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040f5a2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0040f5a3 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0040f5a5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040f5a6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0040f5a7 */
    f->r[4]+=4;return;
mi_0040f5a8:;
    /* 0040f5a8 */
    a=(f->r[0]&0xffffu);b=0x0000001eu;t=mi_alu(f,a,b,16,1);
    /* 0040f5ac */
    if(!(f->flags&MI_ZF))goto mi_0040f5e9;
    /* 0040f5ae */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040f5af */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040f5b4u;f->boundary(0x0040f5afu,0x00475b50u,f);
    /* 0040f5b4 */
    f->r[1]=f->r[0];
    /* 0040f5b6 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040f5b9 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0040f5bc */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0040f5be */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0040f5c1 */
    a=mi_read_0040f2c0((0x00799a18u+f->r[1]),2u);b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 0040f5c9 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0040f5d3;
    /* 0040f5cb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040f5cc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040f5cd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0040f5ce */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0040f5d0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040f5d1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0040f5d2 */
    f->r[4]+=4;return;
mi_0040f5d3:;
    /* 0040f5d3 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read_0040f2c0((0x00799a80u+f->r[1]),1u))&0xffu)<<0);
    /* 0040f5d9 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0040f5db */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0040f674;
    /* 0040f5e1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040f5e2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040f5e3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0040f5e4 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0040f5e6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040f5e7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0040f5e8 */
    f->r[4]+=4;return;
mi_0040f5e9:;
    /* 0040f5e9 */
    a=(f->r[0]&0xffffu);b=0x00000021u;t=mi_alu(f,a,b,16,1);
    /* 0040f5ed */
    if(!(f->flags&MI_ZF))goto mi_0040f626;
    /* 0040f5ef */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040f5f0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040f5f5u;f->boundary(0x0040f5f0u,0x00475b50u,f);
    /* 0040f5f5 */
    f->r[1]=f->r[0];
    /* 0040f5f7 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040f5fa */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0040f5fd */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0040f5ff */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0040f602 */
    a=mi_read_0040f2c0((0x00799a18u+f->r[1]),2u);b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 0040f60a */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0040f614;
    /* 0040f60c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040f60d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040f60e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0040f60f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0040f611 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040f612 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0040f613 */
    f->r[4]+=4;return;
mi_0040f614:;
    /* 0040f614 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read_0040f2c0((0x00799a81u+f->r[1]),1u))&0xffu)<<0);
    /* 0040f61a */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0040f61c */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0040f674;
    /* 0040f61e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040f61f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040f620 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0040f621 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0040f623 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040f624 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0040f625 */
    f->r[4]+=4;return;
mi_0040f626:;
    /* 0040f626 */
    a=(f->r[0]&0xffffu);b=0x000000d2u;t=mi_alu(f,a,b,16,1);
    /* 0040f62a */
    if((f->flags&MI_ZF))goto mi_0040f674;
    /* 0040f62c */
    a=(f->r[0]&0xffffu);b=0x000000e4u;t=mi_alu(f,a,b,16,1);
    /* 0040f630 */
    if(!(f->flags&MI_ZF))goto mi_0040f643;
    /* 0040f632 */
    a=mi_read_0040f2c0((0x00799a30u+f->r[7]),4u);b=0x00000009u;t=mi_alu(f,a,b,32,1);
    /* 0040f639 */
    if((f->flags&MI_ZF))goto mi_0040f674;
    /* 0040f63b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040f63c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040f63d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0040f63e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0040f640 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040f641 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0040f642 */
    f->r[4]+=4;return;
mi_0040f643:;
    /* 0040f643 */
    a=(f->r[0]&0xffffu);b=0x00000047u;t=mi_alu(f,a,b,16,1);
    /* 0040f647 */
    if((f->flags&MI_ZF))goto mi_0040f557;
    /* 0040f64d */
    a=(f->r[0]&0xffffu);b=0x000000bfu;t=mi_alu(f,a,b,16,1);
    /* 0040f651 */
    if((f->flags&MI_ZF))goto mi_0040f557;
    /* 0040f657 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 0040f659 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0040f674;
    /* 0040f65b */
    f->r[0]=(uint32_t)(int32_t)(int16_t)(f->r[0]&0xffffu);
    /* 0040f65e */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 0040f661 */
    f->r[1]=mi_read_0040f2c0((0x00705eb8u+f->r[0]*8u),4u);
    /* 0040f668 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0040f66a */
    if((f->flags&MI_ZF))goto mi_0040f674;
mi_0040f66c:;
    /* 0040f66c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040f66d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040f66e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0040f66f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0040f671 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040f672 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0040f673 */
    f->r[4]+=4;return;
mi_0040f674:;
    /* 0040f674 */
    f->r[0]=mi_read_0040f2c0((0x00000018u+f->r[4]),4u);
    /* 0040f678 */
    f->r[6]=mi_read_0040f2c0((0x0126517cu),4u);
    /* 0040f67e */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 0040f680 */
    f->r[1]=f->r[0];
    /* 0040f682 */
    if(mi_signed_less(f->flags))goto mi_0040f686;
    /* 0040f684 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_0040f686:;
    /* 0040f686 */
    a=f->r[1];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0040f689 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0040f68b */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read_0040f2c0((0x010d6a32u+f->r[1]),1u))&0xffu)<<0);
    /* 0040f691 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read_0040f2c0((0x00605b38u+f->r[2]),1u))&0xffu)<<0);
    /* 0040f697 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0040f699 */
    if(!(f->flags&MI_ZF))goto mi_0040f6a6;
    /* 0040f69b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040f69c */
    f->r[0]=f->r[3];
    /* 0040f69e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040f69f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0040f6a0 */
    a=f->r[0];b=0x0000007fu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 0040f6a3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040f6a4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0040f6a5 */
    f->r[4]+=4;return;
mi_0040f6a6:;
    /* 0040f6a6 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 0040f6a8 */
    f->r[1]=f->r[0];
    /* 0040f6aa */
    if(mi_signed_less(f->flags))goto mi_0040f6ae;
    /* 0040f6ac */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_0040f6ae:;
    /* 0040f6ae */
    a=f->r[1];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0040f6b1 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read_0040f2c0((0x010d6a28u+f->r[1]),1u))&0xffu)<<0);
    /* 0040f6b7 */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0040f6b9 */
    if((f->flags&MI_ZF))goto mi_0040f70d;
    /* 0040f6bb */
    a=(f->r[3]&0xffu);b=0x00000080u;t=mi_alu(f,a,b,8,3);
    /* 0040f6be */
    f->r[2]=0x00000001u;
    /* 0040f6c3 */
    if(!(f->flags&MI_ZF))goto mi_0040f6c9;
    /* 0040f6c5 */
    f->r[2]=mi_read_0040f2c0((0x00000010u+f->r[4]),4u);
mi_0040f6c9:;
    /* 0040f6c9 */
    a=f->r[3];b=0x0000007fu;t=mi_alu(f,a,b,32,3);f->r[3]=t;
    /* 0040f6cc */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 0040f6ce */
    if(!(f->flags&MI_ZF))goto mi_0040f6fe;
    /* 0040f6d0 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 0040f6d1 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 0040f6d3 */
    f->r[1]=f->r[0];
    /* 0040f6d5 */
    if(mi_signed_less(f->flags))goto mi_0040f6d9;
    /* 0040f6d7 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_0040f6d9:;
    /* 0040f6d9 */
    a=f->r[1];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0040f6dc */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read_0040f2c0((0x010d6a16u+f->r[1]),2u);
    /* 0040f6e3 */
    a=f->r[3];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0040f6e5 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0040f72b;
    /* 0040f6e7 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 0040f6e9 */
    if(mi_signed_less(f->flags))goto mi_0040f6ed;
    /* 0040f6eb */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_0040f6ed:;
    /* 0040f6ed */
    a=f->r[0];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0040f6f0 */
    f->r[2]=0x00000001u;
    /* 0040f6f5 */
    f->r[3]=(uint32_t)(int32_t)(int16_t)mi_read_0040f2c0((0x010d6a16u+f->r[0]),2u);
    /* 0040f6fc */
    goto mi_0040f72b;
mi_0040f6fe:;
    /* 0040f6fe */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 0040f6ff */
    a=f->r[3];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0040f702 */
    if(!mi_signed_less(f->flags))goto mi_0040f72b;
    /* 0040f704 */
    f->r[3]=0x00000001u;
    /* 0040f709 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0040f70b */
    goto mi_0040f72b;
mi_0040f70d:;
    /* 0040f70d */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 0040f70e */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 0040f710 */
    if(mi_signed_less(f->flags))goto mi_0040f714;
    /* 0040f712 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_0040f714:;
    /* 0040f714 */
    a=f->r[0];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0040f717 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read_0040f2c0((0x010d6a16u+f->r[0]),2u);
    /* 0040f71e */
    a=f->r[3];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0040f720 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0040f727;
    /* 0040f722 */
    f->r[3]=0x00000001u;
mi_0040f727:;
    /* 0040f727 */
    f->r[2]=mi_read_0040f2c0((0x00000010u+f->r[4]),4u);
mi_0040f72b:;
    /* 0040f72b */
    f->r[0]=mi_read_0040f2c0((0x0000001cu+f->r[4]),4u);
    /* 0040f72f */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 0040f731 */
    MI_U8((0x00adc36cu+f->r[0]))=(uint8_t)((f->r[3]&0xffu));
    /* 0040f737 */
    if((f->flags&MI_ZF))goto mi_0040f744;
    /* 0040f739 */
    f->r[1]=(f->r[1]&0xffffff00u)|((((f->r[3]&0xffu))&0xffu)<<0);
    /* 0040f73b */
    a=(f->r[1]&0xffu);b=0x00000080u;t=mi_alu(f,a,b,8,4);f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0040f73e */
    MI_U8((0x00adc36cu+f->r[0]))=(uint8_t)((f->r[1]&0xffu));
mi_0040f744:;
    /* 0040f744 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040f745 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040f746 */
    f->r[0]=f->r[3];
    /* 0040f748 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0040f749 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040f74a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0040f74b */
    f->r[4]+=4;return;
}


void restore_building_world_00436180(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00436180 */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 00436184 */
    f->r[1]=MI_U32((0x0000000cu+f->r[4]));
    /* 00436188 */
    f->r[2]=MI_U32((0x00000008u+f->r[4]));
    /* 0043618c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043618d */
    f->r[0]=MI_U32((0x00000008u+f->r[4]));
    /* 00436191 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436192 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436193 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436194 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00436199u;f->boundary(0x00436194u,0x004361a0u,f);
    /* 00436199 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043619c */
    f->r[4]+=4;return;
}


void restore_building_world_004361a0(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004361a0 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004361a1 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004361a2 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004361a3 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004361a4 */
    f->r[7]=MI_U32((0x00000014u+f->r[4]));
    /* 004361a8 */
    a=f->r[7];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 004361ab */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004361b2;
    /* 004361ad */
    f->r[7]=0x0000000au;
mi_004361b2:;
    /* 004361b2 */
    f->r[0]=MI_U32((0x00000020u+f->r[4]));
    /* 004361b6 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 004361be */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 004361c1 */
    if(!(f->flags&MI_ZF))goto mi_004361cd;
    /* 004361c3 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(0x0000000cu);
    /* 004361cb */
    goto mi_004361fc;
mi_004361cd:;
    /* 004361cd */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004361cf */
    if((f->flags&MI_ZF))goto mi_004361fc;
    /* 004361d1 */
    a=f->r[7];b=0x00000009u;t=mi_alu(f,a,b,32,1);
    /* 004361d4 */
    if(mi_signed_less(f->flags))goto mi_004361e0;
    /* 004361d6 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(0x00000009u);
    /* 004361de */
    goto mi_004361fc;
mi_004361e0:;
    /* 004361e0 */
    a=f->r[7];b=0x00000007u;t=mi_alu(f,a,b,32,1);
    /* 004361e3 */
    if(mi_signed_less(f->flags))goto mi_004361ef;
    /* 004361e5 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(0x00000006u);
    /* 004361ed */
    goto mi_004361fc;
mi_004361ef:;
    /* 004361ef */
    a=f->r[7];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 004361f2 */
    if(mi_signed_less(f->flags))goto mi_004361fc;
    /* 004361f4 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(0x00000003u);
mi_004361fc:;
    /* 004361fc */
    f->r[0]=MI_U32((0x00f56754u));
    /* 00436201 */
    f->r[1]=MI_U32((0x00000014u+f->r[4]));
    /* 00436205 */
    f->r[6]=(0x00000000u+f->r[0]+f->r[1]*1u);
    /* 00436208 */
    f->r[1]=MI_U32((0x0126517cu));
    /* 0043620e */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 00436210 */
    f->r[0]=f->r[6];
    /* 00436212 */
    if(mi_signed_less(f->flags))goto mi_00436216;
    /* 00436214 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_00436216:;
    /* 00436216 */
    a=f->r[0];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00436219 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 0043621b */
    f->r[5]=(uint32_t)(int32_t)(int16_t)MI_U16((0x010d6a0eu+f->r[0]));
    /* 00436222 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 00436226 */
    if(!(f->flags&MI_ZF))goto mi_00436250;
    /* 00436228 */
    f->r[2]=MI_U32((0x0000001cu+f->r[4]));
    /* 0043622c */
    f->r[1]=MI_U32((0x00000018u+f->r[4]));
    /* 00436230 */
    a=f->r[2];b=0xfffffff8u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00436233 */
    a=f->r[1];b=0x00000009u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00436236 */
    f->r[0]=(0x00000002u+f->r[6]);
    /* 00436239 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043623a */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043623b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043623c */
    f->r[1]=0x00f55bb0u;
    /* 00436241 */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[0]);
    /* 00436246 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043624bu;f->boundary(0x00436246u,0x0053e590u,f);
    /* 0043624b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0043624c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0043624d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0043624e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0043624f */
    f->r[4]+=4;return;
mi_00436250:;
    /* 00436250 */
    a=f->r[7];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 00436253 */
    if(!(f->flags&MI_ZF))goto mi_0043628d;
    /* 00436255 */
    f->r[7]=MI_U32((0x0000001cu+f->r[4]));
    /* 00436259 */
    f->r[3]=MI_U32((0x00000018u+f->r[4]));
    /* 0043625d */
    f->r[1]=(0x00000002u+f->r[6]);
    /* 00436260 */
    f->r[2]=(0xfffffff8u+f->r[7]);
    /* 00436263 */
    f->r[0]=(0x00000009u+f->r[3]);
    /* 00436266 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436267 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436268 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436269 */
    f->r[1]=0x00f55bb0u;
    /* 0043626e */
    f->r[4]-=4;MI_U32(f->r[4])=0x00436273u;f->boundary(0x0043626eu,0x0053e590u,f);
    /* 00436273 */
    a=f->r[7];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 00436275 */
    a=f->r[3];b=0x00000005u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 00436278 */
    a=f->r[7];b=0x00000005u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0043627b */
    f->r[1]=0x00f55bb0u;
    /* 00436280 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436281 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436282 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436283 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00436288u;f->boundary(0x00436283u,0x0053e590u,f);
    /* 00436288 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00436289 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0043628a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0043628b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0043628c */
    f->r[4]+=4;return;
mi_0043628d:;
    /* 0043628d */
    f->r[3]=MI_U32((0x0000001cu+f->r[4]));
    /* 00436291 */
    f->r[5]=MI_U32((0x00000018u+f->r[4]));
    /* 00436295 */
    f->r[1]=(0x00000002u+f->r[6]);
    /* 00436298 */
    f->r[2]=(0xfffffff8u+f->r[3]);
    /* 0043629b */
    f->r[0]=(0x00000009u+f->r[5]);
    /* 0043629e */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043629f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004362a0 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004362a1 */
    f->r[1]=0x00f55bb0u;
    /* 004362a6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004362abu;f->boundary(0x004362a6u,0x0053e590u,f);
    /* 004362ab */
    f->r[1]=MI_U32((0x00000020u+f->r[4]));
    /* 004362af */
    f->r[2]=(0x00000000u+f->r[7]+f->r[7]*4u);
    /* 004362b2 */
    a=f->r[2];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004362b4 */
    f->r[0]=f->r[3];
    /* 004362b6 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004362b8 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004362ba */
    f->r[1]=(0x00000005u+f->r[5]);
    /* 004362bd */
    a=f->r[0];b=0x0000000fu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004362c0 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004362c1 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004362c2 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004362c3 */
    f->r[1]=0x00f55bb0u;
    /* 004362c8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004362cdu;f->boundary(0x004362c8u,0x0053e590u,f);
    /* 004362cd */
    f->r[2]=MI_U32((0x00f56754u));
    /* 004362d3 */
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
    /* 004362d7 */
    f->r[1]=(0x00000001u+f->r[2]+f->r[0]*1u);
    /* 004362db */
    f->r[0]=(0xffffffffu+f->r[7]);
    /* 004362de */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004362e0 */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[1]);
    /* 004362e6 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00436309;
    /* 004362e8 */
    a=f->r[5];b=0x00000011u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 004362eb */
    f->r[6]=(0xfffffffbu+f->r[3]);
    /* 004362ee */
    f->r[7]=f->r[0];
mi_004362f0:;
    /* 004362f0 */
    f->r[2]=MI_U32((0x00d3b5c0u));
    /* 004362f6 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004362f7 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004362f8 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004362f9 */
    f->r[1]=0x00f55bb0u;
    /* 004362fe */
    f->r[4]-=4;MI_U32(f->r[4])=0x00436303u;f->boundary(0x004362feu,0x0053e590u,f);
    /* 00436303 */
    a=f->r[6];b=0x0000000au;t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 00436306 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 00436307 */
    if(!(f->flags&MI_ZF))goto mi_004362f0;
mi_00436309:;
    /* 00436309 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0043630a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0043630b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0043630c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0043630d */
    f->r[4]+=4;return;
}


static const uint8_t mi_rom_00436a28[]={0x80,0x69,0x43,0x00,0x60,0x68,0x43,0x00,0xe8,0x67,0x43,0x00,0x24,0x68,0x43,0x00,0xa8,0x66,0x43,0x00,0x9c,0x68,0x43,0x00,0xfd,0x63,0x43,0x00,0xef,0x64,0x43,0x00,0x2a,0x65,0x43,0x00,0x04,0x69,0x43,0x00,0x77,0x64,0x43,0x00,0xb2,0x64,0x43,0x00,0xc0,0x63,0x43,0x00,0x46,0x67,0x43,0x00,0x83,0x63,0x43,0x00,0x3a,0x64,0x43,0x00,0x65,0x65,0x43,0x00,0x43,0x66,0x43,0x00,0x41,0x69,0x43,0x00,0xbd,0x69,0x43,0x00,0x83,0x67,0x43,0x00,0xa2,0x65,0x43,0x00,0x07,0x66,0x43,0x00,0x1d,0x6a,0x43,0x00};
static const uint8_t mi_rom_00436a88[]={0x00,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x01,0x17,0x17,0x17,0x02,0x17,0x03,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x04,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x05,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x06,0x17,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x0d,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x0e,0x0f,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x10,0x11,0x12,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x13,0x17,0x17,0x17,0x17,0x14,0x15,0x16};
static __inline uint32_t mi_read_00436310(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x00436a28u&&width<=96u&&(offset=address-0x00436a28u)<=96u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00436a28[offset+k]<<(k*8u);return result;
 }
 if(address>=0x00436a88u&&width<=202u&&(offset=address-0x00436a88u)<=202u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00436a88[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}

void restore_building_world_00436310(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00436310 */
    f->r[0]=mi_read_00436310((0x00000004u+f->r[4]),4u);
    /* 00436314 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 00436317 */
    f->r[1]=f->r[0];
    /* 00436319 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0043631c */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0043631e */
    f->r[0]=0x51eb851fu;
    /* 00436323 */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00436326 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436327 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436328 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read_00436310((0x00799a14u+f->r[1]),2u);
    /* 0043632f */
    a=f->r[2];b=0x00000064u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00436332 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436333 */
    a=f->r[0];b=f->r[2];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 00436335 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 00436338 */
    f->r[0]=f->r[2];
    /* 0043633a */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043633b */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 0043633e */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00436340 */
    f->r[0]=0x51eb851fu;
    /* 00436345 */
    f->r[6]=f->r[2];
    /* 00436347 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read_00436310((0x00799aa4u+f->r[1]),2u);
    /* 0043634e */
    a=f->r[2];b=0x00000064u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00436351 */
    a=f->r[0];b=f->r[2];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 00436353 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 00436356 */
    f->r[0]=f->r[2];
    /* 00436358 */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 0043635b */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0043635d */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read_00436310((0x007999e8u+f->r[1]),2u);
    /* 00436364 */
    a=f->r[0];b=0xffffffe0u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00436367 */
    f->r[7]=f->r[2];
    /* 00436369 */
    a=f->r[0];b=0x000000c9u;t=mi_alu(f,a,b,32,1);
    /* 0043636e */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00436a1d;
    /* 00436374 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 00436376 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read_00436310((0x00436a88u+f->r[0]),1u))&0xffu)<<0);
    /* 0043637c */
    switch(mi_read_00436310((0x00436a28u+f->r[2]*4u),4u)){case 0x00436383u:goto mi_00436383;case 0x004363c0u:goto mi_004363c0;case 0x004363fdu:goto mi_004363fd;case 0x0043643au:goto mi_0043643a;case 0x00436477u:goto mi_00436477;case 0x004364b2u:goto mi_004364b2;case 0x004364efu:goto mi_004364ef;case 0x0043652au:goto mi_0043652a;case 0x00436565u:goto mi_00436565;case 0x004365a2u:goto mi_004365a2;case 0x00436607u:goto mi_00436607;case 0x00436643u:goto mi_00436643;case 0x004366a8u:goto mi_004366a8;case 0x00436746u:goto mi_00436746;case 0x00436783u:goto mi_00436783;case 0x004367e8u:goto mi_004367e8;case 0x00436824u:goto mi_00436824;case 0x00436860u:goto mi_00436860;case 0x0043689cu:goto mi_0043689c;case 0x00436904u:goto mi_00436904;case 0x00436941u:goto mi_00436941;case 0x00436980u:goto mi_00436980;case 0x004369bdu:goto mi_004369bd;case 0x00436a1du:goto mi_00436a1d;default:abort();}
mi_00436383:;
    /* 00436383 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 00436385 */
    if(mi_signed_less(f->flags))goto mi_00436a1d;
    /* 0043638b */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0043638e */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00436395;
    /* 00436390 */
    f->r[6]=0x00000001u;
mi_00436395:;
    /* 00436395 */
    f->r[0]=mi_read_00436310((0x0000003cu+f->r[4]),4u);
    /* 00436399 */
    f->r[1]=mi_read_00436310((0x00000038u+f->r[4]),4u);
    /* 0043639d */
    f->r[2]=mi_read_00436310((0x00f56a7cu),4u);
    /* 004363a3 */
    a=f->r[0];b=0x00000007u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004363a6 */
    a=f->r[1];b=0x00000026u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004363a9 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004363aa */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004363ac */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004363ad */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004363ae */
    f->r[1]=0x00f55bb0u;
    /* 004363b3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004363b8u;f->boundary(0x004363b3u,0x0053e590u,f);
    /* 004363b8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004363b9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004363ba */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004363bb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004363bc */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004363bf */
    f->r[4]+=4;return;
mi_004363c0:;
    /* 004363c0 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004363c2 */
    if(mi_signed_less(f->flags))goto mi_00436a1d;
    /* 004363c8 */
    a=f->r[6];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 004363cb */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004363d2;
    /* 004363cd */
    f->r[6]=0x00000004u;
mi_004363d2:;
    /* 004363d2 */
    f->r[0]=mi_read_00436310((0x0000003cu+f->r[4]),4u);
    /* 004363d6 */
    f->r[1]=mi_read_00436310((0x00000038u+f->r[4]),4u);
    /* 004363da */
    f->r[2]=mi_read_00436310((0x00f56a84u),4u);
    /* 004363e0 */
    a=f->r[0];b=0x0000000du;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004363e3 */
    a=f->r[1];b=0x0000003du;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004363e6 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004363e7 */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004363e9 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004363ea */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004363eb */
    f->r[1]=0x00f55bb0u;
    /* 004363f0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004363f5u;f->boundary(0x004363f0u,0x0053e590u,f);
    /* 004363f5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004363f6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004363f7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004363f8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004363f9 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004363fc */
    f->r[4]+=4;return;
mi_004363fd:;
    /* 004363fd */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004363ff */
    if(mi_signed_less(f->flags))goto mi_00436a1d;
    /* 00436405 */
    a=f->r[6];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 00436408 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0043640f;
    /* 0043640a */
    f->r[6]=0x00000004u;
mi_0043640f:;
    /* 0043640f */
    f->r[0]=mi_read_00436310((0x0000003cu+f->r[4]),4u);
    /* 00436413 */
    f->r[1]=mi_read_00436310((0x00000038u+f->r[4]),4u);
    /* 00436417 */
    f->r[2]=mi_read_00436310((0x00f56a6cu),4u);
    /* 0043641d */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00436420 */
    a=f->r[1];b=0x00000041u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00436423 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436424 */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00436426 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436427 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436428 */
    f->r[1]=0x00f55bb0u;
    /* 0043642d */
    f->r[4]-=4;MI_U32(f->r[4])=0x00436432u;f->boundary(0x0043642du,0x0053e590u,f);
    /* 00436432 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00436433 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00436434 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00436435 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00436436 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00436439 */
    f->r[4]+=4;return;
mi_0043643a:;
    /* 0043643a */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 0043643c */
    if(mi_signed_less(f->flags))goto mi_00436a1d;
    /* 00436442 */
    a=f->r[6];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 00436445 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0043644c;
    /* 00436447 */
    f->r[6]=0x00000004u;
mi_0043644c:;
    /* 0043644c */
    f->r[0]=mi_read_00436310((0x0000003cu+f->r[4]),4u);
    /* 00436450 */
    f->r[1]=mi_read_00436310((0x00000038u+f->r[4]),4u);
    /* 00436454 */
    f->r[2]=mi_read_00436310((0x00f56a8cu),4u);
    /* 0043645a */
    a=f->r[0];b=0x00000011u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0043645d */
    a=f->r[1];b=0x00000022u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00436460 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436461 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00436463 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436464 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436465 */
    f->r[1]=0x00f55bb0u;
    /* 0043646a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043646fu;f->boundary(0x0043646au,0x0053e590u,f);
    /* 0043646f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00436470 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00436471 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00436472 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00436473 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00436476 */
    f->r[4]+=4;return;
mi_00436477:;
    /* 00436477 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 00436479 */
    if(mi_signed_less(f->flags))goto mi_00436a1d;
    /* 0043647f */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 00436482 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00436489;
    /* 00436484 */
    f->r[6]=0x00000001u;
mi_00436489:;
    /* 00436489 */
    f->r[0]=mi_read_00436310((0x0000003cu+f->r[4]),4u);
    /* 0043648d */
    f->r[1]=mi_read_00436310((0x00000038u+f->r[4]),4u);
    /* 00436491 */
    f->r[2]=mi_read_00436310((0x00f56a5cu),4u);
    /* 00436497 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00436498 */
    a=f->r[1];b=0x0000003fu;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0043649b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043649c */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0043649e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043649f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004364a0 */
    f->r[1]=0x00f55bb0u;
    /* 004364a5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004364aau;f->boundary(0x004364a5u,0x0053e590u,f);
    /* 004364aa */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004364ab */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004364ac */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004364ad */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004364ae */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004364b1 */
    f->r[4]+=4;return;
mi_004364b2:;
    /* 004364b2 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004364b4 */
    if(mi_signed_less(f->flags))goto mi_00436a1d;
    /* 004364ba */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004364bd */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004364c4;
    /* 004364bf */
    f->r[6]=0x00000001u;
mi_004364c4:;
    /* 004364c4 */
    f->r[0]=mi_read_00436310((0x0000003cu+f->r[4]),4u);
    /* 004364c8 */
    f->r[1]=mi_read_00436310((0x00000038u+f->r[4]),4u);
    /* 004364cc */
    f->r[2]=mi_read_00436310((0x00f56a94u),4u);
    /* 004364d2 */
    a=f->r[0];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004364d5 */
    a=f->r[1];b=0x00000041u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004364d8 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004364d9 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004364db */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004364dc */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004364dd */
    f->r[1]=0x00f55bb0u;
    /* 004364e2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004364e7u;f->boundary(0x004364e2u,0x0053e590u,f);
    /* 004364e7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004364e8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004364e9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004364ea */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004364eb */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004364ee */
    f->r[4]+=4;return;
mi_004364ef:;
    /* 004364ef */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004364f1 */
    if(mi_signed_less(f->flags))goto mi_00436a1d;
    /* 004364f7 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 004364fa */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00436501;
    /* 004364fc */
    f->r[6]=0x00000002u;
mi_00436501:;
    /* 00436501 */
    f->r[0]=mi_read_00436310((0x0000003cu+f->r[4]),4u);
    /* 00436505 */
    f->r[1]=mi_read_00436310((0x00000038u+f->r[4]),4u);
    /* 00436509 */
    f->r[2]=mi_read_00436310((0x00f56a9cu),4u);
    /* 0043650f */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00436510 */
    a=f->r[1];b=0x0000002bu;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00436513 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436514 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00436516 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436517 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436518 */
    f->r[1]=0x00f55bb0u;
    /* 0043651d */
    f->r[4]-=4;MI_U32(f->r[4])=0x00436522u;f->boundary(0x0043651du,0x0053e590u,f);
    /* 00436522 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00436523 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00436524 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00436525 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00436526 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00436529 */
    f->r[4]+=4;return;
mi_0043652a:;
    /* 0043652a */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 0043652c */
    if(mi_signed_less(f->flags))goto mi_00436a1d;
    /* 00436532 */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 00436535 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0043653c;
    /* 00436537 */
    f->r[6]=0x00000001u;
mi_0043653c:;
    /* 0043653c */
    f->r[0]=mi_read_00436310((0x0000003cu+f->r[4]),4u);
    /* 00436540 */
    f->r[1]=mi_read_00436310((0x00000038u+f->r[4]),4u);
    /* 00436544 */
    f->r[2]=mi_read_00436310((0x00f56aacu),4u);
    /* 0043654a */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0043654b */
    a=f->r[1];b=0x00000029u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0043654e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043654f */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00436551 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436552 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436553 */
    f->r[1]=0x00f55bb0u;
    /* 00436558 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043655du;f->boundary(0x00436558u,0x0053e590u,f);
    /* 0043655d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0043655e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0043655f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00436560 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00436561 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00436564 */
    f->r[4]+=4;return;
mi_00436565:;
    /* 00436565 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 00436567 */
    if(mi_signed_less(f->flags))goto mi_00436a1d;
    /* 0043656d */
    a=f->r[6];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 00436570 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00436577;
    /* 00436572 */
    f->r[6]=0x00000004u;
mi_00436577:;
    /* 00436577 */
    f->r[0]=mi_read_00436310((0x0000003cu+f->r[4]),4u);
    /* 0043657b */
    f->r[1]=mi_read_00436310((0x00000038u+f->r[4]),4u);
    /* 0043657f */
    f->r[2]=mi_read_00436310((0x00f56a8cu),4u);
    /* 00436585 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00436588 */
    a=f->r[1];b=0x00000023u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0043658b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043658c */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0043658e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043658f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436590 */
    f->r[1]=0x00f55bb0u;
    /* 00436595 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043659au;f->boundary(0x00436595u,0x0053e590u,f);
    /* 0043659a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0043659b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0043659c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0043659d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0043659e */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004365a1 */
    f->r[4]+=4;return;
mi_004365a2:;
    /* 004365a2 */
    f->r[3]=mi_read_00436310((0x0000003cu+f->r[4]),4u);
    /* 004365a6 */
    f->r[5]=mi_read_00436310((0x00000038u+f->r[4]),4u);
    /* 004365aa */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 004365ac */
    if(mi_signed_less(f->flags))goto mi_004365d3;
    /* 004365ae */
    a=f->r[7];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004365b1 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004365b8;
    /* 004365b3 */
    f->r[7]=0x00000001u;
mi_004365b8:;
    /* 004365b8 */
    f->r[2]=mi_read_00436310((0x00f572c4u),4u);
    /* 004365be */
    f->r[0]=(0x00000014u+f->r[3]);
    /* 004365c1 */
    f->r[1]=(0x00000032u+f->r[5]);
    /* 004365c4 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004365c5 */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004365c7 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004365c8 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004365c9 */
    f->r[1]=0x00f55bb0u;
    /* 004365ce */
    f->r[4]-=4;MI_U32(f->r[4])=0x004365d3u;f->boundary(0x004365ceu,0x0053e590u,f);
mi_004365d3:;
    /* 004365d3 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004365d5 */
    if(mi_signed_less(f->flags))goto mi_00436a1d;
    /* 004365db */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004365de */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004365e5;
    /* 004365e0 */
    f->r[6]=0x00000001u;
mi_004365e5:;
    /* 004365e5 */
    f->r[0]=mi_read_00436310((0x00f572ccu),4u);
    /* 004365ea */
    a=f->r[3];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 004365ed */
    a=f->r[5];b=0x0000000fu;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 004365f0 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004365f1 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004365f3 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004365f4 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004365f5 */
    f->r[1]=0x00f55bb0u;
    /* 004365fa */
    f->r[4]-=4;MI_U32(f->r[4])=0x004365ffu;f->boundary(0x004365fau,0x0053e590u,f);
    /* 004365ff */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00436600 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00436601 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00436602 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00436603 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00436606 */
    f->r[4]+=4;return;
mi_00436607:;
    /* 00436607 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 00436609 */
    if(mi_signed_less(f->flags))goto mi_00436a1d;
    /* 0043660f */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 00436612 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00436619;
    /* 00436614 */
    f->r[6]=0x00000001u;
mi_00436619:;
    /* 00436619 */
    f->r[1]=mi_read_00436310((0x0000003cu+f->r[4]),4u);
    /* 0043661d */
    f->r[2]=mi_read_00436310((0x00000038u+f->r[4]),4u);
    /* 00436621 */
    f->r[0]=mi_read_00436310((0x00f572d4u),4u);
    /* 00436626 */
    a=f->r[1];b=0x00000019u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00436629 */
    a=f->r[2];b=0x0000002du;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0043662c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043662d */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0043662f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436630 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436631 */
    f->r[1]=0x00f55bb0u;
    /* 00436636 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043663bu;f->boundary(0x00436636u,0x0053e590u,f);
    /* 0043663b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0043663c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0043663d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0043663e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0043663f */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00436642 */
    f->r[4]+=4;return;
mi_00436643:;
    /* 00436643 */
    f->r[3]=mi_read_00436310((0x0000003cu+f->r[4]),4u);
    /* 00436647 */
    f->r[5]=mi_read_00436310((0x00000038u+f->r[4]),4u);
    /* 0043664b */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 0043664d */
    if(mi_signed_less(f->flags))goto mi_00436673;
    /* 0043664f */
    a=f->r[7];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 00436652 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00436659;
    /* 00436654 */
    f->r[7]=0x00000001u;
mi_00436659:;
    /* 00436659 */
    f->r[0]=mi_read_00436310((0x00f56a7cu),4u);
    /* 0043665e */
    f->r[1]=(0x00000002u+f->r[3]);
    /* 00436661 */
    f->r[2]=(0x0000005bu+f->r[5]);
    /* 00436664 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436665 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 00436667 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436668 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436669 */
    f->r[1]=0x00f55bb0u;
    /* 0043666e */
    f->r[4]-=4;MI_U32(f->r[4])=0x00436673u;f->boundary(0x0043666eu,0x0053e590u,f);
mi_00436673:;
    /* 00436673 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 00436675 */
    if(mi_signed_less(f->flags))goto mi_00436a1d;
    /* 0043667b */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0043667e */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00436685;
    /* 00436680 */
    f->r[6]=0x00000001u;
mi_00436685:;
    /* 00436685 */
    f->r[1]=mi_read_00436310((0x00f56a94u),4u);
    /* 0043668b */
    a=f->r[3];b=0xffffffefu;t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 0043668e */
    a=f->r[5];b=0x00000034u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 00436691 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436692 */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00436694 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436695 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436696 */
    f->r[1]=0x00f55bb0u;
    /* 0043669b */
    f->r[4]-=4;MI_U32(f->r[4])=0x004366a0u;f->boundary(0x0043669bu,0x0053e590u,f);
    /* 004366a0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004366a1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004366a2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004366a3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004366a4 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004366a7 */
    f->r[4]+=4;return;
mi_004366a8:;
    /* 004366a8 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004366aa */
    if(mi_signed_less(f->flags))goto mi_00436a1d;
    /* 004366b0 */
    f->r[0]=mi_read_00436310((0x00bca2e0u),4u);
    /* 004366b5 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004366b7 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read_00436310((0x00799a82u+f->r[1]),1u))&0xffu)<<0);
    /* 004366bd */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00000046u);
    /* 004366c5 */
    f->r[1]=f->r[2];
    /* 004366c7 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(0x00000019u);
    /* 004366cf */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004366d0 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004366d2 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(0x00000078u);
    /* 004366da */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004366dc */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004366de */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(0xffffffe7u);
    /* 004366e6 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(0x0000005au);
    /* 004366ee */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(0x0000000au);
    /* 004366f6 */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(0x00000028u);
    /* 004366fe */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(0xfffffffbu);
    /* 00436706 */
    if(!(f->flags&MI_SF))goto mi_0043670b;
    /* 00436708 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
mi_0043670b:;
    /* 0043670b */
    a=f->r[6];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 0043670e */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00436715;
    /* 00436710 */
    f->r[6]=0x00000004u;
mi_00436715:;
    /* 00436715 */
    f->r[0]=mi_read_00436310((0x00000014u+f->r[4]+f->r[1]*8u),4u);
    /* 00436719 */
    f->r[5]=mi_read_00436310((0x0000003cu+f->r[4]),4u);
    /* 0043671d */
    f->r[1]=mi_read_00436310((0x00000010u+f->r[4]+f->r[1]*8u),4u);
    /* 00436721 */
    f->r[7]=mi_read_00436310((0x00000038u+f->r[4]),4u);
    /* 00436725 */
    f->r[2]=mi_read_00436310((0x00f56a6cu),4u);
    /* 0043672b */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0043672d */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0043672f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436730 */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00436732 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436733 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436734 */
    f->r[1]=0x00f55bb0u;
    /* 00436739 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043673eu;f->boundary(0x00436739u,0x0053e590u,f);
    /* 0043673e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0043673f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00436740 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00436741 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00436742 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00436745 */
    f->r[4]+=4;return;
mi_00436746:;
    /* 00436746 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 00436748 */
    if(mi_signed_less(f->flags))goto mi_00436a1d;
    /* 0043674e */
    a=f->r[6];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 00436751 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00436758;
    /* 00436753 */
    f->r[6]=0x00000004u;
mi_00436758:;
    /* 00436758 */
    f->r[0]=mi_read_00436310((0x0000003cu+f->r[4]),4u);
    /* 0043675c */
    f->r[1]=mi_read_00436310((0x00000038u+f->r[4]),4u);
    /* 00436760 */
    f->r[2]=mi_read_00436310((0x00f56a6cu),4u);
    /* 00436766 */
    a=f->r[0];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00436769 */
    a=f->r[1];b=0x00000029u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0043676c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043676d */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0043676f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436770 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436771 */
    f->r[1]=0x00f55bb0u;
    /* 00436776 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043677bu;f->boundary(0x00436776u,0x0053e590u,f);
    /* 0043677b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0043677c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0043677d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0043677e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0043677f */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00436782 */
    f->r[4]+=4;return;
mi_00436783:;
    /* 00436783 */
    f->r[3]=mi_read_00436310((0x0000003cu+f->r[4]),4u);
    /* 00436787 */
    f->r[5]=mi_read_00436310((0x00000038u+f->r[4]),4u);
    /* 0043678b */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 0043678d */
    if(mi_signed_less(f->flags))goto mi_004367b4;
    /* 0043678f */
    a=f->r[7];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 00436792 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00436799;
    /* 00436794 */
    f->r[7]=0x00000004u;
mi_00436799:;
    /* 00436799 */
    f->r[2]=mi_read_00436310((0x00f56a94u),4u);
    /* 0043679f */
    f->r[0]=(0x00000007u+f->r[3]);
    /* 004367a2 */
    f->r[1]=(0x00000001u+f->r[5]);
    /* 004367a5 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004367a6 */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004367a8 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004367a9 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004367aa */
    f->r[1]=0x00f55bb0u;
    /* 004367af */
    f->r[4]-=4;MI_U32(f->r[4])=0x004367b4u;f->boundary(0x004367afu,0x0053e590u,f);
mi_004367b4:;
    /* 004367b4 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004367b6 */
    if(mi_signed_less(f->flags))goto mi_00436a1d;
    /* 004367bc */
    a=f->r[6];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 004367bf */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004367c6;
    /* 004367c1 */
    f->r[6]=0x00000004u;
mi_004367c6:;
    /* 004367c6 */
    f->r[0]=mi_read_00436310((0x00f572f4u),4u);
    /* 004367cb */
    a=f->r[3];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 004367ce */
    a=f->r[5];b=0x0000002eu;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 004367d1 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004367d2 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004367d4 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004367d5 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004367d6 */
    f->r[1]=0x00f55bb0u;
    /* 004367db */
    f->r[4]-=4;MI_U32(f->r[4])=0x004367e0u;f->boundary(0x004367dbu,0x0053e590u,f);
    /* 004367e0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004367e1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004367e2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004367e3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004367e4 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004367e7 */
    f->r[4]+=4;return;
mi_004367e8:;
    /* 004367e8 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004367ea */
    if(mi_signed_less(f->flags))goto mi_00436a1d;
    /* 004367f0 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 004367f3 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004367fa;
    /* 004367f5 */
    f->r[6]=0x00000002u;
mi_004367fa:;
    /* 004367fa */
    f->r[1]=mi_read_00436310((0x0000003cu+f->r[4]),4u);
    /* 004367fe */
    f->r[2]=mi_read_00436310((0x00000038u+f->r[4]),4u);
    /* 00436802 */
    f->r[0]=mi_read_00436310((0x00f565fcu),4u);
    /* 00436807 */
    a=f->r[1];b=0x0000000fu;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0043680a */
    a=f->r[2];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0043680d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043680e */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00436810 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436811 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436812 */
    f->r[1]=0x00f55bb0u;
    /* 00436817 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043681cu;f->boundary(0x00436817u,0x0053e590u,f);
    /* 0043681c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0043681d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0043681e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0043681f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00436820 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00436823 */
    f->r[4]+=4;return;
mi_00436824:;
    /* 00436824 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 00436826 */
    if(mi_signed_less(f->flags))goto mi_00436a1d;
    /* 0043682c */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0043682f */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00436836;
    /* 00436831 */
    f->r[6]=0x00000002u;
mi_00436836:;
    /* 00436836 */
    f->r[1]=mi_read_00436310((0x0000003cu+f->r[4]),4u);
    /* 0043683a */
    f->r[2]=mi_read_00436310((0x00000038u+f->r[4]),4u);
    /* 0043683e */
    f->r[0]=mi_read_00436310((0x00f565fcu),4u);
    /* 00436843 */
    a=f->r[1];b=0xfffffff3u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00436846 */
    a=f->r[2];b=0x0000003du;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00436849 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043684a */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0043684c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043684d */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043684e */
    f->r[1]=0x00f55bb0u;
    /* 00436853 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00436858u;f->boundary(0x00436853u,0x0053e590u,f);
    /* 00436858 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00436859 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0043685a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0043685b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0043685c */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043685f */
    f->r[4]+=4;return;
mi_00436860:;
    /* 00436860 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 00436862 */
    if(mi_signed_less(f->flags))goto mi_00436a1d;
    /* 00436868 */
    a=f->r[6];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 0043686b */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00436872;
    /* 0043686d */
    f->r[6]=0x00000004u;
mi_00436872:;
    /* 00436872 */
    f->r[1]=mi_read_00436310((0x0000003cu+f->r[4]),4u);
    /* 00436876 */
    f->r[2]=mi_read_00436310((0x00000038u+f->r[4]),4u);
    /* 0043687a */
    f->r[0]=mi_read_00436310((0x00f56a54u),4u);
    /* 0043687f */
    a=f->r[1];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00436882 */
    a=f->r[2];b=0x00000031u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00436885 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436886 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00436888 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436889 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043688a */
    f->r[1]=0x00f55bb0u;
    /* 0043688f */
    f->r[4]-=4;MI_U32(f->r[4])=0x00436894u;f->boundary(0x0043688fu,0x0053e590u,f);
    /* 00436894 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00436895 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00436896 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00436897 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00436898 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043689b */
    f->r[4]+=4;return;
mi_0043689c:;
    /* 0043689c */
    f->r[3]=mi_read_00436310((0x0000003cu+f->r[4]),4u);
    /* 004368a0 */
    f->r[5]=mi_read_00436310((0x00000038u+f->r[4]),4u);
    /* 004368a4 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004368a6 */
    if(mi_signed_less(f->flags))goto mi_004368cf;
    /* 004368a8 */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004368ab */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004368b2;
    /* 004368ad */
    f->r[6]=0x00000001u;
mi_004368b2:;
    /* 004368b2 */
    f->r[0]=mi_read_00436310((0x00f56a64u),4u);
    /* 004368b7 */
    f->r[1]=(0x00000002u+f->r[3]);
    /* 004368ba */
    f->r[2]=(0x00000083u+f->r[5]);
    /* 004368c0 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004368c1 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004368c3 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004368c4 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004368c5 */
    f->r[1]=0x00f55bb0u;
    /* 004368ca */
    f->r[4]-=4;MI_U32(f->r[4])=0x004368cfu;f->boundary(0x004368cau,0x0053e590u,f);
mi_004368cf:;
    /* 004368cf */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 004368d1 */
    if(mi_signed_less(f->flags))goto mi_00436a1d;
    /* 004368d7 */
    a=f->r[7];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004368da */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004368e1;
    /* 004368dc */
    f->r[7]=0x00000001u;
mi_004368e1:;
    /* 004368e1 */
    f->r[1]=mi_read_00436310((0x00f56aa4u),4u);
    /* 004368e7 */
    a=f->r[3];b=0x0000000au;t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 004368ea */
    a=f->r[5];b=0x0000004au;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 004368ed */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004368ee */
    a=f->r[7];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004368f0 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004368f1 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004368f2 */
    f->r[1]=0x00f55bb0u;
    /* 004368f7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004368fcu;f->boundary(0x004368f7u,0x0053e590u,f);
    /* 004368fc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004368fd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004368fe */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004368ff */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00436900 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00436903 */
    f->r[4]+=4;return;
mi_00436904:;
    /* 00436904 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 00436906 */
    if(mi_signed_less(f->flags))goto mi_00436a1d;
    /* 0043690c */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0043690f */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00436916;
    /* 00436911 */
    f->r[6]=0x00000001u;
mi_00436916:;
    /* 00436916 */
    f->r[2]=mi_read_00436310((0x0000003cu+f->r[4]),4u);
    /* 0043691a */
    f->r[0]=mi_read_00436310((0x00000038u+f->r[4]),4u);
    /* 0043691e */
    f->r[1]=mi_read_00436310((0x00f56a74u),4u);
    /* 00436924 */
    a=f->r[2];b=0x00000013u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00436927 */
    a=f->r[0];b=0x00000045u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0043692a */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043692b */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0043692d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043692e */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043692f */
    f->r[1]=0x00f55bb0u;
    /* 00436934 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00436939u;f->boundary(0x00436934u,0x0053e590u,f);
    /* 00436939 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0043693a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0043693b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0043693c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0043693d */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00436940 */
    f->r[4]+=4;return;
mi_00436941:;
    /* 00436941 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 00436943 */
    if(mi_signed_less(f->flags))goto mi_00436a1d;
    /* 00436949 */
    a=f->r[6];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 0043694c */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00436953;
    /* 0043694e */
    f->r[6]=0x00000004u;
mi_00436953:;
    /* 00436953 */
    f->r[2]=mi_read_00436310((0x0000003cu+f->r[4]),4u);
    /* 00436957 */
    f->r[0]=mi_read_00436310((0x00000038u+f->r[4]),4u);
    /* 0043695b */
    f->r[1]=mi_read_00436310((0x00f56a6cu),4u);
    /* 00436961 */
    a=f->r[2];b=0xffffffe9u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00436964 */
    a=f->r[0];b=0x000000a6u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00436969 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043696a */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0043696c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043696d */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043696e */
    f->r[1]=0x00f55bb0u;
    /* 00436973 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00436978u;f->boundary(0x00436973u,0x0053e590u,f);
    /* 00436978 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00436979 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0043697a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0043697b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0043697c */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043697f */
    f->r[4]+=4;return;
mi_00436980:;
    /* 00436980 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 00436982 */
    if(mi_signed_less(f->flags))goto mi_00436a1d;
    /* 00436988 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0043698b */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00436992;
    /* 0043698d */
    f->r[6]=0x00000002u;
mi_00436992:;
    /* 00436992 */
    f->r[2]=mi_read_00436310((0x0000003cu+f->r[4]),4u);
    /* 00436996 */
    f->r[0]=mi_read_00436310((0x00000038u+f->r[4]),4u);
    /* 0043699a */
    f->r[1]=mi_read_00436310((0x00f56a4cu),4u);
    /* 004369a0 */
    a=f->r[2];b=0x00000005u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004369a3 */
    a=f->r[0];b=0x0000006bu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004369a6 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004369a7 */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004369a9 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004369aa */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004369ab */
    f->r[1]=0x00f55bb0u;
    /* 004369b0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004369b5u;f->boundary(0x004369b0u,0x0053e590u,f);
    /* 004369b5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004369b6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004369b7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004369b8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004369b9 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004369bc */
    f->r[4]+=4;return;
mi_004369bd:;
    /* 004369bd */
    f->r[3]=mi_read_00436310((0x0000003cu+f->r[4]),4u);
    /* 004369c1 */
    f->r[5]=mi_read_00436310((0x00000038u+f->r[4]),4u);
    /* 004369c5 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004369c7 */
    if(mi_signed_less(f->flags))goto mi_004369f1;
    /* 004369c9 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 004369cc */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004369d3;
    /* 004369ce */
    f->r[6]=0x00000002u;
mi_004369d3:;
    /* 004369d3 */
    f->r[1]=mi_read_00436310((0x00f56a84u),4u);
    /* 004369d9 */
    f->r[2]=(0xfffffff6u+f->r[3]);
    /* 004369dc */
    f->r[0]=(0x000000c8u+f->r[5]);
    /* 004369e2 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004369e3 */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004369e5 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004369e6 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004369e7 */
    f->r[1]=0x00f55bb0u;
    /* 004369ec */
    f->r[4]-=4;MI_U32(f->r[4])=0x004369f1u;f->boundary(0x004369ecu,0x0053e590u,f);
mi_004369f1:;
    /* 004369f1 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 004369f3 */
    if(mi_signed_less(f->flags))goto mi_00436a1d;
    /* 004369f5 */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 004369f8 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004369ff;
    /* 004369fa */
    f->r[7]=0x00000002u;
mi_004369ff:;
    /* 004369ff */
    f->r[2]=mi_read_00436310((0x00f56a7cu),4u);
    /* 00436a05 */
    a=f->r[3];b=0x0000000au;t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 00436a08 */
    a=f->r[5];b=0x000000dcu;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 00436a0e */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436a0f */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 00436a11 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436a12 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436a13 */
    f->r[1]=0x00f55bb0u;
    /* 00436a18 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00436a1du;f->boundary(0x00436a18u,0x0053e590u,f);
mi_00436a1d:;
    /* 00436a1d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00436a1e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00436a1f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00436a20 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00436a21 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00436a24 */
    f->r[4]+=4;return;
}


void restore_building_world_0047fd30(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0047fd30 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047fd31 */
    f->r[1]=MI_U32((0x00000008u+f->r[4]));
    /* 0047fd35 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047fd36 */
    f->r[0]=f->r[1];
    /* 0047fd38 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047fd39 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0047fd3c */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0047fd3e */
    f->r[1]=MI_U32((0x00000014u+f->r[4]));
    /* 0047fd42 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0047fd45 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047fd46 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047fd47 */
    f->r[6]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[0]));
    /* 0047fd4e */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[0]));
    /* 0047fd55 */
    f->r[7]=(0x00000000u+f->r[6]+f->r[1]*1u);
    /* 0047fd58 */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 0047fd5a */
    f->r[3]=(0x00000000u+f->r[0]+f->r[1]*1u);
    /* 0047fd5d */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0047fd5f */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 0047fd61 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 0047fd65 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[3]);
    /* 0047fd69 */
    if(!mi_signed_less(f->flags))goto mi_0047fd6d;
    /* 0047fd6b */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
mi_0047fd6d:;
    /* 0047fd6d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0047fd6f */
    if(!mi_signed_less(f->flags))goto mi_0047fd73;
    /* 0047fd71 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_0047fd73:;
    /* 0047fd73 */
    f->r[1]=MI_U32((0x00bca2d0u));
    /* 0047fd79 */
    a=f->r[7];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0047fd7b */
    if(mi_signed_less(f->flags))goto mi_0047fd84;
    /* 0047fd7d */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0047fd7e */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0047fd82 */
    f->r[7]=f->r[1];
mi_0047fd84:;
    /* 0047fd84 */
    f->r[1]=MI_U32((0x00bca2d4u));
    /* 0047fd8a */
    a=f->r[3];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0047fd8c */
    if(mi_signed_less(f->flags))goto mi_0047fd95;
    /* 0047fd8e */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0047fd8f */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0047fd93 */
    f->r[3]=f->r[1];
mi_0047fd95:;
    /* 0047fd95 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 0047fd98 */
    f->r[5]=f->r[6];
    /* 0047fd9a */
    a=f->r[5];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[5]=t;
    /* 0047fd9c */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 0047fda4 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[1]*2u);
    /* 0047fda7 */
    a=f->r[5];b=0x000000e3u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 0047fdad */
    MI_U32((0x00d3b4e0u))=(uint32_t)(f->r[0]);
    /* 0047fdb2 */
    f->r[2]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 0047fdb5 */
    f->r[1]=MI_U32((0x00bca2d8u));
    /* 0047fdbb */
    f->r[2]=(0x00000000u+f->r[1]+f->r[2]*4u);
    /* 0047fdbe */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0047fdc0 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 0047fdc2 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0047fe3e;
mi_0047fdc4:;
    /* 0047fdc4 */
    f->r[0]=MI_U32((0x00000018u+f->r[4]));
    /* 0047fdc8 */
    f->r[7]=f->r[6];
    /* 0047fdca */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0047fdcc */
    MI_U32((0x00d3b4e4u))=(uint32_t)(f->r[7]);
    /* 0047fdd2 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0047fe20;
    /* 0047fdd4 */
    f->r[3]=(0x00b4e6fcu+f->r[2]*2u);
mi_0047fddb:;
    /* 0047fddb */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[3]));
    /* 0047fdde */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0047fde0 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0047fe09;
    /* 0047fde2 */
    f->r[0]=f->r[1];
    /* 0047fde4 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0047fde7 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0047fde9 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0047fdec */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x007999ddu+f->r[0])))&0xffu)<<0);
    /* 0047fdf2 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0047fdf4 */
    if((f->flags&MI_ZF))goto mi_0047fe09;
    /* 0047fdf6 */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
    /* 0047fdfa */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0047fdfb */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0047fdff */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00799a4fu+f->r[0])))&0xffu)<<0);
    /* 0047fe05 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0047fe07 */
    if((f->flags&MI_ZF))goto mi_0047fe49;
mi_0047fe09:;
    /* 0047fe09 */
    f->r[0]=MI_U32((0x00000018u+f->r[4]));
    /* 0047fe0d */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 0047fe0e */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0047fe0f */
    a=f->r[3];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 0047fe12 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0047fe14 */
    MI_U32((0x00d3b4e4u))=(uint32_t)(f->r[7]);
    /* 0047fe1a */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0047fddb;
    /* 0047fe1c */
    f->r[3]=MI_U32((0x0000001cu+f->r[4]));
mi_0047fe20:;
    /* 0047fe20 */
    f->r[0]=MI_U32((0x00d3b4e0u));
    /* 0047fe25 */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0047fe27 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0047fe28 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 0047fe2a */
    MI_U32((0x00d3b4e0u))=(uint32_t)(f->r[0]);
    /* 0047fe2f */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0047fdc4;
    /* 0047fe31 */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 0047fe35 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0047fe37 */
    f->r[0]=0x00000001u;
    /* 0047fe3c */
    if(!(f->flags&MI_ZF))goto mi_0047fe43;
mi_0047fe3e:;
    /* 0047fe3e */
    f->r[0]=0x00000002u;
mi_0047fe43:;
    /* 0047fe43 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0047fe44 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0047fe45 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0047fe46 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0047fe47 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0047fe48 */
    f->r[4]+=4;return;
mi_0047fe49:;
    /* 0047fe49 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0047fe4a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0047fe4b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0047fe4c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0047fe4e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0047fe4f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0047fe50 */
    f->r[4]+=4;return;
}


static const uint8_t mi_rom_004c03a8[]={0x43,0x02,0x4c,0x00,0x1c,0x02,0x4c,0x00,0x1c,0x02,0x4c,0x00,0x43,0x02,0x4c,0x00,0x43,0x02,0x4c,0x00,0x1c,0x02,0x4c,0x00,0x1c,0x02,0x4c,0x00,0x43,0x02,0x4c,0x00};
static __inline uint32_t mi_read_004c0160(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x004c03a8u&&width<=32u&&(offset=address-0x004c03a8u)<=32u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004c03a8[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}

void restore_building_world_004c0160(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004c0160 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 004c0163 */
    f->r[0]=mi_read_004c0160((0x00000024u+f->r[4]),4u);
    /* 004c0167 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0168 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0169 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c016a */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c016b */
    f->r[7]=f->r[0];
    /* 004c016d */
    a=f->r[7];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 004c0170 */
    f->r[1]=mi_read_004c0160((0x00bca2e0u),4u);
    /* 004c0176 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004c0178 */
    a=f->r[7];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 004c017b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004c017d */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c017f */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read_004c0160((0x00799a82u+f->r[7]),1u))&0xffu)<<0);
    /* 004c0185 */
    f->r[3]=0x00000040u;
    /* 004c018a */
    f->r[6]=(uint32_t)(int32_t)(int16_t)mi_read_004c0160((0x007999deu+f->r[7]),2u);
    /* 004c0191 */
    f->r[5]=f->r[0];
    /* 004c0193 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(0xffffffa6u);
    /* 004c019b */
    a=f->r[5];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 004c019d */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[6]);
    /* 004c01a1 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read_004c0160((0x007999e0u+f->r[7]),2u);
    /* 004c01a8 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 004c01ac */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004c01b0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c01b5u;f->boundary(0x004c01b0u,0x004c53d0u,f);
    /* 004c01b5 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c01b8 */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,1);
    /* 004c01bb */
    if(!(f->flags&MI_ZF))goto mi_004c01ce;
    /* 004c01bd */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c01bf */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c01c4u;f->boundary(0x004c01bfu,0x004c53d0u,f);
    /* 004c01c4 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c01c7 */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,1);
    /* 004c01ca */
    if(!(f->flags&MI_ZF))goto mi_004c01ce;
    /* 004c01cc */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_004c01ce:;
    /* 004c01ce */
    f->r[2]=mi_read_004c0160((0x005df778u+f->r[0]*4u),4u);
    /* 004c01d5 */
    a=f->r[5];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 004c01d8 */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004c01dc */
    if(mi_signed_less(f->flags))goto mi_004c01e5;
    /* 004c01de */
    a=f->r[5];b=0x00000008u;t=mi_alu(f,a,b,32,1);f->r[5]=t;
    /* 004c01e1 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[5]);
mi_004c01e5:;
    /* 004c01e5 */
    f->r[0]=f->r[5];
    /* 004c01e7 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(0x00000002u);
    /* 004c01ef */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004c01f2 */
    a=f->r[0];b=0x005df78cu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004c01f7 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004c01fb */
    goto mi_004c0209;
mi_004c01fd:;
    /* 004c01fd */
    f->r[0]=mi_read_004c0160((0x00000024u+f->r[4]),4u);
    /* 004c0201 */
    f->r[6]=mi_read_004c0160((0x00000028u+f->r[4]),4u);
    /* 004c0205 */
    f->r[5]=mi_read_004c0160((0x00000020u+f->r[4]),4u);
mi_004c0209:;
    /* 004c0209 */
    f->r[1]=mi_read_004c0160((0x00000014u+f->r[4]),4u);
    /* 004c020d */
    a=f->r[5];b=0x00000007u;t=mi_alu(f,a,b,32,1);
    /* 004c0210 */
    f->r[2]=mi_read_004c0160((0x00000000u+f->r[0]+f->r[1]*4u),4u);
    /* 004c0213 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004c0229;
    /* 004c0215 */
    switch(mi_read_004c0160((0x004c03a8u+f->r[5]*4u),4u)){case 0x004c021cu:goto mi_004c021c;case 0x004c0243u:goto mi_004c0243;default:abort();}
mi_004c021c:;
    /* 004c021c */
    f->r[3]=0x0000000cu;
    /* 004c0221 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(0xffffffc0u);
mi_004c0229:;
    /* 004c0229 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
mi_004c022b:;
    /* 004c022b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004c022d */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read_004c0160((0x00799a82u+f->r[7]),1u))&0xffu)<<0);
    /* 004c0233 */
    a=f->r[0];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004c0236 */
    if((f->flags&MI_ZF))goto mi_004c026a;
    /* 004c0238 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004c0239 */
    if((f->flags&MI_ZF))goto mi_004c0255;
    /* 004c023b */
    f->r[0]=mi_read_004c0160((0x00000018u+f->r[4]),4u);
    /* 004c023f */
    f->r[1]=f->r[6];
    /* 004c0241 */
    goto mi_004c0281;
mi_004c0243:;
    /* 004c0243 */
    a=f->r[3];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[3]=t;
    /* 004c0246 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(0xffffffbfu);
    /* 004c024e */
    f->r[5]=0x00000001u;
    /* 004c0253 */
    goto mi_004c022b;
mi_004c0255:;
    /* 004c0255 */
    f->r[0]=f->r[2];
    /* 004c0257 */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004c025a */
    f->r[1]=mi_read_004c0160((0x005df818u+f->r[0]),4u);
    /* 004c0260 */
    f->r[0]=mi_read_004c0160((0x005df81cu+f->r[0]),4u);
    /* 004c0266 */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004c0268 */
    goto mi_004c027d;
mi_004c026a:;
    /* 004c026a */
    f->r[0]=f->r[2];
    /* 004c026c */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004c026f */
    f->r[1]=mi_read_004c0160((0x005df81cu+f->r[0]),4u);
    /* 004c0275 */
    f->r[0]=mi_read_004c0160((0x005df818u+f->r[0]),4u);
    /* 004c027b */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[1]=t;
mi_004c027d:;
    /* 004c027d */
    a=f->r[0];b=mi_read_004c0160((0x00000018u+f->r[4]),4u);t=mi_alu(f,a,b,32,0);f->r[0]=t;
mi_004c0281:;
    /* 004c0281 */
    a=mi_read_004c0160((0x00000014u+f->r[4]),4u);b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 004c0286 */
    if(!(f->flags&MI_ZF))goto mi_004c02e7;
    /* 004c0288 */
    f->r[2]=mi_read_004c0160((0x00f56afcu),4u);
    /* 004c028e */
    f->r[6]=(0x00000000u+f->r[2]+f->r[5]*1u);
    /* 004c0291 */
    f->r[2]=(0x00000034u+f->r[4]);
    /* 004c0295 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0296 */
    f->r[2]=(0x00000014u+f->r[4]);
    /* 004c029a */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c029b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c029c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c029d */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c02a2u;f->boundary(0x004c029du,0x004f0ba0u,f);
    /* 004c02a2 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read_004c0160((0x007999d8u+f->r[7]),1u))&0xffu)<<0);
    /* 004c02a8 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c02ab */
    a=(f->r[0]&0xffu);b=0x00000006u;t=mi_alu(f,a,b,8,1);
    /* 004c02ad */
    if(!(f->flags&MI_ZF))goto mi_004c02b8;
    /* 004c02af */
    MI_U16((0x0070e120u))=(uint16_t)(0x0000f863u);
mi_004c02b8:;
    /* 004c02b8 */
    f->r[0]=mi_read_004c0160((0x00000034u+f->r[4]),4u);
    /* 004c02bc */
    f->r[5]=mi_read_004c0160((0x0000001cu+f->r[4]),4u);
    /* 004c02c0 */
    f->r[1]=mi_read_004c0160((0x00000010u+f->r[4]),4u);
    /* 004c02c4 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004c02c6 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004c02c8 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c02c9 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c02ca */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c02cb */
    f->r[1]=0x00f55bb0u;
    /* 004c02d0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c02d5u;f->boundary(0x004c02d0u,0x0053e590u,f);
    /* 004c02d5 */
    f->r[2]=mi_read_004c0160((0x00000034u+f->r[4]),4u);
    /* 004c02d9 */
    f->r[0]=mi_read_004c0160((0x00000010u+f->r[4]),4u);
    /* 004c02dd */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004c02df */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004c02e1 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c02e2 */
    goto mi_004c0378;
mi_004c02e7:;
    /* 004c02e7 */
    f->r[6]=mi_read_004c0160((0x00bca2e0u),4u);
    /* 004c02ed */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004c02ef */
    if((f->flags&MI_ZF))goto mi_004c0326;
    /* 004c02f1 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 004c02f4 */
    if(!(f->flags&MI_ZF))goto mi_004c0305;
    /* 004c02f6 */
    a=f->r[2];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004c02f9 */
    f->r[2]=mi_read_004c0160((0x005df814u+f->r[2]),4u);
    /* 004c02ff */
    f->r[1]=(0xffffffffu+f->r[1]+f->r[2]*1u);
    /* 004c0303 */
    goto mi_004c0326;
mi_004c0305:;
    /* 004c0305 */
    a=f->r[6];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 004c0308 */
    if(!(f->flags&MI_ZF))goto mi_004c0319;
    /* 004c030a */
    a=f->r[2];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004c030d */
    f->r[2]=mi_read_004c0160((0x005df814u+f->r[2]),4u);
    /* 004c0313 */
    f->r[1]=(0xffffffffu+f->r[1]+f->r[2]*1u);
    /* 004c0317 */
    goto mi_004c0322;
mi_004c0319:;
    /* 004c0319 */
    a=f->r[2];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004c031c */
    f->r[2]=mi_read_004c0160((0x005df814u+f->r[2]),4u);
mi_004c0322:;
    /* 004c0322 */
    f->r[0]=(0xffffffffu+f->r[0]+f->r[2]*1u);
mi_004c0326:;
    /* 004c0326 */
    f->r[2]=mi_read_004c0160((0x0000002cu+f->r[4]),4u);
    /* 004c032a */
    f->r[6]=mi_read_004c0160((0x00f55bb4u+f->r[2]*8u),4u);
    /* 004c0331 */
    f->r[2]=(0x00000034u+f->r[4]);
    /* 004c0335 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0336 */
    f->r[2]=(0x00000014u+f->r[4]);
    /* 004c033a */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c033b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c033c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c033d */
    a=f->r[6];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004c033f */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c0344u;f->boundary(0x004c033fu,0x004f0ba0u,f);
    /* 004c0344 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read_004c0160((0x007999d8u+f->r[7]),1u))&0xffu)<<0);
    /* 004c034a */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c034d */
    a=(f->r[0]&0xffu);b=0x00000006u;t=mi_alu(f,a,b,8,1);
    /* 004c034f */
    if(!(f->flags&MI_ZF))goto mi_004c035a;
    /* 004c0351 */
    MI_U16((0x0070e120u))=(uint16_t)(0x0000f863u);
mi_004c035a:;
    /* 004c035a */
    f->r[0]=mi_read_004c0160((0x00000034u+f->r[4]),4u);
    /* 004c035e */
    f->r[1]=mi_read_004c0160((0x00000010u+f->r[4]),4u);
    /* 004c0362 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0363 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0364 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0365 */
    f->r[1]=0x00f55bb0u;
    /* 004c036a */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c036fu;f->boundary(0x004c036au,0x0053e590u,f);
    /* 004c036f */
    f->r[2]=mi_read_004c0160((0x00000034u+f->r[4]),4u);
    /* 004c0373 */
    f->r[0]=mi_read_004c0160((0x00000010u+f->r[4]),4u);
    /* 004c0377 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
mi_004c0378:;
    /* 004c0378 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0379 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c037a */
    f->r[1]=0x00f55bb0u;
    /* 004c037f */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c0384u;f->boundary(0x004c037fu,0x0053f250u,f);
    /* 004c0384 */
    f->r[0]=mi_read_004c0160((0x00000014u+f->r[4]),4u);
    /* 004c0388 */
    MI_U16((0x0070e120u))=(uint16_t)(0x00000000u);
    /* 004c0391 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004c0392 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 004c0395 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004c0399 */
    if(mi_signed_less(f->flags))goto mi_004c01fd;
    /* 004c039f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004c03a0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004c03a1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004c03a2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004c03a3 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c03a6 */
    f->r[4]+=4;return;
}


static const uint8_t mi_rom_004efd3c[]={0x32,0xf9,0x4e,0x00,0x20,0xf9,0x4e,0x00,0x32,0xf9,0x4e,0x00,0x27,0xf9,0x4e,0x00,0x2e,0xf9,0x4e,0x00};
static const uint8_t mi_rom_004efd50[]={0x83,0xf9,0x4e,0x00,0x53,0xf9,0x4e,0x00,0x83,0xf9,0x4e,0x00,0x5a,0xf9,0x4e,0x00,0x85,0xf9,0x4e,0x00};
static const uint8_t mi_rom_004efd64[]={0x3e,0xfb,0x4e,0x00,0x29,0xfb,0x4e,0x00,0x3e,0xfb,0x4e,0x00,0x30,0xfb,0x4e,0x00,0x37,0xfb,0x4e,0x00};
static const uint8_t mi_rom_004efd78[]={0x9e,0xfb,0x4e,0x00,0x5f,0xfb,0x4e,0x00,0x9e,0xfb,0x4e,0x00,0x66,0xfb,0x4e,0x00,0x6d,0xfb,0x4e,0x00};
static __inline uint32_t mi_read_004ef550(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x004efd3cu&&width<=20u&&(offset=address-0x004efd3cu)<=20u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004efd3c[offset+k]<<(k*8u);return result;
 }
 if(address>=0x004efd50u&&width<=20u&&(offset=address-0x004efd50u)<=20u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004efd50[offset+k]<<(k*8u);return result;
 }
 if(address>=0x004efd64u&&width<=20u&&(offset=address-0x004efd64u)<=20u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004efd64[offset+k]<<(k*8u);return result;
 }
 if(address>=0x004efd78u&&width<=20u&&(offset=address-0x004efd78u)<=20u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004efd78[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}

static __inline void mi_idiv32(struct restore_building_world_frame *f,uint32_t divisor_bits)
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

void restore_building_world_004ef550(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004ef550 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 004ef553 */
    f->r[0]=mi_read_004ef550((0x00000030u+f->r[4]),4u);
    /* 004ef557 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ef558 */
    f->r[3]=f->r[0];
    /* 004ef55a */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ef55b */
    a=f->r[3];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[3]=t;
    /* 004ef55e */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 004ef560 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004ef562 */
    a=f->r[3];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[3]=t;
    /* 004ef565 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ef566 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ef567 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read_004ef550((0x00799a2cu+f->r[3]),1u))&0xffu)<<0);
    /* 004ef56d */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read_004ef550((0x00799a2fu+f->r[3]),1u))&0xffu)<<0);
    /* 004ef573 */
    f->r[7]=f->r[0];
    /* 004ef575 */
    f->r[0]=mi_read_004ef550((0x005e4ef4u),4u);
    /* 004ef57a */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004ef57d */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 004ef585 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[3]);
    /* 004ef589 */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[7]);
    /* 004ef58d */
    if(!(f->flags&MI_ZF))goto mi_004ef5a5;
    /* 004ef58f */
    f->r[0]=f->r[1];
    /* 004ef591 */
    f->r[2]=(0x00000000u+f->r[7]+f->r[7]*2u);
    /* 004ef594 */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004ef599 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[2]*2u);
    /* 004ef59c */
    f->r[2]=mi_read_004ef550((0x005e68f8u+f->r[2]*4u),4u);
    /* 004ef5a3 */
    goto mi_004ef5b4;
mi_004ef5a5:;
    /* 004ef5a5 */
    f->r[0]=f->r[1];
    /* 004ef5a7 */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004ef5ac */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 004ef5af */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 004ef5b2 */
    a=f->r[2];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
mi_004ef5b4:;
    /* 004ef5b4 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read_004ef550((0x00799a2du+f->r[3]),1u))&0xffu)<<0);
    /* 004ef5ba */
    f->r[5]=mi_read_004ef550((0x00000038u+f->r[4]),4u);
    /* 004ef5be */
    f->r[6]=mi_read_004ef550((0x0000003cu+f->r[4]),4u);
    /* 004ef5c2 */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004ef5c4 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004ef5c8 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 004ef5cc */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 004ef5d0 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004ef61a;
    /* 004ef5d2 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004ef5d4 */
    if(!(f->flags&MI_ZF))goto mi_004ef5f7;
    /* 004ef5d6 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004ef5d8 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004ef5f7;
    /* 004ef5da */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read_004ef550((0x00799a2eu+f->r[3]),1u))&0xffu)<<0);
    /* 004ef5e0 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004ef5e2 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004ef5f7;
    /* 004ef5e4 */
    a=mi_read_004ef550((0x005e4ef4u),4u);b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 004ef5eb */
    if((f->flags&MI_ZF))goto mi_004ef5f7;
    /* 004ef5ed */
    a=mi_read_004ef550((0x007999eau+f->r[3]),2u);b=0x00000004u;t=mi_alu(f,a,b,16,1);
    /* 004ef5f5 */
    if(!mi_signed_less(f->flags))goto mi_004ef61a;
mi_004ef5f7:;
    /* 004ef5f7 */
    f->r[0]=mi_read_004ef550((0x0070e45cu),4u);
    /* 004ef5fc */
    f->r[1]=mi_read_004ef550((0x00000038u+f->r[4]),4u);
    /* 004ef600 */
    f->r[6]=mi_read_004ef550((0x0000003cu+f->r[4]),4u);
    /* 004ef604 */
    f->r[5]=(0x00000001u+f->r[0]+f->r[1]*1u);
    /* 004ef608 */
    f->r[1]=mi_read_004ef550((0x0070e44cu),4u);
    /* 004ef60e */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 004ef610 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 004ef614 */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 004ef616 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[6]);
mi_004ef61a:;
    /* 004ef61a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004ef61c */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read_004ef550((0x00799a31u+f->r[3]),1u))&0xffu)<<0);
    /* 004ef622 */
    a=f->r[0];b=0x00000007u;t=mi_alu(f,a,b,32,1);
    /* 004ef625 */
    if(!mi_signed_less(f->flags))goto mi_004ef631;
    /* 004ef627 */
    MI_U8((0x00000040u+f->r[4]))=(uint8_t)(0x00000000u);
    /* 004ef62c */
    goto mi_004ef75d;
mi_004ef631:;
    /* 004ef631 */
    f->r[1]=mi_read_004ef550((0x005e4ef4u),4u);
    /* 004ef637 */
    a=f->r[1];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004ef63a */
    if(!(f->flags&MI_ZF))goto mi_004ef653;
mi_004ef63c:;
    /* 004ef63c */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004ef63d */
    f->r[1]=0x00000006u;
    /* 004ef642 */
    a=f->r[1];mi_idiv32(f,a);
    /* 004ef644 */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004ef647 */
    if((f->flags&MI_ZF))goto mi_004ef670;
    /* 004ef649 */
    MI_U8((0x00000040u+f->r[4]))=(uint8_t)(0x00000000u);
    /* 004ef64e */
    goto mi_004ef75d;
mi_004ef653:;
    /* 004ef653 */
    a=f->r[1];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 004ef656 */
    if(!(f->flags&MI_ZF))goto mi_004ef63c;
    /* 004ef658 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004ef659 */
    f->r[1]=0x00000006u;
    /* 004ef65e */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004ef65f */
    a=f->r[1];mi_idiv32(f,a);
    /* 004ef661 */
    a=f->r[2];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 004ef664 */
    if(mi_signed_less(f->flags))goto mi_004ef670;
    /* 004ef666 */
    MI_U8((0x00000040u+f->r[4]))=(uint8_t)(0x00000000u);
    /* 004ef66b */
    goto mi_004ef75d;
mi_004ef670:;
    /* 004ef670 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read_004ef550((0x007999eau+f->r[3]),2u);
    /* 004ef677 */
    f->r[0]=(0x00000000u+f->r[7]+f->r[7]*8u);
    /* 004ef67a */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004ef67d */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004ef67f */
    f->r[0]=mi_read_004ef550((0x00bca2e0u),4u);
    /* 004ef684 */
    f->r[1]=mi_read_004ef550((0x005e4ef8u+f->r[2]*8u),4u);
    /* 004ef68b */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004ef68c */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004ef68e */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004ef690 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 004ef693 */
    if(!mi_signed_less(f->flags))goto mi_004ef69e;
    /* 004ef695 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004ef697 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 004ef69a */
    if(mi_signed_less(f->flags))goto mi_004ef6ad;
    /* 004ef69c */
    goto mi_004ef6aa;
mi_004ef69e:;
    /* 004ef69e */
    a=f->r[1];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 004ef6a1 */
    if(!mi_signed_less(f->flags))goto mi_004ef6ad;
    /* 004ef6a3 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004ef6a5 */
    a=f->r[1];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 004ef6a8 */
    if(mi_signed_less(f->flags))goto mi_004ef6ad;
mi_004ef6aa:;
    /* 004ef6aa */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,1);f->r[1]=t;
mi_004ef6ad:;
    /* 004ef6ad */
    a=mi_read_004ef550((0x005e4ef4u),4u);b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 004ef6b4 */
    if(!(f->flags&MI_ZF))goto mi_004ef6ce;
    /* 004ef6b6 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004ef6b8 */
    if((f->flags&MI_ZF))goto mi_004ef6c4;
    /* 004ef6ba */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 004ef6bd */
    if((f->flags&MI_ZF))goto mi_004ef6c4;
    /* 004ef6bf */
    a=f->r[1];b=0x00000007u;t=mi_alu(f,a,b,32,1);
    /* 004ef6c2 */
    if(!(f->flags&MI_ZF))goto mi_004ef6df;
mi_004ef6c4:;
    /* 004ef6c4 */
    MI_U8((0x00000040u+f->r[4]))=(uint8_t)(0x00000000u);
    /* 004ef6c9 */
    goto mi_004ef75d;
mi_004ef6ce:;
    /* 004ef6ce */
    a=f->r[1];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 004ef6d1 */
    if((f->flags&MI_ZF))goto mi_004ef6df;
    /* 004ef6d3 */
    a=f->r[1];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 004ef6d6 */
    if((f->flags&MI_ZF))goto mi_004ef6df;
    /* 004ef6d8 */
    MI_U8((0x00000040u+f->r[4]))=(uint8_t)(0x00000000u);
    /* 004ef6dd */
    goto mi_004ef75d;
mi_004ef6df:;
    /* 004ef6df */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 004ef6e2 */
    if(!(f->flags&MI_ZF))goto mi_004ef6f0;
    /* 004ef6e4 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004ef6e7 */
    f->r[0]=mi_read_004ef550((0x005700f4u+f->r[1]*4u),4u);
    /* 004ef6ee */
    goto mi_004ef6fa;
mi_004ef6f0:;
    /* 004ef6f0 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004ef6f3 */
    f->r[0]=mi_read_004ef550((0x005700f8u+f->r[2]*4u),4u);
mi_004ef6fa:;
    /* 004ef6fa */
    a=f->r[0];b=mi_read_004ef550((0x007999e4u+f->r[3]),4u);t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004ef700 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read_004ef550((0x00b4e6fcu+f->r[0]*2u),2u);
    /* 004ef708 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004ef70a */
    if(!(f->flags&MI_ZF))goto mi_004ef712;
    /* 004ef70c */
    MI_U8((0x00000040u+f->r[4]))=(uint8_t)((f->r[1]&0xffu));
    /* 004ef710 */
    goto mi_004ef75d;
mi_004ef712:;
    /* 004ef712 */
    f->r[0]=f->r[1];
    /* 004ef714 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004ef717 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004ef719 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004ef71c */
    a=mi_read_004ef550((0x007999e8u+f->r[0]),2u);b=0x000000b7u;t=mi_alu(f,a,b,16,1);
    /* 004ef725 */
    if((f->flags&MI_ZF))goto mi_004ef72e;
    /* 004ef727 */
    MI_U8((0x00000040u+f->r[4]))=(uint8_t)(0x00000000u);
    /* 004ef72c */
    goto mi_004ef75d;
mi_004ef72e:;
    /* 004ef72e */
    f->r[1]=mi_read_004ef550((0x00000040u+f->r[4]),4u);
    /* 004ef732 */
    f->r[2]=mi_read_004ef550((0x00000038u+f->r[4]),4u);
    /* 004ef736 */
    MI_U32((0x00799a44u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 004ef73c */
    f->r[1]=mi_read_004ef550((0x0000003cu+f->r[4]),4u);
    /* 004ef740 */
    MI_U32((0x00799a48u+f->r[0]))=(uint32_t)(f->r[5]);
    /* 004ef746 */
    MI_U32((0x00799a4cu+f->r[0]))=(uint32_t)(f->r[6]);
    /* 004ef74c */
    MI_U32((0x00799a50u+f->r[0]))=(uint32_t)(f->r[2]);
    /* 004ef752 */
    MI_U32((0x00799a54u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 004ef758 */
    MI_U8((0x00000040u+f->r[4]))=(uint8_t)(0x00000001u);
mi_004ef75d:;
    /* 004ef75d */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read_004ef550((0x00000040u+f->r[4]),1u))&0xffu)<<0);
    /* 004ef761 */
    f->r[0]=mi_read_004ef550((0x00799a44u+f->r[3]),4u);
    /* 004ef767 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004ef769 */
    MI_U8((0x00000013u+f->r[4]))=(uint8_t)(0x00000000u);
    /* 004ef76e */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004ef772 */
    if(!(f->flags&MI_ZF))goto mi_004ef7e6;
    /* 004ef774 */
    f->r[1]=mi_read_004ef550((0x00799a40u+f->r[3]),4u);
    /* 004ef77a */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004ef77c */
    if(!(f->flags&MI_ZF))goto mi_004ef7e6;
    /* 004ef77e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004ef780 */
    if(!(f->flags&MI_ZF))goto mi_004ef7e6;
    /* 004ef782 */
    f->r[7]=0x00570084u;
mi_004ef787:;
    /* 004ef787 */
    f->r[0]=mi_read_004ef550((0x007999e4u+f->r[3]),4u);
    /* 004ef78d */
    f->r[2]=mi_read_004ef550((0x00000000u+f->r[7]),4u);
    /* 004ef78f */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004ef791 */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004ef795 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read_004ef550((0x00b350dcu+f->r[0]*2u),2u);
    /* 004ef79d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004ef79f */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004ef7d6;
mi_004ef7a1:;
    /* 004ef7a1 */
    f->r[6]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004ef7a4 */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004ef7a7 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004ef7a9 */
    a=f->r[6];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004ef7ac */
    a=mi_read_004ef550((0x00bcd718u+f->r[6]),2u);b=0x00000003u;t=mi_alu(f,a,b,16,1);
    /* 004ef7b4 */
    if(!(f->flags&MI_ZF))goto mi_004ef7cb;
    /* 004ef7b6 */
    f->r[2]=mi_read_004ef550((0x0000003cu+f->r[4]),4u);
    /* 004ef7ba */
    f->r[1]=mi_read_004ef550((0x00000038u+f->r[4]),4u);
    /* 004ef7be */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ef7c0 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ef7c1 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ef7c2 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ef7c3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004ef7c8u;f->boundary(0x004ef7c3u,0x00438e50u,f);
    /* 004ef7c8 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004ef7cb:;
    /* 004ef7cb */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read_004ef550((0x00bcd600u+f->r[6]),2u);
    /* 004ef7d2 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004ef7d4 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004ef7a1;
mi_004ef7d6:;
    /* 004ef7d6 */
    a=f->r[7];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004ef7d9 */
    a=f->r[7];b=0x00570094u;t=mi_alu(f,a,b,32,1);
    /* 004ef7df */
    if(mi_signed_less(f->flags))goto mi_004ef787;
    /* 004ef7e1 */
    MI_U8((0x00000013u+f->r[4]))=(uint8_t)(0x00000001u);
mi_004ef7e6:;
    /* 004ef7e6 */
    f->r[0]=mi_read_004ef550((0x00000020u+f->r[4]),4u);
    /* 004ef7ea */
    f->r[5]=mi_read_004ef550((0x00000034u+f->r[4]),4u);
    /* 004ef7ee */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004ef7f0 */
    if((f->flags&MI_ZF))goto mi_004ef87b;
    /* 004ef7f6 */
    a=mi_read_004ef550((0x005e4ef4u),4u);b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 004ef7fd */
    if(!(f->flags&MI_ZF))goto mi_004ef866;
    /* 004ef7ff */
    a=mi_read_004ef550((0x00799a2cu+f->r[3]),1u);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 004ef806 */
    if((f->flags&MI_CF))goto mi_004ef866;
    /* 004ef808 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read_004ef550((0x007999eau+f->r[3]),2u);
    /* 004ef80f */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read_004ef550((0x005f5d30u+f->r[2]),1u))&0xffu)<<0);
    /* 004ef815 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004ef817 */
    if((f->flags&MI_ZF))goto mi_004ef866;
    /* 004ef819 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read_004ef550((0x00799a2fu+f->r[3]),1u))&0xffu)<<0);
    /* 004ef81f */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004ef821 */
    if((f->flags&MI_ZF))goto mi_004ef866;
    /* 004ef823 */
    f->r[0]=mi_read_004ef550((0x00000028u+f->r[4]),4u);
    /* 004ef827 */
    f->r[2]=mi_read_004ef550((0x00f57334u),4u);
    /* 004ef82d */
    f->r[3]=mi_read_004ef550((0x00000018u+f->r[4]),4u);
    /* 004ef831 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read_004ef550((0x00acf85cu+f->r[0]),1u))&0xffu)<<0);
    /* 004ef837 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004ef83a */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004ef83d */
    f->r[0]=mi_read_004ef550((0x00000014u+f->r[4]),4u);
    /* 004ef841 */
    f->r[6]=(0x00000065u+f->r[2]+f->r[1]*2u);
    /* 004ef845 */
    f->r[1]=mi_read_004ef550((0x00000020u+f->r[4]),4u);
    /* 004ef849 */
    f->r[7]=(0x00000000u+f->r[0]+f->r[1]*1u);
    /* 004ef84c */
    f->r[1]=0x00f55bb0u;
    /* 004ef851 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ef852 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ef853 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ef854 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004ef859u;f->boundary(0x004ef854u,0x0053e590u,f);
    /* 004ef859 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ef85a */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ef85b */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ef85c */
    f->r[1]=0x00f55bb0u;
    /* 004ef861 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004ef866u;f->boundary(0x004ef861u,0x0053f250u,f);
mi_004ef866:;
    /* 004ef866 */
    f->r[2]=mi_read_004ef550((0x00000014u+f->r[4]),4u);
    /* 004ef86a */
    f->r[0]=mi_read_004ef550((0x00000018u+f->r[4]),4u);
    /* 004ef86e */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ef86f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ef870 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ef871 */
    f->r[1]=0x00f55bb0u;
    /* 004ef876 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004ef87bu;f->boundary(0x004ef876u,0x0053e590u,f);
mi_004ef87b:;
    /* 004ef87b */
    f->r[1]=mi_read_004ef550((0x00000014u+f->r[4]),4u);
    /* 004ef87f */
    f->r[2]=mi_read_004ef550((0x00000018u+f->r[4]),4u);
    /* 004ef883 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ef884 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ef885 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ef886 */
    f->r[1]=0x00f55bb0u;
    /* 004ef88b */
    f->r[4]-=4;MI_U32(f->r[4])=0x004ef890u;f->boundary(0x004ef88bu,0x0053f250u,f);
    /* 004ef890 */
    f->r[0]=mi_read_004ef550((0x00000024u+f->r[4]),4u);
    /* 004ef894 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004ef896 */
    if((f->flags&MI_ZF))goto mi_004efac0;
    /* 004ef89c */
    f->r[0]=mi_read_004ef550((0x0000001cu+f->r[4]),4u);
    /* 004ef8a0 */
    f->r[6]=0x00000003u;
    /* 004ef8a5 */
    f->r[3]=mi_read_004ef550((0x00799a48u+f->r[0]),4u);
    /* 004ef8ab */
    f->r[1]=mi_read_004ef550((0x00799a4cu+f->r[0]),4u);
    /* 004ef8b1 */
    f->r[2]=mi_read_004ef550((0x00799a50u+f->r[0]),4u);
    /* 004ef8b7 */
    f->r[0]=mi_read_004ef550((0x00799a54u+f->r[0]),4u);
    /* 004ef8bd */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004ef8c1 */
    f->r[0]=mi_read_004ef550((0x005e4ef4u),4u);
    /* 004ef8c6 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004ef8c8 */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004ef8cc */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004ef8d0 */
    if(!(f->flags&MI_ZF))goto mi_004ef981;
    /* 004ef8d6 */
    f->r[0]=mi_read_004ef550((0x00000024u+f->r[4]),4u);
    /* 004ef8da */
    f->r[1]=f->r[0];
    /* 004ef8dc */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004ef8df */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004ef8e1 */
    f->r[0]=mi_read_004ef550((0x0000002cu+f->r[4]),4u);
    /* 004ef8e5 */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004ef8e8 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 004ef8eb */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read_004ef550((0x007999eau+f->r[1]),2u);
    /* 004ef8f2 */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004ef8f5 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004ef8f7 */
    a=mi_read_004ef550((0x005e4ef8u+f->r[2]*8u),4u);b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 004ef8ff */
    if(!mi_signed_less(f->flags))goto mi_004ef961;
    /* 004ef901 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004ef903 */
    f->r[7]=0x00000006u;
    /* 004ef908 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read_004ef550((0x00799a31u+f->r[1]),1u))&0xffu)<<0);
    /* 004ef90e */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004ef90f */
    a=f->r[7];mi_idiv32(f,a);
    /* 004ef911 */
    f->r[0]=(0xffffffffu+f->r[2]);
    /* 004ef914 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 004ef917 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004ef932;
    /* 004ef919 */
    switch(mi_read_004ef550((0x004efd3cu+f->r[0]*4u),4u)){case 0x004ef920u:goto mi_004ef920;case 0x004ef927u:goto mi_004ef927;case 0x004ef92eu:goto mi_004ef92e;case 0x004ef932u:goto mi_004ef932;default:abort();}
mi_004ef920:;
    /* 004ef920 */
    f->r[5]=0x00000001u;
    /* 004ef925 */
    goto mi_004ef934;
mi_004ef927:;
    /* 004ef927 */
    f->r[5]=0x00000002u;
    /* 004ef92c */
    goto mi_004ef934;
mi_004ef92e:;
    /* 004ef92e */
    f->r[5]=f->r[6];
    /* 004ef930 */
    goto mi_004ef934;
mi_004ef932:;
    /* 004ef932 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
mi_004ef934:;
    /* 004ef934 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004ef936 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read_004ef550((0x00799a32u+f->r[1]),1u))&0xffu)<<0);
    /* 004ef93c */
    f->r[1]=0x00000006u;
    /* 004ef941 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004ef942 */
    a=f->r[1];mi_idiv32(f,a);
    /* 004ef944 */
    f->r[0]=(0xffffffffu+f->r[2]);
    /* 004ef947 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 004ef94a */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004ef983;
    /* 004ef94c */
    switch(mi_read_004ef550((0x004efd50u+f->r[0]*4u),4u)){case 0x004ef953u:goto mi_004ef953;case 0x004ef95au:goto mi_004ef95a;case 0x004ef983u:goto mi_004ef983;case 0x004ef985u:goto mi_004ef985;default:abort();}
mi_004ef953:;
    /* 004ef953 */
    f->r[6]=0x00000001u;
    /* 004ef958 */
    goto mi_004ef985;
mi_004ef95a:;
    /* 004ef95a */
    f->r[6]=0x00000002u;
    /* 004ef95f */
    goto mi_004ef985;
mi_004ef961:;
    /* 004ef961 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004ef963 */
    f->r[7]=f->r[6];
    /* 004ef965 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read_004ef550((0x00799a31u+f->r[1]),1u))&0xffu)<<0);
    /* 004ef96b */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004ef96c */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004ef96d */
    a=f->r[7];mi_idiv32(f,a);
    /* 004ef96f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004ef971 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read_004ef550((0x00799a32u+f->r[1]),1u))&0xffu)<<0);
    /* 004ef977 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004ef978 */
    f->r[5]=f->r[2];
    /* 004ef97a */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004ef97b */
    a=f->r[6];mi_idiv32(f,a);
    /* 004ef97d */
    f->r[6]=f->r[2];
    /* 004ef97f */
    goto mi_004ef985;
mi_004ef981:;
    /* 004ef981 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
mi_004ef983:;
    /* 004ef983 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
mi_004ef985:;
    /* 004ef985 */
    f->r[0]=mi_read_004ef550((0x00000024u+f->r[4]),4u);
    /* 004ef989 */
    f->r[7]=f->r[0];
    /* 004ef98b */
    a=f->r[7];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 004ef98e */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004ef990 */
    a=f->r[7];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 004ef993 */
    f->r[0]=mi_read_004ef550((0x00799a40u+f->r[7]),4u);
    /* 004ef999 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004ef99b */
    if((f->flags&MI_ZF))goto mi_004efa64;
    /* 004ef9a1 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read_004ef550((0x00799a31u+f->r[7]),1u))&0xffu)<<0);
    /* 004ef9a7 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004ef9a9 */
    if((f->flags&MI_ZF))goto mi_004efa00;
    /* 004ef9ab */
    f->r[1]=(0x00000000u+f->r[0]+f->r[5]*1u);
    /* 004ef9ae */
    f->r[0]=mi_read_004ef550((0x005e4ef4u),4u);
    /* 004ef9b3 */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 004ef9b6 */
    if(!(f->flags&MI_ZF))goto mi_004ef9d5;
    /* 004ef9b8 */
    f->r[5]=mi_read_004ef550((0x00f57334u),4u);
    /* 004ef9be */
    f->r[0]=f->r[1];
    /* 004ef9c0 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004ef9c2 */
    a=f->r[0];b=0x0000006du;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004ef9c5 */
    f->r[2]=mi_read_004ef550((0x005f5b68u+f->r[0]*8u),4u);
    /* 004ef9cc */
    f->r[0]=mi_read_004ef550((0x005f5b6cu+f->r[0]*8u),4u);
    /* 004ef9d3 */
    goto mi_004ef9d9;
mi_004ef9d5:;
    /* 004ef9d5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004ef9d7 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
mi_004ef9d9:;
    /* 004ef9d9 */
    f->r[5]=(uint32_t)(int32_t)(int16_t)mi_read_004ef550((0x00799a36u+f->r[7]),2u);
    /* 004ef9e0 */
    a=f->r[5];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 004ef9e2 */
    f->r[0]=mi_read_004ef550((0x00000034u+f->r[4]),4u);
    /* 004ef9e6 */
    a=f->r[5];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 004ef9e8 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read_004ef550((0x00799a34u+f->r[7]),2u);
    /* 004ef9ef */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004ef9f1 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ef9f2 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004ef9f4 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ef9f5 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ef9f6 */
    f->r[1]=0x00f55bb0u;
    /* 004ef9fb */
    f->r[4]-=4;MI_U32(f->r[4])=0x004efa00u;f->boundary(0x004ef9fbu,0x0053e590u,f);
mi_004efa00:;
    /* 004efa00 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read_004ef550((0x00799a32u+f->r[7]),1u))&0xffu)<<0);
    /* 004efa06 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004efa08 */
    if((f->flags&MI_ZF))goto mi_004efa64;
    /* 004efa0a */
    f->r[1]=mi_read_004ef550((0x00799a40u+f->r[7]),4u);
    /* 004efa10 */
    f->r[0]=mi_read_004ef550((0x005e4ef4u),4u);
    /* 004efa15 */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004efa17 */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 004efa1a */
    if(!(f->flags&MI_ZF))goto mi_004efa39;
    /* 004efa1c */
    f->r[2]=mi_read_004ef550((0x00f57334u),4u);
    /* 004efa22 */
    f->r[0]=f->r[6];
    /* 004efa24 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004efa26 */
    a=f->r[0];b=0x0000006du;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004efa29 */
    f->r[1]=mi_read_004ef550((0x005f5b68u+f->r[0]*8u),4u);
    /* 004efa30 */
    f->r[0]=mi_read_004ef550((0x005f5b6cu+f->r[0]*8u),4u);
    /* 004efa37 */
    goto mi_004efa3d;
mi_004efa39:;
    /* 004efa39 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004efa3b */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_004efa3d:;
    /* 004efa3d */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read_004ef550((0x00799a3au+f->r[7]),2u);
    /* 004efa44 */
    f->r[5]=mi_read_004ef550((0x00000034u+f->r[4]),4u);
    /* 004efa48 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004efa4a */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read_004ef550((0x00799a38u+f->r[7]),2u);
    /* 004efa51 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004efa53 */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004efa55 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004efa57 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004efa58 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004efa59 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004efa5a */
    f->r[1]=0x00f55bb0u;
    /* 004efa5f */
    f->r[4]-=4;MI_U32(f->r[4])=0x004efa64u;f->boundary(0x004efa5fu,0x0053e590u,f);
mi_004efa64:;
    /* 004efa64 */
    f->r[5]=0x00570084u;
mi_004efa69:;
    /* 004efa69 */
    f->r[1]=mi_read_004ef550((0x00000000u+f->r[5]),4u);
    /* 004efa6c */
    f->r[2]=mi_read_004ef550((0x007999e4u+f->r[7]),4u);
    /* 004efa72 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004efa74 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read_004ef550((0x00b350dcu+f->r[1]*2u),2u);
    /* 004efa7c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004efa7e */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004efab5;
mi_004efa80:;
    /* 004efa80 */
    f->r[6]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004efa83 */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004efa86 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004efa88 */
    a=f->r[6];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004efa8b */
    a=mi_read_004ef550((0x00bcd718u+f->r[6]),2u);b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 004efa93 */
    if((f->flags&MI_ZF))goto mi_004efaaa;
    /* 004efa95 */
    f->r[2]=mi_read_004ef550((0x00000028u+f->r[4]),4u);
    /* 004efa99 */
    f->r[1]=mi_read_004ef550((0x00000020u+f->r[4]),4u);
    /* 004efa9d */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004efa9f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004efaa0 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004efaa1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004efaa2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004efaa7u;f->boundary(0x004efaa2u,0x00438e50u,f);
    /* 004efaa7 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004efaaa:;
    /* 004efaaa */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read_004ef550((0x00bcd600u+f->r[6]),2u);
    /* 004efab1 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004efab3 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004efa80;
mi_004efab5:;
    /* 004efab5 */
    a=f->r[5];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 004efab8 */
    a=f->r[5];b=0x00570094u;t=mi_alu(f,a,b,32,1);
    /* 004efabe */
    if(mi_signed_less(f->flags))goto mi_004efa69;
mi_004efac0:;
    /* 004efac0 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read_004ef550((0x00000040u+f->r[4]),1u))&0xffu)<<0);
    /* 004efac4 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004efac6 */
    if(!(f->flags&MI_ZF))goto mi_004efd34;
    /* 004efacc */
    f->r[3]=mi_read_004ef550((0x0000001cu+f->r[4]),4u);
    /* 004efad0 */
    f->r[7]=mi_read_004ef550((0x00799a40u+f->r[3]),4u);
    /* 004efad6 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 004efad8 */
    if((f->flags&MI_ZF))goto mi_004efc66;
    /* 004efade */
    f->r[5]=mi_read_004ef550((0x005e4ef4u),4u);
    /* 004efae4 */
    a=f->r[5];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 004efae7 */
    if(!(f->flags&MI_ZF))goto mi_004efb9c;
    /* 004efaed */
    f->r[0]=mi_read_004ef550((0x0000002cu+f->r[4]),4u);
    /* 004efaf1 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read_004ef550((0x007999eau+f->r[3]),2u);
    /* 004efaf8 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 004efafb */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004efafe */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004efb00 */
    a=mi_read_004ef550((0x005e4ef8u+f->r[2]*8u),4u);b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 004efb08 */
    if(!mi_signed_less(f->flags))goto mi_004efb74;
    /* 004efb0a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004efb0c */
    f->r[1]=0x00000006u;
    /* 004efb11 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read_004ef550((0x00799a31u+f->r[3]),1u))&0xffu)<<0);
    /* 004efb17 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004efb18 */
    a=f->r[1];mi_idiv32(f,a);
    /* 004efb1a */
    f->r[0]=(0xffffffffu+f->r[2]);
    /* 004efb1d */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 004efb20 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004efb3e;
    /* 004efb22 */
    switch(mi_read_004ef550((0x004efd64u+f->r[0]*4u),4u)){case 0x004efb29u:goto mi_004efb29;case 0x004efb30u:goto mi_004efb30;case 0x004efb37u:goto mi_004efb37;case 0x004efb3eu:goto mi_004efb3e;default:abort();}
mi_004efb29:;
    /* 004efb29 */
    f->r[1]=0x00000001u;
    /* 004efb2e */
    goto mi_004efb40;
mi_004efb30:;
    /* 004efb30 */
    f->r[1]=0x00000002u;
    /* 004efb35 */
    goto mi_004efb40;
mi_004efb37:;
    /* 004efb37 */
    f->r[1]=0x00000003u;
    /* 004efb3c */
    goto mi_004efb40;
mi_004efb3e:;
    /* 004efb3e */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_004efb40:;
    /* 004efb40 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004efb42 */
    f->r[6]=0x00000006u;
    /* 004efb47 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read_004ef550((0x00799a32u+f->r[3]),1u))&0xffu)<<0);
    /* 004efb4d */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004efb4e */
    a=f->r[6];mi_idiv32(f,a);
    /* 004efb50 */
    f->r[0]=(0xffffffffu+f->r[2]);
    /* 004efb53 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 004efb56 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004efb9e;
    /* 004efb58 */
    switch(mi_read_004ef550((0x004efd78u+f->r[0]*4u),4u)){case 0x004efb5fu:goto mi_004efb5f;case 0x004efb66u:goto mi_004efb66;case 0x004efb6du:goto mi_004efb6d;case 0x004efb9eu:goto mi_004efb9e;default:abort();}
mi_004efb5f:;
    /* 004efb5f */
    f->r[6]=0x00000001u;
    /* 004efb64 */
    goto mi_004efba0;
mi_004efb66:;
    /* 004efb66 */
    f->r[6]=0x00000002u;
    /* 004efb6b */
    goto mi_004efba0;
mi_004efb6d:;
    /* 004efb6d */
    f->r[6]=0x00000003u;
    /* 004efb72 */
    goto mi_004efba0;
mi_004efb74:;
    /* 004efb74 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004efb76 */
    f->r[1]=0x00000003u;
    /* 004efb7b */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read_004ef550((0x00799a31u+f->r[3]),1u))&0xffu)<<0);
    /* 004efb81 */
    f->r[6]=0x00000003u;
    /* 004efb86 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004efb87 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004efb88 */
    a=f->r[1];mi_idiv32(f,a);
    /* 004efb8a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004efb8c */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read_004ef550((0x00799a32u+f->r[3]),1u))&0xffu)<<0);
    /* 004efb92 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004efb93 */
    f->r[1]=f->r[2];
    /* 004efb95 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004efb96 */
    a=f->r[6];mi_idiv32(f,a);
    /* 004efb98 */
    f->r[6]=f->r[2];
    /* 004efb9a */
    goto mi_004efba0;
mi_004efb9c:;
    /* 004efb9c */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_004efb9e:;
    /* 004efb9e */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
mi_004efba0:;
    /* 004efba0 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read_004ef550((0x00799a31u+f->r[3]),1u))&0xffu)<<0);
    /* 004efba6 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004efba8 */
    if((f->flags&MI_ZF))goto mi_004efc03;
    /* 004efbaa */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004efbac */
    a=f->r[5];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 004efbaf */
    if(!(f->flags&MI_ZF))goto mi_004efbce;
    /* 004efbb1 */
    f->r[7]=mi_read_004ef550((0x00f57334u),4u);
    /* 004efbb7 */
    f->r[0]=f->r[1];
    /* 004efbb9 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004efbbb */
    a=f->r[0];b=0x0000006du;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004efbbe */
    f->r[2]=mi_read_004ef550((0x005f5b68u+f->r[0]*8u),4u);
    /* 004efbc5 */
    f->r[0]=mi_read_004ef550((0x005f5b6cu+f->r[0]*8u),4u);
    /* 004efbcc */
    goto mi_004efbd2;
mi_004efbce:;
    /* 004efbce */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004efbd0 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
mi_004efbd2:;
    /* 004efbd2 */
    f->r[7]=(uint32_t)(int32_t)(int16_t)mi_read_004ef550((0x00799a36u+f->r[3]),2u);
    /* 004efbd9 */
    f->r[5]=mi_read_004ef550((0x00000014u+f->r[4]),4u);
    /* 004efbdd */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004efbdf */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read_004ef550((0x00799a34u+f->r[3]),2u);
    /* 004efbe6 */
    a=f->r[7];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004efbe8 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004efbea */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004efbeb */
    f->r[7]=mi_read_004ef550((0x0000001cu+f->r[4]),4u);
    /* 004efbef */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004efbf1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004efbf2 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004efbf3 */
    f->r[1]=0x00f55bb0u;
    /* 004efbf8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004efbfdu;f->boundary(0x004efbf8u,0x0053e590u,f);
    /* 004efbfd */
    f->r[5]=mi_read_004ef550((0x005e4ef4u),4u);
mi_004efc03:;
    /* 004efc03 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read_004ef550((0x00799a32u+f->r[3]),1u))&0xffu)<<0);
    /* 004efc09 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004efc0b */
    if((f->flags&MI_ZF))goto mi_004efc66;
    /* 004efc0d */
    f->r[1]=mi_read_004ef550((0x00799a40u+f->r[3]),4u);
    /* 004efc13 */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004efc15 */
    a=f->r[5];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 004efc18 */
    if(!(f->flags&MI_ZF))goto mi_004efc37;
    /* 004efc1a */
    f->r[6]=mi_read_004ef550((0x00f57334u),4u);
    /* 004efc20 */
    f->r[0]=f->r[1];
    /* 004efc22 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004efc24 */
    a=f->r[0];b=0x0000006du;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004efc27 */
    f->r[2]=mi_read_004ef550((0x005f5b68u+f->r[0]*8u),4u);
    /* 004efc2e */
    f->r[0]=mi_read_004ef550((0x005f5b6cu+f->r[0]*8u),4u);
    /* 004efc35 */
    goto mi_004efc3b;
mi_004efc37:;
    /* 004efc37 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004efc39 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
mi_004efc3b:;
    /* 004efc3b */
    f->r[6]=(uint32_t)(int32_t)(int16_t)mi_read_004ef550((0x00799a3au+f->r[3]),2u);
    /* 004efc42 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004efc44 */
    f->r[0]=mi_read_004ef550((0x00000014u+f->r[4]),4u);
    /* 004efc48 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004efc4a */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read_004ef550((0x00799a38u+f->r[3]),2u);
    /* 004efc51 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004efc52 */
    f->r[6]=mi_read_004ef550((0x0000001cu+f->r[4]),4u);
    /* 004efc56 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004efc58 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004efc5a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004efc5b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004efc5c */
    f->r[1]=0x00f55bb0u;
    /* 004efc61 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004efc66u;f->boundary(0x004efc61u,0x0053e590u,f);
mi_004efc66:;
    /* 004efc66 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read_004ef550((0x00000013u+f->r[4]),1u))&0xffu)<<0);
    /* 004efc6a */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004efc6c */
    if(!(f->flags&MI_ZF))goto mi_004efccf;
    /* 004efc6e */
    f->r[3]=mi_read_004ef550((0x00000038u+f->r[4]),4u);
    /* 004efc72 */
    f->r[5]=mi_read_004ef550((0x0000003cu+f->r[4]),4u);
    /* 004efc76 */
    f->r[7]=0x00570084u;
mi_004efc7b:;
    /* 004efc7b */
    f->r[1]=mi_read_004ef550((0x0000001cu+f->r[4]),4u);
    /* 004efc7f */
    f->r[6]=mi_read_004ef550((0x00000000u+f->r[7]),4u);
    /* 004efc81 */
    f->r[2]=mi_read_004ef550((0x007999e4u+f->r[1]),4u);
    /* 004efc87 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004efc89 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read_004ef550((0x00b350dcu+f->r[2]*2u),2u);
    /* 004efc91 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004efc93 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004efcc2;
mi_004efc95:;
    /* 004efc95 */
    f->r[6]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004efc98 */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004efc9b */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004efc9d */
    a=f->r[6];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004efca0 */
    a=mi_read_004ef550((0x00bcd718u+f->r[6]),2u);b=0x00000003u;t=mi_alu(f,a,b,16,1);
    /* 004efca8 */
    if(!(f->flags&MI_ZF))goto mi_004efcb7;
    /* 004efcaa */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004efcac */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004efcad */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004efcae */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004efcaf */
    f->r[4]-=4;MI_U32(f->r[4])=0x004efcb4u;f->boundary(0x004efcafu,0x00438e50u,f);
    /* 004efcb4 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004efcb7:;
    /* 004efcb7 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read_004ef550((0x00bcd600u+f->r[6]),2u);
    /* 004efcbe */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004efcc0 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004efc95;
mi_004efcc2:;
    /* 004efcc2 */
    a=f->r[7];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004efcc5 */
    a=f->r[7];b=0x00570094u;t=mi_alu(f,a,b,32,1);
    /* 004efccb */
    if(mi_signed_less(f->flags))goto mi_004efc7b;
    /* 004efccd */
    goto mi_004efcd7;
mi_004efccf:;
    /* 004efccf */
    f->r[3]=mi_read_004ef550((0x00000038u+f->r[4]),4u);
    /* 004efcd3 */
    f->r[5]=mi_read_004ef550((0x0000003cu+f->r[4]),4u);
mi_004efcd7:;
    /* 004efcd7 */
    f->r[7]=0x00570084u;
mi_004efcdc:;
    /* 004efcdc */
    f->r[1]=mi_read_004ef550((0x0000001cu+f->r[4]),4u);
    /* 004efce0 */
    f->r[0]=mi_read_004ef550((0x00000000u+f->r[7]),4u);
    /* 004efce2 */
    a=f->r[0];b=mi_read_004ef550((0x007999e4u+f->r[1]),4u);t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004efce8 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read_004ef550((0x00b350dcu+f->r[0]*2u),2u);
    /* 004efcf0 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004efcf2 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004efd29;
mi_004efcf4:;
    /* 004efcf4 */
    f->r[6]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004efcf7 */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004efcfa */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004efcfc */
    a=f->r[6];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004efcff */
    f->r[1]=(f->r[1]&0xffff0000u)|(((mi_read_004ef550((0x00bcd718u+f->r[6]),2u))&0xffffu)<<0);
    /* 004efd06 */
    a=(f->r[1]&0xffffu);b=(f->r[1]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 004efd09 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004efd1e;
    /* 004efd0b */
    a=(f->r[1]&0xffffu);b=0x00000003u;t=mi_alu(f,a,b,16,1);
    /* 004efd0f */
    if((f->flags&MI_ZF))goto mi_004efd1e;
    /* 004efd11 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004efd13 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004efd14 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004efd15 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004efd16 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004efd1bu;f->boundary(0x004efd16u,0x00438e50u,f);
    /* 004efd1b */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004efd1e:;
    /* 004efd1e */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read_004ef550((0x00bcd600u+f->r[6]),2u);
    /* 004efd25 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004efd27 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004efcf4;
mi_004efd29:;
    /* 004efd29 */
    a=f->r[7];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004efd2c */
    a=f->r[7];b=0x00570094u;t=mi_alu(f,a,b,32,1);
    /* 004efd32 */
    if(mi_signed_less(f->flags))goto mi_004efcdc;
mi_004efd34:;
    /* 004efd34 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004efd35 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004efd36 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004efd37 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004efd38 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004efd3b */
    f->r[4]+=4;return;
}


static const uint8_t mi_rom_00542ea0[]={0x4c,0x2e,0x54,0x00,0x30,0x2e,0x54,0x00,0x37,0x2e,0x54,0x00,0x3e,0x2e,0x54,0x00,0x45,0x2e,0x54,0x00,0x45,0x2e,0x54,0x00,0x3e,0x2e,0x54,0x00,0x3e,0x2e,0x54,0x00,0x37,0x2e,0x54,0x00,0x37,0x2e,0x54,0x00,0x30,0x2e,0x54,0x00,0x30,0x2e,0x54,0x00,0x4c,0x2e,0x54,0x00,0x4c,0x2e,0x54,0x00};
static __inline uint32_t mi_read_00542e10(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x00542ea0u&&width<=56u&&(offset=address-0x00542ea0u)<=56u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00542ea0[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}

void restore_building_world_00542e10(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00542e10 */
    f->r[0]=mi_read_00542e10((0x00000004u+f->r[4]),4u);
    /* 00542e14 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00542e15 */
    f->r[1]=f->r[0];
    /* 00542e17 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00542e18 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00542e1b */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00542e1d */
    f->r[0]=mi_read_00542e10((0x00799a30u+f->r[1]*8u),4u);
    /* 00542e24 */
    a=f->r[0];b=0x0000000du;t=mi_alu(f,a,b,32,1);
    /* 00542e27 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00542e4c;
    /* 00542e29 */
    switch(mi_read_00542e10((0x00542ea0u+f->r[0]*4u),4u)){case 0x00542e30u:goto mi_00542e30;case 0x00542e37u:goto mi_00542e37;case 0x00542e3eu:goto mi_00542e3e;case 0x00542e45u:goto mi_00542e45;case 0x00542e4cu:goto mi_00542e4c;default:abort();}
mi_00542e30:;
    /* 00542e30 */
    f->r[7]=0x00000001u;
    /* 00542e35 */
    goto mi_00542e4e;
mi_00542e37:;
    /* 00542e37 */
    f->r[7]=0x00000002u;
    /* 00542e3c */
    goto mi_00542e4e;
mi_00542e3e:;
    /* 00542e3e */
    f->r[7]=0x00000003u;
    /* 00542e43 */
    goto mi_00542e4e;
mi_00542e45:;
    /* 00542e45 */
    f->r[7]=0x00000004u;
    /* 00542e4a */
    goto mi_00542e4e;
mi_00542e4c:;
    /* 00542e4c */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
mi_00542e4e:;
    /* 00542e4e */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 00542e50 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 00542e52 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00542e9b;
    /* 00542e54 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00542e55 */
    f->r[5]=mi_read_00542e10((0x00000018u+f->r[4]),4u);
    /* 00542e59 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00542e5a */
    f->r[6]=0x00603238u;
mi_00542e5f:;
    /* 00542e5f */
    a=(f->r[3]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,3);
    /* 00542e62 */
    if((f->flags&MI_ZF))goto mi_00542e6f;
    /* 00542e64 */
    f->r[2]=mi_read_00542e10((0x00f56d74u),4u);
    /* 00542e6a */
    f->r[0]=(0x0000000eu+f->r[2]);
    /* 00542e6d */
    goto mi_00542e77;
mi_00542e6f:;
    /* 00542e6f */
    f->r[0]=mi_read_00542e10((0x00f56d74u),4u);
    /* 00542e74 */
    a=f->r[0];b=0x0000000du;t=mi_alu(f,a,b,32,0);f->r[0]=t;
mi_00542e77:;
    /* 00542e77 */
    f->r[1]=mi_read_00542e10((0x00000004u+f->r[6]),4u);
    /* 00542e7a */
    f->r[2]=mi_read_00542e10((0x00000018u+f->r[4]),4u);
    /* 00542e7e */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00542e80 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00542e81 */
    f->r[1]=mi_read_00542e10((0x00000000u+f->r[6]),4u);
    /* 00542e83 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00542e85 */
    f->r[1]=0x00f55bb0u;
    /* 00542e8a */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00542e8b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00542e8c */
    f->r[4]-=4;MI_U32(f->r[4])=0x00542e91u;f->boundary(0x00542e8cu,0x0053e590u,f);
    /* 00542e91 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 00542e92 */
    a=f->r[6];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00542e95 */
    a=f->r[3];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 00542e97 */
    if(mi_signed_less(f->flags))goto mi_00542e5f;
    /* 00542e99 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00542e9a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
mi_00542e9b:;
    /* 00542e9b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00542e9c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00542e9d */
    f->r[4]+=4;return;
}
