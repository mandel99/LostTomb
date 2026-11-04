#include "actor_command.h"
#include "actor_create.h"
#include "actor_combat.h"
#include "editor_validation.h"
#include "building_nearest_search.h"
#include "actor_routing.h"
#include <stdlib.h>
#include <string.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define SENTRY UINT32_C(0x41cd0a17)
typedef char actor_command_pointer32[sizeof(void*)==4?1:-1];
extern int32_t FUN_004eada0(int32_t kind);
extern uint8_t FUN_0051cd80(int32_t,int32_t,int32_t,int32_t);
extern void FUN_004b3250(int32_t actor);
extern void FUN_0053b4a0(int32_t actor,int32_t sound,int32_t variant);
extern void FUN_004b1d50(int32_t actor);
extern uint32_t FUN_004ec7c0(int32_t actor);
/* Audited whole native45-instruction leaf; low WORD kind/X/Y consumed. */
extern int32_t FUN_004b6ff0(uint16_t kind,int32_t mode,int32_t orientation,uint16_t x,uint16_t y);

/* These five bodies replace EC0F0/EC4A0/EC7C0/EC990 and the previously
   unidentified EAE20 callback. Address arithmetic deliberately wraps at 32 bits. */
__declspec(noinline) uint32_t restore_actor_find_other_kind_on_tile(uint32_t tile,uint32_t actor)
{
 int32_t candidate=(int16_t)W(0xb350dcu+tile*2u);
 while(candidate){
  uint32_t offset=(uint32_t)candidate*388u;
  if((uint32_t)candidate!=actor && B(0xbcd606u+offset) && B(0xbcd630u+offset)!=4u){
   uint32_t kind=B(0xbcd602u+offset);
   if(U(0x7090ccu+kind*56u) && B(0xbcd602u+actor*388u)!=kind &&
      !((uint32_t)FUN_004eada0((int32_t)kind)&255u))return (uint32_t)candidate;
  }
  candidate=(int16_t)W(0xbcd600u+offset);
 }
 return 0;
}

__declspec(noinline) uint32_t restore_actor_spiral_search(int32_t x,int32_t y,volatile int32_t *out_x,
 volatile int32_t *out_y,int32_t rings,uint32_t actor,restore_actor_tile_callback callback)
{
 uint32_t base=U(0xbca2d8u)+(uint32_t)y*228u+(uint32_t)x;
 uint32_t span=1,completed=0,remaining=(uint32_t)rings;
 static const uint32_t tile_step[4]={1u,228u,UINT32_MAX,0u-228u};
 *out_x=x;*out_y=y;
 for(;;){
  uint32_t side;
  U(0xbc9768u)=base;
  for(side=0;side<4;++side){
   uint32_t n;
   volatile int32_t *coordinate=(side&1u)?out_y:out_x;
   uint32_t coordinate_step=side<2?1u:UINT32_MAX;
   for(n=0;n<span;++n){
    /* Preserve short-circuit reads and aliases between outputs and map globals. */
    int32_t current_x=*out_x;
    if(current_x>=0){
     int32_t current_y=*out_y;
     if(current_y>=0 && current_x<(int32_t)U(0xbca2d0u) && current_y<(int32_t)U(0xbca2d4u)){
      uint32_t tile=U(0xbc9768u);
      if(!(U(0xaf598cu+tile*4u)&0x80000u)){
       uint32_t result=callback(tile,actor);
       if(result)return result;
      }
     }
    }
    U(0xbc9768u)=U(0xbc9768u)+tile_step[side];
    *coordinate=(int32_t)((uint32_t)*coordinate+coordinate_step);
   }
   U(0xbc9768u)=U(0xbc9768u)-tile_step[side];
   *coordinate=(int32_t)((uint32_t)*coordinate-coordinate_step);
  }
  if(!remaining)return 0;
  ++completed;span+=2u;base-=229u;
  *out_x=(int32_t)((uint32_t)*out_x-1u);
  *out_y=(int32_t)((uint32_t)*out_y-1u);
  --remaining;
  if(completed>=228u)return 0;
 }
}

struct command_random_pair {uint32_t first,second;};
static struct command_random_pair command_random_advance(uint32_t index,uint32_t sample)
{
 struct command_random_pair pair;uint32_t i;
 U(0xd3b1c8u+index*4u)=sample;
 ++index;U(0xd3b1c4u)=index;
 if((int32_t)index>=100)U(0xd3b1c4u)=0;
 pair.first=U(0xd3b384u);pair.second=U(0xd3b380u);
 for(i=0;i<31;++i){
  uint32_t first_feedback=(pair.first^(pair.first>>4))&1u;
  uint32_t second_feedback=(pair.second^(pair.second>>4))&1u;
  pair.first=(pair.first>>1)|(first_feedback<<30);
  pair.second=(pair.second>>1)|(second_feedback<<30);
 }
 return pair;
}
static void command_random_publish(struct command_random_pair pair,int movement)
{
 U(0xd3b384u)=pair.first;
 U(0xd3b378u)=pair.first&7u;
 U(0xd3b380u)=pair.second;
 U(0xd3b364u)=pair.second&32767u;
 if(movement){
  U(0xd3b36cu)=pair.second&7u;
  U(0xd3b370u)=pair.first&32767u;
  U(0xd3b374u)=pair.first&127u;
  U(0xd3b368u)=pair.second&127u;
 }else{
  U(0xd3b370u)=pair.first&32767u;
  U(0xd3b368u)=pair.second&127u;
  U(0xd3b36cu)=pair.second&7u;
  U(0xd3b374u)=pair.first&127u;
 }
}

