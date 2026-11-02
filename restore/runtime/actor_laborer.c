#include "actor_laborer.h"
#include "game_diagnostics.h"
#include "actor_work.h"
#include "animation_state.h"
#include "actor_routing.h"
#include "building_services.h"
#include "actor_walking.h"
#include "actor_combat.h"
#include "population_updates.h"
#include "pyramid_summary.h"
#include "monument_base_queries.h"
#include "monument_complex_queries.h"
#include "monument_progress.h"
#include "menu_actor_message.h"
#include "menu_event_actions.h"
#include "sidebar_undo.h"
#include "placement_support.h"
#include "building_refresh_helpers.h"
#include "building_methods.h"
#include "building_create_model.h"
#include "menu_full_input_numeric.h"
#include "legacy_format.h"
#include "building_allocator.h"
#include "building_support.h"
#include "canal_region_model.h"
#include "map_commit_connect.h"
#include "map_commit_costs.h"
#include "actor_predicate.h"
#include "editor_brush_validation.h"
#include <windows.h>
#include <stdlib.h>
#include <string.h>

#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
extern void FUN_004b1d50(int32_t);
extern void FUN_004b4220(int32_t,int32_t,int32_t,int32_t,int32_t,int32_t);
extern uint8_t FUN_0051cd80(int32_t,int32_t,int32_t,int32_t);

