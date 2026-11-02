/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "remaining_warnings.h"
#include "training_queries.h"
#include "pending_message.h"
#include "map_commit_selection.h"
/* Existing443900 is a complete BYTE/98-record scan over genuine443000. */
extern uint32_t FUN_00443900(uint32_t resource);
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
static __inline uint32_t mi_alu(struct restore_warning_frame *f,uint32_t a,uint32_t b,uint32_t width,uint32_t operation)
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
static __inline uint32_t mi_shift(struct restore_warning_frame *f,uint32_t a,uint32_t count,uint32_t width,uint32_t arithmetic)
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
static __inline uint32_t mi_imul(struct restore_warning_frame *f,uint32_t a,uint32_t b)
{
 int64_t product=(int64_t)(int32_t)a*(int32_t)b;uint32_t result=(uint32_t)product;
 f->flags&=~(MI_CF|MI_OF);if(product!=(int64_t)(int32_t)result)f->flags|=MI_CF|MI_OF;
 return result;
}
static __inline int mi_signed_less(uint32_t flags){return ((flags&MI_SF)!=0)^((flags&MI_OF)!=0);}

static const uint8_t mi_rom_004e4df0[]={0x04,0x48,0x4e,0x00,0x50,0x4a,0x4e,0x00,0xad,0x4c,0x4e,0x00,0x04,0x48,0x4e,0x00,0x4d,0x4d,0x4e,0x00};
static __inline uint32_t mi_read(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x004e4df0u&&width<=20u&&(offset=address-0x004e4df0u)<=20u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004e4df0[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}

void restore_warning_C_004e47d0(struct restore_warning_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004e47d0 */
    f->r[0]=mi_read((0x00605a3cu),4u);
    /* 004e47d5 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e47d6 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004e47d9 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e47da */
    if((f->flags&MI_ZF))goto mi_004e4dec;
    /* 004e47e0 */
    a=mi_read((0x0000000cu+f->r[4]),4u);b=0x000000b7u;t=mi_alu(f,a,b,32,1);
    /* 004e47e8 */
    if(!(f->flags&MI_ZF))goto mi_004e4dec;
    /* 004e47ee */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e47f3u;f->boundary(0x004e47eeu,0x004f44e0u,f);
    /* 004e47f3 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004e47f4 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 004e47f7 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004e4dec;
    /* 004e47fd */
    switch(mi_read((0x004e4df0u+f->r[0]*4u),4u)){case 0x004e4804u:goto mi_004e4804;case 0x004e4a50u:goto mi_004e4a50;case 0x004e4cadu:goto mi_004e4cad;case 0x004e4d4du:goto mi_004e4d4d;default:abort();}
mi_004e4804:;
    /* 004e4804 */
    f->r[2]=mi_read((0x0078d6acu),4u);
    /* 004e480a */
    f->r[6]=mi_read((0x0078d6b0u),4u);
    /* 004e4810 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 004e4812 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e481c;
    /* 004e4814 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004e4816 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e4dec;
mi_004e481c:;
    /* 004e481c */
    f->r[0]=mi_read((0x00eaac28u),4u);
    /* 004e4821 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004e4827 */
    f->r[1]=mi_read((0x00ea61c0u+f->r[0]),4u);
    /* 004e482d */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004e482f */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e483f;
    /* 004e4831 */
    f->r[7]=mi_read((0x00ea61c4u+f->r[0]),4u);
    /* 004e4837 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 004e4839 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e4dec;
mi_004e483f:;
    /* 004e483f */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 004e4841 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e49c1;
    /* 004e4847 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004e4849 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e49c1;
    /* 004e484f */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004e4851 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e4943;
    /* 004e4857 */
    f->r[2]=mi_read((0x00ea61c4u+f->r[0]),4u);
    /* 004e485d */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 004e485f */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e4943;
    /* 004e4865 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4867 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4869 */
    a=0x00000096u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e486e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4873u;f->boundary(0x004e486eu,0x004e2970u,f);
    /* 004e4873 */
    a=0x00000018u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4875 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e487au;f->boundary(0x004e4875u,0x00443b90u,f);
    /* 004e487a */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e487d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e487f */
    if((f->flags&MI_ZF))goto mi_004e488c;
    /* 004e4881 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4883 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4885 */
    a=0x00000097u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e488a */
    goto mi_004e48cc;
mi_004e488c:;
    /* 004e488c */
    a=0x00000018u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e488e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4893u;f->boundary(0x004e488eu,0x00443950u,f);
    /* 004e4893 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4896 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e4898 */
    if((f->flags&MI_ZF))goto mi_004e48c3;
    /* 004e489a */
    f->r[0]=mi_read((0x00eaac28u),4u);
    /* 004e489f */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004e48a5 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x00ea40c6u+f->r[0]),2u))&0xffffu)<<0);
    /* 004e48ac */
    a=(f->r[0]&0xffffu);b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 004e48b0 */
    if((f->flags&MI_ZF))goto mi_004e48d4;
    /* 004e48b2 */
    a=(f->r[0]&0xffffu);b=0x00000003u;t=mi_alu(f,a,b,16,1);
    /* 004e48b6 */
    if((f->flags&MI_ZF))goto mi_004e48d4;
    /* 004e48b8 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e48ba */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e48bc */
    a=0x00000099u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e48c1 */
    goto mi_004e48cc;
mi_004e48c3:;
    /* 004e48c3 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e48c5 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e48c7 */
    a=0x00000098u;f->r[4]-=4;MI_U32(f->r[4])=a;
mi_004e48cc:;
    /* 004e48cc */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e48d1u;f->boundary(0x004e48ccu,0x004e2970u,f);
    /* 004e48d1 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004e48d4:;
    /* 004e48d4 */
    a=0x00000019u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e48d6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e48dbu;f->boundary(0x004e48d6u,0x00443b90u,f);
    /* 004e48db */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e48de */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e48e0 */
    if((f->flags&MI_ZF))goto mi_004e48f6;
    /* 004e48e2 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e48e4 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e48e6 */
    a=0x0000009au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e48eb */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e48f0u;f->boundary(0x004e48ebu,0x004e2970u,f);
    /* 004e48f0 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e48f3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004e48f4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e48f5 */
    f->r[4]+=4;return;
mi_004e48f6:;
    /* 004e48f6 */
    a=0x00000019u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e48f8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e48fdu;f->boundary(0x004e48f8u,0x00443950u,f);
    /* 004e48fd */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4900 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e4902 */
    if((f->flags&MI_ZF))goto mi_004e4b8c;
    /* 004e4908 */
    f->r[1]=mi_read((0x00eaac28u),4u);
    /* 004e490e */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 004e4914 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x00ea40c8u+f->r[1]),2u))&0xffffu)<<0);
    /* 004e491b */
    a=(f->r[0]&0xffffu);b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 004e491f */
    if((f->flags&MI_ZF))goto mi_004e4dec;
    /* 004e4925 */
    a=(f->r[0]&0xffffu);b=0x00000003u;t=mi_alu(f,a,b,16,1);
    /* 004e4929 */
    if((f->flags&MI_ZF))goto mi_004e4dec;
    /* 004e492f */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4931 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4933 */
    a=0x0000009cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4938 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e493du;f->boundary(0x004e4938u,0x004e2970u,f);
    /* 004e493d */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4940 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004e4941 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e4942 */
    f->r[4]+=4;return;
mi_004e4943:;
    /* 004e4943 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004e4945 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e49c1;
    /* 004e4947 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4949 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e494b */
    a=0x0000009du;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4950 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4955u;f->boundary(0x004e4950u,0x004e2970u,f);
    /* 004e4955 */
    a=0x00000018u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4957 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e495cu;f->boundary(0x004e4957u,0x00443b90u,f);
    /* 004e495c */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e495f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e4961 */
    if((f->flags&MI_ZF))goto mi_004e4977;
    /* 004e4963 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4965 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4967 */
    a=0x00000097u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e496c */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4971u;f->boundary(0x004e496cu,0x004e2970u,f);
    /* 004e4971 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4974 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004e4975 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e4976 */
    f->r[4]+=4;return;
mi_004e4977:;
    /* 004e4977 */
    a=0x00000018u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4979 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e497eu;f->boundary(0x004e4979u,0x00443950u,f);
    /* 004e497e */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4981 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e4983 */
    if((f->flags&MI_ZF))goto mi_004e4dde;
    /* 004e4989 */
    f->r[2]=mi_read((0x00eaac28u),4u);
    /* 004e498f */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 004e4995 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x00ea40c6u+f->r[2]),2u))&0xffffu)<<0);
    /* 004e499c */
    a=(f->r[0]&0xffffu);b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 004e49a0 */
    if((f->flags&MI_ZF))goto mi_004e4dec;
    /* 004e49a6 */
    a=(f->r[0]&0xffffu);b=0x00000003u;t=mi_alu(f,a,b,16,1);
    /* 004e49aa */
    if((f->flags&MI_ZF))goto mi_004e4dec;
    /* 004e49b0 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e49b2 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e49b4 */
    a=0x00000020u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e49b6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e49bbu;f->boundary(0x004e49b6u,0x004e2970u,f);
    /* 004e49bb */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e49be */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004e49bf */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e49c0 */
    f->r[4]+=4;return;
mi_004e49c1:;
    /* 004e49c1 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004e49c3 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e4dec;
    /* 004e49c9 */
    f->r[1]=mi_read((0x00ea61c4u+f->r[0]),4u);
    /* 004e49cf */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004e49d1 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e4dec;
    /* 004e49d7 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e49d9 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e49db */
    a=0x0000009eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e49e0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e49e5u;f->boundary(0x004e49e0u,0x004e2970u,f);
    /* 004e49e5 */
    a=0x00000019u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e49e7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e49ecu;f->boundary(0x004e49e7u,0x00443b90u,f);
    /* 004e49ec */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e49ef */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e49f1 */
    if((f->flags&MI_ZF))goto mi_004e4a07;
    /* 004e49f3 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e49f5 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e49f7 */
    a=0x0000009au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e49fc */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4a01u;f->boundary(0x004e49fcu,0x004e2970u,f);
    /* 004e4a01 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4a04 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004e4a05 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e4a06 */
    f->r[4]+=4;return;
mi_004e4a07:;
    /* 004e4a07 */
    a=0x00000019u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4a09 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4a0eu;f->boundary(0x004e4a09u,0x00443950u,f);
    /* 004e4a0e */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4a11 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e4a13 */
    if((f->flags&MI_ZF))goto mi_004e4dde;
    /* 004e4a19 */
    f->r[0]=mi_read((0x00eaac28u),4u);
    /* 004e4a1e */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004e4a24 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x00ea40c8u+f->r[0]),2u))&0xffffu)<<0);
    /* 004e4a2b */
    a=(f->r[0]&0xffffu);b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 004e4a2f */
    if((f->flags&MI_ZF))goto mi_004e4dec;
    /* 004e4a35 */
    a=(f->r[0]&0xffffu);b=0x00000003u;t=mi_alu(f,a,b,16,1);
    /* 004e4a39 */
    if((f->flags&MI_ZF))goto mi_004e4dec;
    /* 004e4a3f */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4a41 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4a43 */
    a=0x00000020u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4a45 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4a4au;f->boundary(0x004e4a45u,0x004e2970u,f);
    /* 004e4a4a */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4a4d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004e4a4e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e4a4f */
    f->r[4]+=4;return;