__declspec(noinline) uint32_t restore_actor_apply_command_state(int32_t actor,uint32_t state)
{
 uint32_t offset=(uint32_t)actor*388u,result;
 B(0xbcd630u+offset)=(uint8_t)state;
 if(state==26u){
  U(0xbcd73cu+offset)=12;W(0xbcd62eu+offset)=20;U(0xbcd740u+offset)=0;return 0;
 }
 if(state==18u){
  uint32_t index=U(0xd3b1c4u),sample=U(0xd3b374u);
  struct command_random_pair pair=command_random_advance(index,sample);
  command_random_publish(pair,0);
  result=(pair.first&32767u)/100u;
  W(0xbcd62eu+offset)=(uint16_t)((pair.first&32767u)%100u+250u);
  U(0xbcd73cu+offset)=12;U(0xbcd740u+offset)=0;return result;
 }
 if(state==19u){
  uint32_t index=U(0xd3b1c4u),sample=U(0xd3b374u),attempt,direction;
  struct command_random_pair pair;
  U(0xbcd73cu+offset)=12;
  pair=command_random_advance(index,sample);command_random_publish(pair,1);
  direction=pair.first&7u;
  for(attempt=0;attempt<8;++attempt){
   uint32_t delta=U(0x5e4e0cu+direction*4u);
   uint32_t tile=U(0xbcd618u+offset);
   uint32_t flags=U(0xaf598cu+(delta+tile)*4u);
   if((flags&4u) && !(flags&0x80000u)){
    uint16_t dx=W(0x5e4e30u+direction*8u),dy=W(0x5e4e34u+direction*8u);
    dx=(uint16_t)(dx+W(0xbcd60cu+offset));W(0xbcd61cu+offset)=dx;
    dy=(uint16_t)(dy+W(0xbcd60eu+offset));W(0xbcd61eu+offset)=dy;
    U(0xbcd740u+offset)=0;return dy;
   }
   ++direction;if(direction>=8u)direction=0;
  }
  {
   /* The native output slots alias the incoming actor/state argument slots. */
   int32_t out_x=actor,out_y=(int32_t)state;
   int32_t y=(int16_t)W(0xbcd60eu+offset);
   int32_t x=(int16_t)W(0xbcd60cu+offset);
   result=(uint32_t)restore_building_nearest_search(x,y,0,NULL,4,&out_x,&out_y,5,actor);
   if(result){
    uint16_t target_x=(uint16_t)out_x,target_y=(uint16_t)out_y;
    W(0xbcd61cu+offset)=target_x;W(0xbcd61eu+offset)=target_y;
    result=(result&0xffff0000u)|target_y;
   }else{
    result=W(0xbcd60cu+offset);W(0xbcd61cu+offset)=(uint16_t)result;
    W(0xbcd61eu+offset)=W(0xbcd60eu+offset);
   }
  }
  U(0xbcd740u+offset)=0;return result;
 }
 result=restore_actor_seek_other_kind(actor);
 U(0xbcd73cu+offset)=7;W(0xbcd62eu+offset)=200;U(0xbcd740u+offset)=0;
 return result;
}

__declspec(noinline) uint32_t restore_actor_choose_command_state(int32_t actor)
{
 uint32_t weights[4],index,sample,roll,total=0,choice=0;
 struct command_random_pair pair;
 uint8_t state=B(0xbcd630u+(uint32_t)actor*388u);
 if(state==8u || state==25u){weights[0]=50;weights[1]=50;weights[2]=0;weights[3]=0;}
 else if(state==19u){weights[0]=75;weights[1]=5;weights[2]=20;weights[3]=0;}
 else if(state==26u){weights[0]=90;weights[1]=1;weights[2]=9;weights[3]=0;}
 else{weights[0]=29;weights[1]=70;weights[2]=0;weights[3]=1;}
 index=U(0xd3b1c4u);sample=U(0xd3b374u);
 pair=command_random_advance(index,sample);command_random_publish(pair,0);
 roll=(pair.first&32767u)%100u;
 do{total+=weights[choice];if(roll<total)break;++choice;}while(choice<4u);
 return FUN_004ec4a0(actor,U(0x5e4e70u+choice*4u));
}

__declspec(noinline) uint32_t restore_actor_seek_other_kind(int32_t actor)
{
 uint32_t offset=(uint32_t)actor*388u;
 uint32_t result=(uint32_t)FUN_004ea9c0(B(0xbcd602u+offset));
 uint8_t status,state;
 if(!(result&255u))return result;
 status=B(0xbcd606u+offset);result=(result&0xffffff00u)|status;
 if(!status)return result;
 state=B(0xbcd630u+offset);result=(result&0xffffff00u)|state;
 if(state==12u || state==4u)return result;
 {
  uint32_t scene=U(0x784cb0u);
  int32_t variant=(int16_t)W(0x784b94u);
  uint32_t radius=U(0x5e4cd4u+((uint32_t)variant+(scene&255u)*2u)*48u)<<1;
  int32_t y=(int16_t)W(0xbcd60eu+offset),x=(int16_t)W(0xbcd60cu+offset);
  int32_t out_x=0,out_y=actor;
  result=restore_actor_spiral_search(x,y,&out_x,&out_y,(int32_t)radius,(uint32_t)actor,restore_actor_find_other_kind_on_tile);
 }
 if(result){
  uint32_t target_offset=result*388u;
  W(0xbcd61cu+offset)=W(0xbcd60cu+target_offset);
  W(0xbcd61eu+offset)=W(0xbcd60eu+target_offset);
  W(0xbcd656u+offset)=(uint16_t)result;
  return result;
 }
 return restore_actor_choose_command_state(actor);
}

__declspec(noinline) uint32_t restore_actor_random_seed(uint32_t seed)
{U(0x604950u)=seed;return seed;}

__declspec(noinline) uint32_t restore_actor_try_periodic_target(int32_t actor)
{
 uint32_t offset=(uint32_t)actor*388u;
 uint32_t value=(uint32_t)FUN_004ea9c0(B(0xbcd602u+offset)),scene,table_offset,radius;
 uint8_t state,counter;
 if(!(value&255u))return value&0xffffff00u;
 if(!B(0xbcd606u+offset))return value&0xffffff00u;
 state=B(0xbcd630u+offset);
 if(state==12u || state==4u || state==5u || state==21u)return value&0xffffff00u;
 counter=B(0xbcd663u+offset);scene=U(0x784cb0u);
 ++counter;B(0xbcd663u+offset)=counter;
 table_offset=((uint32_t)(int32_t)(int16_t)W(0x784b94u)+(scene&255u)*2u)*48u;
 if((int32_t)counter<(int32_t)U(0x5e4cd0u+table_offset))return table_offset&0xffffff00u;
 radius=U(0x5e4cd4u+table_offset);B(0xbcd663u+offset)=0;
 FUN_0056233e(timeGetTime());
 {
  int32_t y=(int16_t)W(0xbcd60eu+offset),x=(int16_t)W(0xbcd60cu+offset);
  int32_t out_x=0,out_y=actor;
  value=FUN_004ec0f0(x,y,&out_x,&out_y,(int32_t)radius,(uint32_t)actor,restore_actor_find_other_kind_on_tile);
 }
 if(!value)return 0;
 W(0xbcd61cu+offset)=W(0xbcd60cu+value*388u);
 W(0xbcd61eu+offset)=W(0xbcd60eu+value*388u);
 B(0xbcd630u+offset)=21;
 W(0xbcd62eu+offset)=(uint16_t)(radius*18u);
 W(0xbcd656u+offset)=(uint16_t)value;
 return (value&0xffffff00u)|1u;
}

