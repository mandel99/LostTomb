/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "construction_controller.h"
#include <stdlib.h>
#include "construction_tiles.h"
#include "bridge_place.h"
#include "displaced_population.h"
#include "construction_lines.h"
#include "construction_refresh.h"
#include "construction_brush.h"
#include "building_reservation.h"
extern void FUN_0040f860(void *,void *,int32_t);
extern void FUN_004264d0(uint32_t,uint32_t,uint32_t);
extern uint32_t FUN_00474380(int32_t);
extern void FUN_00475720(int32_t,int32_t,int32_t,int32_t,uint32_t);
extern int32_t FUN_00475b50(int32_t);
extern void FUN_00475cc0(int32_t,int32_t,int32_t,int32_t,uint32_t);
extern void FUN_00476530(void);
extern void FUN_004767d0(int32_t,int32_t,int32_t,int32_t,uint32_t,uint32_t);
extern void FUN_00477500(int32_t,int32_t,int32_t,int32_t);
extern void FUN_004776f0(int32_t,int32_t,int32_t,int32_t);
extern void FUN_004777c0(int32_t,int32_t,int32_t,int32_t);
extern void FUN_00477890(int32_t,int32_t,int32_t,int32_t);
extern void FUN_0047b590(int32_t,int32_t,int32_t,int32_t);
extern void FUN_0047bae0(int32_t,int32_t,int32_t,int32_t,uint32_t);
extern void FUN_0047bd50(int32_t,int32_t,int32_t,int32_t);
extern uint32_t FUN_00485100(int32_t);
extern void FUN_00485830(void);
extern void FUN_004e2970(int32_t);
extern void FUN_00517370(void);
extern void FUN_00517660(void);
extern void FUN_005178b0(void);
extern void FUN_005178e0(void);
extern void FUN_0053b090(int32_t);

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
static __inline uint32_t mi_alu(struct restore_construction_frame *f,uint32_t a,uint32_t b,uint32_t width,uint32_t operation)
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
static __inline uint32_t mi_shift(struct restore_construction_frame *f,uint32_t a,uint32_t count,uint32_t width,uint32_t arithmetic)
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
static __inline uint32_t mi_imul(struct restore_construction_frame *f,uint32_t a,uint32_t b)
{
 int64_t product=(int64_t)(int32_t)a*(int32_t)b;uint32_t result=(uint32_t)product;
 f->flags&=~(MI_CF|MI_OF);if(product!=(int64_t)(int32_t)result)f->flags|=MI_CF|MI_OF;
 return result;
}
static __inline int mi_signed_less(uint32_t flags){return ((flags&MI_SF)!=0)^((flags&MI_OF)!=0);}

void restore_construction_C_00474700(struct restore_construction_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00474700 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 00474703 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474704 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474705 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474706 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 00474708 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474709 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 0047470d */
    MI_U32((0x00bc971cu))=(uint32_t)(f->r[5]);
    /* 00474713 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00474718u;f->boundary(0x00474713u,0x00485080u,f);
    /* 00474718 */
    a=0x00032c40u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047471d */
    a=0x00af598cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474722 */
    a=0x009de62cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474727 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047472cu;f->boundary(0x00474727u,0x0040f860u,f);
    /* 0047472c */
    a=0x0000cb10u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474731 */
    a=0x00ae8e7cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474736 */
    a=0x009d1b1cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047473b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00474740u;f->boundary(0x0047473bu,0x0040f860u,f);
    /* 00474740 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474741 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00474746u;f->boundary(0x00474741u,0x00485830u,f);
    /* 00474746 */
    f->r[0]=MI_U32((0x00000044u+f->r[4]));
    /* 0047474a */
    f->r[1]=MI_U32((0x0000004cu+f->r[4]));
    /* 0047474e */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00474751 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 00474753 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00474763;
    /* 00474755 */
    f->r[2]=f->r[0];
    /* 00474757 */
    f->r[3]=f->r[1];
    /* 00474759 */
    MI_U32((0x00000030u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0047475d */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 00474761 */
    goto mi_0047476f;
mi_00474763:;
    /* 00474763 */
    f->r[3]=f->r[0];
    /* 00474765 */
    MI_U32((0x00000030u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00474769 */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 0047476d */
    f->r[2]=f->r[1];
mi_0047476f:;
    /* 0047476f */
    f->r[0]=MI_U32((0x0000002cu+f->r[4]));
    /* 00474773 */
    f->r[1]=MI_U32((0x00000034u+f->r[4]));
    /* 00474777 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 00474779 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00474785;
    /* 0047477b */
    f->r[6]=f->r[0];
    /* 0047477d */
    f->r[7]=f->r[1];
    /* 0047477f */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 00474783 */
    goto mi_0047478d;
mi_00474785:;
    /* 00474785 */
    f->r[6]=f->r[1];
    /* 00474787 */
    f->r[7]=f->r[0];
    /* 00474789 */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(f->r[6]);
mi_0047478d:;
    /* 0047478d */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 0047478f */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[7]);
    /* 00474793 */
    if(!mi_signed_less(f->flags))goto mi_0047479b;
    /* 00474795 */
    MI_U32((0x00000030u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 00474799 */
    f->r[2]=f->r[5];
mi_0047479b:;
    /* 0047479b */
    f->r[0]=MI_U32((0x00bca2d0u));
    /* 004747a0 */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004747a2 */
    if(mi_signed_less(f->flags))goto mi_004747ab;
    /* 004747a4 */
    f->r[3]=(0xffffffffu+f->r[0]);
    /* 004747a7 */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[3]);
mi_004747ab:;
    /* 004747ab */
    a=f->r[6];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004747ad */
    if(!mi_signed_less(f->flags))goto mi_004747b5;
    /* 004747af */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 004747b1 */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(f->r[6]);
mi_004747b5:;
    /* 004747b5 */
    f->r[0]=MI_U32((0x00bca2d4u));
    /* 004747ba */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004747bc */
    if(mi_signed_less(f->flags))goto mi_004747c5;
    /* 004747be */
    f->r[7]=(0xffffffffu+f->r[0]);
    /* 004747c1 */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[7]);
mi_004747c5:;
    /* 004747c5 */
    f->r[0]=(0x00000000u+f->r[6]+f->r[6]*8u);
    /* 004747c8 */
    f->r[1]=f->r[3];
    /* 004747ca */
    f->r[3]=MI_U32((0x00bca2d8u));
    /* 004747d0 */
    f->r[5]=f->r[7];
    /* 004747d2 */
    f->r[0]=(0x00000000u+f->r[6]+f->r[0]*2u);
    /* 004747d5 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004747d7 */
    a=f->r[5];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[5]=t;
    /* 004747d9 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004747da */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004747dd */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 004747de */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004747e2 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 004747e6 */
    f->r[0]=(0x00000000u+f->r[3]+f->r[0]*4u);
    /* 004747e9 */
    f->r[3]=MI_U32((0x00000024u+f->r[4]));
    /* 004747ed */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004747ef */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004747f1 */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[0]);
    /* 004747f6 */
    MI_U32((0x00bc9760u))=(uint32_t)(f->r[6]);
    /* 004747fc */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0047498c;
    /* 00474802 */
    goto mi_00474808;
mi_00474804:;
    /* 00474804 */
    f->r[2]=MI_U32((0x00000030u+f->r[4]));
mi_00474808:;
    /* 00474808 */
    MI_U32((0x00bc9764u))=(uint32_t)(f->r[2]);
    /* 0047480e */
    f->r[2]=MI_U32((0x00000028u+f->r[4]));
    /* 00474812 */
    a=MI_U32((0x00000030u+f->r[4]));b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 00474816 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00474963;
mi_0047481c:;
    /* 0047481c */
    f->r[1]=MI_U32((0x00af598cu+f->r[0]*4u));
    /* 00474823 */
    f->r[6]=f->r[1];
    /* 00474825 */
    a=f->r[6];b=0xeefed77fu;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 0047482b */
    a=f->r[6];b=0x02040202u;t=mi_alu(f,a,b,32,3);
    /* 00474831 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 00474835 */
    if(!(f->flags&MI_ZF))goto mi_00474934;
    /* 0047483b */
    f->r[5]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00b4e6fcu+f->r[0]*2u));
    /* 00474843 */
    f->r[2]=f->r[6];
    /* 00474845 */
    a=(f->r[2]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,3);
    /* 00474848 */
    if(!(f->flags&MI_ZF))goto mi_00474a1c;
    /* 0047484e */
    a=f->r[2];b=0x40000000u;t=mi_alu(f,a,b,32,3);
    /* 00474854 */
    if(!(f->flags&MI_ZF))goto mi_00474a1c;
    /* 0047485a */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 0047485c */
    if((f->flags&MI_ZF))goto mi_0047488b;
    /* 0047485e */
    a=f->r[1];b=0x10000000u;t=mi_alu(f,a,b,32,3);
    /* 00474864 */
    if((f->flags&MI_ZF))goto mi_0047488b;
    /* 00474866 */
    f->r[2]=f->r[5];
    /* 00474868 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0047486b */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0047486d */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x007999e8u+f->r[2]*8u)))&0xffffu)<<0);
    /* 00474875 */
    a=(f->r[2]&0xffffu);b=0x000000e1u;t=mi_alu(f,a,b,16,1);
    /* 0047487a */
    if((f->flags&MI_ZF))goto mi_00474a1c;
    /* 00474880 */
    a=(f->r[2]&0xffffu);b=0x000000d5u;t=mi_alu(f,a,b,16,1);
    /* 00474885 */
    if((f->flags&MI_ZF))goto mi_00474a1c;