mi_004e4a50:;
    /* 004e4a50 */
    f->r[2]=mi_read((0x0078d67cu),4u);
    /* 004e4a56 */
    f->r[6]=mi_read((0x0078d6b0u),4u);
    /* 004e4a5c */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 004e4a5e */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e4a68;
    /* 004e4a60 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004e4a62 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e4dec;
mi_004e4a68:;
    /* 004e4a68 */
    f->r[0]=mi_read((0x00eaac28u),4u);
    /* 004e4a6d */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004e4a73 */
    f->r[1]=mi_read((0x00ea6190u+f->r[0]),4u);
    /* 004e4a79 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004e4a7b */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e4a8b;
    /* 004e4a7d */
    f->r[7]=mi_read((0x00ea61c4u+f->r[0]),4u);
    /* 004e4a83 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 004e4a85 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e4dec;
mi_004e4a8b:;
    /* 004e4a8b */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 004e4a8d */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e4c1d;
    /* 004e4a93 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004e4a95 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e4c1d;
    /* 004e4a9b */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004e4a9d */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e4ba0;
    /* 004e4aa3 */
    f->r[2]=mi_read((0x00ea61c4u+f->r[0]),4u);
    /* 004e4aa9 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 004e4aab */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e4ba0;
    /* 004e4ab1 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4ab3 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4ab5 */
    a=0x0000009fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4aba */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4abfu;f->boundary(0x004e4abau,0x004e2970u,f);
    /* 004e4abf */
    a=0x0000000cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4ac1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4ac6u;f->boundary(0x004e4ac1u,0x00443b90u,f);
    /* 004e4ac6 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4ac9 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e4acb */
    if((f->flags&MI_ZF))goto mi_004e4ad8;
    /* 004e4acd */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4acf */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4ad1 */
    a=0x000000a0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4ad6 */
    goto mi_004e4b19;
mi_004e4ad8:;
    /* 004e4ad8 */
    a=0x0000000cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4ada */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4adfu;f->boundary(0x004e4adau,0x00443950u,f);
    /* 004e4adf */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4ae2 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e4ae4 */
    if((f->flags&MI_ZF))goto mi_004e4b10;
    /* 004e4ae6 */
    f->r[1]=mi_read((0x00eaac28u),4u);
    /* 004e4aec */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 004e4af2 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x00ea40aeu+f->r[1]),2u))&0xffffu)<<0);
    /* 004e4af9 */
    a=(f->r[0]&0xffffu);b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 004e4afd */
    if((f->flags&MI_ZF))goto mi_004e4b21;
    /* 004e4aff */
    a=(f->r[0]&0xffffu);b=0x00000003u;t=mi_alu(f,a,b,16,1);
    /* 004e4b03 */
    if((f->flags&MI_ZF))goto mi_004e4b21;
    /* 004e4b05 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4b07 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4b09 */
    a=0x000000a2u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4b0e */
    goto mi_004e4b19;
mi_004e4b10:;
    /* 004e4b10 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4b12 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4b14 */
    a=0x000000a1u;f->r[4]-=4;MI_U32(f->r[4])=a;
mi_004e4b19:;
    /* 004e4b19 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4b1eu;f->boundary(0x004e4b19u,0x004e2970u,f);
    /* 004e4b1e */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004e4b21:;
    /* 004e4b21 */
    a=0x00000019u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4b23 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4b28u;f->boundary(0x004e4b23u,0x00443b90u,f);
    /* 004e4b28 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4b2b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e4b2d */
    if((f->flags&MI_ZF))goto mi_004e4b43;
    /* 004e4b2f */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4b31 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4b33 */
    a=0x0000009au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4b38 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4b3du;f->boundary(0x004e4b38u,0x004e2970u,f);
    /* 004e4b3d */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4b40 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004e4b41 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e4b42 */
    f->r[4]+=4;return;
mi_004e4b43:;
    /* 004e4b43 */
    a=0x00000019u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4b45 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4b4au;f->boundary(0x004e4b45u,0x00443950u,f);
    /* 004e4b4a */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4b4d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e4b4f */
    if((f->flags&MI_ZF))goto mi_004e4b8c;
    /* 004e4b51 */
    f->r[2]=mi_read((0x00eaac28u),4u);
    /* 004e4b57 */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 004e4b5d */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x00ea40c8u+f->r[2]),2u))&0xffffu)<<0);
    /* 004e4b64 */
    a=(f->r[0]&0xffffu);b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 004e4b68 */
    if((f->flags&MI_ZF))goto mi_004e4dec;
    /* 004e4b6e */
    a=(f->r[0]&0xffffu);b=0x00000003u;t=mi_alu(f,a,b,16,1);
    /* 004e4b72 */
    if((f->flags&MI_ZF))goto mi_004e4dec;
    /* 004e4b78 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4b7a */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4b7c */
    a=0x0000009cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4b81 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4b86u;f->boundary(0x004e4b81u,0x004e2970u,f);
    /* 004e4b86 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4b89 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004e4b8a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e4b8b */
    f->r[4]+=4;return;
mi_004e4b8c:;
    /* 004e4b8c */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4b8e */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4b90 */
    a=0x0000009bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4b95 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4b9au;f->boundary(0x004e4b95u,0x004e2970u,f);
    /* 004e4b9a */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4b9d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004e4b9e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e4b9f */
    f->r[4]+=4;return;
mi_004e4ba0:;
    /* 004e4ba0 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004e4ba2 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e4c1d;
    /* 004e4ba4 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4ba6 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4ba8 */
    a=0x000000a3u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4bad */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4bb2u;f->boundary(0x004e4badu,0x004e2970u,f);
    /* 004e4bb2 */
    a=0x0000000cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4bb4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4bb9u;f->boundary(0x004e4bb4u,0x00443b90u,f);
    /* 004e4bb9 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4bbc */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e4bbe */
    if((f->flags&MI_ZF))goto mi_004e4bd4;
    /* 004e4bc0 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4bc2 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4bc4 */
    a=0x000000a0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4bc9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4bceu;f->boundary(0x004e4bc9u,0x004e2970u,f);
    /* 004e4bce */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4bd1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004e4bd2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e4bd3 */
    f->r[4]+=4;return;
mi_004e4bd4:;
    /* 004e4bd4 */
    a=0x0000000cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4bd6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4bdbu;f->boundary(0x004e4bd6u,0x00443950u,f);
    /* 004e4bdb */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4bde */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e4be0 */
    if((f->flags&MI_ZF))goto mi_004e4dde;
    /* 004e4be6 */
    f->r[0]=mi_read((0x00eaac28u),4u);
    /* 004e4beb */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004e4bf1 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x00ea40aeu+f->r[0]),2u))&0xffffu)<<0);
    /* 004e4bf8 */
    a=(f->r[0]&0xffffu);b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 004e4bfc */
    if((f->flags&MI_ZF))goto mi_004e4dec;
    /* 004e4c02 */
    a=(f->r[0]&0xffffu);b=0x00000003u;t=mi_alu(f,a,b,16,1);
    /* 004e4c06 */
    if((f->flags&MI_ZF))goto mi_004e4dec;
    /* 004e4c0c */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4c0e */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4c10 */
    a=0x00000020u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4c12 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4c17u;f->boundary(0x004e4c12u,0x004e2970u,f);
    /* 004e4c17 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4c1a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004e4c1b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e4c1c */
    f->r[4]+=4;return;
mi_004e4c1d:;
    /* 004e4c1d */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004e4c1f */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e4dec;
    /* 004e4c25 */
    f->r[1]=mi_read((0x00ea61c4u+f->r[0]),4u);
    /* 004e4c2b */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004e4c2d */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e4dec;
    /* 004e4c33 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4c35 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4c37 */
    a=0x0000009eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4c3c */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4c41u;f->boundary(0x004e4c3cu,0x004e2970u,f);
    /* 004e4c41 */
    a=0x00000019u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4c43 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4c48u;f->boundary(0x004e4c43u,0x00443b90u,f);
    /* 004e4c48 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4c4b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e4c4d */
    if((f->flags&MI_ZF))goto mi_004e4c63;
    /* 004e4c4f */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4c51 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4c53 */
    a=0x0000009au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4c58 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4c5du;f->boundary(0x004e4c58u,0x004e2970u,f);
    /* 004e4c5d */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4c60 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004e4c61 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e4c62 */
    f->r[4]+=4;return;
mi_004e4c63:;
    /* 004e4c63 */
    a=0x00000019u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4c65 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4c6au;f->boundary(0x004e4c65u,0x00443950u,f);
    /* 004e4c6a */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4c6d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e4c6f */
    if((f->flags&MI_ZF))goto mi_004e4dde;
    /* 004e4c75 */
    f->r[1]=mi_read((0x00eaac28u),4u);
    /* 004e4c7b */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 004e4c81 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x00ea40c8u+f->r[1]),2u))&0xffffu)<<0);
    /* 004e4c88 */
    a=(f->r[0]&0xffffu);b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 004e4c8c */
    if((f->flags&MI_ZF))goto mi_004e4dec;
    /* 004e4c92 */
    a=(f->r[0]&0xffffu);b=0x00000003u;t=mi_alu(f,a,b,16,1);
    /* 004e4c96 */
    if((f->flags&MI_ZF))goto mi_004e4dec;
    /* 004e4c9c */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4c9e */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4ca0 */
    a=0x00000020u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4ca2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4ca7u;f->boundary(0x004e4ca2u,0x004e2970u,f);
    /* 004e4ca7 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4caa */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004e4cab */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e4cac */
    f->r[4]+=4;return;
mi_004e4cad:;
    /* 004e4cad */
    f->r[0]=mi_read((0x0078d6acu),4u);
    /* 004e4cb2 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e4cb4 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e4dec;
    /* 004e4cba */
    f->r[2]=mi_read((0x00eaac28u),4u);
    /* 004e4cc0 */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 004e4cc6 */
    f->r[0]=mi_read((0x00ea61c0u+f->r[2]),4u);
    /* 004e4ccc */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e4cce */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e4dec;
    /* 004e4cd4 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4cd6 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4cd8 */
    a=0x0000009du;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4cdd */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4ce2u;f->boundary(0x004e4cddu,0x004e2970u,f);
    /* 004e4ce2 */
    a=0x00000018u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4ce4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4ce9u;f->boundary(0x004e4ce4u,0x00443b90u,f);
    /* 004e4ce9 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4cec */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e4cee */
    if((f->flags&MI_ZF))goto mi_004e4d04;
    /* 004e4cf0 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4cf2 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4cf4 */
    a=0x00000097u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4cf9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4cfeu;f->boundary(0x004e4cf9u,0x004e2970u,f);
    /* 004e4cfe */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4d01 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004e4d02 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e4d03 */
    f->r[4]+=4;return;
mi_004e4d04:;
    /* 004e4d04 */
    a=0x00000018u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4d06 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4d0bu;f->boundary(0x004e4d06u,0x00443950u,f);
    /* 004e4d0b */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4d0e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e4d10 */
    if((f->flags&MI_ZF))goto mi_004e4dde;
    /* 004e4d16 */
    f->r[0]=mi_read((0x00eaac28u),4u);
    /* 004e4d1b */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004e4d21 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x00ea40c6u+f->r[0]),2u))&0xffffu)<<0);
    /* 004e4d28 */
    a=(f->r[0]&0xffffu);b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 004e4d2c */
    if((f->flags&MI_ZF))goto mi_004e4dec;
    /* 004e4d32 */
    a=(f->r[0]&0xffffu);b=0x00000003u;t=mi_alu(f,a,b,16,1);
    /* 004e4d36 */
    if((f->flags&MI_ZF))goto mi_004e4dec;
    /* 004e4d3c */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4d3e */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4d40 */
    a=0x00000020u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4d42 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4d47u;f->boundary(0x004e4d42u,0x004e2970u,f);
    /* 004e4d47 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4d4a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004e4d4b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e4d4c */
    f->r[4]+=4;return;
