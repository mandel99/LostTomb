#include "migration_flow.h"
#include "population_demography.h"
#include "housing_capacity.h"
#include "warehouse_stock.h"
#include "city_supply.h"
#include "granary.h"
#include "city_updates.h"
#include "actor_civic.h"
#include "actor_predicate.h"
#include "actor_routing.h"
#include "actor_teardown_model.h"
#include "actor_order_flood.h"
#include "building_refresh_helpers.h"
#include "building_release_model.h"
#include "building_footprint.h"
#include "building_rect_commit.h"
#include "building_cleanup.h"
#include "building_controllers.h"
#include "building_create_model.h"
#include "building_support.h"
#include "camera_navigation.h"
#include "canal_region_model.h"
#include "clear_footprint.h"
#include "command_migration.h"
#include "displaced_population.h"
#include "event_publisher.h"
#include "god_building_effects.h"
#include "invasion_commands.h"
#include "map_commit_connect.h"
#include "map_commit_costs.h"
#include "map_commit_links.h"
#include "map_commit_population.h"
#include "map_refresh_postpass1.h"
#include "menu_building_kind.h"
#include "menu_full_input_numeric.h"
#include "menu_group_selection.h"
#include "menu_resource_neighbors.h"
#include "mission_load_closure.h"
#include "placement_support.h"
#include "population_updates.h"
#include "production_updates.h"
#include "order_input.h"
#include "sidebar_undo.h"
#include "surface_passes.h"
#include "terrain_redraw.h"
#include "actor_transport.h"
#include "actor_laborer.h"
#include "menu_building_overlay.h"
#include <windows.h>
#include <mmsystem.h>
#include <stdlib.h>
#include <string.h>
extern uint32_t FUN_004e0c50(int32_t,int32_t *,int32_t *);
extern uint32_t FUN_00542f40(int32_t,int32_t *,int32_t *);
extern uint32_t FUN_0047d350(void);
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
void restore_city_dispatch(uint32_t site,uint32_t target,struct restore_city_frame *f)
{
    uint32_t sp=f->r[4];
    /* Each indirect ESI call follows the native load from this timeGetTime
       IAT slot. Retain that authenticated binding across shared C frames. */
    if(site==0x46235du||site==0x46235fu||site==0x466fcau)target=0x56f22cu;
#define A(n) U(sp+4u*(n))
#define RAW(a) case 0x##a##u:restore_city_C_##a(f);return
    switch(target){
    RAW(00467220);
    RAW(00488400);
    RAW(00455bf0);
    RAW(00455c30);
    RAW(00462140);
    RAW(004612c0);
    RAW(00461490);
    RAW(004615d0);
    RAW(00461720);
    RAW(004bed70);
    RAW(0045bf00);
    RAW(0045bb30);
    RAW(0045ce90);
    RAW(004673c0);
    RAW(00455cc0);
    RAW(00467310);
    RAW(00460ea0);
    RAW(00460fb0);
    RAW(00461f90);
    RAW(00460f70);
    RAW(0046f140);
    RAW(0047c260);
    RAW(004b76b0);
    RAW(004b7400);
    RAW(004b70c0);
    RAW(00455e00);
    RAW(004561e0);
    RAW(00456070);
    RAW(004565f0);
    RAW(00462180);
    RAW(00456fc0);
    RAW(00455ca0);
    RAW(00489180);
    RAW(004b75c0);
    RAW(004c4160);
    RAW(004f68a0);
    RAW(004f68b0);
    RAW(004f6a10);
    case 0x56f22cu:f->r[0]=timeGetTime();break;
    case 0x45b880u:case 0x45d510u:case 0x45e360u:
        restore_actor_transport_raw(target,f->r,&f->flags);return;
    case 0x4620c0u:restore_invasion_raw(target,f->r,&f->flags);return;
    case 0x45b570u:case 0x4806c0u:case 0x4807d0u:case 0x519a30u:
        restore_actor_civic_raw(target,f->r,&f->flags);return;
    case 0x4b1df0u:case 0x4bc140u:case 0x4bc2e0u:case 0x4bc3b0u:case 0x4beeb0u:
        restore_actor_laborer_raw(target,f->r,&f->flags);return;
    case 0x47bf50u:case 0x488e60u:case 0x4890f0u:case 0x4894a0u:
        restore_building_refresh_raw(target,f->r,&f->flags);return;
    case 0x4883d0u:case 0x4cde80u:{
        struct restore_menu_overlay_frame child;memcpy(child.r,f->r,sizeof(child.r));
        child.flags=f->flags;child.boundary=restore_menu_overlay_dispatch;
        restore_menu_overlay_dispatch(site,target,&child);
        memcpy(f->r,child.r,sizeof(f->r));f->flags=child.flags;return;}
    case 0x480460u:f->r[0]=FUN_00480460((int32_t)A(1),(int32_t)A(2),A(3));break;
    case 0x4611d0u:FUN_004611d0((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),
        (uint8_t *)(uintptr_t)A(4),(uint8_t *)(uintptr_t)A(5),(uint32_t *)(uintptr_t)A(6));break;
    RAW(0040f2c0);RAW(0047fd30);
    case 0x459ef0u:f->r[0]=FUN_00459ef0((int32_t)A(1));break;
    RAW(004b7790);
    RAW(004b8130);
    RAW(004b7920);
    RAW(004b79f0);
    RAW(004b8080);
    RAW(004b8b20);
    RAW(004b8cf0);
    RAW(004b92e0);
    RAW(004b7c00);
    RAW(004b7c40);
    RAW(004b9f90);
    RAW(004ba040);
    RAW(004ba090);
    RAW(004ba180);
    RAW(004ba290);
    RAW(004ba3b0);
    RAW(004ba690);
    RAW(004ba7e0);
    RAW(0044bb90);
    RAW(004eb4c0);
    RAW(004eaa10);
    RAW(004eaa60);
    case 0x44bb40u:
        /* Original invalid-index arm dereferences address2. Keep all valid
         * native member paths; reject the proven null-record case safely. */
        if(A(1)!=140u&&((int32_t)A(1)<0||(int32_t)A(1)>=(int32_t)U(f->r[1]))){
            f->r[0]=0;f->r[1]=0;f->r[4]=sp+8u;return;
        }
        restore_city_C_0044bb40(f);return;
    case 0x4b77d0u:FUN_004b77d0();break;
    case 0x4b7b50u:FUN_004b7b50((int32_t)A(1),(int16_t)A(2));break;
    case 0x4b7c80u:case 0x4c52d0u:
        restore_building_control_raw(target,f->r,&f->flags);return;
    case 0x5181c0u:case 0x51a420u:case 0x51b040u:
        restore_actor_routing_raw(target,f->r,&f->flags);return;
    case 0x4c5120u:f->r[0]=FUN_004c5120((int32_t)A(1));break;
    case 0x40b5e0u:f->r[0]=FUN_0040b5e0((int32_t)A(1));break;
    case 0x47d350u:f->r[0]=FUN_0047d350();break;
    RAW(004677c0);RAW(00468170);RAW(004684e0);
    RAW(004c06e0);RAW(004e94a0);RAW(004e94d0);RAW(004e7e60);
    RAW(0045a140);RAW(0045a0a0);
    RAW(00468860);
    RAW(0045e040);
    RAW(0045aa30);
    RAW(004558b0);
    RAW(0045aaa0);
    RAW(0045d5d0);
    RAW(0045e6c0);
    RAW(00457a40);
    RAW(00457af0);
    RAW(00458440);
    RAW(00458540);
    RAW(00459070);
    RAW(00459100);
    RAW(00459350);
    RAW(004595a0);
    RAW(00459670);
    RAW(00455430);
    RAW(00454e20);
    RAW(00454fa0);
    RAW(004539e0);
    RAW(004525f0);
    case 0x4599a0u:FUN_004599a0();break;
    RAW(004550e0);
    RAW(00469b90);
    RAW(0047e290);
    RAW(0046a060);
    RAW(004689e0);
    RAW(0045aae0);
    RAW(0045d630);
    RAW(0045e6e0);
    RAW(00458270);
    RAW(004583f0);
    RAW(004e8610);
    RAW(00458950);
    RAW(00458b60);
    RAW(004e8f80);
    RAW(004e8650);
    RAW(004e8720);
    RAW(004e87a0);
    RAW(00458880);
    RAW(00459400);
    RAW(004816b0);
    RAW(0047fc40);
    RAW(00469730);
    RAW(00468b70);
    RAW(00458d00);
    RAW(00458d80);
    RAW(00458f00);
    RAW(00458e20);
    RAW(004e92c0);
    RAW(004e92f0);
    RAW(004e9460);
    RAW(004696b0);
    RAW(00468b20);
    RAW(004f30b0);
    RAW(00481060);
    RAW(00481210);
    RAW(00487d00);
    RAW(00517c20);
    case 0x40f8f0u:
    case 0x517a40u:
    case 0x47fe60u:
    case 0x459020u:
    case 0x545a30u:
    case 0x558f00u:
    case 0x556230u:
    case 0x54f8a0u:
    case 0x475b50u:
    case 0x4e2970u:
    case 0x4e0cf0u:
    case 0x4cde60u:
    case 0x4cde40u:
    case 0x480180u:
    case 0x48c0e0u:
    case 0x53b090u:
    case 0x4b4800u:
    case 0x562348u:
    case 0x4b3250u:
    case 0x4ea9a0u:
    case 0x4ea9c0u:
    case 0x4ec410u:
        restore_actor_civic_raw(target,f->r,&f->flags);return;
    case 0x43fd00u:
    case 0x4695d0u:
    case 0x46d100u:
    case 0x4bfee0u:
    case 0x4e9170u:
        restore_god_building_raw(target,f->r,&f->flags);return;
    case 0x4c03d0u:
        restore_invasion_raw(target,f->r,&f->flags);return;
    case 0x40d890u:
    case 0x401480u:
    case 0x416fb0u:
    case 0x4e6660u:
    case 0x545720u:
    case 0x558bc0u:
    case 0x555e60u:
    case 0x54f450u:
        restore_sidebar_raw(target,f->r,&f->flags);return;
    case 0x401af0u:
    case 0x40eb90u:
    case 0x417490u:
    case 0x4e6ae0u:
        {struct restore_menu_input_frame child;memcpy(child.r,f->r,sizeof(child.r));child.flags=f->flags;
         child.boundary=restore_building_create_dispatch;restore_building_create_dispatch(site,target,&child);
         memcpy(f->r,child.r,sizeof(f->r));f->flags=child.flags;return;}
    case 0x488030u:restore_building_refresh_raw(target,f->r,&f->flags);return;
    case 0x488050u:f->r[0]=restore_actor_building_gate((int32_t)A(1));f->r[4]=sp+8u;return;
    case 0x459240u:f->r[0]=FUN_00459240((int32_t)A(1));break;
    case 0x417ae0u:FUN_00417ae0((int32_t)A(1));break;
    case 0x430cd0u:FUN_00430cd0(A(1));break;
    case 0x43f060u:FUN_0043f060((int32_t)A(1),(uint8_t)A(2),(int32_t)A(3));break;
    case 0x469d50u:f->r[0]=FUN_00469d50();break;
    case 0x469e40u:FUN_00469e40((int32_t)A(1),(uint8_t)A(2));break;
    case 0x46ac10u:FUN_0046ac10((int32_t)A(1));break;
    case 0x46cf40u:FUN_0046cf40((int32_t)A(1));break;
    case 0x476530u:FUN_00476530();break;
    case 0x4767d0u:FUN_004767d0((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),A(5),A(6));break;
    case 0x477500u:FUN_00477500((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4));break;
    case 0x47b1a0u:FUN_0047b1a0();break;
    case 0x47bae0u:FUN_0047bae0((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),A(5));break;
    case 0x47c2b0u:FUN_0047c2b0((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),(int32_t)A(5));break;
    case 0x47c480u:FUN_0047c480((int32_t)A(1),(int32_t)A(2),(int32_t)A(3));break;
    case 0x4806a0u:f->r[0]=FUN_004806a0((int32_t)A(1),(int32_t)A(2));break;
    case 0x4802c0u:f->r[0]=FUN_004802c0((int32_t)A(1),(int32_t)A(2));break;
    case 0x481420u:f->r[0]=FUN_00481420((int32_t)A(1),(int32_t)A(2),(int32_t)A(3));break;
    case 0x486e70u:FUN_00486e70();break;
    case 0x4a36b0u:FUN_004a36b0((int32_t)A(1));break;
    case 0x4c5340u:f->r[0]=FUN_004c5340((int32_t)A(1));break;
    case 0x4e0c50u:f->r[0]=FUN_004e0c50((int32_t)A(1),(int32_t *)(uintptr_t)A(2),(int32_t *)(uintptr_t)A(3));break;
    case 0x4e2fb0u:restore_building_warning_004e2fb0((int32_t)A(1));break;
    case 0x4e90f0u:FUN_004e90f0((int32_t)A(1),(int32_t)A(2));break;
    case 0x516f90u:FUN_00516f90((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),(uint8_t)A(5));break;
    case 0x517370u:FUN_00517370();break;
    case 0x5173a0u:FUN_005173a0((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4));break;
    case 0x517660u:FUN_00517660();break;
    case 0x5178e0u:FUN_005178e0();break;
    case 0x51ce90u:f->r[0]=FUN_0051ce90((int32_t)A(1),(int32_t)A(2));break;
    case 0x542f40u:f->r[0]=FUN_00542f40((int32_t)A(1),(int32_t *)(uintptr_t)A(2),(int32_t *)(uintptr_t)A(3));break;
    case 0x56133cu:{
        uint8_t raw80[10];uint64_t value;
        __asm { lea eax,raw80 }
        __asm { fstp tbyte ptr[eax] }
        value=restore_menu_input_trunc80(raw80);f->r[0]=(uint32_t)value;f->r[2]=(uint32_t)(value>>32);break;
    }
    default:abort();
    }
    /* Logical C facades normalize argument cleanup. Only 488050 above is a
       native RET4 boundary; generated/member providers retain their real frame. */
    f->r[4]=sp+4u;
#undef A
#undef RAW
}
void restore_city_updates_raw(uint32_t target,uint32_t registers[8],uint32_t *flags)
{
    struct restore_city_frame f;
    memcpy(f.r,registers,sizeof(f.r));f.flags=*flags;f.boundary=restore_city_dispatch;
    restore_city_dispatch(0,target,&f);memcpy(registers,f.r,sizeof(f.r));*flags=f.flags;
}
static uint32_t invoke_member(uint32_t entry,uint32_t self,const uint32_t *args,uint32_t count,uint32_t cleanup)
{
    uint32_t stack[8192],sp=(uint32_t)(uintptr_t)(stack+8000),i;
    struct restore_city_frame f={{0},2,restore_city_dispatch};
    stack[8000]=0;f.r[4]=sp;f.r[1]=self;
    for(i=0;i<count;++i)stack[8001+i]=args[i];
    restore_city_dispatch(0,entry,&f);
    if(f.r[4]!=sp+4u+cleanup)abort();
    return f.r[0];
}
static uint32_t invoke(uint32_t entry,const uint32_t *args,uint32_t count)
{return invoke_member(entry,0,args,count,0);}
void FUN_00462180(void){(void)invoke(0x462180u,0,0);}
int32_t FUN_00456fc0(int32_t building,uint8_t goods,int32_t selected[4],uint8_t *routes_ready)
{const uint32_t a[]={(uint32_t)building,goods,(uint32_t)(uintptr_t)selected,(uint32_t)(uintptr_t)routes_ready};return (int32_t)invoke(0x456fc0u,a,4);}
void FUN_00468860(void)
{(void)invoke(0x00468860u,0,0);}
/* Canonical population entries reuse the existing complete city models.
   Keep the full EAX counts; the old void declarations lost these results. */