mi_0047488b:;
    /* 0047488b */
    f->r[2]=MI_U32((0x00000010u+f->r[4]));
    /* 0047488f */
    a=((f->r[2]>>8)&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,3);
    /* 00474892 */
    if((f->flags&MI_ZF))goto mi_00474900;
    /* 00474894 */
    f->r[2]=MI_U32((0x00bc971cu));
    /* 0047489a */
    a=f->r[1];b=0x03052e80u;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 004748a0 */
    MI_U32((0x00af598cu+f->r[0]*4u))=(uint32_t)(f->r[1]);
    /* 004748a7 */
    MI_U8((0x00ae8e7cu+f->r[0]))=(uint8_t)(0x00000000u);
    /* 004748ae */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00ae8d98u+f->r[0])))&0xffu)<<0);
    /* 004748b4 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004748b5 */
    a=(f->r[1]&0xffu);b=0x00000005u;t=mi_alu(f,a,b,8,1);
    /* 004748b8 */
    MI_U32((0x00bc971cu))=(uint32_t)(f->r[2]);
    /* 004748be */
    if(!(f->flags&MI_ZF))goto mi_004748c7;
    /* 004748c0 */
    MI_U8((0x00ae8d98u+f->r[0]))=(uint8_t)(0x00000001u);
mi_004748c7:;
    /* 004748c7 */
    a=MI_U8((0x00ae8e7du+f->r[0]));b=0x00000006u;t=mi_alu(f,a,b,8,1);
    /* 004748ce */
    if(!(f->flags&MI_ZF))goto mi_004748d7;
    /* 004748d0 */
    MI_U8((0x00ae8e7du+f->r[0]))=(uint8_t)(0x00000002u);
mi_004748d7:;
    /* 004748d7 */
    a=MI_U8((0x00ae8f60u+f->r[0]));b=0x00000005u;t=mi_alu(f,a,b,8,1);
    /* 004748de */
    if(!(f->flags&MI_ZF))goto mi_004748e7;
    /* 004748e0 */
    MI_U8((0x00ae8f60u+f->r[0]))=(uint8_t)(0x00000003u);
mi_004748e7:;
    /* 004748e7 */
    a=MI_U8((0x00ae8e7bu+f->r[0]));b=0x00000006u;t=mi_alu(f,a,b,8,1);
    /* 004748ee */
    if(!(f->flags&MI_ZF))goto mi_00474ce7;
    /* 004748f4 */
    MI_U8((0x00ae8e7bu+f->r[0]))=(uint8_t)(0x00000004u);
    /* 004748fb */
    goto mi_00474ce7;
mi_00474900:;
    /* 00474900 */
    f->r[2]=MI_U32((0x00000010u+f->r[4]));
    /* 00474904 */
    a=(f->r[2]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,3);
    /* 00474907 */
    if((f->flags&MI_ZF))goto mi_004749c8;
    /* 0047490d */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 0047490f */
    if(!(f->flags&MI_ZF))goto mi_004749a1;
    /* 00474915 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474916 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047491bu;f->boundary(0x00474916u,0x0047f9b0u,f);
    /* 0047491b */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047491e */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 00474921 */
    if(mi_signed_less(f->flags))goto mi_0047499c;
    /* 00474923 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474924 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474925 */
    a=0x00000037u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474927 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047492cu;f->boundary(0x00474927u,0x004e2970u,f);
    /* 0047492c */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0047492f:;
    /* 0047492f */
    f->r[0]=MI_U32((0x00bc9768u));
mi_00474934:;
    /* 00474934 */
    f->r[1]=MI_U32((0x00bc9764u));
    /* 0047493a */
    f->r[2]=MI_U32((0x00000028u+f->r[4]));
    /* 0047493e */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0047493f */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00474940 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 00474942 */
    MI_U32((0x00bc9764u))=(uint32_t)(f->r[1]);
    /* 00474948 */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[0]);
    /* 0047494d */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0047481c;
    /* 00474953 */
    f->r[5]=MI_U32((0x00000018u+f->r[4]));
    /* 00474957 */
    f->r[1]=MI_U32((0x0000001cu+f->r[4]));
    /* 0047495b */
    f->r[6]=MI_U32((0x00000034u+f->r[4]));
    /* 0047495f */
    f->r[7]=MI_U32((0x0000002cu+f->r[4]));
mi_00474963:;
    /* 00474963 */
    f->r[2]=MI_U32((0x00bc9760u));
    /* 00474969 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0047496a */
    MI_U32((0x00bc9760u))=(uint32_t)(f->r[2]);
    /* 00474970 */
    f->r[2]=0x000000e4u;
    /* 00474975 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00474977 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00474979 */
    f->r[2]=MI_U32((0x00bc9760u));
    /* 0047497f */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 00474981 */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[0]);
    /* 00474986 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00474804;
mi_0047498c:;
    /* 0047498c */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 0047498e */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00474d0a;
    /* 00474994 */
    f->r[5]=(0x00000002u+f->r[1]);
    /* 00474997 */
    goto mi_00474d0d;
mi_0047499c:;
    /* 0047499c */
    f->r[0]=MI_U32((0x00bc9768u));
mi_004749a1:;
    /* 004749a1 */
    a=MI_U32((0x00bc96b0u));b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004749a8 */
    if(!(f->flags&MI_ZF))goto mi_00474ce7;
    /* 004749ae */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004749af */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004749b0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004749b5u;f->boundary(0x004749b0u,0x0047f810u,f);
    /* 004749b5 */
    f->r[0]=MI_U32((0x00bc971cu));
    /* 004749ba */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004749bd */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004749be */
    MI_U32((0x00bc971cu))=(uint32_t)(f->r[0]);
    /* 004749c3 */
    goto mi_00474ce2;
mi_004749c8:;
    /* 004749c8 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 004749ca */
    if((f->flags&MI_ZF))goto mi_00474ce7;
    /* 004749d0 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00b67d1cu+f->r[0])))&0xffu)<<0);
    /* 004749d6 */
    a=f->r[1];b=0x03052e80u;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 004749dc */
    MI_U32((0x00af598cu+f->r[0]*4u))=(uint32_t)(f->r[1]);
    /* 004749e3 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00b285ccu+f->r[0])))&0xffu)<<0);
    /* 004749e9 */
    a=(f->r[1]&0xffu);b=0x000000f0u;t=mi_alu(f,a,b,8,3);f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004749ec */
    a=(f->r[2]&0xffu);b=0x00000040u;t=mi_alu(f,a,b,8,4);f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004749ef */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 004749f1 */
    MI_U8((0x00b285ccu+f->r[0]))=(uint8_t)((f->r[1]&0xffu));
    /* 004749f7 */
    MI_U8((0x00b67d1cu+f->r[0]))=(uint8_t)((f->r[2]&0xffu));
    /* 004749fd */
    if(!(f->flags&MI_ZF))goto mi_00474a11;
    /* 004749ff */
    a=(f->r[1]&0xffu);b=0x00000070u;t=mi_alu(f,a,b,8,3);f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00474a02 */
    a=(f->r[2]&0xffu);b=0x00000040u;t=mi_alu(f,a,b,8,4);f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00474a05 */
    MI_U8((0x00b285ccu+f->r[0]))=(uint8_t)((f->r[1]&0xffu));
    /* 00474a0b */
    MI_U8((0x00b67d1cu+f->r[0]))=(uint8_t)((f->r[2]&0xffu));