static void dispatch(uint32_t site,uint32_t target,struct restore_laborer_frame *f)
{
    uint32_t sp=f->r[4],cleanup=0;
    if(target==0x4b52f0u||target==0x4b5420u)
        restore_game_actor_grid_event(site,(int32_t)U(sp+4u),target==0x4b52f0u);
    /* 4B23D4 caches OutputDebugStringA's IAT value in EDI. Its sole CALL EDI
       at4B242B precedes every later EDI definition; bind the authenticated slot. */
    if(site==0x4b242bu)target=0x56f0f8u;
#define A(n) U(sp+4u*(n))
#define RAW(a) case 0x##a##u:restore_laborer_C_##a(f);return
#define CHILD(type,callback) do{ \
    struct type child;memcpy(child.r,f->r,sizeof(child.r));child.flags=f->flags; \
    child.boundary=callback;callback(site,target,&child); \
    memcpy(f->r,child.r,sizeof(f->r));f->flags=child.flags;return; \
}while(0)
    switch(target){
    RAW(004b0760);
    RAW(004ab980);
    RAW(004af2c0);
    RAW(004a34f0);
    RAW(004b23c0);
    case 0x4bc2e0u:restore_laborer_tomb_task(f,0x4bc2e0u,2);return;
    case 0x4bc3b0u:restore_laborer_tomb_task(f,0x4bc3b0u,3);return;
    RAW(004a7ea0);
    RAW(004a8800);
    RAW(004bc480);
    RAW(004bc1e0);
    RAW(004eef10);
    RAW(004eeb60);
    RAW(004f0ef0);
    RAW(004f2410);
    RAW(004f2230);
    RAW(004f60d0);
    RAW(004f6130);
    RAW(00547e20);
    RAW(004f01c0);
    RAW(004f0190);
    RAW(004f5e70);
    RAW(004b5420);
    RAW(004b52f0);
    RAW(00545be0);
    RAW(005590c0);
    RAW(00556430);
    RAW(0054fb40);
    RAW(004efe30);
    RAW(004223f0);
    RAW(00545bc0);
    RAW(005590a0);
    RAW(00556410);
    RAW(0054fb20);
    RAW(00547870);
    RAW(004ea930);
    RAW(00545740);
    RAW(00558be0);
    RAW(00555e80);
    RAW(0054f470);
    RAW(00546790);
    case 0x4233f0u:restore_laborer_part_work(f,0x4233f0u,0x5be508u);return;
    case 0x423530u:restore_laborer_part_work(f,0x423530u,0x5be538u);return;
    case 0x423670u:restore_laborer_part_work(f,0x423670u,0x5be568u);return;
    case 0x4237b0u:restore_laborer_part_work(f,0x4237b0u,0x5be748u);return;
    case 0x4238f0u:restore_laborer_part_work(f,0x4238f0u,0x5be778u);return;
    case 0x423a30u:restore_laborer_part_work(f,0x423a30u,0x5be598u);return;
    case 0x423b70u:restore_laborer_part_work(f,0x423b70u,0x5be5c8u);return;
    case 0x423cb0u:restore_laborer_part_work(f,0x423cb0u,0x5be5f8u);return;
    case 0x423df0u:restore_laborer_part_work(f,0x423df0u,0x5be628u);return;
    case 0x423f30u:restore_laborer_part_work(f,0x423f30u,0x5be7a8u);return;
    case 0x424070u:restore_laborer_part_work(f,0x424070u,0x5be7d8u);return;
    case 0x4241b0u:restore_laborer_part_work(f,0x4241b0u,0x5be658u);return;
    case 0x4242f0u:restore_laborer_part_work(f,0x4242f0u,0x5be688u);return;
    case 0x424430u:restore_laborer_part_work(f,0x424430u,0x5be898u);return;
    case 0x424570u:restore_laborer_part_work(f,0x424570u,0x5be808u);return;
    case 0x4246b0u:restore_laborer_part_work(f,0x4246b0u,0x5be8c8u);return;
    case 0x4247f0u:restore_laborer_part_work(f,0x4247f0u,0x5be8f8u);return;
    case 0x424930u:restore_laborer_part_work(f,0x424930u,0x5be6b8u);return;
    case 0x424a70u:restore_laborer_part_work(f,0x424a70u,0x5be6e8u);return;
    case 0x424bb0u:restore_laborer_part_work(f,0x424bb0u,0x5be718u);return;
    case 0x424cf0u:restore_laborer_part_work(f,0x424cf0u,0x5be838u);return;
    case 0x424e30u:restore_laborer_part_work(f,0x424e30u,0x5be868u);return;
    case 0x424f70u:restore_laborer_part_work(f,0x424f70u,0x5be928u);return;
    case 0x4250b0u:restore_laborer_part_work(f,0x4250b0u,0x5be958u);return;
    case 0x4251f0u:restore_laborer_part_work(f,0x4251f0u,0x5be988u);return;
    RAW(004a98b0);
    RAW(004a7600);
    RAW(00401b40);
    RAW(00401be0);
    RAW(00401d30);
    RAW(00401d40);
    RAW(00401ed0);
    RAW(00401f30);
    RAW(00401f50);
    RAW(00417640);
    RAW(004178e0);
    RAW(00417940);
    RAW(00420a90);
    RAW(00420b10);
    RAW(00420dc0);
    RAW(00420e20);
    RAW(004210c0);
    RAW(00421120);
    RAW(004bc140);
    RAW(004e5ae0);
    RAW(004e5b20);
    RAW(004e5c00);
    RAW(004e5d10);
    RAW(004e6bd0);
    RAW(004e6e10);
    RAW(004e6e70);
    RAW(004ef200);
    RAW(004ef3a0);
    RAW(004f01e0);
    RAW(00420240);
    RAW(004207e0);
    RAW(00422a90);
    RAW(0041fd80);
    RAW(0041fca0);
    RAW(004200f0);
    RAW(00420770);
    RAW(00542dd0);
    RAW(004ee630);
    RAW(004ee5d0);
    case 0x4200b0u:FUN_004200b0();break;
    case 0x420130u:FUN_00420130(A(1),(int32_t)A(2),(uint8_t)A(3));break;
    case 0x420350u:f->r[0]=(uint32_t)FUN_00420350(A(1),A(2),(int32_t)A(3));break;
    case 0x420c40u:f->r[0]=(uint32_t)FUN_00420c40((int32_t)A(1),(int32_t)A(2));break;
    case 0x4e5a30u:f->r[0]=(uint32_t)FUN_004e5a30((int32_t)A(1));break;
    case 0x4e59c0u:f->r[0]=(uint32_t)FUN_004e59c0();break;
    RAW(0040f070);
    RAW(0040f0d0);
    RAW(004a35e0);
    RAW(004b1ef0);
    RAW(004b3ff0);
    RAW(004beeb0);
    RAW(004e0050);
    RAW(004e0c00);
    RAW(004f02f0);
    RAW(004f04d0);
    RAW(004f1cb0);
    RAW(004f1d10);
    RAW(004f2960);
    RAW(004f29e0);
    RAW(004f2ba0);
    RAW(004f2cf0);
    RAW(004f3000);
    RAW(004f3370);
    RAW(004f4320);
    RAW(004f4440);
    RAW(00541de0);
    RAW(00542a80);
    RAW(00547b60);
    RAW(004a9730);
    RAW(0040ecf0);
    RAW(0040eb90);
    RAW(0040ed70);
    RAW(005421d0);
    RAW(004176a0);
    RAW(004e6c10);
    RAW(004e05f0);
    RAW(00559110);
    RAW(004f3360);
    RAW(004f28b0);
    RAW(004f28e0);
    RAW(004f25c0);
    RAW(004f4420);
    RAW(004f3690);
    RAW(004f2190);
    RAW(004b4180);
    RAW(004b43a0);
    RAW(004b43e0);
    RAW(004f6570);
    RAW(00480180);
    RAW(004f27b0);
    RAW(004f3d70);
    RAW(00547c10);
    RAW(00420910);
    RAW(00420f50);
    RAW(00421230);
    RAW(00420430);
    RAW(00562069);
    RAW(005621bd);
    RAW(0056220b);
    RAW(004f2720);
    RAW(004f24b0);
    case 0x4a5da0u:case 0x4b2d20u:case 0x4b3c60u:case 0x519840u:
    case 0x45bb50u:case 0x47e340u:case 0x4b21d0u:case 0x4baa10u:
        restore_actor_routing_raw(target,f->r,&f->flags);return;
    case 0x459240u:case 0x4894a0u:case 0x4bf9d0u:
        CHILD(restore_actor_message_frame,restore_actor_message_dispatch);
    case 0x4eea10u:case 0x4ef100u:case 0x4ef450u:case 0x4efd90u:case 0x4f26a0u:
        CHILD(restore_pyramid_summary_frame,restore_pyramid_summary_dispatch);
    case 0x470990u:case 0x547ba0u:
    case 0x4017a0u:
    case 0x40dca0u:case 0x417180u:case 0x4e69c0u:case 0x5468d0u:case 0x541920u:
        CHILD(restore_menu_input_frame,restore_building_create_dispatch);
    case 0x45b2a0u:restore_sidebar_raw(target,f->r,&f->flags);return;
    case 0x4ecea0u:case 0x4ed360u:
    case 0x4e5840u:case 0x4edf50u:
    case 0x488030u:case 0x4dfbb0u:case 0x4ee4a0u:
        restore_building_refresh_raw(target,f->r,&f->flags);return;
    case 0x4b1df0u:case 0x562348u:
    case 0x47fe60u:case 0x4b3250u:case 0x4b4800u:
        restore_event_actions_raw(target,f->r,&f->flags);return;
    case 0x40ec20u:case 0x417590u:case 0x4e6b20u:
        f->r[0]=(f->r[0]&0xffffff00u)|restore_monument_base_worker(target,A(1));cleanup=4;break;
    case 0x5477e0u:
        f->r[0]=(f->r[0]&0xffffff00u)|restore_monument_worker_active(A(1));cleanup=4;break;
    case 0x4e0530u:f->r[0]=(f->r[0]&0xffffff00u)|FUN_004e0530(A(1));break;
    case 0x542110u:f->r[0]=(f->r[0]&0xffffff00u)|FUN_00542110(A(1));break;
    /* Whole native two-instruction metadata methods: MOV EAX,literal; RET.
       These are actual vtable38 values, not a guessed Boolean success. */
    case 0x545f60u:f->r[0]=9;break;
    case 0x559460u:f->r[0]=10;break;
    case 0x5567b0u:f->r[0]=21;break;
    case 0x54fed0u:f->r[0]=31;break;
    case 0x45b570u:f->r[0]=(uint32_t)FUN_0045b570((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4));break;
    case 0x485100u:f->r[0]=FUN_00485100((int32_t)A(1));break;
    case 0x51f780u:FUN_0051f780();break;
    case 0x46a400u:f->r[0]=(uint32_t)restore_building_allocate_0046a400(A(1),A(2),(int32_t)A(3),(int32_t)A(4),(int32_t)A(5),(int32_t)A(6));break;
    case 0x47bae0u:FUN_0047bae0((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),A(5));break;
    case 0x516f90u:FUN_00516f90((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),(uint8_t)A(5));break;
    case 0x5173a0u:FUN_005173a0((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4));break;
    case 0x4b17d0u:FUN_004b17d0((int32_t)A(1));break;
    case 0x4b1d50u:FUN_004b1d50((int32_t)A(1));break;
    case 0x4b3f00u:FUN_004b3f00((int32_t)A(1));break;
    case 0x4b4220u:FUN_004b4220((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),(int32_t)A(5),(int32_t)A(6));break;
    case 0x4bf920u:FUN_004bf920((int32_t)A(1));cleanup=4;break;
    case 0x51cd80u:
        f->r[0]=(f->r[0]&0xffffff00u)|FUN_0051cd80((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4));break;
    case 0x56133cu:{
        uint8_t raw80[10];uint64_t value;
        __asm { lea eax,raw80 }
        __asm { fstp tbyte ptr[eax] }
        value=restore_menu_input_trunc80(raw80);
        f->r[0]=(uint32_t)value;f->r[2]=(uint32_t)(value>>32);break;
    }
    case 0x561a36u:
        /* 4A9E51 has the verified literal with exactly two integer conversions.
           Parent ADD ESP18 also releases earlier delayed argument slots. */
        if(site!=0x4a9e51u)abort();
        f->r[0]=(uint32_t)FUN_00561a36((char *)(uintptr_t)A(1),(const char *)(uintptr_t)A(2),(int32_t)A(3),(int32_t)A(4));break;
    case 0x56f0f8u:OutputDebugStringA((const char *)(uintptr_t)A(1));cleanup=4;break;
    default:abort();
    }
    /* Actual member receivers stay in the shared frame; native RET4/RET12
       is applied exactly once. Cdecl continuations own their delayed cleanup.
       Only genuine query outputs consumed by these callers are published. */
    f->r[4]=sp+4u+cleanup;