int32_t FUN_00458e20(int32_t player,int32_t requested)
{return restore_population_add_births(player,requested);}
int32_t FUN_00458f00(int32_t player,int32_t requested)
{return restore_population_remove_deaths(player,requested);}
void FUN_004e8650(void)
{restore_population_age_and_deaths();}
void FUN_004e8720(void)
{restore_population_births();}
void FUN_0045d630(int32_t player)
{restore_city_rebuild_food(player);}
int32_t FUN_004e92c0(int32_t player,int32_t group)
{return restore_population_age_group(player,group);}
void FUN_004e92f0(int32_t player,int32_t group,int32_t amount)
{restore_population_remove_age_group(player,group,amount);}
void FUN_0045e040(void)
{restore_granary_rebuild_supply();}
void FUN_0045aa30(void)
{restore_building_rebuild_bounds();}
void FUN_004558b0(void)
{restore_building_decay_timers();}
void FUN_0045aaa0(void)
{restore_warehouse_update_stock();}
void FUN_0045d5d0(void)
{restore_city_update_food();}
void FUN_0045e6c0(void)
{restore_city_update_industry();}
void FUN_00457a40(void)
{(void)invoke(0x00457a40u,0,0);}
void FUN_00457af0(void)
{(void)invoke(0x00457af0u,0,0);}
void FUN_00458440(void)
{restore_housing_rebuild_capacity();}
void FUN_00458540(void)
{restore_migration_update();}
void FUN_00459070(void)
{(void)invoke(0x00459070u,0,0);}
void FUN_00459100(void)
{(void)invoke(0x00459100u,0,0);}
void FUN_00459350(void)
{(void)invoke(0x00459350u,0,0);}
void FUN_004595a0(void)
{(void)invoke(0x004595a0u,0,0);}
void FUN_00459670(void)
{(void)invoke(0x00459670u,0,0);}
void FUN_00455430(void)
{(void)invoke(0x00455430u,0,0);}
void FUN_00454e20(void)
{(void)invoke(0x00454e20u,0,0);}
void FUN_00454fa0(void)
{(void)invoke(0x00454fa0u,0,0);}
void FUN_004539e0(void)
{(void)invoke(0x004539e0u,0,0);}
void FUN_004525f0(void)
{(void)invoke(0x004525f0u,0,0);}
void FUN_004550e0(void)
{(void)invoke(0x004550e0u,0,0);}
void FUN_00469b90(void)
{(void)invoke(0x00469b90u,0,0);}
void FUN_0047e290(void)
{(void)invoke(0x0047e290u,0,0);}
void FUN_0046a060(void)
{(void)invoke(0x0046a060u,0,0);}
void FUN_00469730(void)
{(void)invoke(0x00469730u,0,0);}
void FUN_00468b70(void)
{(void)invoke(0x00468b70u,0,0);}
uint8_t FUN_00517c20(int32_t x,int32_t y,int32_t tx,int32_t ty)
{
    const uint32_t args[]={(uint32_t)x,(uint32_t)y,(uint32_t)tx,(uint32_t)ty};
    return (uint8_t)invoke(0x00517c20u,args,4);
}

