/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "sidebar_undo.h"
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
static __inline uint32_t mi_alu(struct restore_sidebar_frame *f,uint32_t a,uint32_t b,uint32_t width,uint32_t operation)
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
static __inline uint32_t mi_shift(struct restore_sidebar_frame *f,uint32_t a,uint32_t count,uint32_t width,uint32_t arithmetic)
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
static __inline uint32_t mi_imul(struct restore_sidebar_frame *f,uint32_t a,uint32_t b)
{
 int64_t product=(int64_t)(int32_t)a*(int32_t)b;uint32_t result=(uint32_t)product;
 f->flags&=~(MI_CF|MI_OF);if(product!=(int64_t)(int32_t)result)f->flags|=MI_CF|MI_OF;
 return result;
}
static __inline int mi_signed_less(uint32_t flags){return ((flags&MI_SF)!=0)^((flags&MI_OF)!=0);}

static const uint8_t mi_rom_00485ce0[]={0xd9,0x5b,0x48,0x00,0x5b,0x5b,0x48,0x00,0xbc,0x5b,0x48,0x00,0x78,0x5b,0x48,0x00,0x1d,0x5c,0x48,0x00,0x71,0x5c,0x48,0x00,0x5d,0x5c,0x48,0x00,0x16,0x5b,0x48,0x00,0xdf,0x5a,0x48,0x00,0x44,0x5c,0x48,0x00,0x87,0x5c,0x48,0x00};
static const uint8_t mi_rom_00485d0c[]={0x00,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x01,0x01,0x01,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x02,0x03,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x04,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x05,0x06,0x0a,0x0a,0x0a,0x07,0x07,0x07,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x08,0x09,0x0a,0x0a,0x06,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x06};
static __inline uint32_t mi_read(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x00485ce0u&&width<=44u&&(offset=address-0x00485ce0u)<=44u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00485ce0[offset+k]<<(k*8u);return result;
 }
 if(address>=0x00485d0cu&&width<=173u&&(offset=address-0x00485d0cu)<=173u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00485d0c[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}


void restore_sidebar_C_00485230(struct restore_sidebar_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00485230 */
    f->r[0]=MI_U32((0x00bc968cu));
    /* 00485235 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 00485238 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485239 */
    f->r[3]=0x00000001u;
    /* 0048523e */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 00485240 */
    if(!(f->flags&MI_ZF))goto mi_0048581e;
    /* 00485246 */
    a=MI_U32((0x00bc9694u));b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 0048524c */
    if(!(f->flags&MI_ZF))goto mi_0048581e;
    /* 00485252 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 00485257 */
    f->r[1]=MI_U32((0x00bc9684u));
    /* 0048525d */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 00485263 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485264 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485265 */
    f->r[6]=MI_U32((0x00ea187cu+f->r[0]));
    /* 0048526b */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 0048526d */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0048526f */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485270 */
    MI_U32((0x00ea187cu+f->r[0]))=(uint32_t)(f->r[6]);
    /* 00485276 */
    f->r[2]=MI_U32((0x00ea44fcu+f->r[0]));
    /* 0048527c */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0048527e */
    MI_U32((0x00bc9694u))=(uint32_t)(f->r[5]);
    /* 00485284 */
    MI_U32((0x00ea44fcu+f->r[0]))=(uint32_t)(f->r[2]);
    /* 0048528a */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 0048528c */
    f->r[6]=0x00bc95f4u;
mi_00485291:;
    /* 00485291 */
    f->r[0]=MI_U32((0x00000000u+f->r[6]));
    /* 00485293 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 00485295 */
    if((f->flags&MI_ZF))goto mi_004852a2;
    /* 00485297 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485298 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485299 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048529a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048529fu;f->boundary(0x0048529au,0x0045afe0u,f);
    /* 0048529f */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004852a2:;
    /* 004852a2 */
    a=f->r[6];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004852a5 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 004852a6 */
    a=f->r[6];b=0x00bc9684u;t=mi_alu(f,a,b,32,1);
    /* 004852ac */
    if(mi_signed_less(f->flags))goto mi_00485291;
    /* 004852ae */
    f->r[0]=MI_U32((0x00bc9690u));
    /* 004852b3 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 004852b5 */
    a=f->r[0];b=0x00000009u;t=mi_alu(f,a,b,32,1);
    /* 004852b8 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 004852bc */
    if(!(f->flags&MI_ZF))goto mi_004853e9;
    /* 004852c2 */
    f->r[0]=MI_U32((0x00bc95f0u));
    /* 004852c7 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004852c9 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004852cb */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0048533c;
    /* 004852cd */
    f->r[5]=0x00ba7888u;
    /* 004852d2 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00ba746cu);
mi_004852da:;
    /* 004852da */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 004852de */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00000000u+f->r[0])))&0xffffu)<<0);
    /* 004852e1 */
    a=(f->r[0]&0xffffu);b=(f->r[0]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 004852e4 */
    if((f->flags&MI_ZF))goto mi_0048531d;
    /* 004852e6 */
    f->r[7]=(uint32_t)(int32_t)(int16_t)(f->r[0]&0xffffu);
    /* 004852e9 */
    f->r[6]=f->r[7];
    /* 004852eb */
    a=0x00000108u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004852f0 */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004852f3 */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004852f5 */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004852f8 */
    f->r[1]=(0x007999d8u+f->r[6]);
    /* 004852fe */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004852ff */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485300 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00485305u;f->boundary(0x00485300u,0x0040f860u,f);
    /* 00485305 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485306 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048530bu;f->boundary(0x00485306u,0x004858e0u,f);
    /* 0048530b */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048530e */
    a=MI_U16((0x007999e8u+f->r[6]));b=0x000000deu;t=mi_alu(f,a,b,16,1);
    /* 00485317 */
    if(!(f->flags&MI_ZF))goto mi_0048531d;
    /* 00485319 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[7]);
mi_0048531d:;
    /* 0048531d */
    f->r[2]=MI_U32((0x00000010u+f->r[4]));
    /* 00485321 */
    f->r[0]=MI_U32((0x00bc95f0u));
    /* 00485326 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 00485327 */
    a=f->r[2];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0048532a */
    a=f->r[5];b=0x00000108u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 00485330 */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00485332 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00485336 */
    if(mi_signed_less(f->flags))goto mi_004852da;
    /* 00485338 */
    f->r[6]=MI_U32((0x00000014u+f->r[4]));
mi_0048533c:;
    /* 0048533c */
    a=0x00032c40u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485341 */
    a=0x00af598cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485346 */
    a=0x009de62cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048534b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00485350u;f->boundary(0x0048534bu,0x0040f860u,f);
    /* 00485350 */
    a=0x0000cb10u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485355 */
    a=0x00ae8e7cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048535a */
    a=0x009d1b1cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048535f */
    f->r[4]-=4;MI_U32(f->r[4])=0x00485364u;f->boundary(0x0048535fu,0x0040f860u,f);
    /* 00485364 */
    a=0x0000cb10u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485369 */
    a=0x00adc36cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048536e */
    a=0x009c500cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485373 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00485378u;f->boundary(0x00485373u,0x0040f860u,f);
    /* 00485378 */
    a=0x00032c40u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048537d */
    a=0x00b7482cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485382 */
    a=0x00a1126cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485387 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048538cu;f->boundary(0x00485387u,0x0040f860u,f);
    /* 0048538c */
    a=0x0000cb10u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485391 */
    a=0x00b285ccu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485396 */
    a=0x009b84fcu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048539b */
    f->r[4]-=4;MI_U32(f->r[4])=0x004853a0u;f->boundary(0x0048539bu,0x0040f860u,f);
    /* 004853a0 */
    a=0x0000cb10u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004853a5 */
    a=0x00b67d1cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004853aa */
    a=0x009ab9ecu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004853af */
    f->r[4]-=4;MI_U32(f->r[4])=0x004853b4u;f->boundary(0x004853afu,0x0040f860u,f);
    /* 004853b4 */
    a=f->r[4];b=0x00000048u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004853b7 */
    a=0x000000afu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004853bc */
    a=0x0000cb10u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004853c1 */
    a=0x00b285ccu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004853c6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004853cbu;f->boundary(0x004853c6u,0x0040f920u,f);
    /* 004853cb */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004853ce */
    f->r[4]-=4;MI_U32(f->r[4])=0x004853d3u;f->boundary(0x004853ceu,0x00485890u,f);
    /* 004853d3 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004853d5 */
    if((f->flags&MI_ZF))goto mi_004857e9;
    /* 004853db */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004853dc */
    f->r[4]-=4;MI_U32(f->r[4])=0x004853e1u;f->boundary(0x004853dcu,0x004e0c80u,f);
    /* 004853e1 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004853e4 */
    goto mi_004857e9;
mi_004853e9:;
    /* 004853e9 */
    a=f->r[0];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 004853ec */
    if((f->flags&MI_ZF))goto mi_004857b8;
    /* 004853f2 */
    a=f->r[0];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 004853f5 */
    if((f->flags&MI_ZF))goto mi_004857b8;
    /* 004853fb */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 004853fe */
    if((f->flags&MI_ZF))goto mi_004857b8;
    /* 00485404 */
    a=f->r[0];b=0x000000a8u;t=mi_alu(f,a,b,32,1);
    /* 00485409 */
    if((f->flags&MI_ZF))goto mi_004857b8;
    /* 0048540f */
    a=f->r[0];b=0x000000a9u;t=mi_alu(f,a,b,32,1);
    /* 00485414 */
    if((f->flags&MI_ZF))goto mi_004857b8;
    /* 0048541a */
    a=f->r[0];b=0x00000052u;t=mi_alu(f,a,b,32,1);
    /* 0048541d */
    if((f->flags&MI_ZF))goto mi_00485793;
    /* 00485423 */
    a=f->r[0];b=0x00000053u;t=mi_alu(f,a,b,32,1);
    /* 00485426 */
    if((f->flags&MI_ZF))goto mi_00485793;
    /* 0048542c */
    a=f->r[0];b=0x00000026u;t=mi_alu(f,a,b,32,1);
    /* 0048542f */
    if((f->flags&MI_ZF))goto mi_00485738;
    /* 00485435 */
    a=f->r[0];b=0x00000027u;t=mi_alu(f,a,b,32,1);
    /* 00485438 */
    if((f->flags&MI_ZF))goto mi_00485738;
    /* 0048543e */
    a=MI_U32((0x00bc95f0u));b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 00485444 */
    if((f->flags&MI_ZF))goto mi_004857e9;
    /* 0048544a */
    a=f->r[0];b=0x00000007u;t=mi_alu(f,a,b,32,1);
    /* 0048544d */
    if(!(f->flags&MI_ZF))goto mi_00485480;
    /* 0048544f */
    a=0x00032c40u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485454 */
    a=0x00af598cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485459 */
    a=0x009de62cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048545e */
    f->r[4]-=4;MI_U32(f->r[4])=0x00485463u;f->boundary(0x0048545eu,0x0040f860u,f);
    /* 00485463 */
    a=0x0000cb10u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485468 */
    a=0x00ae8e7cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048546d */
    a=0x009d1b1cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485472 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00485477u;f->boundary(0x00485472u,0x0040f860u,f);
    /* 00485477 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485478 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048547du;f->boundary(0x00485478u,0x00485830u,f);
    /* 0048547d */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00485480:;
    /* 00485480 */
    f->r[0]=MI_U32((0x00bc95f0u));
    /* 00485485 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 00485489 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 0048548b */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004857e9;
    /* 00485491 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00ba746cu);
    /* 00485499 */
    f->r[3]=0x0000001eu;
    /* 0048549e */
    f->r[5]=0x00000021u;
mi_004854a3:;
    /* 004854a3 */
    f->r[2]=MI_U32((0x00000010u+f->r[4]));
    /* 004854a7 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00000000u+f->r[2])))&0xffffu)<<0);
    /* 004854aa */
    a=(f->r[0]&0xffffu);b=(f->r[0]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 004854ad */
    if((f->flags&MI_ZF))goto mi_00485711;
    /* 004854b3 */
    f->r[6]=(uint32_t)(int32_t)(int16_t)(f->r[0]&0xffffu);
    /* 004854b6 */
    f->r[7]=f->r[6];
    /* 004854b8 */
    a=f->r[7];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 004854bb */
    a=f->r[7];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004854bd */
    a=f->r[7];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 004854c0 */
    a=MI_U16((0x007999e8u+f->r[7]));b=0x000000b7u;t=mi_alu(f,a,b,16,1);
    /* 004854c9 */
    if(!(f->flags&MI_ZF))goto mi_004854de;
    /* 004854cb */
    a=MI_U16((0x007999eau+f->r[7]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 004854d3 */
    if(!(f->flags&MI_ZF))goto mi_004854de;
    /* 004854d5 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004854d6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004854dbu;f->boundary(0x004854d6u,0x004f4930u,f);
    /* 004854db */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004854de:;
    /* 004854de */
    a=MI_U16((0x007999e8u+f->r[7]));b=0x00000088u;t=mi_alu(f,a,b,16,1);
    /* 004854e7 */
    if(!(f->flags&MI_ZF))goto mi_0048550e;
    /* 004854e9 */
    f->r[0]=MI_U32((0x00799a9cu+f->r[7]));
    /* 004854ef */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004854f1 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00485505;
    /* 004854f3 */
    f->r[1]=f->r[0];
    /* 004854f5 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004854f8 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004854fa */
    MI_U32((0x00799a9cu+f->r[1]*8u))=(uint32_t)(0xffffffffu);
mi_00485505:;
    /* 00485505 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485506 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048550bu;f->boundary(0x00485506u,0x004a36b0u,f);
    /* 0048550b */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0048550e:;
    /* 0048550e */
    a=MI_U16((0x007999e8u+f->r[7]));b=0x0000001fu;t=mi_alu(f,a,b,16,1);
    /* 00485516 */
    if(!(f->flags&MI_ZF))goto mi_00485523;
    /* 00485518 */
    a=0x0000001fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048551a */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048551b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00485520u;f->boundary(0x0048551bu,0x0043fd00u,f);
    /* 00485520 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00485523:;
    /* 00485523 */
    a=MI_U16((0x007999e8u+f->r[7]));b=(f->r[3]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 0048552a */
    if(!(f->flags&MI_ZF))goto mi_00485536;
    /* 0048552c */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048552d */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048552e */
    f->r[4]-=4;MI_U32(f->r[4])=0x00485533u;f->boundary(0x0048552eu,0x0043fd00u,f);
    /* 00485533 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00485536:;
    /* 00485536 */
    a=MI_U16((0x007999e8u+f->r[7]));b=(f->r[5]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 0048553d */
    if(!(f->flags&MI_ZF))goto mi_00485549;
    /* 0048553f */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485540 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485541 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00485546u;f->boundary(0x00485541u,0x0043fd00u,f);
    /* 00485546 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00485549:;
    /* 00485549 */
    a=MI_U16((0x007999e8u+f->r[7]));b=0x000000abu;t=mi_alu(f,a,b,16,1);
    /* 00485552 */
    if(!(f->flags&MI_ZF))goto mi_0048555d;
    /* 00485554 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485555 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048555au;f->boundary(0x00485555u,0x004bfee0u,f);
    /* 0048555a */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0048555d:;
    /* 0048555d */
    a=MI_U16((0x007999e8u+f->r[7]));b=0x000000cau;t=mi_alu(f,a,b,16,1);
    /* 00485566 */
    if(!(f->flags&MI_ZF))goto mi_00485571;
    /* 00485568 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485569 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048556eu;f->boundary(0x00485569u,0x004bfee0u,f);
    /* 0048556e */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00485571:;
    /* 00485571 */
    a=MI_U16((0x007999e8u+f->r[7]));b=0x000000e1u;t=mi_alu(f,a,b,16,1);
    /* 0048557a */
    if(!(f->flags&MI_ZF))goto mi_00485587;
    /* 0048557c */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048557d */
    f->r[1]=0x006fd5d8u;
    /* 00485582 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00485587u;f->boundary(0x00485582u,0x0040d890u,f);
mi_00485587:;
    /* 00485587 */
    a=MI_U16((0x007999e8u+f->r[7]));b=0x000000e6u;t=mi_alu(f,a,b,16,1);
    /* 00485590 */
    if(!(f->flags&MI_ZF))goto mi_0048559d;
    /* 00485592 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485593 */
    f->r[1]=0x006de650u;
    /* 00485598 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048559du;f->boundary(0x00485598u,0x00401480u,f);
mi_0048559d:;
    /* 0048559d */
    a=MI_U16((0x007999e8u+f->r[7]));b=0x000000e3u;t=mi_alu(f,a,b,16,1);
    /* 004855a6 */
    if(!(f->flags&MI_ZF))goto mi_004855b3;
    /* 004855a8 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004855a9 */
    f->r[1]=0x006fd368u;
    /* 004855ae */
    f->r[4]-=4;MI_U32(f->r[4])=0x004855b3u;f->boundary(0x004855aeu,0x00416fb0u,f);
mi_004855b3:;
    /* 004855b3 */
    a=MI_U16((0x007999e8u+f->r[7]));b=0x000000e4u;t=mi_alu(f,a,b,16,1);
    /* 004855bc */
    if(!(f->flags&MI_ZF))goto mi_004855c9;
    /* 004855be */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004855bf */
    f->r[1]=0x006de548u;
    /* 004855c4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004855c9u;f->boundary(0x004855c4u,0x004e6660u,f);
mi_004855c9:;
    /* 004855c9 */
    a=MI_U16((0x007999e8u+f->r[7]));b=0x000000e5u;t=mi_alu(f,a,b,16,1);
    /* 004855d2 */
    if(!(f->flags&MI_ZF))goto mi_004855df;
    /* 004855d4 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004855d5 */
    f->r[1]=0x006f9320u;
    /* 004855da */
    f->r[4]-=4;MI_U32(f->r[4])=0x004855dfu;f->boundary(0x004855dau,0x00545720u,f);
mi_004855df:;
    /* 004855df */
    a=MI_U16((0x007999e8u+f->r[7]));b=0x000000eau;t=mi_alu(f,a,b,16,1);
    /* 004855e8 */
    if(!(f->flags&MI_ZF))goto mi_004855f5;
    /* 004855ea */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004855eb */
    f->r[1]=0x006f5648u;
    /* 004855f0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004855f5u;f->boundary(0x004855f0u,0x00558bc0u,f);
mi_004855f5:;
    /* 004855f5 */
    a=MI_U16((0x007999e8u+f->r[7]));b=0x000000ebu;t=mi_alu(f,a,b,16,1);
    /* 004855fe */
    if(!(f->flags&MI_ZF))goto mi_0048560b;
    /* 00485600 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485601 */
    f->r[1]=0x006de9e0u;
    /* 00485606 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048560bu;f->boundary(0x00485606u,0x00555e60u,f);
mi_0048560b:;
    /* 0048560b */
    a=MI_U16((0x007999e8u+f->r[7]));b=0x000000ecu;t=mi_alu(f,a,b,16,1);
    /* 00485614 */
    if(!(f->flags&MI_ZF))goto mi_00485621;
    /* 00485616 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485617 */
    f->r[1]=0x006e9188u;
    /* 0048561c */
    f->r[4]-=4;MI_U32(f->r[4])=0x00485621u;f->boundary(0x0048561cu,0x0054f450u,f);
mi_00485621:;
    /* 00485621 */
    a=MI_U16((0x007999e8u+f->r[7]));b=0x00000039u;t=mi_alu(f,a,b,16,1);
    /* 00485629 */
    if(!(f->flags&MI_ZF))goto mi_0048570a;
    /* 0048562f */
    f->r[1]=0x00000001u;
    /* 00485634 */
    f->r[0]=0x00bcba9au;
mi_00485639:;
    /* 00485639 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0xffffffbeu+f->r[0])))&0xffu)<<0);
    /* 0048563c */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0048563e */
    if((f->flags&MI_ZF))goto mi_0048564d;
    /* 00485640 */
    a=MI_U8((0x00000000u+f->r[0]));b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 00485643 */
    if(!(f->flags&MI_ZF))goto mi_0048564d;
    /* 00485645 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0xffffffc4u+f->r[0]));
    /* 00485649 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 0048564b */
    if((f->flags&MI_ZF))goto mi_0048565f;
mi_0048564d:;
    /* 0048564d */
    a=f->r[0];b=0x00000090u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00485652 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00485653 */
    a=f->r[0];b=0x00bcd62au;t=mi_alu(f,a,b,32,1);
    /* 00485658 */
    if(mi_signed_less(f->flags))goto mi_00485639;
    /* 0048565a */
    goto mi_0048570a;
mi_0048565f:;
    /* 0048565f */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x007999eau+f->r[7])))&0xffffu)<<0);
    /* 00485666 */
    a=(f->r[0]&0xffffu);b=0x0000000du;t=mi_alu(f,a,b,16,1);
    /* 0048566a */
    if(!(f->flags&MI_ZF))goto mi_004856a0;
    /* 0048566c */
    f->r[0]=MI_U32((0x006fdcd4u));
    /* 00485671 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 00485674 */
    a=f->r[1];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00485677 */
    a=MI_U8((0x00bcb9cau+f->r[1]));b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 0048567d */
    if(!(f->flags&MI_ZF))goto mi_0048570a;
    /* 00485683 */
    f->r[2]=MI_U32((0x006fdcd8u));
    /* 00485689 */
    MI_U32((0x006fdcfcu))=(uint32_t)(f->r[0]);
    /* 0048568e */
    f->r[0]=MI_U32((0x006fdcdcu));
    /* 00485693 */
    MI_U32((0x006fdd00u))=(uint32_t)(f->r[2]);
    /* 00485699 */
    MI_U32((0x006fdd04u))=(uint32_t)(f->r[0]);
    /* 0048569e */
    goto mi_0048570a;
mi_004856a0:;
    /* 004856a0 */
    a=(f->r[0]&0xffffu);b=0x0000000cu;t=mi_alu(f,a,b,16,1);
    /* 004856a4 */
    if(!(f->flags&MI_ZF))goto mi_004856d6;
    /* 004856a6 */
    f->r[0]=MI_U32((0x006fdcbcu));
    /* 004856ab */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 004856ae */
    a=f->r[1];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004856b1 */
    a=MI_U8((0x00bcb9cau+f->r[1]));b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 004856b7 */
    if(!(f->flags&MI_ZF))goto mi_0048570a;
    /* 004856b9 */
    f->r[2]=MI_U32((0x006fdcc0u));
    /* 004856bf */
    MI_U32((0x006fdce4u))=(uint32_t)(f->r[0]);
    /* 004856c4 */
    f->r[0]=MI_U32((0x006fdcc4u));
    /* 004856c9 */
    MI_U32((0x006fdce8u))=(uint32_t)(f->r[2]);
    /* 004856cf */
    MI_U32((0x006fdcecu))=(uint32_t)(f->r[0]);
    /* 004856d4 */
    goto mi_0048570a;
mi_004856d6:;
    /* 004856d6 */
    a=(f->r[0]&0xffffu);b=0x0000000bu;t=mi_alu(f,a,b,16,1);
    /* 004856da */
    if(!(f->flags&MI_ZF))goto mi_0048570a;
    /* 004856dc */
    f->r[0]=MI_U32((0x006fdcc8u));
    /* 004856e1 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 004856e4 */
    a=f->r[1];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004856e7 */
    a=MI_U8((0x00bcb9cau+f->r[1]));b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 004856ed */
    if(!(f->flags&MI_ZF))goto mi_0048570a;
    /* 004856ef */
    f->r[2]=MI_U32((0x006fdcccu));
    /* 004856f5 */
    MI_U32((0x006fdcf0u))=(uint32_t)(f->r[0]);
    /* 004856fa */
    f->r[0]=MI_U32((0x006fdcd0u));
    /* 004856ff */
    MI_U32((0x006fdcf4u))=(uint32_t)(f->r[2]);
    /* 00485705 */
    MI_U32((0x006fdcf8u))=(uint32_t)(f->r[0]);
mi_0048570a:;
    /* 0048570a */
    MI_U8((0x007999d8u+f->r[7]))=(uint8_t)(0x00000002u);
mi_00485711:;
    /* 00485711 */
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
    /* 00485715 */
    f->r[2]=MI_U32((0x00000010u+f->r[4]));
    /* 00485719 */
    f->r[1]=MI_U32((0x00bc95f0u));
    /* 0048571f */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00485720 */
    a=f->r[2];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00485723 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 00485725 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00485729 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0048572d */
    if(mi_signed_less(f->flags))goto mi_004854a3;
    /* 00485733 */
    goto mi_004857e9;
mi_00485738:;
    /* 00485738 */
    a=0x00032c40u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048573d */
    a=0x00af598cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485742 */
    a=0x009de62cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485747 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048574cu;f->boundary(0x00485747u,0x0040f860u,f);
    /* 0048574c */
    a=0x0000cb10u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485751 */
    a=0x00ae8e7cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485756 */
    a=0x009d1b1cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048575b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00485760u;f->boundary(0x0048575bu,0x0040f860u,f);
    /* 00485760 */
    a=0x0000cb10u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485765 */
    a=0x00b285ccu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048576a */
    a=0x009b84fcu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048576f */
    f->r[4]-=4;MI_U32(f->r[4])=0x00485774u;f->boundary(0x0048576fu,0x0040f860u,f);
    /* 00485774 */
    a=0x0000cb10u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485779 */
    a=0x00b67d1cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048577e */
    a=0x009ab9ecu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485783 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00485788u;f->boundary(0x00485783u,0x0040f860u,f);
    /* 00485788 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485789 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048578eu;f->boundary(0x00485789u,0x00485830u,f);
    /* 0048578e */
    a=f->r[4];b=0x00000034u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00485791 */
    goto mi_004857e9;
mi_00485793:;
    /* 00485793 */
    a=0x00032c40u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485798 */
    a=0x00af598cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048579d */
    a=0x009de62cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004857a2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004857a7u;f->boundary(0x004857a2u,0x0040f860u,f);
    /* 004857a7 */
    a=0x0000cb10u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004857ac */
    a=0x00adc36cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004857b1 */
    a=0x009c500cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004857b6 */
    goto mi_004857db;
mi_004857b8:;
    /* 004857b8 */
    a=0x00032c40u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004857bd */
    a=0x00af598cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004857c2 */
    a=0x009de62cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004857c7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004857ccu;f->boundary(0x004857c7u,0x0040f860u,f);
    /* 004857cc */
    a=0x0000cb10u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004857d1 */
    a=0x00ae8e7cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004857d6 */
    a=0x009d1b1cu;f->r[4]-=4;MI_U32(f->r[4])=a;
mi_004857db:;
    /* 004857db */
    f->r[4]-=4;MI_U32(f->r[4])=0x004857e0u;f->boundary(0x004857dbu,0x0040f860u,f);
    /* 004857e0 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004857e1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004857e6u;f->boundary(0x004857e1u,0x00485830u,f);
    /* 004857e6 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004857e9:;
    /* 004857e9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004857eeu;f->boundary(0x004857e9u,0x00517370u,f);
    /* 004857ee */
    f->r[4]-=4;MI_U32(f->r[4])=0x004857f3u;f->boundary(0x004857eeu,0x00517660u,f);
    /* 004857f3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004857f8u;f->boundary(0x004857f3u,0x005178e0u,f);
    /* 004857f8 */
    MI_U32((0x00bc95f0u))=(uint32_t)(0x00000000u);
    /* 00485802 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00485807u;f->boundary(0x00485802u,0x0046b320u,f);
    /* 00485807 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048580cu;f->boundary(0x00485807u,0x0046f030u,f);
    /* 0048580c */
    f->r[4]-=4;MI_U32(f->r[4])=0x00485811u;f->boundary(0x0048580cu,0x0046a320u,f);
    /* 00485811 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00485816u;f->boundary(0x00485811u,0x004f61e0u,f);
    /* 00485816 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048581bu;f->boundary(0x00485816u,0x004dea50u,f);
    /* 0048581b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0048581c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0048581d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
mi_0048581e:;
    /* 0048581e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0048581f */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00485822 */
    f->r[4]+=4;return;
}
void restore_sidebar_C_00485890(struct restore_sidebar_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00485890 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485891 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485892 */
    f->r[7]=0x00000001u;
    /* 00485897 */
    f->r[6]=0x00799af0u;
mi_0048589c:;
    /* 0048589c */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0xfffffff0u+f->r[6])))&0xffu)<<0);
    /* 0048589f */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004858a1 */
    if((f->flags&MI_ZF))goto mi_004858c2;
    /* 004858a3 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00000000u+f->r[6])))&0xffffu)<<0);
    /* 004858a6 */
    a=(f->r[0]&0xffffu);b=0x0000000au;t=mi_alu(f,a,b,16,1);
    /* 004858aa */
    if(mi_signed_less(f->flags))goto mi_004858c2;
    /* 004858ac */
    a=(f->r[0]&0xffffu);b=0x0000001du;t=mi_alu(f,a,b,16,1);
    /* 004858b0 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004858c2;
    /* 004858b2 */
    a=MI_U16((0x0000000cu+f->r[6]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 004858b7 */
    if(!(f->flags&MI_ZF))goto mi_004858c2;
    /* 004858b9 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004858ba */
    f->r[4]-=4;MI_U32(f->r[4])=0x004858bfu;f->boundary(0x004858bau,0x0046cf40u,f);
    /* 004858bf */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004858c2:;
    /* 004858c2 */
    a=f->r[6];b=0x00000108u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004858c8 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 004858c9 */
    a=f->r[6];b=0x0089b6e8u;t=mi_alu(f,a,b,32,1);
    /* 004858cf */
    if(mi_signed_less(f->flags))goto mi_0048589c;
    /* 004858d1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004858d2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004858d3 */
    f->r[4]+=4;return;
}
void restore_sidebar_C_004858e0(struct restore_sidebar_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004858e0 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004858e1 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004858e2 */
    f->r[7]=mi_read((0x0000000cu+f->r[4]),4u);
    /* 004858e6 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004858e8 */
    a=f->r[7];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004858ea */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00485cdc;
    /* 004858f0 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004858f1 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004858f2 */
    f->r[6]=f->r[7];
    /* 004858f4 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004858f5 */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004858f8 */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004858fa */
    f->r[1]=0x00bca350u;
    /* 004858ff */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 00485902 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e8u+f->r[6]),2u);
    /* 00485909 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 0048590c */
    f->r[5]=mi_read((0x005d06dcu+f->r[0]*4u),4u);
    /* 00485913 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00485918u;f->boundary(0x00485913u,0x00488030u,f);
    /* 00485918 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0048591a */
    if((f->flags&MI_ZF))goto mi_0048592a;
    /* 0048591c */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048591d */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048591e */
    f->r[1]=0x00bca350u;
    /* 00485923 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00485928u;f->boundary(0x00485923u,0x00488e60u,f);
    /* 00485928 */
    goto mi_00485987;