mi_00474a11:;
    /* 00474a11 */
    a=MI_U32((0x00bc971cu));b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;MI_U32((0x00bc971cu))=(uint32_t)(t);
    /* 00474a17 */
    goto mi_00474ce7;
mi_00474a1c:;
    /* 00474a1c */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 00474a1e */
    if((f->flags&MI_ZF))goto mi_00474ce7;
    /* 00474a24 */
    f->r[6]=f->r[5];
    /* 00474a26 */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 00474a29 */
    a=f->r[6];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00474a2b */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 00474a2e */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x007999e8u+f->r[6])))&0xffffu)<<0);
    /* 00474a35 */
    a=(f->r[1]&0xffffu);b=0x00000063u;t=mi_alu(f,a,b,16,1);
    /* 00474a39 */
    if((f->flags&MI_ZF))goto mi_00474934;
    /* 00474a3f */
    a=(f->r[1]&0xffffu);b=0x00000058u;t=mi_alu(f,a,b,16,1);
    /* 00474a43 */
    if((f->flags&MI_ZF))goto mi_00474934;
    /* 00474a49 */
    a=(f->r[1]&0xffffu);b=0x00000059u;t=mi_alu(f,a,b,16,1);
    /* 00474a4d */
    if((f->flags&MI_ZF))goto mi_00474934;
    /* 00474a53 */
    a=(f->r[1]&0xffffu);b=0x0000005du;t=mi_alu(f,a,b,16,1);
    /* 00474a57 */
    if((f->flags&MI_ZF))goto mi_00474934;
    /* 00474a5d */
    a=(f->r[1]&0xffffu);b=0x000000dau;t=mi_alu(f,a,b,16,1);
    /* 00474a62 */
    if((f->flags&MI_ZF))goto mi_00474934;
    /* 00474a68 */
    a=(f->r[1]&0xffffu);b=0x000000dbu;t=mi_alu(f,a,b,16,1);
    /* 00474a6d */
    if((f->flags&MI_ZF))goto mi_00474934;
    /* 00474a73 */
    a=(f->r[1]&0xffffu);b=0x000000dcu;t=mi_alu(f,a,b,16,1);
    /* 00474a78 */
    if((f->flags&MI_ZF))goto mi_00474934;
    /* 00474a7e */
    a=(f->r[1]&0xffffu);b=0x000000d7u;t=mi_alu(f,a,b,16,1);
    /* 00474a83 */
    if((f->flags&MI_ZF))goto mi_00474934;
    /* 00474a89 */
    a=MI_U8((0x007999d8u+f->r[6]));b=0x00000006u;t=mi_alu(f,a,b,8,1);
    /* 00474a90 */
    if((f->flags&MI_ZF))goto mi_00474934;
    /* 00474a96 */
    a=(f->r[1]&0xffffu);b=0x00000039u;t=mi_alu(f,a,b,16,1);
    /* 00474a9a */
    if((f->flags&MI_ZF))goto mi_00474aa2;
    /* 00474a9c */
    a=(f->r[1]&0xffffu);b=0x00000036u;t=mi_alu(f,a,b,16,1);
    /* 00474aa0 */
    if(!(f->flags&MI_ZF))goto mi_00474ab9;
mi_00474aa2:;
    /* 00474aa2 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 00474aa4 */
    f->r[7]=0x00000001u;
    /* 00474aa9 */
    if(!(f->flags&MI_ZF))goto mi_00474abe;
    /* 00474aab */
    a=MI_U32((0x00bc96b4u));b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 00474ab1 */
    if(!(f->flags&MI_ZF))goto mi_00474934;
    /* 00474ab7 */
    goto mi_00474abe;
mi_00474ab9:;
    /* 00474ab9 */
    f->r[7]=0x00000001u;
mi_00474abe:;
    /* 00474abe */
    f->r[0]=(uint32_t)(int32_t)(int16_t)(f->r[1]&0xffffu);
    /* 00474ac1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474ac2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00474ac7u;f->boundary(0x00474ac2u,0x00474380u,f);
    /* 00474ac7 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00474aca */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00474acc */
    if((f->flags&MI_ZF))goto mi_00474ade;
    /* 00474ace */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 00474ad0 */
    if(!(f->flags&MI_ZF))goto mi_00474ade;
    /* 00474ad2 */
    a=MI_U32((0x00bc96acu));b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 00474ad8 */
    if(!(f->flags&MI_ZF))goto mi_0047492f;
mi_00474ade:;
    /* 00474ade */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x007999e8u+f->r[6])))&0xffffu)<<0);
    /* 00474ae5 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)(f->r[1]&0xffffu);
    /* 00474ae8 */
    a=f->r[0];b=0x00000041u;t=mi_alu(f,a,b,32,1);
    /* 00474aeb */
    if((f->flags&MI_ZF))goto mi_00474b29;
    /* 00474aed */
    a=f->r[0];b=0x00000042u;t=mi_alu(f,a,b,32,1);
    /* 00474af0 */
    if((f->flags&MI_ZF))goto mi_00474b29;
    /* 00474af2 */
    a=f->r[0];b=0x00000043u;t=mi_alu(f,a,b,32,1);
    /* 00474af5 */
    if((f->flags&MI_ZF))goto mi_00474b29;
    /* 00474af7 */
    a=f->r[0];b=0x00000044u;t=mi_alu(f,a,b,32,1);
    /* 00474afa */
    if((f->flags&MI_ZF))goto mi_00474b29;
    /* 00474afc */
    a=f->r[0];b=0x00000045u;t=mi_alu(f,a,b,32,1);
    /* 00474aff */
    if((f->flags&MI_ZF))goto mi_00474b29;
    /* 00474b01 */
    a=f->r[0];b=0x00000061u;t=mi_alu(f,a,b,32,1);
    /* 00474b04 */
    if((f->flags&MI_ZF))goto mi_00474b29;
    /* 00474b06 */
    a=f->r[0];b=0x0000009cu;t=mi_alu(f,a,b,32,1);
    /* 00474b0b */
    if((f->flags&MI_ZF))goto mi_00474b29;
    /* 00474b0d */
    a=f->r[0];b=0x0000009du;t=mi_alu(f,a,b,32,1);
    /* 00474b12 */
    if((f->flags&MI_ZF))goto mi_00474b29;
    /* 00474b14 */
    a=f->r[0];b=0x0000009eu;t=mi_alu(f,a,b,32,1);
    /* 00474b19 */
    if((f->flags&MI_ZF))goto mi_00474b29;
    /* 00474b1b */
    a=f->r[0];b=0x0000009fu;t=mi_alu(f,a,b,32,1);
    /* 00474b20 */
    if((f->flags&MI_ZF))goto mi_00474b29;
    /* 00474b22 */
    a=f->r[0];b=0x000000a0u;t=mi_alu(f,a,b,32,1);
    /* 00474b27 */
    if(!(f->flags&MI_ZF))goto mi_00474b39;
mi_00474b29:;
    /* 00474b29 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 00474b2b */
    if(!(f->flags&MI_ZF))goto mi_00474b39;
    /* 00474b2d */
    a=MI_U32((0x00bc96a8u));b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 00474b33 */
    if(!(f->flags&MI_ZF))goto mi_0047492f;
