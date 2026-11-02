#include "building_cleanup.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
void FUN_0046a2e0(int32_t building)
{
 uint32_t group=B(0x799aa3u+(uint32_t)building*264u);
 if(group==0||group>10)return;
 group--;B(0x9783c0u+group*28u)=0;U(0x9783d8u+group*28u)=0;
}
/* FUN_00485200 is provided by map_commit_state.c. */
void FUN_004f4a80(uint32_t unused_building,int32_t slot)
{
 uint32_t player=U(0xeaac28u);(void)unused_building;
 U(0xea5facu+player*18904u+(uint32_t)slot*4u)=0;
}
void FUN_004b77d0(void)
{
 uint32_t player=U(0xeaac28u),counter=0xea5f08u+player*18904u,count=0,row,member,actor,record;
 uint16_t x,y,kind;uint8_t state;
 U(0xbcb9b8u)=0;U(0xbcb9b4u)=0;U(counter)=0;
 for(row=1;row<50u;++row){
  uint32_t group=0xbcb9c8u+row*144u;
  if(B(group)!=1||B(group+66u)==0)continue;
  count++;kind=W(group+4u);U(0xbcb9b8u)=row;
  if(kind==13)U(counter)=U(counter)+1u;
  if((int16_t)W(group+94u)>0)continue;
  for(member=0;member<16u;++member){
   actor=(uint32_t)(int32_t)(int16_t)W(group+8u+member*2u);
   if(actor==0)break;
   record=0xbcd5f8u+actor*388u;
   if(B(record+14u)==1){
    state=B(record+56u);
    if(state!=6&&state!=7&&state!=22&&state!=23){
     x=W(record+20u);y=W(record+22u);W(group+46u)=x;W(group+48u)=y;break;
    }
   }
  }
 }
 U(0xbcb9b4u)=count;
}
