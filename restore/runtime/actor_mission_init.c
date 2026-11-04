#include "actor_mission_init.h"
#include "actor_create.h"
#include "editor_validation.h"
#include "town_menu.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
extern void FUN_0040f8f0(uint32_t *destination,uint32_t bytes,uint8_t value);
extern uint32_t FUN_00562348(void);
__declspec(noinline) void restore_actor_reset_pool(void)
{
 uint32_t record;
 for(record=0xbcd5f8u;record<0xc8ad38u;record+=388u)
  FUN_0040f8f0((uint32_t *)(uintptr_t)record,388,0);
 U(0xbcd5f0u)=0;U(0xbcd5f4u)=0;
}
__declspec(noinline) void FUN_004b4760(void){restore_actor_reset_pool();}
__declspec(noinline) void restore_town_actor_links_00448a60(void *self)
{
 uint32_t p=(uint32_t)(uintptr_t)self;
 W(p+0x3au)=0;W(p+0x3cu)=0;W(p+0x3eu)=0;W(p+0x40u)=0;
 W(p+0x42u)=0;W(p+0x44u)=0;W(p+0x46u)=0;W(p+0x4au)=0;
}
__declspec(noinline) void restore_actor_reset_town_links(void)
{
 RestoreTownRecord *record=restore_town_next(RESTORE_TOWNS,0,1);
 while(record){
  restore_town_actor_links_00448a60(record);
  record=restore_town_next(RESTORE_TOWNS,(const int16_t *)record,1);
 }
 U(0xd0563cu)=0;U(0xd05638u)=0;U(0xd05634u)=0;
 U(0xd05630u)=0;U(0xd0562cu)=0;
}
__declspec(noinline) void FUN_004b8c90(void){restore_actor_reset_town_links();}
__declspec(noinline) void restore_actor_initialize_reserved(void)
{
 uint32_t actor,tag;
 FUN_004b4760();FUN_004b8c90();
 for(tag=1;tag<=3u;++tag){
  actor=(uint32_t)FUN_004b4800(1,36,-1,-1,0);
  B(0xbcd603u+actor*388u)=(uint8_t)tag;
 }
 /* Native fourth allocation has no tag store, including on failure. */
 (void)FUN_004b4800(1,36,-1,-1,0);
 for(tag=12;tag<=32u;++tag){
  actor=(uint32_t)FUN_004b4800(1,36,-1,-1,0);
  B(0xbcd603u+actor*388u)=(uint8_t)tag;
 }
 for(tag=50;tag<=65u;++tag){
  actor=(uint32_t)FUN_004b4800(1,36,-1,-1,0);
  B(0xbcd603u+actor*388u)=(uint8_t)tag;
 }
}
/* The scenario RNG is a pair of31-step shift registers, separate from the
   CRT LCG used for actor-local delays. Cached pairs cross group iterations. */