/* Native EBC90 caches the tile head once and the requested kind once per
   chain. Terrain fallback exists only on its east segment. */
static uint8_t actor_state_search_point(int32_t count,const uint32_t *types,uint32_t mask,
 volatile int32_t *out_x,volatile int32_t *out_y,int32_t excluded,uint32_t state,
 uint32_t east,uint32_t *result)
{
 int32_t x=*out_x,y;uint32_t i;
 if(x<0)return 0;
 y=*out_y;if(y<0 || x>=(int32_t)U(0xbca2d0u) || y>=(int32_t)U(0xbca2d4u))return 0;
 if(count>0){
  uint32_t head=(uint32_t)(int32_t)(int16_t)W(0xb350dcu+U(0xbc9768u)*2u);
  for(i=0;(int32_t)i<count;++i){
   uint32_t candidate=head;
   if(candidate){
    uint32_t kind=U((uint32_t)(uintptr_t)types+i*4u);
    do{
     uint32_t offset=candidate*388u;
     if((uint32_t)B(0xbcd602u+offset)==kind && candidate!=(uint32_t)excluded &&
        (uint32_t)B(0xbcd630u+offset)==state){*result=candidate;return 1;}
     candidate=(uint32_t)(int32_t)(int16_t)W(0xbcd600u+offset);
    }while(candidate);
   }
  }
 }
 if(east){
  uint32_t flags=U(0xaf598cu+U(0xbc9768u)*4u);
  if(!(flags&0x80000u) && (mask&flags)){*result=(uint32_t)count+1u;return 1;}
 }
 return 0;
}

__declspec(noinline) int32_t restore_actor_find_state_nearby(int32_t x,int32_t y,int32_t count,
 const uint32_t *types,uint32_t mask,volatile int32_t *out_x,volatile int32_t *out_y,
 int32_t rings,int32_t excluded,uint32_t state)
{
 uint32_t base=U(0xbca2d8u)+(uint32_t)y*228u+(uint32_t)x;
 uint32_t span=1,completed=0,remaining=(uint32_t)rings;
 static const uint32_t tile_step[4]={1u,228u,UINT32_MAX,0u-228u};
 *out_x=x;*out_y=y;
 for(;;){
  uint32_t side;
  U(0xbc9768u)=base;
  for(side=0;side<4u;++side){
   uint32_t n,result,coordinate_step=side<2u?1u:UINT32_MAX;
   volatile int32_t *coordinate=(side&1u)?out_y:out_x;
   for(n=0;n<span;++n){
    if(actor_state_search_point(count,types,mask,out_x,out_y,excluded,state,side==0,&result))return (int32_t)result;
    U(0xbc9768u)=U(0xbc9768u)+tile_step[side];
    *coordinate=(int32_t)((uint32_t)*coordinate+coordinate_step);
   }
   if(side==1u){
    uint32_t tile=U(0xbc9768u),legacy_y=U(0xd3b4e0u);
    /* This absolute global store replaces an output-Y correction natively. */
    U(0xbc9768u)=tile-228u;U(0xd3b4e0u)=legacy_y-1u;
   }else{
    U(0xbc9768u)=U(0xbc9768u)-tile_step[side];
    *coordinate=(int32_t)((uint32_t)*coordinate-coordinate_step);
   }
  }
  if(!remaining)return 0;
  ++completed;span+=2u;base-=229u;
  *out_x=(int32_t)((uint32_t)*out_x-1u);
  *out_y=(int32_t)((uint32_t)*out_y-1u);
  --remaining;
  if(completed>=228u)return 0;
 }
}

__declspec(noinline) uint8_t restore_actor_choose_clear_step(int32_t x,int32_t y,
 int32_t from_x,int32_t from_y,volatile int32_t *out_x,volatile int32_t *out_y)
{
 static const int32_t dy[8]={-1,-1,0,1,1,1,0,-1};
 static const int32_t dx[8]={0,-1,-1,-1,0,1,1,1};
 static const int32_t probe[8]={-228,-227,1,229,228,227,-1,-229};
 uint32_t direction=FUN_0051cd80(from_x,from_y,x,y),stop=direction-1u;
 uint32_t base=U(0xbca2d8u)+(uint32_t)y*228u+(uint32_t)x;
 if((int32_t)stop<0)stop+=8u;
 /* The actual direction provider yields 0..7. The previous direction is
    deliberately excluded: seven candidates, distinct probe/output tables. */
 while(direction!=stop){
  uint32_t tile=base+(uint32_t)probe[direction];
  if(!B(0xac2d4cu+tile)){
   uint32_t flags=U(0xaf598cu+tile*4u);
   if(!(flags&0x80000u) && (FUN_004ec410(flags,1)&255u)){
    int32_t target_y=(int32_t)((uint32_t)y+(uint32_t)dy[direction]);
    *out_x=(int32_t)((uint32_t)x+(uint32_t)dx[direction]);
    *out_y=target_y;
    if(FUN_005181c0(x,y,*out_x,target_y))return 1;
   }
  }
  ++direction;if((int32_t)direction>=8)direction-=8u;
 }
 return 0;
}

