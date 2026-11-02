/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "menu_004f96e0.h"
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

static const uint8_t mi_rom_004fa310[]={0x68,0x97,0x4f,0x00,0x85,0x97,0x4f,0x00,0xa6,0x97,0x4f,0x00,0xb0,0x97,0x4f,0x00};
static const uint8_t mi_rom_004fa320[]={0x00,0x00,0x01,0x01,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x02};
static __inline uint32_t mi_read(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x004fa310u&&width<=16u&&(offset=address-0x004fa310u)<=16u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004fa310[offset+k]<<(k*8u);return result;
 }
 if(address>=0x004fa320u&&width<=15u&&(offset=address-0x004fa320u)<=15u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004fa320[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}

void restore_C_004f96e0(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004f96e0 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00e38df8u),1u))&0xffu)<<0);
    /* 004f96e5 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f96e6 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f96e7 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 004f96e9 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004f96eb */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f96ec */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[6]);
    /* 004f96f2 */
    if((f->flags&MI_ZF))goto mi_004f97d9;
    /* 004f96f8 */
    a=mi_read((0x00eadd50u),4u);b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 004f96ff */
    if(!(f->flags&MI_ZF))goto mi_004f97c7;
    /* 004f9705 */
    f->r[0]=mi_read((0x00eadcccu),4u);
    /* 004f970a */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 004f970d */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004f9710 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bcb9f0u+f->r[0]),1u))&0xffu)<<0);
    /* 004f9716 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004f9718 */
    if((f->flags&MI_ZF))goto mi_004f97c7;
    /* 004f971e */
    a=mi_read((0x00bcb9ccu+f->r[0]),2u);b=0x0000000du;t=mi_alu(f,a,b,16,1);
    /* 004f9726 */
    if(!(f->flags&MI_ZF))goto mi_004f9743;
    /* 004f9728 */
    a=mi_read((0x00bcb9f2u+f->r[0]),2u);b=(f->r[6]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 004f972f */
    if(!(f->flags&MI_ZF))goto mi_004f9743;
    /* 004f9731 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004f9732 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004f9733 */
    MI_U8((0x00e38ddau))=(uint8_t)(0x00000049u);
    /* 004f973a */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004f9741 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004f9742 */
    f->r[4]+=4;return;
mi_004f9743:;
    /* 004f9743 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x00bcb9f2u+f->r[0]),2u))&0xffffu)<<0);
    /* 004f974a */
    a=(f->r[0]&0xffffu);b=0x00000006u;t=mi_alu(f,a,b,16,1);
    /* 004f974e */
    if((f->flags&MI_ZF))goto mi_004f97c7;
    /* 004f9750 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)(f->r[0]&0xffffu);
    /* 004f9753 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004f9754 */
    a=f->r[0];b=0x0000000eu;t=mi_alu(f,a,b,32,1);
    /* 004f9757 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004f97b0;
    /* 004f9759 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004f975b */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x004fa320u+f->r[0]),1u))&0xffu)<<0);
    /* 004f9761 */
    switch(mi_read((0x004fa310u+f->r[1]*4u),4u)){case 0x004f9768u:goto mi_004f9768;case 0x004f9785u:goto mi_004f9785;case 0x004f97a6u:goto mi_004f97a6;case 0x004f97b0u:goto mi_004f97b0;default:abort();}
mi_004f9768:;
    /* 004f9768 */
    MI_U32((0x00d3b198u))=(uint32_t)(f->r[6]);
    /* 004f976e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f9773u;f->boundary(0x004f976eu,0x00406720u,f);
    /* 004f9773 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004f9774 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004f9775 */
    MI_U8((0x00e38ddau))=(uint8_t)(0x0000001eu);
    /* 004f977c */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004f9783 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004f9784 */
    f->r[4]+=4;return;
mi_004f9785:;
    /* 004f9785 */
    MI_U32((0x00d3b198u))=(uint32_t)(0x00000001u);
    /* 004f978f */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f9794u;f->boundary(0x004f978fu,0x00406720u,f);
    /* 004f9794 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004f9795 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004f9796 */
    MI_U8((0x00e38ddau))=(uint8_t)(0x0000001eu);
    /* 004f979d */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004f97a4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004f97a5 */
    f->r[4]+=4;return;
mi_004f97a6:;
    /* 004f97a6 */
    MI_U32((0x00d3b198u))=(uint32_t)(0x00000002u);
mi_004f97b0:;
    /* 004f97b0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f97b5u;f->boundary(0x004f97b0u,0x00406720u,f);
    /* 004f97b5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004f97b6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004f97b7 */
    MI_U8((0x00e38ddau))=(uint8_t)(0x0000001eu);
    /* 004f97be */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004f97c5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004f97c6 */
    f->r[4]+=4;return;
mi_004f97c7:;
    /* 004f97c7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004f97c8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004f97c9 */
    MI_U8((0x00e38ddau))=(uint8_t)(0x00000001u);
    /* 004f97d0 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004f97d7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004f97d8 */
    f->r[4]+=4;return;
mi_004f97d9:;
    /* 004f97d9 */
    a=mi_read((0x00eadcc0u),4u);b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004f97df */
    if((f->flags&MI_ZF))goto mi_004f9916;
    /* 004f97e5 */
    f->r[0]=mi_read((0x00eadd6cu),4u);
    /* 004f97ea */
    f->r[2]=f->r[0];
    /* 004f97ec */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004f97ef */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004f97f1 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x007999e8u+f->r[2]*8u),2u))&0xffffu)<<0);
    /* 004f97f9 */
    a=(f->r[0]&0xffffu);b=0x00000048u;t=mi_alu(f,a,b,16,1);
    /* 004f97fd */
    if((f->flags&MI_ZF))goto mi_004f9849;
    /* 004f97ff */
    a=(f->r[0]&0xffffu);b=0x000000c1u;t=mi_alu(f,a,b,16,1);
    /* 004f9803 */
    if((f->flags&MI_ZF))goto mi_004f9849;
    /* 004f9805 */
    a=(f->r[0]&0xffffu);b=0x00000046u;t=mi_alu(f,a,b,16,1);
    /* 004f9809 */
    if((f->flags&MI_ZF))goto mi_004f9819;
    /* 004f980b */
    a=(f->r[0]&0xffffu);b=0x000000beu;t=mi_alu(f,a,b,16,1);
    /* 004f980f */
    if((f->flags&MI_ZF))goto mi_004f9819;
    /* 004f9811 */
    f->r[6]=mi_read((0x00f53dd8u),4u);
    /* 004f9817 */
    goto mi_004f985f;
