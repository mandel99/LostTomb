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
extern uint32_t FUN_00562348(void);
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

/* Crocodile/hyena updates and their cue/state choices. Keep the distinct
   cache, RNG publication and native return phases. */
/* SHL wraps first; signed /32768 then truncates toward zero, like the native
   CDQ / AND 7FFF / ADD / SAR sequence. This also retains explicit child-contract
   results outside the real random provider's 0..32767 range. */
static int32_t actor_kind_random_cue(uint32_t sample,uint32_t shift,int32_t first)
{
    int32_t product=(int32_t)(sample<<shift);
    return first+product/32768;
}

__declspec(noinline) void restore_actor_play_kind_cue(int32_t actor)
{
    uint32_t offset=(uint32_t)actor*388u;
    uint8_t kind=B(0xbcd602u+offset);
    int32_t sound;
    switch(kind){
    case 13:case 45:case 56:sound=13;break;
    case 26:sound=actor_kind_random_cue(FUN_00562348(),2,29);break;
    case 68:sound=8;break;
    case 69:sound=4;break;
    case 70:sound=5;break;
    case 77:case 78:case 92:case 93:case 100:case 101:sound=17;break;
    case 82:sound=7;break;
    case 83:sound=0;break;
    case 84:sound=6;break;
    case 102:sound=3;break;
    case 103:sound=1;break;
    case 104:sound=2;break;
    default:sound=actor_kind_random_cue(FUN_00562348(),3,21);break;
    }
    FUN_0053b4a0(actor,sound,3);
}


static uint32_t animal_state_shift_word(uint32_t word)
{
    return (word>>1)|((((word>>4)^word)&1u)<<30);
}

/* 4ECD40..4ECE92. Weights are fixed from the entry actor state before the
 * history write. The current actor global is never consulted by this body. */
__declspec(noinline) uint32_t restore_actor_choose_crocodile_state(int32_t actor)
{
    uint32_t offset=actor*388u;
    uint32_t weights[2],history,previous,next,a,b,sample,choice,total,i;
    uint8_t state=B(0xbcd630u+offset);

    if(state==8u) {
        weights[0]=50u;
        weights[1]=50u;
    } else if(state==19u) {
        weights[0]=90u;
        weights[1]=10u;
    } else {
        weights[0]=25u;
        weights[1]=75u;
    }

    history=U(0xd3b1c4u);
    previous=U(0xd3b374u);
    U(0xd3b1c8u+history*4u)=previous;
    next=history+1u;
    U(0xd3b1c4u)=next;
    if((int32_t)next>=100)U(0xd3b1c4u)=0u;

    /* Both state words are loaded after the indexed history store and index
     * publication. Out-of-range history indices can alias these words. */
    a=U(0xd3b384u);
    b=U(0xd3b380u);
    for(i=0;i<31u;++i) {
        a=animal_state_shift_word(a);
        b=animal_state_shift_word(b);
    }
    U(0xd3b384u)=a;
    U(0xd3b378u)=a&7u;
    U(0xd3b380u)=b;
    U(0xd3b364u)=b&0x7fffu;
    sample=a&0x7fffu;
    U(0xd3b370u)=sample;
    U(0xd3b368u)=b&0x7fu;
    U(0xd3b36cu)=b&7u;
    U(0xd3b374u)=a&0x7fu;

    /* Preserve the bounded cumulative scan and dynamic state-table read.
     * The native third table entry exists, although sample%100 cannot exhaust
     * these two weights (their sum is100 for every entry state). */
    sample%=100u;
    total=0u;
    for(choice=0u;choice<2u;++choice) {
        total+=weights[choice];
        if(sample<total)break;
    }
    return restore_actor_enter_animal_state(actor,U(0x5e4ee0u+choice*4u));
}

static void animal_remember_state_sample(uint32_t index,uint32_t previous)
{
    uint32_t next=index+1u;
    U(0xd3b1c8u+index*4u)=previous;
    U(0xd3b1c4u)=next;
    if((int32_t)next>=100)U(0xd3b1c4u)=0u;
}

__declspec(noinline) uint32_t restore_actor_enter_animal_state(int32_t actor,uint32_t state)
{
    uint32_t offset=(uint32_t)actor*388u;
    uint32_t index,previous,a,b,i,choice,attempts;
    B(0xbcd630u+offset)=(uint8_t)state;

    if(state!=19u) {
        uint32_t sample,quotient,remainder;
        index=U(0xd3b1c4u);
        previous=U(0xd3b374u);
        animal_remember_state_sample(index,previous);
        a=U(0xd3b384u);
        b=U(0xd3b380u);
        for(i=0;i<31u;++i) {a=animal_state_shift_word(a);b=animal_state_shift_word(b);}
        U(0xd3b384u)=a;
        U(0xd3b378u)=a&7u;
        U(0xd3b380u)=b;
        U(0xd3b364u)=b&0x7fffu;
        sample=a&0x7fffu;
        U(0xd3b370u)=sample;
        U(0xd3b368u)=b&0x7fu;
        U(0xd3b36cu)=b&7u;
        quotient=sample/100u;
        remainder=sample%100u;
        U(0xd3b374u)=a&0x7fu;
        W(0xbcd62eu+offset)=(uint16_t)(remainder+250u);
        U(0xbcd73cu+offset)=12u;
        U(0xbcd740u+offset)=0u;
        return quotient;
    }

    index=U(0xd3b1c4u);
    previous=U(0xd3b374u);
    U(0xbcd73cu+offset)=11u;
    animal_remember_state_sample(index,previous);
    a=U(0xd3b384u);
    b=U(0xd3b380u);
    for(i=0;i<31u;++i) {a=animal_state_shift_word(a);b=animal_state_shift_word(b);}
    U(0xd3b384u)=a;
    U(0xd3b378u)=a&7u;
    U(0xd3b380u)=b;
    U(0xd3b364u)=b&0x7fffu;
    U(0xd3b36cu)=b&7u;
    U(0xd3b370u)=a&0x7fffu;
    U(0xd3b374u)=a&0x7fu;
    U(0xd3b368u)=b&0x7fu;

    choice=(a&0x7fffu)&7u;
    for(attempts=0u;attempts<8u;++attempts) {
        uint32_t delta=U(0x5e4e80u+choice*4u);
        uint32_t tile=U(0xbcd618u+offset);
        uint32_t flags=U(0xaf598cu+(delta+tile)*4u);
        if((flags&4u)!=0u&&(flags&0x80000u)==0u) {
            uint16_t dx=W(0x5e4ea0u+choice*8u);
            uint16_t dy=W(0x5e4ea4u+choice*8u);
            uint16_t x=(uint16_t)(dx+W(0xbcd60cu+offset));
            uint16_t y;
            W(0xbcd61cu+offset)=x;
            y=(uint16_t)(dy+W(0xbcd60eu+offset));
            W(0xbcd61eu+offset)=y;
            U(0xbcd740u+offset)=0u;
            return (uint32_t)y;
        }
        ++choice;
        if(choice>=8u)choice=0u;
    }

    {
        /* Native search outputs reuse the incoming argument slots. Their
         * initial values are actor/state, not zero or uninitialized locals. */
        int32_t out_x=actor,out_y=(int32_t)state;
        int32_t y=(int32_t)(int16_t)W(0xbcd60eu+offset);
        int32_t x=(int32_t)(int16_t)W(0xbcd60cu+offset);
        uint32_t found=(uint32_t)restore_building_nearest_search(x,y,0,(const uint32_t *)0,
            4u,&out_x,&out_y,5,actor);
        if(found==0u) {
            uint16_t current_x=W(0xbcd60cu+offset);
            uint16_t current_y;
            W(0xbcd61cu+offset)=current_x;
            current_y=W(0xbcd60eu+offset);
            W(0xbcd61eu+offset)=current_y;
            U(0xbcd740u+offset)=0u;
            return (uint32_t)current_x;
        } else {
            uint16_t target_x=(uint16_t)out_x;
            uint16_t target_y=(uint16_t)out_y;
            W(0xbcd61cu+offset)=target_x;
            W(0xbcd61eu+offset)=target_y;
            U(0xbcd740u+offset)=0u;
            return (found&0xffff0000u)|(uint32_t)target_y;
        }
    }
}

struct animal_random_pair {uint32_t a,b;};
static uint32_t animal_offset(uint32_t actor){return actor*388u;}
static uint32_t animal_half_speed(uint32_t v){return (uint32_t)((int32_t)v>>1);}
static struct animal_random_pair animal_shift_random_pair(struct animal_random_pair p)
{
    uint32_t n;
    for(n=0;n<31u;++n){uint32_t a=(p.a^(p.a>>4))&1u,b=(p.b^(p.b>>4))&1u;p.a=(p.a>>1)|(a<<30);p.b=(p.b>>1)|(b<<30);}
    return p;
}
static struct animal_random_pair animal_advance_random_history(uint32_t index,uint32_t sample)
{
    struct animal_random_pair p;
    U(0xd3b1c8u+index*4u)=sample;++index;U(0xd3b1c4u)=index;
    if((int32_t)index>=100)U(0xd3b1c4u)=0;
    p.a=U(0xd3b384u);p.b=U(0xd3b380u);return animal_shift_random_pair(p);
}
static uint8_t animal_surrounded_by_flag4(uint32_t tile)
{
    uint32_t p=0xaf598cu+tile*4u;
    return (B(p)&4u)&&(B(p-912u)&4u)&&(B(p+4u)&4u)&&(B(p+912u)&4u)&&
           (B(p-4u)&4u)&&(B(p-908u)&4u)&&(B(p-916u)&4u)&&(B(p+916u)&4u)&&(B(p+908u)&4u);
}
static uint8_t animal_linked_target_differs(uint32_t o,uint32_t linked)
{
    uint16_t a=W(0xbcd60cu+linked),b=W(0xbcd61cu+o);
    if(a!=b)return 1;
    a=W(0xbcd60eu+linked);b=W(0xbcd61eu+o);return a!=b;
}
static void animal_copy_linked_target(uint32_t actor,uint32_t linked)
{
    uint16_t x=W(0xbcd60cu+linked);uint32_t o=animal_offset(actor);
    W(0xbcd61cu+o)=x;W(0xbcd61eu+o)=W(0xbcd60eu+linked);
}
static void crocodile_copy_group_target(uint32_t actor,uint32_t g,uint32_t index,uint32_t gx,uint32_t gy)
{
    uint16_t x=W(0x5cd978u+index*8u);uint32_t o=animal_offset(actor);
    x=(uint16_t)(x+W(gx+g));W(0xbcd61cu+o)=x;
    x=W(0x5cd97cu+index*8u);x=(uint16_t)(x+W(gy+g));W(0xbcd61eu+o)=x;B(0xbcd630u+o)=9;
}
static void hyena_copy_group_target(uint32_t actor,uint32_t g,uint32_t index)
{
    uint16_t x=W(0x5cdb78u+index*8u),y=W(0x5cdb7cu+index*8u);
    uint32_t o=animal_offset(actor);
    x=(uint16_t)(x+W(0xbcba02u+g));y=(uint16_t)(y+W(0xbcba04u+g));
    W(0xbcd61cu+o)=x;W(0xbcd61eu+o)=y;B(0xbcd630u+o)=9;
}
static uint32_t crocodile_animation(uint32_t actor,uint8_t surrounded)
{
    uint32_t o=animal_offset(actor),rotation=U(0xbca2e0u),direction,base,counter;uint8_t state;
    direction=(uint32_t)(int32_t)(int8_t)B(0xbcd609u+o)-rotation;U(0xe94e00u)=direction;
    if((int32_t)direction<0){direction+=8u;U(0xe94e00u)=direction;}
    state=B(0xbcd630u+o);
    switch(state){
    case 4:{int32_t delay=(int16_t)W(0xbcd62eu+o);
        if(surrounded){base=U(0xf56e14u);counter=delay>=23?22u:(uint32_t)delay;}
        else{base=U(0xf55dacu);counter=delay>=8?7u:(uint32_t)delay;}
        U(0xbcd5fcu+o)=base+counter;break;}
    case 5:counter=U(0xbcd740u+o);base=U(0xf55db4u);U(0xbcd5fcu+o)=base+counter*8u+direction;break;
    case 9:case 10:case 19:case 21:
        counter=U(0xbcd740u+o);base=U(surrounded?0xf55dbcu:0xf55da4u);U(0xbcd5fcu+o)=base+counter*8u+direction;break;
    case 23:counter=U(0xbcd740u+o);base=U(0xf55da4u);U(0xbcd5fcu+o)=base+counter*8u+direction;break;
    case 8:base=U(0xf55da4u);U(0xbcd5fcu+o)=base+direction;break;
    default:
        if(surrounded){counter=U(0xbcd740u+o);base=U(0xf55dc4u);U(0xbcd5fcu+o)=base+counter*8u+direction;}
        else{base=U(0xf55da4u);U(0xbcd5fcu+o)=base+direction;}
        break;
    }
    return o;
}
static uint32_t hyena_animation(uint32_t actor)
{
    uint32_t o=animal_offset(actor),counter=U(0xbcd740u+o),cycle=U(0xbcd73cu+o),direction,rotation,base;
    uint8_t state;
    if((int32_t)counter>=(int32_t)cycle || (int32_t)counter<0)U(0xbcd740u+o)=0;
    direction=(uint32_t)(int32_t)(int8_t)B(0xbcd609u+o);rotation=U(0xbca2e0u);direction-=rotation;
    U(0xe94e00u)=direction;if((int32_t)direction<0){direction+=8u;U(0xe94e00u)=direction;}
    state=B(0xbcd630u+o);
    switch(state){
    case 4:case 12:{int32_t delay=(int16_t)W(0xbcd62eu+o);base=U(0xf56204u);U(0xbcd5fcu+o)=base+(delay>=6?5u:(uint32_t)delay);break;}
    case 8:base=U(0xf561f4u);U(0xbcd5fcu+o)=base+direction;break;
    case 9:case 10:case 13:case 21:counter=U(0xbcd740u+o);base=U(0xf561f4u);U(0xbcd5fcu+o)=base+counter*8u+direction;break;
    case 18:counter=U(0xbcd740u+o);base=U(0xf5620cu);U(0xbcd5fcu+o)=base+counter*8u+direction;break;
    case 5:counter=U(0xbcd740u+o);base=U(0xf561fcu);U(0xbcd5fcu+o)=base+counter*8u+direction;break;
    case 20:base=U(0xf56214u);U(0xbcd5fcu+o)=base+direction;break;
    case 19:counter=U(0xbcd740u+o);base=U(0xf56214u);U(0xbcd5fcu+o)=base+counter*8u+direction;break;
    default:break;
    }
    return o;
}

__declspec(noinline) uint32_t restore_actor_update_crocodile(void)
{
    uint32_t actor=U(0xd056f8u),o=animal_offset(actor),kind=B(0xbcd602u+o),counter=U(0xbcd740u+o)+1u;
    uint32_t speed=U(0x7090ecu+kind*56u),tile,cycle;int32_t group,linked;
    uint8_t surrounded,completed=0,animation,state,direction;
    U(0xbcd740u+o)=counter;tile=U(0xbcd618u+o);surrounded=animal_surrounded_by_flag4(tile);
    if(surrounded)U(0xbcd73cu+o)=11;
    counter=U(0xbcd740u+o);cycle=U(0xbcd73cu+o);
    if((int32_t)counter>=(int32_t)cycle){U(0xbcd740u+o)=0;animation=B(0xbcd5f9u+o);++animation;completed=1;B(0xbcd5f9u+o)=animation;}
    {uint32_t profile=U(0xeaac28u)*0x49d8u;B(0xbcd66au+o)=6;W(0xea5ceau+profile)=(uint16_t)(W(0xea5ceau+profile)+1u);}
    B(0xbcd604u+o)=0;B(0xbcd65du+o)=0;animation=B(0xbcd5f9u+o);group=(int16_t)W(0xbcd658u+o);
    ++animation;B(0xbcd5f9u+o)=animation;if(animation>=12u)B(0xbcd5f9u+o)=0;
    state=B(0xbcd630u+o);
    switch(state){
    case 5:FUN_0053b4a0((int32_t)actor,7,2);actor=U(0xd056f8u);U(0xbcd73cu+animal_offset(actor))=7;FUN_004b17d0((int32_t)actor);actor=U(0xd056f8u);break;
    case 4:U(0xbcd73cu+o)=8;FUN_004b1d50((int32_t)actor);actor=U(0xd056f8u);break;
    case 24:
        W(0xbcd62eu+o)=(uint16_t)(W(0xbcd62eu+o)-1u);
        if((int16_t)W(0xbcd62eu+o)<=0){U(0xbcd740u+o)=0;U(0xbcd73cu+o)=8;B(0xbcd630u+o)=8;}
        U(0xbcd5fcu+o)=0;return o;
    case 8:FUN_004ecd40((int32_t)actor);actor=U(0xd056f8u);break;
    case 9:case 10:case 19:
        if(surrounded)speed=animal_half_speed(speed);
        FUN_004a5da0((int32_t)actor,speed);actor=U(0xd056f8u);o=animal_offset(actor);direction=B(0xbcd609u+o);
        if(direction==9u){FUN_004b3250((int32_t)actor);actor=U(0xd056f8u);}
        else if(direction==8u || direction==10u){B(0xbcd609u+o)=B(0xbcd60au+o);FUN_004ecd40((int32_t)actor);actor=U(0xd056f8u);}
        break;
    case 18:
        W(0xbcd62eu+o)=(uint16_t)(W(0xbcd62eu+o)-1u);
        if((int16_t)W(0xbcd62eu+o)<=0){
            uint32_t index=U(0xd3b1c4u),sample=U(0xd3b374u),g=(uint32_t)group*144u;
            struct animal_random_pair p=animal_advance_random_history(index,sample);int32_t x,y,gx,gy,result;
            U(0xd3b384u)=p.a;U(0xd3b378u)=p.a&7u;U(0xd3b380u)=p.b;U(0xd3b364u)=p.b&32767u;
            U(0xd3b36cu)=p.b&7u;y=(int16_t)W(0xbcd60eu+o);U(0xd3b374u)=p.a&127u;U(0xd3b368u)=p.b&127u;
            x=(int16_t)W(0xbcd60cu+o);U(0xd3b370u)=p.a&32767u;
            gy=(int16_t)W(0xbcba04u+g);gx=(int16_t)W(0xbcba02u+g);
            result=FUN_004eab10(gx,gy,(int32_t)(p.a&15u),x,y,0);
            if(result>=0 && result<16){actor=U(0xd056f8u);FUN_004b3250((int32_t)actor);actor=U(0xd056f8u);crocodile_copy_group_target(actor,g,(uint32_t)result,0xbcba02u,0xbcba04u);}
            else{actor=U(0xd056f8u);W(0xbcd62eu+animal_offset(actor))=100;}
        }
        if(completed){restore_actor_try_periodic_target((int32_t)actor);actor=U(0xd056f8u);}break;
    case 20:restore_actor_try_periodic_target((int32_t)actor);actor=U(0xd056f8u);break;
    case 23:{
        uint32_t index=U(0xd3b1c4u),sample=U(0xd3b374u),g=(uint32_t)group*144u;
        struct animal_random_pair p=animal_advance_random_history(index,sample);int32_t x,y,gx,gy,result;
        U(0xd3b384u)=p.a;U(0xd3b378u)=p.a&7u;U(0xd3b380u)=p.b;U(0xd3b364u)=p.b&32767u;
        U(0xd3b36cu)=p.b&7u;y=(int16_t)W(0xbcd60eu+o);U(0xd3b374u)=p.a&127u;U(0xd3b368u)=p.b&127u;
        x=(int16_t)W(0xbcd60cu+o);U(0xd3b370u)=p.a&32767u;
        gy=(int16_t)W(0xbcba04u+g);gx=(int16_t)W(0xbcba02u+g);
        result=FUN_004eab10(gx,gy,(int32_t)(p.a&15u),x,y,0);
        if(result>=0 && result<16){actor=U(0xd056f8u);FUN_004b3250((int32_t)actor);actor=U(0xd056f8u);crocodile_copy_group_target(actor,g,(uint32_t)result,0xbcba02u,0xbcba04u);break;}
        index=U(0xd3b1c4u);sample=U(0xd3b374u);p=animal_advance_random_history(index,sample);
        U(0xd3b384u)=p.a;U(0xd3b378u)=p.a&7u;U(0xd3b380u)=p.b;U(0xd3b364u)=p.b&32767u;
        U(0xd3b368u)=p.b&127u;actor=U(0xd056f8u);U(0xd3b36cu)=p.b&7u;U(0xd3b370u)=p.a&32767u;
        o=animal_offset(actor);y=(int16_t)W(0xbcd60eu+o);x=(int16_t)W(0xbcd60cu+o);
        gy=(int16_t)W(0xbcba00u+g);gx=(int16_t)W(0xbcb9feu+g);U(0xd3b374u)=p.a&127u;
        result=FUN_004eab10(gx,gy,(int32_t)(p.a&15u),x,y,0);
        if(result>=0 && result<16){actor=U(0xd056f8u);FUN_004b3250((int32_t)actor);actor=U(0xd056f8u);crocodile_copy_group_target(actor,g,(uint32_t)result,0xbcb9feu,0xbcba00u);break;}
        actor=U(0xd056f8u);B(0xbcd606u+animal_offset(actor))=2;return actor;
    }
    case 21:
        linked=(int16_t)W(0xbcd656u+o);W(0xbcd62eu+o)=(uint16_t)(W(0xbcd62eu+o)-1u);
        if((int16_t)W(0xbcd62eu+o)<=0){B(0xbcd663u+o)=0;B(0xbcd630u+o)=23;break;}
        if(linked && animal_linked_target_differs(o,animal_offset((uint32_t)linked))){FUN_004b3250((int32_t)actor);actor=U(0xd056f8u);animal_copy_linked_target(actor,animal_offset((uint32_t)linked));}
        if(surrounded)speed=animal_half_speed(speed);
        FUN_004a5da0((int32_t)actor,speed);actor=U(0xd056f8u);o=animal_offset(actor);direction=B(0xbcd609u+o);
        if(direction==9u){FUN_004b3250((int32_t)actor);actor=U(0xd056f8u);break;}
        if(direction==10u)goto reset82;
        if(direction==8u){if(linked && animal_linked_target_differs(o,animal_offset((uint32_t)linked))){FUN_004b3250((int32_t)actor);actor=U(0xd056f8u);animal_copy_linked_target(actor,animal_offset((uint32_t)linked));}else goto reset82;}
        break;
    case 13:
        if(surrounded)speed=animal_half_speed(speed);
        FUN_004a5da0((int32_t)actor,speed);actor=U(0xd056f8u);restore_actor_try_periodic_target((int32_t)actor);actor=U(0xd056f8u);
        o=animal_offset(actor);direction=B(0xbcd609u+o);
        if(direction==9u){FUN_004b3250((int32_t)actor);actor=U(0xd056f8u);}
        else if(direction==8u || direction==10u)goto reset82;
        break;
    case 12:
        if(!W(0xbcd62eu+o)){FUN_004b5050((int32_t)actor);actor=U(0xd056f8u);}
        o=animal_offset(actor);W(0xbcd62eu+o)=(uint16_t)(W(0xbcd62eu+o)+1u);if((int16_t)W(0xbcd62eu+o)>500)B(0xbcd630u+o)=4;break;
    default:B(0xbcd630u+o)=8;break;
    }
    return crocodile_animation(actor,surrounded);
reset82:
    B(0xbcd609u+o)=B(0xbcd60au+o);B(0xbcd630u+o)=23;W(0xbcd62eu+o)=(uint16_t)((uint32_t)B(0xbcd65au+o)*5u);
    return crocodile_animation(actor,surrounded);
}