mi_00474b39:;
    /* 00474b39 */
    a=(f->r[1]&0xffffu);b=0x00000039u;t=mi_alu(f,a,b,16,1);
    /* 00474b3d */
    if((f->flags&MI_ZF))goto mi_00474b45;
    /* 00474b3f */
    a=(f->r[1]&0xffffu);b=0x00000036u;t=mi_alu(f,a,b,16,1);
    /* 00474b43 */
    if(!(f->flags&MI_ZF))goto mi_00474b57;
mi_00474b45:;
    /* 00474b45 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 00474b47 */
    if(!(f->flags&MI_ZF))goto mi_00474b57;
    /* 00474b49 */
    a=MI_U32((0x00bc96b4u));b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 00474b4f */
    if(!(f->flags&MI_ZF))goto mi_00474b57;
    /* 00474b51 */
    MI_U32((0x00bc9694u))=(uint32_t)(f->r[3]);
mi_00474b57:;
    /* 00474b57 */
    a=(f->r[1]&0xffffu);b=0x000000b7u;t=mi_alu(f,a,b,16,1);
    /* 00474b5c */
    if(!(f->flags&MI_ZF))goto mi_00474b6e;
    /* 00474b5e */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474b5f */
    f->r[4]-=4;MI_U32(f->r[4])=0x00474b64u;f->boundary(0x00474b5fu,0x004f4930u,f);
    /* 00474b64 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474b65 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474b66 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00474b6bu;f->boundary(0x00474b66u,0x004f5e70u,f);
    /* 00474b6b */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00474b6e:;
    /* 00474b6e */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00799a99u+f->r[6])))&0xffu)<<0);
    /* 00474b74 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00474b76 */
    if((f->flags&MI_ZF))goto mi_00474b8d;
    /* 00474b78 */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 00474b7d */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 00474b80 */
    a=f->r[1];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00474b83 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00474b85 */
    MI_U8((0x007900c0u+f->r[1]*4u))=(uint8_t)(0x00000000u);
mi_00474b8d:;
    /* 00474b8d */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00799aa3u+f->r[6])))&0xffu)<<0);
    /* 00474b93 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00474b95 */
    if((f->flags&MI_ZF))goto mi_00474ba6;
    /* 00474b97 */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 00474b9c */
    a=f->r[0];b=0x0000001cu;t=mi_imul(f,a,b);f->r[0]=t;
    /* 00474b9f */
    MI_U8((0x009783a4u+f->r[0]))=(uint8_t)(0x00000000u);
mi_00474ba6:;
    /* 00474ba6 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00799aa2u+f->r[6])))&0xffu)<<0);
    /* 00474bac */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00474bae */
    if((f->flags&MI_ZF))goto mi_00474bc5;
    /* 00474bb0 */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 00474bb5 */
    a=f->r[0];b=0x000000e0u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 00474bbb */
    MI_U32((0x00eaaf68u+f->r[0]))=(uint32_t)(0x00000000u);
mi_00474bc5:;
    /* 00474bc5 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x007999ddu+f->r[6])))&0xffu)<<0);
    /* 00474bcb */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00474bcd */
    if((f->flags&MI_ZF))goto mi_00474c09;
    /* 00474bcf */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x007999f4u+f->r[6])))&0xffffu)<<0);
    /* 00474bd6 */
    a=(f->r[0]&0xffffu);b=(f->r[0]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 00474bd9 */
    if((f->flags&MI_ZF))goto mi_00474c09;
    /* 00474bdb */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 00474bdd */
    if(!(f->flags&MI_ZF))goto mi_00474c09;
    /* 00474bdf */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[6]));
    /* 00474be6 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)(f->r[0]&0xffffu);
    /* 00474be9 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[6]));
    /* 00474bf0 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474bf1 */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 00474bf7 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474bf8 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474bf9 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474bfa */
    f->r[4]-=4;MI_U32(f->r[4])=0x00474bffu;f->boundary(0x00474bfau,0x00459020u,f);
    /* 00474bff */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00474c02 */
    MI_U16((0x007999f4u+f->r[6]))=(uint16_t)((f->r[3]&0xffffu));
mi_00474c09:;
    /* 00474c09 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x007999e8u+f->r[6])))&0xffffu)<<0);
    /* 00474c10 */
    a=(f->r[0]&0xffffu);b=0x0000006cu;t=mi_alu(f,a,b,16,1);
    /* 00474c14 */
    if((f->flags&MI_ZF))goto mi_00474c22;
    /* 00474c16 */
    a=(f->r[0]&0xffffu);b=0x000000c3u;t=mi_alu(f,a,b,16,1);
    /* 00474c1a */
    if((f->flags&MI_ZF))goto mi_00474c22;
    /* 00474c1c */
    a=(f->r[0]&0xffffu);b=0x00000073u;t=mi_alu(f,a,b,16,1);
    /* 00474c20 */
    if(!(f->flags&MI_ZF))goto mi_00474c29;
mi_00474c22:;
    /* 00474c22 */
    MI_U8((0x00799a7eu+f->r[6]))=(uint8_t)(0x00000000u);
mi_00474c29:;
    /* 00474c29 */
    a=MI_U8((0x007999d8u+f->r[6]));b=0x00000006u;t=mi_alu(f,a,b,8,1);
    /* 00474c30 */
    if((f->flags&MI_ZF))goto mi_00474c48;
    /* 00474c32 */
    f->r[2]=MI_U32((0x00bc971cu));
    /* 00474c38 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474c39 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00474c3a */
    MI_U32((0x00bc971cu))=(uint32_t)(f->r[2]);
    /* 00474c40 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00474c45u;f->boundary(0x00474c40u,0x00485100u,f);
    /* 00474c45 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00474c48:;
    /* 00474c48 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474c4a */
    MI_U8((0x007999d8u+f->r[6]))=(uint8_t)(0x00000006u);
    /* 00474c51 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00474c56u;f->boundary(0x00474c51u,0x0053b090u,f);
    /* 00474c56 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00474c59 */
    MI_U8((0x00799a97u+f->r[6]))=(uint8_t)(0x00000001u);
    /* 00474c60 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 00474c62 */
    f->r[6]=f->r[5];
mi_00474c64:;
    /* 00474c64 */
    f->r[0]=f->r[6];
    /* 00474c66 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00474c69 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00474c6b */
    f->r[6]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00799a10u+f->r[0]*8u));
    /* 00474c73 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 00474c75 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00474ca1;
    /* 00474c77 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474c78 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00474c7du;f->boundary(0x00474c78u,0x00485100u,f);
    /* 00474c7d */
    f->r[0]=f->r[6];
    /* 00474c7f */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00474c82 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00474c85 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00474c87 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00474c8a */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 00474c8b */
    MI_U8((0x007999d8u+f->r[0]))=(uint8_t)(0x00000006u);
    /* 00474c92 */
    a=f->r[7];b=0x0000020du;t=mi_alu(f,a,b,32,1);
    /* 00474c98 */
    MI_U8((0x00799a97u+f->r[0]))=(uint8_t)(0x00000001u);
    /* 00474c9f */
    if(mi_signed_less(f->flags))goto mi_00474c64;
mi_00474ca1:;
    /* 00474ca1 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 00474ca3 */
    f->r[6]=f->r[5];
mi_00474ca5:;
    /* 00474ca5 */
    f->r[1]=f->r[6];
    /* 00474ca7 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00474caa */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00474cac */
    f->r[6]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00799a12u+f->r[1]*8u));
    /* 00474cb4 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 00474cb6 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00474ce2;
    /* 00474cb8 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474cb9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00474cbeu;f->boundary(0x00474cb9u,0x00485100u,f);
    /* 00474cbe */
    f->r[0]=f->r[6];
    /* 00474cc0 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00474cc3 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00474cc6 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00474cc8 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00474ccb */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 00474ccc */
    MI_U8((0x007999d8u+f->r[0]))=(uint8_t)(0x00000006u);
    /* 00474cd3 */
    a=f->r[7];b=0x0000020du;t=mi_alu(f,a,b,32,1);
    /* 00474cd9 */
    MI_U8((0x00799a97u+f->r[0]))=(uint8_t)(0x00000001u);
    /* 00474ce0 */
    if(mi_signed_less(f->flags))goto mi_00474ca5;
