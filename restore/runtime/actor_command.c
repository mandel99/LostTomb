#include "actor_command.h"
#include "actor_create.h"
#include "actor_combat.h"
#include "editor_validation.h"
#include "building_nearest_search.h"
#include <stdlib.h>
#include <string.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define SENTRY UINT32_C(0x41cd0a17)
typedef char actor_command_pointer32[sizeof(void*)==4?1:-1];
extern int32_t FUN_004eada0(int32_t kind);
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
