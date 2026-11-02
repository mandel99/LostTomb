#include "menu_rotation_actions.h"
#include "building_create_model.h"
#include "building_refresh_helpers.h"
#include "city_footprint_validation.h"
#include "actor_walking.h"
#include "camera_navigation.h"
#include "invasion_commands.h"
#include "lookup_sentinel.h"
#include "map_gates.h"
#include "map_commit_costs.h"
#include "remaining_menu_actions.h"
#include <stdlib.h>
#include <string.h>

#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
extern uint32_t FUN_004e0c50(int32_t building,int32_t *x,int32_t *y);

static void dispatch(uint32_t site,uint32_t target,struct restore_menu_input_frame *f)
{
    uint32_t sp=f->r[4];
#define A(n) U(sp+4u*(n))
#define RAW(address) case 0x##address##u:restore_rotation_C_##address(f);return
    switch(target){
    RAW(00409eb0);RAW(00430b20);RAW(00430be0);RAW(0047d6e0);
    RAW(0047d720);RAW(0047fb70);RAW(0046d840);RAW(004dfd90);RAW(004ee560);
    case 0x497b90u:
        restore_invasion_raw(target,f->r,&f->flags);return;
    case 0x4dfbb0u:case 0x4ee4a0u:
        restore_building_refresh_raw(target,f->r,&f->flags);return;
    case 0x47bf50u:case 0x4bfb30u:case 0x47a1c0u:
    case 0x47d1f0u:case 0x47cf00u:case 0x487ec0u:
    case 0x475b50u:case 0x440060u:case 0x440540u:case 0x4c0d50u:
    case 0x541920u:case 0x40dca0u:case 0x4017a0u:
    case 0x4e69c0u:case 0x5468d0u:{
        /* Same native stack and every GPR/flag. The child graph must use
           its own boundary for its genuine descendants, then restore the
           rotation boundary on return; the stack is never copied/reset. */
        struct restore_menu_input_frame child=*f;
        child.boundary=restore_building_create_dispatch;
        restore_building_create_dispatch(site,target,&child);
        memcpy(f->r,child.r,sizeof(f->r));f->flags=child.flags;return;
    }
    case 0x430530u:FUN_00430530(A(1));break;
    case 0x4304c0u:FUN_004304c0(A(1));break;
    case 0x47d870u:FUN_0047d870(A(1));break;
    case 0x4dea50u:FUN_004dea50();break;
    case 0x5178e0u:FUN_005178e0();break;
    case 0x4ddf90u:FUN_004ddf90();break;
    case 0x4de080u:FUN_004de080();break;
    case 0x4e2970u:restore_remaining_message_4e2970((int32_t)A(1),A(2),A(3));break;
    case 0x4ded50u:
        f->r[0]=(uint32_t)(uintptr_t)restore_city_geometry_004ded50(
            (int32_t *)(uintptr_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4));break;
    case 0x4e0c50u:
        f->r[0]=FUN_004e0c50((int32_t)A(1),(int32_t *)(uintptr_t)A(2),
                            (int32_t *)(uintptr_t)A(3));break;
    case 0x4b5420u:FUN_004b5420((int32_t)A(1));break;
    case 0x4b52f0u:FUN_004b52f0((int32_t)A(1));break;
    case 0x4b3250u:FUN_004b3250((int32_t)A(1));break;
    default:abort();
    }
    /* All typed boundaries are native cdecl/RET0, including the final
       47D6E0->4DDF90 JMP: consume the existing return address once.
       Caller ADD ESP owns argument cleanup. Typed void outputs are dead
       in these native continuations; queries publish consumed full EAX.
       The raw building/member graphs retain their separate RETn contracts. */
    f->r[4]=sp+4u;
#undef RAW
#undef A
}

void restore_rotation_raw(uint32_t entry,uint32_t registers[8],uint32_t *flags)
{
    struct restore_menu_input_frame f;
    memcpy(f.r,registers,sizeof(f.r));f.flags=*flags;f.boundary=dispatch;
    dispatch(0,entry,&f);
    memcpy(registers,f.r,sizeof(f.r));*flags=f.flags;
}

int restore_menu_rotation_action(uint32_t token)
{
    uint32_t stack[4096],sp;
    struct restore_menu_input_frame f={{0},2,dispatch};
    if(token!=0x409eb0u)return 0;
    /* Callback input is global state, with no incoming argument, receiver
       or FPU value. Saved nonvolatile registers are not data inputs.
       Native geometry locals stay natural scratch: 4DED50 writes the
       thirteen DWORDs before reads, 4E0C50 writes both coordinate outputs.
       Raw descendants share this reserved lower stack, including the
       building refresh/member graphs and the owned invasion refresher. */
    sp=(uint32_t)(uintptr_t)(stack+4000);stack[4000]=0;f.r[4]=sp;
    dispatch(0,token,&f);
    if(f.r[4]!=sp+4u)abort();
    return 1;
}