mi_0048592a:;
    /* 0048592a */
    f->r[1]=(f->r[1]&0xffff0000u)|(((mi_read((0x007999e8u+f->r[6]),2u))&0xffffu)<<0);
    /* 00485931 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)(f->r[1]&0xffffu);
    /* 00485934 */
    a=f->r[0];b=0x00000041u;t=mi_alu(f,a,b,32,1);
    /* 00485937 */
    if(mi_signed_less(f->flags))goto mi_0048593e;
    /* 00485939 */
    a=f->r[0];b=0x00000045u;t=mi_alu(f,a,b,32,1);
    /* 0048593c */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00485964;
mi_0048593e:;
    /* 0048593e */
    a=(f->r[1]&0xffffu);b=0x000000cau;t=mi_alu(f,a,b,16,1);
    /* 00485943 */
    if((f->flags&MI_ZF))goto mi_00485964;
    /* 00485945 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e0u+f->r[6]),2u);
    /* 0048594c */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999deu+f->r[6]),2u);
    /* 00485953 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485954 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485955 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485956 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485957 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485958 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485959 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048595a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048595fu;f->boundary(0x0048595au,0x0047bf50u,f);
    /* 0048595f */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00485962 */
    goto mi_00485987;
mi_00485964:;
    /* 00485964 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e0u+f->r[6]),2u);
    /* 0048596b */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999deu+f->r[6]),2u);
    /* 00485972 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485973 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485974 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485975 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485976 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485977 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485978 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485979 */
    f->r[0]=mi_read((0x00eaac28u),4u);
    /* 0048597e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048597f */
    f->r[4]-=4;MI_U32(f->r[4])=0x00485984u;f->boundary(0x0048597fu,0x00470990u,f);
    /* 00485984 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00485987:;
    /* 00485987 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x007999e8u+f->r[6]),2u))&0xffffu)<<0);
    /* 0048598e */
    a=(f->r[0]&0xffffu);b=0x0000004cu;t=mi_alu(f,a,b,16,1);
    /* 00485992 */
    if((f->flags&MI_ZF))goto mi_004859a0;
    /* 00485994 */
    a=(f->r[0]&0xffffu);b=0x000000b5u;t=mi_alu(f,a,b,16,1);
    /* 00485998 */
    if((f->flags&MI_ZF))goto mi_004859a0;
    /* 0048599a */
    a=(f->r[0]&0xffffu);b=0x000000b6u;t=mi_alu(f,a,b,16,1);
    /* 0048599e */
    if(!(f->flags&MI_ZF))goto mi_004859a7;
