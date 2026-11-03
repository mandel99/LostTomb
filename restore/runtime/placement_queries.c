#include "placement_queries.h"
#include "monument_images.h"
#include "monument_entrance.h"
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







/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */

#include <stdlib.h>







/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */






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






/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */

#include <stdlib.h>







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
 case 0x401800u:f->r[0]=restore_monument_image_query(target,f->r[1],MI_U32(sp+4u),MI_U32(sp+8u),f);f->r[4]=sp+12u;break;
 case 0x40de80u:f->r[0]=restore_monument_image_query(target,f->r[1],MI_U32(sp+4u),MI_U32(sp+8u),f);f->r[4]=sp+12u;break;
 case 0x417310u:f->r[0]=restore_monument_image_query(target,f->r[1],MI_U32(sp+4u),MI_U32(sp+8u),f);f->r[4]=sp+12u;break;
 case 0x545760u:restore_placement_query_C_00545760(f);break;
 case 0x558c00u:restore_placement_query_C_00558c00(f);break;
 case 0x555ea0u:restore_placement_query_C_00555ea0(f);break;
 case 0x54f490u:restore_placement_query_C_0054f490(f);break;
 case 0x4e6a70u:f->r[0]=restore_monument_image_query(target,f->r[1],MI_U32(sp+4u),MI_U32(sp+8u),f);f->r[4]=sp+12u;break;
 case 0x401af0u:restore_placement_query_C_00401af0(f);break;
 case 0x40eb90u:f->r[0]=restore_monument_entrance_query(target,f->r[1],MI_U32(sp+4u),MI_U32(sp+8u),MI_U32(sp+12u));f->r[4]=sp+16u;break;
 case 0x417490u:f->r[0]=restore_monument_entrance_query(target,f->r[1],MI_U32(sp+4u),MI_U32(sp+8u),MI_U32(sp+12u));f->r[4]=sp+16u;break;
 case 0x545a30u:restore_placement_query_C_00545a30(f);break;
 case 0x558f00u:restore_placement_query_C_00558f00(f);break;
 case 0x556230u:restore_placement_query_C_00556230(f);break;
 case 0x54f8a0u:restore_placement_query_C_0054f8a0(f);break;
 case 0x4e6ae0u:f->r[0]=restore_monument_entrance_query(target,f->r[1],MI_U32(sp+4u),MI_U32(sp+8u),MI_U32(sp+12u));f->r[4]=sp+16u;break;
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
