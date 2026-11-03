#include "house_levels.h"
#include "house_evolution.h"
#include "building_support.h"
#include "building_controllers.h"
#include <stdlib.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
extern void FUN_00476530(void);
static uint32_t current(void){return U(0x78d838u);}
void restore_house_update_by_entry(uint32_t native_entry)
{
 static const uint32_t entries[20]={
  0x452980u,0x452a20u,0x452b00u,0x452bd0u,0x452ca0u,
  0x452d70u,0x452e40u,0x452f10u,0x452fe0u,0x4530b0u,
  0x4531c0u,0x453280u,0x453340u,0x453400u,0x453500u,
  0x4535c0u,0x453680u,0x453740u,0x453840u,0x453900u};
 uint32_t i;
 for(i=0;i<20u;++i)if(entries[i]==native_entry){restore_house_update_level(i);return;}
 abort();
}
static void change(uint32_t building,uint32_t kind)
{FUN_0046cea0((int32_t)building,(int16_t)kind);}

void restore_house_update_level(uint32_t level)
{
 uint32_t building,decision,slot,delay,area,parcels;
 if(level>=20u)abort();
 if(level<10u){
  building=current();
  /* Vacant huts cannot initiate the neighbourhood merge. Higher levels
     retain their native merge call even when the population WORD is zero. */
  if(level<2u&&H(0x7999f4u+building*264u)<=0)return;
  restore_house_merge_neighbours(building);
 }
 if(restore_house_check_global_mode())return;
 decision=(uint32_t)restore_house_compare_desirability(current());U(0x78d7e4u)=decision;
 if(!restore_house_check_requirements(current(),0)){
  U(0x78d7e4u)=0xffffffffu;
  if(!level)return;
  decision=0xffffffffu;
 }else{
  decision=U(0x78d7e4u);
  if(level<19u&&decision==1u){
   decision=restore_house_check_requirements(current(),1);U(0x78d7e4u)=decision;
  }
 }
 if(!level){if(decision&&decision!=0xffffffffu)change(current(),11);return;}
 building=current();slot=0x799a5cu+building*264u;
 if(decision==0xffffffffu){
  delay=B(slot);
  if(delay<=1u){B(slot)=(uint8_t)(delay+1u);return;}
  B(slot)=0;
  switch(level){
  case 10:restore_house_reduce(building,0x46c1c0u);break;
  case 14:restore_house_reduce(building,0x46cb30u);break;
  case 18:restore_house_reduce(building,0x46ccf0u);break;
  default:change(building,level+9u);break;
  }
  return;
 }
 B(slot)=0;if(!decision)return;
 switch(level){
 case 9:case 13:case 17:
  area=level==9u?4u:level==13u?9u:16u;
  if(!restore_house_can_expand(building,area))return;
  building=current();
  if(level==9u)B(0x7999dcu+building*264u)=0;
  restore_house_clear_expansion(building,area);
  parcels=restore_house_prepare_expansion(current(),area);
  restore_house_expand(current(),level==9u?0x46bab0u:level==13u?0x46bbc0u:0x46bcb0u,parcels);
  U(0x78d588u)=1;FUN_00476530();return;
 case 19:
  /* The top callback redraws kind28 for an unexpected positive trend. */
  change(building,28);return;
 default:change(building,level+11u);return;
 }
}
#define LEVEL(token,index) \
void restore_building_control_C_##token(struct restore_building_control_frame *f) \
{restore_house_update_level(index);f->r[4]+=4u;}
LEVEL(00452980,0)
LEVEL(00452a20,1)
LEVEL(00452b00,2)
LEVEL(00452bd0,3)
LEVEL(00452ca0,4)
LEVEL(00452d70,5)
LEVEL(00452e40,6)
LEVEL(00452f10,7)
LEVEL(00452fe0,8)
LEVEL(004530b0,9)
LEVEL(004531c0,10)
LEVEL(00453280,11)
LEVEL(00453340,12)
LEVEL(00453400,13)
LEVEL(00453500,14)
LEVEL(004535c0,15)
LEVEL(00453680,16)
LEVEL(00453740,17)
LEVEL(00453840,18)
LEVEL(00453900,19)