#undef CHILD
#undef RAW
#undef A
}

void restore_actor_laborer_raw(uint32_t target,uint32_t registers[8],uint32_t *flags)
{
    struct restore_laborer_frame f;
    memcpy(f.r,registers,sizeof(f.r));f.flags=*flags;f.boundary=dispatch;
    dispatch(0,target,&f);
    memcpy(registers,f.r,sizeof(f.r));*flags=f.flags;
}

static uint32_t invoke(uint32_t entry,const uint32_t *args,uint32_t count)
{
    uint32_t stack[8192],i,sp=(uint32_t)(uintptr_t)(stack+8000);
    struct restore_laborer_frame f={{0},2,dispatch};
    /* No incoming receiver or FPU input for these cdecl logical entries.
       Native producers define local scratch; preserve natural storage. */
    stack[8000]=0;f.r[4]=sp;
    for(i=0;i<count;++i)stack[8001+i]=args[i];
    dispatch(0,entry,&f);
    if(f.r[4]!=sp+4u)abort();
    return f.r[0];
}
int32_t FUN_004efe30(uint32_t x,uint32_t y,int32_t actor,int32_t mode)
{const uint32_t a[]={x,y,(uint32_t)actor,(uint32_t)mode};return (int32_t)invoke(0x4efe30u,a,4);}
void FUN_004a98b0(void){(void)invoke(0x4a98b0u,0,0);}
void FUN_004a7600(void){(void)invoke(0x4a7600u,0,0);}
void FUN_004a7ea0(void){(void)invoke(0x4a7ea0u,0,0);}
void FUN_004a8800(void){(void)invoke(0x4a8800u,0,0);}
void FUN_004b0760(void){(void)invoke(0x4b0760u,0,0);}
void FUN_004ab980(void){(void)invoke(0x4ab980u,0,0);}
void FUN_004af2c0(void){(void)invoke(0x4af2c0u,0,0);}
int32_t FUN_004a34f0(int32_t actor,int32_t *length)
{const uint32_t a[]={(uint32_t)actor,(uint32_t)(uintptr_t)length};return (int32_t)invoke(0x4a34f0u,a,2);}
void FUN_004b23c0(int32_t actor,int32_t leader,uint32_t mode,int32_t distance)
{const uint32_t a[]={(uint32_t)actor,(uint32_t)leader,mode,(uint32_t)distance};(void)invoke(0x4b23c0u,a,4);}
int32_t FUN_004bc2e0(int32_t x,int32_t y,int32_t actor)
{const uint32_t a[]={(uint32_t)x,(uint32_t)y,(uint32_t)actor};return (int32_t)invoke(0x4bc2e0u,a,3);}
int32_t FUN_004bc3b0(int32_t x,int32_t y,int32_t actor)
{const uint32_t a[]={(uint32_t)x,(uint32_t)y,(uint32_t)actor};return (int32_t)invoke(0x4bc3b0u,a,3);}
int32_t FUN_004bc140(int32_t x,int32_t y,int32_t actor)
{const uint32_t a[]={(uint32_t)x,(uint32_t)y,(uint32_t)actor};return (int32_t)invoke(0x4bc140u,a,3);}
uint32_t FUN_004a9730(int32_t actor)
{const uint32_t a[]={(uint32_t)actor};return invoke(0x4a9730u,a,1);}