void FUN_004677c0(void){(void)invoke(0x004677c0u,0,0);}
void FUN_00468170(void){(void)invoke(0x00468170u,0,0);}
void FUN_004684e0(void){(void)invoke(0x004684e0u,0,0);}
uint32_t FUN_004c06e0(uint32_t category,int32_t tier)
{
    const uint32_t args[]={category,(uint32_t)tier};
    return invoke(0x004c06e0u,args,2);
}
int32_t FUN_004e94a0(int32_t player)
{
    const uint32_t args[]={(uint32_t)player};
    return (int32_t)invoke(0x004e94a0u,args,1);
}
int32_t FUN_004e94d0(int32_t player)
{
    const uint32_t args[]={(uint32_t)player};
    return (int32_t)invoke(0x004e94d0u,args,1);
}
void FUN_004e7e60(uint32_t *a,uint32_t *b,uint32_t *c,uint32_t *d)
{
    const uint32_t args[]={(uint32_t)(uintptr_t)a,(uint32_t)(uintptr_t)b,
                           (uint32_t)(uintptr_t)c,(uint32_t)(uintptr_t)d};
    (void)invoke(0x004e7e60u,args,4);
}
uint32_t FUN_0045a140(int32_t building)
{
    const uint32_t args[]={(uint32_t)building};
    return invoke(0x0045a140u,args,1);
}
uint32_t FUN_0045a0a0(int32_t amount,uint32_t tier,const int32_t *table)
{
    const uint32_t args[]={(uint32_t)amount,tier,(uint32_t)(uintptr_t)table};
    return invoke(0x0045a0a0u,args,3);
}