mi_004f9819:;
    /* 004f9819 */
    f->r[0]=mi_read((0x00f53e6cu),4u);
    /* 004f981e */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 004f9820 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004f9822 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004f985f;
    /* 004f9824 */
    f->r[5]=0x00f53e70u;
mi_004f9829:;
    /* 004f9829 */
    f->r[0]=mi_read((0x00000000u+f->r[5]),4u);
    /* 004f982c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f982d */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f9832u;f->boundary(0x004f982du,0x005013a0u,f);
    /* 004f9832 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004f9835 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004f9837 */
    if((f->flags&MI_ZF))goto mi_004f983a;
    /* 004f9839 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
mi_004f983a:;
    /* 004f983a */
    f->r[0]=mi_read((0x00f53e6cu),4u);
    /* 004f983f */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 004f9840 */
    a=f->r[5];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 004f9843 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004f9845 */
    if(mi_signed_less(f->flags))goto mi_004f9829;
    /* 004f9847 */
    goto mi_004f985f;
mi_004f9849:;
    /* 004f9849 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f984eu;f->boundary(0x004f9849u,0x00502780u,f);
    /* 004f984e */
    f->r[6]=mi_read((0x00f53e6cu),4u);
    /* 004f9854 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004f9856 */
    if(!mi_signed_less(f->flags))goto mi_004f985f;
    /* 004f9858 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f985du;f->boundary(0x004f9858u,0x00502780u,f);
    /* 004f985d */
    f->r[6]=f->r[0];
mi_004f985f:;
    /* 004f985f */
    f->r[1]=mi_read((0x00eadd8cu),4u);
    /* 004f9865 */
    f->r[0]=mi_read((0x00eadd90u),4u);
    /* 004f986a */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f986b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f986c */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f986d */
    a=0x005c68c8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9872 */
    a=f->r[0];b=0x000000fau;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f9877 */
    a=0x0000004eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9879 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f987a */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f987fu;f->boundary(0x004f987au,0x00426450u,f);
    /* 004f987f */
    f->r[2]=mi_read((0x00eadd90u),4u);
    /* 004f9885 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004f9888 */
    a=f->r[2];b=0x000000fau;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004f988e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f988f */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9890 */
    a=0x005c68c8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9895 */
    a=0x0000004eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9897 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9898 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f989du;f->boundary(0x004f9898u,0x004d6740u,f);
    /* 004f989d */
    MI_U32((0x007056c4u))=(uint32_t)(f->r[0]);
    /* 004f98a2 */
    f->r[0]=mi_read((0x00eadd90u),4u);
    /* 004f98a7 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f98a8 */
    a=0x005c68c8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f98ad */
    a=f->r[0];b=0x000000fau;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f98b2 */
    a=0x0000004eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f98b4 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f98b5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f98bau;f->boundary(0x004f98b5u,0x004261d0u,f);
    /* 004f98ba */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004f98bd */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004f98bf */
    if(!(f->flags&MI_ZF))goto mi_004fa309;
    /* 004f98c5 */
    f->r[1]=(0x00000000u+f->r[6]*4u);
    /* 004f98cc */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f98ce */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f98cf */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f98d4u;f->boundary(0x004f98cfu,0x004cde80u,f);
    /* 004f98d4 */
    f->r[2]=mi_read((0x00eadd90u),4u);
    /* 004f98da */
    a=f->r[0];b=0x00000007u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f98dd */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004f98e0 */
    a=0x0000000au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f98e2 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f98e4 */
    f->r[6]=(0xfffffff8u+f->r[0]);
    /* 004f98e7 */
    a=0x005c7b10u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f98ec */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f98ed */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f98ee */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f98f3u;f->boundary(0x004f98eeu,0x00425bc0u,f);
    /* 004f98f3 */
    f->r[0]=mi_read((0x00eadd90u),4u);
    /* 004f98f8 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f98fa */
    a=0x005c7b10u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f98ff */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9900 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9901 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f9906u;f->boundary(0x004f9901u,0x00425990u,f);
    /* 004f9906 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004f9909 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004f990b */
    if(!(f->flags&MI_ZF))goto mi_004fa309;
    /* 004f9911 */
    goto mi_004f9a31;
mi_004f9916:;
    /* 004f9916 */
    f->r[0]=mi_read((0x00eadd50u),4u);
    /* 004f991b */
    a=f->r[0];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 004f991e */
    if(!(f->flags&MI_ZF))goto mi_004f997f;
    /* 004f9920 */
    f->r[1]=mi_read((0x00eadd84u),4u);
    /* 004f9926 */
    f->r[2]=mi_read((0x00eadd8cu),4u);
    /* 004f992c */
    a=f->r[1];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004f992f */
    a=0x0000000au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9931 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9933 */
    f->r[0]=(0xffffffd8u+f->r[1]+f->r[2]*1u);
    /* 004f9937 */
    f->r[1]=mi_read((0x00eadd90u),4u);
    /* 004f993d */
    a=0x005c7b10u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9942 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9943 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9944 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f9949u;f->boundary(0x004f9944u,0x00425bc0u,f);
    /* 004f9949 */
    f->r[2]=mi_read((0x00eadd84u),4u);
    /* 004f994f */
    f->r[0]=mi_read((0x00eadd8cu),4u);
    /* 004f9954 */
    a=f->r[2];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004f9957 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9959 */
    a=0x005c7b10u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f995e */
    f->r[1]=(0xffffffd8u+f->r[2]+f->r[0]*1u);
    /* 004f9962 */
    f->r[2]=mi_read((0x00eadd90u),4u);
    /* 004f9968 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9969 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
mi_004f996a:;
    /* 004f996a */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f996fu;f->boundary(0x004f996au,0x00425990u,f);
    /* 004f996f */
    a=f->r[4];b=0x00000024u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004f9972 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004f9974 */
    if(!(f->flags&MI_ZF))goto mi_004fa309;
    /* 004f997a */
    goto mi_004f9a31;
mi_004f997f:;
    /* 004f997f */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 004f9982 */
    if(!(f->flags&MI_ZF))goto mi_004f99cf;
    /* 004f9984 */
    f->r[0]=mi_read((0x00eadd84u),4u);
    /* 004f9989 */
    f->r[1]=mi_read((0x00eadd8cu),4u);
    /* 004f998f */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004f9992 */
    a=0x0000000au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9994 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9996 */
    f->r[2]=(0xffffffd8u+f->r[0]+f->r[1]*1u);
    /* 004f999a */
    f->r[0]=mi_read((0x00eadd90u),4u);
    /* 004f999f */
    a=0x005c7b10u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f99a4 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f99a5 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f99a6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f99abu;f->boundary(0x004f99a6u,0x00425bc0u,f);
    /* 004f99ab */
    f->r[1]=mi_read((0x00eadd84u),4u);
    /* 004f99b1 */
    f->r[2]=mi_read((0x00eadd8cu),4u);
    /* 004f99b7 */
    a=f->r[1];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004f99ba */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f99bc */
    a=0x005c7b10u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f99c1 */
    f->r[0]=(0xffffffd8u+f->r[1]+f->r[2]*1u);
    /* 004f99c5 */
    f->r[1]=mi_read((0x00eadd90u),4u);
    /* 004f99cb */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f99cc */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f99cd */
    goto mi_004f996a;