mi_004e4d4d:;
    /* 004e4d4d */
    f->r[0]=mi_read((0x0078d67cu),4u);
    /* 004e4d52 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e4d54 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e4dec;
    /* 004e4d5a */
    f->r[1]=mi_read((0x00eaac28u),4u);
    /* 004e4d60 */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 004e4d66 */
    f->r[0]=mi_read((0x00ea6190u+f->r[1]),4u);
    /* 004e4d6c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e4d6e */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e4dec;
    /* 004e4d70 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4d72 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4d74 */
    a=0x000000a3u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4d79 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4d7eu;f->boundary(0x004e4d79u,0x004e2970u,f);
    /* 004e4d7e */
    a=0x0000000cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4d80 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4d85u;f->boundary(0x004e4d80u,0x00443b90u,f);
    /* 004e4d85 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4d88 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e4d8a */
    if((f->flags&MI_ZF))goto mi_004e4da0;
    /* 004e4d8c */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4d8e */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4d90 */
    a=0x000000a0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4d95 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4d9au;f->boundary(0x004e4d95u,0x004e2970u,f);
    /* 004e4d9a */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4d9d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004e4d9e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e4d9f */
    f->r[4]+=4;return;
mi_004e4da0:;
    /* 004e4da0 */
    a=0x0000000cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4da2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4da7u;f->boundary(0x004e4da2u,0x00443950u,f);
    /* 004e4da7 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4daa */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e4dac */
    if((f->flags&MI_ZF))goto mi_004e4dde;
    /* 004e4dae */
    f->r[2]=mi_read((0x00eaac28u),4u);
    /* 004e4db4 */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 004e4dba */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x00ea40aeu+f->r[2]),2u))&0xffffu)<<0);
    /* 004e4dc1 */
    a=(f->r[0]&0xffffu);b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 004e4dc5 */
    if((f->flags&MI_ZF))goto mi_004e4dec;
    /* 004e4dc7 */
    a=(f->r[0]&0xffffu);b=0x00000003u;t=mi_alu(f,a,b,16,1);
    /* 004e4dcb */
    if((f->flags&MI_ZF))goto mi_004e4dec;
    /* 004e4dcd */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4dcf */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4dd1 */
    a=0x00000020u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4dd3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4dd8u;f->boundary(0x004e4dd3u,0x004e2970u,f);
    /* 004e4dd8 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4ddb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004e4ddc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e4ddd */
    f->r[4]+=4;return;
mi_004e4dde:;
    /* 004e4dde */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4de0 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4de2 */
    a=0x0000001fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4de4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4de9u;f->boundary(0x004e4de4u,0x004e2970u,f);
    /* 004e4de9 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004e4dec:;
    /* 004e4dec */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004e4ded */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e4dee */
    f->r[4]+=4;return;
}

void restore_warning_C_004e3760(struct restore_warning_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004e3760 */
    f->r[0]=MI_U32((0x00605a3cu));
    /* 004e3765 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3766 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004e3769 */
    if((f->flags&MI_ZF))goto mi_004e39ca;
    /* 004e376f */
    a=MI_U32((0x00000008u+f->r[4]));b=0x000000ccu;t=mi_alu(f,a,b,32,1);
    /* 004e3777 */
    if(!(f->flags&MI_ZF))goto mi_004e39ca;
    /* 004e377d */
    f->r[2]=MI_U32((0x0078d678u));
    /* 004e3783 */
    f->r[6]=MI_U32((0x0078d650u));
    /* 004e3789 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 004e378b */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e3795;
    /* 004e378d */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004e378f */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e39ca;
mi_004e3795:;
    /* 004e3795 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 004e379a */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004e37a0 */
    f->r[1]=MI_U32((0x00ea6190u+f->r[0]));
    /* 004e37a6 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004e37a8 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e39ca;
    /* 004e37ae */
    f->r[1]=MI_U32((0x00ea618cu+f->r[0]));
    /* 004e37b4 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004e37b6 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e37c8;
    /* 004e37b8 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e37b9 */
    f->r[7]=MI_U32((0x00ea6184u+f->r[0]));
    /* 004e37bf */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 004e37c1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004e37c2 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e39ca;
mi_004e37c8:;
    /* 004e37c8 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 004e37ca */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e3949;
    /* 004e37d0 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004e37d2 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e3949;
    /* 004e37d8 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004e37da */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e38d3;
    /* 004e37e0 */
    f->r[2]=MI_U32((0x00ea6184u+f->r[0]));
    /* 004e37e6 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 004e37e8 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e38d3;
    /* 004e37ee */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e37f0 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e37f2 */
    a=0x00000091u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e37f7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e37fcu;f->boundary(0x004e37f7u,0x004e2970u,f);
    /* 004e37fc */
    a=0x0000000bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e37fe */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e3803u;f->boundary(0x004e37feu,0x00443b90u,f);
    /* 004e3803 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e3806 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e3808 */
    if((f->flags&MI_ZF))goto mi_004e3812;
    /* 004e380a */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e380c */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e380e */
    a=0x00000024u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3810 */
    goto mi_004e3852;
mi_004e3812:;
    /* 004e3812 */
    a=0x0000000bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3814 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e3819u;f->boundary(0x004e3814u,0x00443950u,f);
    /* 004e3819 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e381c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e381e */
    if((f->flags&MI_ZF))goto mi_004e3849;
    /* 004e3820 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 004e3825 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004e382b */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00ea40acu+f->r[0])))&0xffffu)<<0);
    /* 004e3832 */
    a=(f->r[0]&0xffffu);b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 004e3836 */
    if((f->flags&MI_ZF))goto mi_004e385a;
    /* 004e3838 */
    a=(f->r[0]&0xffffu);b=0x00000003u;t=mi_alu(f,a,b,16,1);
    /* 004e383c */
    if((f->flags&MI_ZF))goto mi_004e385a;
    /* 004e383e */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3840 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3842 */
    a=0x00000093u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3847 */
    goto mi_004e3852;
mi_004e3849:;
    /* 004e3849 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e384b */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e384d */
    a=0x00000092u;f->r[4]-=4;MI_U32(f->r[4])=a;
mi_004e3852:;
    /* 004e3852 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e3857u;f->boundary(0x004e3852u,0x004e2970u,f);
    /* 004e3857 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004e385a:;
    /* 004e385a */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e385c */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e3861u;f->boundary(0x004e385cu,0x00443b90u,f);
    /* 004e3861 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e3864 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e3866 */
    if((f->flags&MI_ZF))goto mi_004e3878;
    /* 004e3868 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e386a */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e386c */
    a=0x00000053u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e386e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e3873u;f->boundary(0x004e386eu,0x004e2970u,f);
    /* 004e3873 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e3876 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e3877 */
    f->r[4]+=4;return;
mi_004e3878:;
    /* 004e3878 */
    a=0x00000009u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e387a */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e387fu;f->boundary(0x004e387au,0x00443950u,f);
    /* 004e387f */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e3882 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e3884 */
    if((f->flags&MI_ZF))goto mi_004e38c0;
    /* 004e3886 */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 004e388c */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 004e3892 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00ea40a8u+f->r[1])))&0xffffu)<<0);
    /* 004e3899 */
    a=(f->r[0]&0xffffu);b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 004e389d */
    if((f->flags&MI_ZF))goto mi_004e39ca;
    /* 004e38a3 */
    a=(f->r[0]&0xffffu);b=0x00000003u;t=mi_alu(f,a,b,16,1);
    /* 004e38a7 */
    if((f->flags&MI_ZF))goto mi_004e39ca;
    /* 004e38ad */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e38af */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e38b1 */
    a=0x00000095u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e38b6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e38bbu;f->boundary(0x004e38b6u,0x004e2970u,f);
    /* 004e38bb */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e38be */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e38bf */
    f->r[4]+=4;return;
mi_004e38c0:;
    /* 004e38c0 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e38c2 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e38c4 */
    a=0x00000094u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e38c9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e38ceu;f->boundary(0x004e38c9u,0x004e2970u,f);
    /* 004e38ce */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e38d1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e38d2 */
    f->r[4]+=4;return;
mi_004e38d3:;
    /* 004e38d3 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004e38d5 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e3949;
    /* 004e38d7 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e38d9 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e38db */
    a=0x0000001du;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e38dd */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e38e2u;f->boundary(0x004e38ddu,0x004e2970u,f);
    /* 004e38e2 */
    a=0x0000000bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e38e4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e38e9u;f->boundary(0x004e38e4u,0x00443b90u,f);
    /* 004e38e9 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e38ec */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e38ee */
    if((f->flags&MI_ZF))goto mi_004e3900;
    /* 004e38f0 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e38f2 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e38f4 */
    a=0x00000024u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e38f6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e38fbu;f->boundary(0x004e38f6u,0x004e2970u,f);
    /* 004e38fb */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e38fe */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e38ff */
    f->r[4]+=4;return;
mi_004e3900:;
    /* 004e3900 */
    a=0x0000000bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3902 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e3907u;f->boundary(0x004e3902u,0x00443950u,f);
    /* 004e3907 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e390a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e390c */
    if((f->flags&MI_ZF))goto mi_004e39bc;
    /* 004e3912 */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 004e3918 */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 004e391e */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00ea40acu+f->r[2])))&0xffffu)<<0);
    /* 004e3925 */
    a=(f->r[0]&0xffffu);b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 004e3929 */
    if((f->flags&MI_ZF))goto mi_004e39ca;
    /* 004e392f */
    a=(f->r[0]&0xffffu);b=0x00000003u;t=mi_alu(f,a,b,16,1);
    /* 004e3933 */
    if((f->flags&MI_ZF))goto mi_004e39ca;
    /* 004e3939 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e393b */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e393d */
    a=0x00000020u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e393f */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e3944u;f->boundary(0x004e393fu,0x004e2970u,f);
    /* 004e3944 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e3947 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e3948 */
    f->r[4]+=4;return;
mi_004e3949:;
    /* 004e3949 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004e394b */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e39ca;
    /* 004e394d */
    f->r[1]=MI_U32((0x00ea6184u+f->r[0]));
    /* 004e3953 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004e3955 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e39ca;
    /* 004e3957 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3959 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e395b */
    a=0x00000052u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e395d */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e3962u;f->boundary(0x004e395du,0x004e2970u,f);
    /* 004e3962 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3964 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e3969u;f->boundary(0x004e3964u,0x00443b90u,f);
    /* 004e3969 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e396c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e396e */
    if((f->flags&MI_ZF))goto mi_004e3980;
    /* 004e3970 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3972 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3974 */
    a=0x00000053u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3976 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e397bu;f->boundary(0x004e3976u,0x004e2970u,f);
    /* 004e397b */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e397e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e397f */
    f->r[4]+=4;return;
mi_004e3980:;
    /* 004e3980 */
    a=0x00000009u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3982 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e3987u;f->boundary(0x004e3982u,0x00443950u,f);
    /* 004e3987 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e398a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e398c */
    if((f->flags&MI_ZF))goto mi_004e39bc;
    /* 004e398e */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 004e3993 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004e3999 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00ea40a8u+f->r[0])))&0xffffu)<<0);
    /* 004e39a0 */
    a=(f->r[0]&0xffffu);b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 004e39a4 */
    if((f->flags&MI_ZF))goto mi_004e39ca;
    /* 004e39a6 */
    a=(f->r[0]&0xffffu);b=0x00000003u;t=mi_alu(f,a,b,16,1);
    /* 004e39aa */
    if((f->flags&MI_ZF))goto mi_004e39ca;
    /* 004e39ac */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e39ae */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e39b0 */
    a=0x00000020u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e39b2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e39b7u;f->boundary(0x004e39b2u,0x004e2970u,f);
    /* 004e39b7 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e39ba */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e39bb */
    f->r[4]+=4;return;
mi_004e39bc:;
    /* 004e39bc */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e39be */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e39c0 */
    a=0x0000001fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e39c2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e39c7u;f->boundary(0x004e39c2u,0x004e2970u,f);
    /* 004e39c7 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004e39ca:;
    /* 004e39ca */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e39cb */
    f->r[4]+=4;return;
}

