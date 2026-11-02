/* Whole4728A0:2160 authenticated native instructions. Integer C frame;
 * real typed providers below, named fail-fast for remaining open algorithms.
 * This source does not claim the whole construction graph is closed. */
#include "map_commit_controller.h"
#include "drag_leaves.h"
#include "editor_brush_validation.h"
#include "map_commit_state.h"
#include "map_commit_warnings.h"
#include "map_commit_costs.h"
#include "map_commit_selection.h"
#include "map_commit_connect.h"
#include "map_commit_proximity.h"
#include "map_commit_resource_warnings.h"
#include "map_commit_resource_gate.h"
#include "map_commit_town_warning.h"
#include "menu_prepare_integer.h"
#include "terrain_redraw.h"
#include "footprint.h"
#include "actor_predicate.h"
#include "region_validation.h"
#include "animation_state.h"
#include "pending_message.h"
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
static __inline uint32_t mi_alu(struct restore_map_commit_frame *f,uint32_t a,uint32_t b,uint32_t width,uint32_t operation)
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
static __inline uint32_t mi_shift(struct restore_map_commit_frame *f,uint32_t a,uint32_t count,uint32_t width,uint32_t arithmetic)
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
static __inline uint32_t mi_imul(struct restore_map_commit_frame *f,uint32_t a,uint32_t b)
{
 int64_t product=(int64_t)(int32_t)a*(int32_t)b;uint32_t result=(uint32_t)product;
 f->flags&=~(MI_CF|MI_OF);if(product!=(int64_t)(int32_t)result)f->flags|=MI_CF|MI_OF;
 return result;
}
static __inline int mi_signed_less(uint32_t flags){return ((flags&MI_SF)!=0)^((flags&MI_OF)!=0);}

