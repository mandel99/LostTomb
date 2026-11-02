/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "world_metric_helpers_model.h"
#include <stdlib.h>
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

static const uint8_t mi_rom_004325fc[]={0x79,0x25,0x43,0x00,0x3d,0x25,0x43,0x00,0x49,0x25,0x43,0x00,0x55,0x25,0x43,0x00,0x61,0x25,0x43,0x00,0x6d,0x25,0x43,0x00};
static __inline uint32_t mi_read_00432320(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x004325fcu&&width<=24u&&(offset=address-0x004325fcu)<=24u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004325fc[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}

void restore_building_world_00432320(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00432320 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 00432323 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00432324 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00432325 */
    f->r[6]=mi_read_00432320((0x0070e488u),4u);
    /* 0043232b */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043232c */
    f->r[2]=mi_read_00432320((0x00af598cu+f->r[6]*4u),4u);
    /* 00432333 */
    a=f->r[2];b=0x26341777u;t=mi_alu(f,a,b,32,3);
    /* 00432339 */
    if((f->flags&MI_ZF))goto mi_0043248e;
    /* 0043233f */
    a=(f->r[2]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,3);
    /* 00432342 */
    if(!(f->flags&MI_ZF))goto mi_004325f5;
    /* 00432348 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read_00432320((0x00b285ccu+f->r[6]),1u))&0xffu)<<0);
    /* 0043234e */
    f->r[0]=mi_read_00432320((0x00b7482cu+f->r[6]*4u),4u);
    /* 00432355 */
    a=f->r[1];b=0x0000000fu;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 00432358 */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[0]);
    /* 0043235d */
    if(!(f->flags&MI_ZF))goto mi_0043237b;
    /* 0043235f */
    f->r[1]=mi_read_00432320((0x00000020u+f->r[4]),4u);
    /* 00432363 */
    f->r[2]=mi_read_00432320((0x0000001cu+f->r[4]),4u);
    /* 00432367 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00432368 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00432369 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043236a */
    f->r[1]=0x00f55bb0u;
    /* 0043236f */
    f->r[4]-=4;MI_U32(f->r[4])=0x00432374u;f->boundary(0x0043236fu,0x0053f250u,f);
    /* 00432374 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00432375 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00432376 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00432377 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043237a */
    f->r[4]+=4;return;
mi_0043237b:;
    /* 0043237b */
    a=f->r[1];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0043237e */
    if(!(f->flags&MI_ZF))goto mi_004323ae;
    /* 00432380 */
    f->r[1]=mi_read_00432320((0x00000020u+f->r[4]),4u);
    /* 00432384 */
    f->r[3]=mi_read_00432320((0x0070e44cu),4u);
    /* 0043238a */
    f->r[2]=mi_read_00432320((0x0070e45cu),4u);
    /* 00432390 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00432392 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00432393 */
    f->r[1]=mi_read_00432320((0x00000020u+f->r[4]),4u);
    /* 00432397 */
    f->r[2]=(0x00000001u+f->r[2]+f->r[1]*1u);
    /* 0043239b */
    f->r[1]=0x00f55bb0u;
    /* 004323a0 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004323a1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004323a2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004323a7u;f->boundary(0x004323a2u,0x0053f250u,f);
    /* 004323a7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004323a8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004323a9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004323aa */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004323ad */
    f->r[4]+=4;return;
mi_004323ae:;
    /* 004323ae */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 004323b1 */
    if(!(f->flags&MI_ZF))goto mi_004323e1;
    /* 004323b3 */
    f->r[1]=mi_read_00432320((0x00000020u+f->r[4]),4u);
    /* 004323b7 */
    f->r[3]=mi_read_00432320((0x0070e448u),4u);
    /* 004323bd */
    f->r[2]=mi_read_00432320((0x0070e458u),4u);
    /* 004323c3 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004323c5 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004323c6 */
    f->r[1]=mi_read_00432320((0x00000020u+f->r[4]),4u);
    /* 004323ca */
    f->r[2]=(0x00000002u+f->r[2]+f->r[1]*1u);
    /* 004323ce */
    f->r[1]=0x00f55bb0u;
    /* 004323d3 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004323d4 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004323d5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004323dau;f->boundary(0x004323d5u,0x0053f250u,f);
    /* 004323da */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004323db */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004323dc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004323dd */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004323e0 */
    f->r[4]+=4;return;
mi_004323e1:;
    /* 004323e1 */
    a=f->r[1];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 004323e4 */
    if(!(f->flags&MI_ZF))goto mi_00432414;
    /* 004323e6 */
    f->r[1]=mi_read_00432320((0x00000020u+f->r[4]),4u);
    /* 004323ea */
    f->r[3]=mi_read_00432320((0x0070e444u),4u);
    /* 004323f0 */
    f->r[2]=mi_read_00432320((0x0070e454u),4u);
    /* 004323f6 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004323f8 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004323f9 */
    f->r[1]=mi_read_00432320((0x00000020u+f->r[4]),4u);
    /* 004323fd */
    f->r[2]=(0x00000003u+f->r[2]+f->r[1]*1u);
    /* 00432401 */
    f->r[1]=0x00f55bb0u;
    /* 00432406 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00432407 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00432408 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043240du;f->boundary(0x00432408u,0x0053f250u,f);
    /* 0043240d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0043240e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0043240f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00432410 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00432413 */
    f->r[4]+=4;return;
mi_00432414:;
    /* 00432414 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 00432417 */
    if(!(f->flags&MI_ZF))goto mi_00432447;
    /* 00432419 */
    f->r[1]=mi_read_00432320((0x00000020u+f->r[4]),4u);
    /* 0043241d */
    f->r[3]=mi_read_00432320((0x0070e440u),4u);
    /* 00432423 */
    f->r[2]=mi_read_00432320((0x0070e450u),4u);
    /* 00432429 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0043242b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043242c */
    f->r[1]=mi_read_00432320((0x00000020u+f->r[4]),4u);
    /* 00432430 */
    f->r[2]=(0x00000004u+f->r[2]+f->r[1]*1u);
    /* 00432434 */
    f->r[1]=0x00f55bb0u;
    /* 00432439 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043243a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043243b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00432440u;f->boundary(0x0043243bu,0x0053f250u,f);
    /* 00432440 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00432441 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00432442 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00432443 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00432446 */
    f->r[4]+=4;return;
mi_00432447:;
    /* 00432447 */
    a=f->r[1];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 0043244a */
    if(!(f->flags&MI_ZF))goto mi_004325f5;
    /* 00432450 */
    f->r[1]=mi_read_00432320((0x00000020u+f->r[4]),4u);
    /* 00432454 */
    f->r[3]=mi_read_00432320((0x0070e440u),4u);
    /* 0043245a */
    f->r[7]=mi_read_00432320((0x0070e44cu),4u);
    /* 00432460 */
    f->r[2]=mi_read_00432320((0x0070e45cu),4u);
    /* 00432466 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00432468 */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0043246a */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043246b */
    f->r[1]=mi_read_00432320((0x0070e450u),4u);
    /* 00432471 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00432473 */
    f->r[2]=mi_read_00432320((0x00000020u+f->r[4]),4u);
    /* 00432477 */
    f->r[1]=(0x00000005u+f->r[1]+f->r[2]*1u);
    /* 0043247b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043247c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043247d */
    f->r[1]=0x00f55bb0u;
    /* 00432482 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00432487u;f->boundary(0x00432482u,0x0053f250u,f);
    /* 00432487 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00432488 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00432489 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0043248a */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043248d */
    f->r[4]+=4;return;
mi_0043248e:;
    /* 0043248e */
    a=(f->r[2]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,3);
    /* 00432491 */
    if(!(f->flags&MI_ZF))goto mi_004324d7;
    /* 00432493 */
    f->r[0]=mi_read_00432320((0x00f56d14u),4u);
    /* 00432498 */
    a=f->r[2];b=0x00002080u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0043249e */
    a=((f->r[2]>>8)&0xffu);b=0x00000020u;t=mi_alu(f,a,b,8,3);
    /* 004324a1 */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[0]);
    /* 004324a6 */
    if((f->flags&MI_ZF))goto mi_004324ad;
    /* 004324a8 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004324ab */
    goto mi_004324b6;
mi_004324ad:;
    /* 004324ad */
    a=f->r[2];b=0x00000080u;t=mi_alu(f,a,b,32,1);
    /* 004324b3 */
    if(!(f->flags&MI_ZF))goto mi_004324bb;
    /* 004324b5 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
mi_004324b6:;
    /* 004324b6 */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[0]);
mi_004324bb:;
    /* 004324bb */
    f->r[2]=mi_read_00432320((0x00000020u+f->r[4]),4u);
    /* 004324bf */
    f->r[1]=mi_read_00432320((0x0000001cu+f->r[4]),4u);
    /* 004324c3 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004324c4 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004324c5 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004324c6 */
    f->r[1]=0x00f55bb0u;
    /* 004324cb */
    f->r[4]-=4;MI_U32(f->r[4])=0x004324d0u;f->boundary(0x004324cbu,0x0053f250u,f);
    /* 004324d0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004324d1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004324d2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004324d3 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004324d6 */
    f->r[4]+=4;return;
mi_004324d7:;
    /* 004324d7 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read_00432320((0x00b4e6fcu+f->r[6]*2u),2u);
    /* 004324df */
    f->r[0]=f->r[1];
    /* 004324e1 */
    a=f->r[2];b=0x00002080u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004324e7 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004324ea */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004324ec */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004324ef */
    f->r[3]=(f->r[3]&0xffffff00u)|(((mi_read_00432320((0x007999ddu+f->r[0]),1u))&0xffu)<<0);
    /* 004324f5 */
    a=(f->r[3]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004324f7 */
    if((f->flags&MI_ZF))goto mi_0043251e;
    /* 004324f9 */
    a=mi_read_00432320((0x00799a4fu+f->r[0]),1u);b=0x00000000u;t=mi_alu(f,a,b,8,1);
    /* 00432500 */
    if((f->flags&MI_ZF))goto mi_0043251e;
    /* 00432502 */
    f->r[2]=mi_read_00432320((0x00000020u+f->r[4]),4u);
    /* 00432506 */
    f->r[0]=mi_read_00432320((0x0000001cu+f->r[4]),4u);
    /* 0043250a */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043250c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043250d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043250e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043250f */
    f->r[4]-=4;MI_U32(f->r[4])=0x00432514u;f->boundary(0x0043250fu,0x00432d30u,f);
    /* 00432514 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00432517 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00432518 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00432519 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0043251a */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043251d */
    f->r[4]+=4;return;
mi_0043251e:;
    /* 0043251e */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 00432520 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00432524 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00432528 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read_00432320((0x00b285ccu+f->r[6]),1u))&0xffu)<<0);
    /* 0043252e */
    a=f->r[1];b=0x0000000fu;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 00432531 */
    a=f->r[1];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 00432534 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00432579;
    /* 00432536 */
    switch(mi_read_00432320((0x004325fcu+f->r[1]*4u),4u)){case 0x0043253du:goto mi_0043253d;case 0x00432549u:goto mi_00432549;case 0x00432555u:goto mi_00432555;case 0x00432561u:goto mi_00432561;case 0x0043256du:goto mi_0043256d;case 0x00432579u:goto mi_00432579;default:abort();}
