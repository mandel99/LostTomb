#include "diplomacy_input_actions.h"
#include "town_trade.h"
#include "placement_support.h"
#include "monument_progress.h"
#include "building_create_model.h"
#include "menu_resource_render.h"
#include "redraw_dispatch_model.h"
#include "actor_walking.h"
#include "order_input.h"
#include "menu_dialog.h"
#include <stdlib.h>
#include <string.h>

#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
typedef char diplomacy_pointer32[sizeof(void *)==4?1:-1];
extern uint32_t FUN_004ea930(void);
extern void FUN_004e96c0(int32_t amount);

static void building_draw(uint32_t site,uint32_t target,struct restore_diplomacy_frame *f)
{
    struct restore_menu_input_frame b;
    memcpy(b.r,f->r,sizeof(b.r));b.flags=f->flags;b.boundary=restore_building_create_dispatch;
    restore_building_create_dispatch(site,target,&b);
    memcpy(f->r,b.r,sizeof(b.r));f->flags=b.flags;
}

static void dispatch(uint32_t site,uint32_t target,struct restore_diplomacy_frame *f)
{
    uint32_t sp=f->r[4],cleanup=0;
#define A(n) U(sp+4u*(n))
#define RAW(a) case 0x##a##u:restore_diplomacy_C_##a(f);return
    switch(target){
    RAW(0040ae10);RAW(0040af30);RAW(0040b140);RAW(0040b1c0);
    RAW(0040b1f0);RAW(0040b380);RAW(0040b3b0);RAW(0044d520);
    RAW(0045a970);RAW(0046ec70);RAW(0046e880);RAW(0046edd0);
    RAW(00402340);RAW(0044c1a0);RAW(004b84b0);RAW(004b8460);
    case 0x44c110u:restore_resource_render_raw(target,f->r,&f->flags);return;
    case 0x5468d0u:building_draw(site,target,f);return;
    /* Native town iterator is ECX+self +one stack argument/RET4. The
       eligibility query is ECX+self, zero stack arguments/RET0/AL result. */
    case 0x44baf0u:
        f->r[0]=(uint32_t)(uintptr_t)restore_town_trade_next((struct RestoreTownList *)(uintptr_t)f->r[1],
                                                                          (const int16_t *)(uintptr_t)A(1));
        cleanup=4;break;
    case 0x44d4c0u:f->r[0]=(f->r[0]&0xffffff00u)|(uint8_t)FUN_0044d4c0((const struct RestoreTownRecord *)(uintptr_t)f->r[1]);break;
    case 0x45b4d0u:f->r[0]=(uint32_t)FUN_0045b4d0((int32_t)A(1),(int32_t)A(2),(int32_t)A(3));break;
    case 0x4ea930u:f->r[0]=FUN_004ea930();break;
    case 0x4e96c0u:FUN_004e96c0((int32_t)A(1));break;
    case 0x40b5e0u:f->r[0]=FUN_0040b5e0((int32_t)A(1));break;
    case 0x4b3250u:FUN_004b3250((int32_t)A(1));break;
    case 0x4dea50u:FUN_004dea50();break;
    case 0x4264d0u:FUN_004264d0(A(1),A(2),A(3));break;
    case 0x545740u:f->r[0]=(uint32_t)restore_monument_progress_00545740((void *)(uintptr_t)f->r[1],A(1),(int32_t *)(uintptr_t)A(2));cleanup=8;break;
    case 0x558be0u:f->r[0]=(uint32_t)restore_monument_progress_00558be0((void *)(uintptr_t)f->r[1],A(1),(int32_t *)(uintptr_t)A(2));cleanup=8;break;
    case 0x555e80u:f->r[0]=(uint32_t)restore_monument_progress_00555e80((void *)(uintptr_t)f->r[1],A(1),(int32_t *)(uintptr_t)A(2));cleanup=8;break;
    case 0x54f470u:f->r[0]=(uint32_t)restore_monument_progress_0054f470((void *)(uintptr_t)f->r[1],A(1),(int32_t *)(uintptr_t)A(2));cleanup=8;break;
    default:abort();
    }
    f->r[4]=sp+4u+cleanup;
#undef RAW
#undef A
}

static void invoke(uint32_t entry)
{
    /* Natural native scratch;44D520 writes the actual DWORD output local
       before any nonzero status can make40AE10 consume it. Virtual member
       receivers, local aliases and RET4/RET8 are retained by the raw graph. */
    uint32_t stack[1024],sp=(uint32_t)(uintptr_t)(stack+768);
    struct restore_diplomacy_frame f={{0},2,dispatch};
    stack[768]=0;f.r[4]=sp;dispatch(0,entry,&f);
    if(f.r[4]!=sp+4u)abort();
}
void restore_diplomacy_0040ae10(void){invoke(0x40ae10u);}
void restore_diplomacy_0040af30(void){invoke(0x40af30u);}
void restore_diplomacy_0040b140(void){invoke(0x40b140u);}
void restore_diplomacy_0040b1c0(void){invoke(0x40b1c0u);}
void restore_diplomacy_0040b1f0(void){invoke(0x40b1f0u);}
void restore_diplomacy_0040b380(void){invoke(0x40b380u);}
void restore_diplomacy_0040b3b0(void){invoke(0x40b3b0u);}
void restore_diplomacy_00402340(void){invoke(0x402340u);}
void restore_diplomacy_0045a970(void){invoke(0x45a970u);}

int restore_diplomacy_input_action(uint32_t token)
{
    switch(token){
    case 0x40ae10u:case 0x40af30u:case 0x40b140u:case 0x40b1c0u:
    case 0x40b1f0u:case 0x40b380u:case 0x40b3b0u:case 0x402340u:invoke(token);return 1;
    default:return 0;
    }
}

/* Compose the same genuine private graph with another caller's native stack.
   Keep its incoming registers/flags and RET cleanup; no second body copy. */
void restore_diplomacy_input_raw(uint32_t target,uint32_t registers[8],uint32_t *flags)
{
    struct restore_diplomacy_frame f;
    memcpy(f.r,registers,sizeof(f.r));f.flags=*flags;f.boundary=dispatch;
    dispatch(0,target,&f);
    memcpy(registers,f.r,sizeof(f.r));*flags=f.flags;
}