mi_004f99cf:;
    /* 004f99cf */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 004f99d2 */
    if(!(f->flags&MI_ZF))goto mi_004f99d9;
    /* 004f99d4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f99d9u;f->boundary(0x004f99d4u,0x00515c60u,f);
mi_004f99d9:;
    /* 004f99d9 */
    f->r[2]=mi_read((0x00eadd84u),4u);
    /* 004f99df */
    f->r[0]=mi_read((0x00eadd8cu),4u);
    /* 004f99e4 */
    a=f->r[2];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004f99e7 */
    a=0x0000000au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f99e9 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f99eb */
    f->r[1]=(0xffffffd8u+f->r[2]+f->r[0]*1u);
    /* 004f99ef */
    f->r[2]=mi_read((0x00eadd90u),4u);
    /* 004f99f5 */
    a=0x005c7b10u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f99fa */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f99fb */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f99fc */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f9a01u;f->boundary(0x004f99fcu,0x00425bc0u,f);
    /* 004f9a01 */
    f->r[0]=mi_read((0x00eadd84u),4u);
    /* 004f9a06 */
    f->r[1]=mi_read((0x00eadd8cu),4u);
    /* 004f9a0c */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004f9a0f */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9a11 */
    a=0x005c7b10u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9a16 */
    f->r[2]=(0xffffffd8u+f->r[0]+f->r[1]*1u);
    /* 004f9a1a */
    f->r[0]=mi_read((0x00eadd90u),4u);
    /* 004f9a1f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9a20 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9a21 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f9a26u;f->boundary(0x004f9a21u,0x00425990u,f);
    /* 004f9a26 */
    a=f->r[4];b=0x00000024u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004f9a29 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004f9a2b */
    if(!(f->flags&MI_ZF))goto mi_004fa309;
mi_004f9a31:;
    /* 004f9a31 */
    f->r[0]=mi_read((0x00eadd50u),4u);
    /* 004f9a36 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004f9a38 */
    if((f->flags&MI_ZF))goto mi_004fa309;
    /* 004f9a3e */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 004f9a41 */
    if(!(f->flags&MI_ZF))goto mi_004f9b66;
    /* 004f9a47 */
    f->r[0]=mi_read((0x00eadd8cu),4u);
    /* 004f9a4c */
    f->r[1]=mi_read((0x00eadd90u),4u);
    /* 004f9a52 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9a53 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9a54 */
    a=0x00000005u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9a56 */
    a=0x005c66d8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9a5b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9a5c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9a5d */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f9a62u;f->boundary(0x004f9a5du,0x00426450u,f);
    /* 004f9a62 */
    f->r[1]=mi_read((0x00eadd8cu),4u);
    /* 004f9a68 */
    f->r[2]=mi_read((0x00eadd90u),4u);
    /* 004f9a6e */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004f9a71 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9a72 */
    a=0x00000005u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9a74 */
    a=0x005c66d8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9a79 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9a7a */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9a7b */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f9a80u;f->boundary(0x004f9a7bu,0x004d6740u,f);
    /* 004f9a80 */
    f->r[1]=mi_read((0x00eadd90u),4u);
    /* 004f9a86 */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004f9a8b */
    f->r[0]=mi_read((0x00eadd8cu),4u);
    /* 004f9a90 */
    a=0x00000005u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9a92 */
    a=0x005c66d8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9a97 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9a98 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9a99 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f9a9eu;f->boundary(0x004f9a99u,0x004261d0u,f);
    /* 004f9a9e */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004f9aa1 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004f9aa3 */
    if(!(f->flags&MI_ZF))goto mi_004fa309;
    /* 004f9aa9 */
    f->r[2]=mi_read((0x00eadd84u),4u);
    /* 004f9aaf */
    f->r[0]=mi_read((0x00eadd8cu),4u);
    /* 004f9ab4 */
    f->r[1]=mi_read((0x00eadd90u),4u);
    /* 004f9aba */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9abb */
    a=f->r[2];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004f9abe */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9abf */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9ac1 */
    f->r[0]=(0xffffffd8u+f->r[2]+f->r[0]*1u);
    /* 004f9ac5 */
    f->r[2]=mi_read((0x00eadd88u),4u);
    /* 004f9acb */
    a=0x005c6768u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9ad0 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9ad1 */
    f->r[0]=(0xffffffecu+f->r[2]);
    /* 004f9ad4 */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004f9ad7 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004f9ad8 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004f9ada */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004f9adc */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f9ade */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9adf */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f9ae4u;f->boundary(0x004f9adfu,0x00426450u,f);
    /* 004f9ae4 */
    f->r[1]=mi_read((0x00eadd8cu),4u);
    /* 004f9aea */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004f9aed */
    f->r[6]=mi_read((0x00eadd90u),4u);
    /* 004f9af3 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9af4 */
    f->r[0]=mi_read((0x00eadd84u),4u);
    /* 004f9af9 */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004f9afc */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9afe */
    a=0x005c6768u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9b03 */
    f->r[2]=(0xffffffd8u+f->r[0]+f->r[1]*1u);
    /* 004f9b07 */
    f->r[0]=mi_read((0x00eadd88u),4u);
    /* 004f9b0c */
    a=f->r[0];b=0xffffffecu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f9b0f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9b10 */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004f9b13 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004f9b14 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004f9b16 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004f9b18 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f9b1a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9b1b */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f9b20u;f->boundary(0x004f9b1bu,0x004d6740u,f);
    /* 004f9b20 */
    f->r[1]=mi_read((0x00eadd84u),4u);
    /* 004f9b26 */
    f->r[2]=mi_read((0x00eadd8cu),4u);
    /* 004f9b2c */
    a=f->r[1];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004f9b2f */
    MI_U32((0x007056c4u))=(uint32_t)(f->r[0]);
    /* 004f9b34 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9b36 */
    f->r[0]=(0xffffffd8u+f->r[1]+f->r[2]*1u);
    /* 004f9b3a */
    f->r[1]=mi_read((0x00eadd88u),4u);
    /* 004f9b40 */
    a=0x005c6768u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9b45 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9b46 */
    f->r[0]=(0xffffffecu+f->r[1]);
    /* 004f9b49 */
    f->r[7]=mi_read((0x00eadd90u),4u);
    /* 004f9b4f */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004f9b52 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004f9b53 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004f9b55 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004f9b57 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f9b59 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9b5a */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f9b5fu;f->boundary(0x004f9b5au,0x004261d0u,f);
    /* 004f9b5f */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004f9b62 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004f9b63 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004f9b64 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004f9b65 */
    f->r[4]+=4;return;