/* Public cdecl callers use the same complete bodies as the actor dispatcher.
   The progress-step result is always positive7 or4, including its EDX half. */
int64_t FUN_004e5ae0(int32_t building)
{const uint32_t a[]={(uint32_t)building};return (int64_t)invoke(0x4e5ae0u,a,1);}
uint8_t FUN_004e5b20(int32_t building)
{const uint32_t a[]={(uint32_t)building};return (uint8_t)invoke(0x4e5b20u,a,1);}
int32_t FUN_004e5c00(uint32_t unused1,uint32_t unused2,int32_t actor)
{const uint32_t a[]={unused1,unused2,(uint32_t)actor};return (int32_t)invoke(0x4e5c00u,a,3);}
void FUN_004e5d10(int32_t building,int32_t actor,uint8_t state)
{const uint32_t a[]={(uint32_t)building,(uint32_t)actor,state};(void)invoke(0x4e5d10u,a,3);}
void FUN_004ef200(int32_t building)
{const uint32_t a[]={(uint32_t)building};(void)invoke(0x4ef200u,a,1);}
void FUN_004ef3a0(int32_t building,int32_t actor)
{const uint32_t a[]={(uint32_t)building,(uint32_t)actor};(void)invoke(0x4ef3a0u,a,2);}
uint32_t FUN_004f01e0(uint32_t unused1,uint32_t unused2,uint32_t actor)
{const uint32_t a[]={unused1,unused2,actor};return invoke(0x4f01e0u,a,3);}
void FUN_00420a90(int32_t building,uint32_t descriptor)
{const uint32_t a[]={(uint32_t)building,descriptor};(void)invoke(0x420a90u,a,2);}
void FUN_00420b10(int32_t building,int32_t actor,uint8_t state)
{const uint32_t a[]={(uint32_t)building,(uint32_t)actor,state};(void)invoke(0x420b10u,a,3);}
void FUN_00420dc0(int32_t building)
{const uint32_t a[]={(uint32_t)building};(void)invoke(0x420dc0u,a,1);}
void FUN_00420e20(int32_t building,int32_t actor,uint8_t state)
{const uint32_t a[]={(uint32_t)building,(uint32_t)actor,state};(void)invoke(0x420e20u,a,3);}
void FUN_004210c0(int32_t building)
{const uint32_t a[]={(uint32_t)building};(void)invoke(0x4210c0u,a,1);}
void FUN_00421120(int32_t building,int32_t actor,uint8_t state)
{const uint32_t a[]={(uint32_t)building,(uint32_t)actor,state};(void)invoke(0x421120u,a,3);}

