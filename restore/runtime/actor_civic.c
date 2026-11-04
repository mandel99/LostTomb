#include "storage_transactions.h"
#include "actor_civic.h"
#include "monument_footprint.h"
#include "actor_predicate.h"
#include "actor_laborer.h"
#include "actor_routing.h"
#include "actor_walking.h"
#include "actor_work.h"
#include "actor_teardown_model.h"
#include "actor_callbacks_chain.h"
#include "unit_button_actions.h"
#include "ground.h"
#include "actor_order_path.h"
#include "actor_order_los.h"
#include "actor_order_action.h"
#include "mission_unlock.h"
#include "actor_command.h"
#include "building_nearest_search.h"
#include "editor_validation.h"
#include <windows.h>
#include <mmsystem.h>
#include "training_queries.h"
#include "map_commit_links.h"
#include "invasion_commands.h"
#include "building_create_model.h"
#include "event_publisher.h"
#include "text_selection.h"
#include "pending_message.h"
#include "actor_transport.h"
#include "menu_actor_message.h"
#include "menu_prepare_integer.h"
#include "menu_group_selection.h"
#include "building_controllers.h"
#include "menu_list_scroll.h"
#include "sidebar_undo.h"
#include "placement_support.h"
#include "actor_constable.h"
#include "actor_combat.h"
#include "actor_callbacks_human.h"
#include "command_migration.h"
#include "population_updates.h"
#include "displaced_population.h"
#include "actor_profile_model.h"
#include "menu_event_actions.h"
#include "legacy_format.h"
#include <stdlib.h>
#include <string.h>
/* Keep the legacy byte-return entry available to native-address lookup. */
#pragma comment(linker, "/include:_FUN_004eaec0")
#pragma comment(linker, "/include:_FUN_0049e8b0")
extern void FUN_0040f8f0(uint32_t *,uint32_t,uint8_t);
extern void FUN_004e96c0(int32_t);
extern void FUN_004e0cc0(int32_t,int32_t *,int32_t *);
extern void FUN_004bd250(int32_t);
extern uint8_t FUN_0051cd80(int32_t,int32_t,int32_t,int32_t);
extern uint8_t FUN_0051cc00(int32_t,int32_t,int32_t,int32_t);
extern uint32_t FUN_0047d350(void);
extern int32_t FUN_004b6ff0(uint16_t,int32_t,int32_t,uint16_t,uint16_t);
extern uint32_t FUN_00477690(uint32_t,uint32_t);
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))

