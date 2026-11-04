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

static void dispatch(uint32_t site,uint32_t target,struct restore_actor_command_frame *f)
{
 uint32_t sp=f->r[4];(void)site;
 switch(target){
#define RAW(a) case 0x##a##u:restore_actor_command_C_##a(f);return;
 RAW(0041cd00)
#undef RAW
 }
#define A(n) U(sp+(uint32_t)(n)*4u)
 switch(target){
 case 0x4ec990u:f->r[0]=restore_actor_seek_other_kind((int32_t)A(1));break;
 case 0x4ec7c0u:f->r[0]=restore_actor_choose_command_state((int32_t)A(1));break;
 case 0x4ec4a0u:f->r[0]=restore_actor_apply_command_state((int32_t)A(1),A(2));break;
 case 0x4eae20u:f->r[0]=restore_actor_find_other_kind_on_tile(A(1),A(2));break;
 case 0x4ec0f0u:f->r[0]=FUN_004ec0f0((int32_t)A(1),(int32_t)A(2),
  (volatile int32_t *)(uintptr_t)A(3),(volatile int32_t *)(uintptr_t)A(4),
  (int32_t)A(5),A(6),(restore_actor_tile_callback)(uintptr_t)A(7));break;
 case 0x4b4800u:
  f->r[0]=(uint32_t)FUN_004b4800((uint8_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),(uint8_t)A(5));break;
 case 0x4b6ff0u:
  f->r[0]=(uint32_t)FUN_004b6ff0((uint16_t)A(1),(int32_t)A(2),(int32_t)A(3),(uint16_t)A(4),(uint16_t)A(5));break;
 case 0x4ec410u:f->r[0]=FUN_004ec410(A(1),(char)A(2));break;
 case 0x4ea9c0u:f->r[0]=(uint32_t)FUN_004ea9c0((int32_t)A(1));break;
 case 0x4eada0u:f->r[0]=(uint32_t)FUN_004eada0((int32_t)A(1));break;
 case 0x4eb800u:
  /* Actual native9-DWORD/RET0 ABI, no ECX. The caller's output buffers are
     VM-stack words, disjoint from map/type data, exactly its accepted domain. */
  f->r[0]=(uint32_t)restore_building_nearest_search(
   (int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(const uint32_t *)(uintptr_t)A(4),A(5),
   (int32_t *)(uintptr_t)A(6),(int32_t *)(uintptr_t)A(7),(int32_t)A(8),(int32_t)A(9));break;
 default:abort();
 }
 f->r[4]=sp+4u; /* Every genuine external target here is native RET0. */
#undef A
}
static uint32_t invoke(uint32_t target,const uint32_t *args,uint32_t count)
{
 uint32_t stack[512],entry,i;struct restore_actor_command_frame f;
 for(i=0;i<512;++i)stack[i]=SENTRY;
 for(i=0;i<8;++i)f.r[i]=0;
 /* Command41CF7C writes actor state25. EC7C0's state25 weights50/50
    select only table5E4E70 entries0/1 =19/18, both nonrecursive EC4A0
    branches. Thus the command-owned raw stack floor is -172 bytes.
    For a valid generic EC990 actor state, only the default weights can
    choose table entry3=25; that permits at most one backedge, floor-208.
    No state clamp or artificial iteration cutoff implements this invariant. */
 entry=(uint32_t)(uintptr_t)(stack+384);
 for(i=0;i<count;++i)stack[385+i]=args[i];
 f.r[4]=entry;f.flags=2;f.boundary=dispatch;dispatch(0,target,&f);
 if(f.r[4]!=entry+4u||stack[384]!=SENTRY||f.r[3]||f.r[5]||f.r[6]||f.r[7])abort();
 for(i=0;i<32;++i)if(stack[i]!=SENTRY)abort();
 for(i=385+count;i<512;++i)if(stack[i]!=SENTRY)abort();
 return f.r[0];
}
void restore_actor_command_0041cd00(void){invoke(0x41cd00u,0,0);}
uint32_t restore_actor_command_004ec990(int32_t actor)
{return restore_actor_seek_other_kind(actor);}
__declspec(noinline) uint32_t FUN_004ec4a0(int32_t actor,uint32_t state)
{return restore_actor_apply_command_state(actor,state);}

/* Immutable command table57C3C8 row3 names this genuine nullary callback.
   Its two native local DWORDs are written before use; none of the six newly
   owned bodies reads a30-byte payload or retired decoder storage. */
int restore_actor_command_action(uint32_t token)
{
 if(token!=0x41cd00u)return 0;
 restore_actor_command_0041cd00();return 1;
}

/* At the reviewed native caller sites ECX is reloaded and flags overwritten;
   EDX is overwritten before use. Only EAX and the RET0 stack movement are live.
   The command41CFA8 suffix also overwrites flags before its next branch. */
void restore_actor_command_raw(uint32_t target,uint32_t registers[8],uint32_t *flags)
{
 struct restore_actor_command_frame f;
 memcpy(f.r,registers,sizeof(f.r));f.flags=*flags;f.boundary=dispatch;
 dispatch(0,target,&f);
 memcpy(registers,f.r,sizeof(f.r));*flags=f.flags;
}