mi_004f9b66:;
    /* 004f9b66 */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 004f9b69 */
    if(!(f->flags&MI_ZF))goto mi_004f9bc9;
    /* 004f9b6b */
    f->r[0]=mi_read((0x00eadd8cu),4u);
    /* 004f9b70 */
    f->r[1]=mi_read((0x00eadd90u),4u);
    /* 004f9b76 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9b77 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9b78 */
    a=0x00000005u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9b7a */
    a=0x005c6788u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9b7f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9b80 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9b81 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f9b86u;f->boundary(0x004f9b81u,0x00426450u,f);
    /* 004f9b86 */
    f->r[2]=mi_read((0x00eadd8cu),4u);
    /* 004f9b8c */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004f9b8f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9b90 */
    f->r[0]=mi_read((0x00eadd90u),4u);
    /* 004f9b95 */
    a=0x00000005u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9b97 */
    a=0x005c6788u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9b9c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9b9d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9b9e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f9ba3u;f->boundary(0x004f9b9eu,0x004d6740u,f);
    /* 004f9ba3 */
    f->r[1]=mi_read((0x00eadd8cu),4u);
    /* 004f9ba9 */
    f->r[2]=mi_read((0x00eadd90u),4u);
    /* 004f9baf */
    a=0x00000005u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9bb1 */
    a=0x005c6788u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9bb6 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9bb7 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9bb8 */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004f9bbd */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f9bc2u;f->boundary(0x004f9bbdu,0x004261d0u,f);
    /* 004f9bc2 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004f9bc5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004f9bc6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004f9bc7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004f9bc8 */
    f->r[4]+=4;return;
mi_004f9bc9:;
    /* 004f9bc9 */
    a=f->r[0];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 004f9bcc */
    if(!(f->flags&MI_ZF))goto mi_004f9c17;
    /* 004f9bce */
    f->r[0]=mi_read((0x00eadd8cu),4u);
    /* 004f9bd3 */
    f->r[1]=mi_read((0x00eadd90u),4u);
    /* 004f9bd9 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9bda */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9bdb */
    a=0x00000005u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9bdd */
    a=0x005c6818u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9be2 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9be3 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9be4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f9be9u;f->boundary(0x004f9be4u,0x00426450u,f);
    /* 004f9be9 */
    f->r[1]=mi_read((0x00eadd90u),4u);
    /* 004f9bef */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004f9bf2 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9bf3 */
    f->r[0]=mi_read((0x00eadd8cu),4u);
    /* 004f9bf8 */
    a=0x00000005u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9bfa */
    a=0x005c6818u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9bff */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9c00 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9c01 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f9c06u;f->boundary(0x004f9c01u,0x004d6740u,f);
    /* 004f9c06 */
    a=0x00000005u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9c08 */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004f9c0d */
    a=0x005c6818u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9c12 */
    goto mi_004fa0c4;
mi_004f9c17:;
    /* 004f9c17 */
    f->r[1]=mi_read((0x00eadd48u),4u);
    /* 004f9c1d */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004f9c1f */
    if((f->flags&MI_ZF))goto mi_004f9daf;
    /* 004f9c25 */
    f->r[0]=mi_read((0x00eadd8cu),4u);
    /* 004f9c2a */
    f->r[1]=mi_read((0x00eadd90u),4u);
    /* 004f9c30 */
    f->r[2]=mi_read((0x00eadd40u),4u);
    /* 004f9c36 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9c37 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9c38 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9c39 */
    a=0x005c5be8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9c3e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9c3f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9c40 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f9c45u;f->boundary(0x004f9c40u,0x00426450u,f);
    /* 004f9c45 */
    f->r[1]=mi_read((0x00eadd8cu),4u);
    /* 004f9c4b */
    f->r[2]=mi_read((0x00eadd90u),4u);
    /* 004f9c51 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004f9c54 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9c55 */
    f->r[0]=mi_read((0x00eadd40u),4u);
    /* 004f9c5a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9c5b */
    a=0x005c5be8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9c60 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9c61 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9c62 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f9c67u;f->boundary(0x004f9c62u,0x004d6740u,f);
    /* 004f9c67 */
    f->r[1]=mi_read((0x00eadd8cu),4u);
    /* 004f9c6d */
    f->r[2]=mi_read((0x00eadd90u),4u);
    /* 004f9c73 */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004f9c78 */
    f->r[0]=mi_read((0x00eadd40u),4u);
    /* 004f9c7d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9c7e */
    a=0x005c5be8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9c83 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9c84 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9c85 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f9c8au;f->boundary(0x004f9c85u,0x004261d0u,f);
    /* 004f9c8a */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004f9c8d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004f9c8f */
    if(!(f->flags&MI_ZF))goto mi_004fa309;
    /* 004f9c95 */
    f->r[0]=mi_read((0x00eadd8cu),4u);
    /* 004f9c9a */
    f->r[1]=mi_read((0x00eadd90u),4u);
    /* 004f9ca0 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9ca1 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9ca2 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9ca4 */
    a=f->r[0];b=0x000000c3u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f9ca9 */
    a=0x005c5cb0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9cae */
    a=f->r[1];b=0x0000011du;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004f9cb4 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9cb5 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9cb6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f9cbbu;f->boundary(0x004f9cb6u,0x00426450u,f);
    /* 004f9cbb */
    f->r[1]=mi_read((0x00eadd90u),4u);
    /* 004f9cc1 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004f9cc4 */
    a=f->r[1];b=0x0000011du;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004f9cca */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9ccb */
    f->r[0]=mi_read((0x00eadd8cu),4u);
    /* 004f9cd0 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9cd2 */
    a=f->r[0];b=0x000000c3u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f9cd7 */
    a=0x005c5cb0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9cdc */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9cdd */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9cde */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f9ce3u;f->boundary(0x004f9cdeu,0x004d6740u,f);
    /* 004f9ce3 */
    f->r[2]=mi_read((0x00eadd8cu),4u);
    /* 004f9ce9 */
    MI_U32((0x007056c4u))=(uint32_t)(f->r[0]);
    /* 004f9cee */
    f->r[0]=mi_read((0x00eadd90u),4u);
    /* 004f9cf3 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9cf5 */
    a=f->r[2];b=0x000000c3u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004f9cfb */
    a=0x005c5cb0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9d00 */
    a=f->r[0];b=0x0000011du;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f9d05 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9d06 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9d07 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f9d0cu;f->boundary(0x004f9d07u,0x004261d0u,f);
    /* 004f9d0c */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004f9d0f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004f9d11 */
    if(!(f->flags&MI_ZF))goto mi_004fa309;
    /* 004f9d17 */
    f->r[1]=mi_read((0x00eadce4u),4u);
    /* 004f9d1d */
    f->r[0]=mi_read((0x00eadd24u+f->r[1]*4u),4u);
    /* 004f9d24 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004f9d27 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004f9d2a */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004f9d2c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004f9d2e */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd729u+f->r[2]*4u),1u))&0xffu)<<0);
    /* 004f9d35 */
    f->r[6]=(0x00bcd729u+f->r[2]*4u);
    /* 004f9d3c */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x006fdc88u+f->r[0]),1u))&0xffu)<<0);
    /* 004f9d42 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004f9d44 */
    if((f->flags&MI_ZF))goto mi_004fa309;
    /* 004f9d4a */
    f->r[2]=mi_read((0x005f78f0u+f->r[0]*4u),4u);
    /* 004f9d51 */
    f->r[1]=(0x00000028u+f->r[0]);
    /* 004f9d54 */
    f->r[0]=mi_read((0x00eadd84u),4u);
    /* 004f9d59 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9d5a */
    f->r[1]=mi_read((0x00eadd8cu),4u);
    /* 004f9d60 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9d62 */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004f9d65 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9d66 */
    f->r[2]=(0xffffffd8u+f->r[0]+f->r[1]*1u);
    /* 004f9d6a */
    f->r[0]=mi_read((0x00eadd90u),4u);
    /* 004f9d6f */
    a=f->r[0];b=0x00000028u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f9d72 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9d73 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9d74 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f9d79u;f->boundary(0x004f9d74u,0x00425bc0u,f);
    /* 004f9d79 */
    f->r[0]=mi_read((0x00eadd84u),4u);
    /* 004f9d7e */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004f9d80 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00000000u+f->r[6]),1u))&0xffu)<<0);
    /* 004f9d82 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9d84 */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004f9d87 */
    f->r[2]=mi_read((0x005f78f0u+f->r[1]*4u),4u);
    /* 004f9d8e */
    f->r[1]=mi_read((0x00eadd8cu),4u);
    /* 004f9d94 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9d95 */
    f->r[2]=(0xffffffd8u+f->r[0]+f->r[1]*1u);
    /* 004f9d99 */
    f->r[0]=mi_read((0x00eadd90u),4u);
    /* 004f9d9e */
    a=f->r[0];b=0x00000028u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f9da1 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9da2 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9da3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f9da8u;f->boundary(0x004f9da3u,0x00425990u,f);
    /* 004f9da8 */
    a=f->r[4];b=0x00000024u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004f9dab */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004f9dac */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004f9dad */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004f9dae */
    f->r[4]+=4;return;
