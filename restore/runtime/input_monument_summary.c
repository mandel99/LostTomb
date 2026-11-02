#include "input_monument_summary.h"
#include "pyramid_summary.h"
#include "campaign_score_numeric.h"
#include "monument_base_queries.h"
#include "monument_complex_queries.h"
#include "monument_progress.h"

#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define SELF(a) ((void *)(uintptr_t)(a))
extern void FUN_004ccbb0(int,int,int,int,int,int);

/* Entire pinned 4192B0..419448, including all fourteen nondefault cases
   of the original 41944C/419484 table. Nullary cdecl, full EAX count, RET0.
   Render/text providers remain actual production C. */
uint32_t FUN_004192b0(void)
{
 uint32_t slot,count=0,line=180;
 int32_t summary[28]; /* Native 112-byte local output; never pre-zeroed. */
 for(slot=0;slot<3;++slot){
  uint32_t building=U(0xea5facu+(U(0xeaac28u)*0x1276u+slot)*4u);
  uint32_t percentage,record;
  int32_t kind;
  if((int32_t)building<=0)continue;
  record=building*264u;
  kind=(int32_t)(int16_t)W(0x7999e8u+record);
  switch(kind){
  case 0xb7:
   (void)FUN_004f1310(building,summary);
   /* 419316 reads ESP+4C before ADD ESP,8: local output DWORD13. */
   percentage=(uint32_t)summary[13];break;
  case 0xcf:case 0xd0:percentage=B(0x799a2du+record);break;
  case 0xd2:{
   int32_t progress=(int32_t)(int16_t)W(0xea5baau+U(0xeaac28u)*0x49d8u);
   percentage=(uint32_t)(progress*100/16);break;
  }
  case 0xd5:percentage=(uint32_t)FUN_00542ad0(building,0);break;
  case 0xde:percentage=(uint32_t)FUN_004df040(building,0);break;
  case 0xe1:percentage=(uint32_t)restore_monument_base_0040d940(SELF(0x6fd5d8u),building,0);break;
  case 0xe3:percentage=(uint32_t)restore_monument_base_00417050(SELF(0x6fd368u),building,0);break;
  case 0xe4:percentage=(uint32_t)restore_monument_base_004e6710(SELF(0x6de548u),building,0);break;
  case 0xe5:percentage=(uint32_t)restore_monument_progress_00545740(SELF(0x6f9320u),building,0);break;
  case 0xe6:percentage=(uint32_t)FUN_00401540(building,0);break;
  case 0xea:percentage=(uint32_t)restore_monument_progress_00558be0(SELF(0x6f5648u),building,0);break;
  case 0xeb:percentage=(uint32_t)restore_monument_progress_00555e80(SELF(0x6de9e0u),building,0);break;
  case 0xec:percentage=(uint32_t)restore_monument_progress_0054f470(SELF(0x6e9188u),building,0);break;
  default:continue;
  }
  if(percentage==100u){
   uint32_t ybase,font,y,player,x,text;
   /* Keep the native ordered reloads after the real progress helper. */
   ybase=U(0xf53db4u);line+=20u;font=U(0xf56f8cu);
   y=ybase+line;player=U(0xeaac28u);x=U(0xf53db8u)+275u;
   ++count;text=U(0xea5fa0u+(player*0x1276u+slot)*4u);
   FUN_004ccbb0(198,(int32_t)text,(int32_t)x,(int32_t)y,(int32_t)font,0);
  }
 }
 return count;
}
