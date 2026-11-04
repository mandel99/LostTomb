#include "building_destruction.h"
#include "storage_delivery.h"
#include "building_removal.h"
#include "god_building_effects.h"
#include "menu_actor_message.h"
#include "menu_prepare_integer.h"
#include "placement_support.h"
#include "building_allocator.h"
#include "building_footprint.h"
#include "building_release_model.h"
#include "clear_footprint.h"
#include "clear_helpers.h"
#include "map_commit_connect.h"
#include "map_commit_costs.h"
#include "displaced_population.h"
#include "event_publisher.h"
#include "mission_unlock.h"
#include "menu_list_scroll.h"
#include "surface_passes.h"
#include "terrain_redraw.h"
#include "road_place.h"
#include "map_refresh_postpass1.h"
#include "map_refresh_postpass3.h"
#include "water_refresh.h"
#include "road_paint_model.h"
#include <stdlib.h>
#include <string.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define SENTRY UINT32_C(0x0d547eff)
typedef char god_pointer32[sizeof(void*)==4?1:-1];
extern uint32_t FUN_00562348(void);
extern uint32_t FUN_0047d350(void);
extern void FUN_005178b0(void);
extern void FUN_00475cc0(int32_t,int32_t,int32_t,int32_t,uint32_t);

static void dispatch(uint32_t site,uint32_t target,struct restore_god_building_frame *f)
{
 uint32_t sp=f->r[4];(void)site;
 /* Owned true native bodies share the same virtual stack and perform their
    native RET themselves. Includes the original sort and real comparator. */
 switch(target){
#define RAW(a) case 0x##a##u:restore_god_C_##a(f);return;
 RAW(0043fd00) RAW(0045b2a0) RAW(0045b700) RAW(0045fe20)
 RAW(004695d0) RAW(0046d0f0) RAW(0046d100) RAW(004bfee0)
 RAW(004c04a0) RAW(004c3950) RAW(004c3ac0) RAW(004c3ed0)
 RAW(004c4020) RAW(004c41b0) RAW(004c42b0) RAW(004c45e0)
 RAW(004c45a0) RAW(004e9170) RAW(00562069) RAW(005621bd) RAW(0056220b)
#undef RAW
 }
 if(target==0x45b230u||target==0x45d510u){
  struct restore_actor_message_frame m;
  memcpy(m.r,f->r,sizeof(m.r));m.flags=f->flags;m.boundary=restore_actor_message_dispatch;
  if(target==0x45b230u)restore_actor_message_C_0045b230(&m);
  else restore_actor_message_C_0045d510(&m);
  memcpy(f->r,m.r,sizeof(m.r));f->flags=m.flags;return;
 }
 if(target==0x459ef0u||target==0x45b490u){
  struct restore_placement_support_frame m;
  /* Both accepted placement bodies are actual leaves with no CALL. */
  memcpy(m.r,f->r,sizeof(m.r));m.flags=f->flags;m.boundary=0;
  if(target==0x459ef0u)restore_placement_support_C_00459ef0(&m);
  else restore_placement_support_C_0045b490(&m);
  memcpy(f->r,m.r,sizeof(m.r));f->flags=m.flags;return;
 }
 /* The authenticated native callers consume only EAX from the root query. */
 if(target==0x475b50u){f->r[0]=FUN_00475b50(U(sp+4u));f->r[4]=sp+4u;return;}
#define A(n) U(sp+(uint32_t)(n)*4u)
 switch(target){
 case 0x562348u:f->r[0]=FUN_00562348();break;
 case 0x47d350u:f->r[0]=FUN_0047d350();break;
 case 0x5013a0u:f->r[0]=FUN_005013a0(A(1));break;
 case 0x417ae0u:FUN_00417ae0((int32_t)A(1));break;
 case 0x46a400u:
  f->r[0]=(uint32_t)restore_building_allocate_0046a400(A(1),A(2),(int32_t)A(3),(int32_t)A(4),(int32_t)A(5),(int32_t)A(6));break;
 case 0x46ac50u:FUN_0046ac50((int32_t)A(1));break;
 case 0x47bf50u:FUN_0047bf50((uint16_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),(int32_t)A(5),A(6),A(7));break;
 case 0x47c480u:FUN_0047c480((int32_t)A(1),(int32_t)A(2),(int32_t)A(3));break;
 case 0x47c2b0u:FUN_0047c2b0((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),(int32_t)A(5));break;
 case 0x53b090u:FUN_0053b090((int32_t)A(1));break;
 case 0x517370u:FUN_00517370();break;
 case 0x517660u:FUN_00517660();break;
 case 0x5178b0u:FUN_005178b0();break;
 case 0x516f90u:FUN_00516f90((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),(uint8_t)A(5));break;
 case 0x5173a0u:FUN_005173a0((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4));break;
 case 0x4e9350u:FUN_004e9350((int32_t)A(1),(int32_t)A(2));break;
 case 0x475720u:FUN_00475720((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),A(5));break;
 case 0x475cc0u:FUN_00475cc0((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),A(5));break;
 case 0x4767d0u:FUN_004767d0((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),A(5),A(6));break;
 case 0x477500u:FUN_00477500((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4));break;
 case 0x4776f0u:FUN_004776f0((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4));break;
 case 0x4777c0u:FUN_004777c0((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4));break;
 case 0x477890u:FUN_00477890((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4));break;
 case 0x47a120u:FUN_0047a120((int32_t)A(1),(int32_t)A(2));break;
 case 0x47b310u:FUN_0047b310((int32_t)A(1),(int32_t)A(2),(int32_t)A(3));break;
 default:abort();
 }
 /* These external genuine providers are native RET0. Reviewed post-call
    volatile reads either reload/zero the whole register, or mask to bits
    already defined locally. The only partial-INC cases (46D43A/4C067C)
    store WORDs then reload before any high bits or arithmetic flags matter. */
 f->r[4]=sp+4u;
#undef A
}
void restore_god_building_raw(uint32_t target,uint32_t registers[8],uint32_t *flags)
{
 struct restore_god_building_frame f;
 memcpy(f.r,registers,sizeof(f.r));f.flags=*flags;f.boundary=dispatch;
 dispatch(0,target,&f);memcpy(registers,f.r,sizeof(f.r));*flags=f.flags;
}
static uint32_t invoke(uint32_t target,const uint32_t *args,uint32_t count)
{
 uint32_t stack[1024],i,entry;struct restore_god_building_frame f;
 for(i=0;i<1024;++i)stack[i]=SENTRY;
 for(i=0;i<8;++i)f.r[i]=0;
 /* Full owned raw graph floor is entry-448, dominated by native qsort.
    Accepted raw leaf/one-child queries are shallower; reserve3072 bytes
    below entry and never borrow the physical caller's scratch slots. */
 entry=(uint32_t)(uintptr_t)(stack+800);
 for(i=0;i<count;++i)stack[801+i]=args[i];
 f.r[4]=entry;f.flags=2;f.boundary=dispatch;dispatch(0,target,&f);
 if(f.r[4]!=entry+4u||stack[800]!=SENTRY||f.r[3]||f.r[5]||f.r[6]||f.r[7])abort();
 for(i=0;i<32;++i)if(stack[i]!=SENTRY)abort();
 for(i=801+count;i<1024;++i)if(stack[i]!=SENTRY)abort();
 return f.r[0];
}
uint32_t restore_god_building_004c3950(void){return invoke(0x4c3950u,0,0);}
void restore_god_building_004c3ac0(void){(void)invoke(0x4c3ac0u,0,0);}
uint32_t restore_god_building_004c3ed0(void){return invoke(0x4c3ed0u,0,0);}
void restore_god_building_004c4020(int32_t mode){uint32_t a[]={(uint32_t)mode};(void)invoke(0x4c4020u,a,1);}
uint32_t restore_god_building_004c41b0(void){return invoke(0x4c41b0u,0,0);}
void restore_god_building_004c42b0(void){(void)invoke(0x4c42b0u,0,0);}
void restore_god_building_004c45e0(void){(void)invoke(0x4c45e0u,0,0);}
int32_t restore_god_building_0045b2a0(int32_t p,int32_t b,int32_t r,int32_t n)
{return restore_storage_deliver(p,b,r,n);}
void restore_god_building_0045b700(int32_t b,int32_t n)
{restore_storage_remove_any(b,n);}
int32_t restore_god_building_0045fe20(int32_t b,int32_t n,int32_t r)
{uint32_t a[]={(uint32_t)b,(uint32_t)n,(uint32_t)r};return (int32_t)invoke(0x45fe20u,a,3);}
void restore_god_building_004695d0(int32_t b,int32_t destroy)
{restore_building_clear_linked_parts(b,destroy);}
void restore_god_building_0046d100(int32_t b,uint8_t mode)
{restore_building_destroy(b,mode);}
void restore_god_building_0043fd00(int32_t b,int32_t kind)
{restore_building_clear_farm_tiles(b,kind);}
void restore_god_building_004c04a0(int32_t b)
{restore_building_destroy_four_parts(b);}
uint32_t restore_god_building_0046d0f0(uint32_t b)
{return restore_building_destruction_root(b);}
void restore_god_building_004e9170(int32_t p,int32_t n)
{restore_population_remove_destroyed(p,n);}
void restore_god_building_004bfee0(int32_t b)
{restore_building_clear_four_part_occupancy(b);}