void restore_map_commit_C_004728a0(struct restore_map_commit_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004728a0 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 004728a3 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004728a4 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004728a5 */
    f->r[5]=MI_U32((0x0000002cu+f->r[4]));
    /* 004728a9 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004728aa */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004728ab */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 004728ad */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004728ae */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 004728b2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004728b7u;f->boundary(0x004728b2u,0x0046fde0u,f);
    /* 004728b7 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004728ba */
    a=f->r[5];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004728bc */
    MI_U8((0x00000014u+f->r[4]))=(uint8_t)((f->r[0]&0xffu));
    /* 004728c0 */
    MI_U32((0x00e94e20u))=(uint32_t)(f->r[6]);
    /* 004728c6 */
    MI_U32((0x00bc9728u))=(uint32_t)(f->r[6]);
    /* 004728cc */
    MI_U32((0x00bc9718u))=(uint32_t)(f->r[6]);
    /* 004728d2 */
    if((f->flags&MI_ZF))goto mi_00474119;
    /* 004728d8 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 004728dd */
    f->r[1]=f->r[0];
    /* 004728df */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 004728e5 */
    a=MI_U32((0x00ea187cu+f->r[1]));b=0xffffec78u;t=mi_alu(f,a,b,32,1);
    /* 004728ef */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00472919;
    /* 004728f1 */
    a=0x000000afu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004728f6 */
    a=0x0000cb10u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004728fb */
    a=0x00b285ccu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472900 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00472905u;f->boundary(0x00472900u,0x0040f920u,f);
    /* 00472905 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472906 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472907 */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472909 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047290eu;f->boundary(0x00472909u,0x004e2970u,f);
    /* 0047290e */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00472911 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00472912 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00472913 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00472914 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00472915 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00472918 */
    f->r[4]+=4;return;
mi_00472919:;
    /* 00472919 */
    f->r[6]=MI_U32((0x00000030u+f->r[4]));
    /* 0047291d */
    f->r[7]=MI_U32((0x0000002cu+f->r[4]));
    /* 00472921 */
    f->r[3]=MI_U32((0x00000028u+f->r[4]));
    /* 00472925 */
    a=f->r[5];b=0x00000009u;t=mi_alu(f,a,b,32,1);
    /* 00472928 */
    if((f->flags&MI_ZF))goto mi_00472a97;
    /* 0047292e */
    f->r[2]=MI_U32((0x00000024u+f->r[4]));
    /* 00472932 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472933 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472934 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472935 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472936 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047293bu;f->boundary(0x00472936u,0x004b5830u,f);
    /* 0047293b */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047293e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00472940 */
    if((f->flags&MI_ZF))goto mi_00472a92;
    /* 00472946 */
    a=f->r[5];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 00472949 */
    if((f->flags&MI_ZF))goto mi_00472a4a;
    /* 0047294f */
    a=f->r[5];b=0x000000a8u;t=mi_alu(f,a,b,32,1);
    /* 00472955 */
    if((f->flags&MI_ZF))goto mi_00472a4a;
    /* 0047295b */
    a=f->r[5];b=0x000000a9u;t=mi_alu(f,a,b,32,1);
    /* 00472961 */
    if((f->flags&MI_ZF))goto mi_00472a4a;
    /* 00472967 */
    a=f->r[5];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 0047296a */
    if((f->flags&MI_ZF))goto mi_00472a4a;
    /* 00472970 */
    a=f->r[5];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 00472973 */
    if((f->flags&MI_ZF))goto mi_00472a4a;
    /* 00472979 */
    a=f->r[5];b=0x00000026u;t=mi_alu(f,a,b,32,1);
    /* 0047297c */
    if((f->flags&MI_ZF))goto mi_004729da;
    /* 0047297e */
    a=f->r[5];b=0x00000027u;t=mi_alu(f,a,b,32,1);
    /* 00472981 */
    if((f->flags&MI_ZF))goto mi_004729da;
    /* 00472983 */
    a=f->r[5];b=0x00000052u;t=mi_alu(f,a,b,32,1);
    /* 00472986 */
    if((f->flags&MI_ZF))goto mi_004729ba;
    /* 00472988 */
    a=f->r[5];b=0x00000053u;t=mi_alu(f,a,b,32,1);
    /* 0047298b */
    if((f->flags&MI_ZF))goto mi_004729ba;
    /* 0047298d */
    a=0x000000afu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472992 */
    a=0x0000cb10u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472997 */
    a=0x00b285ccu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047299c */
    f->r[4]-=4;MI_U32(f->r[4])=0x004729a1u;f->boundary(0x0047299cu,0x0040f920u,f);
    /* 004729a1 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004729a4 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004729a6 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004729a8 */
    a=0x00000032u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004729aa */
    f->r[4]-=4;MI_U32(f->r[4])=0x004729afu;f->boundary(0x004729aau,0x004e2970u,f);
    /* 004729af */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004729b2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004729b3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004729b4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004729b5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004729b6 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004729b9 */
    f->r[4]+=4;return;
mi_004729ba:;
    /* 004729ba */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004729bc */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004729be */
    a=0x00000032u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004729c0 */
    MI_U32((0x00bc96f8u))=(uint32_t)(0x00000000u);
    /* 004729ca */
    f->r[4]-=4;MI_U32(f->r[4])=0x004729cfu;f->boundary(0x004729cau,0x004e2970u,f);
    /* 004729cf */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004729d2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004729d3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004729d4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004729d5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004729d6 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004729d9 */
    f->r[4]+=4;return;
mi_004729da:;
    /* 004729da */
    a=0x00032c40u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004729df */
    a=0x00af598cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004729e4 */
    a=0x009de62cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004729e9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004729eeu;f->boundary(0x004729e9u,0x0040f860u,f);
    /* 004729ee */
    a=0x0000cb10u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004729f3 */
    a=0x00ae8e7cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004729f8 */
    a=0x009d1b1cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004729fd */
    f->r[4]-=4;MI_U32(f->r[4])=0x00472a02u;f->boundary(0x004729fdu,0x0040f860u,f);
    /* 00472a02 */
    a=0x0000cb10u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472a07 */
    a=0x00b285ccu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472a0c */
    a=0x009b84fcu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472a11 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00472a16u;f->boundary(0x00472a11u,0x0040f860u,f);
    /* 00472a16 */
    a=0x0000cb10u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472a1b */
    a=0x00b67d1cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472a20 */
    a=0x009ab9ecu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472a25 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00472a2au;f->boundary(0x00472a25u,0x0040f860u,f);
    /* 00472a2a */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472a2c */
    f->r[4]-=4;MI_U32(f->r[4])=0x00472a31u;f->boundary(0x00472a2cu,0x00485830u,f);
    /* 00472a31 */
    a=f->r[4];b=0x00000034u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00472a34 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472a36 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472a38 */
    a=0x00000032u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472a3a */
    f->r[4]-=4;MI_U32(f->r[4])=0x00472a3fu;f->boundary(0x00472a3au,0x004e2970u,f);
    /* 00472a3f */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00472a42 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00472a43 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00472a44 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00472a45 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00472a46 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00472a49 */
    f->r[4]+=4;return;
mi_00472a4a:;
    /* 00472a4a */
    a=0x00032c40u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472a4f */
    a=0x00af598cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472a54 */
    a=0x009de62cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472a59 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00472a5eu;f->boundary(0x00472a59u,0x0040f860u,f);
    /* 00472a5e */
    a=0x0000cb10u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472a63 */
    a=0x00ae8e7cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472a68 */
    a=0x009d1b1cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472a6d */
    f->r[4]-=4;MI_U32(f->r[4])=0x00472a72u;f->boundary(0x00472a6du,0x0040f860u,f);
    /* 00472a72 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472a74 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00472a79u;f->boundary(0x00472a74u,0x00485830u,f);
    /* 00472a79 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00472a7c */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472a7e */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472a80 */
    a=0x00000032u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472a82 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00472a87u;f->boundary(0x00472a82u,0x004e2970u,f);
    /* 00472a87 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00472a8a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00472a8b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00472a8c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00472a8d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00472a8e */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00472a91 */
    f->r[4]+=4;return;
mi_00472a92:;
    /* 00472a92 */
    f->r[0]=MI_U32((0x00eaac28u));
mi_00472a97:;
    /* 00472a97 */
    a=f->r[5];b=0x000000cfu;t=mi_alu(f,a,b,32,1);
    /* 00472a9d */
    if((f->flags&MI_ZF))goto mi_00472aa7;
    /* 00472a9f */
    a=f->r[5];b=0x000000d0u;t=mi_alu(f,a,b,32,1);
    /* 00472aa5 */
    if(!(f->flags&MI_ZF))goto mi_00472ac0;
mi_00472aa7:;
    /* 00472aa7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00472aacu;f->boundary(0x00472aa7u,0x004e59c0u,f);
    /* 00472aac */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00472aae */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00472abb;
    /* 00472ab0 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472ab2 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472ab4 */
    a=0x00000058u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472ab6 */
    goto mi_00473e4d;
mi_00472abb:;
    /* 00472abb */
    f->r[0]=MI_U32((0x00eaac28u));
mi_00472ac0:;
    /* 00472ac0 */
    f->r[1]=(0x00000000u+f->r[5]+f->r[5]*4u);
    /* 00472ac4 */
    a=f->r[5];b=0x00000009u;t=mi_alu(f,a,b,32,1);
    /* 00472ac7 */
    MI_U32((0x00bc9720u))=(uint32_t)(0x00000000u);
    /* 00472ad1 */
    f->r[2]=MI_U32((0x00705ea4u+f->r[1]*8u));
    /* 00472ad8 */
    MI_U32((0x00bc9728u))=(uint32_t)(f->r[2]);
    /* 00472ade */
    if(!(f->flags&MI_ZF))goto mi_00472b0a;
    /* 00472ae0 */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 00472ae4 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472ae5 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472ae6 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472ae7 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472ae8 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472aea */
    f->r[4]-=4;MI_U32(f->r[4])=0x00472aefu;f->boundary(0x00472aeau,0x004743f0u,f);
mi_00472aef:;
    /* 00472aef */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00472af2:;
    /* 00472af2 */
    f->r[1]=MI_U32((0x00bc971cu));
    /* 00472af8 */
    a=f->r[1];b=MI_U32((0x00bc9728u));t=mi_imul(f,a,b);f->r[1]=t;
    /* 00472aff */
    MI_U32((0x00bc9728u))=(uint32_t)(f->r[1]);
    /* 00472b05 */
    goto mi_004740b7;
mi_00472b0a:;
    /* 00472b0a */
    a=f->r[5];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 00472b0d */
    if(!(f->flags&MI_ZF))goto mi_00472b35;
    /* 00472b0f */
    f->r[2]=MI_U32((0x00000024u+f->r[4]));
    /* 00472b13 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472b14 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472b15 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472b16 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472b17 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00472b1cu;f->boundary(0x00472b17u,0x00470900u,f);
    /* 00472b1c */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00472b1f */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00472b21 */
    if((f->flags&MI_ZF))goto mi_00472b2e;
    /* 00472b23 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472b25 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472b27 */
    a=0x0000003du;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472b29 */
    goto mi_00473e4d;
mi_00472b2e:;
    /* 00472b2e */
    a=0x00004000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472b33 */
    goto mi_00472b8f;
mi_00472b35:;
    /* 00472b35 */
    a=f->r[5];b=0x000000a8u;t=mi_alu(f,a,b,32,1);
    /* 00472b3b */
    if(!(f->flags&MI_ZF))goto mi_00472b63;
    /* 00472b3d */
    f->r[2]=MI_U32((0x00000024u+f->r[4]));
    /* 00472b41 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472b42 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472b43 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472b44 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472b45 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00472b4au;f->boundary(0x00472b45u,0x00470900u,f);
    /* 00472b4a */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00472b4d */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00472b4f */
    if((f->flags&MI_ZF))goto mi_00472b5c;
    /* 00472b51 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472b53 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472b55 */
    a=0x0000003du;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472b57 */
    goto mi_00473e4d;
mi_00472b5c:;
    /* 00472b5c */
    a=0x00404000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472b61 */
    goto mi_00472b8f;
mi_00472b63:;
    /* 00472b63 */
    a=f->r[5];b=0x000000a9u;t=mi_alu(f,a,b,32,1);
    /* 00472b69 */
    if(!(f->flags&MI_ZF))goto mi_00472ba6;
    /* 00472b6b */
    f->r[2]=MI_U32((0x00000024u+f->r[4]));
    /* 00472b6f */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472b70 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472b71 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472b72 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472b73 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00472b78u;f->boundary(0x00472b73u,0x00470900u,f);
    /* 00472b78 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00472b7b */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00472b7d */
    if((f->flags&MI_ZF))goto mi_00472b8a;
    /* 00472b7f */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472b81 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472b83 */
    a=0x0000003du;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472b85 */
    goto mi_00473e4d;
mi_00472b8a:;
    /* 00472b8a */
    a=0x00804000u;f->r[4]-=4;MI_U32(f->r[4])=a;
mi_00472b8f:;
    /* 00472b8f */
    f->r[0]=MI_U32((0x00000028u+f->r[4]));
    /* 00472b93 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472b94 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472b95 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472b96 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472b97 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472b99 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00472b9eu;f->boundary(0x00472b99u,0x00475220u,f);
    /* 00472b9e */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00472ba1 */
    goto mi_00472af2;
mi_00472ba6:;
    /* 00472ba6 */
    a=f->r[5];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 00472ba9 */
    if(!(f->flags&MI_ZF))goto mi_00472bde;
    /* 00472bab */
    f->r[2]=MI_U32((0x00000024u+f->r[4]));
    /* 00472baf */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472bb0 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472bb1 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472bb2 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472bb3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00472bb8u;f->boundary(0x00472bb3u,0x00470900u,f);
    /* 00472bb8 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00472bbb */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00472bbd */
    if((f->flags&MI_ZF))goto mi_00472bca;
    /* 00472bbf */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472bc1 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472bc3 */
    a=0x0000003du;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472bc5 */
    goto mi_00473e4d;
mi_00472bca:;
    /* 00472bca */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 00472bce */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472bcf */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472bd0 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472bd1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472bd2 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472bd4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00472bd9u;f->boundary(0x00472bd4u,0x00474f50u,f);
    /* 00472bd9 */
    goto mi_00472aef;
mi_00472bde:;
    /* 00472bde */
    a=f->r[5];b=0x00000026u;t=mi_alu(f,a,b,32,1);
    /* 00472be1 */
    if(!(f->flags&MI_ZF))goto mi_00472c0b;
    /* 00472be3 */
    f->r[2]=MI_U32((0x00000024u+f->r[4]));
    /* 00472be7 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472be8 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472be9 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472bea */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472beb */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472bed */
    f->r[4]-=4;MI_U32(f->r[4])=0x00472bf2u;f->boundary(0x00472bedu,0x004754a0u,f);
    /* 00472bf2 */
    f->r[0]=MI_U32((0x00bc971cu));
    /* 00472bf7 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00472bfa */
    a=f->r[0];b=MI_U32((0x00bc9728u));t=mi_imul(f,a,b);f->r[0]=t;
    /* 00472c01 */
    MI_U32((0x00bc9728u))=(uint32_t)(f->r[0]);
    /* 00472c06 */
    goto mi_004740b7;
mi_00472c0b:;
    /* 00472c0b */
    a=f->r[5];b=0x0000001fu;t=mi_alu(f,a,b,32,1);
    /* 00472c0e */
    if(!(f->flags&MI_ZF))goto mi_00472c81;
    /* 00472c10 */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 00472c14 */
    f->r[1]=(0x00000001u+f->r[3]);
    /* 00472c17 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472c18 */
    f->r[2]=(0x00000001u+f->r[0]);
    /* 00472c1b */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472c1c */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472c1d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472c1e */
    f->r[4]-=4;MI_U32(f->r[4])=0x00472c23u;f->boundary(0x00472c1eu,0x00470900u,f);
    /* 00472c23 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00472c26 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00472c28 */
    if((f->flags&MI_ZF))goto mi_00472c35;
    /* 00472c2a */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472c2c */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472c2e */
    a=0x0000003du;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472c30 */
    goto mi_00473e4d;
mi_00472c35:;
    /* 00472c35 */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 00472c39 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472c3a */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472c3b */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472c3c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472c3d */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472c3f */
    f->r[4]-=4;MI_U32(f->r[4])=0x00472c44u;f->boundary(0x00472c3fu,0x00440700u,f);
    /* 00472c44 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00472c47 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00472c49 */
    if(!(f->flags&MI_ZF))goto mi_00472c75;
    /* 00472c4b */
    a=0x000000afu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472c50 */
    a=0x0000cb10u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472c55 */
    a=0x00b285ccu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472c5a */
    f->r[4]-=4;MI_U32(f->r[4])=0x00472c5fu;f->boundary(0x00472c5au,0x0040f920u,f);
    /* 00472c5f */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472c61 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472c63 */
    a=0x0000004du;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472c65 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00472c6au;f->boundary(0x00472c65u,0x004e2970u,f);
    /* 00472c6a */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00472c6d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00472c6e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00472c6f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00472c70 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00472c71 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00472c74 */
    f->r[4]+=4;return;
mi_00472c75:;
    /* 00472c75 */
    a=0x0000001fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472c77 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00472c7cu;f->boundary(0x00472c77u,0x004e5070u,f);
    /* 00472c7c */
    goto mi_004740b4;
mi_00472c81:;
    /* 00472c81 */
    a=f->r[5];b=0x0000001eu;t=mi_alu(f,a,b,32,1);
    /* 00472c84 */
    if(!(f->flags&MI_ZF))goto mi_00472ce1;
    /* 00472c86 */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 00472c8a */
    f->r[1]=(0x00000002u+f->r[3]);
    /* 00472c8d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472c8e */
    f->r[2]=(0x00000002u+f->r[0]);
    /* 00472c91 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472c92 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472c93 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472c94 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00472c99u;f->boundary(0x00472c94u,0x00470900u,f);
    /* 00472c99 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00472c9c */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00472c9e */
    if((f->flags&MI_ZF))goto mi_00472cab;
    /* 00472ca0 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472ca2 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472ca4 */
    a=0x0000003du;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472ca6 */
    goto mi_00473e4d;
mi_00472cab:;
    /* 00472cab */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 00472caf */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472cb0 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472cb1 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472cb2 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472cb3 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472cb5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00472cbau;f->boundary(0x00472cb5u,0x00440850u,f);
    /* 00472cba */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00472cbd */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00472cbf */
    if(!(f->flags&MI_ZF))goto mi_00472cd5;
    /* 00472cc1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472cc2 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472cc3 */
    a=0x0000004du;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472cc5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00472ccau;f->boundary(0x00472cc5u,0x004e2970u,f);
    /* 00472cca */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00472ccd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00472cce */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00472ccf */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00472cd0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00472cd1 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00472cd4 */
    f->r[4]+=4;return;
mi_00472cd5:;
    /* 00472cd5 */
    a=0x0000001eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472cd7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00472cdcu;f->boundary(0x00472cd7u,0x004e5070u,f);
    /* 00472cdc */
    goto mi_004740b4;
mi_00472ce1:;
    /* 00472ce1 */
    a=f->r[5];b=0x00000021u;t=mi_alu(f,a,b,32,1);
    /* 00472ce4 */
    if(!(f->flags&MI_ZF))goto mi_00472d41;
    /* 00472ce6 */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 00472cea */
    f->r[1]=(0x00000003u+f->r[3]);
    /* 00472ced */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472cee */
    f->r[2]=(0x00000003u+f->r[0]);
    /* 00472cf1 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472cf2 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472cf3 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472cf4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00472cf9u;f->boundary(0x00472cf4u,0x00470900u,f);
    /* 00472cf9 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00472cfc */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00472cfe */
    if((f->flags&MI_ZF))goto mi_00472d0b;
    /* 00472d00 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472d02 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472d04 */
    a=0x0000003du;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472d06 */
    goto mi_00473e4d;
mi_00472d0b:;
    /* 00472d0b */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 00472d0f */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472d10 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472d11 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472d12 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472d13 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472d15 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00472d1au;f->boundary(0x00472d15u,0x00440b10u,f);
    /* 00472d1a */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00472d1d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00472d1f */
    if(!(f->flags&MI_ZF))goto mi_00472d35;
    /* 00472d21 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472d22 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472d23 */
    a=0x0000004du;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472d25 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00472d2au;f->boundary(0x00472d25u,0x004e2970u,f);
    /* 00472d2a */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00472d2d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00472d2e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00472d2f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00472d30 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00472d31 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00472d34 */
    f->r[4]+=4;return;
mi_00472d35:;
    /* 00472d35 */
    a=0x00000021u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472d37 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00472d3cu;f->boundary(0x00472d37u,0x004e5070u,f);
    /* 00472d3c */
    goto mi_004740b4;
mi_00472d41:;
    /* 00472d41 */
    a=f->r[5];b=0x000000d1u;t=mi_alu(f,a,b,32,1);
    /* 00472d47 */
    if(!(f->flags&MI_ZF))goto mi_00472dc5;
    /* 00472d49 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 00472d4f */
    a=MI_U16((0x00ea6084u+f->r[0]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 00472d57 */
    if((f->flags&MI_ZF))goto mi_00472d6f;
    /* 00472d59 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472d5b */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472d5d */
    a=0x00000004u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472d5f */
    f->r[4]-=4;MI_U32(f->r[4])=0x00472d64u;f->boundary(0x00472d5fu,0x004e2970u,f);
    /* 00472d64 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00472d67 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00472d68 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00472d69 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00472d6a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00472d6b */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00472d6e */
    f->r[4]+=4;return;
mi_00472d6f:;
    /* 00472d6f */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 00472d73 */
    f->r[1]=(0x00000004u+f->r[3]);
    /* 00472d76 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472d77 */
    f->r[2]=(0x00000004u+f->r[0]);
    /* 00472d7a */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472d7b */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472d7c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472d7d */
    f->r[4]-=4;MI_U32(f->r[4])=0x00472d82u;f->boundary(0x00472d7du,0x00470900u,f);
    /* 00472d82 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00472d85 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00472d87 */
    if((f->flags&MI_ZF))goto mi_00472d94;
    /* 00472d89 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472d8b */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472d8d */
    a=0x0000003du;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472d8f */
    goto mi_00473e4d;
mi_00472d94:;
    /* 00472d94 */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 00472d98 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472d99 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472d9a */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472d9b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472d9c */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472d9e */
    f->r[4]-=4;MI_U32(f->r[4])=0x00472da3u;f->boundary(0x00472d9eu,0x00440ed0u,f);
    /* 00472da3 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00472da6 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00472da8 */
    if(!(f->flags&MI_ZF))goto mi_004740b7;
    /* 00472dae */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472daf */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472db0 */
    a=0x000000d7u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472db5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00472dbau;f->boundary(0x00472db5u,0x004e2970u,f);
    /* 00472dba */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00472dbd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00472dbe */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00472dbf */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00472dc0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00472dc1 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00472dc4 */
    f->r[4]+=4;return;
mi_00472dc5:;
    /* 00472dc5 */
    a=f->r[5];b=0x00000027u;t=mi_alu(f,a,b,32,1);
    /* 00472dc8 */
    if(!(f->flags&MI_ZF))goto mi_00472e4f;
    /* 00472dce */
    f->r[1]=MI_U32((0x00000024u+f->r[4]));
    /* 00472dd2 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472dd3 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472dd4 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472dd5 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472dd6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00472ddbu;f->boundary(0x00472dd6u,0x00470900u,f);
    /* 00472ddb */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00472dde */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00472de0 */
    if((f->flags&MI_ZF))goto mi_00472ded;
    /* 00472de2 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472de4 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472de6 */
    a=0x0000003du;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472de8 */
    goto mi_00473e4d;
mi_00472ded:;
    /* 00472ded */
    f->r[2]=MI_U32((0x00000024u+f->r[4]));
    /* 00472df1 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472df2 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472df3 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472df4 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472df5 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472df7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00472dfcu;f->boundary(0x00472df7u,0x004759e0u,f);
    /* 00472dfc */
    f->r[0]=MI_U32((0x00bc971cu));
    /* 00472e01 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472e03 */
    a=f->r[0];b=MI_U32((0x00bc9728u));t=mi_imul(f,a,b);f->r[0]=t;
    /* 00472e0a */
    MI_U32((0x00bc9728u))=(uint32_t)(f->r[0]);
    /* 00472e0f */
    f->r[0]=MI_U32((0x00bc96d0u));
    /* 00472e14 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[6]*1u);
    /* 00472e17 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[7]*1u);
    /* 00472e1a */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472e1b */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472e1c */
    f->r[1]=f->r[6];
    /* 00472e1e */
    f->r[2]=f->r[7];
    /* 00472e20 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00472e22 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00472e24 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472e25 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472e26 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00472e2bu;f->boundary(0x00472e26u,0x00516f90u,f);
    /* 00472e2b */
    f->r[0]=MI_U32((0x00bc96d0u));
    /* 00472e30 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[6]*1u);
    /* 00472e33 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[7]*1u);
    /* 00472e36 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472e37 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472e38 */
    f->r[1]=f->r[6];
    /* 00472e3a */
    f->r[2]=f->r[7];
    /* 00472e3c */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00472e3e */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00472e40 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472e41 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472e42 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00472e47u;f->boundary(0x00472e42u,0x005173a0u,f);
    /* 00472e47 */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00472e4a */
    goto mi_004740b7;
mi_00472e4f:;
    /* 00472e4f */
    a=f->r[5];b=0x00000052u;t=mi_alu(f,a,b,32,1);
    /* 00472e52 */
    if(!(f->flags&MI_ZF))goto mi_00472e7f;
    /* 00472e54 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
mi_00472e56:;
    /* 00472e56 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472e57 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472e58 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00472e5du;f->boundary(0x00472e58u,0x0047f530u,f);
    /* 00472e5d */
    f->r[0]=MI_U32((0x00bc96f8u));
    /* 00472e62 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00472e65 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 00472e68 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004738d5;
    /* 00472e6e */
    a=f->r[0];b=MI_U32((0x00bc9728u));t=mi_imul(f,a,b);f->r[0]=t;
    /* 00472e75 */
    MI_U32((0x00bc9728u))=(uint32_t)(f->r[0]);
    /* 00472e7a */
    goto mi_004740b7;
mi_00472e7f:;
    /* 00472e7f */
    a=f->r[5];b=0x00000053u;t=mi_alu(f,a,b,32,1);
    /* 00472e82 */
    if(!(f->flags&MI_ZF))goto mi_00472e88;
    /* 00472e84 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472e86 */
    goto mi_00472e56;
mi_00472e88:;
    /* 00472e88 */
    a=f->r[5];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 00472e8b */
    if(!(f->flags&MI_ZF))goto mi_00472ea1;
    /* 00472e8d */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 00472e91 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472e92 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472e93 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472e94 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472e95 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472e97 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00472e9cu;f->boundary(0x00472e97u,0x00475090u,f);
    /* 00472e9c */
    goto mi_00472aef;
mi_00472ea1:;
    /* 00472ea1 */
    a=f->r[5];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 00472ea4 */
    if(!(f->flags&MI_ZF))goto mi_00472ed9;
    /* 00472ea6 */
    f->r[2]=MI_U32((0x00000024u+f->r[4]));
    /* 00472eaa */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472eab */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472eac */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472ead */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472eae */
    f->r[4]-=4;MI_U32(f->r[4])=0x00472eb3u;f->boundary(0x00472eaeu,0x00470900u,f);
    /* 00472eb3 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00472eb6 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00472eb8 */
    if((f->flags&MI_ZF))goto mi_00472ec5;
    /* 00472eba */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472ebc */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472ebe */
    a=0x0000003du;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472ec0 */
    goto mi_00473e4d;
mi_00472ec5:;
    /* 00472ec5 */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 00472ec9 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472eca */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472ecb */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472ecc */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472ecd */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472ecf */
    f->r[4]-=4;MI_U32(f->r[4])=0x00472ed4u;f->boundary(0x00472ecfu,0x00474130u,f);
    /* 00472ed4 */
    goto mi_00472aef;
mi_00472ed9:;
    /* 00472ed9 */
    f->r[0]=(0x00000000u+f->r[5]+f->r[5]*4u);
    /* 00472edd */
    a=f->r[5];b=0x0000003au;t=mi_alu(f,a,b,32,1);
    /* 00472ee0 */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(0xffffffffu);
    /* 00472ee8 */
    f->r[0]=MI_U32((0x005d06dcu+f->r[0]*4u));
    /* 00472eef */
    MI_U32((0x00bc96d0u))=(uint32_t)(f->r[0]);
    /* 00472ef4 */
    MI_U32((0x00000030u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00472ef8 */
    if(!(f->flags&MI_ZF))goto mi_00472f07;
    /* 00472efa */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(0xffffffbfu);
    /* 00472f02 */
    goto mi_00472fcb;
mi_00472f07:;
    /* 00472f07 */
    a=f->r[5];b=0x000000aau;t=mi_alu(f,a,b,32,1);
    /* 00472f0d */
    if(!(f->flags&MI_ZF))goto mi_00472f1c;
    /* 00472f0f */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(0xffffffbfu);
    /* 00472f17 */
    goto mi_00472fcb;
mi_00472f1c:;
    /* 00472f1c */
    a=f->r[5];b=0x000000abu;t=mi_alu(f,a,b,32,1);
    /* 00472f22 */
    if(!(f->flags&MI_ZF))goto mi_00472f31;
    /* 00472f24 */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(0xffffffbfu);
    /* 00472f2c */
    goto mi_00472fcb;
mi_00472f31:;
    /* 00472f31 */
    a=f->r[5];b=0x000000c8u;t=mi_alu(f,a,b,32,1);
    /* 00472f37 */
    if(!(f->flags&MI_ZF))goto mi_00472f46;
    /* 00472f39 */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(0xffffffbfu);
    /* 00472f41 */
    goto mi_00472fcb;
mi_00472f46:;
    /* 00472f46 */
    a=f->r[5];b=0x000000c9u;t=mi_alu(f,a,b,32,1);
    /* 00472f4c */
    if(!(f->flags&MI_ZF))goto mi_00472f58;
    /* 00472f4e */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(0xffffffbfu);
    /* 00472f56 */
    goto mi_00472fcb;
mi_00472f58:;
    /* 00472f58 */
    a=f->r[5];b=0x000000cau;t=mi_alu(f,a,b,32,1);
    /* 00472f5e */
    if(!(f->flags&MI_ZF))goto mi_00472f6a;
    /* 00472f60 */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(0xffffffbfu);
    /* 00472f68 */
    goto mi_00472fcb;
mi_00472f6a:;
    /* 00472f6a */
    a=f->r[5];b=0x0000003bu;t=mi_alu(f,a,b,32,1);
    /* 00472f6d */
    if(!(f->flags&MI_ZF))goto mi_00472f79;
    /* 00472f6f */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(0xff3fbfffu);
    /* 00472f77 */
    goto mi_00472fcb;
mi_00472f79:;
    /* 00472f79 */
    a=f->r[5];b=0x000000acu;t=mi_alu(f,a,b,32,1);
    /* 00472f7f */
    if(!(f->flags&MI_ZF))goto mi_00472f8b;
    /* 00472f81 */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(0xff3fbfffu);
    /* 00472f89 */
    goto mi_00472fcb;
mi_00472f8b:;
    /* 00472f8b */
    a=f->r[5];b=0x000000adu;t=mi_alu(f,a,b,32,1);
    /* 00472f91 */
    if(!(f->flags&MI_ZF))goto mi_00472f9d;
    /* 00472f93 */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(0xff3fbfffu);
    /* 00472f9b */
    goto mi_00472fcb;
mi_00472f9d:;
    /* 00472f9d */
    a=f->r[5];b=0x00000038u;t=mi_alu(f,a,b,32,1);
    /* 00472fa0 */
    if(!(f->flags&MI_ZF))goto mi_00472fac;
    /* 00472fa2 */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(0xffffffbfu);
    /* 00472faa */
    goto mi_00472fcb;
mi_00472fac:;
    /* 00472fac */
    a=f->r[5];b=0x0000008au;t=mi_alu(f,a,b,32,1);
    /* 00472fb2 */
    if(!(f->flags&MI_ZF))goto mi_00472fbe;
    /* 00472fb4 */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(0x00000008u);
    /* 00472fbc */
    goto mi_00472fcb;
mi_00472fbe:;
    /* 00472fbe */
    a=f->r[5];b=0x00000048u;t=mi_alu(f,a,b,32,1);
    /* 00472fc1 */
    if(!(f->flags&MI_ZF))goto mi_00472fcb;
    /* 00472fc3 */
    MI_U32((0x00000030u+f->r[4]))=(uint32_t)(0x00000003u);
mi_00472fcb:;
    /* 00472fcb */
    f->r[0]=MI_U32((0x00000030u+f->r[4]));
    /* 00472fcf */
    f->r[1]=MI_U32((0x00000024u+f->r[4]));
    /* 00472fd3 */
    f->r[2]=(0xffffffffu+f->r[0]+f->r[3]*1u);
    /* 00472fd7 */
    f->r[0]=(0xffffffffu+f->r[0]+f->r[1]*1u);
    /* 00472fdb */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472fdc */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472fdd */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472fde */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472fdf */
    f->r[4]-=4;MI_U32(f->r[4])=0x00472fe4u;f->boundary(0x00472fdfu,0x00470900u,f);
    /* 00472fe4 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00472fe7 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00472fe9 */
    if((f->flags&MI_ZF))goto mi_00472ff6;
    /* 00472feb */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472fed */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472fef */
    a=0x0000003du;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472ff1 */
    goto mi_00473e4d;
mi_00472ff6:;
    /* 00472ff6 */
    a=f->r[5];b=0x000000abu;t=mi_alu(f,a,b,32,1);
    /* 00472ffc */
    if((f->flags&MI_ZF))goto mi_00473051;
    /* 00472ffe */
    a=f->r[5];b=0x000000cau;t=mi_alu(f,a,b,32,1);
    /* 00473004 */
    if((f->flags&MI_ZF))goto mi_00473051;
    /* 00473006 */
    f->r[0]=MI_U32((0x00000020u+f->r[4]));
    /* 0047300a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0047300c */
    if((f->flags&MI_ZF))goto mi_00473051;
    /* 0047300e */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 00473011 */
    if(!(f->flags&MI_ZF))goto mi_00473022;
    /* 00473013 */
    f->r[2]=MI_U32((0x00000030u+f->r[4]));
    /* 00473017 */
    f->r[1]=0x00000001u;
    /* 0047301c */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0047301e */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00473020 */
    goto mi_00473051;
mi_00473022:;
    /* 00473022 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 00473025 */
    if(!(f->flags&MI_ZF))goto mi_0047303f;
    /* 00473027 */
    f->r[0]=MI_U32((0x00000030u+f->r[4]));
    /* 0047302b */
    f->r[2]=0x00000001u;
    /* 00473030 */
    f->r[1]=0x00000001u;
    /* 00473035 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00473037 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00473039 */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0047303b */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0047303d */
    goto mi_00473051;
mi_0047303f:;
    /* 0047303f */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 00473042 */
    if(!(f->flags&MI_ZF))goto mi_00473051;
    /* 00473044 */
    f->r[1]=MI_U32((0x00000030u+f->r[4]));
    /* 00473048 */
    f->r[2]=0x00000001u;
    /* 0047304d */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0047304f */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[7]=t;
mi_00473051:;
    /* 00473051 */
    a=f->r[5];b=0x0000004cu;t=mi_alu(f,a,b,32,1);
    /* 00473054 */
    if((f->flags&MI_ZF))goto mi_004738c3;
    /* 0047305a */
    a=f->r[5];b=0x000000b5u;t=mi_alu(f,a,b,32,1);
    /* 00473060 */
    if((f->flags&MI_ZF))goto mi_004738c3;
    /* 00473066 */
    a=f->r[5];b=0x00000007u;t=mi_alu(f,a,b,32,1);
    /* 00473069 */
    if(!(f->flags&MI_ZF))goto mi_00473247;
    /* 0047306f */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473071 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473073 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473074 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473075 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047307au;f->boundary(0x00473075u,0x0047cc70u,f);
    /* 0047307a */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047307d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0047307f */
    if((f->flags&MI_ZF))goto mi_0047308c;
    /* 00473081 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473083 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473085 */
    a=0x00000019u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473087 */
    goto mi_00473e4d;
mi_0047308c:;
    /* 0047308c */
    f->r[2]=MI_U32((0x00bca2d8u));
    /* 00473092 */
    f->r[0]=(0x00000000u+f->r[6]+f->r[6]*8u);
    /* 00473095 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473097 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473099 */
    f->r[0]=(0x00000000u+f->r[6]+f->r[0]*2u);
    /* 0047309c */
    f->r[5]=0x0000005au;
    /* 004730a1 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004730a2 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004730a3 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004730a6 */
    f->r[0]=MI_U32((0x0000002cu+f->r[4]));
    /* 004730aa */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004730ab */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004730ac */
    f->r[3]=(0x00000000u+f->r[2]+f->r[1]*4u);
    /* 004730af */
    a=f->r[3];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 004730b1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004730b6u;f->boundary(0x004730b1u,0x0046a400u,f);
    /* 004730b6 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004730b7 */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004730bb */
    f->r[4]-=4;MI_U32(f->r[4])=0x004730c0u;f->boundary(0x004730bbu,0x004e2d30u,f);
    /* 004730c0 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004730c1 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004730c2 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004730c3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004730c8u;f->boundary(0x004730c3u,0x004e32e0u,f);
    /* 004730c8 */
    f->r[1]=MI_U32((0x00000038u+f->r[4]));
    /* 004730cc */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004730cd */
    f->r[4]-=4;MI_U32(f->r[4])=0x004730d2u;f->boundary(0x004730cdu,0x00485100u,f);
    /* 004730d2 */
    f->r[2]=MI_U32((0x00bca2d4u));
    /* 004730d8 */
    f->r[0]=MI_U32((0x00bca2d0u));
    /* 004730dd */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004730de */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004730e0 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004730e1 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004730e2 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004730e3 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004730e5 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004730e7 */
    MI_U8((0x00ae8e7cu+f->r[3]))=(uint8_t)(0x00000000u);
    /* 004730ee */
    f->r[4]-=4;MI_U32(f->r[4])=0x004730f3u;f->boundary(0x004730eeu,0x0047bae0u,f);
    /* 004730f3 */
    f->r[0]=MI_U32((0x00bc96d0u));
    /* 004730f8 */
    a=f->r[4];b=0x00000040u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004730fb */
    f->r[1]=(0x00000000u+f->r[0]+f->r[6]*1u);
    /* 004730fe */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473100 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[7]*1u);
    /* 00473103 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473104 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473105 */
    f->r[1]=f->r[6];
    /* 00473107 */
    f->r[2]=f->r[7];
    /* 00473109 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0047310b */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0047310d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047310e */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047310f */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473114u;f->boundary(0x0047310fu,0x00516f90u,f);
    /* 00473114 */
    f->r[0]=MI_U32((0x00bc96d0u));
    /* 00473119 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[6]*1u);
    /* 0047311c */
    f->r[2]=(0x00000000u+f->r[0]+f->r[7]*1u);
    /* 0047311f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473120 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473121 */
    f->r[1]=f->r[6];
    /* 00473123 */
    f->r[2]=f->r[7];
    /* 00473125 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00473127 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00473129 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047312a */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047312b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473130u;f->boundary(0x0047312bu,0x005173a0u,f);
    /* 00473130 */
    a=f->r[4];b=0x00000024u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00473133:;
    /* 00473133 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473134 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473139u;f->boundary(0x00473134u,0x004e3330u,f);
    /* 00473139 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047313a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047313fu;f->boundary(0x0047313au,0x004e2d30u,f);
    /* 0047313f */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473140 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473145u;f->boundary(0x00473140u,0x004e3100u,f);
    /* 00473145 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473146 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047314bu;f->boundary(0x00473146u,0x004e3140u,f);
    /* 0047314b */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047314c */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473151u;f->boundary(0x0047314cu,0x004e3180u,f);
    /* 00473151 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473152 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473157u;f->boundary(0x00473152u,0x004e31c0u,f);
    /* 00473157 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473158 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047315du;f->boundary(0x00473158u,0x004e3200u,f);
    /* 0047315d */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047315e */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473163u;f->boundary(0x0047315eu,0x004e2e20u,f);
    /* 00473163 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473164 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473169u;f->boundary(0x00473164u,0x004e3240u,f);
    /* 00473169 */
    f->r[3]=MI_U32((0x00000054u+f->r[4]));
    /* 0047316d */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047316e */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047316f */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473170 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473171 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473176u;f->boundary(0x00473171u,0x004e3280u,f);
    /* 00473176 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473177 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473178 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473179 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047317eu;f->boundary(0x00473179u,0x004e32e0u,f);
    /* 0047317e */
    a=f->r[4];b=0x00000040u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473181 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473182 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473187u;f->boundary(0x00473182u,0x0051f780u,f);
    /* 00473187 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473188 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047318du;f->boundary(0x00473188u,0x004e33a0u,f);
    /* 0047318d */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047318e */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473193u;f->boundary(0x0047318eu,0x004e3460u,f);
    /* 00473193 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473194 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473199u;f->boundary(0x00473194u,0x004e3520u,f);
    /* 00473199 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047319a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047319fu;f->boundary(0x0047319au,0x004e35e0u,f);
    /* 0047319f */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004731a0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004731a5u;f->boundary(0x004731a0u,0x004e36a0u,f);
    /* 004731a5 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004731a6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004731abu;f->boundary(0x004731a6u,0x004e3760u,f);
    /* 004731ab */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004731ac */
    f->r[4]-=4;MI_U32(f->r[4])=0x004731b1u;f->boundary(0x004731acu,0x004e3aa0u,f);
    /* 004731b1 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004731b2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004731b7u;f->boundary(0x004731b2u,0x004e39d0u,f);
    /* 004731b7 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004731b8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004731bdu;f->boundary(0x004731b8u,0x004e3cf0u,f);
    /* 004731bd */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004731be */
    f->r[4]-=4;MI_U32(f->r[4])=0x004731c3u;f->boundary(0x004731beu,0x004e3da0u,f);
    /* 004731c3 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004731c4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004731c9u;f->boundary(0x004731c4u,0x004e3f60u,f);
    /* 004731c9 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004731ca */
    f->r[4]-=4;MI_U32(f->r[4])=0x004731cfu;f->boundary(0x004731cau,0x004e4120u,f);
    /* 004731cf */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004731d0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004731d5u;f->boundary(0x004731d0u,0x004e42d0u,f);
    /* 004731d5 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004731d6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004731dbu;f->boundary(0x004731d6u,0x004e4490u,f);
    /* 004731db */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004731dc */
    f->r[4]-=4;MI_U32(f->r[4])=0x004731e1u;f->boundary(0x004731dcu,0x004e4550u,f);
    /* 004731e1 */
    a=f->r[4];b=0x00000040u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004731e4 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004731e5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004731eau;f->boundary(0x004731e5u,0x004e47d0u,f);
    /* 004731ea */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004731eb */
    f->r[4]-=4;MI_U32(f->r[4])=0x004731f0u;f->boundary(0x004731ebu,0x004e4e10u,f);
    /* 004731f0 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004731f1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004731f6u;f->boundary(0x004731f1u,0x004e4ed0u,f);
    /* 004731f6 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004731f7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004731fcu;f->boundary(0x004731f7u,0x004e5070u,f);
    /* 004731fc */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004731fd */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004731fe */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004731ff */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473200 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473205u;f->boundary(0x00473200u,0x004e2b10u,f);
    /* 00473205 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473208 */
    a=(f->r[3]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,5);f->r[3]=(f->r[3]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0047320a */
    a=f->r[5];b=0x00000028u;t=mi_alu(f,a,b,32,1);
    /* 0047320d */
    MI_U8((0x00605a5cu))=(uint8_t)(0x00000000u);
    /* 00473214 */
    if((f->flags&MI_ZF))goto mi_00474048;
    /* 0047321a */
    a=f->r[5];b=0x0000002cu;t=mi_alu(f,a,b,32,1);
    /* 0047321d */
    if((f->flags&MI_ZF))goto mi_00474048;
    /* 00473223 */
    a=f->r[5];b=0x0000002du;t=mi_alu(f,a,b,32,1);
    /* 00473226 */
    if((f->flags&MI_ZF))goto mi_00474048;
    /* 0047322c */
    a=f->r[5];b=0x000000b7u;t=mi_alu(f,a,b,32,1);
    /* 00473232 */
    if(!(f->flags&MI_ZF))goto mi_00473ee5;
    /* 00473238 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047323a */
    a=0x00000090u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047323f */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473240 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473241 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473242 */
    goto mi_00474050;
mi_00473247:;
    /* 00473247 */
    a=f->r[5];b=0x000000abu;t=mi_alu(f,a,b,32,1);
    /* 0047324d */
    if((f->flags&MI_ZF))goto mi_00473885;
    /* 00473253 */
    a=f->r[5];b=0x000000cau;t=mi_alu(f,a,b,32,1);
    /* 00473259 */
    if((f->flags&MI_ZF))goto mi_00473885;
    /* 0047325f */
    a=f->r[5];b=0x000000b6u;t=mi_alu(f,a,b,32,1);
    /* 00473265 */
    if((f->flags&MI_ZF))goto mi_0047380e;
    /* 0047326b */
    a=f->r[5];b=0x0000004au;t=mi_alu(f,a,b,32,1);
    /* 0047326e */
    if((f->flags&MI_ZF))goto mi_0047380e;
    /* 00473274 */
    a=f->r[5];b=0x00000088u;t=mi_alu(f,a,b,32,1);
    /* 0047327a */
    if(!(f->flags&MI_ZF))goto mi_004732d4;
    /* 0047327c */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00bca314u)))&0xffu)<<0);
    /* 00473281 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00473283 */
    if((f->flags&MI_ZF))goto mi_004732b1;
    /* 00473285 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473287 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473289 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047328a */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047328b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473290u;f->boundary(0x0047328bu,0x0047cc70u,f);
    /* 00473290 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473293 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00473295 */
    if((f->flags&MI_ZF))goto mi_00473133;
    /* 0047329b */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047329d */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047329f */
    a=0x00000019u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004732a1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004732a6u;f->boundary(0x004732a1u,0x004e2970u,f);
    /* 004732a6 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004732a9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004732aa */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004732ab */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004732ac */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004732ad */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004732b0 */
    f->r[4]+=4;return;
