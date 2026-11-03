#include "building_release_model.h"
#include "building_cleanup.h"
#include "group_clear.h"
#include "monument_unlink.h"
#include "monument_clear_model.h"
#include "redraw_dispatch_model.h"
#include "menu_prepare_integer.h"
extern void FUN_004a36b0(int32_t);
extern void FUN_0047b590(int32_t,int32_t,int32_t,int32_t);
extern void FUN_0047bd50(int32_t,int32_t,int32_t,int32_t);
extern void FUN_0040f8f0(uint32_t *,uint32_t,uint8_t);
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))

void restore_building_release_links(int32_t building)
{
 uint32_t record=(uint32_t)building*264u,slot,player,peer,base;int32_t kind,x,y;uint16_t phase=0;
 slot=B(0x799a99u+record);
 if(slot){B(0x799a99u+record)=0;B(0x7900c0u+slot*196u)=0;}
 slot=B(0x799aa2u+record);
 if(slot){B(0x799aa2u+record)=0;U(0xeaaf68u+slot*224u)=0;}
 if(B(0x799aa3u+record)){FUN_0046a2e0(building);B(0x799aa3u+record)=0;}
 kind=H(0x7999e8u+record);
 switch(kind){
 case 50:case 95:case 187:case 188:case 189:
  player=U(0xeaac28u)*0x49d8u;
  base=kind==50?0xea5e68u:kind==95?0xea5d18u:0xea3ffcu;
  if(U(0x7999e4u+record)==U(base+player)){
   U(base+player)=0;W(base-4u+player)=0;W(base-2u+player)=0;
   if(kind==50||kind==95)U(base+8u+player)=0;else W(0xea5cf6u+player)=0;
  }
  return;
 case 57:FUN_004b82e0(H(0x799a28u+record));return;
 case 74:FUN_0046a2e0(building);return;
 case 77:case 78:case 79:U(0xea620cu+U(0xeaac28u)*0x49d8u)=0;return;
 case 94:U(0xea6240u+U(0xeaac28u)*0x49d8u)=0;return;
 case 136:
  peer=U(0x799a9cu+record);
  if((int32_t)peer>0){U(0x799a9cu+peer*264u)=UINT32_MAX;FUN_004a36b0(building);}
  return;
 case 138:case 215:
  y=H(0x7999e0u+record);x=H(0x7999deu+record);FUN_0047b590(x,y,x,y);
  y=H(0x7999e0u+record);x=H(0x7999deu+record);FUN_0047bd50(x,y,x,y);return;
 case 182:U(0xbcb9b0u)=U(0xbcb9b0u)-1u;return;
 case 209:
  player=U(0xeaac28u)*0x49d8u;U(0xea6010u+player)=UINT32_MAX;U(0xea6014u+player)=UINT32_MAX;
  W(0xea6084u+player)=0;U(0x775678u)=0;return;
 case 210:
  player=U(0xeaac28u)*0x49d8u;U(0xea6244u+player)=(uint32_t)H(0xea5baau+player);W(0xea5baau+player)=0;
  /* The Sun Temple continues through the common monument unlink. */
 case 183:case 213:case 222:case 225:case 227:FUN_004f4ae0(building);return;
 case 228:phase=1;break;
 case 229:phase=166;break;
 case 230:phase=15;break;
 case 234:phase=157;break;
 case 235:phase=436;break;
 case 236:phase=512;break;
 default:
  if(kind==207||kind==208){FUN_004e5830(building);return;}
  if(kind>=65&&kind<=69){
   player=U(0xeaac28u)*0x49d8u;W(0xea5ce8u+player)=0;W(0xea5cfau+player)=0;
   FUN_004c0920(FUN_00475b50((uint32_t)building));
   U(0xea5f9cu+U(0xeaac28u)*0x49d8u)=0;
  }
  return;
 }
 FUN_004f4ae0(building);
 if(W(0x7999eau+record)==phase){FUN_004dea50();B(0xe38dcau)=1;}
}

void restore_building_release(int32_t building)
{
 restore_building_release_links(building);
 FUN_0040f8f0((uint32_t *)(uintptr_t)(0x7999d8u+(uint32_t)building*264u),264u,0);
 if(U(0xbc9690u)==9u)FUN_00485200();
}
void FUN_0046ac50(int32_t building){restore_building_release_links(building);}
void FUN_0046ac10(int32_t building){restore_building_release(building);}