__declspec(noinline) uint32_t restore_actor_update_hyena(void)
{
    uint32_t actor=U(0xd056f8u),o=animal_offset(actor),kind=B(0xbcd602u+o),counter=U(0xbcd740u+o)+1u,speed,cycle;
    int32_t group,linked;uint8_t completed=0,animation,state,direction;
    U(0xbcd740u+o)=counter;speed=U(0x7090ecu+kind*56u);cycle=U(0xbcd73cu+o);
    if((int32_t)counter>=(int32_t)cycle){U(0xbcd740u+o)=0;animation=B(0xbcd5f9u+o);++animation;completed=1;B(0xbcd5f9u+o)=animation;}
    {uint32_t profile=U(0xeaac28u)*0x49d8u;B(0xbcd66au+o)=5;W(0xea5ceau+profile)=(uint16_t)(W(0xea5ceau+profile)+1u);}
    B(0xbcd604u+o)=0;B(0xbcd65du+o)=0;animation=B(0xbcd5f9u+o);group=(int16_t)W(0xbcd658u+o);
    ++animation;B(0xbcd5f9u+o)=animation;if(animation>=12u)B(0xbcd5f9u+o)=0;
    state=B(0xbcd630u+o);
    switch(state){
    case 5:U(0xbcd73cu+o)=6;FUN_0053b4a0((int32_t)actor,0,2);actor=U(0xd056f8u);FUN_004b17d0((int32_t)actor);actor=U(0xd056f8u);break;
    case 4:U(0xbcd73cu+o)=6;FUN_004b1d50((int32_t)actor);actor=U(0xd056f8u);break;
    case 24:{uint32_t result=20;W(0xbcd62eu+o)=(uint16_t)(W(0xbcd62eu+o)-1u);
        if((int16_t)W(0xbcd62eu+o)<=0){result=8;U(0xbcd740u+o)=0;U(0xbcd73cu+o)=8;B(0xbcd630u+o)=8;}
        U(0xbcd5fcu+o)=0;return result;}
    case 8:{
        uint32_t index=U(0xd3b1c4u),sample;struct animal_random_pair p;uint8_t previous;
        U(0xbcd73cu+o)=1;previous=B(0xbcd60au+o);B(0xbcd609u+o)=previous;sample=U(0xd3b374u);
        p=animal_advance_random_history(index,sample);
        state=(uint8_t)(((~p.a)&1u)|18u);index=U(0xd3b1c4u);U(0xd3b1c8u+index*4u)=p.a&127u;++index;
        B(0xbcd630u+o)=state;U(0xd3b1c4u)=index;if((int32_t)index>=100)U(0xd3b1c4u)=0;
        p=animal_shift_random_pair(p);
        U(0xd3b384u)=p.a;U(0xd3b378u)=p.a&7u;U(0xd3b380u)=p.b;U(0xd3b364u)=p.b&32767u;
        U(0xd3b370u)=p.a&32767u;U(0xd3b368u)=p.b&127u;U(0xd3b36cu)=p.b&7u;
        U(0xd3b374u)=p.a&127u;W(0xbcd62eu+o)=(uint16_t)((p.a&32767u)%100u+150u);break;
    }
    case 19:
        U(0xbcd73cu+o)=6;W(0xbcd62eu+o)=(uint16_t)(W(0xbcd62eu+o)-1u);if((int16_t)W(0xbcd62eu+o)<=0)B(0xbcd630u+o)=8;break;
    case 18:
        U(0xbcd73cu+o)=6;W(0xbcd62eu+o)=(uint16_t)(W(0xbcd62eu+o)-1u);
        if((int16_t)W(0xbcd62eu+o)<=0){
            uint32_t index=U(0xd3b1c4u),sample=U(0xd3b374u),g=(uint32_t)group*144u;
            struct animal_random_pair p=animal_advance_random_history(index,sample);int32_t x,y,gx,gy,result;
            U(0xd3b384u)=p.a;U(0xd3b378u)=p.a&7u;U(0xd3b380u)=p.b;U(0xd3b364u)=p.b&32767u;
            U(0xd3b36cu)=p.b&7u;U(0xd3b374u)=p.a&127u;y=(int16_t)W(0xbcd60eu+o);x=(int16_t)W(0xbcd60cu+o);
            U(0xd3b370u)=p.a&32767u;U(0xd3b368u)=p.b&127u;gy=(int16_t)W(0xbcba04u+g);gx=(int16_t)W(0xbcba02u+g);
            result=FUN_004eab10(gx,gy,(int32_t)(p.a&15u),x,y,0);
            if(result>=0 && result<16){actor=U(0xd056f8u);FUN_004b3250((int32_t)actor);actor=U(0xd056f8u);hyena_copy_group_target(actor,g,(uint32_t)result);restore_actor_try_periodic_target((int32_t)actor);actor=U(0xd056f8u);break;}
            actor=U(0xd056f8u);W(0xbcd62eu+animal_offset(actor))=100;
        }
        restore_actor_try_periodic_target((int32_t)actor);actor=U(0xd056f8u);break;
    case 23:{
        uint32_t index=U(0xd3b1c4u),sample=U(0xd3b374u),g=(uint32_t)group*144u;struct animal_random_pair p;int32_t x,y,gx,gy,result;
        U(0xbcd73cu+o)=1;p=animal_advance_random_history(index,sample);
        U(0xd3b384u)=p.a;U(0xd3b378u)=p.a&7u;U(0xd3b380u)=p.b;U(0xd3b364u)=p.b&32767u;
        U(0xd3b36cu)=p.b&7u;U(0xd3b374u)=p.a&127u;y=(int16_t)W(0xbcd60eu+o);x=(int16_t)W(0xbcd60cu+o);
        U(0xd3b370u)=p.a&32767u;U(0xd3b368u)=p.b&127u;gy=(int16_t)W(0xbcba04u+g);gx=(int16_t)W(0xbcba02u+g);
        result=FUN_004eab10(gx,gy,(int32_t)(p.a&15u),x,y,0);
        if(result>=0 && result<16){actor=U(0xd056f8u);FUN_004b3250((int32_t)actor);actor=U(0xd056f8u);hyena_copy_group_target(actor,g,(uint32_t)result);}
        else{actor=U(0xd056f8u);o=animal_offset(actor);B(0xbcd630u+o)=18;W(0xbcd62eu+o)=100;}
        break;
    }
    case 20:U(0xbcd73cu+o)=1;restore_actor_try_periodic_target((int32_t)actor);actor=U(0xd056f8u);break;
    case 9:case 10:
        U(0xbcd73cu+o)=12;FUN_004a5da0((int32_t)actor,speed);actor=U(0xd056f8u);o=animal_offset(actor);direction=B(0xbcd609u+o);
        if(direction==9u){FUN_004b3250((int32_t)actor);actor=U(0xd056f8u);break;}
        if(direction==10u){B(0xbcd609u+o)=B(0xbcd60au+o);B(0xbcd630u+o)=8;W(0xbcd62eu+o)=(uint16_t)((uint32_t)B(0xbcd65au+o)*5u);break;}
        if(direction==8u){
            uint32_t index=U(0xd3b1c4u),sample=U(0xd3b374u);uint8_t previous=B(0xbcd60au+o);struct animal_random_pair p;
            U(0xd3b1c8u+index*4u)=sample;++index;B(0xbcd609u+o)=previous;U(0xd3b1c4u)=index;if((int32_t)index>=100)U(0xd3b1c4u)=0;
            p.a=U(0xd3b384u);p.b=U(0xd3b380u);p=animal_shift_random_pair(p);
            B(0xbcd630u+o)=(p.a&1u)?18:19;index=U(0xd3b1c4u);U(0xd3b1c8u+index*4u)=p.a&127u;++index;U(0xd3b1c4u)=index;if((int32_t)index>=100)U(0xd3b1c4u)=0;
            p=animal_shift_random_pair(p);
            U(0xd3b384u)=p.a;U(0xd3b378u)=p.a&7u;U(0xd3b380u)=p.b;U(0xd3b364u)=p.b&32767u;
            U(0xd3b370u)=p.a&32767u;U(0xd3b36cu)=p.b&7u;U(0xd3b368u)=p.b&127u;U(0xd3b374u)=p.a&127u;
            W(0xbcd62eu+o)=(uint16_t)((p.a&32767u)%100u+150u);
        }
        break;
    case 21:
        U(0xbcd73cu+o)=12;linked=(int16_t)W(0xbcd656u+o);W(0xbcd62eu+o)=(uint16_t)(W(0xbcd62eu+o)-1u);
        if((int16_t)W(0xbcd62eu+o)<=0 && completed){W(0xbcd62eu+o)=0;B(0xbcd663u+o)=0;B(0xbcd630u+o)=23;break;}
        if(linked && animal_linked_target_differs(o,animal_offset((uint32_t)linked)) && completed){FUN_004b3250((int32_t)actor);actor=U(0xd056f8u);animal_copy_linked_target(actor,animal_offset((uint32_t)linked));}
        FUN_004a5da0((int32_t)actor,speed);actor=U(0xd056f8u);o=animal_offset(actor);direction=B(0xbcd609u+o);
        if(direction==9u){FUN_004b3250((int32_t)actor);actor=U(0xd056f8u);break;}
        if(direction==8u || direction==10u){B(0xbcd609u+o)=B(0xbcd60au+o);B(0xbcd630u+o)=23;W(0xbcd62eu+o)=(uint16_t)((uint32_t)B(0xbcd65au+o)*5u);}
        break;
    case 13:
        U(0xbcd73cu+o)=12;FUN_004a5da0((int32_t)actor,speed*2u);actor=U(0xd056f8u);o=animal_offset(actor);direction=B(0xbcd609u+o);
        if(direction==9u){FUN_004b3250((int32_t)actor);actor=U(0xd056f8u);}
        else if(direction==8u || direction==10u){B(0xbcd609u+o)=B(0xbcd60au+o);B(0xbcd630u+o)=23;W(0xbcd62eu+o)=(uint16_t)((uint32_t)B(0xbcd65au+o)*5u);}
        if(completed){restore_actor_try_periodic_target((int32_t)actor);actor=U(0xd056f8u);}break;
    case 12:
        U(0xbcd73cu+o)=6;if(!W(0xbcd62eu+o)){FUN_004b5050((int32_t)actor);actor=U(0xd056f8u);}
        o=animal_offset(actor);W(0xbcd62eu+o)=(uint16_t)(W(0xbcd62eu+o)+1u);if((int16_t)W(0xbcd62eu+o)>500)B(0xbcd630u+o)=4;break;
    default:U(0xbcd73cu+o)=1;B(0xbcd630u+o)=8;break;
    }
    return hyena_animation(actor);
}

/* Scorpion, Lion and Asp callbacks. Share only local shift/offset arithmetic;
   keep each cache, history publication and native return phase. */
static struct animal_random_pair scorpion_history(uint32_t index,uint32_t previous)
{
    struct animal_random_pair p;
    U(0xd3b1c8u+index*4u)=previous;++index;U(0xd3b1c4u)=index;
    if((int32_t)index>=100)U(0xd3b1c4u)=0;
    p.a=U(0xd3b384u);p.b=U(0xd3b380u);return animal_shift_random_pair(p);
}
static uint8_t scorpion_target_differs(uint32_t offset,uint32_t linked)
{
    uint16_t x=W(0xbcd60cu+linked),target=W(0xbcd61cu+offset);
    if(x!=target)return 1;
    x=W(0xbcd60eu+linked);target=W(0xbcd61eu+offset);return x!=target;
}
static void scorpion_copy_linked(uint32_t actor,uint32_t linked)
{
    uint16_t x=W(0xbcd60cu+linked);uint32_t offset=animal_offset(actor);
    W(0xbcd61cu+offset)=x;W(0xbcd61eu+offset)=W(0xbcd60eu+linked);
}
static void scorpion_copy_group(uint32_t actor,uint32_t group,uint32_t vector)
{
    uint16_t x=W(0x5cdb78u+vector*8u),y=W(0x5cdb7cu+vector*8u);
    uint32_t offset=animal_offset(actor);
    x=(uint16_t)(x+W(0xbcba02u+group));y=(uint16_t)(y+W(0xbcba04u+group));
    W(0xbcd61cu+offset)=x;W(0xbcd61eu+offset)=y;B(0xbcd630u+offset)=9;
}
static uint32_t scorpion_animation(uint32_t actor)
{
    uint32_t offset=animal_offset(actor),counter=U(0xbcd740u+offset),cycle=U(0xbcd73cu+offset),direction,base;
    uint8_t state;
    if((int32_t)counter>=(int32_t)cycle||(int32_t)counter<0)U(0xbcd740u+offset)=0;
    direction=(uint32_t)(int32_t)(int8_t)B(0xbcd609u+offset);direction-=U(0xbca2e0u);U(0xe94e00u)=direction;
    if((int32_t)direction<0){direction+=8u;U(0xe94e00u)=direction;}
    state=B(0xbcd630u+offset);
    switch(state){
    case 4:case 12:{int32_t delay=(int16_t)W(0xbcd62eu+offset);base=U(0xf570b4u);U(0xbcd5fcu+offset)=base+(delay>=6?5u:(uint32_t)delay);break;}
    case 8:case 9:case 10:case 13:case 21:counter=U(0xbcd740u+offset);base=U(0xf5709cu);U(0xbcd5fcu+offset)=base+counter*8u+direction;break;
    case 5:counter=U(0xbcd740u+offset);base=U(0xf570a4u);U(0xbcd5fcu+offset)=base+counter*8u+direction;break;
    case 20:base=U(0xf570bcu);U(0xbcd5fcu+offset)=base+direction;break;
    case 18:case 19:counter=U(0xbcd740u+offset);base=U(0xf570bcu);U(0xbcd5fcu+offset)=base+counter*8u+direction;break;
    default:break;
    }
    return offset;
}

/* Ordinary candidate for the bounded3390-byte callback; no frame interpreter.
 * Keep each RNG publication phase distinct. Child algorithms are supplied by
 * the isolated verifier protocol, not silently redirected to native code. */
