#include "actor_transport.h"
#include "actor_civic.h"
#include "actor_laborer.h"
#include "actor_routing.h"
#include "actor_constable.h"
#include "actor_combat.h"
#include "actor_callbacks_human.h"
#include "actor_order_path.h"
#include "actor_order_flood.h"
#include "building_refresh_helpers.h"
#include "god_building_effects.h"
#include "sidebar_undo.h"
#include "placement_support.h"
#include "menu_actor_message.h"
#include "menu_resource_neighbors.h"
#include "menu_prepare_integer.h"
#include "city_prepare.h"
#include <stdlib.h>
#include <string.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
void restore_actor_transport_dispatch(uint32_t site,uint32_t target,struct restore_transport_frame *f)
{
    uint32_t sp=f->r[4];(void)site;
#define A(n) U(sp+4u*(n))
#define RAW(a) case 0x##a##u:restore_transport_C_##a(f);return
#define CHILD(frame,call) {struct frame child;memcpy(child.r,f->r,sizeof(child.r));child.flags=f->flags;child.boundary=call;call(site,target,&child);memcpy(f->r,child.r,sizeof(f->r));f->flags=child.flags;return;}
    switch(target){
#define SIX(a) case 0x##a##u:f->r[0]=(uint32_t)FUN_##a((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),(int32_t)A(5),(int32_t)A(6));break
    SIX(00496c70);SIX(00496de0);
    SIX(00496460);SIX(00496550);SIX(004963b0);
    SIX(004968a0);SIX(00496660);
#undef SIX
    RAW(00496640);
    case 0x4b4800u:{
        struct restore_civic_frame child;memcpy(child.r,f->r,sizeof(child.r));
        child.flags=f->flags;child.boundary=restore_actor_civic_dispatch;
        restore_actor_civic_dispatch(site,target,&child);
        memcpy(f->r,child.r,sizeof(f->r));f->flags=child.flags;return;}
    RAW(0045acf0);
    RAW(0045b880);
    RAW(0045d510);
    RAW(0045d9e0);
    RAW(0045dcc0);
    RAW(0045de80);
    RAW(0045e150);
    RAW(0045e360);
    RAW(0045eab0);
    RAW(0045f450);
    RAW(0045f7b0);
    RAW(00460890);
    RAW(0048ae50);
    RAW(0048aef0);
    RAW(0048b320);
    RAW(0048b4a0);
    RAW(0048b620);
    RAW(0048caa0);
    RAW(0048ce30);
    case 0x4b28f0u:f->r[0]=(uint32_t)FUN_004b28f0((int32_t)A(1),(int32_t)A(2),A(3));break;
    RAW(00518dd0);
    RAW(005190a0);
    RAW(00519e30);
    RAW(0051a220);
    case 0x51a9d0u:f->r[0]=(uint32_t)FUN_0051a9d0((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),(int32_t)A(5),(int32_t)A(6));break;
    RAW(0051b220);
    RAW(0051d6d0);
    case 0x45bb50u:case 0x45dc50u:case 0x4a5da0u:case 0x489970u:
    case 0x51b400u:case 0x51d160u:case 0x51d980u:case 0x51dc10u:case 0x51afa0u:
        restore_actor_routing_raw(target,f->r,&f->flags);return;
    case 0x4a35e0u:case 0x4b3250u:restore_actor_laborer_raw(target,f->r,&f->flags);return;
    case 0x518b30u:restore_actor_constable_raw(target,f->r,&f->flags);return;
    case 0x45b2a0u:case 0x45e220u:restore_sidebar_raw(target,f->r,&f->flags);return;
    case 0x45fe20u:restore_god_building_raw(target,f->r,&f->flags);return;
    case 0x488030u:restore_building_refresh_raw(target,f->r,&f->flags);return;
    case 0x459240u:case 0x4894a0u:CHILD(restore_actor_message_frame,restore_actor_message_dispatch);
    case 0x480180u:case 0x4806a0u:CHILD(restore_resource_neighbors_frame,restore_resource_neighbors_dispatch);
    case 0x45b570u:f->r[0]=(uint32_t)FUN_0045b570((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4));break;
    case 0x475b50u:f->r[0]=FUN_00475b50(A(1));break;
    case 0x4cde60u:f->r[0]=(uint32_t)FUN_004cde60((int32_t)A(1),(int32_t)A(2));break;
    case 0x51ce90u:f->r[0]=FUN_0051ce90((int32_t)A(1),(int32_t)A(2));break;
    case 0x51aff0u:f->r[0]=FUN_0051aff0((int32_t)A(1));break;
    case 0x40f8f0u:FUN_0040f8f0((uint32_t *)(uintptr_t)A(1),A(2),(uint8_t)A(3));break;
    case 0x4b17d0u:FUN_004b17d0((int32_t)A(1));break;
    case 0x4b1d50u:FUN_004b1d50((int32_t)A(1));break;
    default:abort();
    }
    /* Typed providers are genuine RET0 queries/void APIs; full EAX queries
       cross here only where native continuations consume it. Members with
       RET4 and all shared route/stock families compose the original frame. */
    f->r[4]=sp+4u;
#undef CHILD
#undef RAW
#undef A
}