mi_00474ce2:;
    /* 00474ce2 */
    f->r[0]=MI_U32((0x00bc9768u));
mi_00474ce7:;
    /* 00474ce7 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 00474ce9 */
    if(!(f->flags&MI_ZF))goto mi_00474934;
    /* 00474cef */
    a=MI_U32((0x00000010u+f->r[4]));b=0x00000148u;t=mi_alu(f,a,b,32,3);
    /* 00474cf7 */
    if((f->flags&MI_ZF))goto mi_00474934;
    /* 00474cfd */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(0x00000005u);
    /* 00474d05 */
    goto mi_00474934;
mi_00474d0a:;
    /* 00474d0a */
    a=f->r[5];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
mi_00474d0d:;
    /* 00474d0d */
    f->r[3]=MI_U32((0x00000028u+f->r[4]));
    /* 00474d11 */
    f->r[0]=MI_U32((0x00000030u+f->r[4]));
    /* 00474d15 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474d17 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474d19 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474d1a */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474d1b */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474d1c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474d1d */
    f->r[4]-=4;MI_U32(f->r[4])=0x00474d22u;f->boundary(0x00474d1du,0x004767d0u,f);
    /* 00474d22 */
    f->r[0]=MI_U32((0x0000002cu+f->r[4]));
    /* 00474d26 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00474d29 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00474d2b */
    if((f->flags&MI_ZF))goto mi_00474d49;
    /* 00474d2d */
    f->r[1]=MI_U32((0x00000030u+f->r[4]));
    /* 00474d31 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474d33 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 00474d35 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474d37 */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 00474d39 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474d3a */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 00474d3c */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474d3d */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00474d3f */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474d40 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474d41 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00474d46u;f->boundary(0x00474d41u,0x004767d0u,f);
    /* 00474d46 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00474d49:;
    /* 00474d49 */
    f->r[3]=MI_U32((0x0000002cu+f->r[4]));
    /* 00474d4d */
    f->r[2]=MI_U32((0x00000028u+f->r[4]));
    /* 00474d51 */
    f->r[6]=MI_U32((0x00000034u+f->r[4]));
    /* 00474d55 */
    f->r[7]=MI_U32((0x00000030u+f->r[4]));
    /* 00474d59 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474d5a */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474d5b */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474d5c */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474d5d */
    f->r[4]-=4;MI_U32(f->r[4])=0x00474d62u;f->boundary(0x00474d5du,0x00477500u,f);
    /* 00474d62 */
    f->r[0]=MI_U32((0x00000038u+f->r[4]));
    /* 00474d66 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474d67 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474d68 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474d69 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474d6a */
    f->r[4]-=4;MI_U32(f->r[4])=0x00474d6fu;f->boundary(0x00474d6au,0x004776f0u,f);
    /* 00474d6f */
    f->r[1]=MI_U32((0x00000048u+f->r[4]));
    /* 00474d73 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474d74 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474d75 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474d76 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474d77 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00474d7cu;f->boundary(0x00474d77u,0x004777c0u,f);
    /* 00474d7c */
    f->r[2]=MI_U32((0x00000058u+f->r[4]));
    /* 00474d80 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474d82 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474d83 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474d84 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474d85 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474d86 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00474d8bu;f->boundary(0x00474d86u,0x0047bae0u,f);
    /* 00474d8b */
    a=f->r[4];b=0x00000044u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00474d8e */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474d8f */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474d90 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474d91 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00474d96u;f->boundary(0x00474d91u,0x0047b480u,f);
    /* 00474d96 */
    f->r[0]=MI_U32((0x00000034u+f->r[4]));
    /* 00474d9a */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474d9b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474d9c */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474d9d */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474d9e */
    f->r[4]-=4;MI_U32(f->r[4])=0x00474da3u;f->boundary(0x00474d9eu,0x00477890u,f);
    /* 00474da3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00474da8u;f->boundary(0x00474da3u,0x00476530u,f);
    /* 00474da8 */
    f->r[1]=MI_U32((0x00bca2d4u));
    /* 00474dae */
    f->r[2]=MI_U32((0x00bca2d0u));
    /* 00474db4 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00474db5 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474db7 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00474db8 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474db9 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474dba */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474dbc */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474dbe */
    f->r[4]-=4;MI_U32(f->r[4])=0x00474dc3u;f->boundary(0x00474dbeu,0x00475720u,f);
    /* 00474dc3 */
    a=f->r[4];b=0x00000030u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00474dc6 */
    f->r[7]=0x00000001u;
    /* 00474dcb */
    f->r[6]=0x00799ae0u;
    /* 00474dd0 */
    f->r[3]=0x0000001eu;
mi_00474dd5:;
    /* 00474dd5 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000000u+f->r[6])))&0xffu)<<0);
    /* 00474dd7 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00474dd9 */
    if((f->flags&MI_ZF))goto mi_00474ec2;
    /* 00474ddf */
    a=(f->r[0]&0xffu);b=0x00000006u;t=mi_alu(f,a,b,8,1);
    /* 00474de1 */
    if((f->flags&MI_ZF))goto mi_00474ec2;
    /* 00474de7 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00000010u+f->r[6])))&0xffffu)<<0);
    /* 00474deb */
    a=(f->r[0]&0xffffu);b=0x0000001fu;t=mi_alu(f,a,b,16,1);
    /* 00474def */
    if(!(f->flags&MI_ZF))goto mi_00474e18;
    /* 00474df1 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474df2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00474df7u;f->boundary(0x00474df2u,0x00475b50u,f);
    /* 00474df7 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00474dfa */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 00474dfc */
    if(!(f->flags&MI_ZF))goto mi_00474ec2;
    /* 00474e02 */
    f->r[0]=MI_U32((0x000000d8u+f->r[6]));
    /* 00474e08 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00474e0a */
    if((f->flags&MI_ZF))goto mi_00474ec2;
    /* 00474e10 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474e11 */
    a=0x0000001fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474e13 */
    goto mi_00474eb3;