/* Reuse complete existing movement bodies and the shared event patrol body. */
void FUN_004b1df0(int32_t actor)
{const uint32_t a[]={(uint32_t)actor};(void)invoke(0x004b1df0u,a,1);}
void FUN_004b1ef0(int32_t actor,int32_t steps)
{const uint32_t a[]={(uint32_t)actor,(uint32_t)steps};(void)invoke(0x004b1ef0u,a,2);}
int32_t FUN_004b3ff0(int32_t actor,int32_t mode)
{const uint32_t a[]={(uint32_t)actor,(uint32_t)mode};return (int32_t)invoke(0x004b3ff0u,a,2);}
void FUN_004b4180(int32_t actor)
{const uint32_t a[]={(uint32_t)actor};(void)invoke(0x004b4180u,a,1);}
void FUN_004b43a0(int32_t actor)
{const uint32_t a[]={(uint32_t)actor};(void)invoke(0x004b43a0u,a,1);}
void FUN_004b43e0(int32_t actor)
{const uint32_t a[]={(uint32_t)actor};(void)invoke(0x004b43e0u,a,1);}
void FUN_004200f0(int32_t building)
{const uint32_t a[]={(uint32_t)building};(void)invoke(0x004200f0u,a,1);}
void FUN_00420240(int32_t building,int32_t actor,uint8_t state)
{const uint32_t a[]={(uint32_t)building,(uint32_t)actor,(uint32_t)state};(void)invoke(0x00420240u,a,3);}
void FUN_00420770(int32_t building)
{const uint32_t a[]={(uint32_t)building};(void)invoke(0x00420770u,a,1);}
void FUN_004207e0(int32_t building,int32_t actor,uint8_t state)
{const uint32_t a[]={(uint32_t)building,(uint32_t)actor,(uint32_t)state};(void)invoke(0x004207e0u,a,3);}
int32_t FUN_00422a90(int32_t building,int32_t actor)
{const uint32_t a[]={(uint32_t)building,(uint32_t)actor};return (int32_t)invoke(0x00422a90u,a,2);}
void FUN_00542dd0(int32_t building)
{const uint32_t a[]={(uint32_t)building};(void)invoke(0x00542dd0u,a,1);}
void FUN_004ee630(int32_t building)
{const uint32_t a[]={(uint32_t)building};(void)invoke(0x004ee630u,a,1);}
void FUN_004ee5d0(int32_t building)
{const uint32_t a[]={(uint32_t)building};(void)invoke(0x004ee5d0u,a,1);}
