#include "actor_combat.h"
/* Original4b17d0: state5 melee tick, explicit actor argument; no receiver. */
void FUN_004b17d0(int32_t id)
{
 uint32_t a=cb_actor((uint32_t)id),t,af,tf;int32_t formation=CB_S(a+0x58u),target,target_formation,attack,defense,bonus,limit,damage,kind,count;uint8_t mode,tick,total;int16_t first;
 if((int8_t)CB_B(a+0x31u)<=5){CB_B(a+0x31u)++;FUN_004b3f00(id);}
 mode=CB_B(a+0x83u);
 if(!mode){FUN_004b1da0(id);return;}
 target=CB_S(a+0x88u);t=cb_actor((uint32_t)target);
 if(mode==1){if(CB_B(t+6u)!=1||CB_B(t+0x30u)==4){FUN_004b1da0(id);return;}}
 else if(mode==2&&(CB_B(t+6u)!=1||CB_B(t+0x30u)==4)){
  first=CB_S(a+0x84u);if(target==first)CB_W(a+0x88u)=CB_W(a+0x86u);else if(target==CB_S(a+0x86u))CB_W(a+0x88u)=(uint16_t)first;
  target=CB_S(a+0x88u);t=cb_actor((uint32_t)target);if(CB_B(t+6u)!=1||CB_B(t+0x30u)==4){FUN_004b1da0(id);return;}
  CB_B(a+0x83u)=1;CB_W(a+0x84u)=(uint16_t)target;CB_W(a+0x86u)=0;
 }
 tick=(uint8_t)(CB_B(a+0x62u)+1u);CB_B(a+0x62u)=tick;if(tick<24)return;
 target=CB_S(a+0x88u);t=cb_actor((uint32_t)target);target_formation=CB_S(t+0x58u);kind=CB_I(0x7090ccu+(uint32_t)CB_B(t+2u)*56u);CB_B(a+0x62u)=(uint8_t)(kind==1||kind==4?12:0);
 af=cb_formation(formation);bonus=CB_B(t+7u)==CB_U(0xeaac28u)?((int32_t)CB_S(af+0x7au)+10)/20:0;
 attack=CB_I(0x7090d4u+(uint32_t)CB_B(a+2u)*56u);defense=(int32_t)(CB_U(0x7090d8u+(uint32_t)CB_B(t+2u)*56u)+(uint32_t)bonus);if(defense>=20)defense=20;
 if(CB_S(t+0x88u)!=id&&FUN_004b1d10((int8_t)CB_B(a+11u),(int8_t)CB_B(t+11u))){
  kind=CB_S(af+4u);if(kind==12||kind==13){attack=(int32_t)((uint32_t)attack+4u);restore_actor_combat_sound(141);}
 }
 if(CB_W(af+0x5au)){
  kind=CB_S(af+4u);
  if(kind==12){if(FUN_004b1d10((int8_t)CB_B(a+11u),CB_B(af+0x6bu))){count=CB_S(af+0x2au);if(count>0){if(count<=2)attack=(int32_t)((uint32_t)attack+4u);else if(count==6)attack=(int32_t)((uint32_t)attack+2u);}}}
  else if(kind==11||kind==13){if(FUN_004b1d10((int8_t)CB_B(a+11u),CB_B(af+0x6bu))&&CB_S(af+0x2au)==6)attack=(int32_t)((uint32_t)attack+2u);}
 }
 tf=cb_formation(target_formation);
 if(CB_W(tf+0x5au)){
  kind=CB_S(tf+4u);
  if(kind==12){
   if(FUN_004b1d10((int8_t)CB_B(t+11u),CB_B(tf+0x6bu))){count=CB_S(tf+0x2au);if(count>0){if(count<=2)defense=(int32_t)((uint32_t)defense+4u);else if(count==6)defense=(int32_t)((uint32_t)defense-2u);}}
   else defense=(int32_t)((uint32_t)defense-4u);
  }else if(kind==11||kind==13){
   if(FUN_004b1d10((int8_t)CB_B(t+11u),CB_B(tf+0x6bu))){if(CB_S(tf+0x2au)==6)defense=(int32_t)((uint32_t)defense-2u);}else defense=(int32_t)((uint32_t)defense-4u);
  }
 }
 if(CB_S(af+4u)==57&&FUN_004b1d10((int8_t)CB_B(a+11u),CB_B(af+0x6bu))){count=CB_S(af+0x2au);if(count>0&&count<=2)attack=(int32_t)((uint32_t)attack+4u);}
 if(CB_S(tf+4u)==57&&FUN_004b1d10((int8_t)CB_B(t+11u),CB_B(tf+0x6bu))){count=CB_S(tf+0x2au);if(count>0&&count<=2)defense=(int32_t)((uint32_t)defense+4u);}
 limit=CB_I(0x7090d0u+(uint32_t)CB_B(t+2u)*56u);if(defense<0)defense=0;if(defense>20)defense=20;
 damage=attack?(int32_t)((20u-(uint32_t)defense)*(uint32_t)attack)/20:0;if(damage<0)damage=0;
 total=(uint8_t)(CB_B(t+0x15u)+(uint8_t)damage);CB_B(t+0x15u)=total;
 if((int32_t)total<=limit){FUN_004b5210(id);return;}
 if(!(uint8_t)FUN_004ea9c0(CB_B(t+2u))&&!(uint8_t)FUN_004ea9a0(CB_B(t+2u))&&CB_B(t+2u)!=23&&CB_B(a+7u)==CB_U(0xeaac28u)){
  bonus=(int32_t)CB_S(af+0x7au)+1;if(bonus>=100)bonus=100;CB_W(af+0x7au)=(uint16_t)bonus;
 }
 CB_B(t+0x30u)=4;CB_W(t+0x2eu)=0;FUN_004b5050(target);FUN_004b7ab0(target_formation);
}