mi_004859a0:;
    /* 004859a0 */
    MI_U16((0x00799a8cu+f->r[6]))=(uint16_t)((f->r[3]&0xffffu));
mi_004859a7:;
    /* 004859a7 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x007999ddu+f->r[6]),1u))&0xffu)<<0);
    /* 004859ad */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004859af */
    if((f->flags&MI_ZF))goto mi_004859b8;
    /* 004859b1 */
    MI_U16((0x007999f4u+f->r[6]))=(uint16_t)((f->r[3]&0xffffu));
mi_004859b8:;
    /* 004859b8 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x007999e8u+f->r[6]),2u))&0xffffu)<<0);
    /* 004859bf */
    f->r[3]=0x00000001u;
    /* 004859c4 */
    a=(f->r[0]&0xffffu);b=0x00000047u;t=mi_alu(f,a,b,16,1);
    /* 004859c8 */
    if((f->flags&MI_ZF))goto mi_004859e8;
    /* 004859ca */
    a=(f->r[0]&0xffffu);b=0x00000048u;t=mi_alu(f,a,b,16,1);
    /* 004859ce */
    if((f->flags&MI_ZF))goto mi_004859e8;
    /* 004859d0 */
    a=(f->r[0]&0xffffu);b=0x00000046u;t=mi_alu(f,a,b,16,1);
    /* 004859d4 */
    if((f->flags&MI_ZF))goto mi_004859e8;
    /* 004859d6 */
    a=(f->r[0]&0xffffu);b=0x000000bfu;t=mi_alu(f,a,b,16,1);
    /* 004859da */
    if((f->flags&MI_ZF))goto mi_004859e8;
    /* 004859dc */
    a=(f->r[0]&0xffffu);b=0x000000c1u;t=mi_alu(f,a,b,16,1);
    /* 004859e0 */
    if((f->flags&MI_ZF))goto mi_004859e8;
    /* 004859e2 */
    a=(f->r[0]&0xffffu);b=0x000000beu;t=mi_alu(f,a,b,16,1);
    /* 004859e6 */
    if(!(f->flags&MI_ZF))goto mi_00485a30;
mi_004859e8:;
    /* 004859e8 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004859ea */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00799a99u+f->r[6]),1u))&0xffu)<<0);
    /* 004859f0 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004859f2 */
    if((f->flags&MI_ZF))goto mi_00485a21;
    /* 004859f4 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004859f7 */
    a=f->r[1];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004859fa */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004859fc */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x007900c0u+f->r[1]*4u),1u))&0xffu)<<0);
    /* 00485a03 */
    f->r[1]=(0x007900c0u+f->r[1]*4u);
    /* 00485a0a */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00485a0c */
    if(!(f->flags&MI_ZF))goto mi_00485a21;
    /* 00485a0e */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)((f->r[3]&0xffu));
    /* 00485a10 */
    f->r[1]=mi_read((0x0078d788u),4u);
    /* 00485a16 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00485a18 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00485a30;
    /* 00485a1a */
    MI_U32((0x0078d788u))=(uint32_t)(f->r[0]);
    /* 00485a1f */
    goto mi_00485a30;
mi_00485a21:;
    /* 00485a21 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485a22 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00485a27u;f->boundary(0x00485a22u,0x0046b100u,f);
    /* 00485a27 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00485a2a */
    MI_U8((0x00799a99u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
mi_00485a30:;
    /* 00485a30 */
    a=mi_read((0x007999e8u+f->r[6]),2u);b=0x00000088u;t=mi_alu(f,a,b,16,1);
    /* 00485a39 */
    if(!(f->flags&MI_ZF))goto mi_00485a76;
    /* 00485a3b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00485a3d */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00799aa2u+f->r[6]),1u))&0xffu)<<0);
    /* 00485a43 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00485a45 */
    if((f->flags&MI_ZF))goto mi_00485a67;
    /* 00485a47 */
    f->r[2]=(0x00000000u+f->r[0]*8u);
    /* 00485a4e */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00485a50 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 00485a53 */
    f->r[1]=mi_read((0x00eaaf68u+f->r[2]),4u);
    /* 00485a59 */
    f->r[0]=(0x00eaaf68u+f->r[2]);
    /* 00485a5f */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00485a61 */
    if(!(f->flags&MI_ZF))goto mi_00485a67;
    /* 00485a63 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 00485a65 */
    goto mi_00485a76;
mi_00485a67:;
    /* 00485a67 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485a68 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00485a6du;f->boundary(0x00485a68u,0x0046efd0u,f);
    /* 00485a6d */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00485a70 */
    MI_U8((0x00799aa2u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
mi_00485a76:;
    /* 00485a76 */
    a=mi_read((0x007999e8u+f->r[6]),2u);b=0x0000004au;t=mi_alu(f,a,b,16,1);
    /* 00485a7e */
    if(!(f->flags&MI_ZF))goto mi_00485ab7;
    /* 00485a80 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00799aa3u+f->r[6]),1u))&0xffu)<<0);
    /* 00485a86 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00485a88 */
    if((f->flags&MI_ZF))goto mi_00485aae;
    /* 00485a8a */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 00485a8f */
    f->r[1]=(0x00000000u+f->r[0]*8u);
    /* 00485a96 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00485a98 */
    f->r[0]=(0x009783a4u+f->r[1]*4u);
    /* 00485a9f */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x009783a4u+f->r[1]*4u),1u))&0xffu)<<0);
    /* 00485aa6 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00485aa8 */
    if(!(f->flags&MI_ZF))goto mi_00485aae;
    /* 00485aaa */
    MI_U8((0x00000000u+f->r[0]))=(uint8_t)((f->r[3]&0xffu));
    /* 00485aac */
    goto mi_00485ab7;
mi_00485aae:;
    /* 00485aae */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485aaf */
    f->r[4]-=4;MI_U32(f->r[4])=0x00485ab4u;f->boundary(0x00485aafu,0x0046a280u,f);
    /* 00485ab4 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00485ab7:;
    /* 00485ab7 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((mi_read((0x007999e8u+f->r[6]),2u))&0xffffu)<<0);
    /* 00485abe */
    f->r[0]=(uint32_t)(int32_t)(int16_t)(f->r[1]&0xffffu);
    /* 00485ac1 */
    f->r[5]=(0xffffffceu+f->r[0]);
    /* 00485ac4 */
    a=f->r[5];b=0x000000acu;t=mi_alu(f,a,b,32,1);
    /* 00485aca */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00485c87;
    /* 00485ad0 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 00485ad2 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00485d0cu+f->r[5]),1u))&0xffu)<<0);
    /* 00485ad8 */
    switch(mi_read((0x00485ce0u+f->r[2]*4u),4u)){case 0x00485adfu:goto mi_00485adf;case 0x00485b16u:goto mi_00485b16;case 0x00485b5bu:goto mi_00485b5b;case 0x00485b78u:goto mi_00485b78;case 0x00485bbcu:goto mi_00485bbc;case 0x00485bd9u:goto mi_00485bd9;case 0x00485c1du:goto mi_00485c1d;case 0x00485c44u:goto mi_00485c44;case 0x00485c5du:goto mi_00485c5d;case 0x00485c71u:goto mi_00485c71;case 0x00485c87u:goto mi_00485c87;default:abort();}
