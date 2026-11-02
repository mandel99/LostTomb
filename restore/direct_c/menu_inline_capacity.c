#include "menu_inline_capacity.h"
#include "menu_route_capacity.h"
#include "menu_full_input_routes.h"
#include "host_allocator.h"
#include <windows.h>
#include <stdio.h>
#define IC_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define IC_B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
typedef struct InlineRouteSite {uint32_t before,allocate,release,offset,need_register;} InlineRouteSite;
static const InlineRouteSite sites[]={
 {0x4da05a,0x4da08e,0x4da076,0x776fb0,5},
 {0x4da424,0x4da458,0x4da440,0x776fb0,5},
 {0x4da8d3,0x4da907,0x4da8ef,0x776fb0,5},
 {0x4dacb8,0x4dacec,0x4dacd4,0x776fb0,5},
 {0x4db446,0x4db474,0x4db45f,0,5},
 {0x4db6e6,0x4db714,0x4db6ff,0,5},
 {0x4db98b,0x4db9b9,0x4db9a4,0,3},
 {0x4dbc55,0x4dbc83,0x4dbc6e,0,5}
};
static __declspec(noreturn) void capacity_failure(const char *reason)
{fprintf(stderr,"menu route capacity: %s\n",reason);fflush(stderr);ExitProcess(0x4d83);}
static void *inline_allocate(uint32_t bytes)
{return restore_heap_allocate(bytes);}
static void inline_release(uint32_t address)
{restore_heap_free((void *)(uintptr_t)address);}
/* Compose the unchanged repaired route algorithms with the owned host pair;
   the legacy implicit-ECX exports and private original heap are not used. */
#define FUN_00561a1d inline_allocate
#define FUN_00561a2b inline_release
#ifndef MENU_ROUTE_SOURCE
#define MENU_ROUTE_SOURCE "../runtime/menu_full_input_routes.c"
#endif
#include MENU_ROUTE_SOURCE
#undef FUN_00561a1d
#undef FUN_00561a2b
static uint32_t required(const InlineRouteSite *site,const struct restore_menu_input_frame *f)
{
 uint32_t self=f->r[6]+site->offset,needed;
 if(!restore_menu_route_required_samples((const uint8_t *)(uintptr_t)(self+8u),IC_B(self+0x141u),IC_B(self),f->r[site->need_register],&needed))capacity_failure("invalid planner inputs");
 if(needed>UINT32_MAX/6u)capacity_failure("sample byte size overflow");
 return needed;
}
void restore_menu_inline_capacity_before(uint32_t instruction,struct restore_menu_input_frame *f)
{
 uint32_t i;
 for(i=0;i<sizeof(sites)/sizeof(sites[0]);++i)if(sites[i].before==instruction){
  const InlineRouteSite *s=sites+i;uint32_t self=f->r[6]+s->offset,need=f->r[s->need_register],old=IC_U(self+0x138u),want=required(s,f),address;
  int resize=(int32_t)need>(int32_t)old&&(int32_t)old<(int32_t)(need+50u);
  if(resize||want<=old)return;
  /* This extra branch exists only in the original underallocated domain.
     Allocate first so a host allocation failure never destroys old storage. */
  address=(uint32_t)(uintptr_t)inline_allocate(want*6u);
  if(!address)capacity_failure("allocation failed");
  if(IC_U(self+4u))inline_release(IC_U(self+4u));
  IC_U(self+4u)=address;IC_U(self+0x138u)=want;
  return;
 }
 capacity_failure("unknown capacity-load instruction");
}
int restore_menu_inline_capacity_boundary(uint32_t call,uint32_t target,struct restore_menu_input_frame *f)
{
 uint32_t i,sp=f->r[4],cleanup=0,handled=0;uint8_t low;
 if(target==0x561a1du||target==0x561a2bu){
  for(i=0;i<sizeof(sites)/sizeof(sites[0]);++i){
   const InlineRouteSite *s=sites+i;
   if(target==0x561a1du&&call==s->allocate){
    uint32_t want=required(s,f);
    if(want>f->r[7]){f->r[7]=want;IC_U(sp+4u)=want*6u;}
    f->r[0]=(uint32_t)(uintptr_t)inline_allocate(IC_U(sp+4u));
    if(!f->r[0])capacity_failure("allocation failed");
    handled=1;break;
   }
   if(target==0x561a2bu&&call==s->release){inline_release(IC_U(sp+4u));handled=1;break;}
  }
  if(!handled)capacity_failure("allocator call outside eight builders");
 }else switch(target){
 case 0x407f90:low=restore_menu_route_endpoints(f->r[1]);f->r[0]=(f->r[0]&0xffffff00u)|low;break;
 case 0x4dd8d0:f->r[0]=(uint32_t)restore_menu_route_last(f->r[1]);break;
 case 0x4dd8e0:f->r[0]=restore_menu_route_clear(f->r[1]);break;
 case 0x4dd900:restore_menu_route_mark(f->r[1],IC_B(sp+4u));cleanup=4;break;
 case 0x4dd910:f->r[0]=(uint32_t)restore_menu_route_start_city(f->r[1]);break;
 case 0x4dd920:f->r[0]=(uint32_t)restore_menu_route_end_city(f->r[1]);break;
 case 0x4dd930:low=restore_menu_route_delete(f->r[1],(int32_t)IC_U(sp+4u));f->r[0]=(f->r[0]&0xffffff00u)|low;cleanup=4;break;
 case 0x4ddbd0:f->r[0]=(uint32_t)restore_menu_route_hit(f->r[1],(int32_t)IC_U(sp+4u),(int32_t)IC_U(sp+8u),(uint8_t *)(uintptr_t)IC_U(sp+12u));cleanup=12;break;
 default:return 0;
 }
 f->r[4]=sp+4u+cleanup;return 1;
}