mi_0043253d:;
    /* 0043253d */
    f->r[6]=0x005d0050u;
    /* 00432542 */
    f->r[7]=0x00000004u;
    /* 00432547 */
    goto mi_00432582;
mi_00432549:;
    /* 00432549 */
    f->r[6]=0x005d0070u;
    /* 0043254e */
    f->r[7]=0x00000009u;
    /* 00432553 */
    goto mi_00432582;
mi_00432555:;
    /* 00432555 */
    f->r[6]=0x005d00b8u;
    /* 0043255a */
    f->r[7]=0x00000010u;
    /* 0043255f */
    goto mi_00432582;
mi_00432561:;
    /* 00432561 */
    f->r[6]=0x005d0138u;
    /* 00432566 */
    f->r[7]=0x00000019u;
    /* 0043256b */
    goto mi_00432582;
mi_0043256d:;
    /* 0043256d */
    f->r[6]=0x005d0200u;
    /* 00432572 */
    f->r[7]=0x00000024u;
    /* 00432577 */
    goto mi_00432582;
mi_00432579:;
    /* 00432579 */
    f->r[6]=(0x00000010u+f->r[4]);
    /* 0043257d */
    f->r[7]=0x00000001u;
mi_00432582:;
    /* 00432582 */
    f->r[1]=mi_read_00432320((0x00f56d14u),4u);
    /* 00432588 */
    a=(f->r[3]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0043258a */
    if((f->flags&MI_ZF))goto mi_00432592;
    /* 0043258c */
    f->r[1]=mi_read_00432320((0x00f56d1cu),4u);
mi_00432592:;
    /* 00432592 */
    a=((f->r[2]>>8)&0xffu);b=0x00000020u;t=mi_alu(f,a,b,8,3);
    /* 00432595 */
    MI_U32((0x0000000cu+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00432599 */
    if(!(f->flags&MI_ZF))goto mi_004325b8;
    /* 0043259b */
    a=(f->r[3]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0043259d */
    if((f->flags&MI_ZF))goto mi_004325a9;
    /* 0043259f */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read_00432320((0x00799a4fu+f->r[0]),1u))&0xffu)<<0);
    /* 004325a5 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004325a7 */
    if(!(f->flags&MI_ZF))goto mi_004325b8;
mi_004325a9:;
    /* 004325a9 */
    a=f->r[2];b=0x00000080u;t=mi_alu(f,a,b,32,1);
    /* 004325af */
    if(!(f->flags&MI_ZF))goto mi_004325c3;
    /* 004325b1 */
    f->r[0]=mi_read_00432320((0x0000000cu+f->r[4]),4u);
    /* 004325b5 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004325b6 */
    goto mi_004325bf;
mi_004325b8:;
    /* 004325b8 */
    f->r[0]=mi_read_00432320((0x0000000cu+f->r[4]),4u);
    /* 004325bc */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
mi_004325bf:;
    /* 004325bf */
    MI_U32((0x0000000cu+f->r[4]))=(uint32_t)(f->r[0]);
mi_004325c3:;
    /* 004325c3 */
    f->r[2]=f->r[7];
    /* 004325c5 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 004325c6 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 004325c8 */
    if((f->flags&MI_ZF))goto mi_004325f5;
    /* 004325ca */
    f->r[3]=mi_read_00432320((0x00000020u+f->r[4]),4u);
    /* 004325ce */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004325cf */
    f->r[5]=mi_read_00432320((0x00000020u+f->r[4]),4u);
    /* 004325d3 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
mi_004325d4:;
    /* 004325d4 */
    f->r[0]=mi_read_00432320((0x00000004u+f->r[6]),4u);
    /* 004325d7 */
    f->r[1]=mi_read_00432320((0x00000000u+f->r[6]),4u);
    /* 004325d9 */
    f->r[2]=mi_read_00432320((0x00000010u+f->r[4]),4u);
    /* 004325dd */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004325df */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004325e1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004325e2 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004325e3 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004325e4 */
    f->r[1]=0x00f55bb0u;
    /* 004325e9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004325eeu;f->boundary(0x004325e9u,0x0053f250u,f);
    /* 004325ee */
    a=f->r[6];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004325f1 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 004325f2 */
    if(!(f->flags&MI_ZF))goto mi_004325d4;
    /* 004325f4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
mi_004325f5:;
    /* 004325f5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004325f6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004325f7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004325f8 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004325fb */
    f->r[4]+=4;return;
}


static const uint8_t mi_rom_00432908[]={0x8d,0x28,0x43,0x00,0x99,0x28,0x43,0x00,0xab,0x28,0x43,0x00,0xbd,0x28,0x43,0x00,0xcf,0x28,0x43,0x00};
static __inline uint32_t mi_read_00432820(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x00432908u&&width<=20u&&(offset=address-0x00432908u)<=20u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00432908[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}

void restore_building_world_00432820(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00432820 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00432821 */
    f->r[5]=mi_read_00432820((0x0070e488u),4u);
    /* 00432827 */
    f->r[0]=mi_read_00432820((0x00af598cu+f->r[5]*4u),4u);
    /* 0043282e */
    a=f->r[0];b=0x20000002u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 00432833 */
    a=f->r[0];b=0x20000002u;t=mi_alu(f,a,b,32,1);
    /* 00432838 */
    if(!(f->flags&MI_ZF))goto mi_0043285b;
    /* 0043283a */
    f->r[1]=mi_read_00432820((0x0000000cu+f->r[4]),4u);
    /* 0043283e */
    f->r[2]=mi_read_00432820((0x00000008u+f->r[4]),4u);
    /* 00432842 */
    f->r[0]=mi_read_00432820((0x00f56d14u),4u);
    /* 00432847 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00432848 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00432849 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043284a */
    f->r[1]=0x00f55bb0u;
    /* 0043284f */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[0]);
    /* 00432854 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00432859u;f->boundary(0x00432854u,0x0053f250u,f);
    /* 00432859 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0043285a */
    f->r[4]+=4;return;
mi_0043285b:;
    /* 0043285b */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read_00432820((0x00b285ccu+f->r[5]),1u))&0xffu)<<0);
    /* 00432861 */
    f->r[0]=mi_read_00432820((0x0000000cu+f->r[4]),4u);
    /* 00432865 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00432866 */
    f->r[3]=mi_read_00432820((0x00b7482cu+f->r[5]*4u),4u);
    /* 0043286d */
    a=f->r[2];b=0x0000000fu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 00432870 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00432871 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00432872 */
    f->r[7]=mi_read_00432820((0x00000014u+f->r[4]),4u);
    /* 00432876 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00432877 */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[3]);
    /* 0043287d */
    a=f->r[2];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 00432880 */
    f->r[1]=f->r[7];
    /* 00432882 */
    f->r[6]=f->r[0];
    /* 00432884 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004328f3;
    /* 00432886 */
    switch(mi_read_00432820((0x00432908u+f->r[2]*4u),4u)){case 0x0043288du:goto mi_0043288d;case 0x00432899u:goto mi_00432899;case 0x004328abu:goto mi_004328ab;case 0x004328bdu:goto mi_004328bd;case 0x004328cfu:goto mi_004328cf;default:abort();}
mi_0043288d:;
    /* 0043288d */
    f->r[1]=mi_read_00432820((0x0070e45cu),4u);
    /* 00432893 */
    f->r[1]=(0x00000001u+f->r[1]+f->r[7]*1u);
    /* 00432897 */
    goto mi_004328e9;
mi_00432899:;
    /* 00432899 */
    f->r[2]=mi_read_00432820((0x0070e458u),4u);
    /* 0043289f */
    f->r[1]=(0x00000002u+f->r[2]+f->r[7]*1u);
    /* 004328a3 */
    f->r[2]=mi_read_00432820((0x0070e448u),4u);
    /* 004328a9 */
    goto mi_004328ef;
mi_004328ab:;
    /* 004328ab */
    f->r[1]=mi_read_00432820((0x0070e454u),4u);
    /* 004328b1 */
    f->r[2]=mi_read_00432820((0x0070e444u),4u);
    /* 004328b7 */
    f->r[1]=(0x00000003u+f->r[1]+f->r[7]*1u);
    /* 004328bb */
    goto mi_004328ef;
mi_004328bd:;
    /* 004328bd */
    f->r[2]=mi_read_00432820((0x0070e450u),4u);
    /* 004328c3 */
    f->r[1]=(0x00000004u+f->r[2]+f->r[7]*1u);
    /* 004328c7 */
    f->r[2]=mi_read_00432820((0x0070e440u),4u);
    /* 004328cd */
    goto mi_004328ef;
mi_004328cf:;
    /* 004328cf */
    f->r[1]=mi_read_00432820((0x0070e45cu),4u);
    /* 004328d5 */
    f->r[2]=mi_read_00432820((0x0070e450u),4u);
    /* 004328db */
    f->r[6]=mi_read_00432820((0x0070e440u),4u);
    /* 004328e1 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004328e3 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004328e5 */
    f->r[1]=(0x00000005u+f->r[2]+f->r[7]*1u);
mi_004328e9:;
    /* 004328e9 */
    f->r[2]=mi_read_00432820((0x0070e44cu),4u);
mi_004328ef:;
    /* 004328ef */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004328f1 */
    f->r[6]=f->r[0];
mi_004328f3:;
    /* 004328f3 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004328f4 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004328f5 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004328f6 */
    f->r[1]=0x00f55bb0u;
    /* 004328fb */
    f->r[4]-=4;MI_U32(f->r[4])=0x00432900u;f->boundary(0x004328fbu,0x0053f250u,f);
    /* 00432900 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00432901 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00432902 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00432903 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00432904 */
    f->r[4]+=4;return;
}


void restore_building_world_00435050(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00435050 */
    f->r[1]=MI_U32((0x0070e488u));
    /* 00435056 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00435058 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00a5d4ccu+f->r[1])))&0xffu)<<0);
    /* 0043505e */
    f->r[2]=MI_U32((0x00af598cu+f->r[1]*4u));
    /* 00435065 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00435067 */
    if(!(f->flags&MI_ZF))goto mi_004351b1;
    /* 0043506d */
    a=f->r[2];b=0x26341677u;t=mi_alu(f,a,b,32,3);
    /* 00435073 */
    if((f->flags&MI_ZF))goto mi_00435262;
    /* 00435079 */
    a=(f->r[2]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,3);
    /* 0043507c */
    if(!(f->flags&MI_ZF))goto mi_00435262;
    /* 00435082 */
    a=MI_U8((0x00b67d1cu+f->r[1]));b=0x00000040u;t=mi_alu(f,a,b,8,3);
    /* 00435089 */
    if((f->flags&MI_ZF))goto mi_00435284;
    /* 0043508f */
    f->r[0]=MI_U32((0x00b7482cu+f->r[1]*4u));
    /* 00435096 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00b285ccu+f->r[1])))&0xffu)<<0);
    /* 0043509c */
    a=f->r[1];b=0x0000000fu;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 0043509f */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[0]);
    /* 004350a4 */
    if(!(f->flags&MI_ZF))goto mi_004350bc;
    /* 004350a6 */
    f->r[1]=MI_U32((0x00000008u+f->r[4]));
    /* 004350aa */
    f->r[2]=MI_U32((0x00000004u+f->r[4]));
    /* 004350ae */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004350af */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004350b0 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004350b1 */
    f->r[1]=0x00f55bb0u;
    /* 004350b6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004350bbu;f->boundary(0x004350b6u,0x0053f250u,f);
    /* 004350bb */
    f->r[4]+=4;return;