void restore_actor_civic_dispatch(uint32_t site,uint32_t target,struct restore_civic_frame *f)
{
    uint32_t sp=f->r[4];(void)site;
#define A(n) U(sp+4u*(n))
#define RAW(a) case 0x##a##u:restore_civic_C_##a(f);return
    switch(target){
    RAW(004ac9f0);
    RAW(004aceb0);
    RAW(004adf60);
    RAW(004aee10);
    RAW(004af140);
    RAW(004af4c0);
    RAW(004af4e0);
    RAW(004af7c0);
    RAW(004af970);
    RAW(004afc40);
    RAW(004afeb0);
    RAW(004b0ec0);
    RAW(004ad350);
    RAW(004ad450);
    RAW(004add90);
    RAW(004adb40);
    RAW(004adc80);
    RAW(004ae1f0);
    RAW(004aed10);
    RAW(004ae270);
    RAW(004ae2c0);
    RAW(004ae650);
    RAW(004ae490);
    RAW(004aeb00);
    RAW(004aec80);
    RAW(004aece0);
    RAW(004ad4c0);
    RAW(004a5c10);
    RAW(004bcf40);
    RAW(004aeab0);
    RAW(004bcf70);
    RAW(004bce90);
    RAW(004463c0);
    RAW(004a1ef0);
    RAW(004a2430);
    RAW(004a3070);
    RAW(004a3b70);
    RAW(004a4fd0);
    RAW(004a7030);
    RAW(004a2320);
    RAW(004867e0);
    RAW(004a34b0);
    RAW(004a36e0);
    RAW(004a38a0);
    RAW(004a3b00);
    RAW(004be4a0);
    RAW(004a46f0);
    RAW(004a4770);
    RAW(004a4950);
    RAW(004a4a80);
    RAW(004a4c40);
    RAW(004a4d00);
    RAW(004a4e10);
    RAW(004a4f60);
    RAW(004ad340);
    RAW(004a58e0);
    RAW(004a6bf0);
    RAW(004a6890);
    RAW(004a6160);
    RAW(004a6300);
    RAW(004a6970);
    RAW(004a6b40);
    RAW(004a6cc0);
    RAW(004a6d40);
    RAW(004b3340);
    RAW(00486810);
    RAW(004a3540);
    RAW(004a5560);
    RAW(004a4850);
    RAW(004a4340);
    RAW(004a43f0);
    RAW(004a4470);
    RAW(004ad880);
    RAW(004a44f0);
    RAW(004b6400);
    RAW(004bcd60);
    RAW(004bcae0);
    RAW(004a5680);
    RAW(004a5a80);
    RAW(004a5880);
    RAW(004a59c0);
    RAW(004a5a20);
    RAW(004f68f0);
    RAW(004bcd90);
    RAW(004bcb10);
    RAW(004f6900);
    RAW(0049b750);
    RAW(0049bd40);
    RAW(0049c750);
    RAW(0049d0f0);
    RAW(0049dc50);
    RAW(0049f710);
    RAW(004a04d0);
    RAW(004a1130);
    RAW(004ecd40);
    RAW(004eca70);
    RAW(00494f70);
    RAW(00497010);
    RAW(00497450);
    RAW(0049a150);
    RAW(0049a5a0);
    RAW(0049a880);
    RAW(0049ac00);
    RAW(0049aeb0);
    RAW(0049b280);
    RAW(0049b610);
    RAW(00495840);
    RAW(00495950);
    RAW(00445980);
    RAW(00445e50);
    RAW(004807d0);
    RAW(00497cf0);
    RAW(00497a30);
    RAW(004b28a0);
    RAW(004b56d0);
    RAW(004b5770);
    RAW(00480e90);
    RAW(00445670);
    RAW(00445ac0);
    RAW(004454d0);
    RAW(00491a90);
    RAW(004923a0);
    RAW(00492580);
    RAW(00492770);
    RAW(004930b0);
    RAW(004937a0);
    RAW(004808a0);
    RAW(004809b0);
    RAW(00480860);
    RAW(00495af0);
    RAW(00495a40);
    RAW(00492360);
    RAW(004b22e0);
    RAW(00492650);
    RAW(00492d20);
    RAW(004f6420);
    RAW(00492fc0);
    RAW(00480b80);
    RAW(00480d60);
    RAW(00480c60);
    RAW(00459ff0);
    RAW(0048e9e0);
    RAW(00490b50);
    RAW(00490e00);
    RAW(00491420);
    RAW(004999e0);
    RAW(00499b50);
    RAW(004b1700);
    RAW(004b1210);
    RAW(004bc4a0);
    RAW(004b5050);
    RAW(004b7ab0);
    RAW(00467450);
    RAW(00468530);
    RAW(004450c0);
    RAW(0045d010);
    RAW(00444bd0);
    RAW(0045d370);
    RAW(00444b80);
    RAW(00519a30);
    RAW(0045d4d0);
    RAW(00494800);
    RAW(00494b30);
    RAW(00497dc0);
    RAW(00497fc0);
    RAW(00498160);
    RAW(00498370);
    RAW(004984a0);
    RAW(00498590);
    RAW(004986b0);
    RAW(00498750);
    RAW(004987f0);
    RAW(004988b0);
    RAW(00498a10);
    RAW(00498ad0);
    RAW(00498c60);
    RAW(00498df0);
    RAW(00499010);
    RAW(004991a0);
    RAW(004998f0);
    RAW(00499e50);
    RAW(004b4420);
    RAW(004b45c0);
    RAW(004b53b0);
    RAW(00494200);
    RAW(004942f0);
    RAW(004943e0);
    RAW(004944d0);
    RAW(004945c0);
    RAW(004946b0);
    RAW(004abd50);
    RAW(0048bff0);
    RAW(0048c6a0);
    RAW(004ac450);
    RAW(004941b0);
    RAW(004947a0);
    RAW(0049b2a0);
    RAW(0048c200);
    RAW(00493610);
    RAW(004b4220);
    RAW(004b46b0);
    RAW(0048ded0);RAW(0048e5e0);RAW(00469a50);
    RAW(0048a720);RAW(00458c30);RAW(004e91e0);
    case 0x49e8b0u:f->r[0]=restore_actor_update_hippo();break;
    case 0x4eaac0u:f->r[0]=restore_actor_route_line_mode(A(1));break;
    case 0x4eab10u:f->r[0]=(uint32_t)FUN_004eab10((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),(int32_t)A(5),(int32_t)A(6));break;
    case 0x4eaec0u:f->r[0]=restore_actor_try_periodic_target((int32_t)A(1));break;
    case 0x4eb000u:f->r[0]=FUN_004eb000((int32_t)A(1));break;
    case 0x4ebc90u:f->r[0]=(uint32_t)FUN_004ebc90((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),
        (const uint32_t *)(uintptr_t)A(4),A(5),(int32_t *)(uintptr_t)A(6),(int32_t *)(uintptr_t)A(7),(int32_t)A(8),(int32_t)A(9),(int32_t)A(10));break;
    case 0x4eabf0u:f->r[0]=FUN_004eabf0((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),
        (int32_t *)(uintptr_t)A(5),(int32_t *)(uintptr_t)A(6));break;
    case 0x56233eu:f->r[0]=restore_actor_random_seed(A(1));break;
    case 0x48c2b0u:FUN_0048c2b0();break;
    case 0x4a5da0u:restore_actor_routing_raw(target,f->r,&f->flags);return;
    case 0x4b1ef0u:case 0x4b3ff0u:case 0x4b1df0u:case 0x4b3250u:
        restore_actor_laborer_raw(target,f->r,&f->flags);return;
    case 0x4b5420u:case 0x4b52f0u:case 0x4b4180u:case 0x4b43a0u:case 0x4b43e0u:
    case 0x4b4800u:case 0x562348u:case 0x4a35e0u:
        restore_actor_laborer_raw(target,f->r,&f->flags);return;
    case 0x45cf50u:case 0x45d180u:case 0x4434e0u:case 0x4435f0u:case 0x45b230u:
        {struct restore_actor_message_frame child;memcpy(child.r,f->r,sizeof(child.r));child.flags=f->flags;
         child.boundary=restore_actor_message_dispatch;restore_actor_message_dispatch(site,target,&child);
         memcpy(f->r,child.r,sizeof(f->r));f->flags=child.flags;return;}
    case 0x45d510u:case 0x4b28f0u:restore_actor_transport_raw(target,f->r,&f->flags);return;
    case 0x45dc50u:restore_actor_routing_raw(target,f->r,&f->flags);return;
    case 0x45b2a0u:restore_sidebar_raw(target,f->r,&f->flags);return;
    case 0x480180u:restore_actor_laborer_raw(target,f->r,&f->flags);return;
    case 0x4b7c80u:case 0x4c52d0u:restore_building_control_raw(target,f->r,&f->flags);return;
    case 0x475b50u:f->r[0]=FUN_00475b50(A(1));break;
    case 0x4c5120u:f->r[0]=FUN_004c5120((int32_t)A(1));break;
    case 0x4ea9c0u:f->r[0]=(uint32_t)FUN_004ea9c0((int32_t)A(1));break;
    case 0x4ea9a0u:f->r[0]=FUN_004ea9a0((int32_t)A(1));break;
    case 0x4b7b50u:FUN_004b7b50((int32_t)A(1),(int16_t)A(2));break;
    case 0x4cde60u:f->r[0]=(uint32_t)FUN_004cde60((int32_t)A(1),(int32_t)A(2));break;
    case 0x4cde40u:f->r[0]=(uint32_t)FUN_004cde40((int32_t)A(1),(int32_t)A(2));break;
    case 0x45b570u:f->r[0]=(uint32_t)FUN_0045b570((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4));break;
    case 0x40f8f0u:FUN_0040f8f0((uint32_t *)(uintptr_t)A(1),A(2),(uint8_t)A(3));break;
    case 0x459ec0u:case 0x4684e0u:restore_invasion_raw(target,f->r,&f->flags);return;
    case 0x4b3c60u:restore_actor_routing_raw(target,f->r,&f->flags);return;
    case 0x545a30u:case 0x558f00u:case 0x556230u:case 0x54f8a0u:case 0x5468d0u:
        {struct restore_menu_input_frame child;memcpy(child.r,f->r,sizeof(child.r));child.flags=f->flags;
         child.boundary=restore_building_create_dispatch;restore_building_create_dispatch(site,target,&child);
         memcpy(f->r,child.r,sizeof(f->r));f->flags=child.flags;return;}
    case 0x4ea930u:restore_actor_laborer_raw(target,f->r,&f->flags);return;
    case 0x4806c0u:f->r[0]=FUN_004806c0((int32_t)A(1),(int32_t)A(2),A(3));break;
    case 0x4e0cf0u:FUN_004e0cf0((int32_t)A(1),A(2),A(3),(const int16_t *)(uintptr_t)A(4),(uint8_t)A(5));break;
    case 0x4e0cc0u:FUN_004e0cc0((int32_t)A(1),(int32_t *)(uintptr_t)A(2),(int32_t *)(uintptr_t)A(3));break;
    case 0x4e96c0u:FUN_004e96c0((int32_t)A(1));break;
    case 0x4cc110u:FUN_004cc110((int32_t)A(1),(int32_t)A(2));break;
    case 0x4e2970u:FUN_004e2970((int32_t)A(1));break;
    case 0x459ef0u:f->r[0]=(uint32_t)FUN_00459ef0((int32_t)A(1));break;
    case 0x480060u:restore_invasion_raw(target,f->r,&f->flags);return;
    case 0x4b1d10u:f->r[0]=FUN_004b1d10((int32_t)A(1),(int32_t)A(2));break;
    case 0x4c5240u:f->r[0]=FUN_004c5240((int32_t)A(1));break;
    case 0x48b4a0u:restore_actor_transport_raw(target,f->r,&f->flags);return;
    case 0x517f20u:case 0x5181c0u:case 0x519be0u:
        restore_actor_routing_raw(target,f->r,&f->flags);return;
    case 0x4ec7c0u:case 0x4ec990u:case 0x4ec0f0u:
        restore_actor_command_raw(target,f->r,&f->flags);return;
    case 0x4ec410u:f->r[0]=FUN_004ec410(A(1),(char)A(2));break;
    case 0x4eb800u:
        f->r[0]=(uint32_t)restore_building_nearest_search(
         (int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(const uint32_t *)(uintptr_t)A(4),A(5),
         (int32_t *)(uintptr_t)A(6),(int32_t *)(uintptr_t)A(7),(int32_t)A(8),(int32_t)A(9));break;
    case 0x56f22cu:f->r[0]=timeGetTime();break;
    case 0x486600u:FUN_00486600(A(1));break;
    case 0x4f6630u:f->r[0]=restore_ring_004f6630(f->r[1]);break;
    case 0x4f6620u:f->r[0]=restore_ring_004f6620(f->r[1]);break;
    case 0x4a45d0u:restore_unit_actor_assign_building(A(1));break;
    case 0x4a4300u:f->r[0]=(f->r[0]&0xffffff00u)|restore_unit_actor_is_linked(A(1));break;
    case 0x4069e0u:f->r[0]=(uint32_t)restore_unit_building_rank(A(1));break;
    case 0x406a30u:f->r[0]=restore_unit_nearest_building(A(1));break;
    case 0x5190a0u:restore_actor_transport_raw(target,f->r,&f->flags);return;
    case 0x49b3c0u:f->r[0]=(uint32_t)FUN_0049b3c0((int32_t)A(1));break;
    case 0x4b27e0u:restore_actor_routing_raw(target,f->r,&f->flags);return;
    case 0x4467a0u:case 0x48c0e0u:restore_invasion_raw(target,f->r,&f->flags);return;
    case 0x40bda0u:
        f->r[0]=(uint32_t)FUN_0040bda0((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),(int32_t *)(uintptr_t)A(5),(int32_t *)(uintptr_t)A(6));break;
    case 0x40c180u:
        f->r[0]=(uint32_t)FUN_0040c180((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),(int32_t *)(uintptr_t)A(5),(int32_t *)(uintptr_t)A(6));break;
    case 0x477690u:f->r[0]=FUN_00477690(A(1),A(2));break;
    case 0x4a34f0u:restore_actor_laborer_raw(target,f->r,&f->flags);return;
    case 0x517a40u:f->r[0]=FUN_00517a40((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4));break;
    case 0x518400u:f->r[0]=FUN_00518400((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4));break;
    case 0x4b63d0u:f->r[0]=FUN_004b63d0((int32_t)A(1));break;
    case 0x53b090u:FUN_0053b090((int32_t)A(1));break;
    case 0x4f6a10u:
        f->r[0]=(f->r[0]&0xffffff00u)|restore_ring_004f6a10((void *)(uintptr_t)f->r[1],(volatile uint32_t *)(uintptr_t)A(1));f->r[4]=sp+8u;return;
    case 0x4f6a50u:
        f->r[0]=restore_ring_004f6a50((void *)(uintptr_t)f->r[1],(int32_t)A(1));f->r[4]=sp+8u;return;
    /* Native51F780 is a single RET, also the type0 callback. */
    case 0x51f780u:break;
    case 0x469140u:restore_invasion_raw(target,f->r,&f->flags);return;
    case 0x489970u:case 0x51dc10u:
        restore_actor_routing_raw(target,f->r,&f->flags);return;
    case 0x44ef60u:case 0x44e6f0u:
        restore_event_actions_raw(target,f->r,&f->flags);return;
    case 0x4b6ff0u:
        f->r[0]=(uint32_t)FUN_004b6ff0((uint16_t)A(1),(int32_t)A(2),(int32_t)A(3),(uint16_t)A(4),(uint16_t)A(5));break;
    case 0x4b5af0u:FUN_004b5af0((int32_t)A(1));break;
    case 0x459020u:FUN_00459020((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4));break;
    case 0x561a36u:
        /* Both4B0155/4B02D3 literals consume one actor ID; cdecl RET0. */
        if(site!=0x4b0155u&&site!=0x4b02d3u)abort();
        f->r[0]=(uint32_t)FUN_00561a36((char *)(uintptr_t)A(1),(const char *)(uintptr_t)A(2),(int32_t)A(3));break;
    case 0x56f0f8u:
        OutputDebugStringA((const char *)(uintptr_t)A(1));f->r[4]=sp+8u;return;
    case 0x53b4a0u:FUN_0053b4a0((int32_t)A(1),(int32_t)A(2),(int32_t)A(3));break;
    case 0x4bd250u:FUN_004bd250((int32_t)A(1));break;
    case 0x4b5210u:FUN_004b5210((int32_t)A(1));break;
    case 0x51cae0u:f->r[0]=(f->r[0]&0xffffff00u)|FUN_0051cae0((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4));break;
    case 0x51cd80u:f->r[0]=(f->r[0]&0xffffff00u)|FUN_0051cd80((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4));break;
    case 0x4b3f00u:FUN_004b3f00((int32_t)A(1));break;
    case 0x4b17d0u:FUN_004b17d0((int32_t)A(1));break;
    case 0x4b1d50u:FUN_004b1d50((int32_t)A(1));break;
    case 0x47fe60u:f->r[0]=FUN_0047fe60((int32_t)A(1),(int32_t)A(2),A(3),(int32_t)A(4));break;
    case 0x51cba0u:f->r[0]=FUN_0051cba0((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4));break;
    case 0x51cc00u:
        f->r[0]=(f->r[0]&0xffffff00u)|FUN_0051cc00((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4));break;
    case 0x47d350u:f->r[0]=FUN_0047d350();break;
    case 0x47a1c0u:
        f->r[0]=(f->r[0]&0xffffff00u)|(uint32_t)(uint8_t)FUN_0047a1c0((int32_t)A(1));break;
    case 0x46cea0u:FUN_0046cea0((int32_t)A(1),(int16_t)A(2));break;
    case 0x4e9400u:FUN_004e9400((int32_t)A(1),(int32_t)A(2));break;
    default:abort();
    }
    /* Only real query EAX/AL values cross these typed boundaries.
       Their native continuations redefine consumed volatile GPRs/flags.
       All shared movement/service families retain their full native frame. */
    f->r[4]=sp+4u;
#undef RAW
#undef A
}
void restore_actor_civic_raw(uint32_t target,uint32_t registers[8],uint32_t *flags)
{
    struct restore_civic_frame f;
    memcpy(f.r,registers,sizeof(f.r));f.flags=*flags;f.boundary=restore_actor_civic_dispatch;
    restore_actor_civic_dispatch(0,target,&f);
    memcpy(registers,f.r,sizeof(f.r));*flags=f.flags;
}
static uint32_t invoke(uint32_t entry,const uint32_t *args,uint32_t count)
{
    uint32_t stack[4096],sp=(uint32_t)(uintptr_t)(stack+4000),i;
    struct restore_civic_frame f={{0},2,restore_actor_civic_dispatch};
    /* Current actor is global; no incoming receiver/FPU or extra caller slots.
       Preserve natural native scratch rather than synthesizing local values. */
    stack[4000]=0;f.r[4]=sp;
    for(i=0;i<count;++i)stack[4001+i]=args[i];
    restore_actor_civic_dispatch(0,entry,&f);
    /* The relocation helper consumes its actor slot with native RET4. */
    if(f.r[4]!=sp+(entry==0x4be4a0u?8u:4u))abort();
    return f.r[0];
}
int32_t FUN_004a3b00(int32_t actor,int32_t period)
{const uint32_t a[]={(uint32_t)actor,(uint32_t)period};return (int32_t)invoke(0x4a3b00u,a,2);}
/* Complete native bodies: WORD cell links/coordinates, BYTE actor kinds and
   phases, DWORD models/counters. These cdecl entries own their local frames;
   no incoming receiver, flags, FPU state or extra caller array is consumed. */
void FUN_004a4f60(int32_t actor)
{const uint32_t a[]={(uint32_t)actor};(void)invoke(0x4a4f60u,a,1);}
void FUN_004a46f0(int32_t actor)
{const uint32_t a[]={(uint32_t)actor};(void)invoke(0x4a46f0u,a,1);}
uint32_t FUN_004a43f0(int32_t actor)
{const uint32_t a[]={(uint32_t)actor};return invoke(0x4a43f0u,a,1);}
void FUN_004a4470(int32_t group)
{const uint32_t a[]={(uint32_t)group};(void)invoke(0x4a4470u,a,1);}
void FUN_004a44f0(int32_t group,int32_t x,int32_t y)
{const uint32_t a[]={(uint32_t)group,(uint32_t)x,(uint32_t)y};(void)invoke(0x4a44f0u,a,3);}
void FUN_004a4770(int32_t actor,int32_t building)
{const uint32_t a[]={(uint32_t)actor,(uint32_t)building};(void)invoke(0x4a4770u,a,2);}
void FUN_004a4850(int32_t *out_x,int32_t *out_y,int32_t dx1,int32_t dy1,
                 int32_t dx2,int32_t dy2,int32_t dx3,int32_t dy3,int32_t actor)
{const uint32_t a[]={(uint32_t)(uintptr_t)out_x,(uint32_t)(uintptr_t)out_y,
 (uint32_t)dx1,(uint32_t)dy1,(uint32_t)dx2,(uint32_t)dy2,(uint32_t)dx3,(uint32_t)dy3,(uint32_t)actor};
 (void)invoke(0x4a4850u,a,9);}
int32_t FUN_004a5560(int32_t actor,int32_t size,int32_t mode)
{const uint32_t a[]={(uint32_t)actor,(uint32_t)size,(uint32_t)mode};return (int32_t)invoke(0x4a5560u,a,3);}
void FUN_004a5680(int32_t actor,int32_t mode)
{const uint32_t a[]={(uint32_t)actor,(uint32_t)mode};(void)invoke(0x4a5680u,a,2);}
void FUN_004a5880(int32_t effect)
{const uint32_t a[]={(uint32_t)effect};(void)invoke(0x4a5880u,a,1);}
int32_t FUN_004a59c0(int32_t cell,int32_t excluded_actor)
{const uint32_t a[]={(uint32_t)cell,(uint32_t)excluded_actor};return (int32_t)invoke(0x4a59c0u,a,2);}
int32_t FUN_004a5a20(int32_t cell,int32_t excluded_actor)
{const uint32_t a[]={(uint32_t)cell,(uint32_t)excluded_actor};return (int32_t)invoke(0x4a5a20u,a,2);}
int32_t FUN_004a5a80(int32_t actor)
{const uint32_t a[]={(uint32_t)actor};return (int32_t)invoke(0x4a5a80u,a,1);}
#define ACTOR_MOVING_ENTRY(token) \
void FUN_##token(int32_t actor,uint32_t *moving) \
{const uint32_t a[]={(uint32_t)actor,(uint32_t)(uintptr_t)moving};(void)invoke(0x##token##u,a,2);}
ACTOR_MOVING_ENTRY(004a6160)
ACTOR_MOVING_ENTRY(004a4a80)
ACTOR_MOVING_ENTRY(004a4950)
ACTOR_MOVING_ENTRY(004a4c40)
ACTOR_MOVING_ENTRY(004a4d00)
ACTOR_MOVING_ENTRY(004a4e10)
ACTOR_MOVING_ENTRY(004a6300)
ACTOR_MOVING_ENTRY(004a6890)
ACTOR_MOVING_ENTRY(004a6970)
ACTOR_MOVING_ENTRY(004a6d40)
#undef ACTOR_MOVING_ENTRY
uint8_t FUN_004a4340(int32_t actor,int32_t building)
{const uint32_t a[]={(uint32_t)actor,(uint32_t)building};return (uint8_t)invoke(0x4a4340u,a,2);}
void FUN_00494200(void){(void)invoke(0x00494200u,0,0);}
void FUN_004942f0(void){(void)invoke(0x004942f0u,0,0);}
void FUN_004943e0(void){(void)invoke(0x004943e0u,0,0);}
void FUN_004944d0(void){(void)invoke(0x004944d0u,0,0);}
void FUN_004945c0(void){(void)invoke(0x004945c0u,0,0);}
void FUN_004946b0(void){(void)invoke(0x004946b0u,0,0);}
void FUN_004abd50(void){(void)invoke(0x004abd50u,0,0);}
void FUN_0048bff0(void){(void)invoke(0x0048bff0u,0,0);}
void FUN_0048c6a0(void){(void)invoke(0x0048c6a0u,0,0);}
void FUN_004ac450(void){(void)invoke(0x004ac450u,0,0);}
void FUN_004941b0(void){(void)invoke(0x004941b0u,0,0);}
void FUN_004947a0(void){(void)invoke(0x004947a0u,0,0);}
void FUN_0049b2a0(void){(void)invoke(0x0049b2a0u,0,0);}
void FUN_0048c200(void){(void)invoke(0x0048c200u,0,0);}
void FUN_00493610(void){(void)invoke(0x493610u,0,0);}
void FUN_004b4220(int32_t actor,int32_t x,int32_t y,int32_t target_x,int32_t target_y,int32_t mode)
{
    const uint32_t a[]={(uint32_t)actor,(uint32_t)x,(uint32_t)y,(uint32_t)target_x,(uint32_t)target_y,(uint32_t)mode};
    (void)invoke(0x4b4220u,a,6);
}
uint32_t FUN_004b46b0(int32_t actor)
{const uint32_t a[]={(uint32_t)actor};return invoke(0x4b46b0u,a,1);}

void FUN_0048ded0(void){(void)invoke(0x48ded0u,0,0);}
uint32_t FUN_0048e5e0(int32_t actor)
{const uint32_t a[]={(uint32_t)actor};return invoke(0x48e5e0u,a,1);}
int32_t FUN_00469a50(int32_t x,int32_t y)
{const uint32_t a[]={(uint32_t)x,(uint32_t)y};return (int32_t)invoke(0x469a50u,a,2);}

void FUN_0048a720(void){(void)invoke(0x48a720u,0,0);}
int32_t FUN_00458c30(int32_t x,int32_t y)
{const uint32_t a[]={(uint32_t)x,(uint32_t)y};return (int32_t)invoke(0x458c30u,a,2);}
void FUN_004e91e0(int32_t player,int32_t amount)
{const uint32_t a[]={(uint32_t)player,(uint32_t)amount};(void)invoke(0x4e91e0u,a,2);}


void FUN_00494800(void){(void)invoke(0x00494800u,0,0);}
void FUN_00494b30(void){(void)invoke(0x00494b30u,0,0);}
void FUN_00497dc0(void){(void)invoke(0x00497dc0u,0,0);}
void FUN_00497fc0(void){(void)invoke(0x00497fc0u,0,0);}
void FUN_00498160(void){(void)invoke(0x00498160u,0,0);}
void FUN_00498370(void){(void)invoke(0x00498370u,0,0);}
void FUN_004984a0(void){(void)invoke(0x004984a0u,0,0);}
void FUN_00498590(void){(void)invoke(0x00498590u,0,0);}
void FUN_004986b0(void){(void)invoke(0x004986b0u,0,0);}
void FUN_00498750(void){(void)invoke(0x00498750u,0,0);}
void FUN_004987f0(void){(void)invoke(0x004987f0u,0,0);}
void FUN_004988b0(void){(void)invoke(0x004988b0u,0,0);}
void FUN_00498a10(void){(void)invoke(0x00498a10u,0,0);}
void FUN_00498ad0(void){(void)invoke(0x00498ad0u,0,0);}
void FUN_00498c60(void){(void)invoke(0x00498c60u,0,0);}
void FUN_00498df0(void){(void)invoke(0x00498df0u,0,0);}
void FUN_00499010(void){(void)invoke(0x00499010u,0,0);}
void FUN_004991a0(void){(void)invoke(0x4991a0u,0,0);}
int32_t FUN_004998f0(int32_t x,int32_t y)
{const uint32_t a[]={(uint32_t)x,(uint32_t)y};return (int32_t)invoke(0x4998f0u,a,2);}
int32_t FUN_00499e50(int32_t actor,int32_t max_distance)
{const uint32_t a[]={(uint32_t)actor,(uint32_t)max_distance};return (int32_t)invoke(0x499e50u,a,2);}
uint32_t FUN_004b4420(int32_t x,int32_t y,int32_t tx,int32_t ty)
{const uint32_t a[]={(uint32_t)x,(uint32_t)y,(uint32_t)tx,(uint32_t)ty};return invoke(0x4b4420u,a,4);}
int32_t FUN_004b45c0(int32_t actor,int32_t x,int32_t y,int32_t tx,int32_t ty,int32_t kind)
{const uint32_t a[]={(uint32_t)actor,(uint32_t)x,(uint32_t)y,(uint32_t)tx,(uint32_t)ty,(uint32_t)kind};return (int32_t)invoke(0x4b45c0u,a,6);}
void FUN_004b53b0(int32_t actor)
{const uint32_t a[]={(uint32_t)actor};(void)invoke(0x4b53b0u,a,1);}
void FUN_004be4a0(int32_t actor)
{const uint32_t a[]={(uint32_t)actor};(void)invoke(0x4be4a0u,a,1);}

void FUN_0048e9e0(void)
{(void)invoke(0x0048e9e0u,0,0);}

void FUN_00490b50(void)
{(void)invoke(0x00490b50u,0,0);}

void FUN_00490e00(void)
{(void)invoke(0x00490e00u,0,0);}

void FUN_00491420(void)
{(void)invoke(0x00491420u,0,0);}

int32_t FUN_004999e0(int32_t p0,int32_t p1,int32_t p2,int32_t p3)
{const uint32_t args[]={(uint32_t)p0,(uint32_t)p1,(uint32_t)p2,(uint32_t)p3};return (int32_t)invoke(0x004999e0u,args,4);}

int32_t FUN_00499b50(int32_t p0,int32_t p1)
{const uint32_t args[]={(uint32_t)p0,(uint32_t)p1};return (int32_t)invoke(0x00499b50u,args,2);}

void FUN_004b1700(int32_t p0)
{const uint32_t args[]={(uint32_t)p0};(void)invoke(0x004b1700u,args,1);}

void FUN_004b5050(int32_t p0)
{const uint32_t args[]={(uint32_t)p0};(void)invoke(0x004b5050u,args,1);}

void FUN_004b7ab0(int32_t p0)
{const uint32_t args[]={(uint32_t)p0};(void)invoke(0x004b7ab0u,args,1);}

void FUN_00468530(int32_t p0)
{const uint32_t args[]={(uint32_t)p0};(void)invoke(0x00468530u,args,1);}

void FUN_004b1210(int32_t p0,int32_t p1)
{const uint32_t args[]={(uint32_t)p0,(uint32_t)p1};(void)invoke(0x004b1210u,args,2);}

void FUN_004bc4a0(int32_t p0,int32_t p1)
{const uint32_t args[]={(uint32_t)p0,(uint32_t)p1};(void)invoke(0x004bc4a0u,args,2);}

void FUN_00444bd0(int32_t p0,int32_t p1)
{const uint32_t args[]={(uint32_t)p0,(uint32_t)p1};(void)invoke(0x00444bd0u,args,2);}

void FUN_00444b80(int32_t p0,int32_t p1)
{const uint32_t args[]={(uint32_t)p0,(uint32_t)p1};(void)invoke(0x00444b80u,args,2);}

void FUN_00519a30(int32_t p0,int32_t p1)
{const uint32_t args[]={(uint32_t)p0,(uint32_t)p1};(void)invoke(0x00519a30u,args,2);}

int32_t FUN_00467450(int32_t p0)
{const uint32_t args[]={(uint32_t)p0};return (int32_t)invoke(0x00467450u,args,1);}

int32_t FUN_004450c0(int32_t p0,uint32_t p1,uint32_t p2,int32_t p3,uint32_t p4)
{const uint32_t args[]={(uint32_t)p0,(uint32_t)p1,(uint32_t)p2,(uint32_t)p3,(uint32_t)p4};return (int32_t)invoke(0x004450c0u,args,5);}

int32_t FUN_0045d010(int32_t p0,int32_t p1,uint32_t p2)
{return restore_storage_export(p0,p1,p2);}

int32_t FUN_0045d370(int32_t p0,int32_t p1,uint32_t p2)
{return restore_storage_import(p0,p1,p2);}

void FUN_0045d4d0(int32_t p0,uint32_t p1,int32_t p2)
{restore_trade_charge_import(p0,p1,p2);}

void FUN_00491a90(void)
{(void)invoke(0x00491a90u,0,0);}

void FUN_004923a0(void)
{(void)invoke(0x004923a0u,0,0);}

void FUN_00492580(void)
{(void)invoke(0x00492580u,0,0);}

void FUN_00492770(void)
{(void)invoke(0x00492770u,0,0);}

void FUN_004930b0(void)
{(void)invoke(0x004930b0u,0,0);}

void FUN_004937a0(void)
{(void)invoke(0x004937a0u,0,0);}

int32_t FUN_004808a0(int32_t p0)
{const uint32_t args[]={(uint32_t)p0};return (int32_t)invoke(0x004808a0u,args,1);}

int32_t FUN_004809b0(int32_t p0)
{const uint32_t args[]={(uint32_t)p0};return (int32_t)invoke(0x004809b0u,args,1);}

int32_t FUN_00480860(int32_t p0)
{const uint32_t args[]={(uint32_t)p0};return (int32_t)invoke(0x00480860u,args,1);}

int32_t FUN_00495af0(int32_t p0)
{const uint32_t args[]={(uint32_t)p0};return (int32_t)invoke(0x00495af0u,args,1);}

int32_t FUN_00495a40(int32_t p0)
{const uint32_t args[]={(uint32_t)p0};return (int32_t)invoke(0x00495a40u,args,1);}

int32_t FUN_00492360(int32_t p0)
{const uint32_t args[]={(uint32_t)p0};return (int32_t)invoke(0x00492360u,args,1);}

int32_t FUN_00492650(int32_t p0)
{const uint32_t args[]={(uint32_t)p0};return (int32_t)invoke(0x00492650u,args,1);}

int32_t FUN_00492d20(int32_t p0)
{const uint32_t args[]={(uint32_t)p0};return (int32_t)invoke(0x00492d20u,args,1);}

int32_t FUN_00480b80(int32_t p0)
{const uint32_t args[]={(uint32_t)p0};return (int32_t)invoke(0x00480b80u,args,1);}

int32_t FUN_00480d60(int32_t p0)
{const uint32_t args[]={(uint32_t)p0};return (int32_t)invoke(0x00480d60u,args,1);}

int32_t FUN_00480c60(int32_t p0)
{const uint32_t args[]={(uint32_t)p0};return (int32_t)invoke(0x00480c60u,args,1);}

int32_t FUN_00459ff0(int32_t p0)
{const uint32_t args[]={(uint32_t)p0};return (int32_t)invoke(0x00459ff0u,args,1);}

void FUN_00492fc0(void)
{restore_monument_consume_resource();}

void FUN_004b22e0(int32_t p0,int32_t p1,int32_t p2)
{const uint32_t args[]={(uint32_t)p0,(uint32_t)p1,(uint32_t)p2};(void)invoke(0x004b22e0u,args,3);}

void FUN_004f6420(int32_t p0,int32_t * p1,int32_t * p2)
{const uint32_t args[]={(uint32_t)p0,(uint32_t)(uintptr_t)p1,(uint32_t)(uintptr_t)p2};(void)invoke(0x004f6420u,args,3);}

void FUN_00494f70(void)
{(void)invoke(0x00494f70u,0,0);}

void FUN_00497010(void)
{(void)invoke(0x00497010u,0,0);}

void FUN_00497450(void)
{(void)invoke(0x00497450u,0,0);}

void FUN_0049a150(void)
{(void)invoke(0x0049a150u,0,0);}

void FUN_0049a5a0(void)
{(void)invoke(0x0049a5a0u,0,0);}

void FUN_0049a880(void)
{(void)invoke(0x0049a880u,0,0);}

void FUN_0049ac00(void)
{(void)invoke(0x0049ac00u,0,0);}

void FUN_0049aeb0(void)
{(void)invoke(0x0049aeb0u,0,0);}

void FUN_0049b280(void)
{(void)invoke(0x0049b280u,0,0);}

void FUN_0049b610(void)
{(void)invoke(0x0049b610u,0,0);}

int32_t FUN_00495840(int32_t p0)
{const uint32_t args[]={(uint32_t)p0};return (int32_t)invoke(0x00495840u,args,1);}

int32_t FUN_00495950(int32_t p0)
{const uint32_t args[]={(uint32_t)p0};return (int32_t)invoke(0x00495950u,args,1);}

int32_t FUN_00497cf0(int32_t p0)
{const uint32_t args[]={(uint32_t)p0};return (int32_t)invoke(0x00497cf0u,args,1);}

int32_t FUN_00497a30(int32_t p0)
{const uint32_t args[]={(uint32_t)p0};return (int32_t)invoke(0x00497a30u,args,1);}

int32_t FUN_004b56d0(int32_t p0)
{const uint32_t args[]={(uint32_t)p0};return (int32_t)invoke(0x004b56d0u,args,1);}

int32_t FUN_004b5770(int32_t p0)
{const uint32_t args[]={(uint32_t)p0};return (int32_t)invoke(0x004b5770u,args,1);}

int32_t FUN_00480e90(int32_t p0)
{const uint32_t args[]={(uint32_t)p0};return (int32_t)invoke(0x00480e90u,args,1);}

int32_t FUN_004807d0(int32_t p0,int32_t p1,int32_t p2)
{const uint32_t args[]={(uint32_t)p0,(uint32_t)p1,(uint32_t)p2};return (int32_t)invoke(0x004807d0u,args,3);}

void FUN_004b28a0(int32_t p0,int32_t p1)
{const uint32_t args[]={(uint32_t)p0,(uint32_t)p1};(void)invoke(0x004b28a0u,args,2);}

int32_t FUN_00445980(int32_t p0,int32_t p1,int32_t p2,int32_t p3)
{const uint32_t args[]={(uint32_t)p0,(uint32_t)p1,(uint32_t)p2,(uint32_t)p3};return (int32_t)invoke(0x00445980u,args,4);}

int32_t FUN_00445e50(int32_t p0,int32_t p1,int32_t p2,int32_t p3)
{const uint32_t args[]={(uint32_t)p0,(uint32_t)p1,(uint32_t)p2,(uint32_t)p3};return (int32_t)invoke(0x00445e50u,args,4);}

int32_t FUN_004454d0(int32_t p0,int32_t p1,int32_t p2)
{const uint32_t args[]={(uint32_t)p0,(uint32_t)p1,(uint32_t)p2};return (int32_t)invoke(0x004454d0u,args,3);}

int32_t FUN_00445670(uint32_t p0,uint32_t p1,int32_t p2,uint32_t p3,uint32_t p4)
{const uint32_t args[]={(uint32_t)p0,(uint32_t)p1,(uint32_t)p2,(uint32_t)p3,(uint32_t)p4};return (int32_t)invoke(0x00445670u,args,5);}

int32_t FUN_00445ac0(uint32_t p0,uint32_t p1,int32_t p2,uint32_t p3,uint32_t p4)
{const uint32_t args[]={(uint32_t)p0,(uint32_t)p1,(uint32_t)p2,(uint32_t)p3,(uint32_t)p4};return (int32_t)invoke(0x00445ac0u,args,5);}

void FUN_0049b750(void)
{(void)invoke(0x0049b750u,0,0);}
void FUN_0049bd40(void)
{(void)invoke(0x0049bd40u,0,0);}
void FUN_0049c750(void)
{(void)invoke(0x0049c750u,0,0);}
void FUN_0049d0f0(void)
{(void)invoke(0x0049d0f0u,0,0);}
void FUN_0049dc50(void)
{(void)invoke(0x0049dc50u,0,0);}
__declspec(noinline) void FUN_0049e8b0(void)
{(void)restore_actor_update_hippo();}
void FUN_0049f710(void)
{(void)invoke(0x0049f710u,0,0);}
void FUN_004a04d0(void)
{(void)invoke(0x004a04d0u,0,0);}
void FUN_004a1130(void)
{(void)invoke(0x004a1130u,0,0);}
__declspec(noinline) uint8_t FUN_004eaac0(int32_t p0)
{return (uint8_t)restore_actor_route_line_mode((uint32_t)p0);}
__declspec(noinline) int32_t FUN_004eab10(int32_t p0,int32_t p1,int32_t p2,int32_t p3,int32_t p4,int32_t p5)
{return restore_actor_find_clear_vector(p0,p1,p2,p3,p4,(uint32_t)p5);}
__declspec(noinline) uint8_t FUN_004eb000(int32_t p0)
{return restore_actor_update_group_target(p0);}
__declspec(noinline) int32_t FUN_004ebc90(int32_t p0,int32_t p1,int32_t p2,const uint32_t * p3,uint32_t p4,int32_t * p5,int32_t * p6,int32_t p7,int32_t p8,int32_t p9)
{return restore_actor_find_state_nearby(p0,p1,p2,p3,p4,p5,p6,p7,p8,(uint32_t)p9);}
__declspec(noinline) uint8_t FUN_004eabf0(int32_t p0,int32_t p1,int32_t p2,int32_t p3,int32_t * p4,int32_t * p5)
{return restore_actor_choose_clear_step(p0,p1,p2,p3,p4,p5);}
__declspec(noinline) uint8_t FUN_004eaec0(int32_t p0)
{return (uint8_t)restore_actor_try_periodic_target(p0);}
void FUN_004ecd40(int32_t p0)
{const uint32_t args[]={(uint32_t)p0};(void)invoke(0x004ecd40u,args,1);}
void FUN_004eca70(int32_t p0,int32_t p1)
{const uint32_t args[]={(uint32_t)p0,(uint32_t)p1};(void)invoke(0x004eca70u,args,2);}
__declspec(noinline) void FUN_0056233e(uint32_t p0)
{(void)restore_actor_random_seed(p0);}

void FUN_004a1ef0(void)
{(void)invoke(0x004a1ef0u,0,0);}
void FUN_004a2430(void)
{(void)invoke(0x004a2430u,0,0);}
void FUN_004a3070(void)
{(void)invoke(0x004a3070u,0,0);}
void FUN_004a3b70(void)
{(void)invoke(0x004a3b70u,0,0);}
void FUN_004a4fd0(void)
{(void)invoke(0x004a4fd0u,0,0);}
void FUN_004a7030(void)
{(void)invoke(0x004a7030u,0,0);}
uint32_t FUN_004ad880(int32_t *x,int32_t *y)
{
 const uint32_t args[]={(uint32_t)(uintptr_t)x,(uint32_t)(uintptr_t)y};
 return (uint32_t)(uint8_t)invoke(0x004ad880u,args,2);
}

void FUN_004ac9f0(void)
{(void)invoke(0x004ac9f0u,0,0);}
void FUN_004aceb0(void)
{(void)invoke(0x004aceb0u,0,0);}
void FUN_004adf60(void)
{(void)invoke(0x004adf60u,0,0);}
void FUN_004aee10(void)
{(void)invoke(0x004aee10u,0,0);}
void FUN_004af140(void)
{(void)invoke(0x004af140u,0,0);}
void FUN_004af4c0(void)
{(void)invoke(0x004af4c0u,0,0);}
void FUN_004af4e0(void)
{(void)invoke(0x004af4e0u,0,0);}
void FUN_004af7c0(void)
{(void)invoke(0x004af7c0u,0,0);}
void FUN_004af970(void)
{(void)invoke(0x004af970u,0,0);}
void FUN_004afc40(void)
{(void)invoke(0x004afc40u,0,0);}
void FUN_004afeb0(void)
{(void)invoke(0x004afeb0u,0,0);}
void FUN_004b0ec0(void)
{(void)invoke(0x004b0ec0u,0,0);}
void FUN_004463c0(int32_t event_index,uint32_t cell)
{
 const uint32_t args[]={(uint32_t)event_index,cell};
 (void)invoke(0x004463c0u,args,2);
}

/* Canonical typed facades for the reviewed naval and targeting helpers. */
void FUN_004ad350(int32_t p0, int32_t * p1)
{const uint32_t args[]={(uint32_t)p0,(uint32_t)(uintptr_t)p1};(void)invoke(0x004ad350u,args,2);}
void FUN_004ad450(int32_t p0, int32_t * p1)
{const uint32_t args[]={(uint32_t)p0,(uint32_t)(uintptr_t)p1};(void)invoke(0x004ad450u,args,2);}
void FUN_004ad4c0(int32_t p0, int32_t p1, int32_t p2)
{const uint32_t args[]={(uint32_t)p0,(uint32_t)p1,(uint32_t)p2};(void)invoke(0x004ad4c0u,args,3);}
void FUN_004adb40(int32_t p0, int32_t * p1)
{const uint32_t args[]={(uint32_t)p0,(uint32_t)(uintptr_t)p1};(void)invoke(0x004adb40u,args,2);}
void FUN_004adc80(int32_t p0)
{const uint32_t args[]={(uint32_t)p0};(void)invoke(0x004adc80u,args,1);}
void FUN_004add90(int32_t p0, int32_t * p1)
{const uint32_t args[]={(uint32_t)p0,(uint32_t)(uintptr_t)p1};(void)invoke(0x004add90u,args,2);}
void FUN_004ae1f0(int32_t p0)
{const uint32_t args[]={(uint32_t)p0};(void)invoke(0x004ae1f0u,args,1);}
void FUN_004ae270(int32_t p0, int32_t * p1)
{const uint32_t args[]={(uint32_t)p0,(uint32_t)(uintptr_t)p1};(void)invoke(0x004ae270u,args,2);}
void FUN_004ae2c0(int32_t p0, int32_t * p1)
{const uint32_t args[]={(uint32_t)p0,(uint32_t)(uintptr_t)p1};(void)invoke(0x004ae2c0u,args,2);}
void FUN_004ae490(int32_t p0, int32_t * p1)
{const uint32_t args[]={(uint32_t)p0,(uint32_t)(uintptr_t)p1};(void)invoke(0x004ae490u,args,2);}
void FUN_004ae650(int32_t p0, int32_t * p1)
{const uint32_t args[]={(uint32_t)p0,(uint32_t)(uintptr_t)p1};(void)invoke(0x004ae650u,args,2);}
uint8_t FUN_004aeab0(int32_t * p0, int32_t * p1)
{const uint32_t args[]={(uint32_t)(uintptr_t)p0,(uint32_t)(uintptr_t)p1};return (uint8_t)invoke(0x004aeab0u,args,2);}
void FUN_004aeb00(int32_t p0, int32_t * p1)
{const uint32_t args[]={(uint32_t)p0,(uint32_t)(uintptr_t)p1};(void)invoke(0x004aeb00u,args,2);}
void FUN_004aec80(int32_t p0, int32_t * p1)
{const uint32_t args[]={(uint32_t)p0,(uint32_t)(uintptr_t)p1};(void)invoke(0x004aec80u,args,2);}
void FUN_004aece0(void)
{(void)invoke(0x004aece0u,0,0);}
void FUN_004aed10(int32_t p0)
{const uint32_t args[]={(uint32_t)p0};(void)invoke(0x004aed10u,args,1);}
int32_t FUN_004a5c10(int32_t p0)
{const uint32_t args[]={(uint32_t)p0};return (int32_t)invoke(0x004a5c10u,args,1);}
int32_t FUN_004bcf40(int32_t p0, int32_t p1)
{const uint32_t args[]={(uint32_t)p0,(uint32_t)p1};return (int32_t)invoke(0x004bcf40u,args,2);}
int32_t FUN_004bcf70(int32_t p0, int32_t p1, int32_t p2)
{const uint32_t args[]={(uint32_t)p0,(uint32_t)p1,(uint32_t)p2};return (int32_t)invoke(0x004bcf70u,args,3);}
uint8_t FUN_004bce90(int32_t p0)
{const uint32_t args[]={(uint32_t)p0};return (uint8_t)invoke(0x004bce90u,args,1);}