mi_004f9daf:;
    /* 004f9daf */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 004f9db2 */
    if(!(f->flags&MI_ZF))goto mi_004fa309;
    /* 004f9db8 */
    f->r[0]=mi_read((0x00eadd6cu),4u);
    /* 004f9dbd */
    f->r[1]=f->r[0];
    /* 004f9dbf */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004f9dc2 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004f9dc4 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((mi_read((0x007999e8u+f->r[1]*8u),2u))&0xffffu)<<0);
    /* 004f9dcc */
    a=(f->r[1]&0xffffu);b=0x0000000au;t=mi_alu(f,a,b,16,1);
    /* 004f9dd0 */
    if(mi_signed_less(f->flags))goto mi_004f9e79;
    /* 004f9dd6 */
    a=(f->r[1]&0xffffu);b=0x0000001du;t=mi_alu(f,a,b,16,1);
    /* 004f9dda */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004f9e79;
    /* 004f9de0 */
    f->r[2]=mi_read((0x00eadd84u),4u);
    /* 004f9de6 */
    f->r[0]=mi_read((0x00eadd8cu),4u);
    /* 004f9deb */
    f->r[1]=mi_read((0x00eadd90u),4u);
    /* 004f9df1 */
    a=f->r[2];b=0xfffffffeu;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004f9df4 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9df5 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9df6 */
    a=f->r[2];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004f9df9 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9dfb */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004f9dfd */
    a=0x005c5a78u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9e02 */
    a=f->r[1];b=0x00000070u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004f9e05 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9e06 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9e07 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f9e0cu;f->boundary(0x004f9e07u,0x00426450u,f);
    /* 004f9e0c */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004f9e0f */
    f->r[1]=mi_read((0x00eadd8cu),4u);
    /* 004f9e15 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9e16 */
    f->r[0]=mi_read((0x00eadd84u),4u);
    /* 004f9e1b */
    a=f->r[0];b=0xfffffffeu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f9e1e */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9e20 */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004f9e23 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f9e25 */
    f->r[1]=mi_read((0x00eadd90u),4u);
    /* 004f9e2b */
    a=0x005c5a78u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9e30 */
    a=f->r[1];b=0x00000070u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004f9e33 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9e34 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9e35 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f9e3au;f->boundary(0x004f9e35u,0x004d6740u,f);
    /* 004f9e3a */
    f->r[2]=mi_read((0x00eadd84u),4u);
    /* 004f9e40 */
    f->r[5]=mi_read((0x00eadd8cu),4u);
    /* 004f9e46 */
    a=f->r[2];b=0xfffffffeu;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004f9e49 */
    MI_U32((0x007056c4u))=(uint32_t)(f->r[0]);
    /* 004f9e4e */
    f->r[0]=mi_read((0x00eadd90u),4u);
    /* 004f9e53 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9e55 */
    a=f->r[2];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004f9e58 */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004f9e5a */
    a=0x005c5a78u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9e5f */
    a=f->r[0];b=0x00000070u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f9e62 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9e63 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9e64 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f9e69u;f->boundary(0x004f9e64u,0x004261d0u,f);
    /* 004f9e69 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004f9e6c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004f9e6e */
    if(!(f->flags&MI_ZF))goto mi_004fa309;
    /* 004f9e74 */
    f->r[0]=mi_read((0x00eadd6cu),4u);