mi_004732b1:;
    /* 004732b1 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00bca315u)))&0xffu)<<0);
    /* 004732b6 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004732b8 */
    if(!(f->flags&MI_ZF))goto mi_00474119;
    /* 004732be */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004732c0 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004732c2 */
    a=0x00000044u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004732c4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004732c9u;f->boundary(0x004732c4u,0x004e2970u,f);
    /* 004732c9 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004732cc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004732cd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004732ce */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004732cf */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004732d0 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004732d3 */
    f->r[4]+=4;return;
mi_004732d4:;
    /* 004732d4 */
    a=f->r[5];b=0x0000004bu;t=mi_alu(f,a,b,32,1);
    /* 004732d7 */
    if((f->flags&MI_ZF))goto mi_004737a1;
    /* 004732dd */
    a=f->r[5];b=0x000000c0u;t=mi_alu(f,a,b,32,1);
    /* 004732e3 */
    if((f->flags&MI_ZF))goto mi_004737a1;
    /* 004732e9 */
    a=f->r[5];b=0x000000e5u;t=mi_alu(f,a,b,32,1);
    /* 004732ef */
    if(!(f->flags&MI_ZF))goto mi_0047333b;
    /* 004732f1 */
    f->r[0]=MI_U32((0x00bca300u));
    /* 004732f6 */
    f->r[1]=0x006f9320u;
    /* 004732fb */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004732fc */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473301u;f->boundary(0x004732fcu,0x005456c0u,f);
    /* 00473301 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00473303 */
    if(!(f->flags&MI_ZF))goto mi_0047332c;
