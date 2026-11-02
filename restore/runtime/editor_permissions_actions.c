#include "editor_permissions_actions.h"
#include "selection_popup.h"
#include "map_validation.h"
#include "record_place.h"
#include "menu_dialog.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
/* Original40866D table4086E8:two targets; byte remap4086F0:32.
   Zero reaches408674 (test/append), one reaches4086A2 (next candidate). */
static const uint8_t primary_remap[32]={
 0,0,0,0,0,0,0,0,1,1,0,1,1,0,1,0,
 1,0,1,0,1,0,1,0,0,0,1,1,0,0,1,0
};
static int unused_or_current(uint32_t first,uint32_t slots,uint32_t slot,uint32_t value)
{
 uint32_t i;
 if(value==B(first+slot))return 1;
 for(i=0;i<slots;++i)if(B(first+i)==value)return 0;
 return 1;
}
/* Actual408610:primary14-byte record choices, original switch/remap domain. */
void restore_editor_permissions_00408610(void)
{
 uint32_t record=U(0x77591cu),base=record*98u,slot,count,cursor,value,target;
 if(B(0x77f749u+base)==0)return;
 slot=U(0xd3b198u);
 U(0x605990u)=slot;
 B(0x77f77bu+base)=10;
 U(0x5d2a2cu)=0;
 U(0x6057f4u)=0;
 count=1;cursor=0x6057f8u;
 for(value=0;value<36u;++value){
  uint32_t remap_index=value-1u;
  if(remap_index>31u||primary_remap[remap_index]!=0)continue;
  if(unused_or_current(0x77f766u+base,14,slot,value)){
   U(cursor)=value;++count;cursor+=4u;
  }
 }
 target=0x77f766u+base+slot;
 FUN_00403c00(200,150,(int32_t)count,23,0,target,0,0,0,1,1,0,1,(int32_t)(count/2u+1u),0);
}
/* The two sibling native bodies use separate14/8-byte arrays. The explicit
   type27 skip and the original1..35 candidate traversal remain unchanged. */
static void open_permission(uint32_t secondary)
{
 uint32_t record=U(0x77591cu),base=record*98u,slot,count,cursor,value,first,target,current;
 if(B(0x77f749u+base)==0)return;
 slot=U(0xd3b198u);
 U(0x605990u)=slot;
 U(0x6057f4u)=0;
 count=1;cursor=0x6057f8u;
 first=(secondary?0x77f77cu:0x77f766u)+base;
 for(value=1;value<36u;++value){
  if(value==27u)continue;
  if(unused_or_current(first,secondary?8u:14u,slot,value)){
   U(cursor)=value;++count;cursor+=4u;
  }
 }
 target=first+slot;
 B(0x77f77bu+base)=10;
 current=B(target);
 U(0x5d2a2cu)=0;
 U(0x605794u)=current;
 FUN_00403c00(200,0,(int32_t)count,23,0,target,0,0,0,1,1,
              secondary?0x408830u:0x408820u,1,18,0);
}
void restore_editor_permissions_00408840(void){open_permission(0);}
void restore_editor_permissions_00408920(void){open_permission(1);}

/* Entire444A40:BYTE type/special/state,98-byte stride. A negative native
   state skips its write; there is no invented upper bound or normalization. */
void restore_editor_permission_set_00444a40(uint32_t record,uint32_t kind,int32_t state)
{
 uint32_t base=record*98u;
 if(B(0x77f748u+base)!=1u)return;
 if(B(0x77f760u+base)==0)return;
 if(state<0)return;
 B(0x77f786u+base+kind)=(uint8_t)state;
}
/* Entire408710:actual post-selection permission maintenance. Old choice is
   reloaded after the search call before clearing its permission state. */
void restore_editor_permission_update_00408710(uint32_t secondary)
{
 uint32_t record=U(0x77591cu),base=record*98u,slot,current,old,found;
 uint8_t branch=(uint8_t)secondary;
 slot=U(0x605990u);
 current=B((branch?0x77f77cu:0x77f766u)+base+slot);
 old=U(0x605794u);
 if(old==0){
  if(FUN_00444a00(record,current)!=0)return;
  found=branch?FUN_00443000(record,current):FUN_00443040(record,current);
  if(found!=0)return;
  restore_editor_permission_set_00444a40(record,current,2);
  return;
 }
 if(current==old)return;
 found=branch?FUN_00443000(record,old):FUN_00443040(record,old);
 if(found==0){
  old=U(0x605794u);
  restore_editor_permission_set_00444a40(record,old,0);
 }
 if(FUN_00444a00(record,current)!=0)return;
 found=branch?FUN_00443000(record,current):FUN_00443040(record,current);
 if(found==0)restore_editor_permission_set_00444a40(record,current,2);
}
/* Genuine nullary four-instruction completion callbacks stored by403C00. */
void restore_editor_permissions_00408820(void){restore_editor_permission_update_00408710(0);}
void restore_editor_permissions_00408830(void){restore_editor_permission_update_00408710(1);}