mi_00474e18:;
    /* 00474e18 */
    a=(f->r[0]&0xffffu);b=(f->r[3]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00474e1b */
    if(!(f->flags&MI_ZF))goto mi_00474e67;
    /* 00474e1d */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x0000003au+f->r[6]));
    /* 00474e21 */
    f->r[1]=f->r[0];
    /* 00474e23 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00474e26 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00474e28 */
    a=MI_U16((0x007999e8u+f->r[1]*8u));b=(f->r[3]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00474e30 */
    if(!(f->flags&MI_ZF))goto mi_00474e45;
    /* 00474e32 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000008u+f->r[6]));
    /* 00474e36 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000006u+f->r[6]));
    /* 00474e3a */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474e3b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474e3c */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474e3d */
    f->r[4]-=4;MI_U32(f->r[4])=0x00474e42u;f->boundary(0x00474e3du,0x00440540u,f);
    /* 00474e42 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00474e45:;
    /* 00474e45 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474e46 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00474e4bu;f->boundary(0x00474e46u,0x00475b50u,f);
    /* 00474e4b */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00474e4e */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 00474e50 */
    if(!(f->flags&MI_ZF))goto mi_00474ec2;
    /* 00474e52 */
    f->r[0]=MI_U32((0x000000d8u+f->r[6]));
    /* 00474e58 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00474e5a */
    if((f->flags&MI_ZF))goto mi_00474ec2;
    /* 00474e5c */
    f->r[1]=MI_U32((0x000000d0u+f->r[6]));
    /* 00474e62 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474e63 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474e64 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474e65 */
    goto mi_00474eba;
mi_00474e67:;
    /* 00474e67 */
    a=(f->r[0]&0xffffu);b=0x00000021u;t=mi_alu(f,a,b,16,1);
    /* 00474e6b */
    if(!(f->flags&MI_ZF))goto mi_00474e90;
    /* 00474e6d */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474e6e */
    f->r[4]-=4;MI_U32(f->r[4])=0x00474e73u;f->boundary(0x00474e6eu,0x00475b50u,f);
    /* 00474e73 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00474e76 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 00474e78 */
    if(!(f->flags&MI_ZF))goto mi_00474ec2;
    /* 00474e7a */
    f->r[0]=MI_U32((0x000000d8u+f->r[6]));
    /* 00474e80 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00474e82 */
    if((f->flags&MI_ZF))goto mi_00474ec2;
    /* 00474e84 */
    f->r[2]=MI_U32((0x000000d0u+f->r[6]));
    /* 00474e8a */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474e8b */
    a=0x00000021u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474e8d */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474e8e */
    goto mi_00474eba;
mi_00474e90:;
    /* 00474e90 */
    a=(f->r[0]&0xffffu);b=0x000000d1u;t=mi_alu(f,a,b,16,1);
    /* 00474e94 */
    if(!(f->flags&MI_ZF))goto mi_00474ec2;
    /* 00474e96 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474e97 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00474e9cu;f->boundary(0x00474e97u,0x00475b50u,f);
    /* 00474e9c */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00474e9f */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 00474ea1 */
    if(!(f->flags&MI_ZF))goto mi_00474ec2;
    /* 00474ea3 */
    f->r[0]=MI_U32((0x000000d8u+f->r[6]));
    /* 00474ea9 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00474eab */
    if((f->flags&MI_ZF))goto mi_00474ec2;
    /* 00474ead */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474eae */
    a=0x000000d1u;f->r[4]-=4;MI_U32(f->r[4])=a;
mi_00474eb3:;
    /* 00474eb3 */
    f->r[0]=MI_U32((0x000000d0u+f->r[6]));
    /* 00474eb9 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
mi_00474eba:;
    /* 00474eba */
    f->r[4]-=4;MI_U32(f->r[4])=0x00474ebfu;f->boundary(0x00474ebau,0x00440060u,f);
    /* 00474ebf */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00474ec2:;
    /* 00474ec2 */
    a=f->r[6];b=0x00000108u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00474ec8 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 00474ec9 */
    a=f->r[6];b=0x0089b6d8u;t=mi_alu(f,a,b,32,1);
    /* 00474ecf */
    if(mi_signed_less(f->flags))goto mi_00474dd5;
    /* 00474ed5 */
    f->r[6]=MI_U32((0x00000034u+f->r[4]));
    /* 00474ed9 */
    f->r[7]=MI_U32((0x00000030u+f->r[4]));
    /* 00474edd */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474ede */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474edf */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474ee0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00474ee5u;f->boundary(0x00474ee0u,0x0047afd0u,f);
    /* 00474ee5 */
    f->r[1]=MI_U32((0x00000038u+f->r[4]));
    /* 00474ee9 */
    f->r[2]=MI_U32((0x00000034u+f->r[4]));
    /* 00474eed */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474eef */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474ef0 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474ef1 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474ef2 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474ef3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00474ef8u;f->boundary(0x00474ef3u,0x00475cc0u,f);
    /* 00474ef8 */
    f->r[0]=MI_U32((0x00000044u+f->r[4]));
    /* 00474efc */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00474eff */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00474f01 */
    if(!(f->flags&MI_ZF))goto mi_00474f48;
    /* 00474f03 */
    f->r[0]=MI_U32((0x0000002cu+f->r[4]));
    /* 00474f07 */
    f->r[1]=MI_U32((0x00000028u+f->r[4]));
    /* 00474f0b */
    f->r[2]=MI_U32((0x00000034u+f->r[4]));
    /* 00474f0f */
    f->r[6]=(0x00000001u+f->r[0]);
    /* 00474f12 */
    f->r[0]=MI_U32((0x00000030u+f->r[4]));
    /* 00474f16 */
    f->r[7]=(0x00000001u+f->r[1]);
    /* 00474f19 */
    f->r[3]=(0xffffffffu+f->r[2]);
    /* 00474f1c */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474f1d */
    f->r[5]=(0xffffffffu+f->r[0]);
    /* 00474f20 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474f21 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474f22 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474f23 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00474f28u;f->boundary(0x00474f23u,0x0047bd50u,f);
    /* 00474f28 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474f29 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474f2a */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474f2b */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474f2c */
    f->r[4]-=4;MI_U32(f->r[4])=0x00474f31u;f->boundary(0x00474f2cu,0x0047b590u,f);
    /* 00474f31 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00474f34 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00474f39u;f->boundary(0x00474f34u,0x005178b0u,f);
    /* 00474f39 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00474f3eu;f->boundary(0x00474f39u,0x00517370u,f);
    /* 00474f3e */
    f->r[4]-=4;MI_U32(f->r[4])=0x00474f43u;f->boundary(0x00474f3eu,0x00517660u,f);
    /* 00474f43 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00474f48u;f->boundary(0x00474f43u,0x005178e0u,f);
mi_00474f48:;
    /* 00474f48 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00474f49 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00474f4a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00474f4b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00474f4c */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00474f4f */
    f->r[4]+=4;return;
}

void restore_construction_C_004743f0(struct restore_construction_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004743f0 */
    f->r[0]=MI_U32((0x00000004u+f->r[4]));
    /* 004743f4 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 004743f7 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004743f8 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 004743fa */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004743fc */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004743fd */
    MI_U32((0x00bc96b4u))=(uint32_t)(f->r[6]);
    /* 00474403 */
    MI_U32((0x00bc96b0u))=(uint32_t)(f->r[6]);
    /* 00474409 */
    MI_U32((0x00bc96acu))=(uint32_t)(f->r[6]);
    /* 0047440f */
    MI_U32((0x00bc96a8u))=(uint32_t)(f->r[6]);
    /* 00474415 */
    if((f->flags&MI_ZF))goto mi_0047443a;
    /* 00474417 */
    f->r[1]=MI_U32((0x0000002cu+f->r[4]));
    /* 0047441b */
    f->r[2]=MI_U32((0x00000028u+f->r[4]));
    /* 0047441f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474420 */
    f->r[1]=MI_U32((0x00000028u+f->r[4]));
    /* 00474424 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474425 */
    f->r[2]=MI_U32((0x00000028u+f->r[4]));
    /* 00474429 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047442a */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047442b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047442c */
    f->r[4]-=4;MI_U32(f->r[4])=0x00474431u;f->boundary(0x0047442cu,0x00474700u,f);
    /* 00474431 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00474434 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00474435 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00474436 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00474439 */
    f->r[4]+=4;return;
mi_0047443a:;
    /* 0047443a */
    f->r[0]=MI_U32((0x00000020u+f->r[4]));
    /* 0047443e */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047443f */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474440 */
    f->r[5]=MI_U32((0x00000030u+f->r[4]));
    /* 00474444 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 00474446 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0047444c;
    /* 00474448 */
    f->r[3]=f->r[0];
    /* 0047444a */
    goto mi_00474450;
mi_0047444c:;
    /* 0047444c */
    f->r[3]=f->r[5];
    /* 0047444e */
    f->r[5]=f->r[0];
mi_00474450:;
    /* 00474450 */
    f->r[2]=MI_U32((0x0000002cu+f->r[4]));
    /* 00474454 */
    f->r[0]=MI_U32((0x00000034u+f->r[4]));
    /* 00474458 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0047445a */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00474464;
    /* 0047445c */
    f->r[1]=f->r[2];
    /* 0047445e */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00474462 */
    goto mi_0047446a;
mi_00474464:;
    /* 00474464 */
    f->r[1]=f->r[0];
    /* 00474466 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[2]);
mi_0047446a:;
    /* 0047446a */
    a=f->r[3];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 0047446c */
    if(!mi_signed_less(f->flags))goto mi_00474470;
    /* 0047446e */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
mi_00474470:;
    /* 00474470 */
    f->r[0]=MI_U32((0x00bca2d0u));
    /* 00474475 */
    a=f->r[5];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00474477 */
    if(mi_signed_less(f->flags))goto mi_0047447c;
    /* 00474479 */
    f->r[5]=(0xffffffffu+f->r[0]);
mi_0047447c:;
    /* 0047447c */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 0047447e */
    if(!mi_signed_less(f->flags))goto mi_00474482;
    /* 00474480 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_00474482:;
    /* 00474482 */
    f->r[0]=MI_U32((0x00bca2d4u));
    /* 00474487 */
    f->r[6]=MI_U32((0x00000024u+f->r[4]));
    /* 0047448b */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0047448d */
    if(mi_signed_less(f->flags))goto mi_00474494;
    /* 0047448f */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00474490 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[0]);
