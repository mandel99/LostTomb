#include "../artifacts/actor_civic_followup/actor_civic_followup.h"
#include "storage_transactions.h"
#include "actor_walking.h"
/* Native BAA10 ignores the extra literal-zero caller slot. */
void restore_actor_walking_service(int32_t actor,int32_t unused)
{
    (void)unused;
    (void)FUN_004baa10(actor);
}
/* The shared native callback is one RET and consumes no actor argument. */
void restore_actor_walking_empty(int32_t actor)
{
    (void)actor;
    FUN_0051f780();
}

#include "actor_routing.h"
#include "building_services.h"
#include "actor_predicate.h"
#include "building_controllers.h"
#include "population_updates.h"
#include "menu_prepare_integer.h"
#include "actor_order_path.h"
#include "actor_order_flood.h"
#include <stdlib.h>
extern void FUN_00469140(int32_t,int32_t,int32_t);
#define ROUTE_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))

static void routing_dispatch(uint32_t site,uint32_t target,struct restore_routing_frame *f)
{
    uint32_t sp=f->r[4],cleanup=0;(void)site;
#define A(n) ROUTE_U(sp+4u*(n))
#define RAW(a) case 0x##a##u:restore_routing_C_##a(f);return
    switch(target){
    case 0x4a5da0u:FUN_004a5da0((int32_t)A(1),A(2));break;
    case 0x4b27e0u:FUN_004b27e0((int32_t)A(1),(int32_t)A(2));break;
    case 0x489970u:
        f->r[0]=(f->r[0]&0xffffff00u)|FUN_00489970((int32_t)A(1),(int32_t *)(uintptr_t)A(2));break;
    case 0x4b2d20u:FUN_004b2d20((int32_t)A(1));break;
    RAW(0051d160);
    case 0x51b400u:f->r[0]=(uint32_t)FUN_0051b400((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),(int32_t)A(5),(int32_t)A(6));break;
    RAW(0051d980);
    RAW(0051dc10);
#define QUERY(a) case 0x##a##u:f->r[0]=FUN_##a((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4));break
    QUERY(00517f20);QUERY(005181c0);QUERY(005186d0);QUERY(00518880);
    QUERY(00519390);QUERY(005195e0);QUERY(00519840);QUERY(00519be0);
    QUERY(0051a030);QUERY(0051b040);
#undef QUERY
    RAW(0051d420);
    case 0x51a420u:f->r[0]=FUN_0051a420((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),(int32_t)A(5),(int32_t)A(6));break;
    case 0x51afa0u:f->r[0]=FUN_0051afa0((int32_t)A(1));break;
    case 0x4b33d0u:FUN_004b33d0((int32_t)A(1));break;
    case 0x4b3530u:FUN_004b3530((int32_t)A(1));break;
    case 0x4b3c60u:FUN_004b3c60((int32_t)A(1));break;
    case 0x4b3470u:f->r[0]=FUN_004b3470((int32_t)A(1),(int32_t)A(2));break;
    RAW(004690f0);
    case 0x40f8f0u:FUN_0040f8f0((uint32_t *)(uintptr_t)A(1),A(2),(uint8_t)A(3));break;
    case 0x484990u:f->r[0]=(uint32_t)FUN_00484990((int32_t)A(1),(int32_t)A(2));break;
    case 0x484a90u:f->r[0]=(uint32_t)FUN_00484a90((int32_t)A(1),(int32_t)A(2));break;
    case 0x483910u:f->r[0]=(uint32_t)FUN_00483910((int32_t)A(1),(int32_t)A(2),(int32_t)A(3));break;
    case 0x483e90u:f->r[0]=(uint32_t)FUN_00483e90((int32_t)A(1),(int32_t)A(2));break;
    case 0x484450u:f->r[0]=(uint32_t)FUN_00484450((int32_t)A(1),(int32_t)A(2));break;
    case 0x482ef0u:FUN_00482ef0((int32_t)A(1),(int32_t)A(2),(int32_t)A(3));break;
    case 0x484120u:f->r[0]=(uint32_t)FUN_00484120((int32_t)A(1),(int32_t)A(2));break;
    case 0x484230u:f->r[0]=(uint32_t)FUN_00484230((int32_t)A(1),(int32_t)A(2));break;
    case 0x484340u:f->r[0]=(uint32_t)FUN_00484340((int32_t)A(1),(int32_t)A(2));break;
    case 0x484560u:f->r[0]=(uint32_t)FUN_00484560((int32_t)A(1),(int32_t)A(2));break;
    case 0x482dd0u:f->r[0]=FUN_00482dd0((int32_t)A(1),(int32_t)A(2),(int32_t)A(3));break;
    case 0x42ff20u:FUN_0042ff20((int32_t)A(1),(int32_t)A(2));break;
    case 0x484780u:f->r[0]=(uint32_t)FUN_00484780((int32_t)A(1),(int32_t)A(2));break;
    case 0x484890u:f->r[0]=FUN_00484890((int32_t)A(1),(int32_t)A(2));break;
    case 0x483fb0u:f->r[0]=(uint32_t)FUN_00483fb0((int32_t)A(1),(int32_t)A(2),(int32_t)A(3));break;
    case 0x483160u:f->r[0]=FUN_00483160((int32_t)A(1),(int32_t)A(2));break;
    case 0x483260u:FUN_00483260((int32_t)A(1),(int32_t)A(2),(int32_t)A(3));break;
    case 0x482c10u:f->r[0]=(uint32_t)FUN_00482c10((int32_t)A(1),(int32_t)A(2));break;
    case 0x483000u:f->r[0]=(uint32_t)FUN_00483000((int32_t)A(1),(int32_t)A(2));break;
    case 0x484670u:FUN_00484670((int32_t)A(1),(int32_t)A(2));break;
    case 0x4833c0u:f->r[0]=(uint32_t)FUN_004833c0((int32_t)A(1),(int32_t)A(2));break;
    case 0x4834d0u:f->r[0]=(uint32_t)FUN_004834d0((int32_t)A(1),(int32_t)A(2));break;
    case 0x4835e0u:f->r[0]=(uint32_t)FUN_004835e0((int32_t)A(1),(int32_t)A(2));break;
    case 0x4836f0u:f->r[0]=(uint32_t)FUN_004836f0((int32_t)A(1),(int32_t)A(2));break;
    case 0x483800u:f->r[0]=(uint32_t)FUN_00483800((int32_t)A(1),(int32_t)A(2));break;
    case 0x47e250u:FUN_0047e250(A(1),(int32_t)A(2));break;
    case 0x4baa10u:f->r[0]=FUN_004baa10((int32_t)A(1));break;
    RAW(004b21d0);
    RAW(0045bb50);
    RAW(0047e340);
    RAW(0045dc50);
    case 0x47e230u:f->r[0]=FUN_0047e230(A(1),(int32_t)A(2));break;
    case 0x4b3f00u:FUN_004b3f00((int32_t)A(1));break;
    case 0x51ce90u:f->r[0]=FUN_0051ce90((int32_t)A(1),(int32_t)A(2));break;
    case 0x518400u:f->r[0]=FUN_00518400((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4));break;
    case 0x51aff0u:f->r[0]=FUN_0051aff0((int32_t)A(1));break;
    case 0x51cba0u:f->r[0]=FUN_0051cba0((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4));break;
    case 0x4b3250u:FUN_004b3250((int32_t)A(1));break;
    case 0x4ad880u:f->r[0]=FUN_004ad880((int32_t *)(uintptr_t)A(1),(int32_t *)(uintptr_t)A(2));break;
    case 0x4b5420u:FUN_004b5420((int32_t)A(1));break;
    case 0x4b52f0u:FUN_004b52f0((int32_t)A(1));break;
    case 0x4f6650u:f->r[0]=restore_actor_walking_queue_full((const void *)(uintptr_t)f->r[1]);break;
    case 0x4f65b0u:
        f->r[0]=restore_actor_walking_queue_push((void *)(uintptr_t)f->r[1],(const int32_t *)(uintptr_t)A(1));cleanup=4;break;
    case 0x4b1210u:FUN_004b1210((int32_t)A(1),(int32_t)A(2));break;
    case 0x469140u:FUN_00469140((int32_t)A(1),(int32_t)A(2),(int32_t)A(3));break;
    case 0x4c5120u:f->r[0]=FUN_004c5120((int32_t)A(1));break;
    case 0x4c52d0u:restore_building_control_raw(target,f->r,&f->flags);return;
    case 0x42fb30u:FUN_0042fb30((int32_t)A(1));break;
    case 0x475b50u:f->r[0]=FUN_00475b50(A(1));break;
    default:abort();
    }
    /* Real members retain their explicit queue receiver and RET4 cleanup.
       Other boundaries are cdecl. Native continuations reload consumed
       volatile registers/flags; query results use the actual full EAX. */
    f->r[4]=sp+4u+cleanup;
#undef RAW
#undef A
}
void restore_actor_routing_raw(uint32_t target,uint32_t registers[8],uint32_t *flags)
{
    struct restore_routing_frame f;uint32_t i;
    for(i=0;i<8;++i)f.r[i]=registers[i];f.flags=*flags;f.boundary=routing_dispatch;
    routing_dispatch(0,target,&f);
    for(i=0;i<8;++i)registers[i]=f.r[i];*flags=f.flags;
}
static uint32_t routing_invoke(uint32_t entry,const uint32_t *args,uint32_t count)
{
    uint32_t stack[4096],i,sp=(uint32_t)(uintptr_t)(stack+4000);
    struct restore_routing_frame f={{0},2,routing_dispatch};
    /* No incoming receiver/x87 value or caller-owned extra slots. Original
       producers initialize locals before use; retain natural scratch. */
    stack[4000]=0;f.r[4]=sp;
    for(i=0;i<count;++i)stack[4001+i]=args[i];
    routing_dispatch(0,entry,&f);
    if(f.r[4]!=sp+4u)abort();
    return f.r[0];
}
uint32_t FUN_0051d160(int32_t x,int32_t y)
{const uint32_t a[]={(uint32_t)x,(uint32_t)y};return (uint32_t)routing_invoke(0x51d160u,a,2);}
int32_t FUN_0051d980(int32_t param_1,int32_t param_2,int32_t param_3,int32_t param_4,int32_t param_5,int32_t param_6)
{const uint32_t a[]={(uint32_t)param_1,(uint32_t)param_2,(uint32_t)param_3,(uint32_t)param_4,(uint32_t)param_5,(uint32_t)param_6};return (int32_t)routing_invoke(0x51d980u,a,6);}
int32_t FUN_0051dc10(int32_t param_1,int32_t param_2,int32_t param_3,int32_t param_4,int32_t param_5,int32_t param_6)
{const uint32_t a[]={(uint32_t)param_1,(uint32_t)param_2,(uint32_t)param_3,(uint32_t)param_4,(uint32_t)param_5,(uint32_t)param_6};return (int32_t)routing_invoke(0x51dc10u,a,6);}
uint32_t FUN_0051d420(int32_t x,int32_t y,int32_t tx,int32_t ty)
{const uint32_t a[]={(uint32_t)x,(uint32_t)y,(uint32_t)tx,(uint32_t)ty};return (uint32_t)routing_invoke(0x51d420u,a,4);}
void FUN_004690f0(int32_t tile,int32_t limit)
{const uint32_t a[]={(uint32_t)tile,(uint32_t)limit};(void)routing_invoke(0x4690f0u,a,2);}

/* Shared service coverage, patrol direction and storage selection. */
__declspec(noinline) void FUN_004b21d0(int32_t actor)
{restore_actor_choose_nearby_road_direction(actor);}
int32_t FUN_0045bb50(int32_t building,int32_t resource,char require_stock)
{return restore_storage_find_source(building,resource,require_stock);}
uint32_t FUN_0047e340(int32_t tile,uint32_t actor)
{const uint32_t a[]={(uint32_t)tile,(uint32_t)actor};return (uint32_t)routing_invoke(0x0047e340u,a,2);}
int32_t FUN_0045dc50(int32_t x1,int32_t y1,int32_t x2,int32_t y2,int32_t region1,int32_t region2)
{return restore_storage_distance(x1,y1,x2,y2,region1,region2);}