mi_004f9e79:;
    /* 004f9e79 */
    f->r[1]=f->r[0];
    /* 004f9e7b */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004f9e7e */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004f9e80 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((mi_read((0x007999e8u+f->r[1]*8u),2u))&0xffffu)<<0);
    /* 004f9e88 */
    a=(f->r[1]&0xffffu);b=0x00000047u;t=mi_alu(f,a,b,16,1);
    /* 004f9e8c */
    if((f->flags&MI_ZF))goto mi_004f9e99;
    /* 004f9e8e */
    a=(f->r[1]&0xffffu);b=0x000000bfu;t=mi_alu(f,a,b,16,1);
    /* 004f9e93 */
    if(!(f->flags&MI_ZF))goto mi_004f9fc2;
mi_004f9e99:;
    /* 004f9e99 */
    f->r[0]=mi_read((0x00eadcc0u),4u);
    /* 004f9e9e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004f9ea0 */
    if((f->flags&MI_ZF))goto mi_004f9f2b;
    /* 004f9ea6 */
    f->r[2]=mi_read((0x00f53dd8u),4u);
    /* 004f9eac */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9eae */
    f->r[0]=(0x00000000u+f->r[2]*4u);
    /* 004f9eb5 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9eb6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f9ebbu;f->boundary(0x004f9eb6u,0x004cde80u,f);
    /* 004f9ebb */
    f->r[1]=mi_read((0x00eadd8cu),4u);
    /* 004f9ec1 */
    a=f->r[0];b=0x00000007u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f9ec4 */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004f9ec7 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004f9eca */
    f->r[6]=(0xfffffff0u+f->r[0]);
    /* 004f9ecd */
    f->r[0]=mi_read((0x00eadd90u),4u);
    /* 004f9ed2 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9ed3 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9ed4 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9ed6 */
    a=0x005c5ba8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9edb */
    a=f->r[0];b=0x00000050u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f9ede */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9edf */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9ee0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f9ee5u;f->boundary(0x004f9ee0u,0x00426450u,f);
    /* 004f9ee5 */
    f->r[2]=mi_read((0x00eadd90u),4u);
    /* 004f9eeb */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004f9eee */
    a=f->r[2];b=0x00000050u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004f9ef1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9ef2 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9ef4 */
    a=0x005c5ba8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9ef9 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9efa */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9efb */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f9f00u;f->boundary(0x004f9efbu,0x004d6740u,f);
    /* 004f9f00 */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004f9f05 */
    f->r[0]=mi_read((0x00eadd90u),4u);
    /* 004f9f0a */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9f0c */
    a=0x005c5ba8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9f11 */
    a=f->r[0];b=0x00000050u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f9f14 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9f15 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9f16 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f9f1bu;f->boundary(0x004f9f16u,0x004261d0u,f);
    /* 004f9f1b */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004f9f1e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004f9f20 */
    if(!(f->flags&MI_ZF))goto mi_004fa309;
    /* 004f9f26 */
    goto mi_004f9fbd;
mi_004f9f2b:;
    /* 004f9f2b */
    f->r[2]=mi_read((0x00eadd84u),4u);
    /* 004f9f31 */
    f->r[0]=mi_read((0x00eadd8cu),4u);
    /* 004f9f36 */
    f->r[1]=mi_read((0x00eadd90u),4u);
    /* 004f9f3c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9f3d */
    a=f->r[2];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004f9f40 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9f41 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9f43 */
    f->r[0]=(0xffffffdeu+f->r[2]+f->r[0]*1u);
    /* 004f9f47 */
    a=0x005c5a98u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9f4c */
    a=f->r[1];b=0x00000064u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004f9f4f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9f50 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9f51 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f9f56u;f->boundary(0x004f9f51u,0x00426450u,f);
    /* 004f9f56 */
    f->r[1]=mi_read((0x00eadd84u),4u);
    /* 004f9f5c */
    f->r[2]=mi_read((0x00eadd8cu),4u);
    /* 004f9f62 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004f9f65 */
    a=f->r[1];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004f9f68 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9f69 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9f6b */
    f->r[0]=(0xffffffdeu+f->r[1]+f->r[2]*1u);
    /* 004f9f6f */
    f->r[1]=mi_read((0x00eadd90u),4u);
    /* 004f9f75 */
    a=0x005c5a98u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9f7a */
    a=f->r[1];b=0x00000064u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004f9f7d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9f7e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9f7f */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f9f84u;f->boundary(0x004f9f7fu,0x004d6740u,f);
    /* 004f9f84 */
    f->r[2]=mi_read((0x00eadd84u),4u);
    /* 004f9f8a */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004f9f8f */
    f->r[0]=mi_read((0x00eadd8cu),4u);
    /* 004f9f94 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9f96 */
    a=f->r[2];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004f9f99 */
    a=0x005c5a98u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9f9e */
    f->r[1]=(0xffffffdeu+f->r[2]+f->r[0]*1u);
    /* 004f9fa2 */
    f->r[2]=mi_read((0x00eadd90u),4u);
    /* 004f9fa8 */
    a=f->r[2];b=0x00000064u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004f9fab */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9fac */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f9fad */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f9fb2u;f->boundary(0x004f9fadu,0x004261d0u,f);
    /* 004f9fb2 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004f9fb5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004f9fb7 */
    if(!(f->flags&MI_ZF))goto mi_004fa309;
mi_004f9fbd:;
    /* 004f9fbd */
    f->r[0]=mi_read((0x00eadd6cu),4u);
