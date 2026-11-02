#include "placement_palette.h"
#include "sprite_libraries.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define H(a) (*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
static uint32_t half(uint32_t value)
{return (uint32_t)((int32_t)value/2);}
uint32_t restore_placement_image_005414b0(int32_t building,uint32_t out_flags)
{
 uint32_t r=(uint32_t)building*264u,kind,stage,choice,asset,rotation,base;
 U(out_flags)=0;
 kind=U(0x601de4u+(uint32_t)(int32_t)H(0x7999eau+r)*52u);
 /* Actual13-entry native table541778 maps0..12 to the following cases. */
 switch(kind){
 case 0:
  stage=U(0x799a30u+r);choice=(int32_t)stage<9?0u:(int32_t)stage>=13?2u:1u;
  asset=U(0x799a2cu+r);
  if(asset==2){
   (void)FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,(const char *)(uintptr_t)U(0x603228u+choice*4u),7);
   rotation=U(0xbca2e0u);base=U(0xf56d64u);
  }else if(asset==1){
   (void)FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,(const char *)(uintptr_t)U(0x603228u+choice*4u),6);
   rotation=U(0xbca2e0u);base=U(0xf56d5cu);
  }else{
   (void)FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,(const char *)(uintptr_t)U(0x603228u+choice*4u),5);
   rotation=U(0xbca2e0u);base=U(0xf56d54u);
  }
  return (half(rotation)&1u)+base;
 case 1:
  if(U(0x799a30u+r)!=2)goto fallback;
  rotation=U(0xbca2e0u);base=U(0xf56d74u);return (half(rotation)&1u)+base;
 case 2:
  if(U(0x799a30u+r)!=2)goto fallback;
  rotation=8u-U(0xbca2e0u);base=U(0xf56d74u);return (half(rotation)&3u)+base+2u;
 case 3:
  if(U(0x799a30u+r)!=2)goto fallback;
  rotation=8u-U(0xbca2e0u);base=U(0xf56d74u);return ((half(rotation)+1u)&3u)+base+2u;
 case 4:
  if(U(0x799a30u+r)!=2)goto fallback;
  rotation=8u-U(0xbca2e0u);base=U(0xf56d74u);return ((half(rotation)-2u)&3u)+base+2u;
 case 5:
  if(U(0x799a30u+r)!=2)goto fallback;
  rotation=8u-U(0xbca2e0u);base=U(0xf56d74u);return ((half(rotation)-1u)&3u)+base+2u;
 case 6:
  if(U(0x799a30u+r)!=2)goto masked_fallback;
  rotation=U(0xbca2e0u);base=U(0xf56d74u);return (half(rotation)&1u)+base+6u;
 case 7:
  if(U(0x799a30u+r)!=2)goto masked_fallback;
  rotation=U(0xbca2e0u);base=U(0xf56d74u);return ((half(rotation)-1u)&1u)+base+6u;
 case 8:
  U(out_flags)=5;stage=U(0x799a30u+r);
  if(stage==2)return U(0xf56d74u)+8u;
  base=U(0xf56b54u);return stage==1?base:0;
 case 9:
  U(out_flags)=5;stage=U(0x799a30u+r);
  if(stage==2)return U(0xf56d74u)+9u;
  base=U(0xf56b54u);return stage==1?base:0;
 case 10:
  if(U(0x799a30u+r)!=2)goto masked_fallback;
  return U(0xf56d74u)+10u;
 case 11:
  if(U(0x799a30u+r)!=2)goto masked_fallback;
  return U(0xf56d74u)+11u;
 case 12:
  if(U(0x799a30u+r)==4)return U(0xf56d74u)+12u;
  U(out_flags)=5;
  if((int32_t)U(0x799a30u+r)>=1)return U(0xf56b54u);
  U(out_flags)=7;return U(0xf5736cu);
 default:return 0;
 }
fallback:
 U(out_flags)=5;
 if(U(0x799a30u+r)==1)return U(0xf56b54u);
 return U(0xf5736cu);
masked_fallback:
 U(out_flags)=5;stage=U(0x799a30u+r);base=U(0xf56b54u);
 /* Native DEC/NEG/SBB/NOT/AND returns base exactly when stage==1. */
 return stage==1?base:0;
}
