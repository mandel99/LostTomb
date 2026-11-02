#include "campaign_summary.h"
#include "campaign_profile.h"
#include "family_menu.h"
#include "text_selection.h"
#define SUMMARY_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define SUMMARY_W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define SUMMARY_B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
typedef struct CityWinner {int32_t value;uint32_t index;uint8_t tie;} CityWinner;
void FUN_004188f0(void)
{
 CityWinner best[3]={{-1,UINT32_MAX,0},{-1,UINT32_MAX,0},{-1,UINT32_MAX,0}};
 uint32_t i;char *path;
 SUMMARY_U(0x6fdcb8u)=SUMMARY_U(0xea5cc0u+SUMMARY_U(0xeaac28u)*0x49d8u);
 for(i=1;i<50;++i){
  uint32_t city=0xbcb9c8u+i*144u;
  if(SUMMARY_B(city)==1&&SUMMARY_B(city+0x42u)!=0){
   uint16_t type=SUMMARY_W(city+4u);int32_t value=(int16_t)SUMMARY_W(city+0x7au);
   uint32_t slot=type==12?0u:type==11?1u:2u;
   if(value>best[slot].value){best[slot].value=value;best[slot].tie=SUMMARY_B(city+0x28u);best[slot].index=i;}
   else if(value==best[slot].value){uint8_t tie=SUMMARY_B(city+0x28u);if(tie>best[slot].tie){best[slot].tie=tie;best[slot].index=i;}}
  }
 }
 for(i=0;i<3;++i){
  uint32_t out=0x6fdcbcu+i*12u;
  SUMMARY_U(out+4u)=(uint32_t)best[i].value;SUMMARY_B(out+8u)=best[i].tie;
  SUMMARY_B(out)=best[i].index==UINT32_MAX?255u:SUMMARY_B(0xbcb9cau+best[i].index*144u);
 }
 FUN_004cc110(0x133,(int32_t)(int8_t)SUMMARY_B(0xe38ea8u));
 if(SUMMARY_B(SUMMARY_U(0xe20768u)+6u)=='1'){
  for(i=1;i<4000;++i){
   uint32_t type_at=0x7999e8u+i*264u;
   if(SUMMARY_B(type_at-16u)==1){
    uint16_t type=SUMMARY_W(type_at);
    switch(type){
    case 0xde:
     {uint16_t first=SUMMARY_W(type_at+0x5cu),low,high;SUMMARY_W(0x70279au)=first;
      low=SUMMARY_W(type_at+0x58u);high=SUMMARY_W(type_at+0x60u);
      SUMMARY_W(0x702798u)=(uint16_t)(low|(uint16_t)((uint32_t)high<<13));}break;
    case 0xe1:
     {uint16_t x=SUMMARY_W(type_at-10u),y=SUMMARY_W(type_at-8u);
      SUMMARY_W(0x70279cu)=(uint16_t)(x-4u);SUMMARY_W(0x70279eu)=(uint16_t)(y+2u);}break;
    case 0xe4:
     {uint32_t offset=(i-(uint32_t)(int32_t)(int16_t)SUMMARY_W(type_at+2u))*264u;
      uint16_t x=SUMMARY_W(0x7999deu+offset),y=SUMMARY_W(0x7999e0u+offset);
      SUMMARY_W(0x7027a0u)=x;SUMMARY_W(0x7027a2u)=y;}break;
    case 0xe5:
     {uint16_t x=SUMMARY_W(type_at-10u),y=SUMMARY_W(type_at-8u);
      SUMMARY_W(0x70278cu)=(uint16_t)(x-5u);SUMMARY_W(0x70278eu)=(uint16_t)(y+6u);}break;
    case 0xea:
     {uint16_t x=SUMMARY_W(type_at-10u),y=SUMMARY_W(type_at-8u);
      SUMMARY_W(0x702790u)=(uint16_t)(x-6u);SUMMARY_W(0x702792u)=(uint16_t)(y+8u);}break;
    case 0xeb:
     {uint16_t x=SUMMARY_W(type_at-10u),y=SUMMARY_W(type_at-8u);
      SUMMARY_W(0x702794u)=(uint16_t)(x+1u);SUMMARY_W(0x702796u)=(uint16_t)(y+17u);}break;
    }
   }
  }
 }
 path=FUN_00516a20();(void)FUN_0041b8a0(RESTORE_CAMPAIGN_PROFILE,path);
}
