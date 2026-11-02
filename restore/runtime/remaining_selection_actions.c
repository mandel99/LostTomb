/* Pinned PE SHA256 fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d.
   Whole bodies:409DA0..409DF5(23),40A6C0..40A72F(32),
   40AB00..40AC49(76),40B4E0..40B51B(17),40D130..40D198(25),
   547A60..547AC5(36). No alignment padding or jump tables are included.
   The final helper is a direct instruction-to-C translation. */
#include "remaining_selection_actions.h"
#include "building_create_model.h"
#include "placement_support.h"
#include <stdlib.h>

typedef char remaining_selection_pointer32[sizeof(void *)==4?1:-1];

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

void restore_remaining_selection_C_00547a60(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00547a60 */
    f->r[0]=MI_U32((0x0078d790u));
    /* 00547a65 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00547a66 */
    f->r[6]=0x00000001u;
    /* 00547a6b */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 00547a6d */
    if(mi_signed_less(f->flags))goto mi_00547ac4;
    /* 00547a6f */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00547a70 */
    f->r[7]=0x00799af0u;
mi_00547a75:;
    /* 00547a75 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00000000u+f->r[7])))&0xffffu)<<0);
    /* 00547a78 */
    a=(f->r[0]&0xffffu);b=0x000000e5u;t=mi_alu(f,a,b,16,1);
    /* 00547a7c */
    if(!(f->flags&MI_ZF))goto mi_00547a86;
    /* 00547a7e */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00547a7f */
    f->r[1]=0x006f9320u;
    /* 00547a84 */
    goto mi_00547aae;
mi_00547a86:;
    /* 00547a86 */
    a=(f->r[0]&0xffffu);b=0x000000eau;t=mi_alu(f,a,b,16,1);
    /* 00547a8a */
    if(!(f->flags&MI_ZF))goto mi_00547a94;
    /* 00547a8c */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00547a8d */
    f->r[1]=0x006f5648u;
    /* 00547a92 */
    goto mi_00547aae;
mi_00547a94:;
    /* 00547a94 */
    a=(f->r[0]&0xffffu);b=0x000000ebu;t=mi_alu(f,a,b,16,1);
    /* 00547a98 */
    if(!(f->flags&MI_ZF))goto mi_00547aa2;
    /* 00547a9a */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00547a9b */
    f->r[1]=0x006de9e0u;
    /* 00547aa0 */
    goto mi_00547aae;
mi_00547aa2:;
    /* 00547aa2 */
    a=(f->r[0]&0xffffu);b=0x000000ecu;t=mi_alu(f,a,b,16,1);
    /* 00547aa6 */
    if(!(f->flags&MI_ZF))goto mi_00547ab3;
    /* 00547aa8 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00547aa9 */
    f->r[1]=0x006e9188u;
mi_00547aae:;
    /* 00547aae */
    f->r[4]-=4;MI_U32(f->r[4])=0x00547ab3u;f->boundary(0x00547aaeu,0x005468d0u,f);
mi_00547ab3:;
    /* 00547ab3 */
    f->r[0]=MI_U32((0x0078d790u));
    /* 00547ab8 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 00547ab9 */
    a=f->r[7];b=0x00000108u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 00547abf */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00547ac1 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00547a75;
    /* 00547ac3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
mi_00547ac4:;
    /* 00547ac4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00547ac5 */
    f->r[4]+=4;return;
}

void restore_remaining_selection_00547a60(void)
{
    uint32_t stack[1024];
    struct restore_menu_input_frame f={{0},2,restore_building_create_dispatch};
    /* Natural scratch belongs to the actual translated member body. This
       is a synthetic return slot, not a replacement for a game field. */
    stack[768]=0;f.r[4]=(uint32_t)(uintptr_t)(stack+768);
    restore_remaining_selection_C_00547a60(&f);
}

/* Whole 409DA0,23 instructions,tail-dispatching the whole 36-instruction
   547A60 above. The selected-value table retains its original address. */
void restore_remaining_selection_00409da0(void)
{
    uint32_t selected=MI_U32(0xd3b198u);
    uint32_t value=MI_U32(0x5cf734u+selected*4u);
    if(value==0u){
        uint32_t previous=MI_U32(0xe94e0cu);
        MI_U32(0xe94e0cu)=0;MI_U32(0xe94e10u)=previous;
    }else if(value==4u||value==0x2du||value==0x1fu||value==0x33u){
        MI_U32(0xe94e10u)=0;MI_U32(0xe94e0cu)=value;
    }
    MI_U8(0xe38ddau)=1;MI_U8(0xe38dcau)=1;
    restore_remaining_selection_00547a60();
}

/* Whole 40A6C0,32 instructions. Count positive DWORDs in nine 20-byte
   records, then account for the selected record using a nonzero test. */