mi_00473305:;
    /* 00473305 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473307 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473309 */
    a=0x00000110u;f->r[4]-=4;MI_U32(f->r[4])=a;
mi_0047330e:;
    /* 0047330e */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473313u;f->boundary(0x0047330eu,0x004e2970u,f);
    /* 00473313 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473315 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473317 */
    a=0x00000111u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047331c */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473321u;f->boundary(0x0047331cu,0x004e2970u,f);
    /* 00473321 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473324 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00473325 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00473326 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00473327 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00473328 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047332b */
    f->r[4]+=4;return;
mi_0047332c:;
    /* 0047332c */
    f->r[1]=0x006f9320u;
    /* 00473331 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473336u;f->boundary(0x00473331u,0x00545700u,f);
    /* 00473336 */
    goto mi_00473b6d;
mi_0047333b:;
    /* 0047333b */
    a=f->r[5];b=0x000000eau;t=mi_alu(f,a,b,32,1);
    /* 00473341 */
    if(!(f->flags&MI_ZF))goto mi_00473366;
    /* 00473343 */
    f->r[0]=MI_U32((0x00bca300u));
    /* 00473348 */
    f->r[1]=0x006f5648u;
    /* 0047334d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047334e */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473353u;f->boundary(0x0047334eu,0x00558b60u,f);
    /* 00473353 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00473355 */
    if((f->flags&MI_ZF))goto mi_00473305;
    /* 00473357 */
    f->r[1]=0x006f5648u;
    /* 0047335c */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473361u;f->boundary(0x0047335cu,0x00558ba0u,f);
    /* 00473361 */
    goto mi_00473b6d;
mi_00473366:;
    /* 00473366 */
    a=f->r[5];b=0x000000ebu;t=mi_alu(f,a,b,32,1);
    /* 0047336c */
    if(!(f->flags&MI_ZF))goto mi_00473391;
    /* 0047336e */
    f->r[0]=MI_U32((0x00bca300u));
    /* 00473373 */
    f->r[1]=0x006de9e0u;
    /* 00473378 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473379 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047337eu;f->boundary(0x00473379u,0x00555e00u,f);
    /* 0047337e */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00473380 */
    if((f->flags&MI_ZF))goto mi_00473305;
    /* 00473382 */
    f->r[1]=0x006de9e0u;
    /* 00473387 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047338cu;f->boundary(0x00473387u,0x00555e40u,f);
    /* 0047338c */
    goto mi_00473b6d;
mi_00473391:;
    /* 00473391 */
    a=f->r[5];b=0x000000ecu;t=mi_alu(f,a,b,32,1);
    /* 00473397 */
    if(!(f->flags&MI_ZF))goto mi_004733c0;
    /* 00473399 */
    f->r[0]=MI_U32((0x00bca300u));
    /* 0047339e */
    f->r[1]=0x006e9188u;
    /* 004733a3 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004733a4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004733a9u;f->boundary(0x004733a4u,0x0054f3f0u,f);
    /* 004733a9 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004733ab */
    if((f->flags&MI_ZF))goto mi_00473305;
    /* 004733b1 */
    f->r[1]=0x006e9188u;
    /* 004733b6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004733bbu;f->boundary(0x004733b6u,0x0054f430u,f);
    /* 004733bb */
    goto mi_00473b6d;
mi_004733c0:;
    /* 004733c0 */
    a=f->r[5];b=0x000000e4u;t=mi_alu(f,a,b,32,1);
    /* 004733c6 */
    if(!(f->flags&MI_ZF))goto mi_004733f6;
    /* 004733c8 */
    f->r[0]=MI_U32((0x00bca300u));
    /* 004733cd */
    f->r[1]=0x006de548u;
    /* 004733d2 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004733d3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004733d8u;f->boundary(0x004733d3u,0x004e65f0u,f);
    /* 004733d8 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004733da */
    if(!(f->flags&MI_ZF))goto mi_004733ea;
    /* 004733dc */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004733de */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004733e0 */
    a=0x000000f9u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004733e5 */
    goto mi_0047330e;
mi_004733ea:;
    /* 004733ea */
    f->r[1]=0x006de548u;
    /* 004733ef */
    f->r[4]-=4;MI_U32(f->r[4])=0x004733f4u;f->boundary(0x004733efu,0x004e6650u,f);
    /* 004733f4 */
    goto mi_00473420;
mi_004733f6:;
    /* 004733f6 */
    a=f->r[5];b=0x000000e6u;t=mi_alu(f,a,b,32,1);
    /* 004733fc */
    if(!(f->flags&MI_ZF))goto mi_00473441;
    /* 004733fe */
    f->r[0]=MI_U32((0x00bca300u));
    /* 00473403 */
    f->r[1]=0x006de650u;
    /* 00473408 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473409 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047340eu;f->boundary(0x00473409u,0x00401400u,f);
    /* 0047340e */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00473410 */
    if((f->flags&MI_ZF))goto mi_00473305;
    /* 00473416 */
    f->r[1]=0x006de650u;
    /* 0047341b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473420u;f->boundary(0x0047341bu,0x00401430u,f);
mi_00473420:;
    /* 00473420 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00473422 */
    if((f->flags&MI_ZF))goto mi_00473133;
    /* 00473428 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047342a */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047342c */
    a=0x000000f4u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473431 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473436u;f->boundary(0x00473431u,0x004e2970u,f);
    /* 00473436 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473439 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0047343a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0047343b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0047343c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0047343d */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473440 */
    f->r[4]+=4;return;
