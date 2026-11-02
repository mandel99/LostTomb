#include "briefing_draw.h"
#include "campaign_actions.h"
#include "menu_list_getter.h"
#include "sprite_render.h"
#include "sprite_libraries.h"
#include "menu_text_glyph.h"
#include "actor_render.h"
#include "rich_message_text.h"
#include "text_selection.h"
#include "menu_training_render.h"
#include <stdlib.h>
#include <string.h>

#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
extern void FUN_005405b0(char *name,char kind);
extern void FUN_004ccbb0(int group,int entry,uint32_t x,uint32_t y,uint32_t font,uint32_t color);

static void dispatch(uint32_t site,uint32_t target,struct restore_briefing_frame *f)
{
    uint32_t sp=f->r[4],cleanup=0;
#define A(n) U(sp+4u*(n))
    switch(target){
    case 0x41a180u:restore_briefing_C_0041a180(f);return;
    case 0x537000u:restore_briefing_C_00537000(f);return;
    case 0x536820u:restore_briefing_C_00536820(f);return;
    case 0x4e2530u:restore_briefing_C_004e2530(f);return;
    case 0x536a30u:{
        struct restore_training_render_frame child;
        memcpy(child.r,f->r,sizeof(child.r));child.flags=f->flags;
        child.boundary=restore_training_render_dispatch;
        restore_training_render_dispatch(site,target,&child);
        memcpy(f->r,child.r,sizeof(f->r));f->flags=child.flags;return;
    }
    case 0x5405b0u:FUN_005405b0((char *)(uintptr_t)A(1),(char)A(2));break;
    case 0x418140u:f->r[0]=(uint32_t)FUN_00418140();break;
    case 0x41b8f0u:
        f->r[0]=(f->r[0]&0xffffff00u)|FUN_0041b8f0(f->r[1]);break;
    case 0x53e590u:
        /* Actual member ECX and RET12, not cdecl sprite-facade cleanup. */
        f->r[0]=(uint32_t)restore_sprite_draw((RestoreSpriteLibraries *)(uintptr_t)f->r[1],
            (int32_t)A(1),(int32_t)A(2),(int32_t)A(3));cleanup=12;break;
    case 0x4cc110u:FUN_004cc110((int32_t)A(1),(int32_t)A(2));break;
    case 0x4cc770u:
        FUN_004cc770((const uint8_t *)(uintptr_t)A(1),(int32_t)A(2),(int32_t)A(3),
                    (int32_t)A(4),(uint16_t)A(5));break;
    case 0x4ccbb0u:
        FUN_004ccbb0((int32_t)A(1),(int32_t)A(2),A(3),A(4),A(5),A(6));break;
    case 0x4cce00u:
        FUN_004cce00(A(1),(uint8_t)A(2),(const char *)(uintptr_t)A(3),A(4),A(5),A(6),A(7));break;
    case 0x4c8070u:
        FUN_004c8070((const char *)(uintptr_t)A(1),(int32_t)A(2),(int32_t)A(3),
                    (int32_t)A(4),(int32_t)A(5),(int32_t)A(6));break;
    default:abort();
    }
    /* Native41A180 continuations reload consumed volatile GPRs before
       use, and redefine arithmetic flags with their own ADD/CMP/TEST.
       Only418140 full EAX,41B8F0 AL and sprite EAX are returned here.
       Parent keeps its real delayed caller cleanup across text calls. */
    f->r[4]=sp+4u+cleanup;
#undef A
}

/* Reuse the complete native models for the city status row and objective.
 * Neither entry consumes an implicit receiver or incoming volatile register. */
static uint32_t status_invoke(uint32_t target,const uint32_t *args,uint32_t count)
{
    uint32_t stack[128],i,sp=(uint32_t)(uintptr_t)(stack+96);
    struct restore_briefing_frame f={{0},2,dispatch};
    stack[96]=0;f.r[4]=sp;
    for(i=0;i<count;++i)stack[97+i]=args[i];
    dispatch(0,target,&f);
    if(f.r[4]!=sp+4u)abort();
    return f.r[0];
}

uint8_t FUN_004e2530(void)
{
    return (uint8_t)status_invoke(0x4e2530u,0,0);
}

void FUN_00537000(int32_t style,int32_t x,int32_t y,int32_t columns,int32_t immediate,int32_t variant)
{
    const uint32_t args[]={(uint32_t)style,(uint32_t)x,(uint32_t)y,
        (uint32_t)columns,(uint32_t)immediate,(uint32_t)variant};
    (void)status_invoke(0x537000u,args,6);
}

void FUN_0041a180(void)
{
    uint32_t stack[4096],sp=(uint32_t)(uintptr_t)(stack+4000);
    struct restore_briefing_frame f={{0},2,dispatch};
    /* No incoming argument, receiver or FPU data. Native local0 is assigned
       before its four-rating loop; all geometry/font/scroll state is global.
       No zero fill, fabricated message offsets or caller scratch values. */
    stack[4000]=0;f.r[4]=sp;
    dispatch(0,0x41a180u,&f);
    if(f.r[4]!=sp+4u)abort();
}
