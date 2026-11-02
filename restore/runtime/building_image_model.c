/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "building_image.h"
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
static __inline uint32_t mi_alu(struct restore_building_image_frame *f,uint32_t a,uint32_t b,uint32_t width,uint32_t operation)
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
static __inline uint32_t mi_shift(struct restore_building_image_frame *f,uint32_t a,uint32_t count,uint32_t width,uint32_t arithmetic)
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
static __inline uint32_t mi_imul(struct restore_building_image_frame *f,uint32_t a,uint32_t b)
{
 int64_t product=(int64_t)(int32_t)a*(int32_t)b;uint32_t result=(uint32_t)product;
 f->flags&=~(MI_CF|MI_OF);if(product!=(int64_t)(int32_t)result)f->flags|=MI_CF|MI_OF;
 return result;
}
static __inline int mi_signed_less(uint32_t flags){return ((flags&MI_SF)!=0)^((flags&MI_OF)!=0);}

static const uint8_t mi_rom_004df994[]={0x12,0xf4,0x4d,0x00,0xba,0xf4,0x4d,0x00,0x62,0xf5,0x4d,0x00,0x0a,0xf6,0x4d,0x00,0xb2,0xf6,0x4d,0x00,0x1b,0xf7,0x4d,0x00,0x4c,0xf7,0x4d,0x00,0x78,0xf7,0x4d,0x00,0xa4,0xf7,0x4d,0x00,0xed,0xf7,0x4d,0x00,0x3c,0xf8,0x4d,0x00,0x8b,0xf8,0x4d,0x00,0xda,0xf8,0x4d,0x00,0x1f,0xf9,0x4d,0x00};
static __inline uint32_t mi_read(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x004df994u&&width<=56u&&(offset=address-0x004df994u)<=56u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004df994[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}

void restore_building_image_C_004df360(struct restore_building_image_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004df360 */
    a=f->r[4];b=0x00000074u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 004df363 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004df364 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004df365 */
    f->r[5]=mi_read((0x00000080u+f->r[4]),4u);
    /* 004df36c */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004df36d */
    f->r[6]=0x00000002u;
    /* 004df372 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004df373 */
    f->r[7]=mi_read((0x0000008cu+f->r[4]),4u);
    /* 004df37a */
    MI_U32((0x00000058u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 004df37e */
    MI_U32((0x00000060u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 004df382 */
    MI_U32((0x00000078u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 004df386 */
    MI_U32((0x00000080u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 004df38d */
    f->r[6]=f->r[5];
    /* 004df38f */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004df392 */
    a=f->r[6];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004df394 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004df396 */
    f->r[1]=0x00000001u;
    /* 004df39b */
    MI_U32((0x00000000u+f->r[7]))=(uint32_t)(f->r[0]);
    /* 004df39d */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004df3a0 */
    MI_U32((0x00000048u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004df3a4 */
    MI_U32((0x00000050u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004df3a8 */
    f->r[3]=mi_read((0x00799a48u+f->r[6]),4u);
    /* 004df3ae */
    MI_U32((0x00000068u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004df3b2 */
    MI_U32((0x00000070u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004df3b6 */
    MI_U32((0x00000044u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004df3ba */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999eau+f->r[6]),2u);
    /* 004df3c1 */
    MI_U32((0x0000004cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004df3c5 */
    f->r[2]=0x00000003u;
    /* 004df3ca */
    MI_U32((0x00000064u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004df3ce */
    MI_U32((0x0000006cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004df3d2 */
    f->r[0]=mi_read((0x00bca2e0u),4u);
    /* 004df3d7 */
    MI_U32((0x00000054u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004df3db */
    MI_U32((0x0000005cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004df3df */
    MI_U32((0x00000074u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004df3e3 */
    MI_U32((0x0000007cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004df3e7 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004df3e8 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004df3e9 */
    f->r[2]=(0x00000018u+f->r[4]);
    /* 004df3ed */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004df3ee */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004df3ef */
    f->r[4]-=4;MI_U32(f->r[4])=0x004df3f4u;f->boundary(0x004df3efu,0x004ded50u,f);
    /* 004df3f4 */
    f->r[1]=mi_read((0x0000002cu+f->r[4]),4u);
    /* 004df3f8 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004df3fb */
    f->r[0]=f->r[1];
    /* 004df3fd */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004df402 */
    a=f->r[0];b=0x0000000du;t=mi_alu(f,a,b,32,1);
    /* 004df405 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004df987;
    /* 004df40b */
    switch(mi_read((0x004df994u+f->r[0]*4u),4u)){case 0x004df412u:goto mi_004df412;case 0x004df4bau:goto mi_004df4ba;case 0x004df562u:goto mi_004df562;case 0x004df60au:goto mi_004df60a;case 0x004df6b2u:goto mi_004df6b2;case 0x004df71bu:goto mi_004df71b;case 0x004df74cu:goto mi_004df74c;case 0x004df778u:goto mi_004df778;case 0x004df7a4u:goto mi_004df7a4;case 0x004df7edu:goto mi_004df7ed;case 0x004df83cu:goto mi_004df83c;case 0x004df88bu:goto mi_004df88b;case 0x004df8dau:goto mi_004df8da;case 0x004df91fu:goto mi_004df91f;default:abort();}
mi_004df412:;
    /* 004df412 */
    a=mi_read((0x007999eeu+f->r[6]),2u);b=0xffffffffu;t=mi_alu(f,a,b,16,1);
    /* 004df41a */
    if(!(f->flags&MI_ZF))goto mi_004df43d;
    /* 004df41c */
    f->r[0]=mi_read((0x00bca2e0u),4u);
    /* 004df421 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004df422 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004df424 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004df425 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004df426 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004df428 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004df429 */
    f->r[1]=f->r[0];
    /* 004df42b */
    f->r[0]=mi_read((0x00f57444u),4u);
    /* 004df430 */
    a=f->r[1];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[1]=t;
    /* 004df432 */
    a=f->r[1];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 004df435 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004df436 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004df438 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004df439 */
    a=f->r[4];b=0x00000074u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004df43c */
    f->r[4]+=4;return;
mi_004df43d:;
    /* 004df43d */
    f->r[0]=mi_read((0x00799a30u+f->r[6]),4u);
    /* 004df443 */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,1);
    /* 004df448 */
    if(!mi_signed_less(f->flags))goto mi_004df44e;
    /* 004df44a */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004df44c */
    goto mi_004df477;
mi_004df44e:;
    /* 004df44e */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999eau+f->r[6]),2u);
    /* 004df455 */
    a=f->r[5];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[5]=t;
    /* 004df457 */
    f->r[1]=f->r[5];
    /* 004df459 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004df45c */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004df45e */
    a=mi_read((0x00799a3cu+f->r[1]*8u),1u);b=0x00000010u;t=mi_alu(f,a,b,8,3);
    /* 004df466 */
    if(!(f->flags&MI_ZF))goto mi_004df46c;
    /* 004df468 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004df46a */
    goto mi_004df477;
mi_004df46c:;
    /* 004df46c */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004df46e */
    a=f->r[0];b=0x0000ffffu;t=mi_alu(f,a,b,32,1);
    /* 004df473 */
    t=(!mi_signed_less(f->flags))?1u:0u;f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004df476 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
mi_004df477:;
    /* 004df477 */
    f->r[0]=mi_read((0x00e94e18u),4u);
    /* 004df47c */
    a=0x00000005u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004df47e */
    f->r[2]=(0x00000000u+f->r[1]+f->r[0]*2u);
    /* 004df481 */
    f->r[1]=0x00f55bb0u;
    /* 004df486 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004df488 */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004df48b */
    f->r[0]=mi_read((0x005e4268u+f->r[0]),4u);
    /* 004df491 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004df492 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004df497u;f->boundary(0x004df492u,0x0053dd90u,f);
    /* 004df497 */
    f->r[1]=mi_read((0x00bca2e0u),4u);
    /* 004df49d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004df49e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004df49f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004df4a0 */
    f->r[0]=(0x00000000u+f->r[3]+f->r[1]*1u);
    /* 004df4a3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004df4a4 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004df4a5 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004df4a7 */
    f->r[2]=f->r[0];
    /* 004df4a9 */
    f->r[0]=mi_read((0x00f56d54u),4u);
    /* 004df4ae */
    a=f->r[2];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 004df4b0 */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004df4b3 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004df4b5 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004df4b6 */
    a=f->r[4];b=0x00000074u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004df4b9 */
    f->r[4]+=4;return;
mi_004df4ba:;
    /* 004df4ba */
    a=mi_read((0x007999eeu+f->r[6]),2u);b=0xffffffffu;t=mi_alu(f,a,b,16,1);
    /* 004df4c2 */
    if(!(f->flags&MI_ZF))goto mi_004df4e5;
    /* 004df4c4 */
    f->r[0]=mi_read((0x00bca2e0u),4u);
    /* 004df4c9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004df4ca */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004df4cc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004df4cd */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004df4ce */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004df4d0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004df4d1 */
    f->r[1]=f->r[0];
    /* 004df4d3 */
    f->r[0]=mi_read((0x00f5744cu),4u);
    /* 004df4d8 */
    a=f->r[1];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[1]=t;
    /* 004df4da */
    a=f->r[1];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 004df4dd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004df4de */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004df4e0 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004df4e1 */
    a=f->r[4];b=0x00000074u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004df4e4 */
    f->r[4]+=4;return;
mi_004df4e5:;
    /* 004df4e5 */
    f->r[0]=mi_read((0x00799a30u+f->r[6]),4u);
    /* 004df4eb */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,1);
    /* 004df4f0 */
    if(!mi_signed_less(f->flags))goto mi_004df4f6;
    /* 004df4f2 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004df4f4 */
    goto mi_004df51f;
mi_004df4f6:;
    /* 004df4f6 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999eau+f->r[6]),2u);
    /* 004df4fd */
    a=f->r[5];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[5]=t;
    /* 004df4ff */
    f->r[1]=f->r[5];
    /* 004df501 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004df504 */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004df506 */
    a=mi_read((0x00799a3cu+f->r[1]*8u),1u);b=0x00000020u;t=mi_alu(f,a,b,8,3);
    /* 004df50e */
    if(!(f->flags&MI_ZF))goto mi_004df514;
    /* 004df510 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004df512 */
    goto mi_004df51f;
mi_004df514:;
    /* 004df514 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004df516 */
    a=f->r[0];b=0x0000ffffu;t=mi_alu(f,a,b,32,1);
    /* 004df51b */
    t=(!mi_signed_less(f->flags))?1u:0u;f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004df51e */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
mi_004df51f:;
    /* 004df51f */
    f->r[0]=mi_read((0x00e94e18u),4u);
    /* 004df524 */
    a=0x00000006u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004df526 */
    f->r[2]=(0x00000000u+f->r[1]+f->r[0]*2u);
    /* 004df529 */
    f->r[1]=0x00f55bb0u;
    /* 004df52e */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004df530 */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004df533 */
    f->r[0]=mi_read((0x005e426cu+f->r[0]),4u);
    /* 004df539 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004df53a */
    f->r[4]-=4;MI_U32(f->r[4])=0x004df53fu;f->boundary(0x004df53au,0x0053dd90u,f);
    /* 004df53f */
    f->r[1]=mi_read((0x00bca2e0u),4u);
    /* 004df545 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004df546 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004df547 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004df548 */
    f->r[0]=(0x00000000u+f->r[3]+f->r[1]*1u);
    /* 004df54b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004df54c */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004df54d */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004df54f */
    f->r[2]=f->r[0];
    /* 004df551 */
    f->r[0]=mi_read((0x00f56d5cu),4u);
    /* 004df556 */
    a=f->r[2];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 004df558 */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004df55b */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004df55d */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004df55e */
    a=f->r[4];b=0x00000074u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004df561 */
    f->r[4]+=4;return;
mi_004df562:;
    /* 004df562 */
    a=mi_read((0x007999eeu+f->r[6]),2u);b=0xffffffffu;t=mi_alu(f,a,b,16,1);
    /* 004df56a */
    if(!(f->flags&MI_ZF))goto mi_004df58d;
    /* 004df56c */
    f->r[0]=mi_read((0x00bca2e0u),4u);
    /* 004df571 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004df572 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004df574 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004df575 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004df576 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004df578 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004df579 */
    f->r[1]=f->r[0];
    /* 004df57b */
    f->r[0]=mi_read((0x00f57454u),4u);
    /* 004df580 */
    a=f->r[1];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[1]=t;
    /* 004df582 */
    a=f->r[1];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 004df585 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004df586 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004df588 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004df589 */
    a=f->r[4];b=0x00000074u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004df58c */
    f->r[4]+=4;return;
mi_004df58d:;
    /* 004df58d */
    f->r[0]=mi_read((0x00799a30u+f->r[6]),4u);
    /* 004df593 */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,1);
    /* 004df598 */
    if(!mi_signed_less(f->flags))goto mi_004df59e;
    /* 004df59a */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004df59c */
    goto mi_004df5c7;
mi_004df59e:;
    /* 004df59e */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999eau+f->r[6]),2u);
    /* 004df5a5 */
    a=f->r[5];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[5]=t;
    /* 004df5a7 */
    f->r[1]=f->r[5];
    /* 004df5a9 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004df5ac */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004df5ae */
    a=mi_read((0x00799a3cu+f->r[1]*8u),1u);b=0x00000040u;t=mi_alu(f,a,b,8,3);
    /* 004df5b6 */
    if(!(f->flags&MI_ZF))goto mi_004df5bc;
    /* 004df5b8 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004df5ba */
    goto mi_004df5c7;
mi_004df5bc:;
    /* 004df5bc */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004df5be */
    a=f->r[0];b=0x0000ffffu;t=mi_alu(f,a,b,32,1);
    /* 004df5c3 */
    t=(!mi_signed_less(f->flags))?1u:0u;f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004df5c6 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
mi_004df5c7:;
    /* 004df5c7 */
    f->r[0]=mi_read((0x00e94e18u),4u);
    /* 004df5cc */
    a=0x00000007u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004df5ce */
    f->r[2]=(0x00000000u+f->r[1]+f->r[0]*2u);
    /* 004df5d1 */
    f->r[1]=0x00f55bb0u;
    /* 004df5d6 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004df5d8 */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004df5db */
    f->r[0]=mi_read((0x005e4270u+f->r[0]),4u);
    /* 004df5e1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004df5e2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004df5e7u;f->boundary(0x004df5e2u,0x0053dd90u,f);
    /* 004df5e7 */
    f->r[1]=mi_read((0x00bca2e0u),4u);
    /* 004df5ed */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004df5ee */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004df5ef */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004df5f0 */
    f->r[0]=(0x00000000u+f->r[3]+f->r[1]*1u);
    /* 004df5f3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004df5f4 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004df5f5 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004df5f7 */
    f->r[2]=f->r[0];
    /* 004df5f9 */
    f->r[0]=mi_read((0x00f56d64u),4u);
    /* 004df5fe */
    a=f->r[2];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 004df600 */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004df603 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004df605 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004df606 */
    a=f->r[4];b=0x00000074u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004df609 */
    f->r[4]+=4;return;
mi_004df60a:;
    /* 004df60a */
    a=mi_read((0x007999eeu+f->r[6]),2u);b=0xffffffffu;t=mi_alu(f,a,b,16,1);
    /* 004df612 */
    if(!(f->flags&MI_ZF))goto mi_004df635;
    /* 004df614 */
    f->r[0]=mi_read((0x00bca2e0u),4u);
    /* 004df619 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004df61a */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004df61c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004df61d */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004df61e */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004df620 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004df621 */
    f->r[1]=f->r[0];
    /* 004df623 */
    f->r[0]=mi_read((0x00f5745cu),4u);
    /* 004df628 */
    a=f->r[1];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[1]=t;
    /* 004df62a */
    a=f->r[1];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 004df62d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004df62e */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004df630 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004df631 */
    a=f->r[4];b=0x00000074u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004df634 */
    f->r[4]+=4;return;
mi_004df635:;
    /* 004df635 */
    f->r[0]=mi_read((0x00799a30u+f->r[6]),4u);
    /* 004df63b */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,1);
    /* 004df640 */
    if(!mi_signed_less(f->flags))goto mi_004df646;
    /* 004df642 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004df644 */
    goto mi_004df66f;
mi_004df646:;
    /* 004df646 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999eau+f->r[6]),2u);
    /* 004df64d */
    a=f->r[5];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[5]=t;
    /* 004df64f */
    f->r[1]=f->r[5];
    /* 004df651 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004df654 */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004df656 */
    a=mi_read((0x00799a3cu+f->r[1]*8u),1u);b=0x00000080u;t=mi_alu(f,a,b,8,3);
    /* 004df65e */
    if(!(f->flags&MI_ZF))goto mi_004df664;
    /* 004df660 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004df662 */
    goto mi_004df66f;
mi_004df664:;
    /* 004df664 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004df666 */
    a=f->r[0];b=0x0000ffffu;t=mi_alu(f,a,b,32,1);
    /* 004df66b */
    t=(!mi_signed_less(f->flags))?1u:0u;f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004df66e */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
mi_004df66f:;
    /* 004df66f */
    f->r[0]=mi_read((0x00e94e18u),4u);
    /* 004df674 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004df676 */
    f->r[2]=(0x00000000u+f->r[1]+f->r[0]*2u);
    /* 004df679 */
    f->r[1]=0x00f55bb0u;
    /* 004df67e */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004df680 */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004df683 */
    f->r[0]=mi_read((0x005e4274u+f->r[0]),4u);
    /* 004df689 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004df68a */
    f->r[4]-=4;MI_U32(f->r[4])=0x004df68fu;f->boundary(0x004df68au,0x0053dd90u,f);
    /* 004df68f */
    f->r[1]=mi_read((0x00bca2e0u),4u);
    /* 004df695 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004df696 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004df697 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004df698 */
    f->r[0]=(0x00000000u+f->r[3]+f->r[1]*1u);
    /* 004df69b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004df69c */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004df69d */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004df69f */
    f->r[2]=f->r[0];
    /* 004df6a1 */
    f->r[0]=mi_read((0x00f56d6cu),4u);
    /* 004df6a6 */
    a=f->r[2];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 004df6a8 */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004df6ab */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004df6ad */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004df6ae */
    a=f->r[4];b=0x00000074u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004df6b1 */
    f->r[4]+=4;return;
mi_004df6b2:;
    /* 004df6b2 */
    a=mi_read((0x007999eeu+f->r[6]),2u);b=0xffffffffu;t=mi_alu(f,a,b,16,1);
    /* 004df6ba */
    if((f->flags&MI_ZF))goto mi_004df6c2;
    /* 004df6bc */
    MI_U32((0x00000000u+f->r[7]))=(uint32_t)(0x00000005u);
mi_004df6c2:;
    /* 004df6c2 */
    f->r[6]=mi_read((0x00799a30u+f->r[6]),4u);
    /* 004df6c8 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 004df6cb */
    if(!(f->flags&MI_ZF))goto mi_004df6da;
    /* 004df6cd */
    f->r[0]=mi_read((0x00f56d74u),4u);
    /* 004df6d2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004df6d3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004df6d4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004df6d5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004df6d6 */
    a=f->r[4];b=0x00000074u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004df6d9 */
    f->r[4]+=4;return;
mi_004df6da:;
    /* 004df6da */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004df6dd */
    if(!(f->flags&MI_ZF))goto mi_004df6ec;
    /* 004df6df */
    f->r[0]=mi_read((0x00f56b54u),4u);
    /* 004df6e4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004df6e5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004df6e6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004df6e7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004df6e8 */
    a=f->r[4];b=0x00000074u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004df6eb */
    f->r[4]+=4;return;
mi_004df6ec:;
    /* 004df6ec */
    a=f->r[1];b=0x00010000u;t=mi_alu(f,a,b,32,3);
    /* 004df6f2 */
    if((f->flags&MI_ZF))goto mi_004df987;
    /* 004df6f8 */
    a=mi_read((0x00784cbcu),1u);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 004df6ff */
    if(!(f->flags&MI_ZF))goto mi_004df70e;
    /* 004df701 */
    f->r[0]=mi_read((0x00f57464u),4u);
    /* 004df706 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004df707 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004df708 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004df709 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004df70a */
    a=f->r[4];b=0x00000074u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004df70d */
    f->r[4]+=4;return;
mi_004df70e:;
    /* 004df70e */
    f->r[0]=mi_read((0x00f5736cu),4u);
    /* 004df713 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004df714 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004df715 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004df716 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004df717 */
    a=f->r[4];b=0x00000074u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004df71a */
    f->r[4]+=4;return;
mi_004df71b:;
    /* 004df71b */
    a=mi_read((0x007999eeu+f->r[6]),2u);b=0xffffffffu;t=mi_alu(f,a,b,16,1);
    /* 004df723 */
    if((f->flags&MI_ZF))goto mi_004df72b;
    /* 004df725 */
    MI_U32((0x00000000u+f->r[7]))=(uint32_t)(0x00000005u);
mi_004df72b:;
    /* 004df72b */
    f->r[6]=mi_read((0x00799a30u+f->r[6]),4u);
    /* 004df731 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 004df734 */
    if(!(f->flags&MI_ZF))goto mi_004df744;
    /* 004df736 */
    f->r[0]=mi_read((0x00f56d74u),4u);
    /* 004df73b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004df73c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004df73d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004df73e */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004df73f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004df740 */
    a=f->r[4];b=0x00000074u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004df743 */
    f->r[4]+=4;return;
mi_004df744:;
    /* 004df744 */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004df747 */
    goto mi_004df974;
mi_004df74c:;
    /* 004df74c */
    a=mi_read((0x007999eeu+f->r[6]),2u);b=0xffffffffu;t=mi_alu(f,a,b,16,1);
    /* 004df754 */
    if((f->flags&MI_ZF))goto mi_004df75c;
    /* 004df756 */
    MI_U32((0x00000000u+f->r[7]))=(uint32_t)(0x00000005u);
mi_004df75c:;
    /* 004df75c */
    f->r[6]=mi_read((0x00799a30u+f->r[6]),4u);
    /* 004df762 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 004df765 */
    if(!(f->flags&MI_ZF))goto mi_004df744;
    /* 004df767 */
    f->r[1]=mi_read((0x00f56d74u),4u);
    /* 004df76d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004df76e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004df76f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004df770 */
    f->r[0]=(0x00000002u+f->r[1]);
    /* 004df773 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004df774 */
    a=f->r[4];b=0x00000074u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004df777 */
    f->r[4]+=4;return;
mi_004df778:;
    /* 004df778 */
    a=mi_read((0x007999eeu+f->r[6]),2u);b=0xffffffffu;t=mi_alu(f,a,b,16,1);
    /* 004df780 */
    if((f->flags&MI_ZF))goto mi_004df788;
    /* 004df782 */
    MI_U32((0x00000000u+f->r[7]))=(uint32_t)(0x00000005u);
mi_004df788:;
    /* 004df788 */
    f->r[6]=mi_read((0x00799a30u+f->r[6]),4u);
    /* 004df78e */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 004df791 */
    if(!(f->flags&MI_ZF))goto mi_004df744;
    /* 004df793 */
    f->r[2]=mi_read((0x00f56d74u),4u);
    /* 004df799 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004df79a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004df79b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004df79c */
    f->r[0]=(0x00000003u+f->r[2]);
    /* 004df79f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004df7a0 */
    a=f->r[4];b=0x00000074u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004df7a3 */
    f->r[4]+=4;return;
mi_004df7a4:;
    /* 004df7a4 */
    a=mi_read((0x007999eeu+f->r[6]),2u);b=0xffffffffu;t=mi_alu(f,a,b,16,1);
    /* 004df7ac */
    if((f->flags&MI_ZF))goto mi_004df7b4;
    /* 004df7ae */
    MI_U32((0x00000000u+f->r[7]))=(uint32_t)(0x00000001u);
mi_004df7b4:;
    /* 004df7b4 */
    a=mi_read((0x00799a30u+f->r[6]),4u);b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 004df7bb */
    if(!(f->flags&MI_ZF))goto mi_004df964;
    /* 004df7c1 */
    f->r[0]=f->r[3];
    /* 004df7c3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004df7c4 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004df7c5 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004df7c7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004df7c8 */
    f->r[1]=f->r[0];
    /* 004df7ca */
    f->r[0]=mi_read((0x00bca2e0u),4u);
    /* 004df7cf */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004df7d0 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004df7d2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004df7d3 */
    a=f->r[1];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[1]=t;
    /* 004df7d5 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004df7d7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004df7d8 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[1]*4u);
    /* 004df7db */
    f->r[1]=mi_read((0x00f56d74u),4u);
    /* 004df7e1 */
    f->r[0]=mi_read((0x00000034u+f->r[4]+f->r[2]*4u),4u);
    /* 004df7e5 */
    f->r[0]=(0x00000004u+f->r[0]+f->r[1]*1u);
    /* 004df7e9 */
    a=f->r[4];b=0x00000074u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004df7ec */
    f->r[4]+=4;return;
mi_004df7ed:;
    /* 004df7ed */
    a=mi_read((0x007999eeu+f->r[6]),2u);b=0xffffffffu;t=mi_alu(f,a,b,16,1);
    /* 004df7f5 */
    if((f->flags&MI_ZF))goto mi_004df7fd;
    /* 004df7f7 */
    MI_U32((0x00000000u+f->r[7]))=(uint32_t)(0x00000001u);
mi_004df7fd:;
    /* 004df7fd */
    a=mi_read((0x00799a30u+f->r[6]),4u);b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 004df804 */
    if(!(f->flags&MI_ZF))goto mi_004df964;
    /* 004df80a */
    f->r[0]=f->r[3];
    /* 004df80c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004df80d */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004df80e */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004df810 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004df811 */
    f->r[1]=f->r[0];
    /* 004df813 */
    f->r[0]=mi_read((0x00bca2e0u),4u);
    /* 004df818 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004df819 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004df81b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004df81c */
    a=f->r[1];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[1]=t;
    /* 004df81e */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004df820 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004df821 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[1]*4u);
    /* 004df824 */
    f->r[1]=mi_read((0x00f56d74u),4u);
    /* 004df82a */
    f->r[0]=mi_read((0x00000034u+f->r[4]+f->r[2]*4u),4u);
    /* 004df82e */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004df831 */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004df834 */
    f->r[0]=(0x00000004u+f->r[0]+f->r[1]*1u);
    /* 004df838 */
    a=f->r[4];b=0x00000074u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004df83b */
    f->r[4]+=4;return;
mi_004df83c:;
    /* 004df83c */
    a=mi_read((0x007999eeu+f->r[6]),2u);b=0xffffffffu;t=mi_alu(f,a,b,16,1);
    /* 004df844 */
    if((f->flags&MI_ZF))goto mi_004df84c;
    /* 004df846 */
    MI_U32((0x00000000u+f->r[7]))=(uint32_t)(0x00000001u);
mi_004df84c:;
    /* 004df84c */
    a=mi_read((0x00799a30u+f->r[6]),4u);b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 004df853 */
    if(!(f->flags&MI_ZF))goto mi_004df964;
    /* 004df859 */
    f->r[0]=f->r[3];
    /* 004df85b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004df85c */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004df85d */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004df85f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004df860 */
    f->r[1]=f->r[0];
    /* 004df862 */
    f->r[0]=mi_read((0x00bca2e0u),4u);
    /* 004df867 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004df868 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004df86a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004df86b */
    a=f->r[1];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[1]=t;
    /* 004df86d */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004df86f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004df870 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[1]*4u);
    /* 004df873 */
    f->r[1]=mi_read((0x00f56d74u),4u);
    /* 004df879 */
    f->r[0]=mi_read((0x00000034u+f->r[4]+f->r[2]*4u),4u);
    /* 004df87d */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004df880 */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004df883 */
    f->r[0]=(0x00000009u+f->r[1]+f->r[0]*2u);
    /* 004df887 */
    a=f->r[4];b=0x00000074u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004df88a */
    f->r[4]+=4;return;
mi_004df88b:;
    /* 004df88b */
    a=mi_read((0x007999eeu+f->r[6]),2u);b=0xffffffffu;t=mi_alu(f,a,b,16,1);
    /* 004df893 */
    if((f->flags&MI_ZF))goto mi_004df89b;
    /* 004df895 */
    MI_U32((0x00000000u+f->r[7]))=(uint32_t)(0x00000001u);
mi_004df89b:;
    /* 004df89b */
    a=mi_read((0x00799a30u+f->r[6]),4u);b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 004df8a2 */
    if(!(f->flags&MI_ZF))goto mi_004df964;
    /* 004df8a8 */
    f->r[0]=f->r[3];
    /* 004df8aa */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004df8ab */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004df8ac */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004df8ae */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004df8af */
    f->r[1]=f->r[0];
    /* 004df8b1 */
    f->r[0]=mi_read((0x00bca2e0u),4u);
    /* 004df8b6 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004df8b7 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004df8b9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004df8ba */
    a=f->r[1];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[1]=t;
    /* 004df8bc */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004df8be */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004df8bf */
    f->r[2]=(0x00000000u+f->r[0]+f->r[1]*4u);
    /* 004df8c2 */
    f->r[1]=mi_read((0x00f56d74u),4u);
    /* 004df8c8 */
    f->r[0]=mi_read((0x00000034u+f->r[4]+f->r[2]*4u),4u);
    /* 004df8cc */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004df8cf */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004df8d2 */
    f->r[0]=(0x00000008u+f->r[1]+f->r[0]*2u);
    /* 004df8d6 */
    a=f->r[4];b=0x00000074u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004df8d9 */
    f->r[4]+=4;return;
mi_004df8da:;
    /* 004df8da */
    a=mi_read((0x007999eeu+f->r[6]),2u);b=0xffffffffu;t=mi_alu(f,a,b,16,1);
    /* 004df8e2 */
    if((f->flags&MI_ZF))goto mi_004df8ea;
    /* 004df8e4 */
    MI_U32((0x00000000u+f->r[7]))=(uint32_t)(0x00000001u);
mi_004df8ea:;
    /* 004df8ea */
    a=mi_read((0x00799a30u+f->r[6]),4u);b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 004df8f1 */
    if(!(f->flags&MI_ZF))goto mi_004df964;
    /* 004df8f3 */
    f->r[0]=f->r[3];
    /* 004df8f5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004df8f6 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004df8f7 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004df8f9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004df8fa */
    f->r[1]=f->r[0];
    /* 004df8fc */
    f->r[0]=mi_read((0x00bca2e0u),4u);
    /* 004df901 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004df902 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004df904 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004df905 */
    a=f->r[1];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[1]=t;
    /* 004df907 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004df909 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004df90a */
    f->r[2]=(0x00000000u+f->r[0]+f->r[1]*4u);
    /* 004df90d */
    f->r[1]=mi_read((0x00f56d74u),4u);
    /* 004df913 */
    f->r[0]=mi_read((0x00000034u+f->r[4]+f->r[2]*4u),4u);
    /* 004df917 */
    f->r[0]=(0x00000008u+f->r[1]+f->r[0]*2u);
    /* 004df91b */
    a=f->r[4];b=0x00000074u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004df91e */
    f->r[4]+=4;return;
mi_004df91f:;
    /* 004df91f */
    a=mi_read((0x007999eeu+f->r[6]),2u);b=0xffffffffu;t=mi_alu(f,a,b,16,1);
    /* 004df927 */
    if((f->flags&MI_ZF))goto mi_004df92f;
    /* 004df929 */
    MI_U32((0x00000000u+f->r[7]))=(uint32_t)(0x00000001u);
mi_004df92f:;
    /* 004df92f */
    a=mi_read((0x00799a30u+f->r[6]),4u);b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 004df936 */
    if(!(f->flags&MI_ZF))goto mi_004df964;
    /* 004df938 */
    f->r[0]=f->r[3];
    /* 004df93a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004df93b */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004df93c */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004df93e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004df93f */
    f->r[1]=f->r[0];
    /* 004df941 */
    f->r[0]=mi_read((0x00bca2e0u),4u);
    /* 004df946 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004df947 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004df949 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004df94a */
    a=f->r[1];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[1]=t;
    /* 004df94c */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004df94e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004df94f */
    f->r[2]=(0x00000000u+f->r[0]+f->r[1]*4u);
    /* 004df952 */
    f->r[1]=mi_read((0x00f56d74u),4u);
    /* 004df958 */
    f->r[0]=mi_read((0x00000034u+f->r[4]+f->r[2]*4u),4u);
    /* 004df95c */
    f->r[0]=(0x00000009u+f->r[1]+f->r[0]*2u);
    /* 004df960 */
    a=f->r[4];b=0x00000074u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004df963 */
    f->r[4]+=4;return;
mi_004df964:;
    /* 004df964 */
    f->r[2]=mi_read((0x00000000u+f->r[7]),4u);
    /* 004df966 */
    a=f->r[2];b=0x00000004u;t=mi_alu(f,a,b,32,4);f->r[2]=t;
    /* 004df969 */
    MI_U32((0x00000000u+f->r[7]))=(uint32_t)(f->r[2]);
    /* 004df96b */
    f->r[0]=mi_read((0x00799a30u+f->r[6]),4u);
    /* 004df971 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
mi_004df974:;
    /* 004df974 */
    if(!(f->flags&MI_ZF))goto mi_004df6ec;
    /* 004df97a */
    f->r[0]=mi_read((0x00f56b54u),4u);
    /* 004df97f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004df980 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004df981 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004df982 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004df983 */
    a=f->r[4];b=0x00000074u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004df986 */
    f->r[4]+=4;return;
mi_004df987:;
    /* 004df987 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004df988 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004df989 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004df98a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004df98c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004df98d */
    a=f->r[4];b=0x00000074u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004df990 */
    f->r[4]+=4;return;
}
