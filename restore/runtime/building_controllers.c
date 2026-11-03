#include "building_controllers.h"
#include "house_evolution.h"
#include "house_levels.h"
#include "resource_availability.h"
#include "actor_create.h"
#include "actor_order_path.h"
#include "building_cleanup.h"
#include "building_allocator.h"
#include "actor_predicate.h"
#include "editor_brush_validation.h"
#include "construction_refresh.h"
#include "menu_list_scroll.h"
#include "road_place.h"
#include "city_prepare.h"
#include "order_input.h"
#include "simulation_tick.h"
#include "building_support.h"
#include "clear_footprint.h"
#include "surface_passes.h"
#include "map_commit_connect.h"
#include "map_commit_costs.h"
#include <stdlib.h>
#include <string.h>

extern uint32_t FUN_004ea9a0(int32_t kind);
extern int32_t FUN_004ea9c0(int32_t kind);
extern uint32_t FUN_0047d350(void);
extern int32_t FUN_00475b50(int32_t building);
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define SENTINEL UINT32_C(0x8e47b123)

static void boundary(uint32_t site,uint32_t target,struct restore_building_control_frame *f)
{
 uint32_t sp=f->r[4],a,b,c,d;(void)site;
 switch(target){
 case 0x004528b0u:restore_building_control_C_004528b0(f);return;
 case 0x00454b40u:restore_building_control_C_00454b40(f);return;
 case 0x0046b740u:restore_building_control_C_0046b740(f);return;
 case 0x004539a0u:restore_building_control_C_004539a0(f);return;
 case 0x00453af0u:restore_building_control_C_00453af0(f);return;
 case 0x00453b40u:restore_building_control_C_00453b40(f);return;
 case 0x0046b840u:restore_building_control_C_0046b840(f);return;
 case 0x0046b480u:restore_building_control_C_0046b480(f);return;
 case 0x0046b940u:restore_building_control_C_0046b940(f);return;
 case 0x0046b9e0u:restore_building_control_C_0046b9e0(f);return;
 case 0x0046bab0u:restore_building_control_C_0046bab0(f);return;
 case 0x0046bbc0u:restore_building_control_C_0046bbc0(f);return;
 case 0x0046bcb0u:restore_building_control_C_0046bcb0(f);return;
 case 0x0046c1c0u:restore_building_control_C_0046c1c0(f);return;
 case 0x0046cb30u:restore_building_control_C_0046cb30(f);return;
 case 0x0046ccf0u:restore_building_control_C_0046ccf0(f);return;
 case 0x0046bda0u:restore_building_control_C_0046bda0(f);return;
 case 0x0046be90u:restore_building_control_C_0046be90(f);return;
 case 0x0046c500u:restore_building_control_C_0046c500(f);return;
 case 0x00452980u:restore_building_control_C_00452980(f);return;
 case 0x00452a20u:restore_building_control_C_00452a20(f);return;
 case 0x00452b00u:restore_building_control_C_00452b00(f);return;
 case 0x00452bd0u:restore_building_control_C_00452bd0(f);return;
 case 0x00452ca0u:restore_building_control_C_00452ca0(f);return;
 case 0x00452d70u:restore_building_control_C_00452d70(f);return;
 case 0x00452e40u:restore_building_control_C_00452e40(f);return;
 case 0x00452f10u:restore_building_control_C_00452f10(f);return;
 case 0x00452fe0u:restore_building_control_C_00452fe0(f);return;
 case 0x004530b0u:restore_building_control_C_004530b0(f);return;
 case 0x004531c0u:restore_building_control_C_004531c0(f);return;
 case 0x00453280u:restore_building_control_C_00453280(f);return;
 case 0x00453340u:restore_building_control_C_00453340(f);return;
 case 0x00453400u:restore_building_control_C_00453400(f);return;
 case 0x00453500u:restore_building_control_C_00453500(f);return;
 case 0x004535c0u:restore_building_control_C_004535c0(f);return;
 case 0x00453680u:restore_building_control_C_00453680(f);return;
 case 0x00453740u:restore_building_control_C_00453740(f);return;
 case 0x00453840u:restore_building_control_C_00453840(f);return;
 case 0x00453900u:restore_building_control_C_00453900(f);return;
 case 0x4b7c80u:restore_building_control_C_004b7c80(f);return;
 case 0x454d70u:FUN_00454d70();goto done;
 case 0x476530u:FUN_00476530();goto done;
 case 0x517370u:FUN_00517370();goto done;
 case 0x517660u:FUN_00517660();goto done;
 case 0x4c0be0u:restore_building_control_C_004c0be0(f);return;
 case 0x4c0860u:restore_building_control_C_004c0860(f);return;
 case 0x4c53d0u:restore_building_control_C_004c53d0(f);return;
 case 0x4c52d0u:restore_building_control_C_004c52d0(f);return;
 case 0x5596d0u:FUN_005596d0();goto done;
 case 0x4b77d0u:FUN_004b77d0();goto done;
 case 0x47d350u:f->r[0]=FUN_0047d350();goto done;
 }
 a=U(sp+4u);
 switch(target){
 case 0x5013a0u:f->r[0]=FUN_005013a0(a);goto done;
 case 0x4ea9a0u:f->r[0]=FUN_004ea9a0((int32_t)a);goto done;
 case 0x4ea9c0u:f->r[0]=(uint32_t)FUN_004ea9c0((int32_t)a);goto done;
 case 0x40b5e0u:f->r[0]=FUN_0040b5e0((int32_t)a);goto done;
 case 0x485100u:f->r[0]=FUN_00485100((int32_t)a);goto done;
 case 0x475b50u:f->r[0]=(uint32_t)FUN_00475b50((int32_t)a);goto done;
 case 0x4c5120u:f->r[0]=FUN_004c5120((int32_t)a);goto done;
 }
 b=U(sp+8u);
 if(target==0x46cea0u){FUN_0046cea0((int32_t)a,(int16_t)b);goto done;}
 if(target==0x46f930u){f->r[0]=FUN_0046f930((int32_t)a,(int32_t)b);goto done;}
 c=U(sp+12u);
 switch(target){
 case 0x47c480u:FUN_0047c480((int32_t)a,(int32_t)b,(int32_t)c);goto done;
 case 0x40f8f0u:
  FUN_0040f8f0((uint32_t *)(uintptr_t)a,b,(uint8_t)c);
  /* Native REP fill leaves its replicated byte in EAX, length in EDX and
   * exhausted count in ECX. 4B7C80 can retain EAX's high bytes across a loop. */
  f->r[0]=(uint32_t)(uint8_t)c*UINT32_C(0x01010101);f->r[1]=0;f->r[2]=b;goto done;
 case 0x47afd0u:FUN_0047afd0((int32_t)a,(int32_t)b,(int32_t)c);goto done;
 case 0x4746d0u:FUN_004746d0((int32_t)a,(int32_t)b,(int32_t)c);goto done;
 }
 d=U(sp+16u);
 switch(target){
 case 0x516f90u:FUN_00516f90((int32_t)a,(int32_t)b,(int32_t)c,(int32_t)d,(uint8_t)U(sp+20u));break;
 case 0x5173a0u:FUN_005173a0((int32_t)a,(int32_t)b,(int32_t)c,(int32_t)d);break;
 case 0x518400u:f->r[0]=FUN_00518400((int32_t)a,(int32_t)b,(int32_t)c,(int32_t)d);break;
 case 0x4b4800u:f->r[0]=(uint32_t)FUN_004b4800((uint8_t)a,(int32_t)b,(int32_t)c,(int32_t)d,(uint8_t)U(sp+20u));break;
 case 0x46a400u:f->r[0]=(uint32_t)restore_building_allocate_0046a400(a,b,(int32_t)c,(int32_t)d,(int32_t)U(sp+20u),(int32_t)U(sp+24u));break;
 case 0x47bf50u:FUN_0047bf50((uint16_t)a,(int32_t)b,(int32_t)c,(int32_t)d,(int32_t)U(sp+20u),U(sp+24u),U(sp+28u));break;
 default:abort();
 }
done:f->r[4]=sp+4u;
}