void FUN_004b7790(int32_t mode)
{const uint32_t a[]={(uint32_t)mode};(void)invoke(0x004b7790u,a,1);}
void FUN_004b8130(void){(void)invoke(0x004b8130u,0,0);}
void FUN_004b7920(void){(void)invoke(0x004b7920u,0,0);}
void FUN_004b79f0(void){(void)invoke(0x004b79f0u,0,0);}
void FUN_004b8080(void){(void)invoke(0x004b8080u,0,0);}
void FUN_004b8b20(void){(void)invoke(0x004b8b20u,0,0);}
void FUN_004b8cf0(void){(void)invoke(0x004b8cf0u,0,0);}
void FUN_004b92e0(void){(void)invoke(0x004b92e0u,0,0);}
void FUN_004b7c00(int32_t delta)
{const uint32_t a[]={(uint32_t)delta};(void)invoke(0x004b7c00u,a,1);}
void FUN_004b7c40(int32_t delta)
{const uint32_t a[]={(uint32_t)delta};(void)invoke(0x004b7c40u,a,1);}
void FUN_004b9f90(void){(void)invoke(0x004b9f90u,0,0);}
void FUN_004ba040(int32_t group)
{const uint32_t a[]={(uint32_t)group};(void)invoke(0x004ba040u,a,1);}
void FUN_004ba090(void){(void)invoke(0x004ba090u,0,0);}
void FUN_004ba180(int32_t x,int32_t y,int32_t radius,int8_t amount)
{const uint32_t a[]={(uint32_t)x,(uint32_t)y,(uint32_t)radius,(uint32_t)(int32_t)amount};(void)invoke(0x004ba180u,a,4);}
uint32_t FUN_004ba290(int32_t x,int32_t y,int32_t radius)
{const uint32_t a[]={(uint32_t)x,(uint32_t)y,(uint32_t)radius};return invoke(0x004ba290u,a,3);}
uint32_t FUN_004ba3b0(int32_t group)
{const uint32_t a[]={(uint32_t)group};return invoke(0x004ba3b0u,a,1);}
void FUN_004ba690(int32_t group)
{const uint32_t a[]={(uint32_t)group};(void)invoke(0x004ba690u,a,1);}
uint32_t FUN_004ba7e0(int32_t group,int32_t x,int32_t y)
{const uint32_t a[]={(uint32_t)group,(uint32_t)x,(uint32_t)y};return invoke(0x004ba7e0u,a,3);}
uint32_t FUN_004eb4c0(int32_t x,int32_t y,int32_t radius,int32_t *out_x,int32_t *out_y,uint32_t mode)
{const uint32_t a[]={(uint32_t)x,(uint32_t)y,(uint32_t)radius,(uint32_t)(uintptr_t)out_x,(uint32_t)(uintptr_t)out_y,mode};return invoke(0x004eb4c0u,a,6);}
int32_t FUN_004eaa10(int32_t group)
{const uint32_t a[]={(uint32_t)group};return (int32_t)invoke(0x004eaa10u,a,1);}
int32_t FUN_004eaa60(int32_t group,uint32_t action)
{const uint32_t a[]={(uint32_t)group,action};return (int32_t)invoke(0x004eaa60u,a,2);}
uint32_t FUN_0040f2c0(int32_t image,int32_t tile)
{const uint32_t a[]={(uint32_t)image,(uint32_t)tile};return invoke(0x0040f2c0u,a,2);}
uint32_t FUN_0047fd30(int32_t building,int32_t radius)
{const uint32_t a[]={(uint32_t)building,(uint32_t)radius};return invoke(0x0047fd30u,a,2);}