void restore_remaining_selection_0040a6c0(void)
{
    uint32_t base=MI_U32(0xeaac28u)*0x49d8u,count=0,index,selected;
    selected=MI_U32(0xd3b198u);MI_U32(0x605988u)=selected;
    MI_U32(0x605984u)=0;
    for(index=0;index<9u;++index){
        if((int32_t)MI_U32(base+0xea43acu+index*20u)>0)++count;
    }
    MI_U32(0x605984u)=count;
    if((int32_t)count<9){
        ++count;MI_U32(0x605984u)=count;
        if(MI_U32(base+0xea43acu+selected*20u)!=0u){
            --count;MI_U32(0x605984u)=count;
        }
    }
    MI_U8(0xe38ddau)=0x2a;MI_U8(0xe38dcau)=1;
}

/* Whole 40AB00,76 instructions. Every arithmetic field is a DWORD; the
   balance gate is signed and additions/subtractions wrap at 32 bits. */
void restore_remaining_selection_0040ab00(void)
{
    uint32_t player=MI_U32(0xeaac28u),base=player*0x49d8u,value,kind,cost;
    if((int32_t)MI_U32(base+0xea187cu)<=-5000)return;
    value=MI_U32(base+0xea5d60u);MI_U32(base+0xea5d70u)=value;
    value=MI_U32(base+0xea5d64u);MI_U32(base+0xea5d68u)=value;
    kind=MI_U32(base+0xea5d64u);
    if(kind==1u)MI_U32(base+0xea5d6cu)=2;
    else if(kind==2u)MI_U32(base+0xea5d6cu)=3;
    else MI_U32(base+0xea5d6cu)=4;
    value=MI_U32(base+0xea5d98u);
    if(value!=0u){
        if(MI_U32(base+0xea5d9cu)!=0u){
            uint32_t duration=MI_U32(base+0xea5d6cu);
            MI_U32(base+0xea5d6cu)=duration+value;
        }else MI_U32(base+0xea5d9cu)=12;
    }else MI_U32(base+0xea5d98u)=12;
    kind=MI_U32(base+0xea5d64u);
    cost=kind==1u?0xea5d74u:kind==2u?0xea5d78u:0xea5d7cu;
    value=MI_U32(base+cost);
    MI_U32(base+0xea187cu)=MI_U32(base+0xea187cu)-value;
    value=MI_U32(base+cost);
    MI_U32(base+0xea5fc8u)=MI_U32(base+0xea5fc8u)+value;
    if(MI_U32(base+0xea5d64u)==3u){
        value=MI_U32(base+0xea5d80u);
        (void)FUN_0045b4d0((int32_t)player,15,(int32_t)value);
    }
    value=MI_U32(0xd3b194u);
    MI_U8(0xe38ddau)=7;
    if(value!=0u)MI_U8(0xe38ddau)=9;
    MI_U8(0xe38dcau)=1;
}

/* Whole 40B4E0,17 instructions. The mode DWORD bypasses the BYTE table
   gate; selected zero returns to mode 1 without writing either selection. */
void restore_remaining_selection_0040b4e0(void)
{
    uint32_t selected=MI_U32(0xd3b198u);
    if(selected==0u){
        MI_U8(0xe38ddau)=1;MI_U8(0xe38dcau)=1;return;
    }
    if(MI_U32(0xe38ebcu)==0u&&MI_U8(0x6fdc88u+selected)==0u)return;
    MI_U32(0x605a44u)=selected;MI_U32(0xe38ea4u)=selected;
    MI_U8(0xe38dcau)=1;
}

/* Whole 40D130,25 instructions. The signed limit follows wrapping DWORD
   addition. Native modes other than 0/1 deliberately bypass both gates. */
void restore_remaining_selection_0040d130(void)
{
    uint32_t selected=MI_U32(0xd3b198u),base,value;
    if(selected==1u){
        base=MI_U32(0xeaac28u)*0x49d8u;
        if(MI_U32(base+0xea3fb0u)==0u){
            value=MI_U32(base+0xea5eb4u);
            value+=MI_U32(base+0xea5ea4u);
            if((int32_t)value>3)return;
        }
    }else if(selected==0u){
        base=MI_U32(0xeaac28u)*0x49d8u;
        if(MI_U32(base+0xea1880u)==100u)return;
    }
    MI_U8(0xe38ddau)=0x4f;MI_U8(0xe38dcau)=1;
}

int restore_remaining_selection_action(uint32_t token)
{
    switch(token){
    case 0x409da0u:restore_remaining_selection_00409da0();return 1;
    case 0x40a6c0u:restore_remaining_selection_0040a6c0();return 1;
    case 0x40ab00u:restore_remaining_selection_0040ab00();return 1;
    case 0x40b4e0u:restore_remaining_selection_0040b4e0();return 1;
    case 0x40d130u:restore_remaining_selection_0040d130();return 1;
    default:return 0;
    }
}