mi_004350bc:;
    /* 004350bc */
    a=f->r[1];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004350bf */
    if(!(f->flags&MI_ZF))goto mi_004350e9;
    /* 004350c1 */
    f->r[1]=MI_U32((0x00000008u+f->r[4]));
    /* 004350c5 */
    f->r[2]=MI_U32((0x0070e44cu));
    /* 004350cb */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004350cd */
    f->r[2]=MI_U32((0x00000004u+f->r[4]));
    /* 004350d1 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004350d2 */
    f->r[1]=MI_U32((0x0070e45cu));
    /* 004350d8 */
    f->r[2]=(0x00000001u+f->r[1]+f->r[2]*1u);
    /* 004350dc */
    f->r[1]=0x00f55bb0u;
    /* 004350e1 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004350e2 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004350e3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004350e8u;f->boundary(0x004350e3u,0x0053f250u,f);
    /* 004350e8 */
    f->r[4]+=4;return;
mi_004350e9:;
    /* 004350e9 */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 004350ec */
    if(!(f->flags&MI_ZF))goto mi_00435116;
    /* 004350ee */
    f->r[1]=MI_U32((0x00000008u+f->r[4]));
    /* 004350f2 */
    f->r[2]=MI_U32((0x0070e448u));
    /* 004350f8 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004350fa */
    f->r[2]=MI_U32((0x00000004u+f->r[4]));
    /* 004350fe */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004350ff */
    f->r[1]=MI_U32((0x0070e458u));
    /* 00435105 */
    f->r[2]=(0x00000002u+f->r[1]+f->r[2]*1u);
    /* 00435109 */
    f->r[1]=0x00f55bb0u;
    /* 0043510e */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043510f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435110 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00435115u;f->boundary(0x00435110u,0x0053f250u,f);
    /* 00435115 */
    f->r[4]+=4;return;
mi_00435116:;
    /* 00435116 */
    a=f->r[1];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 00435119 */
    if(!(f->flags&MI_ZF))goto mi_00435143;
    /* 0043511b */
    f->r[1]=MI_U32((0x00000008u+f->r[4]));
    /* 0043511f */
    f->r[2]=MI_U32((0x0070e444u));
    /* 00435125 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00435127 */
    f->r[2]=MI_U32((0x0070e454u));
    /* 0043512d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043512e */
    f->r[1]=MI_U32((0x00000008u+f->r[4]));
    /* 00435132 */
    f->r[2]=(0x00000003u+f->r[2]+f->r[1]*1u);
    /* 00435136 */
    f->r[1]=0x00f55bb0u;
    /* 0043513b */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043513c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043513d */
    f->r[4]-=4;MI_U32(f->r[4])=0x00435142u;f->boundary(0x0043513du,0x0053f250u,f);
    /* 00435142 */
    f->r[4]+=4;return;
mi_00435143:;
    /* 00435143 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 00435146 */
    if(!(f->flags&MI_ZF))goto mi_00435170;
    /* 00435148 */
    f->r[1]=MI_U32((0x00000008u+f->r[4]));
    /* 0043514c */
    f->r[2]=MI_U32((0x0070e440u));
    /* 00435152 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00435154 */
    f->r[2]=MI_U32((0x0070e450u));
    /* 0043515a */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043515b */
    f->r[1]=MI_U32((0x00000008u+f->r[4]));
    /* 0043515f */
    f->r[2]=(0x00000004u+f->r[2]+f->r[1]*1u);
    /* 00435163 */
    f->r[1]=0x00f55bb0u;
    /* 00435168 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435169 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043516a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043516fu;f->boundary(0x0043516au,0x0053f250u,f);
    /* 0043516f */
    f->r[4]+=4;return;
mi_00435170:;
    /* 00435170 */
    a=f->r[1];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 00435173 */
    if(!(f->flags&MI_ZF))goto mi_00435284;
    /* 00435179 */
    f->r[1]=MI_U32((0x00000008u+f->r[4]));
    /* 0043517d */
    f->r[2]=MI_U32((0x0070e440u));
    /* 00435183 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00435185 */
    f->r[2]=MI_U32((0x0070e44cu));
    /* 0043518b */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0043518d */
    f->r[2]=MI_U32((0x0070e450u));
    /* 00435193 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435194 */
    f->r[1]=MI_U32((0x0070e45cu));
    /* 0043519a */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0043519c */
    f->r[1]=MI_U32((0x00000008u+f->r[4]));
    /* 004351a0 */
    f->r[2]=(0x00000005u+f->r[2]+f->r[1]*1u);
    /* 004351a4 */
    f->r[1]=0x00f55bb0u;
    /* 004351a9 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004351aa */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004351ab */
    f->r[4]-=4;MI_U32(f->r[4])=0x004351b0u;f->boundary(0x004351abu,0x0053f250u,f);
    /* 004351b0 */
    f->r[4]+=4;return;
mi_004351b1:;
    /* 004351b1 */
    a=f->r[0];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 004351b4 */
    if(!mi_signed_less(f->flags))goto mi_004351c1;
    /* 004351b6 */
    f->r[0]=MI_U32((0x00f56ca4u));
    /* 004351bb */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004351bc */
    goto mi_00435247;
mi_004351c1:;
    /* 004351c1 */
    a=f->r[0];b=0x00000014u;t=mi_alu(f,a,b,32,1);
    /* 004351c4 */
    if(!mi_signed_less(f->flags))goto mi_004351d1;
    /* 004351c6 */
    f->r[1]=MI_U32((0x00f56ca4u));
    /* 004351cc */
    f->r[0]=(0x00000002u+f->r[1]);
    /* 004351cf */
    goto mi_00435247;
mi_004351d1:;
    /* 004351d1 */
    a=f->r[0];b=0x0000001eu;t=mi_alu(f,a,b,32,1);
    /* 004351d4 */
    if(!mi_signed_less(f->flags))goto mi_004351e1;
    /* 004351d6 */
    f->r[2]=MI_U32((0x00f56ca4u));
    /* 004351dc */
    f->r[0]=(0x00000003u+f->r[2]);
    /* 004351df */
    goto mi_00435247;
mi_004351e1:;
    /* 004351e1 */
    a=f->r[0];b=0x00000028u;t=mi_alu(f,a,b,32,1);
    /* 004351e4 */
    if(!mi_signed_less(f->flags))goto mi_004351f0;
    /* 004351e6 */
    f->r[0]=MI_U32((0x00f56ca4u));
    /* 004351eb */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004351ee */
    goto mi_00435247;
mi_004351f0:;
    /* 004351f0 */
    a=f->r[0];b=0x00000032u;t=mi_alu(f,a,b,32,1);
    /* 004351f3 */
    if(!mi_signed_less(f->flags))goto mi_00435200;
    /* 004351f5 */
    f->r[1]=MI_U32((0x00f56ca4u));
    /* 004351fb */
    f->r[0]=(0x00000005u+f->r[1]);
    /* 004351fe */
    goto mi_00435247;
mi_00435200:;
    /* 00435200 */
    a=f->r[0];b=0x0000003cu;t=mi_alu(f,a,b,32,1);
    /* 00435203 */
    if(!mi_signed_less(f->flags))goto mi_00435210;
    /* 00435205 */
    f->r[2]=MI_U32((0x00f56ca4u));
    /* 0043520b */
    f->r[0]=(0x00000006u+f->r[2]);
    /* 0043520e */
    goto mi_00435247;
mi_00435210:;
    /* 00435210 */
    a=f->r[0];b=0x00000046u;t=mi_alu(f,a,b,32,1);
    /* 00435213 */
    if(!mi_signed_less(f->flags))goto mi_0043521f;
    /* 00435215 */
    f->r[0]=MI_U32((0x00f56ca4u));
    /* 0043521a */
    a=f->r[0];b=0x00000007u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0043521d */
    goto mi_00435247;
mi_0043521f:;
    /* 0043521f */
    a=f->r[0];b=0x00000050u;t=mi_alu(f,a,b,32,1);
    /* 00435222 */
    if(!mi_signed_less(f->flags))goto mi_0043522f;
    /* 00435224 */
    f->r[1]=MI_U32((0x00f56ca4u));
    /* 0043522a */
    f->r[0]=(0x00000008u+f->r[1]);
    /* 0043522d */
    goto mi_00435247;
mi_0043522f:;
    /* 0043522f */
    a=f->r[0];b=0x0000005au;t=mi_alu(f,a,b,32,1);
    /* 00435232 */
    if(!mi_signed_less(f->flags))goto mi_0043523f;
    /* 00435234 */
    f->r[2]=MI_U32((0x00f56ca4u));
    /* 0043523a */
    f->r[0]=(0x00000009u+f->r[2]);
    /* 0043523d */
    goto mi_00435247;
mi_0043523f:;
    /* 0043523f */
    f->r[0]=MI_U32((0x00f56ca4u));
    /* 00435244 */
    a=f->r[0];b=0x00000009u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
mi_00435247:;
    /* 00435247 */
    f->r[1]=MI_U32((0x00000008u+f->r[4]));
    /* 0043524b */
    f->r[2]=MI_U32((0x00000004u+f->r[4]));
    /* 0043524f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435250 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435251 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435252 */
    f->r[1]=0x00f55bb0u;
    /* 00435257 */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[0]);
    /* 0043525c */
    f->r[4]-=4;MI_U32(f->r[4])=0x00435261u;f->boundary(0x0043525cu,0x0053f250u,f);
    /* 00435261 */
    f->r[4]+=4;return;
mi_00435262:;
    /* 00435262 */
    f->r[0]=MI_U32((0x00000008u+f->r[4]));
    /* 00435266 */
    f->r[2]=MI_U32((0x00000004u+f->r[4]));
    /* 0043526a */
    f->r[1]=MI_U32((0x00b7482cu+f->r[1]*4u));
    /* 00435271 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435272 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435273 */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[1]);
    /* 00435279 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043527a */
    f->r[1]=0x00f55bb0u;
    /* 0043527f */
    f->r[4]-=4;MI_U32(f->r[4])=0x00435284u;f->boundary(0x0043527fu,0x0053f250u,f);
mi_00435284:;
    /* 00435284 */
    f->r[4]+=4;return;
}