/* Canonical production helpers share the complete dispatcher bodies.
 * Public C members take an explicit receiver; native RET4 stays in the frame. */
void FUN_00467220(int32_t building)
{const uint32_t a[]={(uint32_t)building};(void)invoke(0x00467220u,a,1);}
uint32_t FUN_00488400(void * self,int32_t building)
{const uint32_t a[]={(uint32_t)building};return (uint32_t)invoke_member(0x00488400u,(uint32_t)(uintptr_t)self,a,1,4);}
uint8_t FUN_00455bf0(int32_t building)
{const uint32_t a[]={(uint32_t)building};return (uint8_t)invoke(0x00455bf0u,a,1);}
void FUN_00455c30(int32_t building)
{const uint32_t a[]={(uint32_t)building};(void)invoke(0x00455c30u,a,1);}
int32_t FUN_00462140(int32_t building,uint32_t kind)
{const uint32_t a[]={(uint32_t)building,(uint32_t)kind};return (int32_t)invoke(0x00462140u,a,2);}
void FUN_004612c0(int32_t building)
{const uint32_t a[]={(uint32_t)building};(void)invoke(0x004612c0u,a,1);}
void FUN_00461490(int32_t building)
{const uint32_t a[]={(uint32_t)building};(void)invoke(0x00461490u,a,1);}
void FUN_004615d0(int32_t building)
{const uint32_t a[]={(uint32_t)building};(void)invoke(0x004615d0u,a,1);}
void FUN_00461720(int32_t building)
{const uint32_t a[]={(uint32_t)building};(void)invoke(0x00461720u,a,1);}
void FUN_004bed70(int32_t building)
{const uint32_t a[]={(uint32_t)building};(void)invoke(0x004bed70u,a,1);}
int32_t FUN_0045bf00(int32_t building,int32_t resource)
{const uint32_t a[]={(uint32_t)building,(uint32_t)resource};return (int32_t)invoke(0x0045bf00u,a,2);}
uint8_t FUN_0045bb30(int32_t first,int32_t second)
{const uint32_t a[]={(uint32_t)first,(uint32_t)second};return (uint8_t)invoke(0x0045bb30u,a,2);}
int32_t FUN_0045ce90(int32_t building,int32_t resource)
{const uint32_t a[]={(uint32_t)building,(uint32_t)resource};return (int32_t)invoke(0x0045ce90u,a,2);}
uint32_t FUN_004673c0(int32_t building)
{const uint32_t a[]={(uint32_t)building};return (uint32_t)invoke(0x004673c0u,a,1);}
void FUN_00455cc0(int32_t kind,int32_t amount)
{const uint32_t a[]={(uint32_t)kind,(uint32_t)amount};(void)invoke(0x00455cc0u,a,2);}
void FUN_00467310(int32_t building)
{const uint32_t a[]={(uint32_t)building};(void)invoke(0x00467310u,a,1);}
int32_t FUN_00460ea0(uint32_t kind)
{const uint32_t a[]={(uint32_t)kind};return (int32_t)invoke(0x00460ea0u,a,1);}
uint8_t FUN_00460fb0(int32_t * kind)
{const uint32_t a[]={(uint32_t)(uintptr_t)kind};return (uint8_t)invoke(0x00460fb0u,a,1);}
void FUN_00461f90(int32_t actor)
{const uint32_t a[]={(uint32_t)actor};(void)invoke(0x00461f90u,a,1);}
uint8_t FUN_00460f70(int32_t building,int32_t * actor)
{const uint32_t a[]={(uint32_t)building,(uint32_t)(uintptr_t)actor};return (uint8_t)invoke(0x00460f70u,a,2);}
int32_t FUN_0046f140(void)
{return (int32_t)invoke(0x0046f140u,0,0);}
void FUN_0047c260(int32_t building)
{const uint32_t a[]={(uint32_t)building};(void)invoke(0x0047c260u,a,1);}
void FUN_004b76b0(int32_t building)
{const uint32_t a[]={(uint32_t)building};(void)invoke(0x004b76b0u,a,1);}
uint32_t FUN_004b7400(int32_t building)
{const uint32_t a[]={(uint32_t)building};return (uint32_t)invoke(0x004b7400u,a,1);}
uint32_t FUN_004b70c0(int32_t building)
{const uint32_t a[]={(uint32_t)building};return (uint32_t)invoke(0x004b70c0u,a,1);}
void FUN_00455e00(int32_t building,int32_t resource,int32_t * selected)
{const uint32_t a[]={(uint32_t)building,(uint32_t)resource,(uint32_t)(uintptr_t)selected};(void)invoke(0x00455e00u,a,3);}
uint32_t FUN_004561e0(int32_t building,const int32_t * stock,const int32_t * sources)
{const uint32_t a[]={(uint32_t)building,(uint32_t)(uintptr_t)stock,(uint32_t)(uintptr_t)sources};return (uint32_t)invoke(0x004561e0u,a,3);}
void FUN_00456070(int32_t building,int32_t resource,int32_t * selected)
{const uint32_t a[]={(uint32_t)building,(uint32_t)resource,(uint32_t)(uintptr_t)selected};(void)invoke(0x00456070u,a,3);}
uint32_t FUN_004565f0(int32_t building,const int32_t * stock,const int32_t * sources)
{const uint32_t a[]={(uint32_t)building,(uint32_t)(uintptr_t)stock,(uint32_t)(uintptr_t)sources};return (uint32_t)invoke(0x004565f0u,a,3);}
uint32_t FUN_00455ca0(int32_t key,uint32_t unused,const int32_t * selected)
{const uint32_t a[]={(uint32_t)key,(uint32_t)unused,(uint32_t)(uintptr_t)selected};return (uint32_t)invoke(0x00455ca0u,a,3);}
void FUN_00489180(int32_t building,int32_t amount)
{const uint32_t a[]={(uint32_t)building,(uint32_t)amount};(void)invoke(0x00489180u,a,2);}
int32_t FUN_004b75c0(int32_t formation)
{const uint32_t a[]={(uint32_t)formation};return (int32_t)invoke(0x004b75c0u,a,1);}
uint8_t FUN_004c4160(int32_t resource)
{const uint32_t a[]={(uint32_t)resource};return (uint8_t)invoke(0x004c4160u,a,1);}