static struct command_random_pair actor_group_step_pair(struct command_random_pair pair)
{
 uint32_t i;
 for(i=0;i<31u;++i){
  uint32_t a=(pair.first^(pair.first>>4))&1u,b=(pair.second^(pair.second>>4))&1u;
  pair.first=(pair.first>>1)|(a<<30);pair.second=(pair.second>>1)|(b<<30);
 }
 return pair;
}

__declspec(noinline) uint8_t restore_actor_update_group_target(int32_t actor)
{
 uint32_t offset=(uint32_t)actor*388u,found;uint8_t state;
 int32_t out_x=0,out_y=0,x,y;
 if(!((uint32_t)FUN_004ea9a0(B(0xbcd602u+offset))&255u))return 0;
 if(!B(0xbcd606u+offset))return 0;
 state=B(0xbcd630u+offset);
 if(state==12u || state==4u)return 0;
 if(state==16u || state==17u)return 1;
 FUN_0056233e(timeGetTime());
 y=(int16_t)W(0xbcd60eu+offset);x=(int16_t)W(0xbcd60cu+offset);
 found=(uint32_t)FUN_004ebc90(x,y,3,(const uint32_t *)(uintptr_t)0x5e4df0u,0,&out_x,&out_y,1,actor,16);
 if(found){
  FUN_004b3250(actor);
  state=B(0xbcd630u+offset);
  if(state!=14u && state!=15u){B(0xbcd630u+offset)=15;B(0xbcd68au+offset)=0;return 1;}
  {
   uint32_t index=U(0xd3b1c4u),sample=U(0xd3b374u),target=found*388u;
   struct command_random_pair pair;
   uint16_t target_x,target_y;
   B(0xbcd630u+offset)=16;
   pair=command_random_advance(index,sample);
   target_x=(uint16_t)(W(0xbcd61cu+target)+(pair.first&3u)-2u);
   index=U(0xd3b1c4u);U(0xd3b1c8u+index*4u)=pair.first&127u;
   ++index;W(0xbcd61cu+offset)=target_x;U(0xd3b1c4u)=index;
   if((int32_t)index>=100)U(0xd3b1c4u)=0;
   /* The second advance continues the unpublished local pair, even when
      that history/target write aliases one of the global random states. */
   pair=actor_group_step_pair(pair);
   U(0xd3b384u)=pair.first;U(0xd3b378u)=pair.first&7u;
   U(0xd3b380u)=pair.second;U(0xd3b364u)=pair.second&32767u;
   U(0xd3b370u)=pair.first&32767u;U(0xd3b36cu)=pair.second&7u;
   target_y=(uint16_t)(W(0xbcd61eu+target)+(pair.first&3u)-2u);
   W(0xbcd61eu+offset)=target_y;
   U(0xd3b374u)=pair.first&127u;U(0xd3b368u)=pair.second&127u;
   return 1;
  }
 }
 state=B(0xbcd630u+offset);
 if(state!=14u){
  y=(int16_t)W(0xbcd60eu+offset);x=(int16_t)W(0xbcd60cu+offset);
  if(FUN_004ebc90(x,y,3,(const uint32_t *)(uintptr_t)0x5e4df0u,0,&out_x,&out_y,3,actor,14)){
   B(0xbcd630u+offset)=15;B(0xbcd68au+offset)=0;return 1;
  }
  y=(int16_t)W(0xbcd60eu+offset);x=(int16_t)W(0xbcd60cu+offset);
  if(restore_building_nearest_search(x,y,4,(const uint32_t *)(uintptr_t)0x5e4dd0u,0,&out_x,&out_y,6,actor)){
   B(0xbcd630u+offset)=14;B(0xbcd68au+offset)=0;return 1;
  }
  return 0;
 }
 y=(int16_t)W(0xbcd60eu+offset);x=(int16_t)W(0xbcd60cu+offset);
 found=(uint32_t)restore_building_nearest_search(x,y,4,(const uint32_t *)(uintptr_t)0x5e4dd0u,0,&out_x,&out_y,2,actor);
 if(found){
  uint32_t index=U(0xd3b1c4u),sample=U(0xd3b374u);
  struct command_random_pair pair=command_random_advance(index,sample);
  U(0xd3b384u)=pair.first;U(0xd3b378u)=pair.first&7u;
  U(0xd3b380u)=pair.second;U(0xd3b364u)=pair.second&32767u;
  U(0xd3b370u)=pair.first&32767u;U(0xd3b374u)=pair.first&127u;
  U(0xd3b368u)=pair.second&127u;U(0xd3b36cu)=pair.second&7u;
  if(!(pair.first&3u)){
   int32_t target_y=(int16_t)W(0xbcd60eu+found*388u);
   int32_t target_x=(int16_t)W(0xbcd60cu+found*388u);
   int32_t current_y=(int16_t)W(0xbcd60eu+offset),current_x=(int16_t)W(0xbcd60cu+offset);
   int32_t step_x=0,step_y=0;
   if(FUN_004eabf0(current_x,current_y,target_x,target_y,&step_x,&step_y)){
    W(0xbcd61cu+offset)=(uint16_t)step_x;W(0xbcd61eu+offset)=(uint16_t)step_y;
    B(0xbcd630u+offset)=16;return 1;
   }
  }
 }
 state=B(0xbcd630u+offset);
 if(state==14u || state==15u){
  uint8_t progress=B(0xbcd68au+offset);++progress;B(0xbcd68au+offset)=progress;
  if(progress>20u){FUN_004b3250(actor);B(0xbcd630u+offset)=8;}
 }
 return 0;
}

/* Kind84 update and its16-vector route helper. The callback invoker
   overwrites scratch results through its profiler before meaningful use. */
__declspec(noinline) uint32_t restore_actor_route_line_mode(uint32_t mode)
{
    uint32_t offset, table;
    if (mode & 255u) {
        uint32_t scene = U(0x784cb0u);
        offset = (scene & 255u) * 48u;
        table = 0x5e4c3cu;
    } else {
        uint32_t scene = U(0x784cb0u);
        int32_t variant = (int16_t)W(0x784b94u);
        offset = ((uint32_t)variant + (scene & 255u) * 2u) * 48u;
        table = 0x5e4cccu;
    }
    /* MOV AL,[table+offset] replaces only the low byte of the offset. */
    return (offset & 0xffffff00u) | B(table + offset);
}

