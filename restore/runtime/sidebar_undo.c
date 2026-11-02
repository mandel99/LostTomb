#include "sidebar_undo.h"
#include "god_building_effects.h"
#include "building_refresh_helpers.h"
#include "building_support.h"
#include "building_controllers.h"
#include "building_create_model.h"
#include "construction_brush.h"
#include "map_commit_selection.h"
#include "map_commit_state.h"
#include "redraw_dispatch_model.h"
#include "map_commit_links.h"
#include "clear_footprint.h"
#include "road_place.h"
#include "drag_leaves.h"
#include "map_commit_connect.h"
#include "map_commit_costs.h"
#include "menu_building_kind.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>
#include <string.h>

#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
typedef char sidebar_undo_pointer32[sizeof(void *)==4?1:-1];
extern void FUN_0040f860(const void *source,void *destination,int32_t bytes);

/* The exhausted-slot path of native46A280 returns directly after this void
   Win32 API. Undo4858E0 consumes the incidental AL. Capture the real current
   provider's physical EAX, rather than fabricate a success/failure value.
   This OS residue is not a portable or specified OutputDebugString result. */
static uint32_t debug_output_eax(const char *text)
{
    void (WINAPI *output)(LPCSTR)=OutputDebugStringA;uint32_t result;
    __asm {
        push text
        mov eax,output
        call eax
        mov result,eax
    }
    return result;
}

static void dispatch(uint32_t site,uint32_t target,struct restore_sidebar_frame *f)
{
    uint32_t sp=f->r[4],cleanup=0;(void)site;
#define A(n) U(sp+4u*(n))
#define RAW(a) case 0x##a##u:restore_sidebar_C_##a(f);return
    switch(target){
    RAW(00485230);RAW(00485890);RAW(004858e0);
    RAW(0040d890);RAW(00401480);RAW(00416fb0);RAW(004e6660);
    RAW(00545720);RAW(00558bc0);RAW(00555e60);RAW(0054f450);RAW(00546660);
    RAW(0045afe0);RAW(0045e220);RAW(0046b320);RAW(0046f030);RAW(0046a320);
    RAW(004e0c80);RAW(0046a280);
    case 0x43fd00u:case 0x45b2a0u:case 0x4bfee0u:
        restore_god_building_raw(target,f->r,&f->flags);return;
    case 0x488e60u:case 0x4dfbb0u:
        restore_building_refresh_raw(target,f->r,&f->flags);return;
    case 0x40f860u:FUN_0040f860((const void *)(uintptr_t)A(1),(void *)(uintptr_t)A(2),(int32_t)A(3));break;
    case 0x40f920u:FUN_0040f920((void *)(uintptr_t)A(1),(int32_t)A(2),(uint8_t)A(3));break;
    case 0x46b100u:f->r[0]=(uint32_t)FUN_0046b100((int32_t)A(1));break;
    case 0x46cf40u:FUN_0046cf40((int32_t)A(1));break;
    case 0x46efd0u:f->r[0]=(uint32_t)FUN_0046efd0(A(1));break;
    case 0x470990u:restore_placement_create_00470990(A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),
                                                  (int32_t)A(5),(int32_t)A(6),A(7),A(8));break;
    case 0x477a40u:FUN_00477a40((int32_t)A(1),(int32_t)A(2),A(3));break;
    case 0x47bf50u:FUN_0047bf50((uint16_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),
                              (int32_t)A(5),A(6),A(7));break;
    case 0x47c480u:FUN_0047c480((int32_t)A(1),(int32_t)A(2),(int32_t)A(3));break;
    case 0x485830u:FUN_00485830();break; /* Native ignores its extra pushed DWORD. */
    case 0x488030u:f->r[0]=FUN_00488030(A(1));cleanup=4;break;
    case 0x4a36b0u:FUN_004a36b0((int32_t)A(1));break;
    case 0x4dea50u:FUN_004dea50();break;
    case 0x4f4930u:FUN_004f4930((int32_t)A(1));break;
    case 0x4f4f80u:f->r[0]=(uint32_t)FUN_004f4f80((int32_t)A(1));break;
    case 0x4f61e0u:FUN_004f61e0();break;
    case 0x517370u:FUN_00517370();break;
    case 0x517660u:FUN_00517660();break;
    case 0x5178e0u:FUN_005178e0();break;
    case 0x56f0f8u:f->r[0]=debug_output_eax((const char *)(uintptr_t)A(1));cleanup=4;break;
    default:abort();
    }
    f->r[4]=sp+4u+cleanup;
#undef RAW
#undef A
}

void restore_sidebar_raw(uint32_t target,uint32_t registers[8],uint32_t *flags)
{
    struct restore_sidebar_frame f;
    memcpy(f.r,registers,sizeof(f.r));f.flags=*flags;f.boundary=dispatch;
    dispatch(0,target,&f);
    memcpy(registers,f.r,sizeof(f.r));*flags=f.flags;
}

void restore_sidebar_undo_00485230(void)
{
    /* No incoming volatile register or FPU input. Native locals are written
       by the original instructions; unused stack bytes remain uninitialized.
       Real member ECX and RET4/RET12 contracts stay inside the raw graph. */
    uint32_t stack[1024],sp=(uint32_t)(uintptr_t)(stack+768);
    struct restore_sidebar_frame f={{0},2,dispatch};
    stack[768]=0;f.r[4]=sp;restore_sidebar_C_00485230(&f);
    if(f.r[4]!=sp+4u)abort();
}
void restore_sidebar_undo_00406010(void)
{
    restore_sidebar_undo_00485230();B(0xe38dcau)=1;
}
int restore_sidebar_undo_action(uint32_t token)
{
    if(token!=0x406010u)return 0;
    restore_sidebar_undo_00406010();return 1;
}