__declspec(noinline) uint32_t restore_actor_update_scorpion(void)
{
    uint32_t actor=U(0xd056f8u),offset=animal_offset(actor),kind=B(0xbcd602u+offset);
    uint32_t counter=U(0xbcd740u+offset)+1u,speed,cycle;int32_t group,linked;
    uint8_t completed=0,animation,state,direction;
    U(0xbcd740u+offset)=counter;speed=U(0x7090ecu+kind*56u);cycle=U(0xbcd73cu+offset);
    if((int32_t)counter>=(int32_t)cycle){U(0xbcd740u+offset)=0;animation=B(0xbcd5f9u+offset);++animation;completed=1;B(0xbcd5f9u+offset)=animation;}
    {uint32_t profile=U(0xeaac28u)*0x49d8u;B(0xbcd66au+offset)=5;W(0xea5ceau+profile)=(uint16_t)(W(0xea5ceau+profile)+1u);}
    B(0xbcd604u+offset)=0;B(0xbcd65du+offset)=0;animation=B(0xbcd5f9u+offset);group=(int16_t)W(0xbcd658u+offset);
    ++animation;B(0xbcd5f9u+offset)=animation;if(animation>=12u)B(0xbcd5f9u+offset)=0;
    state=B(0xbcd630u+offset);
    switch(state){
    case 5:U(0xbcd73cu+offset)=6;FUN_0053b4a0((int32_t)actor,2,2);actor=U(0xd056f8u);FUN_004b17d0((int32_t)actor);actor=U(0xd056f8u);break;
    case 4:U(0xbcd73cu+offset)=6;FUN_004b1d50((int32_t)actor);actor=U(0xd056f8u);break;
    case 24:{uint32_t result=20;
        W(0xbcd62eu+offset)=(uint16_t)(W(0xbcd62eu+offset)-1u);
        if((int16_t)W(0xbcd62eu+offset)<=0){result=8;U(0xbcd740u+offset)=0;U(0xbcd73cu+offset)=8;B(0xbcd630u+offset)=8;}
        U(0xbcd5fcu+offset)=0;return result;
    }
    case 8:{
        uint32_t sample=U(0xd3b374u),index;struct animal_random_pair p;
        U(0xbcd73cu+offset)=0;B(0xbcd609u+offset)=B(0xbcd60au+offset);index=U(0xd3b1c4u);
        p=scorpion_history(index,sample);index=U(0xd3b1c4u);U(0xd3b1c8u+index*4u)=p.a&127u;++index;
        B(0xbcd630u+offset)=(p.a&1u)?18:19;U(0xd3b1c4u)=index;if((int32_t)index>=100)U(0xd3b1c4u)=0;
        p=animal_shift_random_pair(p);
        U(0xd3b384u)=p.a;U(0xd3b378u)=p.a&7u;U(0xd3b380u)=p.b;U(0xd3b364u)=p.b&32767u;
        U(0xd3b370u)=p.a&32767u;U(0xd3b368u)=p.b&127u;U(0xd3b36cu)=p.b&7u;U(0xd3b374u)=p.a&127u;
        W(0xbcd62eu+offset)=(uint16_t)((p.a&32767u)%100u);break;
    }
    case 19:U(0xbcd73cu+offset)=6;W(0xbcd62eu+offset)=(uint16_t)(W(0xbcd62eu+offset)-1u);if((int16_t)W(0xbcd62eu+offset)<=0)B(0xbcd630u+offset)=8;break;
    case 18:
        U(0xbcd73cu+offset)=6;W(0xbcd62eu+offset)=(uint16_t)(W(0xbcd62eu+offset)-1u);
        if((int16_t)W(0xbcd62eu+offset)<=0){
            uint32_t index=U(0xd3b1c4u),sample=U(0xd3b374u),g=(uint32_t)group*144u;
            struct animal_random_pair p=scorpion_history(index,sample);int32_t x,y,gx,gy,result;
            U(0xd3b384u)=p.a;U(0xd3b378u)=p.a&7u;U(0xd3b380u)=p.b;U(0xd3b364u)=p.b&32767u;
            U(0xd3b36cu)=p.b&7u;U(0xd3b374u)=p.a&127u;y=(int16_t)W(0xbcd60eu+offset);x=(int16_t)W(0xbcd60cu+offset);
            U(0xd3b370u)=p.a&32767u;U(0xd3b368u)=p.b&127u;gy=(int16_t)W(0xbcba04u+g);gx=(int16_t)W(0xbcba02u+g);
            result=FUN_004eab10(gx,gy,(int32_t)(p.a&15u),x,y,0);
            if(result>=0&&result<16){
                actor=U(0xd056f8u);FUN_004b3250((int32_t)actor);actor=U(0xd056f8u);scorpion_copy_group(actor,g,(uint32_t)result);U(0xbcd740u+animal_offset(actor))=0;
                restore_actor_try_periodic_target((int32_t)actor);actor=U(0xd056f8u);break;
            }
            index=U(0xd3b1c4u);sample=U(0xd3b374u);p=scorpion_history(index,sample);
            U(0xd3b384u)=p.a;U(0xd3b378u)=p.a&7u;U(0xd3b380u)=p.b;U(0xd3b364u)=p.b&32767u;
            U(0xd3b370u)=p.a&32767u;U(0xd3b368u)=p.b&127u;U(0xd3b36cu)=p.b&7u;actor=U(0xd056f8u);
            U(0xd3b374u)=p.a&127u;W(0xbcd62eu+animal_offset(actor))=(uint16_t)((p.a&32767u)%100u);
        }
        restore_actor_try_periodic_target((int32_t)actor);actor=U(0xd056f8u);break;
    case 23:{
        uint32_t index=U(0xd3b1c4u),sample=U(0xd3b374u),g=(uint32_t)group*144u;
        struct animal_random_pair p;int32_t x,y,gx,gy,result;
        U(0xbcd73cu+offset)=1;p=scorpion_history(index,sample);
        U(0xd3b384u)=p.a;U(0xd3b378u)=p.a&7u;U(0xd3b380u)=p.b;U(0xd3b364u)=p.b&32767u;
        U(0xd3b36cu)=p.b&7u;U(0xd3b374u)=p.a&127u;y=(int16_t)W(0xbcd60eu+offset);x=(int16_t)W(0xbcd60cu+offset);
        U(0xd3b370u)=p.a&32767u;U(0xd3b368u)=p.b&127u;gy=(int16_t)W(0xbcba04u+g);gx=(int16_t)W(0xbcba02u+g);
        result=FUN_004eab10(gx,gy,(int32_t)(p.a&15u),x,y,0);
        if(result>=0&&result<16){actor=U(0xd056f8u);FUN_004b3250((int32_t)actor);actor=U(0xd056f8u);scorpion_copy_group(actor,g,(uint32_t)result);}
        else{
            actor=U(0xd056f8u);index=U(0xd3b1c4u);sample=U(0xd3b374u);offset=animal_offset(actor);
            U(0xd3b1c8u+index*4u)=sample;++index;B(0xbcd630u+offset)=18;U(0xd3b1c4u)=index;if((int32_t)index>=100)U(0xd3b1c4u)=0;
            p.a=U(0xd3b384u);p.b=U(0xd3b380u);p=animal_shift_random_pair(p);
            U(0xd3b384u)=p.a;U(0xd3b378u)=p.a&7u;U(0xd3b380u)=p.b;U(0xd3b364u)=p.b&32767u;
            U(0xd3b370u)=p.a&32767u;U(0xd3b368u)=p.b&127u;U(0xd3b36cu)=p.b&7u;U(0xd3b374u)=p.a&127u;
            W(0xbcd62eu+offset)=(uint16_t)((p.a&32767u)%100u);
        }
        break;
    }
    case 20:U(0xbcd73cu+offset)=1;restore_actor_try_periodic_target((int32_t)actor);actor=U(0xd056f8u);break;
    case 9:case 10:
        U(0xbcd73cu+offset)=12;FUN_004a5da0((int32_t)actor,speed);actor=U(0xd056f8u);offset=animal_offset(actor);direction=B(0xbcd609u+offset);
        if(direction==9u){FUN_004b3250((int32_t)actor);actor=U(0xd056f8u);break;}
        if(direction==10u){B(0xbcd609u+offset)=B(0xbcd60au+offset);B(0xbcd630u+offset)=8;W(0xbcd62eu+offset)=(uint16_t)((uint32_t)B(0xbcd65au+offset)*5u);break;}
        if(direction==8u){
            uint32_t index=U(0xd3b1c4u),sample=U(0xd3b374u);uint8_t previous=B(0xbcd60au+offset);struct animal_random_pair p;
            U(0xd3b1c8u+index*4u)=sample;++index;B(0xbcd609u+offset)=previous;U(0xd3b1c4u)=index;if((int32_t)index>=100)U(0xd3b1c4u)=0;
            p.a=U(0xd3b384u);p.b=U(0xd3b380u);p=animal_shift_random_pair(p);
            B(0xbcd630u+offset)=(p.a&1u)?18:19;index=U(0xd3b1c4u);U(0xd3b1c8u+index*4u)=p.a&127u;++index;U(0xd3b1c4u)=index;if((int32_t)index>=100)U(0xd3b1c4u)=0;
            p=animal_shift_random_pair(p);
            U(0xd3b384u)=p.a;U(0xd3b378u)=p.a&7u;U(0xd3b380u)=p.b;U(0xd3b364u)=p.b&32767u;
            U(0xd3b370u)=p.a&32767u;U(0xd3b36cu)=p.b&7u;U(0xd3b368u)=p.b&127u;U(0xd3b374u)=p.a&127u;
            W(0xbcd62eu+offset)=(uint16_t)((p.a&32767u)%100u);U(0xbcd740u+offset)=0;
        }
        break;
    case 21:
        U(0xbcd73cu+offset)=12;linked=(int16_t)W(0xbcd656u+offset);W(0xbcd62eu+offset)=(uint16_t)(W(0xbcd62eu+offset)-1u);
        if((int16_t)W(0xbcd62eu+offset)<=0&&completed){W(0xbcd62eu+offset)=0;B(0xbcd663u+offset)=0;B(0xbcd630u+offset)=23;break;}
        if(linked&&scorpion_target_differs(offset,animal_offset((uint32_t)linked))&&completed){FUN_004b3250((int32_t)actor);actor=U(0xd056f8u);scorpion_copy_linked(actor,animal_offset((uint32_t)linked));}
        FUN_004a5da0((int32_t)actor,speed);actor=U(0xd056f8u);offset=animal_offset(actor);direction=B(0xbcd609u+offset);
        if(direction==9u){FUN_004b3250((int32_t)actor);actor=U(0xd056f8u);break;}
        if(direction==8u||direction==10u){B(0xbcd609u+offset)=B(0xbcd60au+offset);B(0xbcd630u+offset)=23;W(0xbcd62eu+offset)=(uint16_t)((uint32_t)B(0xbcd65au+offset)*5u);}break;
    case 13:
        U(0xbcd73cu+offset)=12;FUN_004a5da0((int32_t)actor,speed*2u);actor=U(0xd056f8u);offset=animal_offset(actor);direction=B(0xbcd609u+offset);
        if(direction==9u){FUN_004b3250((int32_t)actor);actor=U(0xd056f8u);}
        else if(direction==8u||direction==10u){B(0xbcd609u+offset)=B(0xbcd60au+offset);B(0xbcd630u+offset)=23;W(0xbcd62eu+offset)=(uint16_t)((uint32_t)B(0xbcd65au+offset)*5u);}
        if(completed){restore_actor_try_periodic_target((int32_t)actor);actor=U(0xd056f8u);}break;
    case 12:
        U(0xbcd73cu+offset)=6;if(!W(0xbcd62eu+offset)){FUN_004b5050((int32_t)actor);actor=U(0xd056f8u);}
        offset=animal_offset(actor);W(0xbcd62eu+offset)=(uint16_t)(W(0xbcd62eu+offset)+1u);if((int16_t)W(0xbcd62eu+offset)>500)B(0xbcd630u+offset)=4;break;
    default:U(0xbcd73cu+offset)=0;B(0xbcd630u+offset)=8;break;
    }
    return scorpion_animation(actor);
}

static void lion_history(uint32_t index,uint32_t sample)
{
    U(0xd3b1c8u+index*4u)=sample;++index;U(0xd3b1c4u)=index;
    if((int32_t)index>=100)U(0xd3b1c4u)=0;
}
static struct animal_random_pair lion_read_shift(void)
{
    struct animal_random_pair p;p.a=U(0xd3b384u);p.b=U(0xd3b380u);return animal_shift_random_pair(p);
}
static uint8_t lion_target_differs(uint32_t o,uint32_t linked)
{
    uint16_t a=W(0xbcd60cu+linked),b=W(0xbcd61cu+o);
    if(a!=b)return 1;
    a=W(0xbcd60eu+linked);b=W(0xbcd61eu+o);return a!=b;
}
static void lion_copy_linked(uint32_t actor,uint32_t linked)
{
    uint16_t x=W(0xbcd60cu+linked);uint32_t o=animal_offset(actor);
    W(0xbcd61cu+o)=x;W(0xbcd61eu+o)=W(0xbcd60eu+linked);
}
static void lion_copy_group(uint32_t actor,uint32_t group,uint32_t index)
{
    uint16_t x=W(0x5cdb78u+index*8u),y=W(0x5cdb7cu+index*8u);
    uint32_t o=animal_offset(actor);
    x=(uint16_t)(x+W(0xbcba02u+group));y=(uint16_t)(y+W(0xbcba04u+group));
    W(0xbcd61cu+o)=x;W(0xbcd61eu+o)=y;B(0xbcd630u+o)=9;
}
static uint32_t lion_animation(uint32_t actor)
{
    uint32_t o=animal_offset(actor),counter=U(0xbcd740u+o),cycle=U(0xbcd73cu+o),direction,base,sprite,result;
    uint8_t state;
    if((int32_t)counter>=(int32_t)cycle || (int32_t)counter<0)U(0xbcd740u+o)=0;
    direction=(uint32_t)(int32_t)(int8_t)B(0xbcd609u+o);direction-=U(0xbca2e0u);
    U(0xe94e00u)=direction;if((int32_t)direction<0){direction+=8u;U(0xe94e00u)=direction;}
    state=B(0xbcd630u+o);result=(uint32_t)state-4u;
    switch(state){
    case 4:case 12:{int32_t delay=(int16_t)W(0xbcd62eu+o);base=U(0xf57084u);
        sprite=base+(delay>=6?5u:(uint32_t)delay)*8u+direction;U(0xbcd5fcu+o)=sprite;return sprite;}
    case 8:base=U(0xf57074u);U(0xbcd5fcu+o)=base+direction;return result;
    case 9:case 10:case 13:case 21:case 23:
        counter=U(0xbcd740u+o);base=U(0xf57074u);sprite=base+counter*8u+direction;U(0xbcd5fcu+o)=sprite;return sprite;
    case 18:
        counter=U(0xbcd740u+o);base=U(0xf5708cu);U(0xbcd5fcu+o)=base+counter*8u+direction;return base;
    case 5:
        counter=U(0xbcd740u+o);base=U(0xf5707cu);sprite=base+counter*8u+direction;U(0xbcd5fcu+o)=sprite;return sprite;
    case 19:case 20:
        counter=U(0xbcd740u+o);base=U(0xf57094u);counter=(uint32_t)((int32_t)counter/2);
        sprite=base+counter*8u+direction;U(0xbcd5fcu+o)=sprite;return sprite;
    default:return result;
    }
}

__declspec(noinline) uint32_t restore_actor_update_lion(void)
{
    uint32_t actor=U(0xd056f8u),o=animal_offset(actor),kind=B(0xbcd602u+o),counter=U(0xbcd740u+o)+1u;
    uint32_t speed,cycle;int32_t group;uint8_t completed=0,animation,state,direction;
    U(0xbcd740u+o)=counter;speed=U(0x7090ecu+kind*56u);cycle=U(0xbcd73cu+o);
    if((int32_t)counter>=(int32_t)cycle){U(0xbcd740u+o)=0;animation=B(0xbcd5f9u+o);++animation;completed=1;B(0xbcd5f9u+o)=animation;}
    {uint32_t profile=U(0xeaac28u)*0x49d8u;B(0xbcd66au+o)=5;W(0xea5ceau+profile)=(uint16_t)(W(0xea5ceau+profile)+1u);}
    B(0xbcd604u+o)=0;B(0xbcd65du+o)=0;animation=B(0xbcd5f9u+o);group=(int16_t)W(0xbcd658u+o);
    ++animation;B(0xbcd5f9u+o)=animation;if(animation>=12u)B(0xbcd5f9u+o)=0;
    state=B(0xbcd630u+o);
    switch(state){
    case 5:
        U(0xbcd73cu+o)=11;FUN_0053b4a0((int32_t)actor,1,2);actor=U(0xd056f8u);
        FUN_004b17d0((int32_t)actor);actor=U(0xd056f8u);break;
    case 4:U(0xbcd73cu+o)=6;FUN_004b1d50((int32_t)actor);actor=U(0xd056f8u);break;
    case 24:
        W(0xbcd62eu+o)=(uint16_t)(W(0xbcd62eu+o)-1u);
        if((int16_t)W(0xbcd62eu+o)<=0){U(0xbcd740u+o)=0;U(0xbcd73cu+o)=8;B(0xbcd630u+o)=8;}
        U(0xbcd5fcu+o)=0;return 0;
    case 8:{
        uint32_t index=U(0xd3b1c4u),sample;struct animal_random_pair p;
        U(0xbcd73cu+o)=0;B(0xbcd609u+o)=B(0xbcd60au+o);sample=U(0xd3b374u);
        lion_history(index,sample);p=lion_read_shift();
        B(0xbcd630u+o)=(uint8_t)(((~p.a)&1u)|18u);
        index=U(0xd3b1c4u);lion_history(index,p.a&127u);p=animal_shift_random_pair(p);
        U(0xd3b384u)=p.a;U(0xd3b378u)=p.a&7u;U(0xd3b380u)=p.b;U(0xd3b364u)=p.b&32767u;
        U(0xd3b370u)=p.a&32767u;U(0xd3b368u)=p.b&127u;U(0xd3b36cu)=p.b&7u;U(0xd3b374u)=p.a&127u;
        W(0xbcd62eu+o)=(uint16_t)((p.a&32767u)%200u+150u);break;}
    case 19:
        U(0xbcd73cu+o)=8;W(0xbcd62eu+o)=(uint16_t)(W(0xbcd62eu+o)-1u);
        if((int16_t)W(0xbcd62eu+o)<=0)B(0xbcd630u+o)=8;
        break;
    case 18:{
        U(0xbcd73cu+o)=10;W(0xbcd62eu+o)=(uint16_t)(W(0xbcd62eu+o)-1u);
        if((int16_t)W(0xbcd62eu+o)<=0){
            uint32_t index=U(0xd3b1c4u),sample=U(0xd3b374u),g=(uint32_t)group*144u;
            struct animal_random_pair p;int32_t x,y,gx,gy,result;
            lion_history(index,sample);p=lion_read_shift();
            U(0xd3b384u)=p.a;U(0xd3b378u)=p.a&7u;U(0xd3b380u)=p.b;U(0xd3b364u)=p.b&32767u;
            U(0xd3b36cu)=p.b&7u;U(0xd3b374u)=p.a&127u;
            y=(int16_t)W(0xbcd60eu+o);x=(int16_t)W(0xbcd60cu+o);U(0xd3b370u)=p.a&32767u;
            U(0xd3b368u)=p.b&127u;gy=(int16_t)W(0xbcba04u+g);gx=(int16_t)W(0xbcba02u+g);
            result=FUN_004eab10(gx,gy,(int32_t)(p.a&15u),x,y,0);
            if(result>=0 && result<16){actor=U(0xd056f8u);FUN_004b3250((int32_t)actor);actor=U(0xd056f8u);lion_copy_group(actor,g,(uint32_t)result);}
            else{actor=U(0xd056f8u);W(0xbcd62eu+animal_offset(actor))=200;}
        }
        restore_actor_try_periodic_target((int32_t)actor);actor=U(0xd056f8u);break;}
    case 23:{
        uint32_t index=U(0xd3b1c4u),sample=U(0xd3b374u),g=(uint32_t)group*144u;
        struct animal_random_pair p;int32_t x,y,gx,gy,result;
        U(0xbcd73cu+o)=11;lion_history(index,sample);p=lion_read_shift();
        U(0xd3b384u)=p.a;U(0xd3b378u)=p.a&7u;U(0xd3b380u)=p.b;U(0xd3b364u)=p.b&32767u;
        U(0xd3b36cu)=p.b&7u;U(0xd3b374u)=p.a&127u;
        y=(int16_t)W(0xbcd60eu+o);x=(int16_t)W(0xbcd60cu+o);U(0xd3b370u)=p.a&32767u;
        U(0xd3b368u)=p.b&127u;gy=(int16_t)W(0xbcba04u+g);gx=(int16_t)W(0xbcba02u+g);
        result=FUN_004eab10(gx,gy,(int32_t)(p.a&15u),x,y,0);
        if(result>=0 && result<16){actor=U(0xd056f8u);FUN_004b3250((int32_t)actor);actor=U(0xd056f8u);lion_copy_group(actor,g,(uint32_t)result);}
        else{actor=U(0xd056f8u);o=animal_offset(actor);B(0xbcd630u+o)=18;W(0xbcd62eu+o)=200;}
        break;}
    case 20:
        U(0xbcd73cu+o)=8;restore_actor_try_periodic_target((int32_t)actor);actor=U(0xd056f8u);break;
    case 9:case 10:
        FUN_004a5da0((int32_t)actor,speed);actor=U(0xd056f8u);o=animal_offset(actor);direction=B(0xbcd609u+o);
        if(direction==9u){FUN_004b3250((int32_t)actor);actor=U(0xd056f8u);}
        else if(direction==10u){B(0xbcd609u+o)=B(0xbcd60au+o);B(0xbcd630u+o)=8;W(0xbcd62eu+o)=(uint16_t)((uint32_t)B(0xbcd65au+o)*5u);}
        else if(direction==8u){
            uint32_t index=U(0xd3b1c4u),sample=U(0xd3b374u),next;uint8_t previous=B(0xbcd60au+o);struct animal_random_pair p;
            U(0xd3b1c8u+index*4u)=sample;next=index+1u;B(0xbcd609u+o)=previous;U(0xd3b1c4u)=next;
            if((int32_t)next>=100)U(0xd3b1c4u)=0;
            p=lion_read_shift();
            if(p.a&1u){B(0xbcd630u+o)=18;U(0xbcd73cu+o)=10;}
            else{B(0xbcd630u+o)=19;U(0xbcd73cu+o)=4;}
            index=U(0xd3b1c4u);lion_history(index,p.a&127u);p=animal_shift_random_pair(p);
            U(0xd3b384u)=p.a;U(0xd3b378u)=p.a&7u;U(0xd3b380u)=p.b;U(0xd3b364u)=p.b&32767u;
            U(0xd3b370u)=p.a&32767u;U(0xd3b36cu)=p.b&7u;U(0xd3b368u)=p.b&127u;U(0xd3b374u)=p.a&127u;
            W(0xbcd62eu+o)=(uint16_t)((p.a&32767u)%200u+150u);
        }
        break;
    case 21:{
        int32_t linked;U(0xbcd73cu+o)=11;linked=(int16_t)W(0xbcd656u+o);
        W(0xbcd62eu+o)=(uint16_t)(W(0xbcd62eu+o)-1u);
        if((int16_t)W(0xbcd62eu+o)<=0 && completed){W(0xbcd62eu+o)=0;B(0xbcd663u+o)=0;B(0xbcd630u+o)=23;break;}
        if(linked){uint32_t linked_o=animal_offset((uint32_t)linked);
            if(lion_target_differs(o,linked_o) && completed){FUN_004b3250((int32_t)actor);actor=U(0xd056f8u);lion_copy_linked(actor,linked_o);}}
        FUN_004a5da0((int32_t)actor,speed);actor=U(0xd056f8u);o=animal_offset(actor);direction=B(0xbcd609u+o);
        if(direction==9u){FUN_004b3250((int32_t)actor);actor=U(0xd056f8u);}
        else if(direction==8u || direction==10u){B(0xbcd609u+o)=B(0xbcd60au+o);B(0xbcd630u+o)=23;W(0xbcd62eu+o)=(uint16_t)((uint32_t)B(0xbcd65au+o)*5u);}
        break;}
    case 13:
        U(0xbcd73cu+o)=11;FUN_004a5da0((int32_t)actor,speed*2u);actor=U(0xd056f8u);o=animal_offset(actor);direction=B(0xbcd609u+o);
        if(direction==9u){FUN_004b3250((int32_t)actor);actor=U(0xd056f8u);}
        else if(direction==8u || direction==10u){B(0xbcd609u+o)=B(0xbcd60au+o);B(0xbcd630u+o)=23;W(0xbcd62eu+o)=(uint16_t)((uint32_t)B(0xbcd65au+o)*5u);}
        if(completed){restore_actor_try_periodic_target((int32_t)actor);actor=U(0xd056f8u);}break;
    case 12:
        U(0xbcd73cu+o)=6;if(!W(0xbcd62eu+o)){FUN_004b5050((int32_t)actor);actor=U(0xd056f8u);}
        o=animal_offset(actor);W(0xbcd62eu+o)=(uint16_t)(W(0xbcd62eu+o)+1u);
        if((int16_t)W(0xbcd62eu+o)>500)B(0xbcd630u+o)=4;
        break;
    default:U(0xbcd73cu+o)=1;B(0xbcd630u+o)=8;break;
    }
    return lion_animation(actor);
}