void restore_warning_C_004e3aa0(struct restore_warning_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004e3aa0 */
    f->r[0]=MI_U32((0x00605a3cu));
    /* 004e3aa5 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3aa6 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004e3aa9 */
    if((f->flags&MI_ZF))goto mi_004e3ce6;
    /* 004e3aaf */
    a=MI_U32((0x00000008u+f->r[4]));b=0x000000e8u;t=mi_alu(f,a,b,32,1);
    /* 004e3ab7 */
    if(!(f->flags&MI_ZF))goto mi_004e3ce6;
    /* 004e3abd */
    f->r[2]=MI_U32((0x0078d6c8u));
    /* 004e3ac3 */
    f->r[6]=MI_U32((0x0078d680u));
    /* 004e3ac9 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 004e3acb */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e3ad5;
    /* 004e3acd */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004e3acf */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e3ce6;
mi_004e3ad5:;
    /* 004e3ad5 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 004e3ada */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004e3ae0 */
    f->r[1]=MI_U32((0x00ea61e8u+f->r[0]));
    /* 004e3ae6 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004e3ae8 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e3ce6;
    /* 004e3aee */
    f->r[1]=MI_U32((0x00ea61dcu+f->r[0]));
    /* 004e3af4 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004e3af6 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e3b08;
    /* 004e3af8 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3af9 */
    f->r[7]=MI_U32((0x00ea6194u+f->r[0]));
    /* 004e3aff */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 004e3b01 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004e3b02 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e3ce6;
mi_004e3b08:;
    /* 004e3b08 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 004e3b0a */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e3c5b;
    /* 004e3b10 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004e3b12 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e3c5b;
    /* 004e3b18 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004e3b1a */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e3c00;
    /* 004e3b20 */
    f->r[2]=MI_U32((0x00ea6194u+f->r[0]));
    /* 004e3b26 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 004e3b28 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e3c00;
    /* 004e3b2e */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3b30 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3b32 */
    a=0x000000ffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3b37 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e3b3cu;f->boundary(0x004e3b37u,0x004e2970u,f);
    /* 004e3b3c */
    a=0x0000001fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3b3e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e3b43u;f->boundary(0x004e3b3eu,0x00443950u,f);
    /* 004e3b43 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e3b46 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e3b48 */
    if((f->flags&MI_ZF))goto mi_004e3b73;
    /* 004e3b4a */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 004e3b4f */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004e3b55 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00ea40d4u+f->r[0])))&0xffffu)<<0);
    /* 004e3b5c */
    a=(f->r[0]&0xffffu);b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 004e3b60 */
    if((f->flags&MI_ZF))goto mi_004e3b84;
    /* 004e3b62 */
    a=(f->r[0]&0xffffu);b=0x00000003u;t=mi_alu(f,a,b,16,1);
    /* 004e3b66 */
    if((f->flags&MI_ZF))goto mi_004e3b84;
    /* 004e3b68 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3b6a */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3b6c */
    a=0x00000103u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3b71 */
    goto mi_004e3b7c;
mi_004e3b73:;
    /* 004e3b73 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3b75 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3b77 */
    a=0x00000104u;f->r[4]-=4;MI_U32(f->r[4])=a;
mi_004e3b7c:;
    /* 004e3b7c */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e3b81u;f->boundary(0x004e3b7cu,0x004e2970u,f);
    /* 004e3b81 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004e3b84:;
    /* 004e3b84 */
    a=0x0000000du;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3b86 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e3b8bu;f->boundary(0x004e3b86u,0x00443b90u,f);
    /* 004e3b8b */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e3b8e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e3b90 */
    if((f->flags&MI_ZF))goto mi_004e3ba5;
    /* 004e3b92 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3b94 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3b96 */
    a=0x00000100u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3b9b */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e3ba0u;f->boundary(0x004e3b9bu,0x004e2970u,f);
    /* 004e3ba0 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e3ba3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e3ba4 */
    f->r[4]+=4;return;
mi_004e3ba5:;
    /* 004e3ba5 */
    a=0x0000000du;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3ba7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e3bacu;f->boundary(0x004e3ba7u,0x00443950u,f);
    /* 004e3bac */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e3baf */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e3bb1 */
    if((f->flags&MI_ZF))goto mi_004e3bed;
    /* 004e3bb3 */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 004e3bb9 */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 004e3bbf */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00ea40b0u+f->r[1])))&0xffffu)<<0);
    /* 004e3bc6 */
    a=(f->r[0]&0xffffu);b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 004e3bca */
    if((f->flags&MI_ZF))goto mi_004e3ce6;
    /* 004e3bd0 */
    a=(f->r[0]&0xffffu);b=0x00000003u;t=mi_alu(f,a,b,16,1);
    /* 004e3bd4 */
    if((f->flags&MI_ZF))goto mi_004e3ce6;
    /* 004e3bda */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3bdc */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3bde */
    a=0x00000101u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3be3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e3be8u;f->boundary(0x004e3be3u,0x004e2970u,f);
    /* 004e3be8 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e3beb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e3bec */
    f->r[4]+=4;return;
mi_004e3bed:;
    /* 004e3bed */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3bef */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3bf1 */
    a=0x00000102u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3bf6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e3bfbu;f->boundary(0x004e3bf6u,0x004e2970u,f);
    /* 004e3bfb */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e3bfe */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e3bff */
    f->r[4]+=4;return;
mi_004e3c00:;
    /* 004e3c00 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004e3c02 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e3c5b;
    /* 004e3c04 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3c06 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3c08 */
    a=0x00000109u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3c0d */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e3c12u;f->boundary(0x004e3c0du,0x004e2970u,f);
    /* 004e3c12 */
    a=0x0000001fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3c14 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e3c19u;f->boundary(0x004e3c14u,0x00443950u,f);
    /* 004e3c19 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e3c1c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e3c1e */
    if((f->flags&MI_ZF))goto mi_004e3cd8;
    /* 004e3c24 */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 004e3c2a */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 004e3c30 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00ea40d4u+f->r[2])))&0xffffu)<<0);
    /* 004e3c37 */
    a=(f->r[0]&0xffffu);b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 004e3c3b */
    if((f->flags&MI_ZF))goto mi_004e3ce6;
    /* 004e3c41 */
    a=(f->r[0]&0xffffu);b=0x00000003u;t=mi_alu(f,a,b,16,1);
    /* 004e3c45 */
    if((f->flags&MI_ZF))goto mi_004e3ce6;
    /* 004e3c4b */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3c4d */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3c4f */
    a=0x00000020u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3c51 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e3c56u;f->boundary(0x004e3c51u,0x004e2970u,f);
    /* 004e3c56 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e3c59 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e3c5a */
    f->r[4]+=4;return;
mi_004e3c5b:;
    /* 004e3c5b */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004e3c5d */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e3ce6;
    /* 004e3c63 */
    f->r[1]=MI_U32((0x00ea6194u+f->r[0]));
    /* 004e3c69 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004e3c6b */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e3ce6;
    /* 004e3c6d */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3c6f */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3c71 */
    a=0x0000010au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3c76 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e3c7bu;f->boundary(0x004e3c76u,0x004e2970u,f);
    /* 004e3c7b */
    a=0x0000000du;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3c7d */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e3c82u;f->boundary(0x004e3c7du,0x00443b90u,f);
    /* 004e3c82 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e3c85 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e3c87 */
    if((f->flags&MI_ZF))goto mi_004e3c9c;
    /* 004e3c89 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3c8b */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3c8d */
    a=0x00000100u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3c92 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e3c97u;f->boundary(0x004e3c92u,0x004e2970u,f);
    /* 004e3c97 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e3c9a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e3c9b */
    f->r[4]+=4;return;
mi_004e3c9c:;
    /* 004e3c9c */
    a=0x0000000du;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3c9e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e3ca3u;f->boundary(0x004e3c9eu,0x00443950u,f);
    /* 004e3ca3 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e3ca6 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e3ca8 */
    if((f->flags&MI_ZF))goto mi_004e3cd8;
    /* 004e3caa */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 004e3caf */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004e3cb5 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00ea40b0u+f->r[0])))&0xffffu)<<0);
    /* 004e3cbc */
    a=(f->r[0]&0xffffu);b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 004e3cc0 */
    if((f->flags&MI_ZF))goto mi_004e3ce6;
    /* 004e3cc2 */
    a=(f->r[0]&0xffffu);b=0x00000003u;t=mi_alu(f,a,b,16,1);
    /* 004e3cc6 */
    if((f->flags&MI_ZF))goto mi_004e3ce6;
    /* 004e3cc8 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3cca */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3ccc */
    a=0x00000020u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3cce */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e3cd3u;f->boundary(0x004e3cceu,0x004e2970u,f);
    /* 004e3cd3 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e3cd6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e3cd7 */
    f->r[4]+=4;return;
mi_004e3cd8:;
    /* 004e3cd8 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3cda */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3cdc */
    a=0x0000001fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3cde */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e3ce3u;f->boundary(0x004e3cdeu,0x004e2970u,f);
    /* 004e3ce3 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004e3ce6:;
    /* 004e3ce6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e3ce7 */
    f->r[4]+=4;return;
}

void restore_warning_C_004e3da0(struct restore_warning_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004e3da0 */
    a=MI_U32((0x00605a3cu));b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004e3da7 */
    if((f->flags&MI_ZF))goto mi_004e3f52;
    /* 004e3dad */
    a=MI_U32((0x00000004u+f->r[4]));b=0x00000020u;t=mi_alu(f,a,b,32,1);
    /* 004e3db2 */
    if(!(f->flags&MI_ZF))goto mi_004e3f52;
    /* 004e3db8 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 004e3dbd */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004e3dc3 */
    f->r[1]=MI_U32((0x00ea619cu+f->r[0]));
    /* 004e3dc9 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004e3dcb */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e3f52;
    /* 004e3dd1 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3dd3 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3dd5 */
    a=0x00000089u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3dda */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e3ddfu;f->boundary(0x004e3ddau,0x004e2970u,f);
    /* 004e3ddf */
    f->r[0]=MI_U32((0x0078d714u));
    /* 004e3de4 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e3de7 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e3de9 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e3f52;
    /* 004e3def */
    a=0x0000000eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3df1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e3df6u;f->boundary(0x004e3df1u,0x00443900u,f);
    /* 004e3df6 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e3df9 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e3dfb */
    if(!(f->flags&MI_ZF))goto mi_004e3e0f;
    /* 004e3dfd */
    a=0x0000000eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3dff */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e3e04u;f->boundary(0x004e3dffu,0x004439c0u,f);
    /* 004e3e04 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e3e07 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e3e09 */
    if((f->flags&MI_ZF))goto mi_004e3e8f;
mi_004e3e0f:;
    /* 004e3e0f */
    f->r[0]=MI_U32((0x0078d718u));
    /* 004e3e14 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e3e16 */
    if(!(f->flags&MI_ZF))goto mi_004e3e8f;
    /* 004e3e18 */
    a=0x0000000fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3e1a */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e3e1fu;f->boundary(0x004e3e1au,0x004439c0u,f);
    /* 004e3e1f */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e3e22 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e3e24 */
    if((f->flags&MI_ZF))goto mi_004e3e7d;
    /* 004e3e26 */
    a=0x0000000fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3e28 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e3e2du;f->boundary(0x004e3e28u,0x00443950u,f);
    /* 004e3e2d */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e3e30 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e3e32 */
    if(!(f->flags&MI_ZF))goto mi_004e3e44;
    /* 004e3e34 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3e35 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3e36 */
    a=0x000000c0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3e3b */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e3e40u;f->boundary(0x004e3e3bu,0x004e2970u,f);
    /* 004e3e40 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e3e43 */
    f->r[4]+=4;return;
mi_004e3e44:;
    /* 004e3e44 */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 004e3e4a */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 004e3e50 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00ea40b4u+f->r[1])))&0xffffu)<<0);
    /* 004e3e57 */
    a=(f->r[0]&0xffffu);b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 004e3e5b */
    if((f->flags&MI_ZF))goto mi_004e3f52;
    /* 004e3e61 */
    a=(f->r[0]&0xffffu);b=0x00000003u;t=mi_alu(f,a,b,16,1);
    /* 004e3e65 */
    if((f->flags&MI_ZF))goto mi_004e3f52;
    /* 004e3e6b */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3e6d */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3e6f */
    a=0x000000bfu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3e74 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e3e79u;f->boundary(0x004e3e74u,0x004e2970u,f);
    /* 004e3e79 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e3e7c */
    f->r[4]+=4;return;