mi_00485adf:;
    /* 00485adf */
    f->r[0]=mi_read((0x00eaac28u),4u);
    /* 00485ae4 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 00485aea */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999deu+f->r[6]),2u);
    /* 00485af1 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e0u+f->r[6]),2u);
    /* 00485af8 */
    MI_U8((0x007999d8u+f->r[6]))=(uint8_t)((f->r[3]&0xffu));
    /* 00485afe */
    MI_U32((0x00ea6010u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 00485b04 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00485b05 */
    MI_U32((0x00ea6014u+f->r[0]))=(uint32_t)(f->r[2]);
    /* 00485b0b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00485b0c */
    MI_U16((0x00ea6084u+f->r[0]))=(uint16_t)((f->r[3]&0xffffu));
    /* 00485b13 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00485b14 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00485b15 */
    f->r[4]+=4;return;
mi_00485b16:;
    /* 00485b16 */
    f->r[0]=mi_read((0x00eaac28u),4u);
    /* 00485b1b */
    f->r[1]=mi_read((0x007999e4u+f->r[6]),4u);
    /* 00485b21 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 00485b27 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((mi_read((0x007999deu+f->r[6]),2u))&0xffffu)<<0);
    /* 00485b2e */
    MI_U32((0x00ea3ffcu+f->r[0]))=(uint32_t)(f->r[1]);
    /* 00485b34 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((mi_read((0x007999e0u+f->r[6]),2u))&0xffffu)<<0);
    /* 00485b3b */
    MI_U8((0x007999d8u+f->r[6]))=(uint8_t)((f->r[3]&0xffu));
    /* 00485b41 */
    MI_U16((0x00ea3ff8u+f->r[0]))=(uint16_t)((f->r[2]&0xffffu));
    /* 00485b48 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00485b49 */
    MI_U16((0x00ea3ffau+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 00485b50 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00485b51 */
    MI_U16((0x00ea5cf6u+f->r[0]))=(uint16_t)((f->r[3]&0xffffu));
    /* 00485b58 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00485b59 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00485b5a */
    f->r[4]+=4;return;
mi_00485b5b:;
    /* 00485b5b */
    f->r[2]=mi_read((0x00eaac28u),4u);
    /* 00485b61 */
    MI_U8((0x007999d8u+f->r[6]))=(uint8_t)((f->r[3]&0xffu));
    /* 00485b67 */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 00485b6d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00485b6e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00485b6f */
    MI_U32((0x00ea620cu+f->r[2]))=(uint32_t)(f->r[3]);
    /* 00485b75 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00485b76 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00485b77 */
    f->r[4]+=4;return;
mi_00485b78:;
    /* 00485b78 */
    f->r[0]=mi_read((0x00eaac28u),4u);
    /* 00485b7d */
    f->r[1]=mi_read((0x007999e4u+f->r[6]),4u);
    /* 00485b83 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 00485b89 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((mi_read((0x007999deu+f->r[6]),2u))&0xffffu)<<0);
    /* 00485b90 */
    MI_U32((0x00ea5d18u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 00485b96 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((mi_read((0x007999e0u+f->r[6]),2u))&0xffffu)<<0);
    /* 00485b9d */
    MI_U8((0x007999d8u+f->r[6]))=(uint8_t)((f->r[3]&0xffu));
    /* 00485ba3 */
    MI_U16((0x00ea5d14u+f->r[0]))=(uint16_t)((f->r[2]&0xffffu));
    /* 00485baa */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00485bab */
    MI_U16((0x00ea5d16u+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 00485bb2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00485bb3 */
    MI_U32((0x00ea5d20u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 00485bb9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00485bba */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00485bbb */
    f->r[4]+=4;return;
mi_00485bbc:;
    /* 00485bbc */
    f->r[2]=mi_read((0x00eaac28u),4u);
    /* 00485bc2 */
    MI_U8((0x007999d8u+f->r[6]))=(uint8_t)((f->r[3]&0xffu));
    /* 00485bc8 */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 00485bce */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00485bcf */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00485bd0 */
    MI_U32((0x00ea6240u+f->r[2]))=(uint32_t)(f->r[3]);
    /* 00485bd6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00485bd7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00485bd8 */
    f->r[4]+=4;return;
mi_00485bd9:;
    /* 00485bd9 */
    f->r[0]=mi_read((0x00eaac28u),4u);
    /* 00485bde */
    f->r[1]=mi_read((0x007999e4u+f->r[6]),4u);
    /* 00485be4 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 00485bea */
    f->r[2]=(f->r[2]&0xffff0000u)|(((mi_read((0x007999deu+f->r[6]),2u))&0xffffu)<<0);
    /* 00485bf1 */
    MI_U32((0x00ea5e68u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 00485bf7 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((mi_read((0x007999e0u+f->r[6]),2u))&0xffffu)<<0);
    /* 00485bfe */
    MI_U8((0x007999d8u+f->r[6]))=(uint8_t)((f->r[3]&0xffu));
    /* 00485c04 */
    MI_U16((0x00ea5e64u+f->r[0]))=(uint16_t)((f->r[2]&0xffffu));
    /* 00485c0b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00485c0c */
    MI_U16((0x00ea5e66u+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 00485c13 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00485c14 */
    MI_U32((0x00ea5e70u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 00485c1a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00485c1b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00485c1c */
    f->r[4]+=4;return;
mi_00485c1d:;
    /* 00485c1d */
    f->r[0]=mi_read((0x00799a9cu+f->r[6]),4u);
    /* 00485c23 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00485c25 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00485cd4;
    /* 00485c2b */
    f->r[2]=f->r[0];
    /* 00485c2d */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 00485c30 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00485c32 */
    MI_U32((0x00799a9cu+f->r[2]*8u))=(uint32_t)(f->r[7]);
    /* 00485c39 */
    MI_U8((0x007999d8u+f->r[6]))=(uint8_t)((f->r[3]&0xffu));
    /* 00485c3f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00485c40 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00485c41 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00485c42 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00485c43 */
    f->r[4]+=4;return;
mi_00485c44:;
    /* 00485c44 */
    f->r[0]=mi_read((0x00eaac28u),4u);
    /* 00485c49 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 00485c4f */
    f->r[1]=(f->r[1]&0xffff0000u)|(((mi_read((0x00ea6244u+f->r[0]),2u))&0xffffu)<<0);
    /* 00485c56 */
    MI_U16((0x00ea5baau+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
mi_00485c5d:;
    /* 00485c5d */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485c5e */
    f->r[4]-=4;MI_U32(f->r[4])=0x00485c63u;f->boundary(0x00485c5eu,0x004f4f80u,f);
    /* 00485c63 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00485c66 */
    MI_U8((0x007999d8u+f->r[6]))=(uint8_t)((f->r[3]&0xffu));
    /* 00485c6c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00485c6d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00485c6e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00485c6f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00485c70 */
    f->r[4]+=4;return;
mi_00485c71:;
    /* 00485c71 */
    f->r[0]=mi_read((0x00bcb9b0u),4u);
    /* 00485c76 */
    MI_U8((0x007999d8u+f->r[6]))=(uint8_t)((f->r[3]&0xffu));
    /* 00485c7c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00485c7d */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00485c7e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00485c7f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00485c80 */
    MI_U32((0x00bcb9b0u))=(uint32_t)(f->r[0]);
    /* 00485c85 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00485c86 */
    f->r[4]+=4;return;
mi_00485c87:;
    /* 00485c87 */
    a=(f->r[1]&0xffffu);b=0x000000cfu;t=mi_alu(f,a,b,16,1);
    /* 00485c8c */
    if((f->flags&MI_ZF))goto mi_00485c95;
    /* 00485c8e */
    a=(f->r[1]&0xffffu);b=0x000000d0u;t=mi_alu(f,a,b,16,1);
    /* 00485c93 */
    if(!(f->flags&MI_ZF))goto mi_00485ca9;
mi_00485c95:;
    /* 00485c95 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00485c96 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00485c9bu;f->boundary(0x00485c96u,0x004f4f80u,f);
    /* 00485c9b */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00485c9e */
    MI_U8((0x007999d8u+f->r[6]))=(uint8_t)((f->r[3]&0xffu));
    /* 00485ca4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00485ca5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00485ca6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00485ca7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00485ca8 */
    f->r[4]+=4;return;
mi_00485ca9:;
    /* 00485ca9 */
    a=f->r[0];b=0x00000041u;t=mi_alu(f,a,b,32,1);
    /* 00485cac */
    if(mi_signed_less(f->flags))goto mi_00485cd4;
    /* 00485cae */
    a=f->r[0];b=0x00000045u;t=mi_alu(f,a,b,32,1);
    /* 00485cb1 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00485cd4;
    /* 00485cb3 */
    f->r[0]=mi_read((0x00eaac28u),4u);
    /* 00485cb8 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 00485cbe */
    MI_U16((0x00ea5ce8u+f->r[0]))=(uint16_t)((f->r[3]&0xffffu));
    /* 00485cc5 */
    MI_U16((0x00ea5cfau+f->r[0]))=(uint16_t)(0x00000002u);
    /* 00485cce */
    MI_U32((0x00ea5f9cu+f->r[0]))=(uint32_t)(f->r[7]);
mi_00485cd4:;
    /* 00485cd4 */
    MI_U8((0x007999d8u+f->r[6]))=(uint8_t)((f->r[3]&0xffu));
    /* 00485cda */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00485cdb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
mi_00485cdc:;
    /* 00485cdc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00485cdd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00485cde */
    f->r[4]+=4;return;
}
void restore_sidebar_C_0040d890(struct restore_sidebar_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0040d890 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 0040d893 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040d894 */
    f->r[3]=MI_U32((0x00000010u+f->r[4]));
    /* 0040d898 */
    f->r[0]=f->r[3];
    /* 0040d89a */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040d89b */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0040d89e */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0040d8a0 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040d8a1 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0040d8a4 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040d8a5 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[0]));
    /* 0040d8ac */
    f->r[5]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[0]));
    /* 0040d8b3 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999eau+f->r[0]));
    /* 0040d8ba */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040d8bb */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040d8bc */
    f->r[6]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0040d8bf */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040d8c0 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0040d8c4 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[6]*4u);
    /* 0040d8c7 */
    f->r[7]=(0x00000054u+f->r[1]+f->r[0]*4u);
    /* 0040d8cb */
    f->r[1]=MI_U32((0x00000054u+f->r[1]+f->r[0]*4u));
    /* 0040d8cf */
    MI_U32((0x00bc9738u))=(uint32_t)(f->r[1]);
    /* 0040d8d5 */
    f->r[0]=MI_U32((0x00000000u+f->r[7]));
    /* 0040d8d7 */
    MI_U32((0x00bc973cu))=(uint32_t)(f->r[0]);
    /* 0040d8dc */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040d8e1u;f->boundary(0x0040d8dcu,0x0047c480u,f);
    /* 0040d8e1 */
    f->r[0]=MI_U32((0x00000000u+f->r[7]));
    /* 0040d8e3 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040d8e6 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0040d8e8 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0040d8ea */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0040d8ee */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0040d928;
mi_0040d8f0:;
    /* 0040d8f0 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 0040d8f2 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0040d8f4 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0040d91d;
    /* 0040d8f6 */
    f->r[2]=MI_U32((0x00000010u+f->r[4]));
    /* 0040d8fa */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0040d8fc */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[1]);
mi_0040d900:;
    /* 0040d900 */
    f->r[1]=MI_U32((0x00000014u+f->r[4]));
    /* 0040d904 */
    f->r[0]=(0x00000000u+f->r[6]+f->r[5]*1u);
    /* 0040d907 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040d908 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040d909 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040d90a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040d90fu;f->boundary(0x0040d90au,0x0047c480u,f);
    /* 0040d90f */
    f->r[0]=MI_U32((0x00000000u+f->r[7]));
    /* 0040d911 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040d914 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0040d915 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0040d917 */
    if(mi_signed_less(f->flags))goto mi_0040d900;
    /* 0040d919 */
    f->r[1]=MI_U32((0x0000001cu+f->r[4]));
mi_0040d91d:;
    /* 0040d91d */
    f->r[0]=MI_U32((0x00000000u+f->r[7]));
    /* 0040d91f */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0040d920 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0040d922 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0040d926 */
    if(mi_signed_less(f->flags))goto mi_0040d8f0;
mi_0040d928:;
    /* 0040d928 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040d929 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040d92a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0040d92b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040d92c */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040d92f */
    f->r[4]+=8;return;
}
void restore_sidebar_C_00401480(struct restore_sidebar_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00401480 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 00401483 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00401484 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00401485 */
    f->r[5]=MI_U32((0x00000014u+f->r[4]));
    /* 00401489 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040148a */
    f->r[0]=f->r[5];
    /* 0040148c */
    MI_U32((0x0000000cu+f->r[4]))=(uint32_t)(0x00000000u);
    /* 00401494 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00401497 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00401499 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0040149c */
    f->r[3]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[0]));
    /* 004014a3 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[0]));
    /* 004014aa */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999eau+f->r[0]));
    /* 004014b1 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004014b5 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004014b8 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*4u);
    /* 004014bb */
    f->r[7]=(0x00000000u+f->r[1]+f->r[0]*4u);
    /* 004014be */
    f->r[0]=MI_U32((0x00000054u+f->r[1]+f->r[0]*4u));
    /* 004014c2 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004014c4 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0040152b;
    /* 004014c6 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
mi_004014c7:;
    /* 004014c7 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004014c9 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(0x00000000u);
    /* 004014d1 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00401519;
    /* 004014d3 */
    f->r[6]=MI_U32((0x00000014u+f->r[4]));
mi_004014d7:;
    /* 004014d7 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004014d8 */
    f->r[0]=0x00000001u;
    /* 004014dd */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004014de */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004014df */
    MI_U32((0x00bc973cu))=(uint32_t)(f->r[0]);
    /* 004014e4 */
    MI_U32((0x00bc9738u))=(uint32_t)(f->r[0]);
    /* 004014e9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004014eeu;f->boundary(0x004014e9u,0x0047c480u,f);
    /* 004014ee */
    f->r[0]=MI_U32((0x00000058u+f->r[7]));
    /* 004014f1 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004014f4 */
    a=f->r[0];b=0x00000009u;t=mi_alu(f,a,b,32,1);
    /* 004014f7 */
    if((f->flags&MI_ZF))goto mi_00401508;
    /* 004014f9 */
    a=0x20000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004014fe */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004014ff */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00401500 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00401505u;f->boundary(0x00401500u,0x00477a40u,f);
    /* 00401505 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00401508:;
    /* 00401508 */
    f->r[0]=MI_U32((0x0000001cu+f->r[4]));
    /* 0040150c */
    f->r[1]=MI_U32((0x00000054u+f->r[7]));
    /* 0040150f */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00401510 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 00401511 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 00401513 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00401517 */
    if(mi_signed_less(f->flags))goto mi_004014d7;
mi_00401519:;
    /* 00401519 */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
    /* 0040151d */
    f->r[0]=MI_U32((0x00000054u+f->r[7]));
    /* 00401520 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00401521 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 00401522 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00401524 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00401528 */
    if(mi_signed_less(f->flags))goto mi_004014c7;
    /* 0040152a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
mi_0040152b:;
    /* 0040152b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040152c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0040152d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040152e */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00401531 */
    f->r[4]+=8;return;
}
void restore_sidebar_C_00416fb0(struct restore_sidebar_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00416fb0 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 00416fb3 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00416fb4 */
    f->r[3]=MI_U32((0x00000010u+f->r[4]));
    /* 00416fb8 */
    f->r[0]=f->r[3];
    /* 00416fba */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00416fbb */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00416fbe */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00416fc0 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00416fc1 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00416fc4 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 00416fc6 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[0]));
    /* 00416fcd */
    MI_U32((0x0000000cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00416fd1 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 00416fd5 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[0]));
    /* 00416fdc */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999eau+f->r[0]));
    /* 00416fe3 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00416fe7 */
    f->r[2]=0x00000001u;
    /* 00416fec */
    MI_U32((0x00bc9738u))=(uint32_t)(f->r[2]);
    /* 00416ff2 */
    MI_U32((0x00bc973cu))=(uint32_t)(f->r[2]);
    /* 00416ff8 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 00416ffb */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*4u);
    /* 00416ffe */
    f->r[7]=(0x00000054u+f->r[1]+f->r[0]*4u);
    /* 00417002 */
    f->r[0]=MI_U32((0x00000054u+f->r[1]+f->r[0]*4u));
    /* 00417006 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00417008 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00417040;
    /* 0041700a */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
mi_0041700b:;
    /* 0041700b */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 0041700d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0041700f */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00417034;
    /* 00417011 */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
    /* 00417015 */
    a=f->r[5];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[5]=t;
mi_00417017:;
    /* 00417017 */
    f->r[2]=MI_U32((0x00000014u+f->r[4]));
    /* 0041701b */
    f->r[0]=(0x00000000u+f->r[6]+f->r[2]*1u);
    /* 0041701e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041701f */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00417020 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00417021 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00417026u;f->boundary(0x00417021u,0x0047c480u,f);
    /* 00417026 */
    f->r[0]=MI_U32((0x00000000u+f->r[7]));
    /* 00417028 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0041702b */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0041702c */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0041702e */
    if(mi_signed_less(f->flags))goto mi_00417017;
    /* 00417030 */
    f->r[5]=MI_U32((0x0000001cu+f->r[4]));
mi_00417034:;
    /* 00417034 */
    f->r[0]=MI_U32((0x00000000u+f->r[7]));
    /* 00417036 */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 00417037 */
    a=f->r[5];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00417039 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[5]);
    /* 0041703d */
    if(mi_signed_less(f->flags))goto mi_0041700b;
    /* 0041703f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
mi_00417040:;
    /* 00417040 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00417041 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00417042 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00417043 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00417046 */
    f->r[4]+=8;return;
}
void restore_sidebar_C_004e6660(struct restore_sidebar_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004e6660 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 004e6663 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e6664 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e6665 */
    f->r[5]=MI_U32((0x00000014u+f->r[4]));
    /* 004e6669 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e666a */
    f->r[0]=f->r[5];
    /* 004e666c */
    MI_U32((0x0000000cu+f->r[4]))=(uint32_t)(0x00000000u);
    /* 004e6674 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004e6677 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004e6679 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004e667c */
    f->r[3]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[0]));
    /* 004e6683 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[0]));
    /* 004e668a */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999eau+f->r[0]));
    /* 004e6691 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004e6695 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004e6698 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*4u);
    /* 004e669b */
    f->r[7]=(0x00000000u+f->r[1]+f->r[0]*4u);
    /* 004e669e */
    f->r[0]=MI_U32((0x00000054u+f->r[1]+f->r[0]*4u));
    /* 004e66a2 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e66a4 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e66f8;
    /* 004e66a6 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
mi_004e66a7:;
    /* 004e66a7 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e66a9 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(0x00000000u);
    /* 004e66b1 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e66e6;
    /* 004e66b3 */
    f->r[6]=MI_U32((0x00000014u+f->r[4]));