static struct animal_random_pair asp_advance_random_history(uint32_t index,uint32_t sample)
{
    struct animal_random_pair p;
    U(0xd3b1c8u+index*4u)=sample;++index;U(0xd3b1c4u)=index;
    if((int32_t)index>=100)U(0xd3b1c4u)=0;
    p.a=U(0xd3b384u);p.b=U(0xd3b380u);return animal_shift_random_pair(p);
}
static uint8_t asp_linked_target_differs(uint32_t o,uint32_t linked)
{
    uint16_t x=W(0xbcd60cu+linked),target=W(0xbcd61cu+o);
    if(x!=target)return 1;
    x=W(0xbcd60eu+linked);target=W(0xbcd61eu+o);return x!=target;
}
static void asp_copy_linked_target(uint32_t actor,uint32_t linked)
{
    uint16_t x=W(0xbcd60cu+linked);uint32_t o=animal_offset(actor);
    W(0xbcd61cu+o)=x;W(0xbcd61eu+o)=W(0xbcd60eu+linked);
}
static void asp_copy_group_target(uint32_t actor,uint32_t g,uint32_t index)
{
    uint16_t x=W(0x5cdb78u+index*8u),y=W(0x5cdb7cu+index*8u);uint32_t o=animal_offset(actor);
    x=(uint16_t)(x+W(0xbcba02u+g));y=(uint16_t)(y+W(0xbcba04u+g));
    W(0xbcd61cu+o)=x;W(0xbcd61eu+o)=y;B(0xbcd630u+o)=9;
}
static uint32_t asp_animation(uint32_t actor)
{
    uint32_t o=animal_offset(actor),counter=U(0xbcd740u+o),cycle=U(0xbcd73cu+o),direction,rotation,base;uint8_t state;
    if((int32_t)counter>=(int32_t)cycle || (int32_t)counter<0)U(0xbcd740u+o)=0;
    direction=(uint32_t)(int32_t)(int8_t)B(0xbcd609u+o);rotation=U(0xbca2e0u);direction-=rotation;
    U(0xe94e00u)=direction;if((int32_t)direction<0){direction+=8u;U(0xe94e00u)=direction;}
    state=B(0xbcd630u+o);
    switch(state){
    case 4:case 12:{int32_t delay=(int16_t)W(0xbcd62eu+o);base=U(0xf57064u);U(0xbcd5fcu+o)=base+(delay>=6?5u:(uint32_t)delay);break;}
    case 9:case 10:case 13:case 21:counter=U(0xbcd740u+o);base=U(0xf5704cu);U(0xbcd5fcu+o)=base+counter*8u+direction;break;
    case 5:counter=U(0xbcd740u+o);base=U(0xf57054u);U(0xbcd5fcu+o)=base+counter*8u+direction;break;
    case 20:base=U(0xf5706cu);U(0xbcd5fcu+o)=base+direction;break;
    case 18:case 19:counter=U(0xbcd740u+o);base=U(0xf5706cu);U(0xbcd5fcu+o)=base+counter*8u+direction;break;
    default:break;
    }
    return o;
}
__declspec(noinline) uint32_t restore_actor_update_asp(void)
{
    uint32_t actor=U(0xd056f8u),o=animal_offset(actor),kind=B(0xbcd602u+o),counter=U(0xbcd740u+o)+1u,speed,cycle;
    int32_t group,linked;uint8_t completed=0,animation,state,direction;
    U(0xbcd740u+o)=counter;speed=U(0x7090ecu+kind*56u);cycle=U(0xbcd73cu+o);
    if((int32_t)counter>=(int32_t)cycle){U(0xbcd740u+o)=0;animation=B(0xbcd5f9u+o);++animation;completed=1;B(0xbcd5f9u+o)=animation;}
    {uint32_t profile=U(0xeaac28u)*0x49d8u;B(0xbcd66au+o)=5;W(0xea5ceau+profile)=(uint16_t)(W(0xea5ceau+profile)+1u);}
    B(0xbcd604u+o)=0;B(0xbcd65du+o)=0;animation=B(0xbcd5f9u+o);group=(int16_t)W(0xbcd658u+o);
    ++animation;B(0xbcd5f9u+o)=animation;if(animation>=12u)B(0xbcd5f9u+o)=0;
    state=B(0xbcd630u+o);
    switch(state){
    case 5:U(0xbcd73cu+o)=6;FUN_0053b4a0((int32_t)actor,3,2);actor=U(0xd056f8u);FUN_004b17d0((int32_t)actor);actor=U(0xd056f8u);break;
    case 4:U(0xbcd73cu+o)=6;FUN_004b1d50((int32_t)actor);actor=U(0xd056f8u);break;
    case 24:{uint32_t result=20;W(0xbcd62eu+o)=(uint16_t)(W(0xbcd62eu+o)-1u);
        if((int16_t)W(0xbcd62eu+o)<=0){result=8;U(0xbcd740u+o)=0;U(0xbcd73cu+o)=8;B(0xbcd630u+o)=8;}
        U(0xbcd5fcu+o)=0;return result;}
    case 8:{
        uint32_t sample=U(0xd3b374u),index;struct animal_random_pair p;uint8_t previous;
        U(0xbcd73cu+o)=0;previous=B(0xbcd60au+o);B(0xbcd609u+o)=previous;index=U(0xd3b1c4u);
        p=asp_advance_random_history(index,sample);
        state=(uint8_t)(((~p.a)&1u)|18u);index=U(0xd3b1c4u);U(0xd3b1c8u+index*4u)=p.a&127u;++index;
        B(0xbcd630u+o)=state;U(0xd3b1c4u)=index;if((int32_t)index>=100)U(0xd3b1c4u)=0;
        p=animal_shift_random_pair(p);
        U(0xd3b384u)=p.a;U(0xd3b378u)=p.a&7u;U(0xd3b380u)=p.b;U(0xd3b364u)=p.b&32767u;
        U(0xd3b370u)=p.a&32767u;U(0xd3b368u)=p.b&127u;U(0xd3b36cu)=p.b&7u;
        U(0xd3b374u)=p.a&127u;W(0xbcd62eu+o)=(uint16_t)((p.a&32767u)%100u+50u);break;
    }
    case 19:
        U(0xbcd73cu+o)=6;W(0xbcd62eu+o)=(uint16_t)(W(0xbcd62eu+o)-1u);if((int16_t)W(0xbcd62eu+o)<=0)B(0xbcd630u+o)=8;break;
    case 18:
        U(0xbcd73cu+o)=6;W(0xbcd62eu+o)=(uint16_t)(W(0xbcd62eu+o)-1u);
        if((int16_t)W(0xbcd62eu+o)<=0){
            uint32_t index=U(0xd3b1c4u),sample=U(0xd3b374u),g=(uint32_t)group*144u;
            struct animal_random_pair p=asp_advance_random_history(index,sample);int32_t x,y,gx,gy,result;
            U(0xd3b384u)=p.a;U(0xd3b378u)=p.a&7u;U(0xd3b380u)=p.b;U(0xd3b364u)=p.b&32767u;
            U(0xd3b36cu)=p.b&7u;U(0xd3b374u)=p.a&127u;y=(int16_t)W(0xbcd60eu+o);x=(int16_t)W(0xbcd60cu+o);
            U(0xd3b370u)=p.a&32767u;U(0xd3b368u)=p.b&127u;gy=(int16_t)W(0xbcba04u+g);gx=(int16_t)W(0xbcba02u+g);
            result=FUN_004eab10(gx,gy,(int32_t)(p.a&15u),x,y,0);
            if(result>=0 && result<16){actor=U(0xd056f8u);FUN_004b3250((int32_t)actor);actor=U(0xd056f8u);asp_copy_group_target(actor,g,(uint32_t)result);U(0xbcd740u+animal_offset(actor))=0;restore_actor_try_periodic_target((int32_t)actor);actor=U(0xd056f8u);break;}
            index=U(0xd3b1c4u);sample=U(0xd3b374u);p=asp_advance_random_history(index,sample);
            U(0xd3b384u)=p.a;U(0xd3b378u)=p.a&7u;U(0xd3b380u)=p.b;U(0xd3b364u)=p.b&32767u;
            U(0xd3b370u)=p.a&32767u;U(0xd3b368u)=p.b&127u;U(0xd3b36cu)=p.b&7u;
            actor=U(0xd056f8u);o=animal_offset(actor);U(0xd3b374u)=p.a&127u;W(0xbcd62eu+o)=(uint16_t)((p.a&32767u)%100u);
        }
        restore_actor_try_periodic_target((int32_t)actor);actor=U(0xd056f8u);break;
    case 23:{
        uint32_t index=U(0xd3b1c4u),sample=U(0xd3b374u),g=(uint32_t)group*144u;struct animal_random_pair p;int32_t x,y,gx,gy,result;
        U(0xbcd73cu+o)=1;p=asp_advance_random_history(index,sample);
        U(0xd3b384u)=p.a;U(0xd3b378u)=p.a&7u;U(0xd3b380u)=p.b;U(0xd3b364u)=p.b&32767u;
        U(0xd3b36cu)=p.b&7u;U(0xd3b374u)=p.a&127u;y=(int16_t)W(0xbcd60eu+o);x=(int16_t)W(0xbcd60cu+o);
        U(0xd3b370u)=p.a&32767u;U(0xd3b368u)=p.b&127u;gy=(int16_t)W(0xbcba04u+g);gx=(int16_t)W(0xbcba02u+g);
        result=FUN_004eab10(gx,gy,(int32_t)(p.a&15u),x,y,0);
        if(result>=0 && result<16){actor=U(0xd056f8u);FUN_004b3250((int32_t)actor);actor=U(0xd056f8u);asp_copy_group_target(actor,g,(uint32_t)result);}
        else{
            actor=U(0xd056f8u);index=U(0xd3b1c4u);sample=U(0xd3b374u);o=animal_offset(actor);
            U(0xd3b1c8u+index*4u)=sample;++index;B(0xbcd630u+o)=18;U(0xd3b1c4u)=index;if((int32_t)index>=100)U(0xd3b1c4u)=0;
            p.a=U(0xd3b384u);p.b=U(0xd3b380u);p=animal_shift_random_pair(p);
            U(0xd3b384u)=p.a;U(0xd3b378u)=p.a&7u;U(0xd3b380u)=p.b;U(0xd3b364u)=p.b&32767u;
            U(0xd3b370u)=p.a&32767u;U(0xd3b368u)=p.b&127u;U(0xd3b36cu)=p.b&7u;
            U(0xd3b374u)=p.a&127u;W(0xbcd62eu+o)=(uint16_t)((p.a&32767u)%100u);
        }
        break;
    }
    case 20:U(0xbcd73cu+o)=1;restore_actor_try_periodic_target((int32_t)actor);actor=U(0xd056f8u);break;
    case 9:case 10:
        U(0xbcd73cu+o)=12;FUN_004a5da0((int32_t)actor,speed);actor=U(0xd056f8u);o=animal_offset(actor);direction=B(0xbcd609u+o);
        if(direction==9u){FUN_004b3250((int32_t)actor);actor=U(0xd056f8u);break;}
        if(direction==10u){B(0xbcd609u+o)=B(0xbcd60au+o);B(0xbcd630u+o)=8;W(0xbcd62eu+o)=(uint16_t)((uint32_t)B(0xbcd65au+o)*5u);break;}
        if(direction==8u){
            uint32_t index=U(0xd3b1c4u),sample=U(0xd3b374u);uint8_t previous=B(0xbcd60au+o);struct animal_random_pair p;
            U(0xd3b1c8u+index*4u)=sample;++index;B(0xbcd609u+o)=previous;U(0xd3b1c4u)=index;if((int32_t)index>=100)U(0xd3b1c4u)=0;
            p.a=U(0xd3b384u);p.b=U(0xd3b380u);p=animal_shift_random_pair(p);
            B(0xbcd630u+o)=(p.a&1u)?18:19;index=U(0xd3b1c4u);U(0xd3b1c8u+index*4u)=p.a&127u;++index;U(0xd3b1c4u)=index;if((int32_t)index>=100)U(0xd3b1c4u)=0;
            p=animal_shift_random_pair(p);
            U(0xd3b384u)=p.a;U(0xd3b378u)=p.a&7u;U(0xd3b380u)=p.b;U(0xd3b364u)=p.b&32767u;
            U(0xd3b370u)=p.a&32767u;U(0xd3b36cu)=p.b&7u;U(0xd3b368u)=p.b&127u;U(0xd3b374u)=p.a&127u;
            W(0xbcd62eu+o)=(uint16_t)((p.a&32767u)%100u);U(0xbcd740u+o)=0;
        }
        break;
    case 21:
        U(0xbcd73cu+o)=12;linked=(int16_t)W(0xbcd656u+o);W(0xbcd62eu+o)=(uint16_t)(W(0xbcd62eu+o)-1u);
        if((int16_t)W(0xbcd62eu+o)<=0 && completed){W(0xbcd62eu+o)=0;B(0xbcd663u+o)=0;B(0xbcd630u+o)=23;break;}
        if(linked && asp_linked_target_differs(o,animal_offset((uint32_t)linked)) && completed){FUN_004b3250((int32_t)actor);actor=U(0xd056f8u);asp_copy_linked_target(actor,animal_offset((uint32_t)linked));}
        FUN_004a5da0((int32_t)actor,speed);actor=U(0xd056f8u);o=animal_offset(actor);direction=B(0xbcd609u+o);
        if(direction==9u){FUN_004b3250((int32_t)actor);actor=U(0xd056f8u);break;}
        if(direction==8u || direction==10u){B(0xbcd609u+o)=B(0xbcd60au+o);B(0xbcd630u+o)=23;W(0xbcd62eu+o)=(uint16_t)((uint32_t)B(0xbcd65au+o)*5u);}
        break;
    case 13:
        U(0xbcd73cu+o)=12;FUN_004a5da0((int32_t)actor,speed*2u);actor=U(0xd056f8u);o=animal_offset(actor);direction=B(0xbcd609u+o);
        if(direction==9u){FUN_004b3250((int32_t)actor);actor=U(0xd056f8u);}
        else if(direction==8u || direction==10u){B(0xbcd609u+o)=B(0xbcd60au+o);B(0xbcd630u+o)=23;W(0xbcd62eu+o)=(uint16_t)((uint32_t)B(0xbcd65au+o)*5u);}
        if(completed){restore_actor_try_periodic_target((int32_t)actor);actor=U(0xd056f8u);}break;
    case 12:
        U(0xbcd73cu+o)=6;if(!W(0xbcd62eu+o)){FUN_004b5050((int32_t)actor);actor=U(0xd056f8u);}
        o=animal_offset(actor);W(0xbcd62eu+o)=(uint16_t)(W(0xbcd62eu+o)+1u);if((int16_t)W(0xbcd62eu+o)>500)B(0xbcd630u+o)=4;break;
    default:U(0xbcd73cu+o)=0;B(0xbcd630u+o)=8;break;
    }
    return asp_animation(actor);
}

static struct animal_random_pair birds_history_draw(uint32_t index,uint32_t sample)
{
    struct animal_random_pair p;
    U(0xd3b1c8u+index*4u)=sample;++index;U(0xd3b1c4u)=index;
    if((int32_t)index>=100)U(0xd3b1c4u)=0;
    p.a=U(0xd3b384u);p.b=U(0xd3b380u);return animal_shift_random_pair(p);
}
static uint32_t birds_animation(uint32_t actor)
{
    uint32_t o=animal_offset(actor),rotation=U(0xbca2e0u),direction,base;uint8_t state;
    direction=(uint32_t)(int32_t)(int8_t)B(0xbcd609u+o)-rotation;
    U(0xe94e00u)=direction;if((int32_t)direction<0){direction+=8u;U(0xe94e00u)=direction;}
    state=B(0xbcd630u+o);
    if(state==4u || state==12u){
        int32_t delay=(int16_t)W(0xbcd62eu+o);
        if(delay>=25)U(0xbcd5fcu+o)=0;
        else{base=U(0xf56084u);U(0xbcd5fcu+o)=base+(uint32_t)delay;}
    }else{
        uint32_t animation;base=U(0xf5607cu);animation=B(0xbcd5f9u+o);
        U(0xbcd5fcu+o)=base+(animation>>1);
    }
    return o;
}
__declspec(noinline) uint32_t restore_actor_update_birds(void)
{
    uint32_t actor=U(0xd056f8u),o=animal_offset(actor),kind=B(0xbcd602u+o),counter=U(0xbcd740u+o)+1u;
    uint32_t speed,building,cycle;int32_t group;uint8_t animation,state,direction;
    U(0xbcd740u+o)=counter;speed=U(0x7090ecu+kind*56u);
    building=U(0xbcd730u+o)*264u;
    if(W(0x7999e8u+building)!=115u || B(0x799a7eu+building)==0u ||
        B(0x7999d8u+building)!=1u || W(0x799a18u+building)==0u)U(0xbcd730u+o)=0;
    counter=U(0xbcd740u+o);cycle=U(0xbcd73cu+o);
    if((int32_t)counter>=(int32_t)cycle){U(0xbcd740u+o)=0;animation=B(0xbcd5f9u+o);++animation;B(0xbcd5f9u+o)=animation;}
    {uint32_t profile=U(0xeaac28u)*0x49d8u;B(0xbcd66au+o)=5;W(0xea5ceau+profile)=(uint16_t)(W(0xea5ceau+profile)+1u);}
    B(0xbcd604u+o)=0;B(0xbcd65du+o)=0;animation=B(0xbcd5f9u+o);group=(int16_t)W(0xbcd658u+o);
    ++animation;B(0xbcd5f9u+o)=animation;if(animation>=50u)B(0xbcd5f9u+o)=0;
    state=B(0xbcd630u+o);
    switch(state){
    case 5:FUN_004b17d0((int32_t)actor);actor=U(0xd056f8u);break;
    case 4:FUN_004b1d50((int32_t)actor);actor=U(0xd056f8u);break;
    case 24:
        W(0xbcd62eu+o)=(uint16_t)(W(0xbcd62eu+o)-1u);
        if((int16_t)W(0xbcd62eu+o)<=0){U(0xbcd740u+o)=0;U(0xbcd73cu+o)=8;B(0xbcd630u+o)=8;}
        U(0xbcd5fcu+o)=0;return o;
    case 8:case 18:
        W(0xbcd62eu+o)=(uint16_t)(W(0xbcd62eu+o)-1u);
        if((int16_t)W(0xbcd62eu+o)<=0){
            uint32_t index=U(0xd3b1c4u),sample=U(0xd3b374u),g=(uint32_t)group*144u;
            struct animal_random_pair p=birds_history_draw(index,sample);int32_t x,y,gx,gy,result;
            U(0xd3b384u)=p.a;U(0xd3b378u)=p.a&7u;U(0xd3b380u)=p.b;U(0xd3b364u)=p.b&32767u;
            U(0xd3b36cu)=p.b&7u;y=(int16_t)W(0xbcd60eu+o);U(0xd3b374u)=p.a&127u;U(0xd3b368u)=p.b&127u;
            x=(int16_t)W(0xbcd60cu+o);U(0xd3b370u)=p.a&32767u;
            gy=(int16_t)W(0xbcba00u+g);gx=(int16_t)W(0xbcb9feu+g);
            result=FUN_004eab10(gx,gy,(int32_t)(p.a&15u),x,y,1);
            actor=U(0xd056f8u);o=animal_offset(actor);
            if(result>=0 && result<16){
                uint16_t vx=W(0x5cdb78u+(uint32_t)result*8u),vy=W(0x5cdb7cu+(uint32_t)result*8u);
                vx=(uint16_t)(vx+W(0xbcb9feu+g));vy=(uint16_t)(vy+W(0xbcba00u+g));
                W(0xbcd61cu+o)=vx;W(0xbcd61eu+o)=vy;B(0xbcd630u+o)=10;
            }else W(0xbcd62eu+o)=100;
        }
        break;
    case 9:case 10:
        FUN_004a5da0((int32_t)actor,speed);actor=U(0xd056f8u);o=animal_offset(actor);direction=B(0xbcd609u+o);
        if(direction==9u){FUN_004b3250((int32_t)actor);actor=U(0xd056f8u);}
        else if(direction==10u)B(0xbcd606u+o)=2;
        else if(direction==8u){
            uint32_t index=U(0xd3b1c4u),sample=U(0xd3b374u);uint8_t previous=B(0xbcd60au+o);struct animal_random_pair p;
            U(0xd3b1c8u+index*4u)=sample;++index;B(0xbcd609u+o)=previous;B(0xbcd630u+o)=18;
            U(0xd3b1c4u)=index;if((int32_t)index>=100)U(0xd3b1c4u)=0;
            p.a=U(0xd3b384u);p.b=U(0xd3b380u);p=animal_shift_random_pair(p);
            U(0xd3b384u)=p.a;U(0xd3b378u)=p.a&7u;U(0xd3b380u)=p.b;U(0xd3b364u)=p.b&32767u;
            U(0xd3b370u)=p.a&32767u;U(0xd3b368u)=p.b&127u;U(0xd3b36cu)=p.b&7u;
            U(0xd3b374u)=p.a&127u;W(0xbcd62eu+o)=(uint16_t)((p.a&32767u)%150u+150u);
        }
        break;
    case 14:case 15:(void)FUN_004eb000((int32_t)actor);actor=U(0xd056f8u);break;
    case 16:case 17:
        FUN_004a5da0((int32_t)actor,speed*2u);actor=U(0xd056f8u);o=animal_offset(actor);direction=B(0xbcd609u+o);
        if(direction==9u){FUN_004b3250((int32_t)actor);actor=U(0xd056f8u);}
        else if(direction==8u || direction==10u){B(0xbcd609u+o)=B(0xbcd60au+o);B(0xbcd630u+o)=14;}
        break;
    case 12:
        if(!W(0xbcd62eu+o)){FUN_004b5050((int32_t)actor);actor=U(0xd056f8u);}
        o=animal_offset(actor);W(0xbcd62eu+o)=(uint16_t)(W(0xbcd62eu+o)+1u);if((int16_t)W(0xbcd62eu+o)>500)B(0xbcd630u+o)=4;break;
    default:B(0xbcd630u+o)=8;break;
    }
    return birds_animation(actor);
}