__declspec(noinline) int32_t restore_actor_find_clear_vector(int32_t x, int32_t y,
    int32_t first_vector, int32_t target_x, int32_t target_y, uint32_t mode)
{
    uint32_t vector = (uint32_t)first_vector, attempt;
    for (attempt = 0; attempt < 16u; ++attempt) {
        uint32_t dy = U(0x5cdb7cu + vector * 8u);
        uint32_t dx = U(0x5cdb78u + vector * 8u);
        uint32_t candidate_y = dy + (uint32_t)y;
        uint32_t map_base = U(0xbca2d8u);
        uint32_t candidate_x = dx + (uint32_t)x;
        uint32_t tile = map_base + candidate_y * 228u + candidate_x;
        uint32_t line_clear;

        /* Coordinates and tile are cached before the selector/line children.
           The selector and terrain predicate use AL; the line tests use EAX. */
        if (FUN_004eaac0((int32_t)mode)) {
            line_clear = FUN_00519be0(target_x, target_y,
                (int32_t)candidate_x, (int32_t)candidate_y);
            if (!line_clear)
                line_clear = FUN_00517f20(target_x, target_y,
                    (int32_t)candidate_x, (int32_t)candidate_y);
        } else {
            line_clear = FUN_005181c0(target_x, target_y,
                (int32_t)candidate_x, (int32_t)candidate_y);
        }
        if (line_clear && !W(0xb350dcu + tile * 2u)) {
            uint32_t terrain = U(0xaf598cu + tile * 4u);
            if (FUN_004ec410(terrain, (char)mode) & 255u)
                return (int32_t)vector;
        }
        ++vector;
        if ((int32_t)vector >= 16)
            vector = 0;
    }
    return -1;
}

struct hippo_random_pair {uint32_t a,b;};

static uint32_t hippo_offset(uint32_t actor){return actor*388u;}
static uint32_t hippo_half(uint32_t value){return (uint32_t)((int32_t)value>>1);}
static struct hippo_random_pair hippo_random_advance(void)
{
    uint32_t index=U(0xd3b1c4u),sample=U(0xd3b374u),i;
    struct hippo_random_pair p;
    U(0xd3b1c8u+index*4u)=sample;
    ++index;U(0xd3b1c4u)=index;
    if((int32_t)index>=100)U(0xd3b1c4u)=0;
    p.a=U(0xd3b384u);p.b=U(0xd3b380u);
    for(i=0;i<31u;++i){
        uint32_t a=(p.a^(p.a>>4))&1u,b=(p.b^(p.b>>4))&1u;
        p.a=(p.a>>1)|(a<<30);p.b=(p.b>>1)|(b<<30);
    }
    return p;
}

static uint8_t hippo_target_differs(uint32_t actor_offset,uint32_t linked_offset)
{
    uint16_t linked_x=W(0xbcd60cu+linked_offset),target_x=W(0xbcd61cu+actor_offset);
    uint16_t linked_y,target_y;
    if(linked_x!=target_x)return 1;
    linked_y=W(0xbcd60eu+linked_offset);target_y=W(0xbcd61eu+actor_offset);
    return linked_y!=target_y;
}

/* Caller clears the route first. Each load/store remains fresh and ordered. */
static uint32_t hippo_copy_linked_target(uint32_t actor,uint32_t linked_offset)
{
    uint16_t x=W(0xbcd60cu+linked_offset);
    uint32_t o=hippo_offset(actor);
    W(0xbcd61cu+o)=x;
    W(0xbcd61eu+o)=W(0xbcd60eu+linked_offset);
    return o;
}

static uint32_t hippo_copy_group_target(uint32_t actor,uint32_t group_offset,
                                       uint32_t direction,uint32_t group_x,uint32_t group_y)
{
    uint16_t x=W(0x5cd978u+direction*8u);
    uint32_t o=hippo_offset(actor);
    x=(uint16_t)(x+W(group_x+group_offset));W(0xbcd61cu+o)=x;
    x=W(0x5cd97cu+direction*8u);
    x=(uint16_t)(x+W(group_y+group_offset));W(0xbcd61eu+o)=x;
    B(0xbcd630u+o)=9;
    return o;
}

static uint8_t hippo_surrounded_by_flag4(uint32_t tile)
{
    uint32_t p=0xaf598cu+tile*4u;
    return (B(p)&4u) && (B(p-912u)&4u) && (B(p+4u)&4u) &&
           (B(p+912u)&4u) && (B(p-4u)&4u) && (B(p-908u)&4u) &&
           (B(p-916u)&4u) && (B(p+916u)&4u) && (B(p+908u)&4u);
}

static uint32_t hippo_animation(uint32_t actor,uint8_t surrounded)
{
    uint32_t o=hippo_offset(actor),base,counter,direction,rotation;
    uint8_t state;
    if(B(0x7027e1u)){
        state=B(0xbcd630u+o);
        if(state!=4u && state!=12u){
            base=U(0xf56ea4u);U(0xbcd73cu+o)=14;
            counter=U(0xbcd740u+o);U(0xbcd5fcu+o)=counter+base+5u;
        }else{
            int32_t delay;
            U(0xbcd73cu+o)=18;delay=(int16_t)W(0xbcd62eu+o);
            base=U(0xf56ea4u);
            U(0xbcd5fcu+o)=base+(delay>=18?36u:(uint32_t)delay+19u);
        }
        return o;
    }
    rotation=U(0xbca2e0u);
    direction=(uint32_t)(int32_t)(int8_t)B(0xbcd609u+o)-rotation;
    U(0xe94e00u)=direction;
    if((int32_t)direction<0){direction+=8u;U(0xe94e00u)=direction;}
    state=B(0xbcd630u+o);
    switch(state){
    case 4:case 12:{
        int32_t delay=(int16_t)W(0xbcd62eu+o);
        if(surrounded){base=U(0xf56e14u);counter=delay>=23?22u:(uint32_t)delay;}
        else{base=U(0xf56e74u);counter=delay>=8?7u:(uint32_t)delay;}
        U(0xbcd5fcu+o)=base+counter;return o;
    }
    case 9:case 10:case 19:case 21:
        counter=U(0xbcd740u+o);base=U(surrounded?0xf56e84u:0xf56e6cu);
        U(0xbcd5fcu+o)=base+counter*8u+direction;return o;
    case 25:
        counter=U(0xbcd740u+o);base=U(surrounded?0xf56e84u:0xf56e7cu);
        U(0xbcd5fcu+o)=base+counter*8u+direction;return o;
    case 8:case 23:
        base=U(surrounded?0xf56e94u:0xf56e6cu);
        U(0xbcd5fcu+o)=base+direction;return o;
    case 26:
        if(surrounded){counter=U(0xbcd740u+o);base=U(0xf56e8cu);U(0xbcd5fcu+o)=base+counter*8u+direction;}
        else{base=U(0xf56e6cu);U(0xbcd5fcu+o)=base+direction;}
        return o;
    default:
        base=U(0xf56e9cu);counter=U(0xbcd740u+o);
        if(surrounded)base=U(0xf56e94u);
        U(0xbcd5fcu+o)=base+counter*8u+direction;return o;
    }
}