mi_00473441:;
    /* 00473441 */
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
    /* 00473445 */
    f->r[1]=MI_U32((0x00000034u+f->r[4]));
    /* 00473449 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047344a */
    f->r[0]=MI_U32((0x00000034u+f->r[4]));
    /* 0047344e */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473450 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473451 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473452 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473453 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473454 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473455 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047345au;f->boundary(0x00473455u,0x0047c880u,f);
    /* 0047345a */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047345d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0047345f */
    if(!(f->flags&MI_ZF))goto mi_0047346a;
    /* 00473461 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473462 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473463 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473465 */
    goto mi_00473e4d;
mi_0047346a:;
    /* 0047346a */
    a=f->r[5];b=0x00000035u;t=mi_alu(f,a,b,32,1);
    /* 0047346d */
    if(!(f->flags&MI_ZF))goto mi_004734be;
    /* 0047346f */
    a=MI_U32((0x00605a0cu));b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 00473476 */
    if((f->flags&MI_ZF))goto mi_00473133;
    /* 0047347c */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 00473481 */
    f->r[3]=0x000001f4u;
    /* 00473486 */
    f->r[2]=f->r[0];
    /* 00473488 */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 0047348e */
    a=MI_U32((0x00ea61bcu+f->r[2]));b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 00473494 */
    if(!mi_signed_less(f->flags))goto mi_004734a1;
    /* 00473496 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473498 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047349a */
    a=0x0000004fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047349c */
    goto mi_00473e4d;
mi_004734a1:;
    /* 004734a1 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004734a2 */
    a=0x00000017u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004734a4 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004734a5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004734aau;f->boundary(0x004734a5u,0x0045b4d0u,f);
    /* 004734aa */
    f->r[0]=MI_U32((0x00bc9650u));
    /* 004734af */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004734b2 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004734b4 */
    MI_U32((0x00bc9650u))=(uint32_t)(f->r[0]);
    /* 004734b9 */
    goto mi_00473133;
mi_004734be:;
    /* 004734be */
    a=f->r[5];b=0x000000cfu;t=mi_alu(f,a,b,32,1);
    /* 004734c4 */
    if(!(f->flags&MI_ZF))goto mi_004734f8;
    /* 004734c6 */
    a=MI_U32((0x00605a0cu));b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004734cd */
    if((f->flags&MI_ZF))goto mi_00473133;
    /* 004734d3 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 004734d8 */
    f->r[3]=0x00002710u;
    /* 004734dd */
    f->r[1]=f->r[0];
    /* 004734df */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 004734e5 */
    a=MI_U32((0x00ea61c8u+f->r[1]));b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004734eb */
    if(!mi_signed_less(f->flags))goto mi_00473532;
    /* 004734ed */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004734ef */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004734f1 */
    a=0x00000055u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004734f3 */
    goto mi_00473e4d;
mi_004734f8:;
    /* 004734f8 */
    a=f->r[5];b=0x000000d0u;t=mi_alu(f,a,b,32,1);
    /* 004734fe */
    if(!(f->flags&MI_ZF))goto mi_0047354f;
    /* 00473500 */
    a=MI_U32((0x00605a0cu));b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 00473507 */
    if((f->flags&MI_ZF))goto mi_00473133;
    /* 0047350d */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 00473512 */
    f->r[3]=0x00004e20u;
    /* 00473517 */
    f->r[2]=f->r[0];
    /* 00473519 */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 0047351f */
    a=MI_U32((0x00ea61c8u+f->r[2]));b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 00473525 */
    if(!mi_signed_less(f->flags))goto mi_00473532;
    /* 00473527 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473529 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047352b */
    a=0x00000056u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047352d */
    goto mi_00473e4d;
mi_00473532:;
    /* 00473532 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473533 */
    a=0x0000001au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473535 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473536 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047353bu;f->boundary(0x00473536u,0x0045b4d0u,f);
    /* 0047353b */
    f->r[0]=MI_U32((0x00bc965cu));
    /* 00473540 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473543 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00473545 */
    MI_U32((0x00bc965cu))=(uint32_t)(f->r[0]);
    /* 0047354a */
    goto mi_00473133;
mi_0047354f:;
    /* 0047354f */
    a=f->r[5];b=0x000000a1u;t=mi_alu(f,a,b,32,1);
    /* 00473555 */
    if(!(f->flags&MI_ZF))goto mi_004735a7;
    /* 00473557 */
    f->r[0]=MI_U32((0x00605a48u));
    /* 0047355c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0047355e */
    if((f->flags&MI_ZF))goto mi_00473587;
    /* 00473560 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473561 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473563 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473565 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473566 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473567 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047356cu;f->boundary(0x00473567u,0x00482720u,f);
    /* 0047356c */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047356f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00473571 */
    if(!(f->flags&MI_ZF))goto mi_00473587;
    /* 00473573 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473574 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473575 */
    a=0x00000016u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473577 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047357cu;f->boundary(0x00473577u,0x004e2970u,f);
    /* 0047357c */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047357f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00473580 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00473581 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00473582 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00473583 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473586 */
    f->r[4]+=4;return;
mi_00473587:;
    /* 00473587 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047358cu;f->boundary(0x00473587u,0x00459fa0u,f);
    /* 0047358c */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0047358e */
    if(!(f->flags&MI_ZF))goto mi_00473133;
    /* 00473594 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473596 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473598 */
    a=0x0000003cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047359a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047359fu;f->boundary(0x0047359au,0x004e2970u,f);
    /* 0047359f */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004735a2 */
    goto mi_00473133;
mi_004735a7:;
    /* 004735a7 */
    f->r[0]=MI_U32((0x006059ccu));
    /* 004735ac */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004735ae */
    if((f->flags&MI_ZF))goto mi_004735df;
    /* 004735b0 */
    a=0x00010800u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004735b5 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004735b7 */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004735b9 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004735ba */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004735bb */
    f->r[4]-=4;MI_U32(f->r[4])=0x004735c0u;f->boundary(0x004735bbu,0x00482650u,f);
    /* 004735c0 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004735c3 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004735c5 */
    if(!(f->flags&MI_ZF))goto mi_004738ef;
    /* 004735cb */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004735cc */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004735cd */
    a=0x00000014u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004735cf */
    f->r[4]-=4;MI_U32(f->r[4])=0x004735d4u;f->boundary(0x004735cfu,0x004e2970u,f);
    /* 004735d4 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004735d7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004735d8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004735d9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004735da */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004735db */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004735de */
    f->r[4]+=4;return;
mi_004735df:;
    /* 004735df */
    f->r[0]=MI_U32((0x00605a48u));
    /* 004735e4 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004735e6 */
    if((f->flags&MI_ZF))goto mi_00473613;
    /* 004735e8 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004735e9 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004735eb */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004735ed */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004735ee */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004735ef */
    f->r[4]-=4;MI_U32(f->r[4])=0x004735f4u;f->boundary(0x004735efu,0x00482720u,f);
    /* 004735f4 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004735f7 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004735f9 */
    if(!(f->flags&MI_ZF))goto mi_004738ef;
    /* 004735ff */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473600 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473601 */
    a=0x00000016u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473603 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473608u;f->boundary(0x00473603u,0x004e2970u,f);
    /* 00473608 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047360b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0047360c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0047360d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0047360e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0047360f */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473612 */
    f->r[4]+=4;return;
mi_00473613:;
    /* 00473613 */
    f->r[0]=MI_U32((0x006059c8u));
    /* 00473618 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0047361a */
    if((f->flags&MI_ZF))goto mi_00473648;
    /* 0047361c */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047361e */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473620 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473622 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473623 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473624 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473629u;f->boundary(0x00473624u,0x00482650u,f);
    /* 00473629 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047362c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0047362e */
    if(!(f->flags&MI_ZF))goto mi_004738ef;
    /* 00473634 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473635 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473636 */
    a=0x00000017u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473638 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047363du;f->boundary(0x00473638u,0x004e2970u,f);
    /* 0047363d */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473640 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00473641 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00473642 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00473643 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00473644 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473647 */
    f->r[4]+=4;return;
mi_00473648:;
    /* 00473648 */
    f->r[0]=MI_U32((0x006059c0u));
    /* 0047364d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0047364f */
    if((f->flags&MI_ZF))goto mi_00473680;
    /* 00473651 */
    a=0x00040000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473656 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473658 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047365a */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047365b */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047365c */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473661u;f->boundary(0x0047365cu,0x00482650u,f);
    /* 00473661 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473664 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00473666 */
    if(!(f->flags&MI_ZF))goto mi_004738ef;
    /* 0047366c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047366d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047366e */
    a=0x0000003au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473670 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473675u;f->boundary(0x00473670u,0x004e2970u,f);
    /* 00473675 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473678 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00473679 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0047367a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0047367b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0047367c */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047367f */
    f->r[4]+=4;return;
mi_00473680:;
    /* 00473680 */
    f->r[0]=MI_U32((0x006059c4u));
    /* 00473685 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00473687 */
    if((f->flags&MI_ZF))goto mi_004736b8;
    /* 00473689 */
    a=0x00010004u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047368e */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473690 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473692 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473693 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473694 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473699u;f->boundary(0x00473694u,0x00482650u,f);
    /* 00473699 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047369c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0047369e */
    if(!(f->flags&MI_ZF))goto mi_004738ef;
    /* 004736a4 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004736a5 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004736a6 */
    a=0x00000015u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004736a8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004736adu;f->boundary(0x004736a8u,0x004e2970u,f);
    /* 004736ad */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004736b0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004736b1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004736b2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004736b3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004736b4 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004736b7 */
    f->r[4]+=4;return;
mi_004736b8:;
    /* 004736b8 */
    f->r[0]=MI_U32((0x006059bcu));
    /* 004736bd */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004736bf */
    if((f->flags&MI_ZF))goto mi_004736f0;
    /* 004736c1 */
    a=0x00004000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004736c6 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004736c8 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004736ca */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004736cb */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004736cc */
    f->r[4]-=4;MI_U32(f->r[4])=0x004736d1u;f->boundary(0x004736ccu,0x004828e0u,f);
    /* 004736d1 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004736d4 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004736d6 */
    if(!(f->flags&MI_ZF))goto mi_004738ef;
    /* 004736dc */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004736dd */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004736de */
    a=0x00000031u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004736e0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004736e5u;f->boundary(0x004736e0u,0x004e2970u,f);
    /* 004736e5 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004736e8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004736e9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004736ea */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004736eb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004736ec */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004736ef */
    f->r[4]+=4;return;
mi_004736f0:;
    /* 004736f0 */
    f->r[0]=MI_U32((0x006059b8u));
    /* 004736f5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004736f7 */
    if((f->flags&MI_ZF))goto mi_00473759;
    /* 004736f9 */
    f->r[2]=MI_U32((0x00bca2d8u));
    /* 004736ff */
    f->r[0]=(0x00000000u+f->r[6]+f->r[6]*8u);
    /* 00473702 */
    a=0x00000040u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473704 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473706 */
    f->r[0]=(0x00000000u+f->r[6]+f->r[0]*2u);
    /* 00473709 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047370b */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047370c */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047370d */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 00473710 */
    f->r[3]=(0x00000000u+f->r[2]+f->r[1]*4u);
    /* 00473713 */
    a=f->r[3];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 00473715 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047371au;f->boundary(0x00473715u,0x004828e0u,f);
    /* 0047371a */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047371d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0047371f */
    if(!(f->flags&MI_ZF))goto mi_00473735;
    /* 00473721 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473722 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473723 */
    a=0x00000042u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473725 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047372au;f->boundary(0x00473725u,0x004e2970u,f);
    /* 0047372a */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047372d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0047372e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0047372f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00473730 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00473731 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473734 */
    f->r[4]+=4;return;
mi_00473735:;
    /* 00473735 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00adc36cu+f->r[3])))&0xffu)<<0);
    /* 0047373b */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0047373d */
    if((f->flags&MI_ZF))goto mi_004738eb;
    /* 00473743 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473745 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473747 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473749 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047374eu;f->boundary(0x00473749u,0x004e2970u,f);
    /* 0047374e */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473751 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00473752 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00473753 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00473754 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00473755 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473758 */
    f->r[4]+=4;return;
mi_00473759:;
    /* 00473759 */
    f->r[1]=MI_U32((0x006059b4u));
    /* 0047375f */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00473761 */
    if((f->flags&MI_ZF))goto mi_004738ef;
    /* 00473767 */
    f->r[0]=(0x00000000u+f->r[5]+f->r[5]*4u);
    /* 0047376b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047376c */
    a=0x00000080u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473771 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473773 */
    f->r[0]=MI_U32((0x005d06dcu+f->r[0]*4u));
    /* 0047377a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047377b */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047377c */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047377d */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473782u;f->boundary(0x0047377du,0x004829b0u,f);
    /* 00473782 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473785 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00473787 */
    if(!(f->flags&MI_ZF))goto mi_004738ef;
    /* 0047378d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047378e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047378f */
    a=0x0000004cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473791 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473796u;f->boundary(0x00473791u,0x004e2970u,f);
    /* 00473796 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473799 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0047379a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0047379b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0047379c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0047379d */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004737a0 */
    f->r[4]+=4;return;
mi_004737a1:;
    /* 004737a1 */
    f->r[0]=MI_U32((0x00705c1cu));
    /* 004737a6 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004737a8 */
    a=f->r[0];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 004737ab */
    if(mi_signed_less(f->flags))goto mi_004737c4;
    /* 004737ad */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004737af */
    a=0x000000ceu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004737b4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004737b9u;f->boundary(0x004737b4u,0x004e2970u,f);
    /* 004737b9 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004737bc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004737bd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004737be */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004737bf */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004737c0 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004737c3 */
    f->r[4]+=4;return;