mi_004f9fc2:;
    /* 004f9fc2 */
    f->r[1]=f->r[0];
    /* 004f9fc4 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004f9fc7 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004f9fc9 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x007999e8u+f->r[1]*8u),2u))&0xffffu)<<0);
    /* 004f9fd1 */
    a=(f->r[0]&0xffffu);b=0x00000046u;t=mi_alu(f,a,b,16,1);
    /* 004f9fd5 */
    if((f->flags&MI_ZF))goto mi_004fa215;
    /* 004f9fdb */
    a=(f->r[0]&0xffffu);b=0x000000beu;t=mi_alu(f,a,b,16,1);
    /* 004f9fdf */
    if((f->flags&MI_ZF))goto mi_004fa215;
    /* 004f9fe5 */
    a=(f->r[0]&0xffffu);b=0x00000048u;t=mi_alu(f,a,b,16,1);
    /* 004f9fe9 */
    if((f->flags&MI_ZF))goto mi_004fa0dd;
    /* 004f9fef */
    a=(f->r[0]&0xffffu);b=0x000000c1u;t=mi_alu(f,a,b,16,1);
    /* 004f9ff3 */
    if((f->flags&MI_ZF))goto mi_004fa0dd;
    /* 004f9ff9 */
    a=(f->r[0]&0xffffu);b=0x000000d1u;t=mi_alu(f,a,b,16,1);
    /* 004f9ffd */
    if(!(f->flags&MI_ZF))goto mi_004fa06a;
    /* 004f9fff */
    f->r[0]=mi_read((0x00eadd8cu),4u);
    /* 004fa004 */
    f->r[1]=mi_read((0x00eadd90u),4u);
    /* 004fa00a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa00b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa00c */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa00e */
    a=0x005c5188u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa013 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa014 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa015 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004fa01au;f->boundary(0x004fa015u,0x00426450u,f);
    /* 004fa01a */
    f->r[2]=mi_read((0x00eadd8cu),4u);
    /* 004fa020 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004fa023 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa024 */
    f->r[0]=mi_read((0x00eadd90u),4u);
    /* 004fa029 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa02b */
    a=0x005c5188u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa030 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa031 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa032 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004fa037u;f->boundary(0x004fa032u,0x004d6740u,f);
    /* 004fa037 */
    f->r[1]=mi_read((0x00eadd8cu),4u);
    /* 004fa03d */
    f->r[2]=mi_read((0x00eadd90u),4u);
    /* 004fa043 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa045 */
    a=0x005c5188u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa04a */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa04b */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa04c */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004fa051 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004fa056u;f->boundary(0x004fa051u,0x004261d0u,f);
    /* 004fa056 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004fa059 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004fa05b */
    if(!(f->flags&MI_ZF))goto mi_004fa309;
    /* 004fa061 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004fa066u;f->boundary(0x004fa061u,0x00516020u,f);
    /* 004fa066 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004fa067 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004fa068 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004fa069 */
    f->r[4]+=4;return;
mi_004fa06a:;
    /* 004fa06a */
    a=(f->r[0]&0xffffu);b=0x0000004du;t=mi_alu(f,a,b,16,1);
    /* 004fa06e */
    if((f->flags&MI_ZF))goto mi_004fa080;
    /* 004fa070 */
    a=(f->r[0]&0xffffu);b=0x0000004eu;t=mi_alu(f,a,b,16,1);
    /* 004fa074 */
    if((f->flags&MI_ZF))goto mi_004fa080;
    /* 004fa076 */
    a=(f->r[0]&0xffffu);b=0x0000004fu;t=mi_alu(f,a,b,16,1);
    /* 004fa07a */
    if(!(f->flags&MI_ZF))goto mi_004fa309;
mi_004fa080:;
    /* 004fa080 */
    f->r[0]=mi_read((0x00eadd8cu),4u);
    /* 004fa085 */
    f->r[1]=mi_read((0x00eadd90u),4u);
    /* 004fa08b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa08c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa08d */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa08f */
    a=0x005c5448u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa094 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa095 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa096 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004fa09bu;f->boundary(0x004fa096u,0x00426450u,f);
    /* 004fa09b */
    f->r[1]=mi_read((0x00eadd90u),4u);
    /* 004fa0a1 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004fa0a4 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa0a5 */
    f->r[0]=mi_read((0x00eadd8cu),4u);
    /* 004fa0aa */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa0ac */
    a=0x005c5448u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa0b1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa0b2 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa0b3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004fa0b8u;f->boundary(0x004fa0b3u,0x004d6740u,f);
    /* 004fa0b8 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa0ba */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004fa0bf */
    a=0x005c5448u;f->r[4]-=4;MI_U32(f->r[4])=a;
mi_004fa0c4:;
    /* 004fa0c4 */
    f->r[2]=mi_read((0x00eadd8cu),4u);
    /* 004fa0ca */
    f->r[0]=mi_read((0x00eadd90u),4u);
    /* 004fa0cf */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa0d0 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa0d1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004fa0d6u;f->boundary(0x004fa0d1u,0x004261d0u,f);
    /* 004fa0d6 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004fa0d9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004fa0da */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004fa0db */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004fa0dc */
    f->r[4]+=4;return;
mi_004fa0dd:;
    /* 004fa0dd */
    f->r[0]=mi_read((0x00eadcc0u),4u);
    /* 004fa0e2 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004fa0e4 */
    if((f->flags&MI_ZF))goto mi_004fa187;
    /* 004fa0ea */
    f->r[4]-=4;MI_U32(f->r[4])=0x004fa0efu;f->boundary(0x004fa0eau,0x00502780u,f);
    /* 004fa0ef */
    f->r[1]=mi_read((0x00f53e6cu),4u);
    /* 004fa0f5 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004fa0f7 */
    if(!mi_signed_less(f->flags))goto mi_004fa100;
    /* 004fa0f9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004fa0feu;f->boundary(0x004fa0f9u,0x00502780u,f);
    /* 004fa0fe */
    goto mi_004fa102;
mi_004fa100:;
    /* 004fa100 */
    f->r[0]=f->r[1];
mi_004fa102:;
    /* 004fa102 */
    f->r[1]=(0x00000000u+f->r[0]*4u);
    /* 004fa109 */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa10b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa10c */
    f->r[4]-=4;MI_U32(f->r[4])=0x004fa111u;f->boundary(0x004fa10cu,0x004cde80u,f);
    /* 004fa111 */
    f->r[6]=f->r[0];
    /* 004fa113 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004fa116 */
    a=f->r[6];b=0x00000007u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004fa119 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004fa11eu;f->boundary(0x004fa119u,0x0040d1b0u,f);
    /* 004fa11e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004fa120 */
    if(!(f->flags&MI_ZF))goto mi_004fa309;
    /* 004fa126 */
    f->r[2]=mi_read((0x00eadd8cu),4u);
    /* 004fa12c */
    f->r[0]=mi_read((0x00eadd90u),4u);
    /* 004fa131 */
    a=f->r[6];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004fa134 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa135 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa136 */
    a=f->r[6];b=0xffffffe8u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004fa139 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa13b */
    a=0x005c5bc8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa140 */
    a=f->r[0];b=0x00000050u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004fa143 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa144 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa145 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004fa14au;f->boundary(0x004fa145u,0x00426450u,f);
    /* 004fa14a */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004fa14d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa14e */
    f->r[0]=mi_read((0x00eadd90u),4u);
    /* 004fa153 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa155 */
    a=0x005c5bc8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa15a */
    a=f->r[0];b=0x00000050u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004fa15d */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa15e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa15f */
    f->r[4]-=4;MI_U32(f->r[4])=0x004fa164u;f->boundary(0x004fa15fu,0x004d6740u,f);
    /* 004fa164 */
    f->r[1]=mi_read((0x00eadd90u),4u);
    /* 004fa16a */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa16c */
    a=0x005c5bc8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa171 */
    a=f->r[1];b=0x00000050u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004fa174 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa175 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa176 */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004fa17b */
    f->r[4]-=4;MI_U32(f->r[4])=0x004fa180u;f->boundary(0x004fa17bu,0x004261d0u,f);
    /* 004fa180 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004fa183 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004fa184 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004fa185 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004fa186 */
    f->r[4]+=4;return;