typedef struct ScenarioRandom {uint32_t first,second;} ScenarioRandom;
static ScenarioRandom load_scenario_random(void)
{
 ScenarioRandom value;
 value.first=U(0xd3b384u);value.second=U(0xd3b380u);return value;
}
static ScenarioRandom advance_scenario_random(ScenarioRandom value)
{
 uint32_t step;
 for(step=0;step<31u;++step){
  uint32_t feedback_first=(value.first^(value.first>>4))&1u;
  uint32_t feedback_second=(value.second^(value.second>>4))&1u;
  value.first=(value.first>>1)|(feedback_first<<30);
  value.second=(value.second>>1)|(feedback_second<<30);
 }
 return value;
}
static uint32_t record_scenario_random(uint32_t index,uint32_t sample)
{
 U(0xd3b1c8u+index*4u)=sample;
 ++index;U(0xd3b1c4u)=index;
 if((int32_t)index>=100){index=0;U(0xd3b1c4u)=0;}
 return index;
}
static void publish_search_random(ScenarioRandom value)
{
 U(0xd3b370u)=value.first&0x7fffu;U(0xd3b374u)=value.first&127u;
 U(0xd3b384u)=value.first;U(0xd3b380u)=value.second;
 U(0xd3b378u)=value.first&7u;U(0xd3b364u)=value.second&0x7fffu;
 U(0xd3b368u)=value.second&127u;U(0xd3b36cu)=value.second&7u;
}
static void publish_creation_prefix(ScenarioRandom value,int word_coordinates)
{
 U(0xd3b384u)=value.first;U(0xd3b378u)=value.first&7u;
 if(word_coordinates){
  U(0xd3b370u)=value.first&0x7fffu;U(0xd3b364u)=value.second&0x7fffu;
  U(0xd3b380u)=value.second;
 }else{
  U(0xd3b364u)=value.second&0x7fffu;U(0xd3b380u)=value.second;
  U(0xd3b370u)=value.first&0x7fffu;
 }
}
static void publish_member_random(ScenarioRandom value)
{
 U(0xd3b374u)=value.first&127u;U(0xd3b370u)=value.first&0x7fffu;
 U(0xd3b378u)=value.first&7u;U(0xd3b364u)=value.second&0x7fffu;
 U(0xd3b380u)=value.second;U(0xd3b368u)=value.second&127u;
 U(0xd3b384u)=value.first;U(0xd3b36cu)=value.second&7u;
}
static uint32_t scenario_probe_terrain(uint32_t coordinate,uint32_t offsets,uint32_t candidate,int word_coordinates)
{
 uint32_t x,y,dx,dy,base;
 if(word_coordinates){
  y=(uint32_t)(int32_t)(int16_t)W(coordinate+8u);
  dy=U(offsets+candidate*8u+4u);dx=U(offsets+candidate*8u);
  base=U(0xbca2d8u);x=(uint32_t)(int32_t)(int16_t)W(coordinate);
 }else{
  dy=U(offsets+candidate*8u+4u);y=U(coordinate+16u);
  x=U(coordinate);dx=U(offsets+candidate*8u);base=U(0xbca2d8u);
 }
 return U(0xaf598cu+(base+(y+dy)*228u+x+dx)*4u);
}
static void initialize_scenario_group(uint32_t coordinate,int word_coordinates,uint32_t kind,uint32_t count,uint32_t group)
{
 uint32_t group_offset=group*144u,ordinal=0,history,candidate,attempt,actor,actor_offset;
 uint32_t x,y,dx,dy,sample;
 uint16_t center_x=W(coordinate),center_y=W(coordinate+(word_coordinates?8u:16u));
 uint32_t offsets=word_coordinates?0x5cd978u:0x5cdb78u;
 ScenarioRandom random;
 W(0xbcba02u+group_offset)=center_x;W(0xbcba04u+group_offset)=center_y;
 B(0xbcba3cu+group_offset)=0;B(0xbcba31u+group_offset)=(uint8_t)(word_coordinates?1:2);
 W(0xbcba16u+group_offset)=24;B(0xbcb9f1u+group_offset)=(uint8_t)count;
 if((int32_t)count<=0)return;
 history=U(0xd3b1c4u);random=load_scenario_random();
 do{
  sample=U(0xd3b374u);history=record_scenario_random(history,sample);
  random=advance_scenario_random(random);publish_search_random(random);
  candidate=ordinal;attempt=0;
  do{
   uint32_t terrain=scenario_probe_terrain(coordinate,offsets,candidate,word_coordinates);
   if((uint8_t)FUN_004ec410(terrain,(char)(word_coordinates?0:1)))break;
   ++candidate;if((int32_t)candidate>=16)candidate=0;
   ++attempt;
  }while(attempt<16u);
  /* Exhaustion still creates an actor at the resulting next candidate. */
  history=U(0xd3b1c4u);sample=U(0xd3b374u);
  history=record_scenario_random(history,sample);
  random=load_scenario_random();random=advance_scenario_random(random);
  publish_creation_prefix(random,word_coordinates);
  if(word_coordinates){
   y=(uint32_t)(int32_t)(int16_t)W(coordinate+8u);
   U(0xd3b36cu)=random.second&7u;
   dy=U(offsets+candidate*8u+4u);U(0xd3b368u)=random.second&127u;
   x=(uint32_t)(int32_t)(int16_t)W(coordinate);dx=U(offsets+candidate*8u);
  }else{
   U(0xd3b36cu)=random.second&7u;
   y=U(coordinate+16u);dy=U(offsets+candidate*8u+4u);
   dx=U(offsets+candidate*8u);x=U(coordinate);
   U(0xd3b368u)=random.second&127u;
  }
  U(0xd3b374u)=random.first&127u;
  actor=(uint32_t)FUN_004b4800(1,(int32_t)kind,(int32_t)(x+dx),(int32_t)(y+dy),(uint8_t)(random.first&7u));
  history=U(0xd3b1c4u);actor_offset=actor*388u;sample=U(0xd3b374u);
  U(0xd3b1c8u+history*4u)=sample;++history;
  B(0xbcd630u+actor_offset)=24;W(0xbcd658u+actor_offset)=(uint16_t)group;
  if(word_coordinates)W(0xbcd62eu+actor_offset)=0;
  U(0xd3b1c4u)=history;
  if((int32_t)history>=100){history=0;U(0xd3b1c4u)=0;}
  random=load_scenario_random();random=advance_scenario_random(random);
  publish_member_random(random);
  W(0xbcd62eu+actor_offset)=(uint16_t)((random.first&0x7fffu)%25u*(word_coordinates?4u:1u));
  B(0xbcd65au+actor_offset)=(uint8_t)ordinal;
  ++ordinal;
 }while((int32_t)ordinal<(int32_t)count);
}
__declspec(noinline) void restore_actor_initialize_scenario_groups(void)
{
 uint32_t scene=U(0x784cb0u),coordinate,index,kind_dword,count_dword,kind_word,count_word;
 uint32_t group_mode;int32_t x,group;
 index=(scene&255u)*48u;kind_dword=U(0x5e4c20u+index);count_dword=U(0x5e4c28u+index);
 index=((uint32_t)(int32_t)(int16_t)W(0x784b94u)+(scene&255u)*2u)*48u;
 kind_word=U(0x5e4cb0u+index);count_word=U(0x5e4cb8u+index);
 /* Original performs these table reads before checking editor mode. */
 if(U(0x605a3cu)!=0)return;
 for(coordinate=0x784b64u;coordinate<0x784b6cu;coordinate+=2u){
  x=(int32_t)(int16_t)W(coordinate);
  if(x>0){
   uint16_t y=W(coordinate+8u);
   index=((uint32_t)(int32_t)(int16_t)W(0x784b94u)+(scene&255u)*2u)*48u;
   group_mode=U(0x5e4cdcu+index);
   group=FUN_004b6ff0((uint16_t)kind_word,(int32_t)group_mode,0,(uint16_t)x,y);
   if(group>0)initialize_scenario_group(coordinate,1,kind_word,count_word,(uint32_t)group);
   scene=U(0x784cb0u);
  }
 }
 for(coordinate=0x784cc0u;coordinate<0x784cd0u;coordinate+=4u){
  x=(int32_t)U(coordinate);
  if(x>0){
   uint32_t y=U(coordinate+16u);
   group_mode=U(0x5e4c4cu+(scene&255u)*48u);
   group=FUN_004b6ff0((uint16_t)kind_dword,(int32_t)group_mode,0,(uint16_t)x,(uint16_t)y);
   if(group>0)initialize_scenario_group(coordinate,0,kind_dword,count_dword,(uint32_t)group);
   scene=U(0x784cb0u);
  }
 }
}
__declspec(noinline) void restore_actor_initialize_scenario_points(void)
{
 uint32_t coordinate,index,sample,actor;int32_t x,y;ScenarioRandom random;
 if(U(0x605a3cu)!=0)return;
 for(coordinate=0x784b74u;coordinate<0x784b84u;coordinate+=2u){
  x=(int32_t)(int16_t)W(coordinate);
  if(x>0){
   index=U(0xd3b1c4u);sample=U(0xd3b374u);
   (void)record_scenario_random(index,sample);
   random=load_scenario_random();random=advance_scenario_random(random);
   U(0xd3b384u)=random.first;U(0xd3b380u)=random.second;
   U(0xd3b370u)=random.first&0x7fffu;U(0xd3b374u)=random.first&127u;
   U(0xd3b378u)=random.first&7u;U(0xd3b36cu)=random.second&7u;
   U(0xd3b364u)=random.second&0x7fffu;
   y=(int32_t)(int16_t)W(coordinate+16u);
   U(0xd3b368u)=random.second&127u;
   actor=(uint32_t)FUN_004b4800(1,95,x,y,0);
   sample=FUN_00562348();B(0xbcd5f9u+actor*388u)=(uint8_t)(sample&31u);
  }
 }
}
void FUN_004bb090(void){restore_actor_initialize_reserved();}
void FUN_004bb3f0(void){restore_actor_initialize_scenario_groups();}
void FUN_004bbc10(void){restore_actor_initialize_scenario_points();}
