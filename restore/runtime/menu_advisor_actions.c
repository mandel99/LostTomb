#include "menu_advisor_actions.h"
#include "menu_labor_actions.h"
#include "menu_economy_actions.h"
#include "diplomacy_input_actions.h"
#include "actor_panel_actions.h"
#include "building_controllers.h"
#include "building_refresh_helpers.h"
#include "menu_resource_neighbors.h"
#include "actor_predicate.h"
#include "map_commit_population.h"
#include "message_panel_input.h"
#include "remaining_menu_actions.h"
#include "event_publisher.h"
#include "menu_full_input_numeric.h"
#include <stdlib.h>
#include <string.h>

#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))

static void dispatch(uint32_t site,uint32_t target,struct restore_advisor_frame *f)
{
    uint32_t sp=f->r[4];(void)site;
#define A(n) U(sp+4u*(n))
#define RAW(address) case 0x##address##u:restore_advisor_C_##address(f);return
    switch(target){
    RAW(0040b520);RAW(004179a0);RAW(0040a160);
    RAW(0045a8c0);RAW(0045a930);RAW(0045d5d0);RAW(0045d630);
    RAW(00468170);RAW(0046e1d0);RAW(0046e1c0);
    RAW(004269b0);RAW(00426a00);RAW(00426a80);RAW(00426b00);
    RAW(0046e410);RAW(004c38f0);RAW(00451b10);RAW(004e9500);
    RAW(004c06e0);RAW(004e94a0);RAW(004e94d0);RAW(004e7e60);
    RAW(004f6ad0);RAW(004ea280);
    case 0x459400u:case 0x4595a0u:case 0x459670u:
        restore_labor_raw(target,f->r,&f->flags);return;
    case 0x45a970u:case 0x46ec70u:
        restore_diplomacy_input_raw(target,f->r,&f->flags);return;
    case 0x4b7c80u:case 0x4c52d0u:
        restore_building_control_raw(target,f->r,&f->flags);return;
    case 0x4b8aa0u:
        restore_actor_panel_raw(target,f->r,&f->flags);return;
    case 0x4ed360u:
        restore_building_refresh_raw(target,f->r,&f->flags);return;
    case 0x459d20u:restore_economy_refresh_00459d20();break;
    case 0x45a990u:restore_economy_refresh_0045a990();break;
    case 0x45a7b0u:restore_economy_refresh_0045a7b0();break;
    case 0x51f780u:break; /* Genuine C3; no invented return value. */
    case 0x4c5120u:f->r[0]=FUN_004c5120((int32_t)A(1));break;
    case 0x4806a0u:f->r[0]=FUN_004806a0((int32_t)A(1),(int32_t)A(2));break;
    case 0x4c5340u:f->r[0]=FUN_004c5340((int32_t)A(1));break;
    case 0x4cde60u:f->r[0]=(uint32_t)FUN_004cde60((int32_t)A(1),(int32_t)A(2));break;
    case 0x4cde40u:f->r[0]=(uint32_t)FUN_004cde40((int32_t)A(1),(int32_t)A(2));break;
    case 0x417ae0u:FUN_00417ae0((int32_t)A(1));break;
    case 0x4e2970u:restore_remaining_message_4e2970((int32_t)A(1),A(2),A(3));break;
    case 0x56133cu:{
        uint8_t raw80[10];uint64_t value;
        /* Native468170 keeps FILD32/FIDIV32 in x87 precision through
           the compiler FTOL helper. Spill extended precision, not double;
           the accepted primitive preserves CW and pops its own value.
           Masked exceptions/inherited PC+RC; FPSW/traps are not an API. */
        __asm { lea eax,raw80 }
        __asm { fstp tbyte ptr[eax] }
        value=restore_menu_input_trunc80(raw80);
        f->r[0]=(uint32_t)value;f->r[2]=(uint32_t)(value>>32);break;
    }
    default:abort();
    }
    /* Typed providers are native cdecl/RET0. The actual continuations
       consume full EAX queries; other volatile outputs/flags are dead.
      40A160's three finance steps pass only their global publications to
       their following nullary helpers. Owned raw families retain all eight
       GPRs/flags, local aliases and their own genuine return cleanup. */
    f->r[4]=sp+4u;
#undef RAW
#undef A
}

void restore_advisor_raw(uint32_t target,uint32_t registers[8],uint32_t *flags)
{
    struct restore_advisor_frame f;
    memcpy(f.r,registers,sizeof(f.r));f.flags=*flags;f.boundary=dispatch;
    dispatch(0,target,&f);
    memcpy(registers,f.r,sizeof(f.r));*flags=f.flags;
}

int restore_menu_advisor_action(uint32_t token)
{
    uint32_t stack[4096],sp;
    struct restore_advisor_frame f={{0},2,dispatch};
    if(token!=0x40b520u)return 0;
    /* Genuine UI entry: globals supply all inputs, no incoming receiver,
       argument or FPU value. Locals remain natural scratch until native
       producers write them; in45D630, REP STOSD initializes all nine food
       totals before accumulation. The native graph shares this one stack. */
    sp=(uint32_t)(uintptr_t)(stack+4000);stack[4000]=0;f.r[4]=sp;
    dispatch(0,token,&f);
    if(f.r[4]!=sp+4u)abort();
    return 1;
}
