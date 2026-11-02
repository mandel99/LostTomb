#include "actor_spawn.h"
#include "actor_create.h"
#include "actor_teardown_model.h"
#include "actor_callbacks_human.h"
extern uint32_t FUN_00562348(void);
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
static const uint8_t subtype[20]={3,1,3,2,1,3,2,3,2,1,3,3,2,3,3,3,1,2,0,1};
static const uint16_t delay[20]={10,50,100,130,200,250,400,430,500,600,70,750,820,830,900,980,1010,1030,1200,1300};
void FUN_004bbd40(int32_t x,int32_t y,int32_t enabled)
{
 uint32_t p,history,left,right,j,actor,record,selected,selected_record,phase;
 int32_t spawn_x,spawn_y,target_y,target_x,pixel_y,pixel_x;
 if(U(0x605a3cu)==0){
  for(p=0x784b74u;p<0x784b84u;p+=2u){
   spawn_x=(int32_t)(int16_t)W(p);
   if(spawn_x<=0)continue;
   history=U(0xd3b1c4u);U(0xd3b1c8u+history*4u)=U(0xd3b374u);
   history++;U(0xd3b1c4u)=history;if((int32_t)history>=100)U(0xd3b1c4u)=0;
   left=U(0xd3b384u);right=U(0xd3b380u);
   for(j=0;j<31u;++j){left=(left>>1)|((((left>>4)^left)&1u)<<30);right=(right>>1)|((((right>>4)^right)&1u)<<30);}
   U(0xd3b384u)=left;U(0xd3b380u)=right;U(0xd3b370u)=left&0x7fffu;U(0xd3b374u)=left&0x7fu;U(0xd3b378u)=left&7u;
   U(0xd3b36cu)=right&7u;U(0xd3b364u)=right&0x7fffu;
   spawn_y=(int32_t)(int16_t)W(p+16u);U(0xd3b368u)=right&0x7fu;
   actor=(uint32_t)FUN_004b4800(1,65,spawn_x,spawn_y,0);record=0xbcd5f8u+actor*388u;
   B(record+1u)=(uint8_t)(FUN_00562348()&31u);
   phase=FUN_00562348();selected=U(0xd056f8u);B(record+57u)=(uint8_t)(phase&7u);
   selected_record=0xbcd5f8u+selected*388u;
   target_y=(int32_t)(int16_t)W(selected_record+38u)*15;target_x=(int32_t)(int16_t)W(selected_record+36u)*15;
   pixel_y=(int32_t)(int16_t)W(selected_record+76u);pixel_x=(int32_t)(int16_t)W(selected_record+74u);
   FUN_004b4220((int32_t)selected,pixel_x,pixel_y,target_x,target_y,0);
  }
 }
 if(enabled==0)return;
 if(U(0x605a3cu)==0 && B(0x784cb1u)==0)return;
 for(actor=1;actor<2000u;++actor){record=0xbcd5f8u+actor*388u;if(B(record+14u)!=0 && B(record+10u)==37)FUN_004b4980((int32_t)actor);}
 for(j=0;j<20u;++j){
  actor=(uint32_t)FUN_004b4800(1,37,x,y,0);record=0xbcd5f8u+actor*388u;
  B(record+56u)=8;B(record+11u)=subtype[j];W(record+54u)=delay[j];
 }
}
