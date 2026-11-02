#include "menu_labor_actions.h"
#include "menu_actor_message.h"
#include "actor_teardown_model.h"
#include "map_commit_connect.h"
#include "map_commit_links.h"
#include "message_panel_input.h"
#include <stdlib.h>
#include <string.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))

static void dispatch(uint32_t site,uint32_t target,struct restore_labor_frame *f)
{
    uint32_t sp=f->r[4],cleanup=0;
#define A(n) U(sp+4u*(n))
    switch(target){
    case 0x40a730u:restore_labor_C_0040a730(f);return;
    case 0x459400u:restore_labor_C_00459400(f);return;
    case 0x4595a0u:restore_labor_C_004595a0(f);return;
    case 0x459670u:restore_labor_C_00459670(f);return;
    case 0x51f780u:f->r[4]=sp+4u;return; /* Actual sole C3 instruction. */
    case 0x459240u:{
        struct restore_actor_message_frame child;
        memcpy(child.r,f->r,sizeof(child.r));child.flags=f->flags;
        child.boundary=restore_actor_message_dispatch;
        restore_actor_message_dispatch(site,target,&child);
        memcpy(f->r,child.r,sizeof(f->r));f->flags=child.flags;return;
    }
    case 0x488050u:
        /* Native ECX=BCA350 +one DWORD/RET4. The genuine normalized gate
           owns that fixed manager; continuations consume AL, then redefine
           volatile registers and arithmetic flags before other uses. */
        f->r[0]=restore_actor_building_gate((int32_t)A(1));cleanup=4;break;
    case 0x4cde60u:f->r[0]=(uint32_t)FUN_004cde60((int32_t)A(1),(int32_t)A(2));break;
    case 0x4cde40u:f->r[0]=(uint32_t)FUN_004cde40((int32_t)A(1),(int32_t)A(2));break;
    case 0x43f060u:FUN_0043f060((int32_t)A(1),(uint8_t)A(2),(int32_t)A(3));break;
    case 0x4a36b0u:FUN_004a36b0((int32_t)A(1));break;
    default:abort();
    }
    /* All other actual boundaries are cdecl/RET0. Arithmetic helper
       continuations redefine flags with ADD ESP or their new multiply;
       void completion continuations discard volatile outputs entirely. */
    f->r[4]=sp+4u+cleanup;
#undef A
}
void restore_labor_raw(uint32_t target,uint32_t registers[8],uint32_t *flags)
{
    struct restore_labor_frame f;
    memcpy(f.r,registers,sizeof(f.r));f.flags=*flags;f.boundary=dispatch;
    dispatch(0,target,&f);
    memcpy(registers,f.r,sizeof(f.r));*flags=f.flags;
}
int restore_menu_labor_action(uint32_t token)
{
    uint32_t stack[2048],sp;
    struct restore_labor_frame f={{0},2,dispatch};
    if(token!=0x40a730u)return 0;
    /* Natural scratch: all two priority locals and all ten work-array
       DWORDs are assigned by the native producer before reads. The entry
       has no incoming receiver, argument, FPU or caller-scratch input. */
    sp=(uint32_t)(uintptr_t)(stack+1792);stack[1792]=0;f.r[4]=sp;
    dispatch(0,token,&f);
    if(f.r[4]!=sp+4u)abort();
    return 1;
}