mi_004737c4:;
    /* 004737c4 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004737c5 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004737c6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004737cbu;f->boundary(0x004737c6u,0x0047d060u,f);
    /* 004737cb */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004737ce */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004737d0 */
    if((f->flags&MI_ZF))goto mi_004737e8;
    /* 004737d2 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004737d4 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004737d6 */
    a=0x00000019u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004737d8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004737ddu;f->boundary(0x004737d8u,0x004e2970u,f);
    /* 004737dd */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004737e0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004737e1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004737e2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004737e3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004737e4 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004737e7 */
    f->r[4]+=4;return;
mi_004737e8:;
    /* 004737e8 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004737e9 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004737ea */
    f->r[4]-=4;MI_U32(f->r[4])=0x004737efu;f->boundary(0x004737eau,0x00457ac0u,f);
    /* 004737ef */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004737f2 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004737f4 */
    if(!(f->flags&MI_ZF))goto mi_004738ef;
    /* 004737fa */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004737fb */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004737fc */
    a=0x00000038u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004737fe */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473803u;f->boundary(0x004737feu,0x004e2970u,f);
    /* 00473803 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473806 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00473807 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00473808 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00473809 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0047380a */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047380d */
    f->r[4]+=4;return;
mi_0047380e:;
    /* 0047380e */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473810 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473811 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473812 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473817u;f->boundary(0x00473812u,0x0047d060u,f);
    /* 00473817 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047381a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0047381c */
    if((f->flags&MI_ZF))goto mi_00473834;
    /* 0047381e */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473820 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473822 */
    a=0x00000019u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473824 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473829u;f->boundary(0x00473824u,0x004e2970u,f);
    /* 00473829 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047382c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0047382d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0047382e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0047382f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00473830 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473833 */
    f->r[4]+=4;return;
mi_00473834:;
    /* 00473834 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473835 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473836 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047383bu;f->boundary(0x00473836u,0x00457ac0u,f);
    /* 0047383b */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047383e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00473840 */
    if(!(f->flags&MI_ZF))goto mi_00473856;
    /* 00473842 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473843 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473844 */
    a=0x00000038u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473846 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047384bu;f->boundary(0x00473846u,0x004e2970u,f);
    /* 0047384b */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047384e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0047384f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00473850 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00473851 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00473852 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473855 */
    f->r[4]+=4;return;
mi_00473856:;
    /* 00473856 */
    a=f->r[5];b=0x000000b6u;t=mi_alu(f,a,b,32,1);
    /* 0047385c */
    if(!(f->flags&MI_ZF))goto mi_004738ef;
    /* 00473862 */
    a=MI_U32((0x00bcb9b0u));b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 00473869 */
    if(mi_signed_less(f->flags))goto mi_00473133;
    /* 0047386f */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473871 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473873 */
    a=0x00000054u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473875 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047387au;f->boundary(0x00473875u,0x004e2970u,f);
    /* 0047387a */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047387d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0047387e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0047387f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00473880 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00473881 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473884 */
    f->r[4]+=4;return;
mi_00473885:;
    /* 00473885 */
    f->r[0]=MI_U32((0x00bc9714u));
    /* 0047388a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047388b */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047388c */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047388d */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473892u;f->boundary(0x0047388du,0x004bff60u,f);
    /* 00473892 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473895 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00473897 */
    if(!(f->flags&MI_ZF))goto mi_004738ef;
    /* 00473899 */
    a=0x000000afu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047389e */
    a=0x0000cb10u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004738a3 */
    a=0x00b285ccu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004738a8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004738adu;f->boundary(0x004738a8u,0x0040f920u,f);
    /* 004738ad */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004738af */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004738b1 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004738b3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004738b8u;f->boundary(0x004738b3u,0x004e2970u,f);
    /* 004738b8 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004738bb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004738bc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004738bd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004738be */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004738bf */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004738c2 */
    f->r[4]+=4;return;
mi_004738c3:;
    /* 004738c3 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004738c5 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004738c7 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004738c8 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004738c9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004738ceu;f->boundary(0x004738c9u,0x0047cc70u,f);
    /* 004738ce */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004738d1 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004738d3 */
    if((f->flags&MI_ZF))goto mi_004738ef;
mi_004738d5:;
    /* 004738d5 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004738d7 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004738d9 */
    a=0x00000019u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004738db */
    f->r[4]-=4;MI_U32(f->r[4])=0x004738e0u;f->boundary(0x004738dbu,0x004e2970u,f);
    /* 004738e0 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004738e3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004738e4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004738e5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004738e6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004738e7 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004738ea */
    f->r[4]+=4;return;
mi_004738eb:;
    /* 004738eb */
    f->r[3]=MI_U32((0x00000028u+f->r[4]));
mi_004738ef:;
    /* 004738ef */
    a=f->r[5];b=0x00000028u;t=mi_alu(f,a,b,32,1);
    /* 004738f2 */
    if((f->flags&MI_ZF))goto mi_004738fe;
    /* 004738f4 */
    a=f->r[5];b=0x0000002cu;t=mi_alu(f,a,b,32,1);
    /* 004738f7 */
    if((f->flags&MI_ZF))goto mi_004738fe;
    /* 004738f9 */
    a=f->r[5];b=0x0000002du;t=mi_alu(f,a,b,32,1);
    /* 004738fc */
    if(!(f->flags&MI_ZF))goto mi_00473955;
mi_004738fe:;
    /* 004738fe */
    f->r[1]=MI_U32((0x00000014u+f->r[4]));
    /* 00473902 */
    f->r[2]=MI_U32((0x00000034u+f->r[4]));
    /* 00473906 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473907 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473909 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047390a */
    a=0x00000004u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047390c */
    f->r[0]=(0xffffffffu+f->r[6]);
    /* 0047390f */
    a=0x00000004u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473911 */
    f->r[1]=(0x00000003u+f->r[7]);
    /* 00473914 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473915 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473916 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047391bu;f->boundary(0x00473916u,0x0047c880u,f);
    /* 0047391b */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047391e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00473920 */
    if(!(f->flags&MI_ZF))goto mi_00473936;
    /* 00473922 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473923 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473924 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473926 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047392bu;f->boundary(0x00473926u,0x004e2970u,f);
    /* 0047392b */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047392e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0047392f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00473930 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00473931 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00473932 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473935 */
    f->r[4]+=4;return;
mi_00473936:;
    /* 00473936 */
    a=MI_U32((0x00bcb9b4u));b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 0047393d */
    if(mi_signed_less(f->flags))goto mi_00473955;
    /* 0047393f */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473941 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473943 */
    a=0x00000034u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473945 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047394au;f->boundary(0x00473945u,0x004e2970u,f);
    /* 0047394a */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047394d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0047394e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0047394f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00473950 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00473951 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473954 */
    f->r[4]+=4;return;