mi_004e3e7d:;
    /* 004e3e7d */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3e7f */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3e81 */
    a=0x0000008au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3e86 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e3e8bu;f->boundary(0x004e3e86u,0x004e2970u,f);
    /* 004e3e8b */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e3e8e */
    f->r[4]+=4;return;
mi_004e3e8f:;
    /* 004e3e8f */
    a=0x0000000eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3e91 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e3e96u;f->boundary(0x004e3e91u,0x004439c0u,f);
    /* 004e3e96 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e3e99 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e3e9b */
    if((f->flags&MI_ZF))goto mi_004e3efb;
    /* 004e3e9d */
    f->r[0]=MI_U32((0x0078d718u));
    /* 004e3ea2 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e3ea4 */
    if((f->flags&MI_ZF))goto mi_004e3efb;
    /* 004e3ea6 */
    a=0x0000000eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3ea8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e3eadu;f->boundary(0x004e3ea8u,0x00443950u,f);
    /* 004e3ead */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e3eb0 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e3eb2 */
    if((f->flags&MI_ZF))goto mi_004e3ee9;
    /* 004e3eb4 */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 004e3eba */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 004e3ec0 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00ea40b2u+f->r[2])))&0xffffu)<<0);
    /* 004e3ec7 */
    a=(f->r[0]&0xffffu);b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 004e3ecb */
    if((f->flags&MI_ZF))goto mi_004e3f52;
    /* 004e3ed1 */
    a=(f->r[0]&0xffffu);b=0x00000003u;t=mi_alu(f,a,b,16,1);
    /* 004e3ed5 */
    if((f->flags&MI_ZF))goto mi_004e3f52;
    /* 004e3ed7 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3ed9 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3edb */
    a=0x000000a8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3ee0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e3ee5u;f->boundary(0x004e3ee0u,0x004e2970u,f);
    /* 004e3ee5 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e3ee8 */
    f->r[4]+=4;return;
mi_004e3ee9:;
    /* 004e3ee9 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3eeb */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3eed */
    a=0x000000a9u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3ef2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e3ef7u;f->boundary(0x004e3ef2u,0x004e2970u,f);
    /* 004e3ef7 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e3efa */
    f->r[4]+=4;return;
mi_004e3efb:;
    /* 004e3efb */
    a=0x0000000fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3efd */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e3f02u;f->boundary(0x004e3efdu,0x004439c0u,f);
    /* 004e3f02 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e3f05 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e3f07 */
    if((f->flags&MI_ZF))goto mi_004e3f52;
    /* 004e3f09 */
    a=0x0000000fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3f0b */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e3f10u;f->boundary(0x004e3f0bu,0x00443950u,f);
    /* 004e3f10 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e3f13 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e3f15 */
    if((f->flags&MI_ZF))goto mi_004e3f44;
    /* 004e3f17 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 004e3f1c */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004e3f22 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00ea40b4u+f->r[0])))&0xffffu)<<0);
    /* 004e3f29 */
    a=(f->r[0]&0xffffu);b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 004e3f2d */
    if((f->flags&MI_ZF))goto mi_004e3f52;
    /* 004e3f2f */
    a=(f->r[0]&0xffffu);b=0x00000003u;t=mi_alu(f,a,b,16,1);
    /* 004e3f33 */
    if((f->flags&MI_ZF))goto mi_004e3f52;
    /* 004e3f35 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3f37 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3f39 */
    a=0x00000020u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3f3b */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e3f40u;f->boundary(0x004e3f3bu,0x004e2970u,f);
    /* 004e3f40 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e3f43 */
    f->r[4]+=4;return;
mi_004e3f44:;
    /* 004e3f44 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3f46 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3f48 */
    a=0x0000001fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3f4a */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e3f4fu;f->boundary(0x004e3f4au,0x004e2970u,f);
    /* 004e3f4f */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004e3f52:;
    /* 004e3f52 */
    f->r[4]+=4;return;
}

void restore_warning_C_004e3f60(struct restore_warning_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004e3f60 */
    a=MI_U32((0x00605a3cu));b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004e3f67 */
    if((f->flags&MI_ZF))goto mi_004e411b;
    /* 004e3f6d */
    a=MI_U32((0x00000004u+f->r[4]));b=0x000000e2u;t=mi_alu(f,a,b,32,1);
    /* 004e3f75 */
    if(!(f->flags&MI_ZF))goto mi_004e411b;
    /* 004e3f7b */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 004e3f80 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004e3f86 */
    f->r[1]=MI_U32((0x00ea6180u+f->r[0]));
    /* 004e3f8c */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004e3f8e */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e411b;
    /* 004e3f94 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3f96 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3f98 */
    a=0x000000edu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3f9d */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e3fa2u;f->boundary(0x004e3f9du,0x004e2970u,f);
    /* 004e3fa2 */
    f->r[0]=MI_U32((0x0078d6fcu));
    /* 004e3fa7 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e3faa */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e3fac */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e411b;
    /* 004e3fb2 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3fb4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e3fb9u;f->boundary(0x004e3fb4u,0x00443900u,f);
    /* 004e3fb9 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e3fbc */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e3fbe */
    if(!(f->flags&MI_ZF))goto mi_004e3fce;
    /* 004e3fc0 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3fc2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e3fc7u;f->boundary(0x004e3fc2u,0x004439c0u,f);
    /* 004e3fc7 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e3fca */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e3fcc */
    if((f->flags&MI_ZF))goto mi_004e4031;
mi_004e3fce:;
    /* 004e3fce */
    f->r[0]=MI_U32((0x0078d6fcu));
    /* 004e3fd3 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e3fd5 */
    if(!(f->flags&MI_ZF))goto mi_004e4031;
    /* 004e3fd7 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3fd9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e3fdeu;f->boundary(0x004e3fd9u,0x004439c0u,f);
    /* 004e3fde */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e3fe1 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e3fe3 */
    if((f->flags&MI_ZF))goto mi_004e4026;
    /* 004e3fe5 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3fe7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e3fecu;f->boundary(0x004e3fe7u,0x00443950u,f);
    /* 004e3fec */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e3fef */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e3ff1 */
    if(!(f->flags&MI_ZF))goto mi_004e3ffc;
    /* 004e3ff3 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3ff4 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3ff5 */
    a=0x000000f0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e3ffa */
    goto mi_004e407a;
mi_004e3ffc:;
    /* 004e3ffc */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 004e4002 */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 004e4008 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00ea409au+f->r[1])))&0xffffu)<<0);
    /* 004e400f */
    a=(f->r[0]&0xffffu);b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 004e4013 */
    if((f->flags&MI_ZF))goto mi_004e4082;
    /* 004e4015 */
    a=(f->r[0]&0xffffu);b=0x00000003u;t=mi_alu(f,a,b,16,1);
    /* 004e4019 */
    if((f->flags&MI_ZF))goto mi_004e4082;
    /* 004e401b */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e401d */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e401f */
    a=0x000000efu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4024 */
    goto mi_004e407a;
mi_004e4026:;
    /* 004e4026 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4028 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e402a */
    a=0x000000eeu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e402f */
    goto mi_004e407a;
mi_004e4031:;
    /* 004e4031 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4033 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4038u;f->boundary(0x004e4033u,0x004439c0u,f);
    /* 004e4038 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e403b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e403d */
    if((f->flags&MI_ZF))goto mi_004e4082;
    /* 004e403f */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4041 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4046u;f->boundary(0x004e4041u,0x00443950u,f);
    /* 004e4046 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4049 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e404b */
    if((f->flags&MI_ZF))goto mi_004e4074;
    /* 004e404d */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 004e4053 */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 004e4059 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00ea40a6u+f->r[2])))&0xffffu)<<0);
    /* 004e4060 */
    a=(f->r[0]&0xffffu);b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 004e4064 */
    if((f->flags&MI_ZF))goto mi_004e4082;
    /* 004e4066 */
    a=(f->r[0]&0xffffu);b=0x00000003u;t=mi_alu(f,a,b,16,1);
    /* 004e406a */
    if((f->flags&MI_ZF))goto mi_004e4082;
    /* 004e406c */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e406e */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4070 */
    a=0x00000020u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4072 */
    goto mi_004e407a;
mi_004e4074:;
    /* 004e4074 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4076 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4078 */
    a=0x0000001fu;f->r[4]-=4;MI_U32(f->r[4])=a;
mi_004e407a:;
    /* 004e407a */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e407fu;f->boundary(0x004e407au,0x004e2970u,f);
    /* 004e407f */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004e4082:;
    /* 004e4082 */
    f->r[0]=MI_U32((0x0078d650u));
    /* 004e4087 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e4089 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e411b;
    /* 004e408f */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 004e4094 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004e409a */
    f->r[1]=MI_U32((0x00ea6184u+f->r[0]));
    /* 004e40a0 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004e40a2 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e411b;
    /* 004e40a4 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e40a6 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e40a8 */
    a=0x00000052u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e40aa */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e40afu;f->boundary(0x004e40aau,0x004e2970u,f);
    /* 004e40af */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e40b1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e40b6u;f->boundary(0x004e40b1u,0x00443b90u,f);
    /* 004e40b6 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e40b9 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e40bb */
    if((f->flags&MI_ZF))goto mi_004e40cc;
    /* 004e40bd */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e40bf */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e40c1 */
    a=0x00000053u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e40c3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e40c8u;f->boundary(0x004e40c3u,0x004e2970u,f);
    /* 004e40c8 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e40cb */
    f->r[4]+=4;return;
mi_004e40cc:;
    /* 004e40cc */
    a=0x00000009u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e40ce */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e40d3u;f->boundary(0x004e40ceu,0x00443950u,f);
    /* 004e40d3 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e40d6 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e40d8 */
    if((f->flags&MI_ZF))goto mi_004e410a;
    /* 004e40da */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 004e40df */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004e40e5 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00ea40a8u+f->r[0])))&0xffffu)<<0);
    /* 004e40ec */
    a=(f->r[0]&0xffffu);b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 004e40f0 */
    if((f->flags&MI_ZF))goto mi_004e411b;
    /* 004e40f2 */
    a=(f->r[0]&0xffffu);b=0x00000003u;t=mi_alu(f,a,b,16,1);
    /* 004e40f6 */
    if((f->flags&MI_ZF))goto mi_004e411b;
    /* 004e40f8 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e40fa */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e40fc */
    a=0x00000095u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4101 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4106u;f->boundary(0x004e4101u,0x004e2970u,f);
    /* 004e4106 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4109 */
    f->r[4]+=4;return;
mi_004e410a:;
    /* 004e410a */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e410c */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e410e */
    a=0x00000094u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4113 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4118u;f->boundary(0x004e4113u,0x004e2970u,f);
    /* 004e4118 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004e411b:;
    /* 004e411b */
    f->r[4]+=4;return;
}