__declspec(noinline) uint32_t restore_actor_update_hippo(void)
{
    uint32_t actor=U(0xd056f8u),o=hippo_offset(actor),kind=B(0xbcd602u+o);
    uint32_t counter=U(0xbcd740u+o)+1u,speed,cycle,tile;
    int32_t group,linked=0;
    uint8_t cycle_advanced=0,surrounded,state,animation_byte,direction;
    U(0xbcd740u+o)=counter;
    speed=U(0x7090ecu+kind*56u);cycle=U(0xbcd73cu+o);
    if((int32_t)counter>=(int32_t)cycle){
        U(0xbcd740u+o)=0;animation_byte=B(0xbcd5f9u+o);
        ++animation_byte;cycle_advanced=1;B(0xbcd5f9u+o)=animation_byte;
    }
    B(0xbcd66au+o)=6;B(0xbcd604u+o)=0;B(0xbcd65du+o)=0;
    tile=U(0xbcd618u+o);group=(int16_t)W(0xbcd658u+o);
    surrounded=hippo_surrounded_by_flag4(tile);
    {uint32_t profile=U(0xeaac28u)*0x49d8u;W(0xea5ceau+profile)=(uint16_t)(W(0xea5ceau+profile)+1u);}
    animation_byte=B(0xbcd5f9u+o);++animation_byte;B(0xbcd5f9u+o)=animation_byte;
    if(animation_byte>=12u)B(0xbcd5f9u+o)=0;
    state=B(0xbcd630u+o);
    switch(state){
    case 5:
        FUN_0053b4a0((int32_t)actor,6,2);actor=U(0xd056f8u);
        FUN_004b17d0((int32_t)actor);actor=U(0xd056f8u);break;
    case 4:
        if(!W(0xbcd62eu+o)){FUN_004b5050((int32_t)actor);actor=U(0xd056f8u);}
        o=hippo_offset(actor);U(0xbcd73cu+o)=8;
        FUN_004b1d50((int32_t)actor);actor=U(0xd056f8u);break;
    case 24:
        W(0xbcd62eu+o)=(uint16_t)(W(0xbcd62eu+o)-1u);
        if((int16_t)W(0xbcd62eu+o)<=0){U(0xbcd740u+o)=0;U(0xbcd73cu+o)=8;B(0xbcd630u+o)=8;}
        U(0xbcd5fcu+o)=0;return o;
    case 26:
        cycle=hippo_half(U(0xbcd73cu+o));counter=U(0xbcd740u+o);
        if(counter==cycle){
            W(0xbcd62eu+o)=(uint16_t)(W(0xbcd62eu+o)-1u);
            if((int16_t)W(0xbcd62eu+o)>0)U(0xbcd740u+o)=hippo_half(U(0xbcd73cu+o))-1u;
        }else if(cycle_advanced){FUN_004ec7c0((int32_t)actor);actor=U(0xd056f8u);}
        break;
    case 8:
        FUN_004ec7c0((int32_t)actor);actor=U(0xd056f8u);break;
    case 9:case 10:case 19:
        if(surrounded)speed=hippo_half(speed);
        FUN_004a5da0((int32_t)actor,speed);actor=U(0xd056f8u);o=hippo_offset(actor);
        direction=B(0xbcd609u+o);
        if(direction==9u){FUN_004b3250((int32_t)actor);actor=U(0xd056f8u);}
        else if(direction==10u || direction==8u){
            B(0xbcd609u+o)=B(0xbcd60au+o);
            FUN_004ec7c0((int32_t)actor);actor=U(0xd056f8u);
        }
        break;
    case 18:
        W(0xbcd62eu+o)=(uint16_t)(W(0xbcd62eu+o)-1u);
        if((int16_t)W(0xbcd62eu+o)<=0){
            struct hippo_random_pair p=hippo_random_advance();
            uint32_t g=(uint32_t)group*144u;int32_t x,y,gx,gy,index;
            U(0xd3b384u)=p.a;U(0xd3b378u)=p.a&7u;
            U(0xd3b380u)=p.b;U(0xd3b364u)=p.b&32767u;
            U(0xd3b374u)=p.a&127u;U(0xd3b36cu)=p.b&7u;
            y=(int16_t)W(0xbcd60eu+o);U(0xd3b368u)=p.b&127u;
            x=(int16_t)W(0xbcd60cu+o);U(0xd3b370u)=p.a&32767u;
            gy=(int16_t)W(0xbcba04u+g);gx=(int16_t)W(0xbcba02u+g);
            index=FUN_004eab10(gx,gy,(int32_t)(p.a&15u),x,y,0);
            if(index>=0 && index<16){
                actor=U(0xd056f8u);FUN_004b3250((int32_t)actor);actor=U(0xd056f8u);
                hippo_copy_group_target(actor,g,(uint32_t)index,0xbcba02u,0xbcba04u);
                restore_actor_try_periodic_target((int32_t)actor);actor=U(0xd056f8u);break;
            }
            actor=U(0xd056f8u);o=hippo_offset(actor);W(0xbcd62eu+o)=100;
        }
        /* State20 has the same periodic-call tail. */
        restore_actor_try_periodic_target((int32_t)actor);actor=U(0xd056f8u);break;
    case 20:
        restore_actor_try_periodic_target((int32_t)actor);actor=U(0xd056f8u);break;
    case 23:{
        struct hippo_random_pair p=hippo_random_advance();
        uint32_t g=(uint32_t)group*144u;int32_t x,y,gx,gy,index;
        U(0xd3b384u)=p.a;U(0xd3b378u)=p.a&7u;
        U(0xd3b380u)=p.b;U(0xd3b364u)=p.b&32767u;
        U(0xd3b374u)=p.a&127u;U(0xd3b36cu)=p.b&7u;
        y=(int16_t)W(0xbcd60eu+o);U(0xd3b368u)=p.b&127u;
        x=(int16_t)W(0xbcd60cu+o);U(0xd3b370u)=p.a&32767u;
        gy=(int16_t)W(0xbcba04u+g);gx=(int16_t)W(0xbcba02u+g);
        index=FUN_004eab10(gx,gy,(int32_t)(p.a&15u),x,y,0);
        if(index>=0 && index<16){
            actor=U(0xd056f8u);FUN_004b3250((int32_t)actor);actor=U(0xd056f8u);
            hippo_copy_group_target(actor,g,(uint32_t)index,0xbcba02u,0xbcba04u);break;
        }
        p=hippo_random_advance();
        U(0xd3b384u)=p.a;U(0xd3b378u)=p.a&7u;
        U(0xd3b380u)=p.b;U(0xd3b364u)=p.b&32767u;
        U(0xd3b368u)=p.b&127u;actor=U(0xd056f8u);
        U(0xd3b36cu)=p.b&7u;U(0xd3b370u)=p.a&32767u;
        o=hippo_offset(actor);y=(int16_t)W(0xbcd60eu+o);x=(int16_t)W(0xbcd60cu+o);
        gy=(int16_t)W(0xbcba00u+g);gx=(int16_t)W(0xbcb9feu+g);
        U(0xd3b374u)=p.a&127u;
        index=FUN_004eab10(gx,gy,(int32_t)(p.a&15u),x,y,0);
        if(index>=0 && index<16){
            actor=U(0xd056f8u);FUN_004b3250((int32_t)actor);actor=U(0xd056f8u);
            hippo_copy_group_target(actor,g,(uint32_t)index,0xbcb9feu,0xbcba00u);break;
        }
        actor=U(0xd056f8u);B(0xbcd606u+hippo_offset(actor))=2;return actor;
    }
    case 25:
        linked=(int16_t)W(0xbcd656u+o);
        W(0xbcd62eu+o)=(uint16_t)(W(0xbcd62eu+o)-1u);
        if((int16_t)W(0xbcd62eu+o)<=0){B(0xbcd663u+o)=0;B(0xbcd630u+o)=23;break;}
        if(linked && B(0xbcd630u+hippo_offset((uint32_t)linked))!=4u && B(0xbcd606u+hippo_offset((uint32_t)linked))==1u){
            if(cycle_advanced && hippo_target_differs(o,hippo_offset((uint32_t)linked))){
                FUN_004b3250((int32_t)actor);actor=U(0xd056f8u);
                o=hippo_copy_linked_target(actor,hippo_offset((uint32_t)linked));
            }
        }else{
            B(0xbcd609u+o)=B(0xbcd60au+o);restore_actor_seek_other_kind((int32_t)actor);actor=U(0xd056f8u);
        }
        FUN_004a5da0((int32_t)actor,surrounded?speed:speed*2u);actor=U(0xd056f8u);o=hippo_offset(actor);
        direction=B(0xbcd609u+o);
        if(direction==9u){FUN_004b3250((int32_t)actor);actor=U(0xd056f8u);break;}
        if(direction==10u){B(0xbcd609u+o)=B(0xbcd60au+o);B(0xbcd663u+o)=0;B(0xbcd630u+o)=23;break;}
        if(direction==8u){
            if(linked && hippo_target_differs(o,hippo_offset((uint32_t)linked)) &&
               B(0xbcd630u+hippo_offset((uint32_t)linked))!=4u && B(0xbcd606u+hippo_offset((uint32_t)linked))==1u){
                FUN_004b3250((int32_t)actor);actor=U(0xd056f8u);
                hippo_copy_linked_target(actor,hippo_offset((uint32_t)linked));
            }else{
                B(0xbcd609u+o)=B(0xbcd60au+o);restore_actor_seek_other_kind((int32_t)actor);actor=U(0xd056f8u);
            }
        }
        break;
    case 21:
        linked=(int16_t)W(0xbcd656u+o);
        W(0xbcd62eu+o)=(uint16_t)(W(0xbcd62eu+o)-1u);
        if((int16_t)W(0xbcd62eu+o)<=0){B(0xbcd663u+o)=0;B(0xbcd630u+o)=23;break;}
        if(cycle_advanced && linked && hippo_target_differs(o,hippo_offset((uint32_t)linked))){
            FUN_004b3250((int32_t)actor);actor=U(0xd056f8u);
            hippo_copy_linked_target(actor,hippo_offset((uint32_t)linked));
        }
        if(surrounded)speed=hippo_half(speed);
        FUN_004a5da0((int32_t)actor,speed);actor=U(0xd056f8u);o=hippo_offset(actor);
        direction=B(0xbcd609u+o);
        if(direction==9u){FUN_004b3250((int32_t)actor);actor=U(0xd056f8u);break;}
        if(direction==10u)goto reset_ordinal_delay;
        if(direction==8u){
            if(linked && hippo_target_differs(o,hippo_offset((uint32_t)linked))){
                FUN_004b3250((int32_t)actor);actor=U(0xd056f8u);
                hippo_copy_linked_target(actor,hippo_offset((uint32_t)linked));
            }else goto reset_ordinal_delay;
        }
        break;
    case 13:
        if(surrounded)speed=hippo_half(speed);
        FUN_004a5da0((int32_t)actor,speed);actor=U(0xd056f8u);
        restore_actor_try_periodic_target((int32_t)actor);actor=U(0xd056f8u);o=hippo_offset(actor);
        direction=B(0xbcd609u+o);
        if(direction==9u){FUN_004b3250((int32_t)actor);actor=U(0xd056f8u);break;}
        if(direction==10u || direction==8u)goto reset_ordinal_delay;
        break;
    case 12:
        if(!W(0xbcd62eu+o)){FUN_004b5050((int32_t)actor);actor=U(0xd056f8u);}
        o=hippo_offset(actor);W(0xbcd62eu+o)=(uint16_t)(W(0xbcd62eu+o)+1u);
        if((int16_t)W(0xbcd62eu+o)>500)B(0xbcd630u+o)=4;
        break;
    default:B(0xbcd630u+o)=8;break;
    }
    return hippo_animation(actor,surrounded);
reset_ordinal_delay:
    B(0xbcd609u+o)=B(0xbcd60au+o);B(0xbcd630u+o)=23;
    W(0xbcd62eu+o)=(uint16_t)((uint32_t)B(0xbcd65au+o)*5u);
    return hippo_animation(actor,surrounded);
}