void restore_building_world_00435290(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00435290 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435291 */
    f->r[0]=MI_U32((0x00000008u+f->r[4]));
    /* 00435295 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435296 */
    f->r[6]=f->r[0];
    /* 00435298 */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0043529b */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0043529d */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004352a0 */
    a=MI_U16((0x00799a10u+f->r[6]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 004352a8 */
    if(!(f->flags&MI_ZF))goto mi_0043539d;
    /* 004352ae */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004352af */
    f->r[1]=0x00bca350u;
    /* 004352b4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004352b9u;f->boundary(0x004352b4u,0x00488030u,f);
    /* 004352b9 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004352bb */
    if((f->flags&MI_ZF))goto mi_0043530c;
    /* 004352bd */
    f->r[0]=MI_U32((0x0070e488u));
    /* 004352c2 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004352c4 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00b67d1cu+f->r[0])))&0xffu)<<0);
    /* 004352ca */
    f->r[0]=MI_U32((0x00bca2e0u));
    /* 004352cf */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004352d1 */
    if(!(f->flags&MI_ZF))goto mi_004352de;
    /* 004352d3 */
    a=f->r[1];b=0x00000050u;t=mi_alu(f,a,b,32,1);
    /* 004352d6 */
    if(!(f->flags&MI_ZF))goto mi_0043539d;
    /* 004352dc */
    goto mi_0043530c;
mi_004352de:;
    /* 004352de */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 004352e1 */
    if(!(f->flags&MI_ZF))goto mi_004352ee;
    /* 004352e3 */
    a=f->r[1];b=0x00000040u;t=mi_alu(f,a,b,32,1);
    /* 004352e6 */
    if(!(f->flags&MI_ZF))goto mi_0043539d;
    /* 004352ec */
    goto mi_0043530c;
mi_004352ee:;
    /* 004352ee */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 004352f1 */
    if(!(f->flags&MI_ZF))goto mi_004352fe;
    /* 004352f3 */
    a=f->r[1];b=0x00000042u;t=mi_alu(f,a,b,32,1);
    /* 004352f6 */
    if(!(f->flags&MI_ZF))goto mi_0043539d;
    /* 004352fc */
    goto mi_0043530c;
mi_004352fe:;
    /* 004352fe */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 00435301 */
    if(!(f->flags&MI_ZF))goto mi_0043530c;
    /* 00435303 */
    a=f->r[1];b=0x00000052u;t=mi_alu(f,a,b,32,1);
    /* 00435306 */
    if(!(f->flags&MI_ZF))goto mi_0043539d;
mi_0043530c:;
    /* 0043530c */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00799a20u+f->r[6]));
    /* 00435313 */
    f->r[0]=0x51eb851fu;
    /* 00435318 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435319 */
    a=f->r[0];b=f->r[1];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 0043531b */
    f->r[0]=MI_U32((0x00000018u+f->r[4]));
    /* 0043531f */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 00435322 */
    f->r[1]=f->r[2];
    /* 00435324 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00435328 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e8u+f->r[6]));
    /* 0043532f */
    a=f->r[1];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[1]=t;
    /* 00435332 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00435334 */
    a=f->r[0];b=0x00000041u;t=mi_alu(f,a,b,32,1);
    /* 00435337 */
    f->r[7]=f->r[2];
    /* 00435339 */
    f->r[2]=MI_U32((0x00000014u+f->r[4]));
    /* 0043533d */
    MI_U32((0x00000008u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00435341 */
    if(mi_signed_less(f->flags))goto mi_00435387;
    /* 00435343 */
    a=f->r[0];b=0x00000045u;t=mi_alu(f,a,b,32,1);
    /* 00435346 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00435387;
    /* 00435348 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0043534a */
    f->r[1]=(0x00000010u+f->r[4]);
    /* 0043534e */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00799a82u+f->r[6])))&0xffu)<<0);
    /* 00435354 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435355 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[6]));
    /* 0043535c */
    a=f->r[0];t=a>>1u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|((a>>0u)&1u)|mi_szp(t,32)|((a&0x80000000u)?MI_OF:0);f->r[0]=t;
    /* 0043535e */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00435361 */
    f->r[2]=(0x0000000cu+f->r[4]);
    /* 00435365 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435366 */
    f->r[2]=MI_U32((0x005d1ffcu+f->r[0]));
    /* 0043536c */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0043536e */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[6]));
    /* 00435375 */
    f->r[6]=MI_U32((0x005d1ff8u+f->r[0]));
    /* 0043537b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043537c */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0043537e */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043537f */
    f->r[4]-=4;MI_U32(f->r[4])=0x00435384u;f->boundary(0x0043537fu,0x004f0ba0u,f);
    /* 00435384 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00435387:;
    /* 00435387 */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 0043538b */
    f->r[1]=MI_U32((0x00000008u+f->r[4]));
    /* 0043538f */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435391 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435392 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435393 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435394 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00435399u;f->boundary(0x00435394u,0x00436180u,f);
    /* 00435399 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043539c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
mi_0043539d:;
    /* 0043539d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0043539e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0043539f */
    f->r[4]+=4;return;
}


void restore_building_world_004353a0(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004353a0 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004353a1 */
    f->r[0]=MI_U32((0x00000008u+f->r[4]));
    /* 004353a5 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004353a6 */
    f->r[6]=f->r[0];
    /* 004353a8 */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004353ab */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004353ad */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004353b0 */
    a=MI_U16((0x00799a10u+f->r[6]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 004353b8 */
    if(!(f->flags&MI_ZF))goto mi_004354a9;
    /* 004353be */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004353bf */
    f->r[1]=0x00bca350u;
    /* 004353c4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004353c9u;f->boundary(0x004353c4u,0x00488030u,f);
    /* 004353c9 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004353cb */
    if((f->flags&MI_ZF))goto mi_0043541c;
    /* 004353cd */
    f->r[0]=MI_U32((0x0070e488u));
    /* 004353d2 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004353d4 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00b67d1cu+f->r[0])))&0xffu)<<0);
    /* 004353da */
    f->r[0]=MI_U32((0x00bca2e0u));
    /* 004353df */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004353e1 */
    if(!(f->flags&MI_ZF))goto mi_004353ee;
    /* 004353e3 */
    a=f->r[1];b=0x00000050u;t=mi_alu(f,a,b,32,1);
    /* 004353e6 */
    if(!(f->flags&MI_ZF))goto mi_004354a9;
    /* 004353ec */
    goto mi_0043541c;
mi_004353ee:;
    /* 004353ee */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 004353f1 */
    if(!(f->flags&MI_ZF))goto mi_004353fe;
    /* 004353f3 */
    a=f->r[1];b=0x00000040u;t=mi_alu(f,a,b,32,1);
    /* 004353f6 */
    if(!(f->flags&MI_ZF))goto mi_004354a9;
    /* 004353fc */
    goto mi_0043541c;
mi_004353fe:;
    /* 004353fe */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 00435401 */
    if(!(f->flags&MI_ZF))goto mi_0043540e;
    /* 00435403 */
    a=f->r[1];b=0x00000042u;t=mi_alu(f,a,b,32,1);
    /* 00435406 */
    if(!(f->flags&MI_ZF))goto mi_004354a9;
    /* 0043540c */
    goto mi_0043541c;
mi_0043540e:;
    /* 0043540e */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 00435411 */
    if(!(f->flags&MI_ZF))goto mi_0043541c;
    /* 00435413 */
    a=f->r[1];b=0x00000052u;t=mi_alu(f,a,b,32,1);
    /* 00435416 */
    if(!(f->flags&MI_ZF))goto mi_004354a9;
mi_0043541c:;
    /* 0043541c */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e8u+f->r[6]));
    /* 00435423 */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
    /* 00435427 */
    f->r[2]=MI_U32((0x00000014u+f->r[4]));
    /* 0043542b */
    a=f->r[0];b=0x00000041u;t=mi_alu(f,a,b,32,1);
    /* 0043542e */
    MI_U32((0x00000004u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00435432 */
    MI_U32((0x0000000cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00435436 */
    if(mi_signed_less(f->flags))goto mi_0043547c;
    /* 00435438 */
    a=f->r[0];b=0x00000045u;t=mi_alu(f,a,b,32,1);
    /* 0043543b */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0043547c;
    /* 0043543d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0043543f */
    f->r[1]=(0x0000000cu+f->r[4]);
    /* 00435443 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00799a82u+f->r[6])))&0xffu)<<0);
    /* 00435449 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043544a */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[6]));
    /* 00435451 */
    a=f->r[0];t=a>>1u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|((a>>0u)&1u)|mi_szp(t,32)|((a&0x80000000u)?MI_OF:0);f->r[0]=t;
    /* 00435453 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00435456 */
    f->r[2]=(0x00000008u+f->r[4]);
    /* 0043545a */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043545b */
    f->r[2]=MI_U32((0x005d1ffcu+f->r[0]));
    /* 00435461 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00435463 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[6]));
    /* 0043546a */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043546b */
    f->r[1]=MI_U32((0x005d1ff8u+f->r[0]));
    /* 00435471 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00435473 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435474 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00435479u;f->boundary(0x00435474u,0x004f0ba0u,f);
    /* 00435479 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0043547c:;
    /* 0043547c */
    f->r[0]=MI_U32((0x0000000cu+f->r[4]));
    /* 00435480 */
    f->r[1]=MI_U32((0x00000004u+f->r[4]));
    /* 00435484 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435486 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435487 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435488 */
    f->r[0]=0x51eb851fu;
    /* 0043548d */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00799a1eu+f->r[6]));
    /* 00435494 */
    a=f->r[0];b=f->r[1];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 00435496 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 00435499 */
    f->r[0]=f->r[2];
    /* 0043549b */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 0043549e */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004354a0 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004354a1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004354a6u;f->boundary(0x004354a1u,0x00436180u,f);
    /* 004354a6 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004354a9:;
    /* 004354a9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004354aa */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004354ab */
    f->r[4]+=4;return;
}


void restore_building_world_004354b0(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004354b0 */
    f->r[0]=MI_U32((0x0070e488u));
    /* 004354b5 */
    f->r[1]=MI_U32((0x00b7482cu+f->r[0]*4u));
    /* 004354bc */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[1]);
    /* 004354c2 */
    f->r[1]=MI_U32((0x00000004u+f->r[4]));
    /* 004354c6 */
    f->r[0]=f->r[1];
    /* 004354c8 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004354cb */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004354cd */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004354d0 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x007999ddu+f->r[0])))&0xffu)<<0);
    /* 004354d6 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004354d8 */
    if((f->flags&MI_ZF))goto mi_00435506;
    /* 004354da */
    f->r[2]=MI_U32((0x0000000cu+f->r[4]));
    /* 004354de */
    f->r[1]=MI_U32((0x00000008u+f->r[4]));
    /* 004354e2 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004354e4 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004354e5 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004354e6 */
    f->r[1]=MI_U32((0x00799ad4u+f->r[0]));
    /* 004354ec */
    f->r[0]=0x51eb851fu;
    /* 004354f1 */
    a=f->r[0];b=f->r[1];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 004354f3 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 004354f6 */
    f->r[0]=f->r[2];
    /* 004354f8 */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 004354fb */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004354fd */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004354fe */
    f->r[4]-=4;MI_U32(f->r[4])=0x00435503u;f->boundary(0x004354feu,0x00436180u,f);
    /* 00435503 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00435506:;
    /* 00435506 */
    f->r[4]+=4;return;
}