mi_004e66b7:;
    /* 004e66b7 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e66b8 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e66b9 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e66ba */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e66bfu;f->boundary(0x004e66bau,0x0047c480u,f);
    /* 004e66bf */
    f->r[0]=MI_U32((0x00000058u+f->r[7]));
    /* 004e66c2 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e66c5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e66c7 */
    if(!(f->flags&MI_ZF))goto mi_004e66d5;
    /* 004e66c9 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e66cb */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e66cc */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e66cd */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e66d2u;f->boundary(0x004e66cdu,0x00477a40u,f);
    /* 004e66d2 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004e66d5:;
    /* 004e66d5 */
    f->r[0]=MI_U32((0x0000001cu+f->r[4]));
    /* 004e66d9 */
    f->r[1]=MI_U32((0x00000054u+f->r[7]));
    /* 004e66dc */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004e66dd */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 004e66de */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004e66e0 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004e66e4 */
    if(mi_signed_less(f->flags))goto mi_004e66b7;
mi_004e66e6:;
    /* 004e66e6 */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
    /* 004e66ea */
    f->r[0]=MI_U32((0x00000054u+f->r[7]));
    /* 004e66ed */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004e66ee */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 004e66ef */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004e66f1 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004e66f5 */
    if(mi_signed_less(f->flags))goto mi_004e66a7;
    /* 004e66f7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
mi_004e66f8:;
    /* 004e66f8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004e66f9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004e66fa */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004e66fb */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e66fe */
    f->r[4]+=8;return;
}
void restore_sidebar_C_00545720(struct restore_sidebar_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00545720 */
    f->r[2]=MI_U32((0x00000004u+f->r[4]));
    /* 00545724 */
    f->r[0]=(0x00001e5cu+f->r[1]);
    /* 0054572a */
    a=0x000000a7u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0054572f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00545730 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00545731 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00545736u;f->boundary(0x00545731u,0x00546660u,f);
    /* 00545736 */
    f->r[4]+=8;return;
}
void restore_sidebar_C_00558bc0(struct restore_sidebar_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00558bc0 */
    f->r[2]=MI_U32((0x00000004u+f->r[4]));
    /* 00558bc4 */
    f->r[0]=(0x00001cbcu+f->r[1]);
    /* 00558bca */
    a=0x0000009eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00558bcf */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00558bd0 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00558bd1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00558bd6u;f->boundary(0x00558bd1u,0x00546660u,f);
    /* 00558bd6 */
    f->r[4]+=8;return;
}
void restore_sidebar_C_00555e60(struct restore_sidebar_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00555e60 */
    f->r[2]=MI_U32((0x00000004u+f->r[4]));
    /* 00555e64 */
    f->r[0]=(0x00004ee0u+f->r[1]);
    /* 00555e6a */
    a=0x000001b5u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00555e6f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00555e70 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00555e71 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00555e76u;f->boundary(0x00555e71u,0x00546660u,f);
    /* 00555e76 */
    f->r[4]+=8;return;
}
void restore_sidebar_C_0054f450(struct restore_sidebar_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0054f450 */
    f->r[2]=MI_U32((0x00000004u+f->r[4]));
    /* 0054f454 */
    f->r[0]=(0x00005c88u+f->r[1]);
    /* 0054f45a */
    a=0x00000201u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0054f45f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0054f460 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0054f461 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0054f466u;f->boundary(0x0054f461u,0x00546660u,f);
    /* 0054f466 */
    f->r[4]+=8;return;
}
void restore_sidebar_C_00546660(struct restore_sidebar_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00546660 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 00546663 */
    MI_U32((0x00000008u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00546667 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00546668 */
    f->r[1]=MI_U32((0x00000018u+f->r[4]));
    /* 0054666c */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0054666d */
    f->r[0]=f->r[1];
    /* 0054666f */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00546670 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00546673 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00546675 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00546676 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00546679 */
    f->r[7]=MI_U32((0x00000028u+f->r[4]));
    /* 0054667d */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 0054667f */
    f->r[5]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999eau+f->r[0]));
    /* 00546686 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[0]));
    /* 0054668d */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[0]));
    /* 00546694 */
    f->r[0]=(0x00000000u+f->r[5]+f->r[5]*2u);
    /* 00546698 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0054669c */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0054669e */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 005466a2 */
    f->r[0]=(0x00000000u+f->r[5]+f->r[0]*4u);
    /* 005466a6 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 005466aa */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 005466ae */
    f->r[3]=(0x00000008u+f->r[7]+f->r[0]*4u);
    /* 005466b2 */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 005466b6 */
    f->r[0]=MI_U32((0x00000000u+f->r[3]));
    /* 005466b8 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 005466ba */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00546784;
mi_005466c0:;
    /* 005466c0 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 005466c2 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 005466c4 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00546775;
    /* 005466ca */
    f->r[0]=(0x00000000u+f->r[2]+f->r[2]*8u);
    /* 005466cd */
    f->r[0]=(0x00000000u+f->r[2]+f->r[0]*2u);
    /* 005466d0 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 005466d3 */
    f->r[0]=MI_U32((0x00bca2d8u));
    /* 005466d8 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[2]*4u);
    /* 005466db */
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
    /* 005466df */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 005466e1 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 005466e3 */
    f->r[6]=(0x00af598cu+f->r[0]*4u);
    /* 005466ea */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*1u);
    /* 005466ed */
    goto mi_005466f3;
mi_005466ef:;
    /* 005466ef */
    f->r[5]=MI_U32((0x00000024u+f->r[4]));