static void antelope_animation(uint32_t actor)
{
    uint32_t o=animal_offset(actor),rotation=U(0xbca2e0u),direction,base,frame,sprite;
    uint8_t state;
    direction=(uint32_t)(int32_t)(int8_t)B(0xbcd609u+o)-rotation;U(0xe94e00u)=direction;
    if((int32_t)direction<0){direction+=8u;U(0xe94e00u)=direction;}
    state=B(0xbcd630u+o);
    switch(state){
    case 4:base=U(0xf56eb4u);sprite=base+7u;break;
    case 12:{int32_t delay=(int16_t)W(0xbcd62eu+o);base=U(0xf56eb4u);sprite=base+(delay>=8?7u:(uint32_t)delay);break;}
    case 19:frame=U(0xbcd740u+o);base=U(0xf56eccu);sprite=base+frame*8u+direction;break;
    case 18:frame=U(0xbcd740u+o);base=U(0xf56ec4u);sprite=base+frame*8u+direction;break;
    case 8:case 14:case 15:base=U(0xf56eacu);sprite=base+direction;break;
    case 16:case 17:frame=U(0xbcd740u+o);base=U(0xf56ed4u);sprite=base+frame*8u+direction;break;
    case 5:frame=U(0xbcd740u+o);base=U(0xf56ebcu);sprite=base+frame*8u+direction;break;
    default:frame=U(0xbcd740u+o);base=U(0xf56eacu);sprite=base+frame*8u+direction;break;
    }
    U(0xbcd5fcu+o)=sprite;
}

/* Ordinary void callback. Native incidental EAX is outside this contract;
 * the complete original RAW callback/model must remain available. */
__declspec(noinline) void restore_actor_update_antelope(void)
{
    uint32_t actor=U(0xd056f8u),o=animal_offset(actor),kind=B(0xbcd602u+o);
    uint32_t profile,speed,counter,building,b,cycle;int32_t group;uint8_t completed=0,state;
    B(0xbcd66au+o)=5;B(0xbcd604u+o)=0;B(0xbcd65du+o)=0;
    profile=U(0xeaac28u)*0x49d8u;W(0xea5ceau+profile)=(uint16_t)(W(0xea5ceau+profile)+1u);
    speed=U(0x7090ecu+kind*56u);group=(int16_t)W(0xbcd658u+o);
    counter=U(0xbcd740u+o)+1u;U(0xbcd740u+o)=counter;
    building=U(0xbcd730u+o);b=building*264u;
    if(W(0x7999e8u+b)!=0x73u||!B(0x799a7eu+b)||B(0x7999d8u+b)!=1u||!W(0x799a18u+b))U(0xbcd730u+o)=0;
    counter=U(0xbcd740u+o);cycle=U(0xbcd73cu+o);
    if((int32_t)counter>=(int32_t)cycle){uint8_t animation;U(0xbcd740u+o)=0;animation=B(0xbcd5f9u+o);++animation;completed=1;B(0xbcd5f9u+o)=animation;}
    state=B(0xbcd630u+o);
    switch(state){
    case 5:U(0xbcd73cu+o)=7;FUN_004b17d0((int32_t)actor);actor=U(0xd056f8u);break;
    case 4:U(0xbcd73cu+o)=8;FUN_004b1d50((int32_t)actor);actor=U(0xd056f8u);break;
    case 24:{
        W(0xbcd62eu+o)=(uint16_t)(W(0xbcd62eu+o)-1u);
        if((int16_t)W(0xbcd62eu+o)<=0){U(0xbcd740u+o)=0;U(0xbcd73cu+o)=8;B(0xbcd630u+o)=8;}
        U(0xbcd5fcu+o)=0;B(0xbcd65du+o)=1;return;
    }
    case 8:{
        uint32_t index=U(0xd3b1c4u),previous=U(0xd3b374u),sample;struct animal_random_pair p;
        U(0xd3b1c8u+index*4u)=previous;++index;if((int32_t)index>=100)index=0;
        p.a=U(0xd3b384u);p.b=U(0xd3b380u);p=animal_shift_random_pair(p);
        U(0xd3b1c8u+index*4u)=p.a&127u;++index;
        B(0xbcd630u+o)=(uint8_t)((~p.a&1u)|18u);U(0xbcd740u+o)=0;U(0xbcd73cu+o)=8;
        U(0xd3b1c4u)=index;if((int32_t)index>=100)U(0xd3b1c4u)=0;
        p=animal_shift_random_pair(p);
        U(0xd3b384u)=p.a;U(0xd3b378u)=p.a&7u;U(0xd3b380u)=p.b;U(0xd3b364u)=p.b&32767u;
        sample=p.a&32767u;U(0xd3b370u)=sample;U(0xd3b368u)=p.b&127u;U(0xd3b36cu)=p.b&7u;U(0xd3b374u)=p.a&127u;
        W(0xbcd62eu+o)=(uint16_t)(sample%150u+150u);return;
    }
    case 18:{int32_t delay;
        U(0xbcd73cu+o)=8;W(0xbcd62eu+o)=(uint16_t)(W(0xbcd62eu+o)-1u);delay=(int16_t)W(0xbcd62eu+o);
        if(delay<0){W(0xbcd62eu+o)=0;delay=0;}
        if(delay<=0&&completed){
            uint32_t index=U(0xd3b1c4u),previous=U(0xd3b374u),g=(uint32_t)group*144u;
            struct animal_random_pair p;int32_t x,y,gx,gy,result;
            U(0xd3b1c8u+index*4u)=previous;++index;U(0xd3b1c4u)=index;if((int32_t)index>=100)U(0xd3b1c4u)=0;
            p.a=U(0xd3b384u);p.b=U(0xd3b380u);p=animal_shift_random_pair(p);
            U(0xd3b384u)=p.a;U(0xd3b378u)=p.a&7u;U(0xd3b380u)=p.b;U(0xd3b364u)=p.b&32767u;
            U(0xd3b36cu)=p.b&7u;U(0xd3b374u)=p.a&127u;y=(int16_t)W(0xbcd60eu+o);x=(int16_t)W(0xbcd60cu+o);
            U(0xd3b370u)=p.a&32767u;U(0xd3b368u)=p.b&127u;gy=(int16_t)W(0xbcba04u+g);gx=(int16_t)W(0xbcba02u+g);
            result=FUN_004eab10(gx,gy,(int32_t)(p.a&15u),x,y,1);
            if(result>=0&&result<16){uint16_t dx,dy;actor=U(0xd056f8u);o=animal_offset(actor);
                dx=W(0x5cdb78u+(uint32_t)result*8u);dy=W(0x5cdb7cu+(uint32_t)result*8u);
                dx=(uint16_t)(dx+W(0xbcba02u+g));dy=(uint16_t)(dy+W(0xbcba04u+g));
                W(0xbcd61cu+o)=dx;W(0xbcd61eu+o)=dy;B(0xbcd630u+o)=10;return;
            }
            actor=U(0xd056f8u);W(0xbcd62eu+animal_offset(actor))=100;
        }
        if(completed){(void)FUN_004eb000((int32_t)actor);actor=U(0xd056f8u);}break;
    }
    case 19:
        U(0xbcd73cu+o)=8;W(0xbcd62eu+o)=(uint16_t)(W(0xbcd62eu+o)-1u);
        if((int16_t)W(0xbcd62eu+o)<=0){if(completed){B(0xbcd630u+o)=18;return;}}
        else if(completed){(void)FUN_004eb000((int32_t)actor);actor=U(0xd056f8u);}break;
    case 9:case 10:{uint8_t direction;
        U(0xbcd73cu+o)=12;FUN_004a5da0((int32_t)actor,speed);actor=U(0xd056f8u);o=animal_offset(actor);direction=B(0xbcd609u+o);
        if(direction==9u){FUN_004b3250((int32_t)actor);actor=U(0xd056f8u);}
        else if(direction==10u){uint16_t delay;B(0xbcd609u+o)=B(0xbcd60au+o);B(0xbcd630u+o)=8;
            delay=(uint16_t)((uint32_t)B(0xbcd65au+o)*5u);
            W(0xbcd62eu+o)=delay;U(0xbcd740u+o)=0;return;
        }else if(direction==8u){
            uint32_t index=U(0xd3b1c4u),previous=U(0xd3b374u),sample;uint8_t old_direction=B(0xbcd60au+o);struct animal_random_pair p;
            U(0xd3b1c8u+index*4u)=previous;++index;B(0xbcd609u+o)=old_direction;U(0xd3b1c4u)=index;if((int32_t)index>=100)U(0xd3b1c4u)=0;
            p.a=U(0xd3b384u);p.b=U(0xd3b380u);p=animal_shift_random_pair(p);B(0xbcd630u+o)=(p.a&1u)?18:19;
            index=U(0xd3b1c4u);U(0xd3b1c8u+index*4u)=p.a&127u;++index;U(0xd3b1c4u)=index;if((int32_t)index>=100)U(0xd3b1c4u)=0;
            p=animal_shift_random_pair(p);
            U(0xd3b384u)=p.a;U(0xd3b378u)=p.a&7u;U(0xd3b380u)=p.b;U(0xd3b364u)=p.b&32767u;
            sample=p.a&32767u;U(0xd3b370u)=sample;U(0xd3b36cu)=p.b&7u;U(0xd3b368u)=p.b&127u;U(0xd3b374u)=p.a&127u;
            W(0xbcd62eu+o)=(uint16_t)(sample%150u+150u);break;
        }
        if(completed){(void)FUN_004eb000((int32_t)actor);actor=U(0xd056f8u);}break;
    }
    case 14:case 15:U(0xbcd73cu+o)=1;(void)FUN_004eb000((int32_t)actor);actor=U(0xd056f8u);break;
    case 16:case 17:{uint8_t direction;
        U(0xbcd73cu+o)=8;counter=U(0xbcd740u+o)+1u;U(0xbcd740u+o)=counter;if((int32_t)counter>=12)U(0xbcd740u+o)=0;
        FUN_004a5da0((int32_t)actor,speed*2u);actor=U(0xd056f8u);o=animal_offset(actor);direction=B(0xbcd609u+o);
        if(direction==9u){FUN_004b3250((int32_t)actor);actor=U(0xd056f8u);}
        else if(direction==10u||direction==8u){B(0xbcd609u+o)=B(0xbcd60au+o);B(0xbcd630u+o)=14;U(0xbcd740u+o)=0;return;}break;
    }
    case 12:
        if(!W(0xbcd62eu+o)){FUN_004b5050((int32_t)actor);actor=U(0xd056f8u);}o=animal_offset(actor);
        W(0xbcd62eu+o)=(uint16_t)(W(0xbcd62eu+o)+1u);if((int16_t)W(0xbcd62eu+o)>500)B(0xbcd630u+o)=4;break;
    default:B(0xbcd630u+o)=8;break;
    }
    antelope_animation(actor);
}


#define OSTRICH_H(a) ((int32_t)(int16_t)W(a))
static uint32_t ostrich_actor(uint32_t id) { return 0xbcd600u+id*388u; }
static void ostrich_shift(uint32_t *a,uint32_t *b)
{
    uint32_t i,x=*a,y=*b;
    for(i=0;i<31u;++i) {
        uint32_t p=((x>>4)^x)&1u,q=((y>>4)^y)&1u;
        x=(x>>1)|(p<<30);y=(y>>1)|(q<<30);
    }
    *a=x;*b=y;
}

/* State8 retains the first next-history index locally. Neither RNG state is
 * published/reloaded between the two shift phases. Returns second A15/150. */
static uint32_t ostrich_initial_wait(uint32_t actor)
{
    uint32_t index=U(0xd3b1c4u),a,b,choice,remainder;
    U(0xd3b1c8u+index*4u)=U(0xd3b374u);
    ++index;if((int32_t)index>=100)index=0;
    a=U(0xd3b384u);b=U(0xd3b380u);ostrich_shift(&a,&b);
    choice=a&0x7fffu;
    U(0xd3b1c8u+index*4u)=a&0x7fu;
    ++index;
    B(actor+0x30u)=(uint8_t)((~choice&1u)|0x12u);
    U(actor+0x140u)=0;
    U(actor+0x13cu)=8;
    U(0xd3b1c4u)=index;
    if((int32_t)index>=100)U(0xd3b1c4u)=0;
    ostrich_shift(&a,&b);
    U(0xd3b384u)=a;U(0xd3b378u)=a&7u;
    U(0xd3b380u)=b;U(0xd3b364u)=b&0x7fffu;
    choice=a&0x7fffu;
    U(0xd3b370u)=choice;U(0xd3b368u)=b&0x7fu;U(0xd3b36cu)=b&7u;
    remainder=choice%150u;
    U(0xd3b374u)=a&0x7fu;
    W(actor+0x2eu)=(uint16_t)(remainder+150u);
    return choice/150u;
}

/* Direction8 has a different history/index/store phase from state8. */
static void ostrich_moving_wait(uint32_t actor)
{
    uint32_t index=U(0xd3b1c4u),a,b,choice;
    uint32_t previous=U(0xd3b374u);
    uint8_t direction=B(actor+10u);
    U(0xd3b1c8u+index*4u)=previous;++index;
    B(actor+9u)=direction;
    U(0xd3b1c4u)=index;if((int32_t)index>=100)U(0xd3b1c4u)=0;
    a=U(0xd3b384u);b=U(0xd3b380u);ostrich_shift(&a,&b);
    B(actor+0x30u)=(uint8_t)((a&1u)?18u:19u);
    index=U(0xd3b1c4u);U(0xd3b1c8u+index*4u)=a&0x7fu;++index;
    U(0xd3b1c4u)=index;if((int32_t)index>=100)U(0xd3b1c4u)=0;
    ostrich_shift(&a,&b);
    U(0xd3b384u)=a;U(0xd3b378u)=a&7u;
    U(0xd3b380u)=b;U(0xd3b364u)=b&0x7fffu;
    choice=a&0x7fffu;
    U(0xd3b370u)=choice;U(0xd3b36cu)=b&7u;U(0xd3b368u)=b&0x7fu;
    U(0xd3b374u)=a&0x7fu;
    W(actor+0x2eu)=(uint16_t)(choice%150u+150u);
}

static uint32_t ostrich_image(uint32_t id)
{
    uint32_t offset=id*388u,actor=0xbcd600u+offset,rotation=U(0xbca2e0u),heading;
    uint32_t counter,cycle,sprite,base;uint8_t state;
    int16_t delay;
    heading=(uint32_t)(int32_t)(int8_t)B(actor+9u)-rotation;
    U(0xe94e00u)=heading;
    if((int32_t)heading<0) {
        heading+=8u;U(0xe94e00u)=heading;
        if((int32_t)heading<0) {heading+=((7u-heading)>>3)*8u;U(0xe94e00u)=heading;}
    }
    if((int32_t)heading>=8) {heading-=(heading>>3)*8u;U(0xe94e00u)=heading;}
    counter=U(actor+0x140u);cycle=U(actor+0x13cu);
    if((int32_t)counter>(int32_t)cycle)U(actor+0x140u)=counter-cycle;
    counter=U(actor+0x140u);if((int32_t)counter<0)U(actor+0x140u)=0;
    state=B(actor+0x30u);
    switch(state) {
    case 4:sprite=U(0xf561d4u)+7u;break;
    case 12:
        delay=OSTRICH_H(actor+0x2eu);base=U(0xf561d4u);
        sprite=base+(delay<8?(uint32_t)(int32_t)delay:7u);break;
    case 8:case 18:
        counter=U(actor+0x140u);base=U(0xf561e4u);sprite=base+counter*8u+heading;break;
    case 19:
        counter=U(actor+0x140u);base=U(0xf561ecu);sprite=base+counter*8u+heading;break;
    case 14:case 15:sprite=U(0xf561ecu)+heading;break;
    case 5:
        counter=U(actor+0x140u);base=U(0xf561dcu);sprite=base+counter*8u+heading;break;
    default:
        counter=U(actor+0x140u);base=U(0xf561ccu);sprite=base+counter*8u+heading;break;
    }
    U(actor-4u)=sprite;return offset;
}

__declspec(noinline) void restore_actor_update_ostrich(void)
{
    uint32_t id=U(0xd056f8u),a=ostrich_actor(id),kind=B(a+2u),speed,building,b,profile,counter,cycle;
    int32_t group;uint8_t due,state,direction;
    B(a+0x6au)=5;B(a+4u)=0;B(a+0x5du)=0;
    profile=U(0xeaac28u)*0x49d8u+0xea5ceau;W(profile)=(uint16_t)(W(profile)+1u);
    speed=U(0x7090ecu+kind*56u);group=OSTRICH_H(a+0x58u);
    counter=U(a+0x140u)+1u;U(a+0x140u)=counter;
    building=U(a+0x130u);b=0x7999d8u+building*264u;
    if(W(b+16u)!=0x73u||B(b+0xa6u)==0||B(b)!=1||W(b+0x40u)==0)U(a+0x130u)=0;
    counter=U(a+0x140u);cycle=U(a+0x13cu);due=(uint8_t)((int32_t)counter>=(int32_t)cycle);
    if(due){U(a+0x140u)=0;B(a-7u)=(uint8_t)(B(a-7u)+1u);}
    state=B(a+0x30u);
    switch(state) {
    case 4:U(a+0x13cu)=8;FUN_004b1d50((int32_t)id);id=U(0xd056f8u);break;
    case 5:U(a+0x13cu)=7;FUN_004b17d0((int32_t)id);id=U(0xd056f8u);break;
    case 8:(void)ostrich_initial_wait(a);return;
    case 9:case 10:
        U(a+0x13cu)=12;FUN_004a5da0((int32_t)id,speed);id=U(0xd056f8u);a=ostrich_actor(id);
        direction=B(a+9u);
        if(direction==9){FUN_004b3250((int32_t)id);id=U(0xd056f8u);a=ostrich_actor(id);B(a+9u)=B(a+10u);}
        else if(direction==10){
            B(a+9u)=B(a+10u);B(a+0x30u)=8;
            W(a+0x2eu)=(uint16_t)((uint32_t)B(a+0x5au)*5u);U(a+0x140u)=0;return;
        }
        else if(direction==8)ostrich_moving_wait(a);
        if(due){FUN_004eb000((int32_t)id);id=U(0xd056f8u);}break;
    case 12:
        if(OSTRICH_H(a+0x2eu)==0){FUN_004b5050((int32_t)id);id=U(0xd056f8u);}
        a=ostrich_actor(id);W(a+0x2eu)=(uint16_t)(W(a+0x2eu)+1u);
        if(OSTRICH_H(a+0x2eu)>500)B(a+0x30u)=4;break;
    case 14:case 15:U(a+0x13cu)=1;FUN_004eb000((int32_t)id);id=U(0xd056f8u);break;
    case 16:case 17:
        U(a+0x13cu)=12;counter=U(a+0x140u)+1u;U(a+0x140u)=counter;
        if((int32_t)counter>=12)U(a+0x140u)=0;
        FUN_004a5da0((int32_t)id,speed*2u);id=U(0xd056f8u);a=ostrich_actor(id);
        direction=B(a+9u);
        if(direction==9){FUN_004b3250((int32_t)id);id=U(0xd056f8u);a=ostrich_actor(id);B(a+9u)=B(a+10u);B(a+0x30u)=14;}
        else if(direction==10){B(a+6u)=2;return;}
        else if(direction==8){B(a+9u)=B(a+10u);B(a+0x30u)=14;U(a+0x140u)=0;return;}
        break;
    case 18:
    {
        int16_t remaining;
        U(a+0x13cu)=8;W(a+0x2eu)=(uint16_t)(W(a+0x2eu)-1u);
        remaining=OSTRICH_H(a+0x2eu);if(remaining<0){W(a+0x2eu)=0;remaining=0;}
        if(remaining<=0&&due) {
            uint32_t index=U(0xd3b1c4u),rng_a,rng_b,formation=0xbcb9c8u+(uint32_t)group*144u;
            int32_t x,y,tx,ty,position;uint16_t target_x,target_y;
            U(0xd3b1c8u+index*4u)=U(0xd3b374u);++index;
            U(0xd3b1c4u)=index;if((int32_t)index>=100)U(0xd3b1c4u)=0;
            rng_a=U(0xd3b384u);rng_b=U(0xd3b380u);ostrich_shift(&rng_a,&rng_b);
            U(0xd3b384u)=rng_a;U(0xd3b378u)=rng_a&7u;
            U(0xd3b380u)=rng_b;U(0xd3b364u)=rng_b&0x7fffu;
            U(0xd3b36cu)=rng_b&7u;U(0xd3b374u)=rng_a&0x7fu;
            ty=OSTRICH_H(a+14u);tx=OSTRICH_H(a+12u);
            U(0xd3b370u)=rng_a&0x7fffu;U(0xd3b368u)=rng_b&0x7fu;
            y=OSTRICH_H(formation+0x3cu);x=OSTRICH_H(formation+0x3au);
            position=FUN_004eab10(x,y,(int32_t)(rng_a&15u),tx,ty,1);
            if(position>=0&&position<16){
                id=U(0xd056f8u);a=ostrich_actor(id);
                target_x=W(0x5cdb78u+(uint32_t)position*8u);target_y=W(0x5cdb7cu+(uint32_t)position*8u);
                target_x=(uint16_t)(target_x+W(formation+0x3au));target_y=(uint16_t)(target_y+W(formation+0x3cu));
                W(a+0x1cu)=target_x;W(a+0x1eu)=target_y;B(a+0x30u)=10;return;
            }
            id=U(0xd056f8u);a=ostrich_actor(id);W(a+0x2eu)=100;
        }
        if(due){FUN_004eb000((int32_t)id);id=U(0xd056f8u);}break;
    }
    case 19:
        U(a+0x13cu)=8;W(a+0x2eu)=(uint16_t)(W(a+0x2eu)-1u);
        if(OSTRICH_H(a+0x2eu)<=0){if(due){B(a+0x30u)=18;return;}}
        else if(due){FUN_004eb000((int32_t)id);id=U(0xd056f8u);}break;
    case 24:
        W(a+0x2eu)=(uint16_t)(W(a+0x2eu)-1u);
        if(OSTRICH_H(a+0x2eu)<=0){U(a+0x140u)=0;U(a+0x13cu)=8;B(a+0x30u)=8;}
        U(a-4u)=0;return;
    default:B(a+0x30u)=8;break;
    }
    (void)ostrich_image(id);
}