static void reject_permission(uint32_t target,uint32_t message)
{
 W(target)=0;
 FUN_004264d0(message,0x51f780u,1);
 B(0xe38dcau)=1;
}
/* Entire408CE0:requirements use the accepted AL-return query. All rejected
   branches clear their exact WORD before calling the actual dialog provider. */
void restore_editor_permissions_00408ce0(void)
{
 uint32_t action=U(0xd3b198u),kind=U(0x5cf58cu+action*4u);
 uint32_t target=0x784ce0u+kind*2u;
 if(kind==22u){
  if(!FUN_00442870(17)){reject_permission(0x784d0cu,0x30);return;}
  goto toggle;
 }
 if(kind==14u){
  if(!FUN_00442870(15)){reject_permission(0x784cfcu,0x32);return;}
  goto toggle;
 }
 if(kind==16u||kind==17u){
  if(!FUN_00442870(23)){reject_permission(target,0x34);return;}
 }
 if(kind==41u){
  if(!FUN_00442870(29)){reject_permission(0x784d32u,0x3e);return;}
  goto toggle;
 }
 if(kind==42u){
  if(!FUN_00442870(20)){reject_permission(0x784d34u,0x42);return;}
  goto toggle;
 }
 if(kind==37u||kind==38u||kind==39u||kind==40u){
  if(W(0x784d28u)==0){reject_permission(target,0x46);return;}
  if(kind==37u){
   if(W(0x784d32u)==0&&!FUN_00442870(10)){
    reject_permission(0x784d2au,0x54);return;
   }
  }else if(kind==39u){
   if(W(0x784d34u)==0&&!FUN_00442870(28)){
    reject_permission(0x784d2eu,0x56);return;
   }
  }
 }
toggle:
 B(0xe38dcau)=1;
 W(target)=(uint16_t)(W(target)==0u);
}
/* Entire409150:two-state WORD choice, but values other than0/1 stay as-is. */
void restore_editor_permissions_00409150(void)
{
 uint32_t action=U(0xd3b198u),offset=action*2u;
 uint16_t current;
 if(W(0x7848e2u+offset)==0){W(0x784db0u+offset)=0;B(0xe38dcau)=1;return;}
 current=W(0x784db0u+offset);
 if(current==1u){W(0x784db0u+offset)=0;B(0xe38dcau)=1;return;}
 if(current==0u)W(0x784db0u+offset)=1;
 B(0xe38dcau)=1;
}
/* Entire4091B0:INC/store/reload a WORD, signed comparison to2, then a
   second reload for the dependent choice clear. Preserve native wrap. */
void restore_editor_permissions_004091b0(void)
{
 uint32_t action=U(0xd3b198u),offset=action*2u;
 uint16_t next=(uint16_t)(W(0x7848e2u+offset)+1u),observed;
 W(0x7848e2u+offset)=next;
 observed=W(0x7848e2u+offset);
 if((int16_t)observed>2)W(0x7848e2u+offset)=0;
 if(W(0x7848e2u+offset)==0)W(0x784db0u+offset)=0;
 B(0xe38dcau)=1;
}
int restore_editor_permissions_action(uint32_t token)
{
 switch(token){
 case 0x408610u:restore_editor_permissions_00408610();return 1;
 case 0x408840u:restore_editor_permissions_00408840();return 1;
 case 0x408920u:restore_editor_permissions_00408920();return 1;
 case 0x408ce0u:restore_editor_permissions_00408ce0();return 1;
 case 0x409150u:restore_editor_permissions_00409150();return 1;
 case 0x4091b0u:restore_editor_permissions_004091b0();return 1;
 default:return 0;
 }
}
int restore_editor_permissions_completion_action(uint32_t token)
{
 switch(token){
 case 0x408820u:restore_editor_permissions_00408820();return 1;
 case 0x408830u:restore_editor_permissions_00408830();return 1;
 default:return 0;
 }
}