mi_00473955:;
    /* 00473955 */
    a=f->r[5];b=0x000000d2u;t=mi_alu(f,a,b,32,1);
    /* 0047395b */
    if(!(f->flags&MI_ZF))goto mi_00473a40;
    /* 00473961 */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 00473967 */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 0047396d */
    a=MI_U16((0x00ea5baau+f->r[2]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 00473975 */
    if((f->flags&MI_ZF))goto mi_0047398d;
    /* 00473977 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473979 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047397b */
    a=0x00000004u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047397d */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473982u;f->boundary(0x0047397du,0x004e2970u,f);
    /* 00473982 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473985 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00473986 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00473987 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00473988 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00473989 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047398c */
    f->r[4]+=4;return;
mi_0047398d:;
    /* 0047398d */
    f->r[1]=MI_U32((0x00000014u+f->r[4]));
    /* 00473991 */
    f->r[2]=MI_U32((0x00000034u+f->r[4]));
    /* 00473995 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473996 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473998 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473999 */
    a=0x00000006u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047399b */
    f->r[0]=(0x00000006u+f->r[7]);
    /* 0047399e */
    a=0x00000006u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004739a0 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004739a1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004739a2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004739a7u;f->boundary(0x004739a2u,0x0047c880u,f);
    /* 004739a7 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004739aa */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004739ac */
    if(!(f->flags&MI_ZF))goto mi_004739c2;
    /* 004739ae */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004739af */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004739b0 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004739b2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004739b7u;f->boundary(0x004739b2u,0x004e2970u,f);
    /* 004739b7 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004739ba */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004739bb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004739bc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004739bd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004739be */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004739c1 */
    f->r[4]+=4;return;
mi_004739c2:;
    /* 004739c2 */
    f->r[1]=MI_U32((0x00000014u+f->r[4]));
    /* 004739c6 */
    f->r[2]=MI_U32((0x00000034u+f->r[4]));
    /* 004739ca */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004739cb */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004739cd */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004739ce */
    a=0x00000006u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004739d0 */
    f->r[0]=(0x0000000cu+f->r[7]);
    /* 004739d3 */
    a=0x00000006u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004739d5 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004739d6 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004739d7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004739dcu;f->boundary(0x004739d7u,0x0047c880u,f);
    /* 004739dc */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004739df */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004739e1 */
    if(!(f->flags&MI_ZF))goto mi_004739f7;
    /* 004739e3 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004739e4 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004739e5 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004739e7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004739ecu;f->boundary(0x004739e7u,0x004e2970u,f);
    /* 004739ec */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004739ef */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004739f0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004739f1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004739f2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004739f3 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004739f6 */
    f->r[4]+=4;return;
mi_004739f7:;
    /* 004739f7 */
    a=f->r[3];b=0x00000005u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 004739fa */
    f->r[0]=(0x0000000bu+f->r[7]);
    /* 004739fd */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004739fe */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004739ff */
    f->r[0]=(0x00000006u+f->r[7]);
    /* 00473a02 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473a03 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473a04 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473a09u;f->boundary(0x00473a04u,0x00470900u,f);
    /* 00473a09 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473a0c */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00473a0e */
    if((f->flags&MI_ZF))goto mi_00473a1b;
    /* 00473a10 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473a12 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473a14 */
    a=0x0000003du;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473a16 */
    goto mi_00473e4d;
mi_00473a1b:;
    /* 00473a1b */
    f->r[1]=(0x00000011u+f->r[7]);
    /* 00473a1e */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473a1f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473a20 */
    f->r[0]=(0x0000000cu+f->r[7]);
    /* 00473a23 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473a24 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473a25 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473a2au;f->boundary(0x00473a25u,0x00470900u,f);
    /* 00473a2a */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473a2d */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00473a2f */
    if((f->flags&MI_ZF))goto mi_00473133;
    /* 00473a35 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473a37 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473a39 */
    a=0x0000003du;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473a3b */
    goto mi_00473e4d;
mi_00473a40:;
    /* 00473a40 */
    a=f->r[5];b=0x000000d5u;t=mi_alu(f,a,b,32,1);
    /* 00473a46 */
    if(!(f->flags&MI_ZF))goto mi_00473ad4;
    /* 00473a4c */
    f->r[0]=MI_U32((0x00bca300u));
    /* 00473a51 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473a52 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473a57u;f->boundary(0x00473a52u,0x00541ae0u,f);
    /* 00473a57 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473a5a */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00473a5c */
    if(!(f->flags&MI_ZF))goto mi_00473a74;
    /* 00473a5e */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473a60 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473a62 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473a64 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473a69u;f->boundary(0x00473a64u,0x004e2970u,f);
    /* 00473a69 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473a6c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00473a6d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00473a6e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00473a6f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00473a70 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473a73 */
    f->r[4]+=4;return;
mi_00473a74:;
    /* 00473a74 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473a79u;f->boundary(0x00473a74u,0x00542ee0u,f);
    /* 00473a79 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00473a7b */
    if((f->flags&MI_ZF))goto mi_00473a93;
    /* 00473a7d */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473a7f */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473a81 */
    a=0x0000005bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473a83 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473a88u;f->boundary(0x00473a83u,0x004e2970u,f);
    /* 00473a88 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473a8b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00473a8c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00473a8d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00473a8e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00473a8f */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473a92 */
    f->r[4]+=4;return;
mi_00473a93:;
    /* 00473a93 */
    f->r[0]=MI_U32((0x00605a0cu));
    /* 00473a98 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00473a9a */
    if(!(f->flags&MI_ZF))goto mi_00473133;
    /* 00473aa0 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 00473aa5 */
    f->r[3]=0x000055f0u;
    /* 00473aaa */
    f->r[2]=f->r[0];
    /* 00473aac */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 00473ab2 */
    a=MI_U32((0x00ea61d8u+f->r[2]));b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 00473ab8 */
    if(!mi_signed_less(f->flags))goto mi_00473c15;
    /* 00473abe */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473ac0 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473ac2 */
    a=0x0000005au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473ac4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473ac9u;f->boundary(0x00473ac4u,0x004e2970u,f);
    /* 00473ac9 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473acc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00473acd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00473ace */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00473acf */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00473ad0 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473ad3 */
    f->r[4]+=4;return;
mi_00473ad4:;
    /* 00473ad4 */
    a=f->r[5];b=0x000000e1u;t=mi_alu(f,a,b,32,1);
    /* 00473ada */
    if(!(f->flags&MI_ZF))goto mi_00473b31;
    /* 00473adc */
    f->r[0]=MI_U32((0x00bca300u));
    /* 00473ae1 */
    f->r[1]=0x006fd5d8u;
    /* 00473ae6 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473ae7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473aecu;f->boundary(0x00473ae7u,0x0040d850u,f);
    /* 00473aec */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00473aee */
    if(!(f->flags&MI_ZF))goto mi_00473b06;
    /* 00473af0 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473af2 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473af4 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473af6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473afbu;f->boundary(0x00473af6u,0x004e2970u,f);
    /* 00473afb */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473afe */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00473aff */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00473b00 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00473b01 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00473b02 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473b05 */
    f->r[4]+=4;return;
mi_00473b06:;
    /* 00473b06 */
    f->r[1]=0x006fd5d8u;
    /* 00473b0b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473b10u;f->boundary(0x00473b0bu,0x0040d880u,f);
    /* 00473b10 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00473b12 */
    if((f->flags&MI_ZF))goto mi_00473133;
    /* 00473b18 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473b1a */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473b1c */
    a=0x000000f3u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473b21 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473b26u;f->boundary(0x00473b21u,0x004e2970u,f);
    /* 00473b26 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473b29 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00473b2a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00473b2b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00473b2c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00473b2d */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473b30 */
    f->r[4]+=4;return;
mi_00473b31:;
    /* 00473b31 */
    a=f->r[5];b=0x000000e3u;t=mi_alu(f,a,b,32,1);
    /* 00473b37 */
    if(!(f->flags&MI_ZF))goto mi_00473b8e;
    /* 00473b39 */
    f->r[0]=MI_U32((0x00bca300u));
    /* 00473b3e */
    f->r[1]=0x006fd368u;
    /* 00473b43 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473b44 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473b49u;f->boundary(0x00473b44u,0x00416f70u,f);
    /* 00473b49 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00473b4b */
    if(!(f->flags&MI_ZF))goto mi_00473b63;
    /* 00473b4d */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473b4f */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473b51 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473b53 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473b58u;f->boundary(0x00473b53u,0x004e2970u,f);
    /* 00473b58 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473b5b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00473b5c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00473b5d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00473b5e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00473b5f */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473b62 */
    f->r[4]+=4;return;
mi_00473b63:;
    /* 00473b63 */
    f->r[1]=0x006fd368u;
    /* 00473b68 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473b6du;f->boundary(0x00473b68u,0x00416fa0u,f);
mi_00473b6d:;
    /* 00473b6d */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00473b6f */
    if((f->flags&MI_ZF))goto mi_00473133;
    /* 00473b75 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473b77 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473b79 */
    a=0x000000f5u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473b7e */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473b83u;f->boundary(0x00473b7eu,0x004e2970u,f);
    /* 00473b83 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473b86 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00473b87 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00473b88 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00473b89 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00473b8a */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473b8d */
    f->r[4]+=4;return;
mi_00473b8e:;
    /* 00473b8e */
    a=f->r[5];b=0x000000deu;t=mi_alu(f,a,b,32,1);
    /* 00473b94 */
    if(!(f->flags&MI_ZF))goto mi_00473c32;
    /* 00473b9a */
    f->r[2]=MI_U32((0x00bca2d8u));
    /* 00473ba0 */
    f->r[0]=(0x00000000u+f->r[3]+f->r[3]*8u);
    /* 00473ba3 */
    f->r[0]=(0x00000000u+f->r[3]+f->r[0]*2u);
    /* 00473ba6 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 00473ba9 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[1]*4u);
    /* 00473bac */
    f->r[1]=MI_U32((0x00000024u+f->r[4]));
    /* 00473bb0 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00473bb2 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473bb3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473bb8u;f->boundary(0x00473bb3u,0x004def00u,f);
    /* 00473bb8 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473bbb */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00473bbd */
    if(!(f->flags&MI_ZF))goto mi_00473bd5;
    /* 00473bbf */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473bc1 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473bc3 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473bc5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473bcau;f->boundary(0x00473bc5u,0x004e2970u,f);
    /* 00473bca */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473bcd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00473bce */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00473bcf */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00473bd0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00473bd1 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473bd4 */
    f->r[4]+=4;return;
mi_00473bd5:;
    /* 00473bd5 */
    f->r[0]=MI_U32((0x00605a0cu));
    /* 00473bda */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00473bdc */
    if(!(f->flags&MI_ZF))goto mi_00473133;
    /* 00473be2 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 00473be7 */
    f->r[3]=0x00005dc0u;
    /* 00473bec */
    f->r[1]=f->r[0];
    /* 00473bee */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 00473bf4 */
    a=MI_U32((0x00ea61d8u+f->r[1]));b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 00473bfa */
    if(!mi_signed_less(f->flags))goto mi_00473c15;
    /* 00473bfc */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473bfe */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473c00 */
    a=0x000000beu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473c05 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473c0au;f->boundary(0x00473c05u,0x004e2970u,f);
    /* 00473c0a */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473c0d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00473c0e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00473c0f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00473c10 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00473c11 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473c14 */
    f->r[4]+=4;return;
mi_00473c15:;
    /* 00473c15 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473c16 */
    a=0x0000001eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473c18 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473c19 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473c1eu;f->boundary(0x00473c19u,0x0045b4d0u,f);
    /* 00473c1e */
    f->r[0]=MI_U32((0x00bc966cu));
    /* 00473c23 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473c26 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00473c28 */
    MI_U32((0x00bc966cu))=(uint32_t)(f->r[0]);
    /* 00473c2d */
    goto mi_00473133;
mi_00473c32:;
    /* 00473c32 */
    a=f->r[5];b=0x00000041u;t=mi_alu(f,a,b,32,1);
    /* 00473c35 */
    if(mi_signed_less(f->flags))goto mi_00473c80;
    /* 00473c37 */
    a=f->r[5];b=0x00000045u;t=mi_alu(f,a,b,32,1);
    /* 00473c3a */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00473c80;
    /* 00473c3c */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 00473c42 */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 00473c48 */
    a=MI_U16((0x00ea5ce8u+f->r[2]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 00473c50 */
    if(!(f->flags&MI_ZF))goto mi_00473e47;
    /* 00473c56 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473c57 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473c58 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473c5du;f->boundary(0x00473c58u,0x004c3730u,f);
    /* 00473c5d */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473c60 */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,1);
    /* 00473c63 */
    if(!(f->flags&MI_ZF))goto mi_00473c70;
    /* 00473c65 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473c67 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473c69 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473c6b */
    goto mi_00473e4d;
mi_00473c70:;
    /* 00473c70 */
    a=f->r[0];b=0xfffffffeu;t=mi_alu(f,a,b,32,1);
    /* 00473c73 */
    if(!(f->flags&MI_ZF))goto mi_00473c80;
    /* 00473c75 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473c77 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473c79 */
    a=0x0000003du;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473c7b */
    goto mi_00473e4d;
mi_00473c80:;
    /* 00473c80 */
    a=f->r[5];b=0x000000d3u;t=mi_alu(f,a,b,32,1);
    /* 00473c86 */
    if(!(f->flags&MI_ZF))goto mi_00473d20;
    /* 00473c8c */
    f->r[0]=MI_U32((0x00bca300u));
    /* 00473c91 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00b4e6fcu+f->r[0]*2u));
    /* 00473c99 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473c9a */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473c9fu;f->boundary(0x00473c9au,0x00475b50u,f);
    /* 00473c9f */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 00473ca5 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473ca8 */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 00473cae */
    a=MI_U16((0x00ea5ce8u+f->r[1]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 00473cb6 */
    if(!(f->flags&MI_ZF))goto mi_00473cc3;
    /* 00473cb8 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473cba */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473cbc */
    a=0x00000049u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473cbe */
    goto mi_00473e4d;
mi_00473cc3:;
    /* 00473cc3 */
    f->r[2]=f->r[0];
    /* 00473cc5 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 00473cc8 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00473cca */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e8u+f->r[2]*8u));
    /* 00473cd2 */
    a=f->r[0];b=0x00000041u;t=mi_alu(f,a,b,32,1);
    /* 00473cd5 */
    if(mi_signed_less(f->flags))goto mi_00473cdc;
    /* 00473cd7 */
    a=f->r[0];b=0x00000045u;t=mi_alu(f,a,b,32,1);
    /* 00473cda */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00473ce7;
mi_00473cdc:;
    /* 00473cdc */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473cde */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473ce0 */
    a=0x0000004bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473ce2 */
    goto mi_00473e4d;
mi_00473ce7:;
    /* 00473ce7 */
    a=MI_U16((0x00ea5cfau+f->r[1]));b=0x00000002u;t=mi_alu(f,a,b,16,1);
    /* 00473cef */
    if(!mi_signed_less(f->flags))goto mi_00473cfc;
    /* 00473cf1 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473cf3 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473cf5 */
    a=0x0000004eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473cf7 */
    goto mi_00473e4d;
mi_00473cfc:;
    /* 00473cfc */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
mi_00473cfe:;
    /* 00473cfe */
    f->r[0]=MI_U32((0x00ea5f9cu+f->r[1]));
    /* 00473d04 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473d05 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473d0au;f->boundary(0x00473d05u,0x004c52d0u,f);
    /* 00473d0a */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473d0d */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00473d0f */
    if((f->flags&MI_ZF))goto mi_00473133;
    /* 00473d15 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473d17 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473d19 */
    a=0x0000004au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473d1b */
    goto mi_00473e4d;
mi_00473d20:;
    /* 00473d20 */
    a=f->r[5];b=0x000000d4u;t=mi_alu(f,a,b,32,1);
    /* 00473d26 */
    if(!(f->flags&MI_ZF))goto mi_00473da0;
    /* 00473d28 */
    f->r[1]=MI_U32((0x00bca300u));
    /* 00473d2e */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00b4e6fcu+f->r[1]*2u));
    /* 00473d36 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473d37 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473d3cu;f->boundary(0x00473d37u,0x00475b50u,f);
    /* 00473d3c */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 00473d42 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473d45 */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 00473d4b */
    a=MI_U16((0x00ea5ce8u+f->r[1]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 00473d53 */
    if(!(f->flags&MI_ZF))goto mi_00473d60;
    /* 00473d55 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473d57 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473d59 */
    a=0x00000049u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473d5b */
    goto mi_00473e4d;
mi_00473d60:;
    /* 00473d60 */
    f->r[2]=f->r[0];
    /* 00473d62 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 00473d65 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00473d67 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e8u+f->r[2]*8u));
    /* 00473d6f */
    a=f->r[0];b=0x00000041u;t=mi_alu(f,a,b,32,1);
    /* 00473d72 */
    if(mi_signed_less(f->flags))goto mi_00473d79;
    /* 00473d74 */
    a=f->r[0];b=0x00000045u;t=mi_alu(f,a,b,32,1);
    /* 00473d77 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00473d84;
mi_00473d79:;
    /* 00473d79 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473d7b */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473d7d */
    a=0x0000004bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473d7f */
    goto mi_00473e4d;
mi_00473d84:;
    /* 00473d84 */
    a=MI_U16((0x00ea5cfau+f->r[1]));b=0x00000002u;t=mi_alu(f,a,b,16,1);
    /* 00473d8c */
    if(!mi_signed_less(f->flags))goto mi_00473d99;
    /* 00473d8e */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473d90 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473d92 */
    a=0x0000004eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473d94 */
    goto mi_00473e4d;
mi_00473d99:;
    /* 00473d99 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473d9b */
    goto mi_00473cfe;
mi_00473da0:;
    /* 00473da0 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 00473da5 */
    a=f->r[5];b=0x000000bdu;t=mi_alu(f,a,b,32,1);
    /* 00473dab */
    if((f->flags&MI_ZF))goto mi_00473dbd;
    /* 00473dad */
    a=f->r[5];b=0x000000bcu;t=mi_alu(f,a,b,32,1);
    /* 00473db3 */
    if((f->flags&MI_ZF))goto mi_00473dbd;
    /* 00473db5 */
    a=f->r[5];b=0x000000bbu;t=mi_alu(f,a,b,32,1);
    /* 00473dbb */
    if(!(f->flags&MI_ZF))goto mi_00473dcf;
mi_00473dbd:;
    /* 00473dbd */
    f->r[1]=f->r[0];
    /* 00473dbf */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 00473dc5 */
    a=MI_U16((0x00ea5cf6u+f->r[1]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 00473dcd */
    if(!(f->flags&MI_ZF))goto mi_00473e47;
mi_00473dcf:;
    /* 00473dcf */
    a=f->r[5];b=0x0000004fu;t=mi_alu(f,a,b,32,1);
    /* 00473dd2 */
    if((f->flags&MI_ZF))goto mi_00473dde;
    /* 00473dd4 */
    a=f->r[5];b=0x0000004eu;t=mi_alu(f,a,b,32,1);
    /* 00473dd7 */
    if((f->flags&MI_ZF))goto mi_00473dde;
    /* 00473dd9 */
    a=f->r[5];b=0x0000004du;t=mi_alu(f,a,b,32,1);
    /* 00473ddc */
    if(!(f->flags&MI_ZF))goto mi_00473df0;
mi_00473dde:;
    /* 00473dde */
    f->r[2]=f->r[0];
    /* 00473de0 */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 00473de6 */
    f->r[1]=MI_U32((0x00ea620cu+f->r[2]));
    /* 00473dec */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00473dee */
    if(!(f->flags&MI_ZF))goto mi_00473e47;
mi_00473df0:;
    /* 00473df0 */
    a=f->r[5];b=0x00000056u;t=mi_alu(f,a,b,32,1);
    /* 00473df3 */
    if(!(f->flags&MI_ZF))goto mi_00473e11;
    /* 00473df5 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 00473dfb */
    a=MI_U16((0x00ea5cf6u+f->r[0]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 00473e03 */
    if(!(f->flags&MI_ZF))goto mi_00473133;
    /* 00473e09 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473e0b */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473e0d */
    a=0x00000059u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473e0f */
    goto mi_00473e4d;
mi_00473e11:;
    /* 00473e11 */
    a=f->r[5];b=0x0000005fu;t=mi_alu(f,a,b,32,1);
    /* 00473e14 */
    if(!(f->flags&MI_ZF))goto mi_00473e2e;
    /* 00473e16 */
    f->r[0]=MI_U32((0x00705c6cu));
    /* 00473e1b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00473e1d */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00473e47;
    /* 00473e1f */
    MI_U32((0x00705c6cu))=(uint32_t)(0x00000001u);
    /* 00473e29 */
    goto mi_00473133;
mi_00473e2e:;
    /* 00473e2e */
    a=f->r[5];b=0x0000005eu;t=mi_alu(f,a,b,32,1);
    /* 00473e31 */
    if(!(f->flags&MI_ZF))goto mi_00473e71;
    /* 00473e33 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 00473e39 */
    f->r[1]=MI_U32((0x00ea6240u+f->r[0]));
    /* 00473e3f */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00473e41 */
    if((f->flags&MI_ZF))goto mi_00473133;
mi_00473e47:;
    /* 00473e47 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473e49 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473e4b */
    a=0x00000004u;f->r[4]-=4;MI_U32(f->r[4])=a;
mi_00473e4d:;
    /* 00473e4d */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473e52u;f->boundary(0x00473e4du,0x004e2970u,f);
    /* 00473e52 */
    a=0x000000afu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473e57 */
    a=0x0000cb10u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473e5c */
    a=0x00b285ccu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473e61 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473e66u;f->boundary(0x00473e61u,0x0040f920u,f);
    /* 00473e66 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473e69 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00473e6a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00473e6b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00473e6c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00473e6d */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473e70 */
    f->r[4]+=4;return;
mi_00473e71:;
    /* 00473e71 */
    a=f->r[5];b=0x000000b7u;t=mi_alu(f,a,b,32,1);
    /* 00473e77 */
    if(!(f->flags&MI_ZF))goto mi_00473133;
    /* 00473e7d */
    f->r[2]=MI_U32((0x00bca2d8u));
    /* 00473e83 */
    f->r[0]=(0x00000000u+f->r[6]+f->r[6]*8u);
    /* 00473e86 */
    f->r[0]=(0x00000000u+f->r[6]+f->r[0]*2u);
    /* 00473e89 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 00473e8c */
    f->r[0]=(0x00000000u+f->r[2]+f->r[1]*4u);
    /* 00473e8f */
    f->r[2]=MI_U32((0x005e6970u));
    /* 00473e95 */
    f->r[1]=(0x00000024u+f->r[4]);
    /* 00473e99 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00473e9b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473e9c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473e9d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473e9e */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473ea3u;f->boundary(0x00473e9eu,0x004ecec0u,f);
    /* 00473ea3 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473ea6 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00473ea8 */
    if(!(f->flags&MI_ZF))goto mi_00473133;
    /* 00473eae */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 00473eb2 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473eb4 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 00473eb7 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473eb9 */
    if(!(f->flags&MI_ZF))goto mi_00473ed0;
    /* 00473ebb */
    a=0x000000d6u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473ec0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473ec5u;f->boundary(0x00473ec0u,0x004e2970u,f);
    /* 00473ec5 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473ec8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00473ec9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00473eca */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00473ecb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00473ecc */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473ecf */
    f->r[4]+=4;return;
mi_00473ed0:;
    /* 00473ed0 */
    a=0x000000d5u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473ed5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473edau;f->boundary(0x00473ed5u,0x004e2970u,f);
    /* 00473eda */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473edd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00473ede */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00473edf */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00473ee0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00473ee1 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473ee4 */
    f->r[4]+=4;return;
mi_00473ee5:;
    /* 00473ee5 */
    a=f->r[5];b=0x000000d5u;t=mi_alu(f,a,b,32,1);
    /* 00473eeb */
    if(!(f->flags&MI_ZF))goto mi_00473f0c;
    /* 00473eed */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473eef */
    a=0x00000064u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473ef1 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473ef2 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473ef3 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
mi_00473ef4:;
    /* 00473ef4 */
    f->r[1]=MI_U32((0x00000030u+f->r[4]));
    /* 00473ef8 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473ef9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473efeu;f->boundary(0x00473ef9u,0x0046a400u,f);
    /* 00473efe */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473f01 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00473f05 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 00473f07 */
    goto mi_00474061;
mi_00473f0c:;
    /* 00473f0c */
    a=f->r[5];b=0x000000e6u;t=mi_alu(f,a,b,32,1);
    /* 00473f12 */
    if(!(f->flags&MI_ZF))goto mi_00473f33;
    /* 00473f14 */
    f->r[2]=MI_U32((0x0000001cu+f->r[4]));
    /* 00473f18 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473f1a */
    a=0x00000010u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473f1c */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473f1d */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473f1e */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473f1f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473f20 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473f25u;f->boundary(0x00473f20u,0x0046a400u,f);
    /* 00473f25 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473f28 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00473f2c */
    f->r[3]=(f->r[3]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 00473f2e */
    goto mi_00474061;
mi_00473f33:;
    /* 00473f33 */
    a=f->r[5];b=0x000000e1u;t=mi_alu(f,a,b,32,1);
    /* 00473f39 */
    if(!(f->flags&MI_ZF))goto mi_00473f5a;
    /* 00473f3b */
    f->r[0]=MI_U32((0x0000001cu+f->r[4]));
    /* 00473f3f */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473f41 */
    a=0x00000019u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473f43 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473f44 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473f45 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473f46 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473f47 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473f4cu;f->boundary(0x00473f47u,0x0046a400u,f);
    /* 00473f4c */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473f4f */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00473f53 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 00473f55 */
    goto mi_00474061;
mi_00473f5a:;
    /* 00473f5a */
    a=f->r[5];b=0x000000e3u;t=mi_alu(f,a,b,32,1);
    /* 00473f60 */
    if(!(f->flags&MI_ZF))goto mi_00473f6b;
    /* 00473f62 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473f64 */
    a=0x00000009u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473f66 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473f67 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473f68 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473f69 */
    goto mi_00473ef4;
mi_00473f6b:;
    /* 00473f6b */
    a=f->r[5];b=0x000000e5u;t=mi_alu(f,a,b,32,1);
    /* 00473f71 */
    if(!(f->flags&MI_ZF))goto mi_00473f95;
    /* 00473f73 */
    f->r[2]=MI_U32((0x0000001cu+f->r[4]));
    /* 00473f77 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473f79 */
    a=0x000000a7u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473f7e */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473f7f */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473f80 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473f81 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473f82 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473f87u;f->boundary(0x00473f82u,0x0046a400u,f);
    /* 00473f87 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473f8a */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00473f8e */
    f->r[3]=(f->r[3]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 00473f90 */
    goto mi_00474061;
mi_00473f95:;
    /* 00473f95 */
    a=f->r[5];b=0x000000eau;t=mi_alu(f,a,b,32,1);
    /* 00473f9b */
    if(!(f->flags&MI_ZF))goto mi_00473fbf;
    /* 00473f9d */
    f->r[0]=MI_U32((0x0000001cu+f->r[4]));
    /* 00473fa1 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473fa3 */
    a=0x0000009eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473fa8 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473fa9 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473faa */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473fab */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473fac */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473fb1u;f->boundary(0x00473facu,0x0046a400u,f);
    /* 00473fb1 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473fb4 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00473fb8 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 00473fba */
    goto mi_00474061;
mi_00473fbf:;
    /* 00473fbf */
    a=f->r[5];b=0x000000ebu;t=mi_alu(f,a,b,32,1);
    /* 00473fc5 */
    if(!(f->flags&MI_ZF))goto mi_00473fd6;
    /* 00473fc7 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473fc9 */
    a=0x000001b5u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473fce */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473fcf */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473fd0 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473fd1 */
    goto mi_00473ef4;
mi_00473fd6:;
    /* 00473fd6 */
    a=f->r[5];b=0x000000ecu;t=mi_alu(f,a,b,32,1);
    /* 00473fdc */
    if(!(f->flags&MI_ZF))goto mi_00473ffd;
    /* 00473fde */
    f->r[2]=MI_U32((0x0000001cu+f->r[4]));
    /* 00473fe2 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473fe4 */
    a=0x00000201u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473fe9 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473fea */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473feb */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473fec */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00473fed */
    f->r[4]-=4;MI_U32(f->r[4])=0x00473ff2u;f->boundary(0x00473fedu,0x0046a400u,f);
    /* 00473ff2 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00473ff5 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00473ff9 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 00473ffb */
    goto mi_00474061;
mi_00473ffd:;
    /* 00473ffd */
    a=f->r[5];b=0x000000e4u;t=mi_alu(f,a,b,32,1);
    /* 00474003 */
    if(!(f->flags&MI_ZF))goto mi_00474021;
    /* 00474005 */
    f->r[0]=MI_U32((0x0000001cu+f->r[4]));
    /* 00474009 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047400b */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047400d */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047400e */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047400f */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474010 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474011 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00474016u;f->boundary(0x00474011u,0x0046a400u,f);
    /* 00474016 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00474019 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0047401d */
    f->r[3]=(f->r[3]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 0047401f */
    goto mi_00474061;
mi_00474021:;
    /* 00474021 */
    a=f->r[5];b=0x000000deu;t=mi_alu(f,a,b,32,1);
    /* 00474027 */
    if(!(f->flags&MI_ZF))goto mi_00474035;
    /* 00474029 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047402b */
    a=0x00000074u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047402d */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047402e */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047402f */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474030 */
    goto mi_00473ef4;
mi_00474035:;
    /* 00474035 */
    a=f->r[5];b=0x0000005au;t=mi_alu(f,a,b,32,1);
    /* 00474038 */
    if((f->flags&MI_ZF))goto mi_00474061;
    /* 0047403a */
    f->r[2]=MI_U32((0x0000001cu+f->r[4]));
    /* 0047403e */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474040 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474042 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474043 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474044 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474045 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474046 */
    goto mi_00474055;
mi_00474048:;
    /* 00474048 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047404a */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047404c */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047404d */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047404e */
    a=0x00000039u;f->r[4]-=4;MI_U32(f->r[4])=a;
mi_00474050:;
    /* 00474050 */
    f->r[0]=MI_U32((0x00000030u+f->r[4]));
    /* 00474054 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
mi_00474055:;
    /* 00474055 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047405au;f->boundary(0x00474055u,0x0046a400u,f);
    /* 0047405a */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047405d */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
mi_00474061:;
    /* 00474061 */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 00474065 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00474067 */
    if((f->flags&MI_ZF))goto mi_00474119;
    /* 0047406d */
    a=(f->r[3]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0047406f */
    if(!(f->flags&MI_ZF))goto mi_0047407c;
    /* 00474071 */
    f->r[1]=f->r[0];
    /* 00474073 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474074 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00474079u;f->boundary(0x00474074u,0x00485100u,f);
    /* 00474079 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0047407c:;
    /* 0047407c */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 00474080 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00474082 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00474119;
    /* 00474088 */
    f->r[2]=MI_U32((0x00bc96d0u));
    /* 0047408e */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474090 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474092 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474093 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474094 */
    f->r[3]=f->r[0];
    /* 00474096 */
    f->r[0]=MI_U32((0x0000002cu+f->r[4]));
    /* 0047409a */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047409b */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047409c */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047409d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047409e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004740a3u;f->boundary(0x0047409eu,0x00470990u,f);
    /* 004740a3 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004740a6 */
    a=f->r[5];b=0x00000088u;t=mi_alu(f,a,b,32,1);
    /* 004740ac */
    if(!(f->flags&MI_ZF))goto mi_004740b7;
    /* 004740ae */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004740af */
    f->r[4]-=4;MI_U32(f->r[4])=0x004740b4u;f->boundary(0x004740afu,0x00486020u,f);
mi_004740b4:;
    /* 004740b4 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004740b7:;
    /* 004740b7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004740bcu;f->boundary(0x004740b7u,0x00517370u,f);
    /* 004740bc */
    f->r[4]-=4;MI_U32(f->r[4])=0x004740c1u;f->boundary(0x004740bcu,0x00517660u,f);
    /* 004740c1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004740c6u;f->boundary(0x004740c1u,0x005178e0u,f);
    /* 004740c6 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004740c7 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004740c8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004740cdu;f->boundary(0x004740c8u,0x004b9ee0u,f);
    /* 004740cd */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004740d0 */
    a=f->r[5];b=0x00000088u;t=mi_alu(f,a,b,32,1);
    /* 004740d6 */
    if(!(f->flags&MI_ZF))goto mi_004740e5;
    /* 004740d8 */
    f->r[0]=MI_U32((0x00bca310u));
    /* 004740dd */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004740df */
    if(!(f->flags&MI_ZF))goto mi_004740e5;
    /* 004740e1 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004740e3 */
    goto mi_004740ea;
mi_004740e5:;
    /* 004740e5 */
    f->r[0]=MI_U32((0x00bc9728u));
mi_004740ea:;
    /* 004740ea */
    f->r[7]=MI_U32((0x00bc9718u));
    /* 004740f0 */
    f->r[1]=MI_U32((0x0000001cu+f->r[4]));
    /* 004740f4 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004740f6 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004740f7 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004740f8 */
    MI_U32((0x00bc9728u))=(uint32_t)(f->r[0]);
    /* 004740fd */
    f->r[4]-=4;MI_U32(f->r[4])=0x00474102u;f->boundary(0x004740fdu,0x004e9680u,f);
    /* 00474102 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00474105 */
    a=f->r[5];b=0x00000038u;t=mi_alu(f,a,b,32,1);
    /* 00474108 */
    if((f->flags&MI_ZF))goto mi_00474114;
    /* 0047410a */
    a=f->r[5];b=0x00000009u;t=mi_alu(f,a,b,32,1);
    /* 0047410d */
    if((f->flags&MI_ZF))goto mi_00474114;
    /* 0047410f */
    f->r[4]-=4;MI_U32(f->r[4])=0x00474114u;f->boundary(0x0047410fu,0x00485200u,f);
mi_00474114:;
    /* 00474114 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00474119u;f->boundary(0x00474114u,0x004f61e0u,f);
mi_00474119:;
    /* 00474119 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0047411a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0047411b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0047411c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0047411d */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00474120 */
    f->r[4]+=4;return;
}