mi_00474494:;
    /* 00474494 */
    f->r[6]=MI_U32((0x00bca2d8u));
    /* 0047449a */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 0047449d */
    f->r[7]=f->r[5];
    /* 0047449f */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 004744a7 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[0]*2u);
    /* 004744aa */
    a=f->r[7];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 004744ac */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 004744ad */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(0x00000000u);
    /* 004744b5 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004744b8 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 004744c0 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 004744c8 */
    MI_U32((0x00bc9760u))=(uint32_t)(f->r[1]);
    /* 004744ce */
    f->r[0]=(0x00000000u+f->r[6]+f->r[0]*4u);
    /* 004744d1 */
    f->r[6]=MI_U32((0x00000024u+f->r[4]));
    /* 004744d5 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004744d7 */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004744d9 */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[0]);
    /* 004744de */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00474611;
mi_004744e4:;
    /* 004744e4 */
    a=f->r[3];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004744e6 */
    MI_U32((0x00bc9764u))=(uint32_t)(f->r[3]);
    /* 004744ec */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004745e4;
mi_004744f2:;
    /* 004744f2 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00b4e6fcu+f->r[0]*2u));
    /* 004744fa */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004744fc */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00adc36cu+f->r[0])))&0xffu)<<0);
    /* 00474502 */
    f->r[0]=MI_U32((0x00af598cu+f->r[0]*4u));
    /* 00474509 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 0047450c */
    f->r[6]=f->r[2];
    /* 0047450e */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00474510 */
    if((f->flags&MI_ZF))goto mi_00474535;
    /* 00474512 */
    f->r[2]=f->r[1];
    /* 00474514 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 00474517 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00474519 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x007999e8u+f->r[2]*8u)))&0xffffu)<<0);
    /* 00474521 */
    a=(f->r[2]&0xffffu);b=0x00000039u;t=mi_alu(f,a,b,16,1);
    /* 00474525 */
    if((f->flags&MI_ZF))goto mi_0047452d;
    /* 00474527 */
    a=(f->r[2]&0xffffu);b=0x00000036u;t=mi_alu(f,a,b,16,1);
    /* 0047452b */
    if(!(f->flags&MI_ZF))goto mi_00474535;
mi_0047452d:;
    /* 0047452d */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(0x00000001u);
mi_00474535:;
    /* 00474535 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 00474537 */
    if((f->flags&MI_ZF))goto mi_00474549;
    /* 00474539 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0047453b */
    if((f->flags&MI_ZF))goto mi_00474549;
    /* 0047453d */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0047453f */
    if(!(f->flags&MI_ZF))goto mi_00474549;
    /* 00474541 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(0x00000001u);
mi_00474549:;
    /* 00474549 */
    f->r[0]=f->r[1];
    /* 0047454b */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0047454e */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00474550 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e8u+f->r[0]*8u));
    /* 00474558 */
    f->r[6]=(0x007999e8u+f->r[0]*8u);
    /* 0047455f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474560 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00474565u;f->boundary(0x00474560u,0x00474380u,f);
    /* 00474565 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00474568 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0047456a */
    if((f->flags&MI_ZF))goto mi_00474574;
    /* 0047456c */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00000001u);
mi_00474574:;
    /* 00474574 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[6]));
    /* 00474577 */
    a=f->r[0];b=0x00000041u;t=mi_alu(f,a,b,32,1);
    /* 0047457a */
    if((f->flags&MI_ZF))goto mi_004745b8;
    /* 0047457c */
    a=f->r[0];b=0x00000042u;t=mi_alu(f,a,b,32,1);
    /* 0047457f */
    if((f->flags&MI_ZF))goto mi_004745b8;
    /* 00474581 */
    a=f->r[0];b=0x00000043u;t=mi_alu(f,a,b,32,1);
    /* 00474584 */
    if((f->flags&MI_ZF))goto mi_004745b8;
    /* 00474586 */
    a=f->r[0];b=0x00000044u;t=mi_alu(f,a,b,32,1);
    /* 00474589 */
    if((f->flags&MI_ZF))goto mi_004745b8;
    /* 0047458b */
    a=f->r[0];b=0x00000045u;t=mi_alu(f,a,b,32,1);
    /* 0047458e */
    if((f->flags&MI_ZF))goto mi_004745b8;
    /* 00474590 */
    a=f->r[0];b=0x00000061u;t=mi_alu(f,a,b,32,1);
    /* 00474593 */
    if((f->flags&MI_ZF))goto mi_004745b8;
    /* 00474595 */
    a=f->r[0];b=0x0000009cu;t=mi_alu(f,a,b,32,1);
    /* 0047459a */
    if((f->flags&MI_ZF))goto mi_004745b8;
    /* 0047459c */
    a=f->r[0];b=0x0000009du;t=mi_alu(f,a,b,32,1);
    /* 004745a1 */
    if((f->flags&MI_ZF))goto mi_004745b8;
    /* 004745a3 */
    a=f->r[0];b=0x0000009eu;t=mi_alu(f,a,b,32,1);
    /* 004745a8 */
    if((f->flags&MI_ZF))goto mi_004745b8;
    /* 004745aa */
    a=f->r[0];b=0x0000009fu;t=mi_alu(f,a,b,32,1);
    /* 004745af */
    if((f->flags&MI_ZF))goto mi_004745b8;
    /* 004745b1 */
    a=f->r[0];b=0x000000a0u;t=mi_alu(f,a,b,32,1);
    /* 004745b6 */
    if(!(f->flags&MI_ZF))goto mi_004745c0;
mi_004745b8:;
    /* 004745b8 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(0x00000001u);
mi_004745c0:;
    /* 004745c0 */
    f->r[1]=MI_U32((0x00bc9764u));
    /* 004745c6 */
    f->r[0]=MI_U32((0x00bc9768u));
    /* 004745cb */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004745cc */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004745cd */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004745cf */
    MI_U32((0x00bc9764u))=(uint32_t)(f->r[1]);
    /* 004745d5 */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[0]);
    /* 004745da */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004744f2;
    /* 004745e0 */
    f->r[2]=MI_U32((0x0000002cu+f->r[4]));
mi_004745e4:;
    /* 004745e4 */
    f->r[1]=MI_U32((0x00bc9760u));
    /* 004745ea */
    f->r[6]=MI_U32((0x00000024u+f->r[4]));
    /* 004745ee */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004745ef */
    MI_U32((0x00bc9760u))=(uint32_t)(f->r[1]);
    /* 004745f5 */
    f->r[1]=0x000000e4u;
    /* 004745fa */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004745fc */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004745fe */
    f->r[1]=MI_U32((0x00bc9760u));
    /* 00474604 */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 00474606 */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[0]);
    /* 0047460b */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004744e4;
mi_00474611:;
    /* 00474611 */
    f->r[7]=MI_U32((0x00000010u+f->r[4]));
    /* 00474615 */
    f->r[1]=MI_U32((0x00000028u+f->r[4]));
    /* 00474619 */
    f->r[6]=MI_U32((0x00000030u+f->r[4]));
    /* 0047461d */
    f->r[0]=MI_U32((0x00000034u+f->r[4]));
    /* 00474621 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00474622 */
    MI_U32((0x00bc96a4u))=(uint32_t)(f->r[1]);
    /* 00474628 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 0047462a */
    MI_U32((0x00bc96a0u))=(uint32_t)(f->r[2]);
    /* 00474630 */
    MI_U32((0x00bc969cu))=(uint32_t)(f->r[6]);
    /* 00474636 */
    MI_U32((0x00bc9698u))=(uint32_t)(f->r[0]);
    /* 0047463b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0047463c */
    if((f->flags&MI_ZF))goto mi_00474655;
    /* 0047463e */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474640 */
    a=0x004021d0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474645 */
    a=0x00000068u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474647 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047464cu;f->boundary(0x00474647u,0x004264d0u,f);
    /* 0047464c */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047464f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00474650 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00474651 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00474654 */
    f->r[4]+=4;return;