void restore_building_control_raw(uint32_t target,uint32_t registers[8],uint32_t *flags)
{
 struct restore_building_control_frame f;
 memcpy(f.r,registers,sizeof(f.r));f.flags=*flags;f.boundary=boundary;
 boundary(0,target,&f);
 memcpy(registers,f.r,sizeof(f.r));*flags=f.flags;
}

static uint32_t execute(void (*body)(struct restore_building_control_frame *),const uint32_t *args,unsigned count)
{
 uint32_t stack[256];struct restore_building_control_frame f;unsigned i;
 for(i=0;i<256;++i)stack[i]=SENTINEL;for(i=0;i<8;++i)f.r[i]=0;
 for(i=0;i<count;++i)stack[129+i]=args[i];
 f.flags=2;f.boundary=boundary;f.r[4]=(uint32_t)(uintptr_t)(stack+128);body(&f);
 if(f.r[4]!=(uint32_t)(uintptr_t)(stack+129)||stack[128]!=SENTINEL||f.r[3]||f.r[5]||f.r[6]||f.r[7])abort();
 for(i=0;i<32;++i)if(stack[i]!=SENTINEL)abort();
 for(i=129+count;i<256;++i)if(stack[i]!=SENTINEL)abort();
 return f.r[0];
}

int32_t FUN_0046b100(int32_t building)
{uint32_t a[1]={(uint32_t)building};return (int32_t)execute(restore_building_control_C_0046b100,a,1);}
void FUN_004528b0(void)
{(void)execute(restore_building_control_C_004528b0,0,0);}
void FUN_00454b40(int32_t building)
{const uint32_t a[]={(uint32_t)building};(void)execute(restore_building_control_C_00454b40,a,1);}
void FUN_0046b740(int32_t building)
{const uint32_t a[]={(uint32_t)building};(void)execute(restore_building_control_C_0046b740,a,1);}
uint32_t FUN_004539a0(void)
{return restore_house_check_global_mode();}
uint32_t FUN_00453af0(int32_t building)
{return (uint32_t)restore_house_compare_desirability((uint32_t)building);}
uint32_t FUN_00453b40(int32_t building,int32_t level)
{return restore_house_check_requirements((uint32_t)building,(uint32_t)level);}
void FUN_00472790(int32_t count,int32_t group,int32_t x,int32_t y)
{uint32_t a[4]={(uint32_t)count,(uint32_t)group,(uint32_t)x,(uint32_t)y};(void)execute(restore_building_control_C_00472790,a,4);}
int32_t FUN_004b6e10(int32_t building)
{uint32_t a[1]={(uint32_t)building};return (int32_t)execute(restore_building_control_C_004b6e10,a,1);}
void FUN_004bfb30(int32_t building)
{uint32_t a[1]={(uint32_t)building};(void)execute(restore_building_control_C_004bfb30,a,1);}
void FUN_004c0d50(int32_t building,int32_t image_offset)
{uint32_t a[2]={(uint32_t)building,(uint32_t)image_offset};(void)execute(restore_building_control_C_004c0d50,a,2);}
void FUN_004c3420(int32_t building,int32_t x,int32_t y,uint32_t owner,int32_t kind,uint32_t initialize)
{uint32_t a[6]={(uint32_t)building,(uint32_t)x,(uint32_t)y,owner,(uint32_t)kind,initialize};(void)execute(restore_building_control_C_004c3420,a,6);}
void FUN_004c0be0(int32_t building)
{uint32_t a[1]={(uint32_t)building};(void)execute(restore_building_control_C_004c0be0,a,1);}
void FUN_004c0860(int32_t x,int32_t y,int32_t group,uint32_t image_offset)
{uint32_t a[4]={(uint32_t)x,(uint32_t)y,(uint32_t)group,image_offset};(void)execute(restore_building_control_C_004c0860,a,4);}
int32_t FUN_004c53d0(int32_t category)
{uint32_t a[1]={(uint32_t)category};return (int32_t)execute(restore_building_control_C_004c53d0,a,1);}
void FUN_004b7c80(void)
{(void)execute(restore_building_control_C_004b7c80,0,0);}

