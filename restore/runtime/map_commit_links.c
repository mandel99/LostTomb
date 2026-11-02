#include "map_commit_links.h"
#include "actor_ring_queue.h"
#include "menu_window_cursor_consumer.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
extern void FUN_0047db00(uint32_t);
extern int32_t FUN_00484f40(void);
uint32_t FUN_0047cbd0(int32_t x,int32_t y)
{
 uint32_t orientation=U(0xbca2e0u),cell,pattern,side,wanted,offset,matches;
 if(orientation==6)y=(int32_t)((uint32_t)y-1u);
 else if(orientation==4){x=(int32_t)((uint32_t)x-1u);y=(int32_t)((uint32_t)y-1u);}
 else if(orientation==2)x=(int32_t)((uint32_t)x-1u);
 cell=U(0xbca2d8u)+(uint32_t)y*228u+(uint32_t)x;
 for(pattern=0;pattern<4;++pattern){
  matches=0;
  for(side=0;side<4;++side){
   wanted=U(0x5dbda8u+(pattern*4u+side)*4u);offset=U(0x5cccc8u+side*4u);
   if((wanted!=0)==((B(0xaf598cu+(cell+offset)*4u)&4u)!=0))++matches;
  }
  if(matches==4)return pattern;
 }
 return 999;
}
uint32_t FUN_004806c0(int32_t x,int32_t y,uint32_t layout)
{
 uint32_t cell=U(0xbca2d8u)+(uint32_t)y*228u+(uint32_t)x,index,offset,flags,table=0x5cc9e0u+layout*96u,entry,px,py;
 for(index=0;index<24;++index){
  offset=U(table+index*4u);if(!offset)return 0;
  flags=U(0xaf598cu+(cell+offset)*4u);
  if((flags&4u)&&!(flags&8u)){
   U(0xbc9768u)=cell+offset;FUN_0047db00(4);
   if(FUN_00484f40()>=8){
    entry=(layout*24u+index)*8u;
    px=U(0x5cce98u+entry)+(uint32_t)x;py=U(0x5cce9cu+entry)+(uint32_t)y;
    U(0xd3b4e4u)=px;U(0xd3b4e0u)=py;return 1;
   }
  }
 }
 return 0;
}
void restore_ring_004f6560(uint32_t self)
{
 (void)restore_actor_ring_clear((void *)(uintptr_t)self);
}
uint32_t restore_ring_004f65f0(uint32_t self,uint32_t *index)
{
 return restore_actor_ring_next((const void *)(uintptr_t)self,index);
}
uint32_t restore_ring_004f6620(uint32_t self)
{
 return restore_actor_ring_pop((void *)(uintptr_t)self);
}
uint32_t restore_ring_004f6630(uint32_t self)
{
 return restore_actor_ring_count((const void *)(uintptr_t)self);
}
void FUN_004a3630(int32_t building)
{
 uint32_t group=B(0x799aa2u+(uint32_t)building*264u),self=0xeaaf68u+group*224u,iteration=0,actor,value;
 if(restore_ring_004f6630(self)){
  if((int32_t)restore_ring_004f6630(self)>0){
   do{
    actor=restore_ring_004f6620(self);value=B(0xbcd608u+actor*388u);B(0xbcd630u+actor*388u)=(uint8_t)value;
    FUN_004b3250((int32_t)actor);++iteration;
   }while((int32_t)iteration<(int32_t)restore_ring_004f6630(self));
  }
  restore_ring_004f6560(self);
 }
}
void FUN_004a36b0(int32_t building)
{
 int32_t linked=(int32_t)U(0x799a9cu+(uint32_t)building*264u);
 FUN_004a3630(building);if(linked>0)FUN_004a3630(linked);
}