void restore_warning_C_004e4120(struct restore_warning_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004e4120 */
    a=MI_U32((0x00605a3cu));b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004e4127 */
    if((f->flags&MI_ZF))goto mi_004e42c8;
    /* 004e412d */
    f->r[0]=MI_U32((0x00000004u+f->r[4]));
    /* 004e4131 */
    a=f->r[0];b=0x00000033u;t=mi_alu(f,a,b,32,1);
    /* 004e4134 */
    if((f->flags&MI_ZF))goto mi_004e413f;
    /* 004e4136 */
    a=f->r[0];b=0x00000035u;t=mi_alu(f,a,b,32,1);
    /* 004e4139 */
    if(!(f->flags&MI_ZF))goto mi_004e42c8;
mi_004e413f:;
    /* 004e413f */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 004e4144 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004e414a */
    f->r[1]=MI_U32((0x00ea61bcu+f->r[0]));
    /* 004e4150 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004e4152 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e42c8;
    /* 004e4158 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e415a */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e415c */
    a=0x0000008bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4161 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4166u;f->boundary(0x004e4161u,0x004e2970u,f);
    /* 004e4166 */
    f->r[0]=MI_U32((0x0078d734u));
    /* 004e416b */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e416e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e4170 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e42c8;
    /* 004e4176 */
    a=0x00000016u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4178 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e417du;f->boundary(0x004e4178u,0x00443900u,f);
    /* 004e417d */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4180 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e4182 */
    if(!(f->flags&MI_ZF))goto mi_004e4192;
    /* 004e4184 */
    a=0x00000016u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4186 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e418bu;f->boundary(0x004e4186u,0x004439c0u,f);
    /* 004e418b */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e418e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e4190 */
    if((f->flags&MI_ZF))goto mi_004e4205;
mi_004e4192:;
    /* 004e4192 */
    f->r[0]=MI_U32((0x0078d738u));
    /* 004e4197 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e4199 */
    if(!(f->flags&MI_ZF))goto mi_004e4205;
    /* 004e419b */
    a=0x00000017u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e419d */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e41a2u;f->boundary(0x004e419du,0x004439c0u,f);
    /* 004e41a2 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e41a5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e41a7 */
    if((f->flags&MI_ZF))goto mi_004e41f3;
    /* 004e41a9 */
    a=0x00000017u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e41ab */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e41b0u;f->boundary(0x004e41abu,0x00443950u,f);
    /* 004e41b0 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e41b3 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e41b5 */
    if(!(f->flags&MI_ZF))goto mi_004e41c7;
    /* 004e41b7 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e41b8 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e41b9 */
    a=0x000000c2u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e41be */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e41c3u;f->boundary(0x004e41beu,0x004e2970u,f);
    /* 004e41c3 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e41c6 */
    f->r[4]+=4;return;
mi_004e41c7:;
    /* 004e41c7 */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 004e41cd */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 004e41d3 */
    a=MI_U16((0x00ea40c4u+f->r[1]));b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 004e41db */
    if((f->flags&MI_ZF))goto mi_004e42c8;
    /* 004e41e1 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e41e3 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e41e5 */
    a=0x000000c1u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e41ea */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e41efu;f->boundary(0x004e41eau,0x004e2970u,f);
    /* 004e41ef */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e41f2 */
    f->r[4]+=4;return;
mi_004e41f3:;
    /* 004e41f3 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e41f5 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e41f7 */
    a=0x0000008cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e41fc */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4201u;f->boundary(0x004e41fcu,0x004e2970u,f);
    /* 004e4201 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4204 */
    f->r[4]+=4;return;
mi_004e4205:;
    /* 004e4205 */
    a=0x00000016u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4207 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e420cu;f->boundary(0x004e4207u,0x004439c0u,f);
    /* 004e420c */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e420f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e4211 */
    if((f->flags&MI_ZF))goto mi_004e4271;
    /* 004e4213 */
    f->r[0]=MI_U32((0x0078d738u));
    /* 004e4218 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e421a */
    if((f->flags&MI_ZF))goto mi_004e4271;
    /* 004e421c */
    a=0x00000016u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e421e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4223u;f->boundary(0x004e421eu,0x00443950u,f);
    /* 004e4223 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4226 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e4228 */
    if((f->flags&MI_ZF))goto mi_004e425f;
    /* 004e422a */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 004e4230 */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 004e4236 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00ea40c2u+f->r[2])))&0xffffu)<<0);
    /* 004e423d */
    a=(f->r[0]&0xffffu);b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 004e4241 */
    if((f->flags&MI_ZF))goto mi_004e42c8;
    /* 004e4247 */
    a=(f->r[0]&0xffffu);b=0x00000003u;t=mi_alu(f,a,b,16,1);
    /* 004e424b */
    if((f->flags&MI_ZF))goto mi_004e42c8;
    /* 004e424d */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e424f */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4251 */
    a=0x000000aau;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4256 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e425bu;f->boundary(0x004e4256u,0x004e2970u,f);
    /* 004e425b */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e425e */
    f->r[4]+=4;return;
mi_004e425f:;
    /* 004e425f */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4261 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4263 */
    a=0x000000abu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4268 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e426du;f->boundary(0x004e4268u,0x004e2970u,f);
    /* 004e426d */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4270 */
    f->r[4]+=4;return;
mi_004e4271:;
    /* 004e4271 */
    a=0x00000017u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4273 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4278u;f->boundary(0x004e4273u,0x004439c0u,f);
    /* 004e4278 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e427b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e427d */
    if((f->flags&MI_ZF))goto mi_004e42c8;
    /* 004e427f */
    a=0x00000017u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4281 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4286u;f->boundary(0x004e4281u,0x00443950u,f);
    /* 004e4286 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4289 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e428b */
    if((f->flags&MI_ZF))goto mi_004e42ba;
    /* 004e428d */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 004e4292 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004e4298 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00ea40c4u+f->r[0])))&0xffffu)<<0);
    /* 004e429f */
    a=(f->r[0]&0xffffu);b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 004e42a3 */
    if((f->flags&MI_ZF))goto mi_004e42c8;
    /* 004e42a5 */
    a=(f->r[0]&0xffffu);b=0x00000003u;t=mi_alu(f,a,b,16,1);
    /* 004e42a9 */
    if((f->flags&MI_ZF))goto mi_004e42c8;
    /* 004e42ab */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e42ad */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e42af */
    a=0x00000020u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e42b1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e42b6u;f->boundary(0x004e42b1u,0x004e2970u,f);
    /* 004e42b6 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e42b9 */
    f->r[4]+=4;return;
mi_004e42ba:;
    /* 004e42ba */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e42bc */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e42be */
    a=0x0000001fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e42c0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e42c5u;f->boundary(0x004e42c0u,0x004e2970u,f);
    /* 004e42c5 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004e42c8:;
    /* 004e42c8 */
    f->r[4]+=4;return;
}

void restore_warning_C_004e42d0(struct restore_warning_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004e42d0 */
    a=MI_U32((0x00605a3cu));b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004e42d7 */
    if((f->flags&MI_ZF))goto mi_004e4482;
    /* 004e42dd */
    a=MI_U32((0x00000004u+f->r[4]));b=0x0000002fu;t=mi_alu(f,a,b,32,1);
    /* 004e42e2 */
    if(!(f->flags&MI_ZF))goto mi_004e4482;
    /* 004e42e8 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 004e42ed */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004e42f3 */
    f->r[1]=MI_U32((0x00ea61a4u+f->r[0]));
    /* 004e42f9 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004e42fb */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e4482;
    /* 004e4301 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4303 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4305 */
    a=0x0000008du;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e430a */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e430fu;f->boundary(0x004e430au,0x004e2970u,f);
    /* 004e430f */
    f->r[0]=MI_U32((0x0078d71cu));
    /* 004e4314 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4317 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e4319 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e4482;
    /* 004e431f */
    a=0x00000010u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4321 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4326u;f->boundary(0x004e4321u,0x00443900u,f);
    /* 004e4326 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4329 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e432b */
    if(!(f->flags&MI_ZF))goto mi_004e433f;
    /* 004e432d */
    a=0x00000010u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e432f */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4334u;f->boundary(0x004e432fu,0x004439c0u,f);
    /* 004e4334 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4337 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e4339 */
    if((f->flags&MI_ZF))goto mi_004e43bf;
mi_004e433f:;
    /* 004e433f */
    f->r[0]=MI_U32((0x0078d720u));
    /* 004e4344 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e4346 */
    if(!(f->flags&MI_ZF))goto mi_004e43bf;
    /* 004e4348 */
    a=0x00000011u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e434a */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e434fu;f->boundary(0x004e434au,0x004439c0u,f);
    /* 004e434f */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4352 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e4354 */
    if((f->flags&MI_ZF))goto mi_004e43ad;
    /* 004e4356 */
    a=0x00000011u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4358 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e435du;f->boundary(0x004e4358u,0x00443950u,f);
    /* 004e435d */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4360 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e4362 */
    if(!(f->flags&MI_ZF))goto mi_004e4374;
    /* 004e4364 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4365 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4366 */
    a=0x000000c4u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e436b */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4370u;f->boundary(0x004e436bu,0x004e2970u,f);
    /* 004e4370 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4373 */
    f->r[4]+=4;return;
mi_004e4374:;
    /* 004e4374 */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 004e437a */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 004e4380 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00ea40b8u+f->r[1])))&0xffffu)<<0);
    /* 004e4387 */
    a=(f->r[0]&0xffffu);b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 004e438b */
    if((f->flags&MI_ZF))goto mi_004e4482;
    /* 004e4391 */
    a=(f->r[0]&0xffffu);b=0x00000003u;t=mi_alu(f,a,b,16,1);
    /* 004e4395 */
    if((f->flags&MI_ZF))goto mi_004e4482;
    /* 004e439b */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e439d */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e439f */
    a=0x000000c3u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e43a4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e43a9u;f->boundary(0x004e43a4u,0x004e2970u,f);
    /* 004e43a9 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e43ac */
    f->r[4]+=4;return;
mi_004e43ad:;
    /* 004e43ad */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e43af */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e43b1 */
    a=0x0000008eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e43b6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e43bbu;f->boundary(0x004e43b6u,0x004e2970u,f);
    /* 004e43bb */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e43be */
    f->r[4]+=4;return;
mi_004e43bf:;
    /* 004e43bf */
    a=0x00000010u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e43c1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e43c6u;f->boundary(0x004e43c1u,0x004439c0u,f);
    /* 004e43c6 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e43c9 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e43cb */
    if((f->flags&MI_ZF))goto mi_004e442b;
    /* 004e43cd */
    f->r[0]=MI_U32((0x0078d720u));
    /* 004e43d2 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e43d4 */
    if((f->flags&MI_ZF))goto mi_004e442b;
    /* 004e43d6 */
    a=0x00000010u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e43d8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e43ddu;f->boundary(0x004e43d8u,0x00443950u,f);
    /* 004e43dd */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e43e0 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e43e2 */
    if((f->flags&MI_ZF))goto mi_004e4419;
    /* 004e43e4 */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 004e43ea */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 004e43f0 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00ea40b6u+f->r[2])))&0xffffu)<<0);
    /* 004e43f7 */
    a=(f->r[0]&0xffffu);b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 004e43fb */
    if((f->flags&MI_ZF))goto mi_004e4482;
    /* 004e4401 */
    a=(f->r[0]&0xffffu);b=0x00000003u;t=mi_alu(f,a,b,16,1);
    /* 004e4405 */
    if((f->flags&MI_ZF))goto mi_004e4482;
    /* 004e4407 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4409 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e440b */
    a=0x000000aeu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4410 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4415u;f->boundary(0x004e4410u,0x004e2970u,f);
    /* 004e4415 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4418 */
    f->r[4]+=4;return;
mi_004e4419:;
    /* 004e4419 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e441b */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e441d */
    a=0x000000afu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4422 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4427u;f->boundary(0x004e4422u,0x004e2970u,f);
    /* 004e4427 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e442a */
    f->r[4]+=4;return;