void restore_building_world_00435510(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00435510 */
    f->r[0]=MI_U32((0x0070e488u));
    /* 00435515 */
    f->r[1]=MI_U32((0x00b7482cu+f->r[0]*4u));
    /* 0043551c */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[1]);
    /* 00435522 */
    f->r[1]=MI_U32((0x00000004u+f->r[4]));
    /* 00435526 */
    f->r[0]=f->r[1];
    /* 00435528 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0043552b */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0043552d */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00435530 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x007999ddu+f->r[0])))&0xffu)<<0);
    /* 00435536 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00435538 */
    if((f->flags&MI_ZF))goto mi_00435566;
    /* 0043553a */
    f->r[2]=MI_U32((0x0000000cu+f->r[4]));
    /* 0043553e */
    f->r[1]=MI_U32((0x00000008u+f->r[4]));
    /* 00435542 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435544 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435545 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435546 */
    f->r[1]=MI_U32((0x00799ac0u+f->r[0]));
    /* 0043554c */
    f->r[0]=0x51eb851fu;
    /* 00435551 */
    a=f->r[0];b=f->r[1];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 00435553 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 00435556 */
    f->r[0]=f->r[2];
    /* 00435558 */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 0043555b */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0043555d */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043555e */
    f->r[4]-=4;MI_U32(f->r[4])=0x00435563u;f->boundary(0x0043555eu,0x00436180u,f);
    /* 00435563 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00435566:;
    /* 00435566 */
    f->r[4]+=4;return;
}


void restore_building_world_00435570(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00435570 */
    f->r[0]=MI_U32((0x0070e488u));
    /* 00435575 */
    f->r[1]=MI_U32((0x00b7482cu+f->r[0]*4u));
    /* 0043557c */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[1]);
    /* 00435582 */
    f->r[1]=MI_U32((0x00000004u+f->r[4]));
    /* 00435586 */
    f->r[0]=f->r[1];
    /* 00435588 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0043558b */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0043558d */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00435590 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x007999ddu+f->r[0])))&0xffu)<<0);
    /* 00435596 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00435598 */
    if((f->flags&MI_ZF))goto mi_004355c6;
    /* 0043559a */
    f->r[2]=MI_U32((0x0000000cu+f->r[4]));
    /* 0043559e */
    f->r[1]=MI_U32((0x00000008u+f->r[4]));
    /* 004355a2 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004355a4 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004355a5 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004355a6 */
    f->r[1]=MI_U32((0x00799abcu+f->r[0]));
    /* 004355ac */
    f->r[0]=0x51eb851fu;
    /* 004355b1 */
    a=f->r[0];b=f->r[1];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 004355b3 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 004355b6 */
    f->r[0]=f->r[2];
    /* 004355b8 */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 004355bb */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004355bd */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004355be */
    f->r[4]-=4;MI_U32(f->r[4])=0x004355c3u;f->boundary(0x004355beu,0x004361a0u,f);
    /* 004355c3 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004355c6:;
    /* 004355c6 */
    f->r[4]+=4;return;
}


void restore_building_world_004355d0(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004355d0 */
    f->r[0]=MI_U32((0x0070e488u));
    /* 004355d5 */
    f->r[1]=MI_U32((0x00b7482cu+f->r[0]*4u));
    /* 004355dc */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[1]);
    /* 004355e2 */
    f->r[1]=MI_U32((0x00000004u+f->r[4]));
    /* 004355e6 */
    f->r[0]=f->r[1];
    /* 004355e8 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004355eb */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004355ed */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004355f0 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x007999ddu+f->r[0])))&0xffu)<<0);
    /* 004355f6 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004355f8 */
    if((f->flags&MI_ZF))goto mi_00435627;
    /* 004355fa */
    f->r[2]=MI_U32((0x0000000cu+f->r[4]));
    /* 004355fe */
    f->r[1]=MI_U32((0x00000008u+f->r[4]));
    /* 00435602 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435604 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435605 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435606 */
    f->r[1]=(uint32_t)(int32_t)(int8_t)MI_U8((0x00799a58u+f->r[0]));
    /* 0043560d */
    f->r[0]=0x66666667u;
    /* 00435612 */
    a=f->r[0];b=f->r[1];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 00435614 */
    a=f->r[2];b=0x00000002u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 00435617 */
    f->r[0]=f->r[2];
    /* 00435619 */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 0043561c */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0043561e */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043561f */
    f->r[4]-=4;MI_U32(f->r[4])=0x00435624u;f->boundary(0x0043561fu,0x004361a0u,f);
    /* 00435624 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00435627:;
    /* 00435627 */
    f->r[4]+=4;return;
}


void restore_building_world_00435630(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00435630 */
    f->r[0]=MI_U32((0x0070e488u));
    /* 00435635 */
    f->r[1]=MI_U32((0x00b7482cu+f->r[0]*4u));
    /* 0043563c */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[1]);
    /* 00435642 */
    f->r[1]=MI_U32((0x00000004u+f->r[4]));
    /* 00435646 */
    f->r[0]=f->r[1];
    /* 00435648 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0043564b */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0043564d */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00435650 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x007999ddu+f->r[0])))&0xffu)<<0);
    /* 00435656 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00435658 */
    if((f->flags&MI_ZF))goto mi_00435694;
    /* 0043565a */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00799a59u+f->r[0])))&0xffu)<<0);
    /* 00435660 */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 00435662 */
    if(!(f->flags&MI_ZF))goto mi_0043566b;
    /* 00435664 */
    f->r[0]=0x00000002u;
    /* 00435669 */
    goto mi_0043567f;
mi_0043566b:;
    /* 0043566b */
    a=(f->r[0]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 0043566d */
    if(!(f->flags&MI_ZF))goto mi_00435676;
    /* 0043566f */
    f->r[0]=0x00000005u;
    /* 00435674 */
    goto mi_0043567f;
mi_00435676:;
    /* 00435676 */
    a=(f->r[0]&0xffu);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 00435678 */
    if(!(f->flags&MI_ZF))goto mi_00435694;
    /* 0043567a */
    f->r[0]=0x00000008u;
mi_0043567f:;
    /* 0043567f */
    f->r[2]=MI_U32((0x0000000cu+f->r[4]));
    /* 00435683 */
    f->r[1]=MI_U32((0x00000008u+f->r[4]));
    /* 00435687 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435689 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043568a */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043568b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043568c */
    f->r[4]-=4;MI_U32(f->r[4])=0x00435691u;f->boundary(0x0043568cu,0x004361a0u,f);
    /* 00435691 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00435694:;
    /* 00435694 */
    f->r[4]+=4;return;
}


void restore_building_world_004356a0(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004356a0 */
    f->r[0]=MI_U32((0x0070e488u));
    /* 004356a5 */
    f->r[1]=MI_U32((0x00b7482cu+f->r[0]*4u));
    /* 004356ac */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[1]);
    /* 004356b2 */
    f->r[1]=MI_U32((0x00000004u+f->r[4]));
    /* 004356b6 */
    f->r[0]=f->r[1];
    /* 004356b8 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004356bb */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004356bd */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004356c0 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x007999ddu+f->r[0])))&0xffu)<<0);
    /* 004356c6 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004356c8 */
    if((f->flags&MI_ZF))goto mi_004356f8;
    /* 004356ca */
    f->r[2]=MI_U32((0x0000000cu+f->r[4]));
    /* 004356ce */
    f->r[1]=MI_U32((0x00000008u+f->r[4]));
    /* 004356d2 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004356d4 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004356d5 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004356d6 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004356d8 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00799a44u+f->r[0])))&0xffu)<<0);
    /* 004356de */
    f->r[0]=0x66666667u;
    /* 004356e3 */
    a=f->r[0];b=f->r[1];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 004356e5 */
    a=f->r[2];b=0x00000002u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 004356e8 */
    f->r[0]=f->r[2];
    /* 004356ea */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 004356ed */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004356ef */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004356f0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004356f5u;f->boundary(0x004356f0u,0x004361a0u,f);
    /* 004356f5 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004356f8:;
    /* 004356f8 */
    f->r[4]+=4;return;
}


void restore_building_world_00435700(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00435700 */
    f->r[0]=MI_U32((0x0070e488u));
    /* 00435705 */
    f->r[1]=MI_U32((0x00b7482cu+f->r[0]*4u));
    /* 0043570c */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[1]);
    /* 00435712 */
    f->r[1]=MI_U32((0x00000004u+f->r[4]));
    /* 00435716 */
    f->r[0]=f->r[1];
    /* 00435718 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0043571b */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0043571d */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00435720 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x007999ddu+f->r[0])))&0xffu)<<0);
    /* 00435726 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00435728 */
    if((f->flags&MI_ZF))goto mi_00435758;
    /* 0043572a */
    f->r[2]=MI_U32((0x0000000cu+f->r[4]));
    /* 0043572e */
    f->r[1]=MI_U32((0x00000008u+f->r[4]));
    /* 00435732 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435734 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435735 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435736 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 00435738 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00799a45u+f->r[0])))&0xffu)<<0);
    /* 0043573e */
    f->r[0]=0x66666667u;
    /* 00435743 */
    a=f->r[0];b=f->r[1];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 00435745 */
    a=f->r[2];b=0x00000002u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 00435748 */
    f->r[0]=f->r[2];
    /* 0043574a */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 0043574d */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0043574f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435750 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00435755u;f->boundary(0x00435750u,0x004361a0u,f);
    /* 00435755 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00435758:;
    /* 00435758 */
    f->r[4]+=4;return;
}


void restore_building_world_00435760(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00435760 */
    f->r[0]=MI_U32((0x0070e488u));
    /* 00435765 */
    f->r[1]=MI_U32((0x00b7482cu+f->r[0]*4u));
    /* 0043576c */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[1]);
    /* 00435772 */
    f->r[1]=MI_U32((0x00000004u+f->r[4]));
    /* 00435776 */
    f->r[0]=f->r[1];
    /* 00435778 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0043577b */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0043577d */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00435780 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x007999ddu+f->r[0])))&0xffu)<<0);
    /* 00435786 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00435788 */
    if((f->flags&MI_ZF))goto mi_004357b8;
    /* 0043578a */
    f->r[2]=MI_U32((0x0000000cu+f->r[4]));
    /* 0043578e */
    f->r[1]=MI_U32((0x00000008u+f->r[4]));
    /* 00435792 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435794 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435795 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435796 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 00435798 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00799a47u+f->r[0])))&0xffu)<<0);
    /* 0043579e */
    f->r[0]=0x66666667u;
    /* 004357a3 */
    a=f->r[0];b=f->r[1];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 004357a5 */
    a=f->r[2];b=0x00000002u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 004357a8 */
    f->r[0]=f->r[2];
    /* 004357aa */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 004357ad */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004357af */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004357b0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004357b5u;f->boundary(0x004357b0u,0x004361a0u,f);
    /* 004357b5 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004357b8:;
    /* 004357b8 */
    f->r[4]+=4;return;
}