mi_005466f3:;
    /* 005466f3 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((MI_U8((0x00b285ccu+f->r[0])))&0xffu)<<0);
    /* 005466f9 */
    MI_U16((0x00b4e6fcu+f->r[2]))=(uint16_t)((f->r[1]&0xffffu));
    /* 00546700 */
    a=(f->r[3]&0xffu);b=0x000000e0u;t=mi_alu(f,a,b,8,3);f->r[3]=(f->r[3]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00546703 */
    MI_U32((0x00000000u+f->r[6]))=(uint32_t)(f->r[1]);
    /* 00546705 */
    MI_U8((0x00b285ccu+f->r[0]))=(uint8_t)((f->r[3]&0xffu));
    /* 0054670b */
    f->r[3]=(f->r[3]&0xffffff00u)|(((MI_U8((0x00b67d1cu+f->r[0])))&0xffu)<<0);
    /* 00546711 */
    a=(f->r[3]&0xffu);b=0x000000c0u;t=mi_alu(f,a,b,8,3);f->r[3]=(f->r[3]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00546714 */
    MI_U8((0x00ae8e7cu+f->r[0]))=(uint8_t)((f->r[1]&0xffu));
    /* 0054671a */
    a=(f->r[3]&0xffu);b=0x00000040u;t=mi_alu(f,a,b,8,4);f->r[3]=(f->r[3]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0054671d */
    MI_U16((0x00a9cc1cu+f->r[2]))=(uint16_t)((f->r[1]&0xffffu));
    /* 00546724 */
    MI_U8((0x00b67d1cu+f->r[0]))=(uint8_t)((f->r[3]&0xffu));
    /* 0054672a */
    f->r[3]=MI_U32((0x00000018u+f->r[4]));
    /* 0054672e */
    MI_U8((0x00adc36cu+f->r[0]))=(uint8_t)((f->r[1]&0xffu));
    /* 00546734 */
    a=f->r[5];b=MI_U32((0x00000050u+f->r[3]));t=mi_alu(f,a,b,32,1);f->r[5]=t;
    /* 00546737 */
    f->r[3]=MI_U32((0x00000028u+f->r[4]));
    /* 0054673b */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,0,a,32,1);f->r[5]=t;
    /* 0054673d */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,2);f->r[5]=t;
    /* 0054673f */
    a=f->r[2];b=0x000001c8u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00546745 */
    a=f->r[5];b=0x1fffff82u;t=mi_alu(f,a,b,32,3);f->r[5]=t;
    /* 0054674b */
    a=f->r[6];b=0x00000390u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00546751 */
    a=f->r[5];b=0x00000080u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 00546757 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 00546758 */
    MI_U32((0xfffffc70u+f->r[6]))=(uint32_t)(f->r[5]);
    /* 0054675e */
    f->r[5]=MI_U32((0x00000000u+f->r[3]));
    /* 00546760 */
    a=f->r[0];b=0x000000e4u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00546765 */
    a=f->r[7];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 00546767 */
    if(mi_signed_less(f->flags))goto mi_005466ef;
    /* 00546769 */
    f->r[6]=MI_U32((0x00000010u+f->r[4]));
    /* 0054676d */
    f->r[5]=MI_U32((0x00000024u+f->r[4]));
    /* 00546771 */
    f->r[2]=MI_U32((0x0000001cu+f->r[4]));
mi_00546775:;
    /* 00546775 */
    f->r[0]=MI_U32((0x00000000u+f->r[3]));
    /* 00546777 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 00546778 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0054677a */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 0054677e */
    if(mi_signed_less(f->flags))goto mi_005466c0;
mi_00546784:;
    /* 00546784 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00546785 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00546786 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00546787 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00546788 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0054678b */
    f->r[4]+=16;return;
}
void restore_sidebar_C_0045afe0(struct restore_sidebar_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0045afe0 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 0045afe3 */
    f->r[2]=MI_U32((0x0078d790u));
    /* 0045afe9 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045afea */
    f->r[1]=0x00000001u;
    /* 0045afef */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045aff0 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0045aff2 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045aff3 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0045aff5 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045aff6 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0045affa */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0045affe */
    if(mi_signed_less(f->flags))goto mi_0045b21a;
    /* 0045b004 */
    f->r[7]=MI_U32((0x0000001cu+f->r[4]));
    /* 0045b008 */
    f->r[3]=MI_U32((0x00000020u+f->r[4]));
    /* 0045b00c */
    f->r[5]=0x00799b24u;
    /* 0045b011 */
    f->r[0]=(0x00799b36u+f->r[7]*2u);
    /* 0045b018 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[0]);
mi_0045b01c:;
    /* 0045b01c */
    a=MI_U8((0xffffffbcu+f->r[5]));b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 0045b020 */
    if(!(f->flags&MI_ZF))goto mi_0045b1eb;
    /* 0045b026 */
    a=MI_U8((0x00000000u+f->r[5]));b=0x00000000u;t=mi_alu(f,a,b,8,1);
    /* 0045b02a */
    if((f->flags&MI_ZF))goto mi_0045b1eb;
    /* 0045b030 */
    a=MI_U16((0xffffffdcu+f->r[5]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 0045b035 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0045b1eb;
    /* 0045b03b */
    a=MI_U16((0xfffffffcu+f->r[5]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 0045b040 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0045b1eb;
    /* 0045b046 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0xffffffccu+f->r[5])))&0xffffu)<<0);
    /* 0045b04a */
    a=(f->r[0]&0xffffu);b=0x00000048u;t=mi_alu(f,a,b,16,1);
    /* 0045b04e */
    if(!(f->flags&MI_ZF))goto mi_0045b136;
    /* 0045b054 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0045b056 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0045b058 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x0000007du+f->r[5])))&0xffu)<<0);
    /* 0045b05b */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0045b05e */
    a=f->r[1];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0045b061 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0045b063 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x007900c2u+f->r[7]+f->r[1]*4u)))&0xffu)<<0);
    /* 0045b06a */
    f->r[1]=f->r[2];
    /* 0045b06c */
    a=f->r[1];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0045b06f */
    if((f->flags&MI_ZF))goto mi_0045b08f;
    /* 0045b071 */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0045b074 */
    if(!(f->flags&MI_ZF))goto mi_0045b1eb;
    /* 0045b07a */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0045b07d */
    a=f->r[1];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0045b080 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0045b082 */
    f->r[2]=(0x00000000u+f->r[7]+f->r[1]*2u);
    /* 0045b085 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00790134u+f->r[2]*2u));
    /* 0045b08d */
    goto mi_0045b0a2;
mi_0045b08f:;
    /* 0045b08f */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0045b092 */
    a=f->r[1];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0045b095 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0045b097 */
    f->r[2]=(0x00000000u+f->r[7]+f->r[1]*2u);
    /* 0045b09a */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007900ecu+f->r[2]*2u));
mi_0045b0a2:;
    /* 0045b0a2 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0xfffffff6u+f->r[5]));
    /* 0045b0a6 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 0045b0a8 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0045b0aa */
    if((f->flags&MI_ZF))goto mi_0045b0f0;
mi_0045b0ac:;
    /* 0045b0ac */
    f->r[0]=f->r[1];
    /* 0045b0ae */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0045b0b1 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0045b0b3 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0045b0b6 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x007999eau+f->r[0])))&0xffffu)<<0);
    /* 0045b0bd */
    a=(f->r[1]&0xffffu);b=(f->r[1]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 0045b0c0 */
    if(!(f->flags&MI_ZF))goto mi_0045b0ca;
    /* 0045b0c2 */
    a=f->r[6];b=0x00000190u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0045b0c8 */
    goto mi_0045b0e5;
mi_0045b0ca:;
    /* 0045b0ca */
    f->r[1]=(uint32_t)(int32_t)(int16_t)(f->r[1]&0xffffu);
    /* 0045b0cd */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 0045b0cf */
    if(!(f->flags&MI_ZF))goto mi_0045b0e5;
    /* 0045b0d1 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00799a14u+f->r[0]));
    /* 0045b0d8 */
    f->r[3]=0x00000190u;
    /* 0045b0dd */
    a=f->r[3];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 0045b0df */
    a=f->r[6];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0045b0e1 */
    f->r[3]=MI_U32((0x00000020u+f->r[4]));
mi_0045b0e5:;
    /* 0045b0e5 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00799a12u+f->r[0]));
    /* 0045b0ec */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0045b0ee */
    if(!(f->flags&MI_ZF))goto mi_0045b0ac;
mi_0045b0f0:;
    /* 0045b0f0 */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0045b0f2 */
    if(mi_signed_less(f->flags))goto mi_0045b0f6;
    /* 0045b0f4 */
    f->r[6]=f->r[2];
mi_0045b0f6:;
    /* 0045b0f6 */
    a=f->r[6];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 0045b0f8 */
    if(mi_signed_less(f->flags))goto mi_0045b0fc;
    /* 0045b0fa */
    f->r[6]=f->r[3];
mi_0045b0fc:;
    /* 0045b0fc */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
    /* 0045b100 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000024u+f->r[4])))&0xffu)<<0);
    /* 0045b104 */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0045b106 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0045b108 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0045b10c */
    if(!(f->flags&MI_ZF))goto mi_0045b123;
    /* 0045b10e */
    f->r[2]=MI_U32((0x00000014u+f->r[4]));
    /* 0045b112 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 0045b117 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045b118 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045b119 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045b11a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045b11b */
    f->r[4]-=4;MI_U32(f->r[4])=0x0045b120u;f->boundary(0x0045b11bu,0x0045b2a0u,f);
    /* 0045b120 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0045b123:;
    /* 0045b123 */
    a=f->r[3];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 0045b125 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 0045b127 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 0045b12b */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0045b216;
    /* 0045b131 */
    goto mi_0045b1eb;
mi_0045b136:;
    /* 0045b136 */
    a=(f->r[0]&0xffffu);b=0x00000047u;t=mi_alu(f,a,b,16,1);
    /* 0045b13a */
    if((f->flags&MI_ZF))goto mi_0045b146;
    /* 0045b13c */
    a=(f->r[0]&0xffffu);b=0x000000bfu;t=mi_alu(f,a,b,16,1);
    /* 0045b140 */
    if(!(f->flags&MI_ZF))goto mi_0045b1eb;
mi_0045b146:;
    /* 0045b146 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 0045b148 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0045b1eb;
    /* 0045b14e */
    a=f->r[7];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 0045b151 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0045b1eb;
    /* 0045b157 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0045b159 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0045b15b */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x0000007du+f->r[5])))&0xffu)<<0);
    /* 0045b15e */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0045b161 */
    a=f->r[1];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0045b164 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0045b166 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x007900c2u+f->r[7]+f->r[1]*4u)))&0xffu)<<0);
    /* 0045b16d */
    f->r[1]=f->r[2];
    /* 0045b16f */
    a=f->r[1];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0045b172 */
    if((f->flags&MI_ZF))goto mi_0045b18e;
    /* 0045b174 */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0045b177 */
    if(!(f->flags&MI_ZF))goto mi_0045b1eb;
    /* 0045b179 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0045b17c */
    a=f->r[1];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0045b17f */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0045b181 */
    f->r[2]=(0x00000000u+f->r[7]+f->r[1]*2u);
    /* 0045b184 */
    f->r[6]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00790134u+f->r[2]*2u));
    /* 0045b18c */
    goto mi_0045b1a1;
mi_0045b18e:;
    /* 0045b18e */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0045b191 */
    a=f->r[1];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0045b194 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0045b196 */
    f->r[2]=(0x00000000u+f->r[7]+f->r[1]*2u);
    /* 0045b199 */
    f->r[6]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007900ecu+f->r[2]*2u));
mi_0045b1a1:;
    /* 0045b1a1 */
    f->r[0]=MI_U32((0x0000001cu+f->r[4]));
    /* 0045b1a5 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[0]));
    /* 0045b1a8 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000012u+f->r[5]));
    /* 0045b1ac */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 0045b1ae */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0045b1b0 */
    if(mi_signed_less(f->flags))goto mi_0045b1b4;
    /* 0045b1b2 */
    f->r[6]=f->r[0];
mi_0045b1b4:;
    /* 0045b1b4 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 0045b1b6 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0045b1eb;
    /* 0045b1b8 */
    a=f->r[6];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 0045b1ba */
    if(mi_signed_less(f->flags))goto mi_0045b1be;
    /* 0045b1bc */
    f->r[6]=f->r[3];
mi_0045b1be:;
    /* 0045b1be */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
    /* 0045b1c2 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000024u+f->r[4])))&0xffu)<<0);
    /* 0045b1c6 */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0045b1c8 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0045b1ca */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0045b1ce */
    if(!(f->flags&MI_ZF))goto mi_0045b1e1;
    /* 0045b1d0 */
    f->r[2]=MI_U32((0x00000014u+f->r[4]));
    /* 0045b1d4 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045b1d5 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045b1d7 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045b1d8 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045b1d9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0045b1deu;f->boundary(0x0045b1d9u,0x0045e220u,f);
    /* 0045b1de */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0045b1e1:;
    /* 0045b1e1 */
    a=f->r[3];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 0045b1e3 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 0045b1e5 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 0045b1e9 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0045b216;
mi_0045b1eb:;
    /* 0045b1eb */
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
    /* 0045b1ef */
    f->r[6]=MI_U32((0x0000001cu+f->r[4]));
    /* 0045b1f3 */
    f->r[1]=MI_U32((0x0078d790u));
    /* 0045b1f9 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0045b1fa */
    a=f->r[6];b=0x00000108u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0045b200 */
    a=f->r[5];b=0x00000108u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 0045b206 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0045b208 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0045b20c */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[6]);
    /* 0045b210 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0045b01c;
mi_0045b216:;
    /* 0045b216 */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
mi_0045b21a:;
    /* 0045b21a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0045b21b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0045b21c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0045b21d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0045b21e */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0045b221 */
    f->r[4]+=4;return;
}
void restore_sidebar_C_0045e220(struct restore_sidebar_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0045e220 */
    f->r[2]=MI_U32((0x00000004u+f->r[4]));
    /* 0045e224 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0045e226 */
    f->r[1]=f->r[2];
    /* 0045e228 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045e229 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0045e22c */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0045e22e */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045e22f */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0045e232 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045e233 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045e234 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00799a99u+f->r[1])))&0xffu)<<0);
    /* 0045e23a */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 0045e23c */
    f->r[6]=f->r[0];
    /* 0045e23e */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0045e24a;
    /* 0045e240 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0045e241 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0045e242 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0045e243 */
    f->r[0]=0x00000001u;
    /* 0045e248 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0045e249 */
    f->r[4]+=4;return;