/* Native command table row3 decodes to Hippo Stomp. Keep its two distinct
   RNG publication phases and the post-probe vector advance literally. */
__declspec(noinline) void restore_actor_command_spawn_hippos(void)
{
 int32_t group;uint32_t group_offset,ordinal,center_x,center_y;
 if(B(0xe38ddau)!=1u)return;
 center_y=U(0xbca2ccu);center_x=U(0xbca2c8u);
 group=FUN_004b6ff0(84,5,0,(uint16_t)center_x,(uint16_t)center_y);
 if(group<=0)return;
 group_offset=(uint32_t)group*144u;
 B(0xbcba31u+group_offset)=1;
 W(0xbcba16u+group_offset)=24;
 B(0xbcb9f1u+group_offset)=8;
 for(ordinal=0;ordinal<8u;++ordinal){
  uint32_t index=U(0xd3b1c4u),sample=U(0xd3b374u),candidate=ordinal,attempt;
  struct command_random_pair pair=command_random_advance(index,sample);
  U(0xd3b384u)=pair.first;
  U(0xd3b380u)=pair.second;
  U(0xd3b370u)=pair.first&32767u;
  U(0xd3b374u)=pair.first&127u;
  U(0xd3b378u)=pair.first&7u;
  U(0xd3b364u)=pair.second&32767u;
  U(0xd3b368u)=pair.second&127u;
  U(0xd3b36cu)=pair.second&7u;
  for(attempt=0;attempt<8u;++attempt){
   uint32_t dy=U(0x5cd97cu+candidate*8u);
   uint32_t base=U(0xbca2d8u);
   uint32_t cy=U(0xbca2ccu);
   uint32_t dx=U(0x5cd978u+candidate*8u);
   uint32_t cx=U(0xbca2c8u);
   /* Original adds the center Y unscaled to this probe index. */
   uint32_t tile=base+dx+dy*228u+cx+cy;
   uint32_t terrain=U(0xaf598cu+tile*4u);
   uint32_t result=FUN_004ec410(terrain,0);
   ++candidate;if(candidate>=8u)candidate=0;
   if(!(result&255u))break;
  }
  index=U(0xd3b1c4u);sample=U(0xd3b374u);
  pair=command_random_advance(index,sample);
  U(0xd3b384u)=pair.first;
  center_y=U(0xbca2ccu);
  U(0xd3b378u)=pair.first&7u;
  U(0xd3b370u)=pair.first&32767u;
  {
   uint32_t dy=U(0x5cd97cu+candidate*8u),dx,actor,offset;
   U(0xd3b364u)=pair.second&32767u;
   U(0xd3b380u)=pair.second;
   U(0xd3b36cu)=pair.second&7u;
   dx=U(0x5cd978u+candidate*8u);
   center_x=U(0xbca2c8u);
   U(0xd3b374u)=pair.first&127u;
   U(0xd3b368u)=pair.second&127u;
   actor=(uint32_t)FUN_004b4800(1,84,(int32_t)(dx+center_x),(int32_t)(dy+center_y),(uint8_t)(pair.first&7u));
   offset=actor*388u;
   /* Allocation failure still writes actor zero in the original. */
   B(0xbcd630u+offset)=25;
   W(0xbcd658u+offset)=(uint16_t)group;
   W(0xbcd62eu+offset)=1000;
   B(0xbcd65au+offset)=(uint8_t)ordinal;
   U(0xbcd73cu+offset)=7;
   (void)restore_actor_seek_other_kind((int32_t)actor);
  }
 }
}