void restore_building_world_004357c0(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004357c0 */
    f->r[0]=MI_U32((0x0070e488u));
    /* 004357c5 */
    f->r[1]=MI_U32((0x00b7482cu+f->r[0]*4u));
    /* 004357cc */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[1]);
    /* 004357d2 */
    f->r[1]=MI_U32((0x00000004u+f->r[4]));
    /* 004357d6 */
    f->r[0]=f->r[1];
    /* 004357d8 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004357db */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004357dd */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004357e0 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x007999ddu+f->r[0])))&0xffu)<<0);
    /* 004357e6 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004357e8 */
    if((f->flags&MI_ZF))goto mi_00435818;
    /* 004357ea */
    f->r[2]=MI_U32((0x0000000cu+f->r[4]));
    /* 004357ee */
    f->r[1]=MI_U32((0x00000008u+f->r[4]));
    /* 004357f2 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004357f4 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004357f5 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004357f6 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004357f8 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00799a49u+f->r[0])))&0xffu)<<0);
    /* 004357fe */
    f->r[0]=0x66666667u;
    /* 00435803 */
    a=f->r[0];b=f->r[1];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 00435805 */
    a=f->r[2];b=0x00000002u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 00435808 */
    f->r[0]=f->r[2];
    /* 0043580a */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 0043580d */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0043580f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435810 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00435815u;f->boundary(0x00435810u,0x004361a0u,f);
    /* 00435815 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00435818:;
    /* 00435818 */
    f->r[4]+=4;return;
}


void restore_building_world_00435820(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00435820 */
    f->r[0]=MI_U32((0x0070e488u));
    /* 00435825 */
    f->r[1]=MI_U32((0x00b7482cu+f->r[0]*4u));
    /* 0043582c */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[1]);
    /* 00435832 */
    f->r[1]=MI_U32((0x00000004u+f->r[4]));
    /* 00435836 */
    f->r[0]=f->r[1];
    /* 00435838 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0043583b */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0043583d */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00435840 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x007999ddu+f->r[0])))&0xffu)<<0);
    /* 00435846 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00435848 */
    if((f->flags&MI_ZF))goto mi_00435878;
    /* 0043584a */
    f->r[2]=MI_U32((0x0000000cu+f->r[4]));
    /* 0043584e */
    f->r[1]=MI_U32((0x00000008u+f->r[4]));
    /* 00435852 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435854 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435855 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435856 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 00435858 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00799a5eu+f->r[0])))&0xffu)<<0);
    /* 0043585e */
    f->r[0]=0x66666667u;
    /* 00435863 */
    a=f->r[0];b=f->r[1];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 00435865 */
    a=f->r[2];b=0x00000002u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 00435868 */
    f->r[0]=f->r[2];
    /* 0043586a */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 0043586d */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0043586f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435870 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00435875u;f->boundary(0x00435870u,0x004361a0u,f);
    /* 00435875 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00435878:;
    /* 00435878 */
    f->r[4]+=4;return;
}


static const uint8_t mi_rom_004359b8[]={0x04,0x59,0x43,0x00,0x0f,0x59,0x43,0x00,0x16,0x59,0x43,0x00,0x1d,0x59,0x43,0x00,0x24,0x59,0x43,0x00,0x08,0x59,0x43,0x00,0x32,0x59,0x43,0x00,0x2b,0x59,0x43,0x00,0x55,0x59,0x43,0x00,0x55,0x59,0x43,0x00,0x55,0x59,0x43,0x00,0x55,0x59,0x43,0x00,0x4e,0x59,0x43,0x00,0x55,0x59,0x43,0x00,0x47,0x59,0x43,0x00,0x55,0x59,0x43,0x00,0x39,0x59,0x43,0x00,0x55,0x59,0x43,0x00,0x40,0x59,0x43,0x00};
static __inline uint32_t mi_read_00435880(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x004359b8u&&width<=76u&&(offset=address-0x004359b8u)<=76u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004359b8[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}

void restore_building_world_00435880(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00435880 */
    f->r[0]=mi_read_00435880((0x0070e488u),4u);
    /* 00435885 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435886 */
    f->r[7]=mi_read_00435880((0x00000008u+f->r[4]),4u);
    /* 0043588a */
    f->r[1]=mi_read_00435880((0x00b7482cu+f->r[0]*4u),4u);
    /* 00435891 */
    f->r[0]=f->r[7];
    /* 00435893 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00435896 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00435898 */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[1]);
    /* 0043589e */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004358a1 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read_00435880((0x007999ddu+f->r[0]),1u))&0xffu)<<0);
    /* 004358a7 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004358a9 */
    if((f->flags&MI_ZF))goto mi_004359b3;
    /* 004358af */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read_00435880((0x007999eau+f->r[0]),2u);
    /* 004358b6 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004358b7 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004358b8 */
    f->r[2]=(0x00000000u+f->r[1]+f->r[1]*4u);
    /* 004358bb */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004358bc */
    f->r[1]=(0x00000000u+f->r[1]+f->r[2]*2u);
    /* 004358bf */
    f->r[5]=mi_read_00435880((0x0070cdf4u+f->r[1]*8u),4u);
    /* 004358c6 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 004358c8 */
    if((f->flags&MI_ZF))goto mi_004358d1;
    /* 004358ca */
    f->r[5]=(uint32_t)(int32_t)(int16_t)mi_read_00435880((0x007999f4u+f->r[0]),2u);
mi_004358d1:;
    /* 004358d1 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004358d3 */
    f->r[6]=0x00000001u;
mi_004358d8:;
    /* 004358d8 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004358d9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004358deu;f->boundary(0x004358d9u,0x00443b90u,f);
    /* 004358de */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004358e1 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004358e3 */
    if(!(f->flags&MI_ZF))goto mi_004358f2;
    /* 004358e5 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004358e6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004358ebu;f->boundary(0x004358e6u,0x004439c0u,f);
    /* 004358eb */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004358ee */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004358f0 */
    if((f->flags&MI_ZF))goto mi_0043596c;
mi_004358f2:;
    /* 004358f2 */
    f->r[2]=(0xffffffffu+f->r[6]);
    /* 004358f5 */
    a=f->r[2];b=0x00000012u;t=mi_alu(f,a,b,32,1);
    /* 004358f8 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00435955;
    /* 004358fa */
    f->r[0]=(0xffffffffu+f->r[6]);
    /* 004358fd */
    switch(mi_read_00435880((0x004359b8u+f->r[0]*4u),4u)){case 0x00435904u:goto mi_00435904;case 0x00435908u:goto mi_00435908;case 0x0043590fu:goto mi_0043590f;case 0x00435916u:goto mi_00435916;case 0x0043591du:goto mi_0043591d;case 0x00435924u:goto mi_00435924;case 0x0043592bu:goto mi_0043592b;case 0x00435932u:goto mi_00435932;case 0x00435939u:goto mi_00435939;case 0x00435940u:goto mi_00435940;case 0x00435947u:goto mi_00435947;case 0x0043594eu:goto mi_0043594e;case 0x00435955u:goto mi_00435955;default:abort();}
mi_00435904:;
    /* 00435904 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00435906 */
    goto mi_00435958;
mi_00435908:;
    /* 00435908 */
    f->r[0]=0x00000005u;
    /* 0043590d */
    goto mi_00435958;
mi_0043590f:;
    /* 0043590f */
    f->r[0]=0x00000001u;
    /* 00435914 */
    goto mi_00435958;
mi_00435916:;
    /* 00435916 */
    f->r[0]=0x00000002u;
    /* 0043591b */
    goto mi_00435958;
mi_0043591d:;
    /* 0043591d */
    f->r[0]=0x00000003u;
    /* 00435922 */
    goto mi_00435958;
mi_00435924:;
    /* 00435924 */
    f->r[0]=0x00000004u;
    /* 00435929 */
    goto mi_00435958;
mi_0043592b:;
    /* 0043592b */
    f->r[0]=0x00000007u;
    /* 00435930 */
    goto mi_00435958;
mi_00435932:;
    /* 00435932 */
    f->r[0]=0x00000006u;
    /* 00435937 */
    goto mi_00435958;
mi_00435939:;
    /* 00435939 */
    f->r[0]=0x0000000au;
    /* 0043593e */
    goto mi_00435958;
mi_00435940:;
    /* 00435940 */
    f->r[0]=0x0000000bu;
    /* 00435945 */
    goto mi_00435958;
mi_00435947:;
    /* 00435947 */
    f->r[0]=0x00000008u;
    /* 0043594c */
    goto mi_00435958;
mi_0043594e:;
    /* 0043594e */
    f->r[0]=0x00000009u;
    /* 00435953 */
    goto mi_00435958;
mi_00435955:;
    /* 00435955 */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[0]=t;
mi_00435958:;
    /* 00435958 */
    f->r[1]=f->r[7];
    /* 0043595a */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0043595d */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0043595f */
    f->r[2]=(0x00000000u+f->r[0]+f->r[1]*4u);
    /* 00435962 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read_00435880((0x00799a2cu+f->r[2]*2u),2u);
    /* 0043596a */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[3]=t;
mi_0043596c:;
    /* 0043596c */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0043596d */
    a=f->r[6];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 00435970 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004358d8;
    /* 00435976 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 00435978 */
    if((f->flags&MI_ZF))goto mi_00435988;
    /* 0043597a */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043597b */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043597c */
    f->r[4]-=4;MI_U32(f->r[4])=0x00435981u;f->boundary(0x0043597cu,0x004cde60u,f);
    /* 00435981 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00435984 */
    f->r[1]=f->r[0];
    /* 00435986 */
    goto mi_0043598a;
mi_00435988:;
    /* 00435988 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_0043598a:;
    /* 0043598a */
    f->r[2]=mi_read_00435880((0x0000001cu+f->r[4]),4u);
    /* 0043598e */
    f->r[0]=mi_read_00435880((0x00000018u+f->r[4]),4u);
    /* 00435992 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435994 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435995 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435996 */
    f->r[0]=0x51eb851fu;
    /* 0043599b */
    a=f->r[0];b=f->r[1];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 0043599d */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 004359a0 */
    f->r[1]=f->r[2];
    /* 004359a2 */
    a=f->r[1];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[1]=t;
    /* 004359a5 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004359a7 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004359a8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004359adu;f->boundary(0x004359a8u,0x004361a0u,f);
    /* 004359ad */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004359b0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004359b1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004359b2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
mi_004359b3:;
    /* 004359b3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004359b4 */
    f->r[4]+=4;return;
}


void restore_building_world_00435a10(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00435a10 */
    f->r[0]=MI_U32((0x0070e488u));
    /* 00435a15 */
    f->r[1]=MI_U32((0x00b7482cu+f->r[0]*4u));
    /* 00435a1c */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[1]);
    /* 00435a22 */
    f->r[1]=MI_U32((0x00000004u+f->r[4]));
    /* 00435a26 */
    f->r[0]=f->r[1];
    /* 00435a28 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00435a2b */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00435a2d */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00435a30 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x007999ddu+f->r[0])))&0xffu)<<0);
    /* 00435a36 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00435a38 */
    if((f->flags&MI_ZF))goto mi_00435a68;
    /* 00435a3a */
    f->r[2]=MI_U32((0x0000000cu+f->r[4]));
    /* 00435a3e */
    f->r[1]=MI_U32((0x00000008u+f->r[4]));
    /* 00435a42 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435a44 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435a45 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435a46 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 00435a48 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00799a4fu+f->r[0])))&0xffu)<<0);
    /* 00435a4e */
    f->r[0]=0x66666667u;
    /* 00435a53 */
    a=f->r[0];b=f->r[1];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 00435a55 */
    a=f->r[2];b=0x00000002u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 00435a58 */
    f->r[0]=f->r[2];
    /* 00435a5a */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 00435a5d */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00435a5f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435a60 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00435a65u;f->boundary(0x00435a60u,0x004361a0u,f);
    /* 00435a65 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00435a68:;
    /* 00435a68 */
    f->r[4]+=4;return;
}