mi_0045e24a:;
    /* 0045e24a */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x007999e8u+f->r[1])))&0xffffu)<<0);
    /* 0045e251 */
    a=(f->r[0]&0xffffu);b=0x00000047u;t=mi_alu(f,a,b,16,1);
    /* 0045e255 */
    if((f->flags&MI_ZF))goto mi_0045e25d;
    /* 0045e257 */
    a=(f->r[0]&0xffffu);b=0x000000bfu;t=mi_alu(f,a,b,16,1);
    /* 0045e25b */
    if(!(f->flags&MI_ZF))goto mi_0045e2ba;
mi_0045e25d:;
    /* 0045e25d */
    f->r[7]=MI_U32((0x00000018u+f->r[4]));
    /* 0045e261 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 0045e263 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0045e2ba;
    /* 0045e265 */
    a=f->r[7];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 0045e268 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0045e2ba;
    /* 0045e26a */
    f->r[5]=(f->r[5]&0xffff0000u)|(((MI_U16((0x00799a2eu+f->r[1])))&0xffffu)<<0);
    /* 0045e271 */
    a=(f->r[5]&0xffffu);b=(f->r[5]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 0045e274 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0045e2ba;
    /* 0045e276 */
    f->r[0]=(0x00000000u+f->r[6]+f->r[6]*2u);
    /* 0045e279 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 0045e27b */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0045e27e */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0045e280 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((MI_U8((0x007900c2u+f->r[7]+f->r[0]*4u)))&0xffu)<<0);
    /* 0045e287 */
    f->r[0]=f->r[3];
    /* 0045e289 */
    a=f->r[0];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0045e28c */
    if((f->flags&MI_ZF))goto mi_0045e2c1;
    /* 0045e28e */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0045e291 */
    if(!(f->flags&MI_ZF))goto mi_0045e2e0;
    /* 0045e293 */
    f->r[0]=f->r[2];
    /* 0045e295 */
    f->r[3]=(0x00000000u+f->r[6]+f->r[6]*2u);
    /* 0045e298 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0045e29b */
    a=f->r[3];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[3]=t;
    /* 0045e29e */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0045e2a0 */
    a=f->r[3];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 0045e2a2 */
    f->r[6]=(0x00000000u+f->r[7]+f->r[3]*2u);
    /* 0045e2a5 */
    f->r[0]=(0x00000000u+f->r[7]+f->r[0]*4u);
    /* 0045e2a8 */
    f->r[6]=(f->r[6]&0xffff0000u)|(((MI_U16((0x00790134u+f->r[6]*2u)))&0xffffu)<<0);
mi_0045e2b0:;
    /* 0045e2b0 */
    a=(f->r[6]&0xffffu);b=MI_U16((0x00799a2eu+f->r[0]*2u));t=mi_alu(f,a,b,16,1);
    /* 0045e2b8 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0045e2e0;
mi_0045e2ba:;
    /* 0045e2ba */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0045e2bb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0045e2bc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0045e2bd */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0045e2bf */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0045e2c0 */
    f->r[4]+=4;return;
mi_0045e2c1:;
    /* 0045e2c1 */
    f->r[0]=f->r[2];
    /* 0045e2c3 */
    f->r[3]=(0x00000000u+f->r[6]+f->r[6]*2u);
    /* 0045e2c6 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0045e2c9 */
    a=f->r[3];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[3]=t;
    /* 0045e2cc */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0045e2ce */
    a=f->r[3];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 0045e2d0 */
    f->r[6]=(0x00000000u+f->r[7]+f->r[3]*2u);
    /* 0045e2d3 */
    f->r[0]=(0x00000000u+f->r[7]+f->r[0]*4u);
    /* 0045e2d6 */
    f->r[6]=(f->r[6]&0xffff0000u)|(((MI_U16((0x007900ecu+f->r[6]*2u)))&0xffffu)<<0);
    /* 0045e2de */
    goto mi_0045e2b0;
mi_0045e2e0:;
    /* 0045e2e0 */
    f->r[0]=MI_U32((0x0000001cu+f->r[4]));
    /* 0045e2e4 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0045e2e6 */
    f->r[0]=MI_U32((0x00000020u+f->r[4]));
    /* 0045e2ea */
    if((f->flags&MI_ZF))goto mi_0045e308;
    /* 0045e2ec */
    f->r[6]=MI_U32((0x00eaac28u));
    /* 0045e2f2 */
    a=f->r[6];b=0x000049d8u;t=mi_imul(f,a,b);f->r[6]=t;
    /* 0045e2f8 */
    f->r[3]=MI_U32((0x00ea5e54u+f->r[6]));
    /* 0045e2fe */
    f->r[6]=(0x00ea5e54u+f->r[6]);
    /* 0045e304 */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 0045e306 */
    MI_U32((0x00000000u+f->r[6]))=(uint32_t)(f->r[3]);
mi_0045e308:;
    /* 0045e308 */
    f->r[6]=(uint32_t)(int32_t)(int16_t)(f->r[5]&0xffffu);
    /* 0045e30b */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0045e30d */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0045e32d;
    /* 0045e30f */
    f->r[6]=f->r[2];
    /* 0045e311 */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0045e314 */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0045e316 */
    f->r[2]=(0x00000000u+f->r[7]+f->r[6]*4u);
    /* 0045e319 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0045e31a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0045e31b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0045e31c */
    a=MI_U16((0x00799a2eu+f->r[2]*2u));b=(f->r[0]&0xffffu);t=mi_alu(f,a,b,16,0);MI_U16((0x00799a2eu+f->r[2]*2u))=(uint16_t)(t);
    /* 0045e324 */
    a=MI_U16((0x00799a2eu+f->r[1]));b=(f->r[0]&0xffffu);t=mi_alu(f,a,b,16,1);MI_U16((0x00799a2eu+f->r[1]))=(uint16_t)(t);
    /* 0045e32b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0045e32c */
    f->r[4]+=4;return;
mi_0045e32d:;
    /* 0045e32d */
    f->r[0]=f->r[2];
    /* 0045e32f */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0045e332 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0045e334 */
    f->r[0]=(0x00000000u+f->r[7]+f->r[0]*4u);
    /* 0045e337 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0045e338 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0045e339 */
    a=MI_U16((0x00799a2eu+f->r[0]*2u));b=(f->r[5]&0xffffu);t=mi_alu(f,a,b,16,0);MI_U16((0x00799a2eu+f->r[0]*2u))=(uint16_t)(t);
    /* 0045e341 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0045e342 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00799a2eu+f->r[1]));
    /* 0045e349 */
    MI_U16((0x00799a2eu+f->r[1]))=(uint16_t)(0x00000000u);
    /* 0045e352 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0045e353 */
    f->r[4]+=4;return;
}
void restore_sidebar_C_0046b320(struct restore_sidebar_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0046b320 */
    f->r[0]=0x00790180u;
mi_0046b325:;
    /* 0046b325 */
    MI_U16((0x00000000u+f->r[0]))=(uint16_t)(0x00000000u);
    /* 0046b32a */
    a=f->r[0];b=0x000000c4u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0046b32f */
    a=f->r[0];b=0x007999dcu;t=mi_alu(f,a,b,32,1);
    /* 0046b334 */
    if(mi_signed_less(f->flags))goto mi_0046b325;
    /* 0046b336 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046b337 */
    f->r[6]=MI_U32((0x0078d790u));
    /* 0046b33d */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0046b340 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046b341 */
    if(mi_signed_less(f->flags))goto mi_0046b3a0;
    /* 0046b343 */
    f->r[1]=0x00799af0u;
    /* 0046b348 */
    f->r[2]=f->r[6];
mi_0046b34a:;
    /* 0046b34a */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0xfffffff0u+f->r[1])))&0xffu)<<0);
    /* 0046b34d */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0046b34f */
    if((f->flags&MI_ZF))goto mi_0046b397;
    /* 0046b351 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00000000u+f->r[1])))&0xffffu)<<0);
    /* 0046b354 */
    a=(f->r[0]&0xffffu);b=0x00000047u;t=mi_alu(f,a,b,16,1);
    /* 0046b358 */
    if((f->flags&MI_ZF))goto mi_0046b378;
    /* 0046b35a */
    a=(f->r[0]&0xffffu);b=0x00000048u;t=mi_alu(f,a,b,16,1);
    /* 0046b35e */
    if((f->flags&MI_ZF))goto mi_0046b378;
    /* 0046b360 */
    a=(f->r[0]&0xffffu);b=0x00000046u;t=mi_alu(f,a,b,16,1);
    /* 0046b364 */
    if((f->flags&MI_ZF))goto mi_0046b378;
    /* 0046b366 */
    a=(f->r[0]&0xffffu);b=0x000000bfu;t=mi_alu(f,a,b,16,1);
    /* 0046b36a */
    if((f->flags&MI_ZF))goto mi_0046b378;
    /* 0046b36c */
    a=(f->r[0]&0xffffu);b=0x000000c1u;t=mi_alu(f,a,b,16,1);
    /* 0046b370 */
    if((f->flags&MI_ZF))goto mi_0046b378;
    /* 0046b372 */
    a=(f->r[0]&0xffffu);b=0x000000beu;t=mi_alu(f,a,b,16,1);
    /* 0046b376 */
    if(!(f->flags&MI_ZF))goto mi_0046b397;
mi_0046b378:;
    /* 0046b378 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x000000b1u+f->r[1])))&0xffu)<<0);
    /* 0046b37e */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0046b380 */
    if((f->flags&MI_ZF))goto mi_0046b397;
    /* 0046b382 */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 0046b387 */
    f->r[7]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0046b38a */
    a=f->r[7];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 0046b38d */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0046b38f */
    MI_U8((0x007900c0u+f->r[7]*4u))=(uint8_t)(0x00000001u);
mi_0046b397:;
    /* 0046b397 */
    a=f->r[1];b=0x00000108u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046b39d */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0046b39e */
    if(!(f->flags&MI_ZF))goto mi_0046b34a;
mi_0046b3a0:;
    /* 0046b3a0 */
    f->r[7]=0x00000001u;
    /* 0046b3a5 */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 0046b3a7 */
    if(mi_signed_less(f->flags))goto mi_0046b442;
    /* 0046b3ad */
    f->r[6]=0x00799af0u;
mi_0046b3b2:;
    /* 0046b3b2 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0xfffffff0u+f->r[6])))&0xffu)<<0);
    /* 0046b3b5 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0046b3b7 */
    if((f->flags&MI_ZF))goto mi_0046b42e;
    /* 0046b3b9 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00000000u+f->r[6])))&0xffffu)<<0);
    /* 0046b3bc */
    a=(f->r[0]&0xffffu);b=0x00000047u;t=mi_alu(f,a,b,16,1);
    /* 0046b3c0 */
    if((f->flags&MI_ZF))goto mi_0046b3e0;
    /* 0046b3c2 */
    a=(f->r[0]&0xffffu);b=0x00000048u;t=mi_alu(f,a,b,16,1);
    /* 0046b3c6 */
    if((f->flags&MI_ZF))goto mi_0046b3e0;
    /* 0046b3c8 */
    a=(f->r[0]&0xffffu);b=0x00000046u;t=mi_alu(f,a,b,16,1);
    /* 0046b3cc */
    if((f->flags&MI_ZF))goto mi_0046b3e0;
    /* 0046b3ce */
    a=(f->r[0]&0xffffu);b=0x000000bfu;t=mi_alu(f,a,b,16,1);
    /* 0046b3d2 */
    if((f->flags&MI_ZF))goto mi_0046b3e0;
    /* 0046b3d4 */
    a=(f->r[0]&0xffffu);b=0x000000c1u;t=mi_alu(f,a,b,16,1);
    /* 0046b3d8 */
    if((f->flags&MI_ZF))goto mi_0046b3e0;
    /* 0046b3da */
    a=(f->r[0]&0xffffu);b=0x000000beu;t=mi_alu(f,a,b,16,1);
    /* 0046b3de */
    if(!(f->flags&MI_ZF))goto mi_0046b42e;