mi_004e442b:;
    /* 004e442b */
    a=0x00000011u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e442d */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4432u;f->boundary(0x004e442du,0x004439c0u,f);
    /* 004e4432 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4435 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e4437 */
    if((f->flags&MI_ZF))goto mi_004e4482;
    /* 004e4439 */
    a=0x00000011u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e443b */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4440u;f->boundary(0x004e443bu,0x00443950u,f);
    /* 004e4440 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4443 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e4445 */
    if((f->flags&MI_ZF))goto mi_004e4474;
    /* 004e4447 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 004e444c */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004e4452 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00ea40b8u+f->r[0])))&0xffffu)<<0);
    /* 004e4459 */
    a=(f->r[0]&0xffffu);b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 004e445d */
    if((f->flags&MI_ZF))goto mi_004e4482;
    /* 004e445f */
    a=(f->r[0]&0xffffu);b=0x00000003u;t=mi_alu(f,a,b,16,1);
    /* 004e4463 */
    if((f->flags&MI_ZF))goto mi_004e4482;
    /* 004e4465 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4467 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4469 */
    a=0x00000020u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e446b */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4470u;f->boundary(0x004e446bu,0x004e2970u,f);
    /* 004e4470 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4473 */
    f->r[4]+=4;return;
mi_004e4474:;
    /* 004e4474 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4476 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4478 */
    a=0x0000001fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e447a */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e447fu;f->boundary(0x004e447au,0x004e2970u,f);
    /* 004e447f */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004e4482:;
    /* 004e4482 */
    f->r[4]+=4;return;
}

void restore_warning_C_004e4550(struct restore_warning_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004e4550 */
    a=MI_U32((0x00605a3cu));b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004e4557 */
    if((f->flags&MI_ZF))goto mi_004e47c4;
    /* 004e455d */
    a=MI_U32((0x00000004u+f->r[4]));b=0x000000e7u;t=mi_alu(f,a,b,32,1);
    /* 004e4565 */
    if(!(f->flags&MI_ZF))goto mi_004e47c4;
    /* 004e456b */
    f->r[0]=MI_U32((0x0078d678u));
    /* 004e4570 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e4572 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 004e4577 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e458b;
    /* 004e4579 */
    f->r[1]=f->r[0];
    /* 004e457b */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 004e4581 */
    f->r[2]=MI_U32((0x00ea618cu+f->r[1]));
    /* 004e4587 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 004e4589 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e45ad;
mi_004e458b:;
    /* 004e458b */
    f->r[1]=MI_U32((0x0078d6d0u));
    /* 004e4591 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004e4593 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e47c4;
    /* 004e4599 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004e459f */
    f->r[1]=MI_U32((0x00ea61e4u+f->r[0]));
    /* 004e45a5 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004e45a7 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e47c4;
mi_004e45ad:;
    /* 004e45ad */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e45af */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e45b1 */
    a=0x0000010eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e45b6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e45bbu;f->boundary(0x004e45b6u,0x004e2970u,f);
    /* 004e45bb */
    f->r[0]=MI_U32((0x0078d678u));
    /* 004e45c0 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e45c3 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e45c5 */
    if(!(f->flags&MI_ZF))goto mi_004e463b;
    /* 004e45c7 */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 004e45cd */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 004e45d3 */
    f->r[0]=MI_U32((0x00ea618cu+f->r[2]));
    /* 004e45d9 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e45db */
    if(!(f->flags&MI_ZF))goto mi_004e463b;
    /* 004e45dd */
    a=0x0000000bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e45df */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e45e4u;f->boundary(0x004e45dfu,0x00443b90u,f);
    /* 004e45e4 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e45e7 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e45e9 */
    if((f->flags&MI_ZF))goto mi_004e45f3;
    /* 004e45eb */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e45ed */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e45ef */
    a=0x00000024u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e45f1 */
    goto mi_004e4633;
mi_004e45f3:;
    /* 004e45f3 */
    a=0x0000000bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e45f5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e45fau;f->boundary(0x004e45f5u,0x00443950u,f);
    /* 004e45fa */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e45fd */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e45ff */
    if((f->flags&MI_ZF))goto mi_004e462a;
    /* 004e4601 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 004e4606 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004e460c */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00ea40acu+f->r[0])))&0xffffu)<<0);
    /* 004e4613 */
    a=(f->r[0]&0xffffu);b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 004e4617 */
    if((f->flags&MI_ZF))goto mi_004e463b;
    /* 004e4619 */
    a=(f->r[0]&0xffffu);b=0x00000003u;t=mi_alu(f,a,b,16,1);
    /* 004e461d */
    if((f->flags&MI_ZF))goto mi_004e463b;
    /* 004e461f */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4621 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4623 */
    a=0x00000093u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4628 */
    goto mi_004e4633;
mi_004e462a:;
    /* 004e462a */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e462c */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e462e */
    a=0x00000092u;f->r[4]-=4;MI_U32(f->r[4])=a;
mi_004e4633:;
    /* 004e4633 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4638u;f->boundary(0x004e4633u,0x004e2970u,f);
    /* 004e4638 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004e463b:;
    /* 004e463b */
    f->r[0]=MI_U32((0x0078d6d0u));
    /* 004e4640 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e4642 */
    if(!(f->flags&MI_ZF))goto mi_004e47c4;
    /* 004e4648 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 004e464d */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004e4653 */
    f->r[1]=MI_U32((0x00ea61e4u+f->r[0]));
    /* 004e4659 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004e465b */
    if(!(f->flags&MI_ZF))goto mi_004e47c4;
    /* 004e4661 */
    a=0x00000020u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4663 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4668u;f->boundary(0x004e4663u,0x00443900u,f);
    /* 004e4668 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e466b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e466d */
    if(!(f->flags&MI_ZF))goto mi_004e4681;
    /* 004e466f */
    a=0x00000020u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4671 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4676u;f->boundary(0x004e4671u,0x004439c0u,f);
    /* 004e4676 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4679 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e467b */
    if((f->flags&MI_ZF))goto mi_004e4701;
mi_004e4681:;
    /* 004e4681 */
    f->r[0]=MI_U32((0x0078d760u));
    /* 004e4686 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e4688 */
    if(!(f->flags&MI_ZF))goto mi_004e4701;
    /* 004e468a */
    a=0x00000021u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e468c */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4691u;f->boundary(0x004e468cu,0x004439c0u,f);
    /* 004e4691 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4694 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e4696 */
    if((f->flags&MI_ZF))goto mi_004e46ef;
    /* 004e4698 */
    a=0x00000021u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e469a */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e469fu;f->boundary(0x004e469au,0x00443950u,f);
    /* 004e469f */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e46a2 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e46a4 */
    if(!(f->flags&MI_ZF))goto mi_004e46b6;
    /* 004e46a6 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e46a7 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e46a8 */
    a=0x0000010bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e46ad */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e46b2u;f->boundary(0x004e46adu,0x004e2970u,f);
    /* 004e46b2 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e46b5 */
    f->r[4]+=4;return;
mi_004e46b6:;
    /* 004e46b6 */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 004e46bc */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 004e46c2 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00ea40d8u+f->r[1])))&0xffffu)<<0);
    /* 004e46c9 */
    a=(f->r[0]&0xffffu);b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 004e46cd */
    if((f->flags&MI_ZF))goto mi_004e47c4;
    /* 004e46d3 */
    a=(f->r[0]&0xffffu);b=0x00000003u;t=mi_alu(f,a,b,16,1);
    /* 004e46d7 */
    if((f->flags&MI_ZF))goto mi_004e47c4;
    /* 004e46dd */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e46df */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e46e1 */
    a=0x0000010cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e46e6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e46ebu;f->boundary(0x004e46e6u,0x004e2970u,f);
    /* 004e46eb */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e46ee */
    f->r[4]+=4;return;
mi_004e46ef:;
    /* 004e46ef */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e46f1 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e46f3 */
    a=0x0000010du;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e46f8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e46fdu;f->boundary(0x004e46f8u,0x004e2970u,f);
    /* 004e46fd */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4700 */
    f->r[4]+=4;return;
mi_004e4701:;
    /* 004e4701 */
    a=0x00000020u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4703 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4708u;f->boundary(0x004e4703u,0x004439c0u,f);
    /* 004e4708 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e470b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e470d */
    if((f->flags&MI_ZF))goto mi_004e476d;
    /* 004e470f */
    f->r[0]=MI_U32((0x0078d760u));
    /* 004e4714 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e4716 */
    if((f->flags&MI_ZF))goto mi_004e476d;
    /* 004e4718 */
    a=0x00000020u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e471a */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e471fu;f->boundary(0x004e471au,0x00443950u,f);
    /* 004e471f */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4722 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e4724 */
    if((f->flags&MI_ZF))goto mi_004e475b;
    /* 004e4726 */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 004e472c */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 004e4732 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00ea40d6u+f->r[2])))&0xffffu)<<0);
    /* 004e4739 */
    a=(f->r[0]&0xffffu);b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 004e473d */
    if((f->flags&MI_ZF))goto mi_004e47c4;
    /* 004e4743 */
    a=(f->r[0]&0xffffu);b=0x00000003u;t=mi_alu(f,a,b,16,1);
    /* 004e4747 */
    if((f->flags&MI_ZF))goto mi_004e47c4;
    /* 004e4749 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e474b */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e474d */
    a=0x00000107u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4752 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4757u;f->boundary(0x004e4752u,0x004e2970u,f);
    /* 004e4757 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e475a */
    f->r[4]+=4;return;
mi_004e475b:;
    /* 004e475b */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e475d */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e475f */
    a=0x00000108u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4764 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4769u;f->boundary(0x004e4764u,0x004e2970u,f);
    /* 004e4769 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e476c */
    f->r[4]+=4;return;
mi_004e476d:;
    /* 004e476d */
    a=0x00000021u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e476f */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4774u;f->boundary(0x004e476fu,0x004439c0u,f);
    /* 004e4774 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4777 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e4779 */
    if((f->flags&MI_ZF))goto mi_004e47c4;
    /* 004e477b */
    a=0x00000021u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e477d */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4782u;f->boundary(0x004e477du,0x00443950u,f);
    /* 004e4782 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4785 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e4787 */
    if((f->flags&MI_ZF))goto mi_004e47b6;
    /* 004e4789 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 004e478e */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004e4794 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00ea40d8u+f->r[0])))&0xffffu)<<0);
    /* 004e479b */
    a=(f->r[0]&0xffffu);b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 004e479f */
    if((f->flags&MI_ZF))goto mi_004e47c4;
    /* 004e47a1 */
    a=(f->r[0]&0xffffu);b=0x00000003u;t=mi_alu(f,a,b,16,1);
    /* 004e47a5 */
    if((f->flags&MI_ZF))goto mi_004e47c4;
    /* 004e47a7 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e47a9 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e47ab */
    a=0x00000020u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e47ad */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e47b2u;f->boundary(0x004e47adu,0x004e2970u,f);
    /* 004e47b2 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e47b5 */
    f->r[4]+=4;return;
mi_004e47b6:;
    /* 004e47b6 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e47b8 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e47ba */
    a=0x0000001fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e47bc */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e47c1u;f->boundary(0x004e47bcu,0x004e2970u,f);
    /* 004e47c1 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004e47c4:;
    /* 004e47c4 */
    f->r[4]+=4;return;
}

