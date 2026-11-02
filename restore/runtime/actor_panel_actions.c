#include "actor_panel_actions.h"
#include "unit_keyboard_actions.h"
#include "diplomacy_input_actions.h"
#include "camera_navigation.h"
#include "menu_dialog.h"
#include "remaining_menu_actions.h"
#include <stdlib.h>
#include <string.h>

#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
typedef char actor_panel_pointer32[sizeof(void *)==4?1:-1];
/* These unique canonical APIs expose already-owned genuine implementations;
   neither declaration permits a legacy implicit-receiver fallback. */
extern void restore_unit_actor_return_004bc670(uint32_t id);
extern void restore_diplomacy_input_raw(uint32_t target,uint32_t registers[8],uint32_t *flags);

static void dispatch(uint32_t site,uint32_t target,struct restore_actor_panel_frame *f)
{
    uint32_t sp=f->r[4];(void)site;
#define A(n) U(sp+4u*(n))
#define RAW(a) case 0x##a##u:restore_actor_panel_C_##a(f);return
    switch(target){
    /* Native40A550 is one tail JMP: keep the same virtual entry SP. */
    case 0x40a550u:restore_actor_panel_C_0040a450(f);return;
    RAW(0040a450);RAW(0040a560);RAW(0040a5c0);RAW(0040a600);
    RAW(004bc620);RAW(00521a60);RAW(00521aa0);RAW(004b8aa0);
    case 0x44baf0u:case 0x44d4c0u:case 0x44d520u:case 0x44c1a0u:
        restore_diplomacy_input_raw(target,f->r,&f->flags);return;
    case 0x4bc670u:restore_unit_actor_return_004bc670(A(1));break;
    case 0x430cd0u:FUN_00430cd0(A(1));break;
    case 0x4264d0u:FUN_004264d0(A(1),A(2),A(3));break;
    case 0x40a0e0u:FUN_0040a0e0();break;
    default:abort();
    }
    f->r[4]=sp+4u;
#undef RAW
#undef A
}

void restore_actor_panel_raw(uint32_t target,uint32_t registers[8],uint32_t *flags)
{
    struct restore_actor_panel_frame f;
    memcpy(f.r,registers,sizeof(f.r));f.flags=*flags;f.boundary=dispatch;
    dispatch(0,target,&f);memcpy(registers,f.r,sizeof(f.r));*flags=f.flags;
}

static void invoke(uint32_t target)
{
    /* Native40A450's DWORD output slot is entrySP-4. 44D520 returns zero
       without writing it when no qualifying record exists. 40A504 still
       reads the slot; a previous successful iteration deliberately retains
       its value. Do not initialize/reset it or manufacture a default result.
       The raw API accepts an exact caller-supplied prestate. This ordinary
       entry uses natural uninitialized C scratch; the first never-written
       slot's bit equivalence to an original caller stack remains unproved. */
    uint32_t stack[512],sp=(uint32_t)(uintptr_t)(stack+384);
    struct restore_actor_panel_frame f={{0},2,dispatch};
    stack[384]=0;f.r[4]=sp;dispatch(0,target,&f);
    if(f.r[4]!=sp+4u)abort();
}
void restore_actor_panel_0040a450(void){invoke(0x40a450u);}
void restore_actor_panel_0040a550(void){invoke(0x40a550u);}
void restore_actor_panel_0040a560(void){invoke(0x40a560u);}
void restore_actor_panel_0040a5c0(void){invoke(0x40a5c0u);}
void restore_actor_panel_0040a600(void){invoke(0x40a600u);}
void restore_actor_panel_requests_00521a60(uint8_t *first,uint8_t *second)
{
    uint32_t stack[128],sp=(uint32_t)(uintptr_t)(stack+96);
    struct restore_actor_panel_frame f={{0},2,dispatch};
    stack[96]=0;stack[97]=(uint32_t)(uintptr_t)first;stack[98]=(uint32_t)(uintptr_t)second;
    f.r[4]=sp;dispatch(0,0x521a60u,&f);
}
int restore_actor_panel_action(uint32_t token)
{
    switch(token){
    case 0x40a450u:case 0x40a550u:case 0x40a560u:case 0x40a5c0u:case 0x40a600u:invoke(token);return 1;
    default:return 0;
    }
}