#undef OSTRICH_H

extern void FUN_004b17d0(int32_t),FUN_004b1d50(int32_t),FUN_004b3250(int32_t);
extern void FUN_004a5da0(int32_t,uint32_t);
extern int32_t FUN_004450c0(int32_t,uint32_t,uint32_t,int32_t,uint32_t);
extern int32_t FUN_0045cf50(int32_t,int32_t,int32_t,int32_t);
extern int32_t FUN_0045d010(int32_t,int32_t,uint32_t);
extern void FUN_00444bd0(int32_t,int32_t);
static uint32_t native_trader_offset(uint32_t actor){return actor*388u;}
static void native_trader_take_destination(uint32_t actor,uint32_t building)
{
    uint32_t o=native_trader_offset(actor);uint16_t x;
    W(0xbcd656u+o)=(uint16_t)building;x=W(0xd3b4e4u);
    B(0xbcd630u+o)=8;W(0xbcd62eu+o)=0;W(0xbcd61cu+o)=x;
    W(0xbcd61eu+o)=W(0xd3b4e0u);B(0xbcd5f9u+o)=0;
}
static uint32_t native_trader_render(uint32_t actor)
{
    uint32_t o=native_trader_offset(actor),direction,rotation,base,overlay,variant;
    int32_t facing=(int8_t)B(0xbcd609u+o);
    if(facing>=8)facing=(int8_t)B(0xbcd60au+o);
    rotation=U(0xbca2e0u);direction=(uint32_t)facing-rotation;U(0xe94e00u)=direction;
    if((int32_t)direction<0){direction+=8u;U(0xe94e00u)=direction;}
    if(B(0xbcd630u+o)==4u){
        int32_t half=(int16_t)W(0xbcd62eu+o)/2;
        uint32_t frame=B(0x5d048cu+(uint32_t)half);base=U(0xf55e4cu);
        U(0xbcd5fcu+o)=base+frame;U(0xbcd778u+o)=0;
    }else{
        uint32_t animation;base=U(0xf55e44u);animation=B(0xbcd5f9u+o);
        U(0xbcd5fcu+o)=base+animation*8u+direction;
    }
    base=U(0xf55d5cu);variant=B(0xbcd603u+o);overlay=base+variant*8u+8u;
    U(0xbcd778u+o)=overlay;
    if(overlay!=0u){
        uint8_t x=B(0x5dc23cu+direction*2u),y;
        overlay+=direction;U(0xbcd778u+o)=overlay;
        y=B(0x5dc23du+direction*2u);B(0xbcd664u+o)=x;B(0xbcd665u+o)=y;
        return (overlay&0xffffff00u)|(uint32_t)y;
    }
    return 0;
}
__declspec(noinline) uint32_t restore_actor_update_native_trader(void)
{
    uint32_t actor=U(0xd056f8u),o=native_trader_offset(actor),kind=B(0xbcd602u+o),speed;
    uint8_t animation,state,direction;
    B(0xbcd65du+o)=0;B(0xbcd66au+o)=0;animation=B(0xbcd5f9u+o);++animation;
    speed=U(0x7090ecu+kind*56u);B(0xbcd5f9u+o)=animation;if(animation>=12u)B(0xbcd5f9u+o)=0;
    U(0xbcd778u+o)=0;state=B(0xbcd630u+o);
    switch(state){
    case 5:FUN_004b17d0((int32_t)actor);actor=U(0xd056f8u);break;
    case 4:FUN_004b1d50((int32_t)actor);actor=U(0xd056f8u);break;
    case 10:
        B(0xbcd65du+o)=1;W(0xbcd62eu+o)=(uint16_t)(W(0xbcd62eu+o)+1u);
        if((int16_t)W(0xbcd62eu+o)>10){
            int32_t y,x,result;W(0xbcd62eu+o)=0;y=(int16_t)W(0xbcd60eu+o);x=(int16_t)W(0xbcd60cu+o);
            result=FUN_004450c0((int32_t)actor,(uint32_t)x,(uint32_t)y,0,0xffffffffu);
            actor=U(0xd056f8u);o=native_trader_offset(actor);
            if(result!=0){native_trader_take_destination(actor,(uint32_t)result);break;}
            B(0xbcd606u+o)=2;
        }
        B(0xbcd5f9u+o)=0;break;
    case 8:{
        int32_t linked;
        FUN_004a5da0((int32_t)actor,speed);actor=U(0xd056f8u);o=native_trader_offset(actor);direction=B(0xbcd609u+o);
        if(direction==8u)B(0xbcd630u+o)=11;
        else if(direction==9u){FUN_004b3250((int32_t)actor);actor=U(0xd056f8u);}
        else if(direction==10u){B(0xbcd606u+o)=2;B(0xbcd65du+o)=1;}
        o=native_trader_offset(actor);linked=(int16_t)W(0xbcd656u+o);
        if(B(0x7999d8u+(uint32_t)linked*264u)!=1u)B(0xbcd606u+o)=2;
        break;}
    case 11:
        W(0xbcd62eu+o)=(uint16_t)(W(0xbcd62eu+o)+1u);
        if((int16_t)W(0xbcd62eu+o)>10){
            int32_t linked,player,success;
            W(0xbcd62eu+o)=0;linked=(int16_t)W(0xbcd656u+o);player=B(0xbcd607u+o);
            success=FUN_0045cf50((int32_t)actor,player,linked,0);actor=U(0xd056f8u);o=native_trader_offset(actor);
            if(success!=0){
                int32_t amount;uint8_t count;
                linked=(int16_t)W(0xbcd656u+o);player=B(0xbcd607u+o);
                amount=FUN_0045d010(player,linked,0);actor=U(0xd056f8u);
                FUN_00444bd0((int32_t)actor,amount);actor=U(0xd056f8u);o=native_trader_offset(actor);
                count=B(0xbcd667u+o);count=(uint8_t)(count+3u);B(0xbcd667u+o)=count;
                B(0xbcd5f9u+o)=0;break;
            }else{
                int32_t y=(int16_t)W(0xbcd60eu+o),x=(int16_t)W(0xbcd60cu+o),result;
                result=FUN_004450c0((int32_t)actor,(uint32_t)x,(uint32_t)y,0,0xffffffffu);
                actor=U(0xd056f8u);o=native_trader_offset(actor);
                if(result!=0){native_trader_take_destination(actor,(uint32_t)result);break;}
                B(0xbcd630u+o)=9;W(0xbcd62eu+o)=0;W(0xbcd61cu+o)=W(0xbcd624u+o);
                W(0xbcd61eu+o)=W(0xbcd626u+o);
            }
        }
        B(0xbcd5f9u+o)=0;break;
    case 9:
        FUN_004a5da0((int32_t)actor,speed);actor=U(0xd056f8u);o=native_trader_offset(actor);direction=B(0xbcd609u+o);
        if(direction==8u || direction==10u)B(0xbcd606u+o)=2;
        else if(direction==9u){FUN_004b3250((int32_t)actor);actor=U(0xd056f8u);}
        break;
    default:B(0xbcd630u+o)=10;break;
    }
    return native_trader_render(actor);
}

extern int32_t FUN_004b3ff0(int32_t actor,int32_t mode);
extern int32_t FUN_004b5770(int32_t actor);
extern uint32_t FUN_004b1d10(int32_t direction,int32_t other);
extern void FUN_004b5050(int32_t actor),FUN_004b7ab0(int32_t group);
extern uint32_t FUN_0047d350(void);
static uint32_t arrow_offset(uint32_t actor){return actor*388u;}

/* Ordinary native721-byte callback. All additions/products are wrapped DWORD
 * arithmetic; only the two explicit small signed quotients use C division. */
__declspec(noinline) uint32_t restore_actor_update_arrow(void)
{
    uint32_t actor=U(0xd056f8u),o=arrow_offset(actor),target;int32_t advanced;
    uint8_t age;
    B(0xbcd604u+o)=1;age=(uint8_t)(B(0xbcd631u+o)+1u);B(0xbcd631u+o)=age;
    if((int8_t)age>120)B(0xbcd606u+o)=2;
    advanced=FUN_004b3ff0((int32_t)actor,4);actor=U(0xd056f8u);
    target=(uint32_t)FUN_004b5770((int32_t)actor);
    if(target){
        uint32_t t=arrow_offset(target),context=U(0xeaac28u),kind,g,limit,resistance,strength,damage,count;
        uint8_t target_context=B(0xbcd607u+t);int32_t group=(int16_t)W(0xbcd658u+t),bonus=0;
        uint16_t formation_enabled;
        if((uint32_t)target_context==context){
            int32_t value=(int32_t)(int16_t)W(0xbcba42u+(uint32_t)group*144u)+10;
            bonus=value/20;
        }
        kind=B(0xbcd602u+t);limit=U(0x7090d0u+kind*56u);resistance=U(0x7090dcu+kind*56u)+(uint32_t)bonus;
        if((int32_t)resistance>=20)resistance=20;
        actor=U(0xd056f8u);o=arrow_offset(actor);g=(uint32_t)group*144u;kind=B(0xbcd602u+o);
        formation_enabled=W(0xbcba22u+g);strength=U(0x7090e0u+kind*56u);
        if(formation_enabled){
            int32_t formation=(int16_t)W(0xbcb9ccu+g);
            if(formation>=11&&formation<=13){
                int32_t direction=(int8_t)B(0xbcd60bu+t);uint8_t facing=B(0xbcba33u+g);
                uint32_t aligned=FUN_004b1d10(direction,(int32_t)facing);
                if(!aligned)resistance-=4u;
                else if(formation==13){if(W(0xbcb9f2u+g)==6u)resistance-=2u;}
                else{
                    int32_t subtype=(int16_t)W(0xbcb9f2u+g);
                    if(subtype==3||subtype==4)resistance+=4u;else if(subtype==6)resistance-=2u;
                }
            }
        }
        if((int32_t)resistance<0)resistance=0;if((int32_t)resistance>20)resistance=20;
        damage=0;
        if(strength){damage=((30u-resistance)/20u)*strength;if((int32_t)damage<0)damage=0;}
        count=(uint32_t)B(0xbcd615u+t)+damage;
        if((int32_t)count>(int32_t)limit){
            B(0xbcd615u+t)=(uint8_t)(limit+1u);B(0xbcd630u+t)=4;W(0xbcd62eu+t)=0;
            FUN_004b5050((int32_t)target);FUN_004b7ab0(group);
        }else B(0xbcd615u+t)=(uint8_t)count;
        actor=U(0xd056f8u);o=arrow_offset(actor);B(0xbcd606u+o)=2;
        if(group){
            int32_t linked=(int16_t)W(0xbcd652u+o);
            W(0xbcba26u+g)=6;W(0xbcba28u+g)=W(0xbcd658u+arrow_offset((uint32_t)linked));
        }
        (void)FUN_0047d350();actor=U(0xd056f8u);
    }else{
        actor=U(0xd056f8u);if(advanced)B(0xbcd606u+arrow_offset(actor))=2;
    }
    {
        uint32_t rotation,direction,base;
        o=arrow_offset(actor);rotation=U(0xbca2e0u)*2u;
        direction=(uint32_t)(int32_t)(int8_t)B(0xbcd609u+o)-rotation;U(0xe94e00u)=direction;
        if((int32_t)direction<0){direction+=16u;U(0xe94e00u)=direction;}
        base=U(0xf56424u);U(0xbcd5fcu+o)=base+direction+16u;
    }
    return o;
}

extern int32_t FUN_004b3ff0(int32_t actor,int32_t mode);
extern int32_t FUN_004b56d0(int32_t actor);
extern void FUN_004b5050(int32_t actor);
extern void FUN_004b7ab0(int32_t group);
extern int32_t FUN_004ea9c0(int32_t kind);
extern uint32_t FUN_004ea9a0(int32_t kind);
extern void restore_actor_combat_sound(int32_t event);
#define RESTORE_ACTOR_UPDATE_JAVELIN_H(a) ((int32_t)(int16_t)W(a))
static uint32_t javelin_actor(uint32_t id){return 0xbcd600u+id*388u;}
static uint32_t javelin_group(int32_t id){return 0xbcb9c8u+(uint32_t)id*144u;}

__declspec(noinline) uint32_t restore_actor_update_javelin(void)
{
    uint32_t id=U(0xd056f8u),actor=javelin_actor(id),owner,target,owner_group,hit_group,kind,attack_kind;
    uint32_t limit,armour,attack,damage,heading,base;
    int32_t collision,hit;uint8_t age,owner_kind;int special;
    B(actor+4u)=1;
    age=(uint8_t)(B(actor+0x31u)+1u);B(actor+0x31u)=age;
    if((int8_t)age>120)B(actor+6u)=2;
    collision=FUN_004b3ff0((int32_t)id,4);
    id=U(0xd056f8u);hit=FUN_004b56d0((int32_t)id);
    id=U(0xd056f8u);actor=javelin_actor(id);
    owner=javelin_actor((uint32_t)(int32_t)RESTORE_ACTOR_UPDATE_JAVELIN_H(actor+0x52u));
    owner_group=(uint32_t)(int32_t)RESTORE_ACTOR_UPDATE_JAVELIN_H(owner+0x58u);
    if(hit!=0) {
        target=javelin_actor((uint32_t)hit);
        hit_group=(uint32_t)(int32_t)RESTORE_ACTOR_UPDATE_JAVELIN_H(target+0x58u);
        owner_kind=B(owner+2u);
        if(owner_kind==62u)attack_kind=62;
        else if(owner_kind==78u) {
            if(B(owner+6u)==2)goto consume;
            attack_kind=78;
        } else attack_kind=(uint32_t)(int32_t)RESTORE_ACTOR_UPDATE_JAVELIN_H(javelin_group((int32_t)owner_group)+4u);
        kind=B(target+2u);
        special=kind==93u||kind==92u||kind==100u||kind==101u;
        limit=special?U(target+0x124u):U(0x7090d0u+kind*56u);
        armour=U(0x7090dcu+kind*56u);if((int32_t)armour>=20)armour=20;
        attack=U(0x7090e0u+attack_kind*56u);
        damage=0;
        if((int32_t)attack>0) {
            uint32_t product=(20u-armour)*attack;
            int32_t quotient=(int32_t)product/20;
            if(quotient>=0)damage=(uint32_t)quotient;
        }
        if(!special) {
            uint32_t accumulated=(uint32_t)B(target+0x15u)+damage;
            if((int32_t)accumulated>(int32_t)limit) {
                B(target+0x15u)=(uint8_t)(limit+1u);
                B(target+0x30u)=4;W(target+0x2eu)=0;
                FUN_004b5050(hit);
                FUN_004b7ab0((int32_t)hit_group);
                if(((uint32_t)FUN_004ea9c0((int32_t)B(target+2u))&255u)==0) {
                    if((FUN_004ea9a0((int32_t)B(target+2u))&255u)==0) {
                        if(B(target+2u)!=23u) {
                            uint32_t profile=U(0xeaac28u);
                            uint32_t owner_profile=B(owner+7u);
                            if(owner_profile==profile) {
                                uint32_t address=javelin_group((int32_t)owner_group)+0x7au;
                                int32_t value=(int32_t)RESTORE_ACTOR_UPDATE_JAVELIN_H(address)+1;
                                if(value>=100)value=100;
                                W(address)=(uint16_t)value;
                            }
                        }
                    }
                }
            } else B(target+0x15u)=(uint8_t)accumulated;
        } else {
            uint32_t remaining=U(target+0x120u)-damage;
            if((int32_t)remaining>0)U(target+0x120u)=remaining;
            else {
                U(target+0x120u)=0;B(target+0x30u)=4;W(target+0x2eu)=0;
                FUN_004b5050(hit);FUN_004b7ab0((int32_t)hit_group);
            }
        }
        if(hit_group!=0) {
            uint32_t group=javelin_group((int32_t)hit_group);
            W(group+0x5eu)=6;W(group+0x60u)=(uint16_t)owner_group;
        }
        restore_actor_combat_sound(24);
        id=U(0xd056f8u);actor=javelin_actor(id);
consume:
        B(actor+6u)=2;
    } else if(collision!=0)B(actor+6u)=2;
    heading=(uint32_t)(int32_t)(int8_t)B(actor+9u);
    heading-=U(0xbca2e0u)*4u;U(0xe94e00u)=heading;
    if((int32_t)heading<0){heading+=32u;U(0xe94e00u)=heading;}
    owner_kind=B(owner+2u);
    if(owner_kind==42u||owner_kind==62u) {
        base=U(0xf56f1cu);U(actor-4u)=base+heading;
        base=U(0xf56f14u);U(actor+0x178u)=base+heading;
    } else {
        base=U(0xf55cecu);U(actor-4u)=base+heading;
        base=U(0xf55cf4u);U(actor+0x178u)=base+heading;
    }
    return id*388u;
}

#undef RESTORE_ACTOR_UPDATE_JAVELIN_H