void restore_building_world_00435a70(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00435a70 */
    f->r[0]=MI_U32((0x0070e488u));
    /* 00435a75 */
    f->r[1]=MI_U32((0x00b7482cu+f->r[0]*4u));
    /* 00435a7c */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[1]);
    /* 00435a82 */
    f->r[1]=MI_U32((0x00000004u+f->r[4]));
    /* 00435a86 */
    f->r[0]=f->r[1];
    /* 00435a88 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00435a8b */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00435a8d */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00435a90 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x007999ddu+f->r[0])))&0xffu)<<0);
    /* 00435a96 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00435a98 */
    if((f->flags&MI_ZF))goto mi_00435abd;
    /* 00435a9a */
    f->r[0]=(uint32_t)(int32_t)(int8_t)MI_U8((0x00799a5bu+f->r[0]));
    /* 00435aa1 */
    f->r[2]=MI_U32((0x0000000cu+f->r[4]));
    /* 00435aa5 */
    f->r[1]=MI_U32((0x00000008u+f->r[4]));
    /* 00435aa9 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435aab */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435aac */
    f->r[2]=f->r[0];
    /* 00435aae */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435aaf */
    a=f->r[2];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 00435ab2 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00435ab4 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435ab5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00435abau;f->boundary(0x00435ab5u,0x004361a0u,f);
    /* 00435aba */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00435abd:;
    /* 00435abd */
    f->r[4]+=4;return;
}


void restore_building_world_00435ac0(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00435ac0 */
    f->r[0]=MI_U32((0x0070e488u));
    /* 00435ac5 */
    f->r[1]=MI_U32((0x00b7482cu+f->r[0]*4u));
    /* 00435acc */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[1]);
    /* 00435ad2 */
    f->r[1]=MI_U32((0x00000004u+f->r[4]));
    /* 00435ad6 */
    f->r[0]=f->r[1];
    /* 00435ad8 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00435adb */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00435add */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00435ae0 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x007999ddu+f->r[0])))&0xffu)<<0);
    /* 00435ae6 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00435ae8 */
    if((f->flags&MI_ZF))goto mi_00435b18;
    /* 00435aea */
    f->r[2]=MI_U32((0x0000000cu+f->r[4]));
    /* 00435aee */
    f->r[1]=MI_U32((0x00000008u+f->r[4]));
    /* 00435af2 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435af4 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435af5 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435af6 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 00435af8 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00799a4au+f->r[0])))&0xffu)<<0);
    /* 00435afe */
    f->r[0]=0x66666667u;
    /* 00435b03 */
    a=f->r[0];b=f->r[1];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 00435b05 */
    a=f->r[2];b=0x00000002u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 00435b08 */
    f->r[0]=f->r[2];
    /* 00435b0a */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 00435b0d */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00435b0f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435b10 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00435b15u;f->boundary(0x00435b10u,0x004361a0u,f);
    /* 00435b15 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00435b18:;
    /* 00435b18 */
    f->r[4]+=4;return;
}


void restore_building_world_00435b20(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00435b20 */
    f->r[0]=MI_U32((0x0070e488u));
    /* 00435b25 */
    f->r[1]=MI_U32((0x00b7482cu+f->r[0]*4u));
    /* 00435b2c */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[1]);
    /* 00435b32 */
    f->r[1]=MI_U32((0x00000004u+f->r[4]));
    /* 00435b36 */
    f->r[0]=f->r[1];
    /* 00435b38 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00435b3b */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00435b3d */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00435b40 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x007999ddu+f->r[0])))&0xffu)<<0);
    /* 00435b46 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00435b48 */
    if((f->flags&MI_ZF))goto mi_00435b78;
    /* 00435b4a */
    f->r[2]=MI_U32((0x0000000cu+f->r[4]));
    /* 00435b4e */
    f->r[1]=MI_U32((0x00000008u+f->r[4]));
    /* 00435b52 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435b54 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435b55 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435b56 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 00435b58 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00799a4bu+f->r[0])))&0xffu)<<0);
    /* 00435b5e */
    f->r[0]=0x66666667u;
    /* 00435b63 */
    a=f->r[0];b=f->r[1];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 00435b65 */
    a=f->r[2];b=0x00000002u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 00435b68 */
    f->r[0]=f->r[2];
    /* 00435b6a */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 00435b6d */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00435b6f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435b70 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00435b75u;f->boundary(0x00435b70u,0x004361a0u,f);
    /* 00435b75 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00435b78:;
    /* 00435b78 */
    f->r[4]+=4;return;
}


void restore_building_world_00435b80(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00435b80 */
    f->r[0]=MI_U32((0x0070e488u));
    /* 00435b85 */
    f->r[1]=MI_U32((0x00b7482cu+f->r[0]*4u));
    /* 00435b8c */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[1]);
    /* 00435b92 */
    f->r[1]=MI_U32((0x00000004u+f->r[4]));
    /* 00435b96 */
    f->r[0]=f->r[1];
    /* 00435b98 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00435b9b */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00435b9d */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00435ba0 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x007999ddu+f->r[0])))&0xffu)<<0);
    /* 00435ba6 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00435ba8 */
    if((f->flags&MI_ZF))goto mi_00435bd8;
    /* 00435baa */
    f->r[2]=MI_U32((0x0000000cu+f->r[4]));
    /* 00435bae */
    f->r[1]=MI_U32((0x00000008u+f->r[4]));
    /* 00435bb2 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435bb4 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435bb5 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435bb6 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 00435bb8 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00799a4du+f->r[0])))&0xffu)<<0);
    /* 00435bbe */
    f->r[0]=0x66666667u;
    /* 00435bc3 */
    a=f->r[0];b=f->r[1];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 00435bc5 */
    a=f->r[2];b=0x00000002u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 00435bc8 */
    f->r[0]=f->r[2];
    /* 00435bca */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 00435bcd */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00435bcf */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435bd0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00435bd5u;f->boundary(0x00435bd0u,0x004361a0u,f);
    /* 00435bd5 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00435bd8:;
    /* 00435bd8 */
    f->r[4]+=4;return;
}


void restore_building_world_00435be0(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00435be0 */
    f->r[0]=MI_U32((0x0070e488u));
    /* 00435be5 */
    f->r[1]=MI_U32((0x00b7482cu+f->r[0]*4u));
    /* 00435bec */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[1]);
    /* 00435bf2 */
    f->r[1]=MI_U32((0x00000004u+f->r[4]));
    /* 00435bf6 */
    f->r[0]=f->r[1];
    /* 00435bf8 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00435bfb */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00435bfd */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00435c00 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x007999ddu+f->r[0])))&0xffu)<<0);
    /* 00435c06 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00435c08 */
    if((f->flags&MI_ZF))goto mi_00435c38;
    /* 00435c0a */
    f->r[2]=MI_U32((0x0000000cu+f->r[4]));
    /* 00435c0e */
    f->r[1]=MI_U32((0x00000008u+f->r[4]));
    /* 00435c12 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435c14 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435c15 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435c16 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 00435c18 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00799a46u+f->r[0])))&0xffu)<<0);
    /* 00435c1e */
    f->r[0]=0x66666667u;
    /* 00435c23 */
    a=f->r[0];b=f->r[1];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 00435c25 */
    a=f->r[2];b=0x00000002u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 00435c28 */
    f->r[0]=f->r[2];
    /* 00435c2a */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 00435c2d */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00435c2f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435c30 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00435c35u;f->boundary(0x00435c30u,0x004361a0u,f);
    /* 00435c35 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00435c38:;
    /* 00435c38 */
    f->r[4]+=4;return;
}


void restore_building_world_00435c40(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00435c40 */
    f->r[0]=MI_U32((0x0070e488u));
    /* 00435c45 */
    f->r[1]=MI_U32((0x00b7482cu+f->r[0]*4u));
    /* 00435c4c */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[1]);
    /* 00435c52 */
    f->r[1]=MI_U32((0x00000004u+f->r[4]));
    /* 00435c56 */
    f->r[0]=f->r[1];
    /* 00435c58 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00435c5b */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00435c5d */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00435c60 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x007999ddu+f->r[0])))&0xffu)<<0);
    /* 00435c66 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00435c68 */
    if((f->flags&MI_ZF))goto mi_00435c98;
    /* 00435c6a */
    f->r[2]=MI_U32((0x0000000cu+f->r[4]));
    /* 00435c6e */
    f->r[1]=MI_U32((0x00000008u+f->r[4]));
    /* 00435c72 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435c74 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435c75 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435c76 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 00435c78 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00799a4eu+f->r[0])))&0xffu)<<0);
    /* 00435c7e */
    f->r[0]=0x66666667u;
    /* 00435c83 */
    a=f->r[0];b=f->r[1];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 00435c85 */
    a=f->r[2];b=0x00000002u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 00435c88 */
    f->r[0]=f->r[2];
    /* 00435c8a */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 00435c8d */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00435c8f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435c90 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00435c95u;f->boundary(0x00435c90u,0x004361a0u,f);
    /* 00435c95 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00435c98:;
    /* 00435c98 */
    f->r[4]+=4;return;
}


void restore_building_world_00435ca0(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00435ca0 */
    f->r[0]=MI_U32((0x0070e488u));
    /* 00435ca5 */
    f->r[1]=MI_U32((0x00b7482cu+f->r[0]*4u));
    /* 00435cac */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[1]);
    /* 00435cb2 */
    f->r[1]=MI_U32((0x00000004u+f->r[4]));
    /* 00435cb6 */
    f->r[0]=f->r[1];
    /* 00435cb8 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00435cbb */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00435cbd */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00435cc0 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x007999ddu+f->r[0])))&0xffu)<<0);
    /* 00435cc6 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00435cc8 */
    if((f->flags&MI_ZF))goto mi_00435cf8;
    /* 00435cca */
    f->r[2]=MI_U32((0x0000000cu+f->r[4]));
    /* 00435cce */
    f->r[1]=MI_U32((0x00000008u+f->r[4]));
    /* 00435cd2 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435cd4 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435cd5 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435cd6 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 00435cd8 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00799a50u+f->r[0])))&0xffu)<<0);
    /* 00435cde */
    f->r[0]=0x66666667u;
    /* 00435ce3 */
    a=f->r[0];b=f->r[1];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 00435ce5 */
    a=f->r[2];b=0x00000002u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 00435ce8 */
    f->r[0]=f->r[2];
    /* 00435cea */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 00435ced */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00435cef */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435cf0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00435cf5u;f->boundary(0x00435cf0u,0x004361a0u,f);
    /* 00435cf5 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00435cf8:;
    /* 00435cf8 */
    f->r[4]+=4;return;
}