void restore_warning_C_004e4ed0(struct restore_warning_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004e4ed0 */
    f->r[0]=MI_U32((0x00605a3cu));
    /* 004e4ed5 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4ed6 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004e4ed9 */
    if((f->flags&MI_ZF))goto mi_004e5068;
    /* 004e4edf */
    f->r[6]=MI_U32((0x00000008u+f->r[4]));
    /* 004e4ee3 */
    a=f->r[6];b=0x000000e4u;t=mi_alu(f,a,b,32,1);
    /* 004e4ee9 */
    if((f->flags&MI_ZF))goto mi_004e4eff;
    /* 004e4eeb */
    a=f->r[6];b=0x000000e3u;t=mi_alu(f,a,b,32,1);
    /* 004e4ef1 */
    if((f->flags&MI_ZF))goto mi_004e4eff;
    /* 004e4ef3 */
    a=f->r[6];b=0x000000e1u;t=mi_alu(f,a,b,32,1);
    /* 004e4ef9 */
    if(!(f->flags&MI_ZF))goto mi_004e5068;
mi_004e4eff:;
    /* 004e4eff */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 004e4f04 */
    f->r[1]=f->r[0];
    /* 004e4f06 */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 004e4f0c */
    f->r[2]=MI_U32((0x00ea61ecu+f->r[1]));
    /* 004e4f12 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 004e4f14 */
    if(!(f->flags&MI_ZF))goto mi_004e4f6d;
    /* 004e4f16 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4f18 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4f1a */
    a=0x000000f2u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4f1f */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4f24u;f->boundary(0x004e4f1fu,0x004e2970u,f);
    /* 004e4f24 */
    a=0x00000023u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4f26 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4f2bu;f->boundary(0x004e4f26u,0x00443950u,f);
    /* 004e4f2b */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4f2e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e4f30 */
    if((f->flags&MI_ZF))goto mi_004e4f5a;
    /* 004e4f32 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 004e4f37 */
    f->r[2]=f->r[0];
    /* 004e4f39 */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 004e4f3f */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x00ea40dcu+f->r[2])))&0xffffu)<<0);
    /* 004e4f46 */
    a=(f->r[1]&0xffffu);b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 004e4f4a */
    if((f->flags&MI_ZF))goto mi_004e4f6d;
    /* 004e4f4c */
    a=(f->r[1]&0xffffu);b=0x00000003u;t=mi_alu(f,a,b,16,1);
    /* 004e4f50 */
    if((f->flags&MI_ZF))goto mi_004e4f6d;
    /* 004e4f52 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4f54 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4f56 */
    a=0x00000020u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4f58 */
    goto mi_004e4f60;
mi_004e4f5a:;
    /* 004e4f5a */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4f5c */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4f5e */
    a=0x0000001fu;f->r[4]-=4;MI_U32(f->r[4])=a;
mi_004e4f60:;
    /* 004e4f60 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4f65u;f->boundary(0x004e4f60u,0x004e2970u,f);
    /* 004e4f65 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 004e4f6a */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004e4f6d:;
    /* 004e4f6d */
    a=f->r[6];b=0x000000e1u;t=mi_alu(f,a,b,32,1);
    /* 004e4f73 */
    if(!(f->flags&MI_ZF))goto mi_004e4fed;
    /* 004e4f75 */
    f->r[1]=MI_U32((0x0078d6c0u));
    /* 004e4f7b */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004e4f7d */
    if(!(f->flags&MI_ZF))goto mi_004e5068;
    /* 004e4f83 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004e4f89 */
    f->r[1]=MI_U32((0x00ea61d4u+f->r[0]));
    /* 004e4f8f */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004e4f91 */
    if(!(f->flags&MI_ZF))goto mi_004e5068;
    /* 004e4f97 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4f99 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4f9b */
    a=0x000000f1u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4fa0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4fa5u;f->boundary(0x004e4fa0u,0x004e2970u,f);
    /* 004e4fa5 */
    a=0x0000001du;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4fa7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4facu;f->boundary(0x004e4fa7u,0x00443950u,f);
    /* 004e4fac */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4faf */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e4fb1 */
    if((f->flags&MI_ZF))goto mi_004e505a;
    /* 004e4fb7 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 004e4fbc */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004e4fc2 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00ea40d0u+f->r[0])))&0xffffu)<<0);
    /* 004e4fc9 */
    a=(f->r[0]&0xffffu);b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 004e4fcd */
    if((f->flags&MI_ZF))goto mi_004e5068;
    /* 004e4fd3 */
    a=(f->r[0]&0xffffu);b=0x00000003u;t=mi_alu(f,a,b,16,1);
    /* 004e4fd7 */
    if((f->flags&MI_ZF))goto mi_004e5068;
    /* 004e4fdd */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4fdf */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4fe1 */
    a=0x00000020u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e4fe3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e4fe8u;f->boundary(0x004e4fe3u,0x004e2970u,f);
    /* 004e4fe8 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e4feb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e4fec */
    f->r[4]+=4;return;
mi_004e4fed:;
    /* 004e4fed */
    a=f->r[6];b=0x000000e3u;t=mi_alu(f,a,b,32,1);
    /* 004e4ff3 */
    if(!(f->flags&MI_ZF))goto mi_004e5068;
    /* 004e4ff5 */
    f->r[1]=MI_U32((0x0078d6b4u));
    /* 004e4ffb */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004e4ffd */
    if(!(f->flags&MI_ZF))goto mi_004e5068;
    /* 004e4fff */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004e5005 */
    f->r[1]=MI_U32((0x00ea61c8u+f->r[0]));
    /* 004e500b */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004e500d */
    if(!(f->flags&MI_ZF))goto mi_004e5068;
    /* 004e500f */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e5011 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e5013 */
    a=0x000000f8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e5018 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e501du;f->boundary(0x004e5018u,0x004e2970u,f);
    /* 004e501d */
    a=0x0000001au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e501f */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e5024u;f->boundary(0x004e501fu,0x00443950u,f);
    /* 004e5024 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e5027 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e5029 */
    if((f->flags&MI_ZF))goto mi_004e505a;
    /* 004e502b */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 004e5031 */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 004e5037 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00ea40cau+f->r[1])))&0xffffu)<<0);
    /* 004e503e */
    a=(f->r[0]&0xffffu);b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 004e5042 */
    if((f->flags&MI_ZF))goto mi_004e5068;
    /* 004e5044 */
    a=(f->r[0]&0xffffu);b=0x00000003u;t=mi_alu(f,a,b,16,1);
    /* 004e5048 */
    if((f->flags&MI_ZF))goto mi_004e5068;
    /* 004e504a */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e504c */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e504e */
    a=0x00000020u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e5050 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e5055u;f->boundary(0x004e5050u,0x004e2970u,f);
    /* 004e5055 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e5058 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e5059 */
    f->r[4]+=4;return;
mi_004e505a:;
    /* 004e505a */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e505c */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e505e */
    a=0x0000001fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e5060 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e5065u;f->boundary(0x004e5060u,0x004e2970u,f);
    /* 004e5065 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004e5068:;
    /* 004e5068 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e5069 */
    f->r[4]+=4;return;
}

void restore_warning_C_004e5070(struct restore_warning_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004e5070 */
    f->r[0]=MI_U32((0x00605a3cu));
    /* 004e5075 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e5076 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004e5079 */
    if((f->flags&MI_ZF))goto mi_004e50e6;
    /* 004e507b */
    f->r[6]=MI_U32((0x00000008u+f->r[4]));
    /* 004e507f */
    a=f->r[6];b=0x0000001fu;t=mi_alu(f,a,b,32,1);
    /* 004e5082 */
    if((f->flags&MI_ZF))goto mi_004e508e;
    /* 004e5084 */
    a=f->r[6];b=0x00000021u;t=mi_alu(f,a,b,32,1);
    /* 004e5087 */
    if((f->flags&MI_ZF))goto mi_004e508e;
    /* 004e5089 */
    a=f->r[6];b=0x0000001eu;t=mi_alu(f,a,b,32,1);
    /* 004e508c */
    if(!(f->flags&MI_ZF))goto mi_004e50e6;
mi_004e508e:;
    /* 004e508e */
    f->r[0]=MI_U32((0x00705b80u));
    /* 004e5093 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e5095 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e50a8;
    /* 004e5097 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e5099 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e509b */
    a=0x000000d0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e50a0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e50a5u;f->boundary(0x004e50a0u,0x004e2970u,f);
    /* 004e50a5 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004e50a8:;
    /* 004e50a8 */
    f->r[0]=MI_U32((0x00705b78u));
    /* 004e50ad */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e50af */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e50c7;
    /* 004e50b1 */
    a=f->r[6];b=0x0000001fu;t=mi_alu(f,a,b,32,1);
    /* 004e50b4 */
    if((f->flags&MI_ZF))goto mi_004e50c7;
    /* 004e50b6 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e50b8 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e50ba */
    a=0x000000d1u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e50bf */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e50c4u;f->boundary(0x004e50bfu,0x004e2970u,f);
    /* 004e50c4 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004e50c7:;
    /* 004e50c7 */
    f->r[0]=MI_U32((0x00705b7cu));
    /* 004e50cc */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e50ce */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e50e6;
    /* 004e50d0 */
    a=f->r[6];b=0x00000021u;t=mi_alu(f,a,b,32,1);
    /* 004e50d3 */
    if(!(f->flags&MI_ZF))goto mi_004e50e6;
    /* 004e50d5 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e50d7 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e50d9 */
    a=0x000000d2u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e50de */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e50e3u;f->boundary(0x004e50deu,0x004e2970u,f);
    /* 004e50e3 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004e50e6:;
    /* 004e50e6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e50e7 */
    f->r[4]+=4;return;
}

static void warning_boundary(uint32_t site,uint32_t target,struct restore_warning_frame *f)
{
 uint32_t sp=f->r[4],a=MI_U32(sp+4u);(void)site;
 switch(target){
 case 0x443900u:f->r[0]=FUN_00443900(a);break;
 case 0x443950u:f->r[0]=FUN_00443950((int32_t)a);break;
 case 0x4439c0u:f->r[0]=FUN_004439c0((int32_t)a);break;
 case 0x443b90u:f->r[0]=FUN_00443b90((int32_t)a);break;
 case 0x4e2970u:FUN_004e2970((int32_t)a);break;
 case 0x4f44e0u:f->r[0]=(uint32_t)FUN_004f44e0();break;
 default:abort();
 }
 f->r[4]=sp+4u;
}
static void warning(uint32_t entry,int32_t tool)
{
 uint32_t stack[256],i,sp;struct restore_warning_frame f={{0},2,warning_boundary};
 for(i=0;i<256;++i)stack[i]=0x8e47cdefu;
 stack[241]=(uint32_t)tool;sp=(uint32_t)(uintptr_t)(stack+240);f.r[4]=sp;
 switch(entry){
 case 0x4e3760u:restore_warning_C_004e3760(&f);break;
 case 0x4e3aa0u:restore_warning_C_004e3aa0(&f);break;
 case 0x4e3da0u:restore_warning_C_004e3da0(&f);break;
 case 0x4e3f60u:restore_warning_C_004e3f60(&f);break;
 case 0x4e4120u:restore_warning_C_004e4120(&f);break;
 case 0x4e42d0u:restore_warning_C_004e42d0(&f);break;
 case 0x4e4550u:restore_warning_C_004e4550(&f);break;
 case 0x4e47d0u:restore_warning_C_004e47d0(&f);break;
 case 0x4e4ed0u:restore_warning_C_004e4ed0(&f);break;
 case 0x4e5070u:restore_warning_C_004e5070(&f);break;
 default:abort();
 }
 if(f.r[4]!=sp+4u||f.r[3]||f.r[5]||f.r[6]||f.r[7])abort();
 for(i=0;i<16;++i)if(stack[i]!=0x8e47cdefu)abort();for(i=242;i<256;++i)if(stack[i]!=0x8e47cdefu)abort();
}
void FUN_004e3760(int32_t tool){warning(0x4e3760u,tool);}
void FUN_004e3aa0(int32_t tool){warning(0x4e3aa0u,tool);}
void FUN_004e3da0(int32_t tool){warning(0x4e3da0u,tool);}
void FUN_004e3f60(int32_t tool){warning(0x4e3f60u,tool);}
void FUN_004e4120(int32_t tool){warning(0x4e4120u,tool);}
void FUN_004e42d0(int32_t tool){warning(0x4e42d0u,tool);}
void FUN_004e4550(int32_t tool){warning(0x4e4550u,tool);}
void FUN_004e47d0(int32_t tool){warning(0x4e47d0u,tool);}
void FUN_004e4ed0(int32_t tool){warning(0x4e4ed0u,tool);}
void FUN_004e5070(int32_t tool){warning(0x4e5070u,tool);}