extern int32_t FUN_004b3ff0(int32_t,int32_t),FUN_004b56d0(int32_t);
extern uint32_t FUN_004b1d10(int32_t,int32_t);
extern void FUN_004b5050(int32_t),FUN_004b7ab0(int32_t);
extern uint32_t FUN_0047d350(void);
static uint32_t bolt_offset(uint32_t actor){return actor*388u;}
static uint32_t bolt_apply_hit(uint32_t target)
{
    uint32_t o=bolt_offset(target),profile=U(0xeaac28u),owner,kind,guard,limit,model,attacker,power,damage,sum,g;
    int32_t group=(int16_t)W(0xbcd658u+o),bonus=0;uint16_t formation_active;
    owner=B(0xbcd607u+o);g=(uint32_t)group*144u;
    if(owner==profile)bonus=((int32_t)(int16_t)W(0xbcba42u+g)+10)/20;
    kind=B(0xbcd602u+o);model=kind*56u;guard=U(0x7090dcu+model);limit=U(0x7090d0u+model);
    guard+=(uint32_t)bonus;if((int32_t)guard>=20)guard=20;
    attacker=U(0xd056f8u);kind=B(0xbcd602u+bolt_offset(attacker));
    formation_active=W(0xbcba22u+g);power=U(0x7090e0u+kind*56u);
    if(formation_active!=0u){
        int32_t formation=(int16_t)W(0xbcb9ccu+g);
        if(formation==13){
            int32_t direction=(int8_t)B(0xbcd60bu+o),other=B(0xbcba33u+g);
            if(FUN_004b1d10(direction,other)==0u)guard-=4u;
            else if(W(0xbcb9f2u+g)==6u)guard-=2u;
        }else if(formation==11 || formation==12){
            int32_t direction=(int8_t)B(0xbcd60bu+o),other=B(0xbcba33u+g);
            if(FUN_004b1d10(direction,other)==0u)guard-=4u;
            else{
                int32_t state=(int16_t)W(0xbcb9f2u+g);
                if(state==3 || state==4)guard+=4u;
                else if(state==6)guard-=2u;
            }
        }
    }
    if((int32_t)guard<0)guard=0;if((int32_t)guard>20)guard=20;
    damage=0;
    if(power!=0u){damage=(uint32_t)((30-(int32_t)guard)/20)*power;if((int32_t)damage<0)damage=0;}
    sum=(uint32_t)B(0xbcd615u+o)+damage;
    if((int32_t)sum>(int32_t)limit){
        B(0xbcd615u+o)=(uint8_t)(limit+1u);B(0xbcd630u+o)=4;W(0xbcd62eu+o)=0;
        FUN_004b5050((int32_t)target);FUN_004b7ab0(group);
    }else B(0xbcd615u+o)=(uint8_t)sum;
    /* Native pushes unused15 here (14 on the miss/movement branch). The
       authenticated47D350 is XOR EAX,EAX;RET and reads no argument slots. */
    (void)FUN_0047d350();attacker=U(0xd056f8u);B(0xbcd606u+bolt_offset(attacker))=2;
    return attacker;
}
__declspec(noinline) uint32_t restore_actor_update_bolt(void)
{
    uint32_t actor=U(0xd056f8u),o=bolt_offset(actor),rotation,direction,base;int32_t movement,hit;uint8_t age;
    B(0xbcd604u+o)=1;age=B(0xbcd631u+o);++age;B(0xbcd631u+o)=age;
    if((int8_t)age>120)B(0xbcd606u+o)=2;
    movement=FUN_004b3ff0((int32_t)actor,4);actor=U(0xd056f8u);hit=FUN_004b56d0((int32_t)actor);
    if(hit!=0)actor=bolt_apply_hit((uint32_t)hit);
    else{
        if(movement!=0){actor=U(0xd056f8u);B(0xbcd606u+bolt_offset(actor))=2;(void)FUN_0047d350();}
        actor=U(0xd056f8u);
    }
    o=bolt_offset(actor);rotation=U(0xbca2e0u)*2u;
    direction=(uint32_t)(int32_t)(int8_t)B(0xbcd609u+o)-rotation;U(0xe94e00u)=direction;
    if((int32_t)direction<0){direction+=16u;U(0xe94e00u)=direction;}
    base=U(0xf56424u);U(0xbcd5fcu+o)=base+direction+32u;
    return o;
}

extern void FUN_004b5420(int32_t actor);
extern void FUN_004b52f0(int32_t actor);
extern int32_t FUN_00499b50(int32_t actor,int32_t range);
extern uint8_t FUN_0051cd80(int32_t x,int32_t y,int32_t target_x,int32_t target_y);
extern int32_t FUN_004b45c0(int32_t actor,int32_t x,int32_t y,int32_t target_x,int32_t target_y,int32_t kind);
extern void restore_actor_combat_sound(int32_t event);
#define RESTORE_ACTOR_UPDATE_BALLISTA_H(a) ((int32_t)(int16_t)W(a))
static uint32_t ballista_actor(uint32_t id){return 0xbcd600u+id*388u;}

__declspec(noinline) uint32_t restore_actor_update_ballista(void)
{
    uint32_t id=U(0xd056f8u),actor=ballista_actor(id),building,rotation,grid,kind,heading;
    uint8_t state,ready=0,timer;
    B(actor+0x6au)=4;B(actor+4u)=0;B(actor+0x5du)=1;B(actor+0x6fu)=10;B(actor+0x70u)=45;
    building=0x7999d8u+(uint32_t)(int32_t)RESTORE_ACTOR_UPDATE_BALLISTA_H(actor+0x52u)*264u;
    if(B(building)!=1)B(actor+6u)=2;
    else if((uint32_t)(int32_t)RESTORE_ACTOR_UPDATE_BALLISTA_H(building+0x30u)!=id)B(actor+6u)=2;
    if(RESTORE_ACTOR_UPDATE_BALLISTA_H(building+0x40u)<=0)B(actor+6u)=2;
    if(RESTORE_ACTOR_UPDATE_BALLISTA_H(building+0x2au)<=0)B(actor+6u)=2;
    FUN_004b5420((int32_t)id);
    rotation=U(0xbca2e0u);id=U(0xd056f8u);actor=ballista_actor(id);
    switch(rotation) {
    case 0:W(actor+12u)=W(building+6u);W(actor+14u)=W(building+8u);break;
    case 2:W(actor+12u)=(uint16_t)(W(building+6u)+1u);W(actor+14u)=W(building+8u);break;
    case 4:W(actor+12u)=(uint16_t)(W(building+6u)+1u);W(actor+14u)=(uint16_t)(W(building+8u)+1u);break;
    case 6:W(actor+12u)=W(building+6u);W(actor+14u)=(uint16_t)(W(building+8u)+1u);break;
    default:break;
    }
    grid=U(0xbca2d8u);
    {int32_t y=RESTORE_ACTOR_UPDATE_BALLISTA_H(actor+14u),x=RESTORE_ACTOR_UPDATE_BALLISTA_H(actor+12u);U(actor+0x18u)=grid+(uint32_t)x+(uint32_t)y*228u;}
    FUN_004b52f0((int32_t)id);id=U(0xd056f8u);actor=ballista_actor(id);
    state=B(actor+0x30u);
    if(state==4)B(actor+6u)=2;
    else if(state==8) {
        W(actor+0x2eu)=(uint16_t)(W(actor+0x2eu)+1u);
        if(RESTORE_ACTOR_UPDATE_BALLISTA_H(actor+0x2eu)>20) {
            int32_t range;
            W(actor+0x2eu)=0;kind=B(actor+2u);range=(int32_t)U(0x7090e4u+kind*56u);
            if(FUN_00499b50((int32_t)id,range)!=0) {
                id=U(0xd056f8u);actor=ballista_actor(id);B(actor+0x30u)=9;
                kind=B(actor+2u);timer=B(0x7090e8u+kind*56u);B(actor+0x63u)=timer;
            } else {id=U(0xd056f8u);actor=ballista_actor(id);}
        }
    } else if(state==9) {
        int32_t threshold;
        timer=(uint8_t)(B(actor+0x63u)+1u);B(actor+0x63u)=timer;
        kind=B(actor+2u);threshold=(int32_t)U(0x7090e8u+kind*56u);
        if((int32_t)timer>threshold) {
            int32_t range=(int32_t)U(0x7090e4u+kind*56u);
            ready=1;
            {int32_t found=FUN_00499b50((int32_t)id,range);
             id=U(0xd056f8u);actor=ballista_actor(id);
             if(found==0)B(actor+0x30u)=8;
             else {
                uint32_t cached_actor=actor,target_x=U(0xd3b4e4u),target_y=U(0xd3b4e0u);
                int32_t x,y;uint8_t direction;int16_t delay;
                W(cached_actor+0x2eu)=(uint16_t)(W(cached_actor+0x2eu)+1u);
                y=RESTORE_ACTOR_UPDATE_BALLISTA_H(cached_actor+14u);x=RESTORE_ACTOR_UPDATE_BALLISTA_H(cached_actor+12u);
                direction=FUN_0051cd80(x,y,(int32_t)target_x,(int32_t)target_y);
                B(cached_actor+9u)=direction;
                delay=RESTORE_ACTOR_UPDATE_BALLISTA_H(cached_actor+0x2eu);
                if(delay==10) {
                    target_y=U(0xd3b4e0u);target_x=U(0xd3b4e4u);
                    y=RESTORE_ACTOR_UPDATE_BALLISTA_H(cached_actor+14u);x=RESTORE_ACTOR_UPDATE_BALLISTA_H(cached_actor+12u);id=U(0xd056f8u);
                    (void)FUN_004b45c0((int32_t)id,x,y,(int32_t)target_x,(int32_t)target_y,60);
                    restore_actor_combat_sound(13);
                } else if(delay>=24) {W(cached_actor+0x2eu)=0;B(cached_actor+0x63u)=0;}
                id=U(0xd056f8u);actor=ballista_actor(id);
             }}
        }
    } else B(actor+0x30u)=8;
    rotation=U(0xbca2e0u);
    heading=(uint32_t)(int32_t)(int8_t)B(actor+9u)-rotation;U(0xe94e00u)=heading;
    if((int32_t)heading<0){heading+=8u;U(0xe94e00u)=heading;}
    if(B(actor+0x30u)==9) {
        if(ready) {
            int32_t delay=(int32_t)RESTORE_ACTOR_UPDATE_BALLISTA_H(actor+0x2eu);
            uint32_t base=U(0xf56e54u);
            uint32_t image=base+(uint32_t)(delay/2)*8u+heading;
            U(actor-4u)=image;return image;
        }
        U(actor-4u)=U(0xf56e44u)+heading;
        return rotation&0xffffff00u;
    }
    U(actor-4u)=U(0xf56e44u)+heading;return rotation;
}

#undef RESTORE_ACTOR_UPDATE_BALLISTA_H

extern void FUN_004b5420(int32_t actor);
extern int32_t FUN_00480e90(int32_t actor);
extern void FUN_004b52f0(int32_t actor);
#define RESTORE_ACTOR_UPDATE_SHIPWRECK_H(a) ((int32_t)(int16_t)W(a))
static uint32_t shipwreck_actor(uint32_t id){return 0xbcd600u+id*388u;}

__declspec(noinline) uint32_t restore_actor_update_shipwreck(void)
{
    uint32_t id=U(0xd056f8u),actor=shipwreck_actor(id),base;uint8_t animation;
    B(actor+0x5du)=0;B(actor+0x6fu)=0;B(actor+0x6eu)=1;
    animation=(uint8_t)(B(actor-7u)+1u);B(actor-7u)=animation;
    if(animation>=36)B(actor+6u)=2;
    if(RESTORE_ACTOR_UPDATE_SHIPWRECK_H(actor+0x2eu)<1000) {
        int32_t found;
        FUN_004b5420((int32_t)id);id=U(0xd056f8u);
        found=FUN_00480e90((int32_t)id);id=U(0xd056f8u);
        if(found!=0) {
            uint16_t target_x=W(0xd3b4e4u),target_y;uint32_t grid=U(0xbca2d8u);
            int32_t x,y;
            actor=shipwreck_actor(id);
            W(actor+12u)=target_x;
            target_y=W(0xd3b4e0u);W(actor+14u)=target_y;
            y=(int32_t)(int16_t)target_y;x=RESTORE_ACTOR_UPDATE_SHIPWRECK_H(actor+12u);
            U(actor+0x18u)=grid+(uint32_t)x+(uint32_t)y*228u;
            W(actor+0x42u)=(uint16_t)((uint32_t)W(actor+12u)*15u);
            W(actor+0x44u)=(uint16_t)((uint32_t)W(actor+14u)*15u);
            W(actor+0x42u)=(uint16_t)(W(actor+0x42u)+7u);
            W(actor+0x44u)=(uint16_t)(W(actor+0x44u)+7u);
        }
        FUN_004b52f0((int32_t)id);id=U(0xd056f8u);actor=shipwreck_actor(id);
        W(actor+0x2eu)=1000;
    }
    base=U(0xf56dbcu);animation=B(actor-7u);U(actor-4u)=base+animation;
    return id*388u;
}

#undef RESTORE_ACTOR_UPDATE_SHIPWRECK_H

extern void FUN_00493610(void);
__declspec(noinline) uint32_t restore_actor_update_creature(void)
{
    uint32_t actor=U(0xd056f8u),sprite=U(0xf55da4u);
    U(0xbcd5fcu+animal_offset(actor))=sprite;
    return actor;
}

extern void FUN_00493610(void);
__declspec(noinline) uint32_t restore_actor_update_missionary(void)
{
    uint32_t actor=U(0xd056f8u),o=animal_offset(actor),building,rotation,direction;
    int32_t linked;uint8_t frame;
    B(0xbcd66au+o)=1;B(0xbcd604u+o)=0;W(0xbcd63au+o)=192;
    linked=(int16_t)W(0xbcd652u+o);building=(uint32_t)linked*264u;
    if(B(0x7999d8u+building)!=1u||(uint32_t)(int32_t)(int16_t)W(0x799a02u+building)!=actor)B(0xbcd606u+o)=2;
    frame=(uint8_t)(B(0xbcd5f9u+o)+1u);B(0xbcd5f9u+o)=frame;if(frame>=12u)B(0xbcd5f9u+o)=0;
    FUN_00493610();actor=U(0xd056f8u);rotation=U(0xbca2e0u);o=animal_offset(actor);
    direction=(uint32_t)(int32_t)(int8_t)B(0xbcd609u+o)-rotation;U(0xe94e00u)=direction;
    if((int32_t)direction<0){direction+=8u;U(0xe94e00u)=direction;}
    if(B(0xbcd630u+o)==4u){
        int32_t timer=(int16_t)W(0xbcd62eu+o),index=timer/2;uint32_t base;
        frame=B(0x5d048cu+(uint32_t)index);base=U(0xf56364u);U(0xbcd5fcu+o)=base+(uint32_t)frame;
        return base;
    }else{
        uint32_t base=U(0xf5635cu);frame=B(0xbcd5f9u+o);U(0xbcd5fcu+o)=base+(uint32_t)frame*8u+direction;
        return direction;
    }
}

extern void FUN_004b17d0(int32_t),FUN_004b1d50(int32_t),FUN_004b3250(int32_t),FUN_0048b4a0(int32_t);
extern void FUN_004a5da0(int32_t,uint32_t);
extern int32_t FUN_00495840(int32_t),FUN_00495950(int32_t);
extern int32_t FUN_00445980(int32_t,int32_t,int32_t,int32_t),FUN_00445e50(int32_t,int32_t,int32_t,int32_t);
extern uint32_t FUN_004a35e0(int32_t);
static uint32_t docker_offset(uint32_t actor){return actor*388u;}
static void docker_set_home_target(uint32_t actor)
{
    uint32_t o=docker_offset(actor);
    B(0xbcd630u+o)=14;W(0xbcd62eu+o)=0;
    W(0xbcd61cu+o)=W(0xbcd624u+o);W(0xbcd61eu+o)=W(0xbcd626u+o);
}
static uint32_t docker_move(uint32_t actor,uint32_t speed)
{
    FUN_0048b4a0((int32_t)actor);actor=U(0xd056f8u);
    FUN_004a5da0((int32_t)actor,speed);return U(0xd056f8u);
}
static uint32_t docker_render(uint32_t actor)
{
    uint32_t o=docker_offset(actor),direction,rotation,base,overlay,offset_direction;
    int32_t facing=(int8_t)B(0xbcd609u+o);uint8_t state,cargo,x,y;
    if(facing>=8)facing=(int8_t)B(0xbcd60au+o);
    rotation=U(0xbca2e0u);direction=(uint32_t)facing-rotation;U(0xe94e00u)=direction;
    if((int32_t)direction<0){direction+=8u;U(0xe94e00u)=direction;}
    state=B(0xbcd630u+o);
    if(state==4u){
        int32_t half=(int16_t)W(0xbcd62eu+o)/2;uint32_t frame=B(0x5d048cu+(uint32_t)half);
        base=U(0xf560e4u);U(0xbcd5fcu+o)=base+frame;U(0xbcd778u+o)=0;
    }else if(state!=6u){
        uint32_t animation;base=U(0xf560dcu);animation=B(0xbcd5f9u+o);
        U(0xbcd5fcu+o)=base+animation*8u+direction;
    }
    overlay=U(0xbcd778u+o);
    if(overlay==0u){U(0xbcd5fcu+o)=0;return 0;}
    overlay+=direction;U(0xbcd778u+o)=overlay;cargo=B(0xbcd603u+o);offset_direction=direction;
    switch(cargo){
    case 12:case 24:case 25:case 26:case 30:case 35:
        offset_direction+=4u;if((int32_t)offset_direction>=8)offset_direction-=8u;break;
    default:break;
    }
    x=B(0x5dc23cu+offset_direction*2u);y=B(0x5dc23du+offset_direction*2u);
    B(0xbcd664u+o)=x;B(0xbcd665u+o)=y;
    return (overlay&0xffffff00u)|(uint32_t)x;
}
__declspec(noinline) uint32_t restore_actor_update_docker(void)
{
    uint32_t actor=U(0xd056f8u),o=docker_offset(actor),kind=B(0xbcd602u+o),speed,home;
    int32_t home_id,boat;uint8_t animation,cooldown,state,direction;uint16_t type;
    animation=B(0xbcd5f9u+o);++animation;B(0xbcd5f9u+o)=animation;
    speed=U(0x7090ecu+kind*56u);if(animation>=12u)B(0xbcd5f9u+o)=0;
    U(0xbcd778u+o)=0;B(0xbcd65du+o)=0;home_id=(int16_t)W(0xbcd652u+o);home=(uint32_t)home_id*264u;
    if(B(0x7999d8u+home)!=1u)B(0xbcd606u+o)=2;
    type=W(0x7999e8u+home);if(type!=75u && type!=76u && type!=192u)B(0xbcd606u+o)=2;
    cooldown=B(0x799a7fu+home);if(cooldown!=0u)B(0x799a7fu+home)=(uint8_t)(cooldown-1u);
    boat=(int16_t)W(0x799a8cu+home);
    if(boat!=0){
        uint32_t linked=docker_offset((uint32_t)boat);
        if(B(0xbcd606u+linked)!=1u || B(0xbcd602u+linked)!=20u || B(0xbcd630u+linked)==13u)W(0x799a8cu+home)=0;
    }
    B(0xbcd66au+o)=1;state=B(0xbcd630u+o);
    switch(state){
    case 5:FUN_004b17d0((int32_t)actor);actor=U(0xd056f8u);break;
    case 4:FUN_004b1d50((int32_t)actor);actor=U(0xd056f8u);break;
    case 8:
        B(0xbcd603u+o)=0;U(0xbcd778u+o)=0;
        if(FUN_00495840(home_id)==0)(void)FUN_00495950(home_id);
        actor=U(0xd056f8u);o=docker_offset(actor);B(0xbcd5f9u+o)=0;
        {uint32_t tile=U(0xbcd618u+o);if((B(0xaf598cu+tile*4u)&64u)==0u)B(0xbcd606u+o)=2;}
        break;
    case 9:{
        int16_t queued=(int16_t)W(0x799a2cu+home);uint32_t i;uint8_t found=0;
        U(0xbcd778u+o)=0;B(0xbcd5f9u+o)=0;
        if(queued<=0){W(0x799a2cu+home)=(uint16_t)actor;W(0xbcd62eu+o)=0;}
        queued=(int16_t)W(0x799a2cu+home);
        if((uint32_t)(int32_t)queued==actor){
            int16_t elapsed;W(0xbcd62eu+o)=(uint16_t)(W(0xbcd62eu+o)+1u);elapsed=(int16_t)W(0xbcd62eu+o);
            B(0x799a7fu+home)=120;
            if(elapsed>=80){B(0xbcd630u+o)=11;W(0xbcd62eu+o)=0;FUN_0048b4a0((int32_t)actor);W(0x799a2cu+home)=0;actor=U(0xd056f8u);}
        }else{
            for(i=0;i<3u;++i){
                int32_t id=(int16_t)W(0x799a86u+home+i*2u);
                if(id!=0){
                    uint32_t linked=docker_offset((uint32_t)id);
                    if(B(0xbcd606u+linked)==1u){
                        uint8_t phase=B(0xbcd630u+linked);
                        if(phase==9u || phase==10u){int32_t owner=(int16_t)W(0x799a2cu+home);if(owner==id)found=1;}
                    }
                }
            }
            if(!found)W(0x799a2cu+home)=0;
        }
        break;}
    case 10:
        FUN_0048b4a0((int32_t)actor);actor=U(0xd056f8u);o=docker_offset(actor);
        if((int16_t)W(0x799a2cu+home)<=0){W(0x799a2cu+home)=(uint16_t)actor;W(0xbcd62eu+o)=0;}
        if((uint32_t)(int32_t)(int16_t)W(0x799a2cu+home)==actor){
            B(0x799a7fu+home)=120;W(0xbcd62eu+o)=(uint16_t)(W(0xbcd62eu+o)+1u);
            if((int16_t)W(0xbcd62eu+o)>=80){B(0xbcd630u+o)=8;W(0xbcd62eu+o)=0;U(0xbcd5fcu+o)=0;U(0xbcd778u+o)=0;W(0x799a2cu+home)=0;}
        }
        W(0xbcd62eu+o)=(uint16_t)(W(0xbcd62eu+o)+1u);
        if((int16_t)W(0xbcd62eu+o)>=20){B(0xbcd630u+o)=8;W(0xbcd62eu+o)=0;}
        B(0xbcd5f9u+o)=0;break;
    case 11:case 12:case 13:{
        int32_t destination;
        actor=docker_move(actor,speed);o=docker_offset(actor);direction=B(0xbcd609u+o);
        if(direction==8u){B(0xbcd630u+o)=(uint8_t)(state==11u?15u:state==12u?16u:10u);if(state==13u)W(0xbcd62eu+o)=0;}
        else if(direction==9u){FUN_004b3250((int32_t)actor);actor=U(0xd056f8u);}
        else if(direction==10u)B(0xbcd606u+o)=2;
        o=docker_offset(actor);destination=(int16_t)W(0xbcd656u+o);
        if(B(0x7999d8u+(uint32_t)destination*264u)!=1u)B(0xbcd606u+o)=2;
        break;}
    case 15:
        FUN_0048b4a0((int32_t)actor);actor=U(0xd056f8u);o=docker_offset(actor);
        W(0xbcd62eu+o)=(uint16_t)(W(0xbcd62eu+o)+1u);
        if((int16_t)W(0xbcd62eu+o)>10){
            int32_t amount=0,destination;boat=(int16_t)W(0x799a8cu+home);
            if(boat!=0)amount=B(0xbcd666u+docker_offset((uint32_t)boat));
            destination=(int16_t)W(0xbcd656u+o);type=W(0x7999e8u+(uint32_t)destination*264u);
            if(type==72u || type==193u){
                int32_t cargo=B(0xbcd603u+o),result=FUN_00445980(destination,cargo,amount,boat);
                actor=U(0xd056f8u);
                if(result!=0){docker_set_home_target(actor);B(0xbcd603u+docker_offset(actor))=0;(void)FUN_00495950(home_id);actor=U(0xd056f8u);}
                else docker_set_home_target(actor);
            }else docker_set_home_target(actor);
            o=docker_offset(actor);W(0xbcd62eu+o)=0;
        }
        B(0xbcd5f9u+o)=0;break;
    case 16:{
        int32_t amount=0;
        FUN_0048b4a0((int32_t)actor);boat=(int16_t)W(0x799a8cu+home);
        if(boat!=0)amount=B(0xbcd666u+docker_offset((uint32_t)boat));
        actor=U(0xd056f8u);o=docker_offset(actor);W(0xbcd62eu+o)=(uint16_t)(W(0xbcd62eu+o)+1u);
        if((int16_t)W(0xbcd62eu+o)>10){
            int32_t destination,cargo,result;
            docker_set_home_target(actor);W(0xbcd62eu+o)=0;
            destination=(int16_t)W(0xbcd656u+o);cargo=B(0xbcd603u+o);
            result=FUN_00445e50(destination,cargo,amount,boat);
            if(result!=0){actor=U(0xd056f8u);B(0xbcd630u+docker_offset(actor))=13;}
            else{(void)FUN_00495950(home_id);actor=U(0xd056f8u);}
            o=docker_offset(actor);
        }
        B(0xbcd5f9u+o)=0;break;}
    case 14:
        actor=docker_move(actor,speed);o=docker_offset(actor);direction=B(0xbcd609u+o);
        if(direction==8u)B(0xbcd630u+o)=8;
        else if(direction==9u){FUN_004b3250((int32_t)actor);actor=U(0xd056f8u);}
        else if(direction==10u)B(0xbcd606u+o)=2;
        break;
    case 7:{
        uint32_t old_direction=U(0xe94e00u),base=U(0xf560dcu);
        U(0xbcd5fcu+o)=base+old_direction+1u;B(0xbcd5f9u+o)=0;FUN_0048b4a0((int32_t)actor);
        actor=U(0xd056f8u);o=docker_offset(actor);W(0xbcd62eu+o)=(uint16_t)(W(0xbcd62eu+o)+1u);
        if((int16_t)W(0xbcd62eu+o)>100){
            uint32_t result;W(0xbcd62eu+o)=0;result=FUN_004a35e0((int32_t)actor);actor=U(0xd056f8u);
            if((uint8_t)result==0u)B(0xbcd630u+docker_offset(actor))=8;
        }
        break;}
    case 6:U(0xbcd5fcu+o)=0;if(U(0xbcd778u+o)!=0u)U(0xbcd778u+o)=0;break;
    default:B(0xbcd630u+o)=8;break;
    }
    return docker_render(actor);
}