/* Typed access to genuine layout helpers while their separate model bodies
   remain pending. All callees and nested frame boundaries still execute. */
void restore_house_merge_neighbours(uint32_t building)
{const uint32_t a[]={building};(void)execute(restore_building_control_C_0046b840,a,1);}
uint32_t restore_house_can_expand(uint32_t building,uint32_t area)
{const uint32_t a[]={building,area};return execute(restore_building_control_C_0046b480,a,2);}
void restore_house_clear_expansion(uint32_t building,uint32_t area)
{const uint32_t a[]={building,area};(void)execute(restore_building_control_C_0046b940,a,2);}
uint32_t restore_house_prepare_expansion(uint32_t building,uint32_t area)
{const uint32_t a[]={building,area};return execute(restore_building_control_C_0046b9e0,a,2);}
void restore_house_expand(uint32_t building,uint32_t layout,uint32_t parcels)
{
 const uint32_t a[]={building,parcels};
 switch(layout){
 case 0x46bab0u:(void)execute(restore_building_control_C_0046bab0,a,2);break;
 case 0x46bbc0u:(void)execute(restore_building_control_C_0046bbc0,a,2);break;
 case 0x46bcb0u:(void)execute(restore_building_control_C_0046bcb0,a,2);break;
 default:abort();
 }
}
void restore_house_reduce(uint32_t building,uint32_t layout)
{
 const uint32_t a[]={building};
 switch(layout){
 case 0x46c1c0u:(void)execute(restore_building_control_C_0046c1c0,a,1);break;
 case 0x46cb30u:(void)execute(restore_building_control_C_0046cb30,a,1);break;
 case 0x46ccf0u:(void)execute(restore_building_control_C_0046ccf0,a,1);break;
 default:abort();
 }
}