mi_0046b3e0:;
    /* 0046b3e0 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x000000b1u+f->r[6])))&0xffu)<<0);
    /* 0046b3e6 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0046b3e8 */
    if(!(f->flags&MI_ZF))goto mi_0046b3fb;
    /* 0046b3ea */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046b3eb */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046b3f0u;f->boundary(0x0046b3ebu,0x0046b100u,f);
    /* 0046b3f0 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046b3f3 */
    MI_U8((0x000000b1u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 0046b3f9 */
    goto mi_0046b42e;
mi_0046b3fb:;
    /* 0046b3fb */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 0046b400 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0046b403 */
    a=f->r[1];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0046b406 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046b408 */
    a=MI_U16((0x007900bcu+f->r[1]*4u));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 0046b411 */
    f->r[0]=(0x007900bcu+f->r[1]*4u);
    /* 0046b418 */
    if((f->flags&MI_ZF))goto mi_0046b42b;
    /* 0046b41a */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046b41b */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046b420u;f->boundary(0x0046b41bu,0x0046b100u,f);
    /* 0046b420 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046b423 */
    MI_U8((0x000000b1u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 0046b429 */
    goto mi_0046b42e;
mi_0046b42b:;
    /* 0046b42b */
    MI_U16((0x00000000u+f->r[0]))=(uint16_t)((f->r[7]&0xffffu));
mi_0046b42e:;
    /* 0046b42e */
    f->r[0]=MI_U32((0x0078d790u));
    /* 0046b433 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 0046b434 */
    a=f->r[6];b=0x00000108u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0046b43a */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0046b43c */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0046b3b2;
mi_0046b442:;
    /* 0046b442 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0046b443 */
    MI_U32((0x0078d788u))=(uint32_t)(0x00000000u);
    /* 0046b44d */
    f->r[1]=0x00000001u;
    /* 0046b452 */
    f->r[0]=0x00790184u;
    /* 0046b457 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
mi_0046b458:;
    /* 0046b458 */
    a=MI_U8((0x00000000u+f->r[0]));b=0x00000000u;t=mi_alu(f,a,b,8,1);
    /* 0046b45b */
    if((f->flags&MI_ZF))goto mi_0046b463;
    /* 0046b45d */
    MI_U32((0x0078d788u))=(uint32_t)(f->r[1]);
mi_0046b463:;
    /* 0046b463 */
    a=f->r[0];b=0x000000c4u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0046b468 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0046b469 */
    a=f->r[0];b=0x007999e0u;t=mi_alu(f,a,b,32,1);
    /* 0046b46e */
    if(mi_signed_less(f->flags))goto mi_0046b458;
    /* 0046b470 */
    f->r[4]+=4;return;
}
void restore_sidebar_C_0046f030(struct restore_sidebar_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0046f030 */
    f->r[0]=0x00eab124u;
mi_0046f035:;
    /* 0046f035 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(0x00000000u);
    /* 0046f03b */
    a=f->r[0];b=0x000000e0u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0046f040 */
    a=f->r[0];b=0x00eadc04u;t=mi_alu(f,a,b,32,1);
    /* 0046f045 */
    if(mi_signed_less(f->flags))goto mi_0046f035;
    /* 0046f047 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046f048 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046f049 */
    f->r[6]=MI_U32((0x0078d790u));
    /* 0046f04f */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046f050 */
    f->r[7]=0x00000001u;
    /* 0046f055 */
    f->r[3]=0x00000088u;
    /* 0046f05a */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 0046f05c */
    if(mi_signed_less(f->flags))goto mi_0046f113;
    /* 0046f062 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046f063 */
    f->r[1]=0x00799baau;
    /* 0046f068 */
    f->r[2]=f->r[6];
mi_0046f06a:;
    /* 0046f06a */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0xffffff36u+f->r[1])))&0xffu)<<0);
    /* 0046f070 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0046f072 */
    if((f->flags&MI_ZF))goto mi_0046f09a;
    /* 0046f074 */
    a=MI_U16((0xffffff46u+f->r[1]));b=(f->r[3]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 0046f07b */
    if(!(f->flags&MI_ZF))goto mi_0046f09a;
    /* 0046f07d */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000000u+f->r[1])))&0xffu)<<0);
    /* 0046f07f */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0046f081 */
    if((f->flags&MI_ZF))goto mi_0046f09a;
    /* 0046f083 */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 0046f088 */
    f->r[5]=(0x00000000u+f->r[0]*8u);
    /* 0046f08f */
    a=f->r[5];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[5]=t;
    /* 0046f091 */
    a=f->r[5];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[5]=t;
    /* 0046f094 */
    MI_U32((0x00eaaf68u+f->r[5]))=(uint32_t)(f->r[7]);
mi_0046f09a:;
    /* 0046f09a */
    a=f->r[1];b=0x00000108u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046f0a0 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0046f0a1 */
    if(!(f->flags&MI_ZF))goto mi_0046f06a;
    /* 0046f0a3 */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 0046f0a5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0046f0a6 */
    if(mi_signed_less(f->flags))goto mi_0046f113;
    /* 0046f0a8 */
    f->r[6]=0x00799baau;
mi_0046f0ad:;
    /* 0046f0ad */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0xffffff36u+f->r[6])))&0xffu)<<0);
    /* 0046f0b3 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0046f0b5 */
    if((f->flags&MI_ZF))goto mi_0046f103;
    /* 0046f0b7 */
    a=MI_U16((0xffffff46u+f->r[6]));b=(f->r[3]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 0046f0be */
    if(!(f->flags&MI_ZF))goto mi_0046f103;
    /* 0046f0c0 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000000u+f->r[6])))&0xffu)<<0);
    /* 0046f0c2 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0046f0c4 */
    if(!(f->flags&MI_ZF))goto mi_0046f0d3;
    /* 0046f0c6 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046f0c7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046f0ccu;f->boundary(0x0046f0c7u,0x0046efd0u,f);
    /* 0046f0cc */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046f0cf */
    MI_U8((0x00000000u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 0046f0d1 */
    goto mi_0046f103;
mi_0046f0d3:;
    /* 0046f0d3 */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 0046f0d8 */
    f->r[1]=(0x00000000u+f->r[0]*8u);
    /* 0046f0df */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0046f0e1 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0046f0e4 */
    f->r[0]=(0x00eab044u+f->r[1]);
    /* 0046f0ea */
    f->r[1]=MI_U32((0x00eab044u+f->r[1]));
    /* 0046f0f0 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0046f0f2 */
    if((f->flags&MI_ZF))goto mi_0046f101;
    /* 0046f0f4 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046f0f5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046f0fau;f->boundary(0x0046f0f5u,0x0046efd0u,f);
    /* 0046f0fa */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046f0fd */
    MI_U8((0x00000000u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 0046f0ff */
    goto mi_0046f103;
mi_0046f101:;
    /* 0046f101 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[7]);
mi_0046f103:;
    /* 0046f103 */
    f->r[0]=MI_U32((0x0078d790u));
    /* 0046f108 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 0046f109 */
    a=f->r[6];b=0x00000108u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0046f10f */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0046f111 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0046f0ad;
mi_0046f113:;
    /* 0046f113 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0046f114 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046f115 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0046f116 */
    f->r[4]+=4;return;
}
void restore_sidebar_C_0046a320(struct restore_sidebar_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0046a320 */
    f->r[0]=0x009783d8u;
mi_0046a325:;
    /* 0046a325 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(0x00000000u);
    /* 0046a32b */
    a=f->r[0];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0046a32e */
    a=f->r[0];b=0x009784f0u;t=mi_alu(f,a,b,32,1);
    /* 0046a333 */
    if(mi_signed_less(f->flags))goto mi_0046a325;
    /* 0046a335 */
    f->r[2]=MI_U32((0x0078d790u));
    /* 0046a33b */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046a33c */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0046a33f */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046a340 */
    f->r[5]=0x0000004au;
    /* 0046a345 */
    if(mi_signed_less(f->flags))goto mi_0046a38a;
    /* 0046a347 */
    f->r[1]=0x00799babu;
mi_0046a34c:;
    /* 0046a34c */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0xffffff35u+f->r[1])))&0xffu)<<0);
    /* 0046a352 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0046a354 */
    if((f->flags&MI_ZF))goto mi_0046a37b;
    /* 0046a356 */
    a=MI_U16((0xffffff45u+f->r[1]));b=(f->r[5]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 0046a35d */
    if(!(f->flags&MI_ZF))goto mi_0046a37b;
    /* 0046a35f */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000000u+f->r[1])))&0xffu)<<0);
    /* 0046a361 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0046a363 */
    if((f->flags&MI_ZF))goto mi_0046a37b;
    /* 0046a365 */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 0046a36a */
    f->r[6]=(0x00000000u+f->r[0]*8u);
    /* 0046a371 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 0046a373 */
    MI_U8((0x009783a4u+f->r[6]*4u))=(uint8_t)(0x00000001u);
mi_0046a37b:;
    /* 0046a37b */
    a=f->r[1];b=0x00000108u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046a381 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0046a382 */
    if(!(f->flags&MI_ZF))goto mi_0046a34c;
    /* 0046a384 */
    f->r[2]=MI_U32((0x0078d790u));
mi_0046a38a:;
    /* 0046a38a */
    f->r[6]=0x00000001u;
    /* 0046a38f */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 0046a391 */
    if(mi_signed_less(f->flags))goto mi_0046a3fb;
    /* 0046a393 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046a394 */
    f->r[7]=0x00799babu;
mi_0046a399:;
    /* 0046a399 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0xffffff35u+f->r[7])))&0xffu)<<0);
    /* 0046a39f */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0046a3a1 */
    if((f->flags&MI_ZF))goto mi_0046a3ea;
    /* 0046a3a3 */
    a=MI_U16((0xffffff45u+f->r[7]));b=(f->r[5]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 0046a3aa */
    if(!(f->flags&MI_ZF))goto mi_0046a3ea;
    /* 0046a3ac */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000000u+f->r[7])))&0xffu)<<0);
    /* 0046a3ae */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0046a3b0 */
    if(!(f->flags&MI_ZF))goto mi_0046a3bd;
    /* 0046a3b2 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046a3b3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046a3b8u;f->boundary(0x0046a3b3u,0x0046a280u,f);
    /* 0046a3b8 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046a3bb */
    goto mi_0046a3ea;
mi_0046a3bd:;
    /* 0046a3bd */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 0046a3c2 */
    f->r[1]=(0x00000000u+f->r[0]*8u);
    /* 0046a3c9 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0046a3cb */
    f->r[0]=(0x009783bcu+f->r[1]*4u);
    /* 0046a3d2 */
    f->r[1]=MI_U32((0x009783bcu+f->r[1]*4u));
    /* 0046a3d9 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0046a3db */
    if((f->flags&MI_ZF))goto mi_0046a3e8;
    /* 0046a3dd */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046a3de */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046a3e3u;f->boundary(0x0046a3deu,0x0046a280u,f);
    /* 0046a3e3 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046a3e6 */
    goto mi_0046a3ea;
mi_0046a3e8:;
    /* 0046a3e8 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[6]);
mi_0046a3ea:;
    /* 0046a3ea */
    f->r[0]=MI_U32((0x0078d790u));
    /* 0046a3ef */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0046a3f0 */
    a=f->r[7];b=0x00000108u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0046a3f6 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0046a3f8 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0046a399;
    /* 0046a3fa */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
mi_0046a3fb:;
    /* 0046a3fb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046a3fc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0046a3fd */
    f->r[4]+=4;return;
}
void restore_sidebar_C_004e0c80(struct restore_sidebar_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004e0c80 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e0c81 */
    f->r[7]=MI_U32((0x00000008u+f->r[4]));
    /* 004e0c85 */
    f->r[0]=f->r[7];
    /* 004e0c87 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004e0c8a */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004e0c8c */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004e0c8f */
    a=MI_U16((0x007999e8u+f->r[0]));b=0x000000deu;t=mi_alu(f,a,b,16,1);
    /* 004e0c98 */
    if(!(f->flags&MI_ZF))goto mi_004e0cb9;
    /* 004e0c9a */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999eau+f->r[0]));
    /* 004e0ca1 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e0ca2 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 004e0ca4 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
mi_004e0ca6:;
    /* 004e0ca6 */
    f->r[1]=(0x00000000u+f->r[6]+f->r[7]*1u);
    /* 004e0ca9 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e0caa */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e0cafu;f->boundary(0x004e0caau,0x004dfbb0u,f);
    /* 004e0caf */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e0cb2 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 004e0cb3 */
    a=f->r[6];b=0x00000074u;t=mi_alu(f,a,b,32,1);
    /* 004e0cb6 */
    if(mi_signed_less(f->flags))goto mi_004e0ca6;
    /* 004e0cb8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
mi_004e0cb9:;
    /* 004e0cb9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004e0cba */
    f->r[4]+=4;return;
}
void restore_sidebar_C_0046a280(struct restore_sidebar_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0046a280 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0046a282 */
    f->r[0]=0x009783c0u;
mi_0046a287:;
    /* 0046a287 */
    a=MI_U8((0x00000000u+f->r[0]));b=0x00000000u;t=mi_alu(f,a,b,8,1);
    /* 0046a28a */
    if((f->flags&MI_ZF))goto mi_0046a2a3;
    /* 0046a28c */
    a=f->r[0];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0046a28f */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0046a290 */
    a=f->r[0];b=0x009784d8u;t=mi_alu(f,a,b,32,1);
    /* 0046a295 */
    if(mi_signed_less(f->flags))goto mi_0046a287;
    /* 0046a297 */
    a=0x005dbba0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046a29c */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046a2a2u;f->boundary(0x0046a29cu,0x0056f0f8u,f);
    /* 0046a2a2 */
    f->r[4]+=4;return;
mi_0046a2a3:;
    /* 0046a2a3 */
    f->r[2]=MI_U32((0x00000004u+f->r[4]));
    /* 0046a2a7 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046a2a8 */
    f->r[6]=f->r[2];
    /* 0046a2aa */
    f->r[0]=(f->r[0]&0xffffff00u)|((((f->r[1]&0xffu))&0xffu)<<0);
    /* 0046a2ac */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0046a2af */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0046a2b1 */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0046a2b3 */
    MI_U8((0x00799aa3u+f->r[6]*8u))=(uint8_t)((f->r[0]&0xffu));
    /* 0046a2ba */
    f->r[0]=(0x00000000u+f->r[1]*8u);
    /* 0046a2c1 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0046a2c3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046a2c4 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0046a2c7 */
    MI_U8((0x009783c0u+f->r[0]))=(uint8_t)(0x00000001u);
    /* 0046a2ce */
    MI_U32((0x009783d8u+f->r[0]))=(uint32_t)(f->r[2]);
    /* 0046a2d4 */
    f->r[4]+=4;return;
}
