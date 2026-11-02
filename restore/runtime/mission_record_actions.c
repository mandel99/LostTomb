#include "mission_record_actions.h"
#include "building_create_model.h"
#include "building_allocator.h"
#include "sidebar_undo.h"
#include "text_selection.h"
#include "redraw_dispatch_model.h"
#include <windows.h>
#include <mmsystem.h>
#include <stdlib.h>
#include <string.h>

#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
extern void FUN_0046f8d0(void);

static void dispatch(uint32_t site,uint32_t target,struct restore_mission_record_frame *f)
{
    uint32_t sp=f->r[4];
#define A(n) U(sp+4u*(n))
#define RAW(a) case 0x##a##u:restore_mission_record_C_##a(f);return
    switch(target){
    RAW(00418bf0);RAW(004e1770);RAW(004e1830);
    RAW(00481f20);
    RAW(004df2c0);RAW(0040d7f0);RAW(004e65a0);RAW(00547ad0);
    RAW(00545f70);RAW(00559470);RAW(005567c0);
    case 0x40dca0u:case 0x4e69c0u:case 0x5468d0u:{
        struct restore_menu_input_frame child;
        memcpy(child.r,f->r,sizeof(child.r));child.flags=f->flags;
        child.boundary=restore_building_create_dispatch;
        restore_building_create_dispatch(site,target,&child);
        memcpy(f->r,child.r,sizeof(f->r));f->flags=child.flags;return;
    }
    case 0x4e0c80u:restore_sidebar_raw(target,f->r,&f->flags);return;
    case 0x46a400u:
        f->r[0]=(uint32_t)restore_building_allocate_0046a400(A(1),A(2),
            (int32_t)A(3),(int32_t)A(4),(int32_t)A(5),(int32_t)A(6));break;
    case 0x470990u:
        FUN_00470990(A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),
                     (int32_t)A(5),(int32_t)A(6),A(7),A(8));break;
    case 0x4cc110u:FUN_004cc110((int32_t)A(1),(int32_t)A(2));break;
    case 0x4dea50u:FUN_004dea50();break;
    case 0x46f8d0u:FUN_0046f8d0();break;
    case 0x56f22cu:f->r[0]=timeGetTime();break;
    default:abort();
    }
    /* Only allocator EAX and timeGetTime EAX are consumed by these actual
       continuations. Other typed results/flags are dead; native cdecl
       argument cleanup remains in the translated parent. Member methods
       retain their real receiver, registers and RET4 on the shared frame. */
    f->r[4]=sp+4u;
#undef RAW
#undef A
}

static void invoke(uint32_t entry)
{
    uint32_t stack[8192],sp=(uint32_t)(uintptr_t)(stack+8000);
    struct restore_mission_record_frame f={{0},2,dispatch};
    /* These actual entries are nullary; no incoming receiver, FPU value
       or caller scratch input. Keep natural scratch through member calls. */
    stack[8000]=0;f.r[4]=sp;dispatch(0,entry,&f);
    if(f.r[4]!=sp+4u)abort();
}

void FUN_00418bf0(void){invoke(0x418bf0u);}
void FUN_004e1770(void){invoke(0x4e1770u);}
void FUN_004e1830(void){invoke(0x4e1830u);}
void FUN_00481f20(void){invoke(0x481f20u);}
