#include "campaign_score.h"
#include "campaign_score_numeric.h"
#include "monument_progress.h"
#include "monument_base_queries.h"
#include "monument_complex_queries.h"
#include "pyramid_summary.h"
#define CS_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define CS_W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define CS_B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
int32_t FUN_0041c380(uint32_t months)
{
 uint32_t i,scene=0,bonus=0;
 for(i=0;i<3;++i){
  int32_t building;uint32_t progress,offset;
  scene=CS_U(0xeaac28u);offset=scene*0x49d8u;
  building=(int32_t)CS_U(0xea5facu+offset+i*4u);
  if(building<=0)continue;
  switch((int16_t)CS_W(0x7999e8u+(uint32_t)building*264u)){
  case 0xb7:
   {int32_t summary[28];(void)FUN_004f1310((uint32_t)building,summary);progress=(uint32_t)summary[13];}break;
  case 0xcf:case 0xd0:progress=CS_B(0x799a2du+(uint32_t)building*264u);break;
  case 0xd2:progress=(uint32_t)((int32_t)(int16_t)CS_W(0xea5baau+offset)*100/16);break;
  case 0xd5:progress=(uint32_t)FUN_00542ad0((uint32_t)building,0);break;
  case 0xde:progress=(uint32_t)FUN_004df040((uint32_t)building,0);break;
  case 0xe1:progress=(uint32_t)restore_monument_base_0040d940((void *)0x6fd5d8u,(uint32_t)building,0);break;
  case 0xe3:progress=(uint32_t)restore_monument_base_00417050((void *)0x6fd368u,(uint32_t)building,0);break;
  case 0xe4:progress=(uint32_t)restore_monument_base_004e6710((void *)0x6de548u,(uint32_t)building,0);break;
  case 0xe5:progress=(uint32_t)restore_monument_progress_00545740((void *)0x6f9320u,(uint32_t)building,0);break;
  case 0xe6:progress=(uint32_t)FUN_00401540((uint32_t)building,0);break;
  case 0xea:progress=(uint32_t)restore_monument_progress_00558be0((void *)0x6f5648u,(uint32_t)building,0);break;
  case 0xeb:progress=(uint32_t)restore_monument_progress_00555e80((void *)0x6de9e0u,(uint32_t)building,0);break;
  case 0xec:progress=(uint32_t)restore_monument_progress_0054f470((void *)0x6e9188u,(uint32_t)building,0);break;
  default:continue;
  }
  scene=CS_U(0xeaac28u);
  if(progress==100){uint32_t kind=CS_U(0xea5fa0u+scene*0x49d8u+i*4u);bonus+=(uint32_t)(int32_t)(int16_t)CS_W(0x5d19d4u+kind*16u);}
 }
 return restore_campaign_score_numeric(scene*0x49d8u,bonus,months);
}