extern void FUN_004bd250(int32_t actor);
extern void FUN_004b17d0(int32_t actor);
extern void FUN_004b1d50(int32_t actor);
extern void FUN_004a5da0(int32_t actor,uint32_t speed);
extern void FUN_004b3250(int32_t actor);
extern int32_t FUN_004807d0(int32_t x,int32_t y,int32_t kind);
#define RESTORE_ACTOR_UPDATE_BARBARIAN_H(a) ((int32_t)(int16_t)W(a))
static uint32_t barbarian_actor(uint32_t id){return 0xbcd600u+id*388u;}

static uint32_t barbarian_render(uint32_t id)
{
    uint32_t actor=barbarian_actor(id),heading,base,image;uint8_t state=B(actor+0x30u),direction,timer;
    if(state==5)heading=(uint32_t)(int32_t)(int8_t)B(actor+11u);
    else {
        direction=B(actor+9u);
        if(direction==11)heading=(uint32_t)(int32_t)(int8_t)B(actor+11u);
        else if((int8_t)direction>=8)heading=(uint32_t)(int32_t)(int8_t)B(actor+10u);
        else heading=(uint32_t)(int32_t)(int8_t)direction;
    }
    heading-=U(0xbca2e0u);U(0xe94e00u)=heading;
    if((int32_t)heading<0){heading+=8u;U(0xe94e00u)=heading;}
    base=U(0xf563bcu);
    if(state==5) {
        timer=B(actor+0x62u);
        if(timer<12){image=base+0xc0u+heading;U(actor-4u)=image;return image;}
        timer=(uint8_t)(timer-12u);
        image=base+0xc0u+heading+(uint32_t)(timer/2u)*8u;U(actor-4u)=image;return timer/2u;
    }
    if(state==4) {
        int32_t half=(int32_t)RESTORE_ACTOR_UPDATE_BARBARIAN_H(actor+0x2eu)/2;
        uint32_t frame=B(0x5d048cu+(uint32_t)half);
        image=base+0xf0u+frame;U(actor-4u)=image;return image;
    }
    if(B(actor+9u)==11)image=base+0xc0u+heading+(uint32_t)(B(actor-7u)>>1)*8u;
    else if(state==11)image=base+0x60u+heading+(uint32_t)B(actor-7u)*8u;
    else image=base+heading+(uint32_t)B(actor-7u)*8u;
    U(actor-4u)=image;return image;
}

__declspec(noinline) uint32_t restore_actor_update_barbarian(void)
{
    uint32_t id=U(0xd056f8u),actor=barbarian_actor(id),grid=U(actor+0x18u),kind,speed,building;
    uint8_t animation,state,direction;
    if((U(0xaf598cu+grid*4u)&0x10004u)==0x10004u) {
        FUN_004bd250((int32_t)id);
        /* Native21-byte child returns its untouched argument in EAX. */
        return id;
    }
    kind=B(actor+2u);B(actor+0x6au)=0;B(actor+4u)=0;W(actor+0x3au)=800;
    building=0x7999d8u+(uint32_t)(int32_t)RESTORE_ACTOR_UPDATE_BARBARIAN_H(actor+0x52u)*264u;
    speed=U(0x7090ecu+kind*56u);
    if(B(building)!=1)B(actor+6u)=2;
    else if((uint32_t)(int32_t)RESTORE_ACTOR_UPDATE_BARBARIAN_H(building+0x2au)!=id)B(actor+6u)=2;
    animation=(uint8_t)(B(actor-7u)+1u);B(actor-7u)=animation;
    if(animation>=12)B(actor-7u)=0;
    state=B(actor+0x30u);
    switch(state) {
    case 5:FUN_004b17d0((int32_t)id);id=U(0xd056f8u);break;
    case 4:FUN_004b1d50((int32_t)id);id=U(0xd056f8u);break;
    case 11:
    {
        uint32_t profile=U(0xeaac28u)*0x49d8u;
        B(actor+0x6au)=2;U(0xea5db4u+profile)=10;
        U(0xea5f98u+profile)=U(0xea5f98u+profile)+1u;
        FUN_004a5da0((int32_t)id,speed);id=U(0xd056f8u);actor=barbarian_actor(id);
        direction=B(actor+9u);if(direction==9||direction==10||direction==8)B(actor+0x30u)=10;
        break;
    }
    case 10:
    {
        B(actor-7u)=0;W(actor+0x2eu)=(uint16_t)(W(actor+0x2eu)+1u);
        if((int32_t)RESTORE_ACTOR_UPDATE_BARBARIAN_H(actor+0x2eu)>(int32_t)((id&3u)+10u)) {
            int32_t linked_building=RESTORE_ACTOR_UPDATE_BARBARIAN_H(building+0x12u);
            uint32_t profile=U(0xeaac28u)*0x49d8u;
            W(actor+0x2eu)=0;
            if(U(0xea5e04u+profile)!=0) {
                uint16_t value=W(0xbcba02u);
                B(actor+0x30u)=11;W(actor+0x1cu)=value;
                value=W(0xbcba04u);W(actor+0x1eu)=value;
                value=W(0xbcba06u);W(actor+0x56u)=value;
            } else {
                uint32_t target_building=0x7999d8u+(uint32_t)linked_building*264u;
                int32_t target_kind=B(target_building+3u),target_y=RESTORE_ACTOR_UPDATE_BARBARIAN_H(target_building+8u),target_x=RESTORE_ACTOR_UPDATE_BARBARIAN_H(target_building+6u);
                int32_t found=FUN_004807d0(target_x,target_y,target_kind);
                id=U(0xd056f8u);
                if(found!=0) {
                    uint16_t value=W(0xd3b4e4u);
                    actor=barbarian_actor(id);B(actor+0x30u)=8;W(actor+0x1cu)=value;
                    value=W(0xd3b4e0u);W(actor+0x1eu)=value;
                }
            }
            FUN_004b3250((int32_t)id);id=U(0xd056f8u);
        }
        break;
    }
    case 8:
        FUN_004a5da0((int32_t)id,speed);id=U(0xd056f8u);actor=barbarian_actor(id);
        direction=B(actor+9u);
        if(direction==9||direction==10)B(actor+6u)=2;
        else if(direction==8) {
            B(actor+0x30u)=9;W(actor+0x1cu)=W(actor+0x24u);W(actor+0x1eu)=W(actor+0x26u);
        }
        break;
    case 9:
        FUN_004a5da0((int32_t)id,speed);id=U(0xd056f8u);actor=barbarian_actor(id);
        direction=B(actor+9u);if(direction==9||direction==10)B(actor+6u)=2;
        if(B(actor+9u)==8)B(actor+6u)=2;
        break;
    default:B(actor+0x30u)=10;break;
    }
    return barbarian_render(id);
}

#undef RESTORE_ACTOR_UPDATE_BARBARIAN_H

extern int32_t FUN_00497cf0(int32_t actor);
extern int32_t FUN_00497a30(int32_t actor);
extern void FUN_0053b4a0(int32_t actor,int32_t sound,int32_t mode);
extern void FUN_004b17d0(int32_t actor);
extern void FUN_004b1d50(int32_t actor);
extern void FUN_004a5da0(int32_t actor,uint32_t speed);
extern void FUN_004b5420(int32_t actor);
extern void FUN_004b52f0(int32_t actor);
extern void FUN_004b3250(int32_t actor);
extern void FUN_004b28a0(int32_t actor,int32_t mode);
extern int32_t FUN_00499b50(int32_t actor,int32_t range);
extern uint8_t FUN_0051cd80(int32_t x,int32_t y,int32_t tx,int32_t ty);
extern int32_t FUN_004b45c0(int32_t actor,int32_t x,int32_t y,int32_t tx,int32_t ty,int32_t kind);
extern uint32_t FUN_004a35e0(int32_t actor);
#define RESTORE_ACTOR_UPDATE_SENTRY_H(a) ((int32_t)(int16_t)W(a))
static uint32_t sentry_actor(uint32_t id){return 0xbcd600u+id*388u;}

__declspec(noinline) uint32_t restore_actor_update_sentry(void)
{
    uint32_t id=U(0xd056f8u),actor=sentry_actor(id),kind=B(actor+2u),building,speed,heading,base,flags;
    uint8_t state,animation,direction,timer;
    B(actor+0x6au)=4;B(actor+4u)=0;B(actor+0x5du)=1;B(actor+0x6fu)=10;W(actor+0x3au)=800;
    building=0x7999d8u+(uint32_t)(int32_t)RESTORE_ACTOR_UPDATE_SENTRY_H(actor+0x52u)*264u;
    speed=U(0x7090ecu+kind*56u);
    if(B(building)!=1)B(actor+6u)=2;
    else if((uint32_t)(int32_t)RESTORE_ACTOR_UPDATE_SENTRY_H(building+0x2au)!=id)B(actor+6u)=2;
    animation=(uint8_t)(B(actor-7u)+1u);B(actor-7u)=animation;if(animation>=12)B(actor-7u)=0;
    state=B(actor+0x30u);
    if(state!=12&&state!=7&&state!=6){(void)FUN_00497cf0((int32_t)id);id=U(0xd056f8u);actor=sentry_actor(id);}
    state=B(actor+0x30u);
    switch(state) {
    case 5:
        FUN_0053b4a0((int32_t)id,9,2);id=U(0xd056f8u);FUN_004b17d0((int32_t)id);id=U(0xd056f8u);break;
    case 4:FUN_004b1d50((int32_t)id);id=U(0xd056f8u);break;
    case 8:
        B(actor-7u)=0;W(actor+0x2eu)=(uint16_t)(W(actor+0x2eu)+1u);
        if(RESTORE_ACTOR_UPDATE_SENTRY_H(actor+0x2eu)>40) {
            int32_t found;
            W(actor+0x2eu)=0;found=FUN_00497a30((int32_t)id);
            if(found!=0) {
                uint16_t value;
                id=U(0xd056f8u);value=W(0xd3b4e4u);actor=sentry_actor(id);
                B(actor+0x30u)=9;W(actor+0x1cu)=value;value=W(0xd3b4e0u);W(actor+0x1eu)=value;
                FUN_004b3250((int32_t)id);
            }
            id=U(0xd056f8u);
        }
        break;
    case 12:
        B(actor+0x6au)=10;B(actor+0x5du)=0;B(actor+0x6fu)=0;
        FUN_004a5da0((int32_t)id,speed);id=U(0xd056f8u);actor=sentry_actor(id);direction=B(actor+9u);
        if(direction==9||direction==10)B(actor+6u)=2;
        else if(direction==8) {
            uint16_t x,y;uint32_t grid;
            FUN_004b5420((int32_t)id);id=U(0xd056f8u);
            x=W(building+6u);y=W(building+8u);grid=U(0xbca2d8u);actor=sentry_actor(id);
            W(actor+12u)=x;W(actor+0x24u)=x;W(actor+14u)=y;W(actor+0x26u)=y;
            {int32_t fresh_y=RESTORE_ACTOR_UPDATE_SENTRY_H(actor+14u),fresh_x=RESTORE_ACTOR_UPDATE_SENTRY_H(actor+12u);U(actor+0x18u)=grid+(uint32_t)fresh_x+(uint32_t)fresh_y*228u;}
            FUN_004b52f0((int32_t)id);id=U(0xd056f8u);actor=sentry_actor(id);B(actor+0x30u)=8;
            FUN_004b3250((int32_t)id);id=U(0xd056f8u);
        }
        break;
    case 9:
        FUN_004a5da0((int32_t)id,speed);id=U(0xd056f8u);actor=sentry_actor(id);direction=B(actor+9u);
        if(direction==9||direction==10)B(actor+0x30u)=8;
        else if(direction==8) {
            B(actor+0x30u)=11;W(actor+0x1cu)=W(actor+0x24u);W(actor+0x1eu)=W(actor+0x26u);
            FUN_004b3250((int32_t)id);id=U(0xd056f8u);
        }
        break;
    case 11:
        FUN_004a5da0((int32_t)id,speed);id=U(0xd056f8u);actor=sentry_actor(id);direction=B(actor+9u);
        if(direction==9||direction==10)B(actor+6u)=2;
        else if(direction==8)B(actor+0x30u)=8;
        break;
    case 10:
        FUN_004b28a0((int32_t)id,1);id=U(0xd056f8u);actor=sentry_actor(id);
        timer=(uint8_t)(B(actor+0x63u)+1u);B(actor+0x63u)=timer;kind=B(actor+2u);
        if((int32_t)timer>(int32_t)U(0x7090e8u+kind*56u)) {
            int32_t range=(int32_t)U(0x7090e4u+kind*56u);
            if(FUN_00499b50((int32_t)id,range)!=0) {
                uint32_t cached_actor,tx,ty;int32_t x,y;uint8_t aimed_direction;
                id=U(0xd056f8u);FUN_0053b4a0((int32_t)id,9,2);id=U(0xd056f8u);
                ty=U(0xd3b4e0u);tx=U(0xd3b4e4u);cached_actor=sentry_actor(id);
                y=RESTORE_ACTOR_UPDATE_SENTRY_H(cached_actor+14u);x=RESTORE_ACTOR_UPDATE_SENTRY_H(cached_actor+12u);
                aimed_direction=FUN_0051cd80(x,y,(int32_t)tx,(int32_t)ty);
                tx=U(0xd3b4e4u);B(cached_actor+9u)=aimed_direction;
                ty=U(0xd3b4e0u);B(cached_actor+0x63u)=0;
                y=RESTORE_ACTOR_UPDATE_SENTRY_H(cached_actor+14u);x=RESTORE_ACTOR_UPDATE_SENTRY_H(cached_actor+12u);id=U(0xd056f8u);
                (void)FUN_004b45c0((int32_t)id,x,y,(int32_t)tx,(int32_t)ty,60);
                id=U(0xd056f8u);
            } else {
                id=U(0xd056f8u);actor=sentry_actor(id);B(actor+0x30u)=11;
                W(actor+0x1cu)=W(actor+0x24u);W(actor+0x1eu)=W(actor+0x26u);
                FUN_004b3250((int32_t)id);id=U(0xd056f8u);
            }
        }
        break;
    case 6:B(actor+0x6au)=10;U(actor-4u)=0;U(actor+0x178u)=0;break;
    case 7:
        B(actor+0x6au)=10;B(actor-7u)=0;W(actor+0x2eu)=(uint16_t)(W(actor+0x2eu)+1u);
        if(RESTORE_ACTOR_UPDATE_SENTRY_H(actor+0x2eu)>100) {
            uint32_t result;
            W(actor+0x2eu)=0;result=FUN_004a35e0((int32_t)id);id=U(0xd056f8u);
            if((result&255u)==0){actor=sentry_actor(id);B(actor+6u)=2;}
        }
        break;
    default:B(actor+0x30u)=8;break;
    }
    actor=sentry_actor(id);flags=U(0xaf598cu+U(actor+0x18u)*4u);
    if(flags&0x4000u)B(actor+0x70u)=18;
    else if(flags&0x8000u)B(actor+0x38u)=24;
    else {state=B(actor+0x30u);if(state!=12&&state!=7&&state!=6)B(actor+6u)=2;}
    timer=B(actor+0x38u);if(timer!=0){B(actor+0x38u)=(uint8_t)(timer-1u);B(actor+0x6fu)=0;}
    heading=(uint32_t)(int32_t)(int8_t)B(actor+9u);heading-=U(0xbca2e0u);U(0xe94e00u)=heading;
    if((int32_t)heading<0){heading+=8u;U(0xe94e00u)=heading;}
    state=B(actor+0x30u);
    if(state==4) {
        int32_t half=(int32_t)RESTORE_ACTOR_UPDATE_SENTRY_H(actor+0x2eu)/2;uint32_t frame=B(0x5d048cu+(uint32_t)half);
        base=U(0xf55ea4u);U(actor-4u)=base+frame;return base;
    }
    if(state==10) {
        uint32_t frame=B(0x5d050cu+(uint32_t)(B(actor+0x63u)>>1));
        base=U(0xf55eacu);U(actor-4u)=base+frame*8u+heading;return heading;
    }
    base=U(0xf55e9cu);animation=B(actor-7u);U(actor-4u)=base+(uint32_t)animation*8u+heading;return heading;
}

#undef RESTORE_ACTOR_UPDATE_SENTRY_H

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