__declspec(noinline) void restore_actor_command_0041cd00(void)
{restore_actor_command_spawn_hippos();}
uint32_t restore_actor_command_004ec990(int32_t actor)
{return restore_actor_seek_other_kind(actor);}
__declspec(noinline) uint32_t FUN_004ec4a0(int32_t actor,uint32_t state)
{return restore_actor_apply_command_state(actor,state);}

/* Immutable command table57C3C8 row3 is the nullary Hippo Stomp action. */
int restore_actor_command_action(uint32_t token)
{
 if(token!=0x41cd00u)return 0;
 restore_actor_command_0041cd00();return 1;
}

/* Reviewed civic callers reload ECX and overwrite flags; EDX is replaced
   before meaningful use. Preserve full EAX, nonvolatile registers and RET0. */
void restore_actor_command_raw(uint32_t target,uint32_t registers[8],uint32_t *flags)
{
 uint32_t sp=registers[4];(void)flags;
#define A(n) U(sp+(uint32_t)(n)*4u)
 switch(target){
 case 0x4ec990u:registers[0]=restore_actor_seek_other_kind((int32_t)A(1));break;
 case 0x4ec7c0u:registers[0]=restore_actor_choose_command_state((int32_t)A(1));break;
 case 0x4ec0f0u:registers[0]=FUN_004ec0f0((int32_t)A(1),(int32_t)A(2),
  (volatile int32_t *)(uintptr_t)A(3),(volatile int32_t *)(uintptr_t)A(4),
  (int32_t)A(5),A(6),(restore_actor_tile_callback)(uintptr_t)A(7));break;
 default:abort();
 }
 registers[4]=sp+4u;
#undef A
}