mi_00474655:;
    /* 00474655 */
    f->r[7]=MI_U32((0x0000000cu+f->r[4]));
    /* 00474659 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 0047465b */
    if((f->flags&MI_ZF))goto mi_00474674;
    /* 0047465d */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047465f */
    a=0x00402210u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474664 */
    a=0x0000006au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474666 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047466bu;f->boundary(0x00474666u,0x004264d0u,f);
    /* 0047466b */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047466e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0047466f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00474670 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00474673 */
    f->r[4]+=4;return;
mi_00474674:;
    /* 00474674 */
    f->r[7]=MI_U32((0x00000010u+f->r[4]));
    /* 00474678 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 0047467a */
    if((f->flags&MI_ZF))goto mi_00474693;
    /* 0047467c */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047467e */
    a=0x00402250u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474683 */
    a=0x00000010u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00474685 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047468au;f->boundary(0x00474685u,0x004264d0u,f);
    /* 0047468a */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047468d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0047468e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0047468f */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00474692 */
    f->r[4]+=4;return;
mi_00474693:;
    /* 00474693 */
    f->r[7]=MI_U32((0x00000014u+f->r[4]));
    /* 00474697 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 00474699 */
    if((f->flags&MI_ZF))goto mi_004746b2;
    /* 0047469b */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047469d */
    a=0x00402290u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004746a2 */
    a=0x00000014u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004746a4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004746a9u;f->boundary(0x004746a4u,0x004264d0u,f);
    /* 004746a9 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004746ac */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004746ad */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004746ae */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004746b1 */
    f->r[4]+=4;return;
mi_004746b2:;
    /* 004746b2 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004746b3 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004746b4 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004746b5 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004746b6 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004746b8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004746bdu;f->boundary(0x004746b8u,0x00474700u,f);
    /* 004746bd */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004746c0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004746c1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004746c2 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004746c5 */
    f->r[4]+=4;return;
}

void restore_construction_dispatch(uint32_t site,uint32_t target,struct restore_construction_frame *f)
{
 uint32_t sp=f->r[4],a=MI_U32(sp+4u); (void)site;
 switch(target){
 case 0x40f860u:FUN_0040f860((void *)(uintptr_t)a,(void *)(uintptr_t)MI_U32(sp+8u),(int32_t)MI_U32(sp+12u));break;
 case 0x4264d0u:FUN_004264d0(a,MI_U32(sp+8u),MI_U32(sp+12u));break;
 case 0x440060u:FUN_00440060(a,(int32_t)MI_U32(sp+8u),(uint16_t)MI_U32(sp+12u));break;
 case 0x440540u:f->r[1]=restore_bridge_00440540((uint32_t *)(uintptr_t)(sp+4u),f->r[1]);break;
 case 0x459020u:FUN_00459020((int32_t)a,(int32_t)MI_U32(sp+8u),(int32_t)MI_U32(sp+12u),(int32_t)MI_U32(sp+16u));break;
 case 0x474380u:f->r[0]=(a&0xffffff00u)|(uint8_t)FUN_00474380((int32_t)a);break;
 case 0x474700u:restore_construction_C_00474700(f);return;
 case 0x475720u:FUN_00475720((int32_t)a,(int32_t)MI_U32(sp+8u),(int32_t)MI_U32(sp+12u),(int32_t)MI_U32(sp+16u),MI_U32(sp+20u));break;
 case 0x475b50u:f->r[0]=(uint32_t)FUN_00475b50((int32_t)a);break;
 case 0x475cc0u:FUN_00475cc0((int32_t)a,(int32_t)MI_U32(sp+8u),(int32_t)MI_U32(sp+12u),(int32_t)MI_U32(sp+16u),MI_U32(sp+20u));break;
 case 0x476530u:FUN_00476530();break;
 case 0x4767d0u:FUN_004767d0((int32_t)a,(int32_t)MI_U32(sp+8u),(int32_t)MI_U32(sp+12u),(int32_t)MI_U32(sp+16u),MI_U32(sp+20u),MI_U32(sp+24u));break;
 case 0x477500u:FUN_00477500((int32_t)a,(int32_t)MI_U32(sp+8u),(int32_t)MI_U32(sp+12u),(int32_t)MI_U32(sp+16u));break;
 case 0x4776f0u:FUN_004776f0((int32_t)a,(int32_t)MI_U32(sp+8u),(int32_t)MI_U32(sp+12u),(int32_t)MI_U32(sp+16u));break;
 case 0x4777c0u:FUN_004777c0((int32_t)a,(int32_t)MI_U32(sp+8u),(int32_t)MI_U32(sp+12u),(int32_t)MI_U32(sp+16u));break;
 case 0x477890u:FUN_00477890((int32_t)a,(int32_t)MI_U32(sp+8u),(int32_t)MI_U32(sp+12u),(int32_t)MI_U32(sp+16u));break;
 case 0x47afd0u:FUN_0047afd0((int32_t)a,(int32_t)MI_U32(sp+8u),(int32_t)MI_U32(sp+12u));break;
 case 0x47b480u:FUN_0047b480((int32_t)a,(int32_t)MI_U32(sp+8u),(int32_t)MI_U32(sp+12u));break;
 case 0x47b590u:FUN_0047b590((int32_t)a,(int32_t)MI_U32(sp+8u),(int32_t)MI_U32(sp+12u),(int32_t)MI_U32(sp+16u));break;
 case 0x47bae0u:FUN_0047bae0((int32_t)a,(int32_t)MI_U32(sp+8u),(int32_t)MI_U32(sp+12u),(int32_t)MI_U32(sp+16u),MI_U32(sp+20u));break;
 case 0x47bd50u:FUN_0047bd50((int32_t)a,(int32_t)MI_U32(sp+8u),(int32_t)MI_U32(sp+12u),(int32_t)MI_U32(sp+16u));break;
 case 0x47f810u:FUN_0047f810(a,MI_U32(sp+8u));break;
 case 0x47f9b0u:f->r[0]=FUN_0047f9b0(a);break;
 case 0x485080u:FUN_00485080();break;
 case 0x485100u:f->r[0]=FUN_00485100((int32_t)a);break;
 case 0x485830u:FUN_00485830();break;
 case 0x4e2970u:FUN_004e2970((int32_t)a);break;
 case 0x4f4930u:FUN_004f4930((int32_t)a);break;
 case 0x4f5e70u:FUN_004f5e70((int32_t)a,(uint8_t)MI_U32(sp+8u));break;
 case 0x517370u:FUN_00517370();break;
 case 0x517660u:FUN_00517660();break;
 case 0x5178b0u:FUN_005178b0();break;
 case 0x5178e0u:FUN_005178e0();break;
 case 0x53b090u:FUN_0053b090((int32_t)a);break;
 default:abort();
 }
 f->r[4]=sp+4u;
}
static void construction(uint32_t entry,int32_t preview,int32_t x1,int32_t y1,int32_t x2,int32_t y2)
{
 uint32_t stack[256],i,sp;struct restore_construction_frame f={{0},2,restore_construction_dispatch};
 for(i=0;i<256;++i)stack[i]=0x9a47c123u;
 stack[241]=(uint32_t)preview;stack[242]=(uint32_t)x1;stack[243]=(uint32_t)y1;stack[244]=(uint32_t)x2;stack[245]=(uint32_t)y2;
 sp=(uint32_t)(uintptr_t)(stack+240);f.r[4]=sp;
 if(entry==0x4743f0u)restore_construction_C_004743f0(&f);else restore_construction_C_00474700(&f);
 if(f.r[4]!=sp+4u||f.r[3]||f.r[5]||f.r[6]||f.r[7])abort();
 for(i=0;i<16;++i)if(stack[i]!=0x9a47c123u)abort();for(i=246;i<256;++i)if(stack[i]!=0x9a47c123u)abort();
}
void FUN_004743f0(int32_t preview,int32_t x1,int32_t y1,int32_t x2,int32_t y2){construction(0x4743f0u,preview,x1,y1,x2,y2);}
void FUN_00474700(int32_t preview,int32_t x1,int32_t y1,int32_t x2,int32_t y2){construction(0x474700u,preview,x1,y1,x2,y2);}