void restore_building_world_00435d00(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00435d00 */
    f->r[0]=MI_U32((0x0070e488u));
    /* 00435d05 */
    f->r[1]=MI_U32((0x00b7482cu+f->r[0]*4u));
    /* 00435d0c */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[1]);
    /* 00435d12 */
    f->r[1]=MI_U32((0x00000004u+f->r[4]));
    /* 00435d16 */
    f->r[0]=f->r[1];
    /* 00435d18 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00435d1b */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00435d1d */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00435d20 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x007999ddu+f->r[0])))&0xffu)<<0);
    /* 00435d26 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00435d28 */
    if((f->flags&MI_ZF))goto mi_00435d58;
    /* 00435d2a */
    f->r[2]=MI_U32((0x0000000cu+f->r[4]));
    /* 00435d2e */
    f->r[1]=MI_U32((0x00000008u+f->r[4]));
    /* 00435d32 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435d34 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435d35 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435d36 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 00435d38 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00799a48u+f->r[0])))&0xffu)<<0);
    /* 00435d3e */
    f->r[0]=0x66666667u;
    /* 00435d43 */
    a=f->r[0];b=f->r[1];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 00435d45 */
    a=f->r[2];b=0x00000002u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 00435d48 */
    f->r[0]=f->r[2];
    /* 00435d4a */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 00435d4d */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00435d4f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435d50 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00435d55u;f->boundary(0x00435d50u,0x004361a0u,f);
    /* 00435d55 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00435d58:;
    /* 00435d58 */
    f->r[4]+=4;return;
}


void restore_building_world_00435d60(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00435d60 */
    f->r[0]=MI_U32((0x0070e488u));
    /* 00435d65 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435d66 */
    f->r[3]=MI_U32((0x00000008u+f->r[4]));
    /* 00435d6a */
    f->r[1]=MI_U32((0x00b7482cu+f->r[0]*4u));
    /* 00435d71 */
    f->r[0]=f->r[3];
    /* 00435d73 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00435d76 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00435d78 */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[1]);
    /* 00435d7e */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00435d81 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x007999ddu+f->r[0])))&0xffu)<<0);
    /* 00435d87 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00435d89 */
    if((f->flags&MI_ZF))goto mi_00435dd3;
    /* 00435d8b */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 00435d8d */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435d8e */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00799a26u+f->r[0])))&0xffu)<<0);
    /* 00435d94 */
    f->r[6]=MI_U32((0x00000014u+f->r[4]));
    /* 00435d98 */
    f->r[0]=f->r[2];
    /* 00435d9a */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435d9b */
    f->r[7]=MI_U32((0x00000014u+f->r[4]));
    /* 00435d9f */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435da1 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 00435da4 */
    f->r[0]=0x51eb851fu;
    /* 00435da9 */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00435dac */
    a=f->r[0];b=f->r[1];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 00435dae */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 00435db1 */
    f->r[0]=f->r[2];
    /* 00435db3 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435db4 */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 00435db7 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00435db9 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435dba */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435dbb */
    f->r[4]-=4;MI_U32(f->r[4])=0x00435dc0u;f->boundary(0x00435dbbu,0x004361a0u,f);
    /* 00435dc0 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435dc1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00435dc6u;f->boundary(0x00435dc1u,0x0046f1c0u,f);
    /* 00435dc6 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435dc7 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435dc8 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435dc9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00435dceu;f->boundary(0x00435dc9u,0x00435de0u,f);
    /* 00435dce */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00435dd1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00435dd2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
mi_00435dd3:;
    /* 00435dd3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00435dd4 */
    f->r[4]+=4;return;
}


void restore_building_world_00435de0(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00435de0 */
    f->r[1]=MI_U32((0x00000004u+f->r[4]));
    /* 00435de4 */
    a=f->r[1];b=0x000000c8u;t=mi_alu(f,a,b,32,1);
    /* 00435dea */
    if(mi_signed_less(f->flags))goto mi_00435df1;
    /* 00435dec */
    f->r[1]=0x000000c8u;
mi_00435df1:;
    /* 00435df1 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00435df3 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00435df5 */
    t=((f->flags&MI_ZF)||mi_signed_less(f->flags))?1u:0u;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00435df8 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00435df9 */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 00435dfc */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00435dfe */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00435e02;
    /* 00435e00 */
    f->r[1]=f->r[0];
mi_00435e02:;
    /* 00435e02 */
    f->r[0]=0x55555556u;
    /* 00435e07 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435e08 */
    a=f->r[0];b=f->r[1];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 00435e0a */
    f->r[0]=f->r[2];
    /* 00435e0c */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 00435e0f */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00435e11 */
    f->r[6]=f->r[2];
    /* 00435e13 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 00435e15 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00435e63;
    /* 00435e17 */
    f->r[1]=MI_U32((0x0000000cu+f->r[4]));
    /* 00435e1b */
    f->r[2]=MI_U32((0x00000010u+f->r[4]));
    /* 00435e1f */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435e20 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435e21 */
    f->r[3]=(0x0000000fu+f->r[1]);
    /* 00435e24 */
    f->r[7]=(0x0000000au+f->r[2]);
mi_00435e27:;
    /* 00435e27 */
    a=f->r[6];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 00435e2a */
    if(mi_signed_less(f->flags))goto mi_00435e33;
    /* 00435e2c */
    f->r[0]=0x00000003u;
    /* 00435e31 */
    goto mi_00435e42;
mi_00435e33:;
    /* 00435e33 */
    f->r[0]=f->r[6];
    /* 00435e35 */
    a=f->r[0];b=0x80000003u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 00435e3a */
    if(!(f->flags&MI_SF))goto mi_00435e41;
    /* 00435e3c */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00435e3d */
    a=f->r[0];b=0xfffffffcu;t=mi_alu(f,a,b,32,4);f->r[0]=t;
    /* 00435e40 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
mi_00435e41:;
    /* 00435e41 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
mi_00435e42:;
    /* 00435e42 */
    f->r[1]=MI_U32((0x00f56a34u));
    /* 00435e48 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435e49 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00435e4b */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435e4c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00435e4d */
    f->r[1]=0x00f55bb0u;
    /* 00435e52 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00435e57u;f->boundary(0x00435e52u,0x0053e590u,f);
    /* 00435e57 */
    a=f->r[6];b=0x00000004u;t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 00435e5a */
    a=f->r[7];b=0x0000000cu;t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 00435e5d */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 00435e5f */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00435e27;
    /* 00435e61 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00435e62 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
mi_00435e63:;
    /* 00435e63 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00435e64 */
    f->r[4]+=4;return;
}


void restore_building_world_004360a0(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004360a0 */
    f->r[0]=MI_U32((0x0070e488u));
    /* 004360a5 */
    f->r[1]=MI_U32((0x00b7482cu+f->r[0]*4u));
    /* 004360ac */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[1]);
    /* 004360b2 */
    f->r[1]=MI_U32((0x00000004u+f->r[4]));
    /* 004360b6 */
    f->r[0]=f->r[1];
    /* 004360b8 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004360bb */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004360bd */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004360c0 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x007999ddu+f->r[0])))&0xffu)<<0);
    /* 004360c6 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004360c8 */
    if((f->flags&MI_ZF))goto mi_00436139;
    /* 004360ca */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00799a5fu+f->r[0])))&0xffu)<<0);
    /* 004360d0 */
    a=(f->r[1]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004360d3 */
    if(!(f->flags&MI_ZF))goto mi_00436105;
    /* 004360d5 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004360d7 */
    f->r[2]=MI_U32((0x0000000cu+f->r[4]));
    /* 004360db */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x007999dbu+f->r[0])))&0xffu)<<0);
    /* 004360e1 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004360e2 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004360e3 */
    f->r[2]=MI_U32((0x0000000cu+f->r[4]));
    /* 004360e7 */
    a=f->r[1];b=MI_U32((0x0070e45cu));t=mi_imul(f,a,b);f->r[1]=t;
    /* 004360ee */
    f->r[0]=(0x00000012u+f->r[1]+f->r[2]*1u);
    /* 004360f2 */
    f->r[1]=MI_U32((0x00f56724u));
    /* 004360f8 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004360f9 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004360fa */
    f->r[1]=0x00f55bb0u;
    /* 004360ff */
    f->r[4]-=4;MI_U32(f->r[4])=0x00436104u;f->boundary(0x004360ffu,0x0053e590u,f);
    /* 00436104 */
    f->r[4]+=4;return;
mi_00436105:;
    /* 00436105 */
    a=(f->r[1]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 00436108 */
    if(!(f->flags&MI_ZF))goto mi_00436139;
    /* 0043610a */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0043610c */
    f->r[2]=MI_U32((0x0000000cu+f->r[4]));
    /* 00436110 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x007999dbu+f->r[0])))&0xffu)<<0);
    /* 00436116 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436117 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00436118 */
    f->r[2]=MI_U32((0x0000000cu+f->r[4]));
    /* 0043611c */
    a=f->r[1];b=MI_U32((0x0070e45cu));t=mi_imul(f,a,b);f->r[1]=t;
    /* 00436123 */
    f->r[0]=(0x00000012u+f->r[1]+f->r[2]*1u);
    /* 00436127 */
    f->r[1]=MI_U32((0x00f572e4u));
    /* 0043612d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043612e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043612f */
    f->r[1]=0x00f55bb0u;
    /* 00436134 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00436139u;f->boundary(0x00436134u,0x0053e590u,f);
mi_00436139:;
    /* 00436139 */
    f->r[4]+=4;return;
}


void restore_building_world_00436140(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00436140 */
    f->r[1]=MI_U32((0x00000004u+f->r[4]));
    /* 00436144 */
    f->r[0]=f->r[1];
    /* 00436146 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00436149 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0043614b */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0043614e */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x007999ddu+f->r[0])))&0xffu)<<0);
    /* 00436154 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00436156 */
    if(!(f->flags&MI_ZF))goto mi_0043617f;
    /* 00436158 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999f0u+f->r[0]));
    /* 0043615f */
    a=0x0000012cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436164 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436165 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043616au;f->boundary(0x00436165u,0x004cde60u,f);
    /* 0043616a */
    f->r[1]=MI_U32((0x00000014u+f->r[4]));
    /* 0043616e */
    f->r[2]=MI_U32((0x00000010u+f->r[4]));
    /* 00436172 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436174 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436175 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436176 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436177 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043617cu;f->boundary(0x00436177u,0x004361a0u,f);
    /* 0043617c */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0043617f:;
    /* 0043617f */
    f->r[4]+=4;return;
}


void restore_building_world_0046f1c0(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0046f1c0 */
    f->r[2]=MI_U32((0x00000004u+f->r[4]));
    /* 0046f1c4 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0046f1c6 */
    f->r[1]=f->r[2];
    /* 0046f1c8 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0046f1cb */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046f1cd */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0046f1d0 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x007999ddu+f->r[1])))&0xffu)<<0);
    /* 0046f1d6 */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0046f1d8 */
    if((f->flags&MI_ZF))goto mi_0046f1f9;
    /* 0046f1da */
    f->r[1]=MI_U32((0x00799a64u+f->r[1]));
    /* 0046f1e0 */
    f->r[0]=0x68db8badu;
    /* 0046f1e5 */
    a=f->r[1];b=0x00001388u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046f1eb */
    a=f->r[0];b=f->r[1];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 0046f1ed */
    a=f->r[2];b=0x0000000cu;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 0046f1f0 */
    f->r[0]=f->r[2];
    /* 0046f1f2 */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 0046f1f5 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0046f1f7 */
    f->r[0]=f->r[2];
mi_0046f1f9:;
    /* 0046f1f9 */
    f->r[4]+=4;return;
}