mi_004fa187:;
    /* 004fa187 */
    f->r[2]=mi_read((0x00eadd84u),4u);
    /* 004fa18d */
    f->r[0]=mi_read((0x00eadd8cu),4u);
    /* 004fa192 */
    f->r[1]=mi_read((0x00eadd90u),4u);
    /* 004fa198 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa199 */
    a=f->r[2];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004fa19c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa19d */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa19f */
    f->r[0]=(0xffffffdeu+f->r[2]+f->r[0]*1u);
    /* 004fa1a3 */
    a=0x005c5ad8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa1a8 */
    a=f->r[1];b=0x00000064u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004fa1ab */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa1ac */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa1ad */
    f->r[4]-=4;MI_U32(f->r[4])=0x004fa1b2u;f->boundary(0x004fa1adu,0x00426450u,f);
    /* 004fa1b2 */
    f->r[1]=mi_read((0x00eadd84u),4u);
    /* 004fa1b8 */
    f->r[2]=mi_read((0x00eadd8cu),4u);
    /* 004fa1be */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004fa1c1 */
    a=f->r[1];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004fa1c4 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa1c5 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa1c7 */
    f->r[0]=(0xffffffdeu+f->r[1]+f->r[2]*1u);
    /* 004fa1cb */
    f->r[1]=mi_read((0x00eadd90u),4u);
    /* 004fa1d1 */
    a=0x005c5ad8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa1d6 */
    a=f->r[1];b=0x00000064u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004fa1d9 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa1da */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa1db */
    f->r[4]-=4;MI_U32(f->r[4])=0x004fa1e0u;f->boundary(0x004fa1dbu,0x004d6740u,f);
    /* 004fa1e0 */
    f->r[2]=mi_read((0x00eadd84u),4u);
    /* 004fa1e6 */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004fa1eb */
    f->r[0]=mi_read((0x00eadd8cu),4u);
    /* 004fa1f0 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa1f2 */
    a=f->r[2];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004fa1f5 */
    a=0x005c5ad8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa1fa */
    f->r[1]=(0xffffffdeu+f->r[2]+f->r[0]*1u);
    /* 004fa1fe */
    f->r[2]=mi_read((0x00eadd90u),4u);
    /* 004fa204 */
    a=f->r[2];b=0x00000064u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004fa207 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa208 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa209 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004fa20eu;f->boundary(0x004fa209u,0x004261d0u,f);
    /* 004fa20e */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004fa211 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004fa212 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004fa213 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004fa214 */
    f->r[4]+=4;return;
mi_004fa215:;
    /* 004fa215 */
    f->r[0]=mi_read((0x00eadcc0u),4u);
    /* 004fa21a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004fa21c */
    f->r[0]=mi_read((0x00eadd8cu),4u);
    /* 004fa221 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa222 */
    if((f->flags&MI_ZF))goto mi_004fa285;
    /* 004fa224 */
    f->r[0]=mi_read((0x00eadd90u),4u);
    /* 004fa229 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa22a */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa22c */
    a=0x005c5ba8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa231 */
    a=f->r[0];b=0x00000050u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004fa234 */
    a=0x000001b4u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa239 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa23a */
    f->r[4]-=4;MI_U32(f->r[4])=0x004fa23fu;f->boundary(0x004fa23au,0x00426450u,f);
    /* 004fa23f */
    f->r[1]=mi_read((0x00eadd90u),4u);
    /* 004fa245 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004fa248 */
    a=f->r[1];b=0x00000050u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004fa24b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa24c */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa24e */
    a=0x005c5ba8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa253 */
    a=0x000001b4u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa258 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa259 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004fa25eu;f->boundary(0x004fa259u,0x004d6740u,f);
    /* 004fa25e */
    f->r[2]=mi_read((0x00eadd90u),4u);
    /* 004fa264 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa266 */
    a=0x005c5ba8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa26b */
    a=f->r[2];b=0x00000050u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004fa26e */
    a=0x000001b4u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa273 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa274 */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004fa279 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004fa27eu;f->boundary(0x004fa279u,0x004261d0u,f);
    /* 004fa27e */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004fa281 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004fa282 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004fa283 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004fa284 */
    f->r[4]+=4;return;
mi_004fa285:;
    /* 004fa285 */
    f->r[2]=mi_read((0x00eadd84u),4u);
    /* 004fa28b */
    f->r[1]=mi_read((0x00eadd90u),4u);
    /* 004fa291 */
    a=f->r[2];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004fa294 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa295 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa297 */
    f->r[0]=(0xffffffdeu+f->r[2]+f->r[0]*1u);
    /* 004fa29b */
    a=0x005c5ab8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa2a0 */
    a=f->r[1];b=0x00000064u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004fa2a3 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa2a4 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa2a5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004fa2aau;f->boundary(0x004fa2a5u,0x00426450u,f);
    /* 004fa2aa */
    f->r[1]=mi_read((0x00eadd84u),4u);
    /* 004fa2b0 */
    f->r[2]=mi_read((0x00eadd8cu),4u);
    /* 004fa2b6 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004fa2b9 */
    a=f->r[1];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004fa2bc */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa2bd */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa2bf */
    f->r[0]=(0xffffffdeu+f->r[1]+f->r[2]*1u);
    /* 004fa2c3 */
    f->r[1]=mi_read((0x00eadd90u),4u);
    /* 004fa2c9 */
    a=0x005c5ab8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa2ce */
    a=f->r[1];b=0x00000064u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004fa2d1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa2d2 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa2d3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004fa2d8u;f->boundary(0x004fa2d3u,0x004d6740u,f);
    /* 004fa2d8 */
    f->r[2]=mi_read((0x00eadd84u),4u);
    /* 004fa2de */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004fa2e3 */
    f->r[0]=mi_read((0x00eadd8cu),4u);
    /* 004fa2e8 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa2ea */
    a=f->r[2];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004fa2ed */
    a=0x005c5ab8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa2f2 */
    f->r[1]=(0xffffffdeu+f->r[2]+f->r[0]*1u);
    /* 004fa2f6 */
    f->r[2]=mi_read((0x00eadd90u),4u);
    /* 004fa2fc */
    a=f->r[2];b=0x00000064u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004fa2ff */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa300 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa301 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004fa306u;f->boundary(0x004fa301u,0x004261d0u,f);
    /* 004fa306 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004fa309:;
    /* 004fa309 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004fa30a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004fa30b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004fa30c */
    f->r[4]+=4;return;
}
