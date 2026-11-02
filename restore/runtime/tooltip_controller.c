#include "tooltip_controller.h"
#include "text_selection.h"
#include "rich_message_text.h"
#include "menu_text_glyph.h"
#include "menu_training_render.h"
#include "menu_resource_render.h"
#include "campaign_map_input.h"
#include "map_validation.h"
#include "record_place.h"
#include "training_queries.h"
#include "actor_render.h"
#include <stdlib.h>
#include <string.h>
extern void FUN_004ccbb0(int,int,uint32_t,uint32_t,uint32_t,uint32_t);
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
void restore_tooltip_dispatch(uint32_t site,uint32_t target,struct restore_tooltip_frame *f)
{
    uint32_t sp=f->r[4];(void)site;
#define A(n) U(sp+4u*(n))
#define RAW(a) case 0x##a##u:restore_tooltip_C_##a(f);return
    switch(target){
    RAW(004c8750);
    RAW(004ca930);
    RAW(004ca980);
    RAW(004cabd0);
    RAW(004caf10);
    RAW(004c92a0);
    RAW(004c9340);
    RAW(004c95d0);
    RAW(004ca9a0);
    RAW(004cb0b0);
    RAW(004cb300);
    case 0x443000u:f->r[0]=FUN_00443000(A(1),A(2));break;
    case 0x443040u:f->r[0]=FUN_00443040(A(1),A(2));break;
    case 0x447790u:f->r[0]=(f->r[0]&0xffffff00u)|(uint32_t)(uint8_t)FUN_00447790((int32_t)A(1),(int32_t)A(2));break;
    case 0x4cf620u:f->r[0]=(uint32_t)FUN_004cf620(A(1),A(2),A(3),A(4));break;
    case 0x4cc110u:FUN_004cc110((int32_t)A(1),(int32_t)A(2));break;
    case 0x4c8070u:FUN_004c8070((const char *)(uintptr_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),(int32_t)A(5),(int32_t)A(6));break;
    case 0x4cbf30u:f->r[0]=(uint32_t)FUN_004cbf30((const uint8_t *)(uintptr_t)A(1),(int32_t)A(2));break;
    case 0x4cc770u:FUN_004cc770((const uint8_t *)(uintptr_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),(uint16_t)A(5));break;
    case 0x4ccbb0u:FUN_004ccbb0((int32_t)A(1),(int32_t)A(2),A(3),A(4),A(5),A(6));break;
    case 0x4cce00u:FUN_004cce00(A(1),(uint8_t)A(2),(const char *)(uintptr_t)A(3),A(4),A(5),A(6),A(7));break;
    case 0x4cd9d0u:restore_resource_render_raw(target,f->r,&f->flags);return;
    case 0x4cd680u:case 0x559fc0u:{
        struct restore_training_render_frame child;
        memcpy(child.r,f->r,sizeof(child.r));child.flags=f->flags;child.boundary=restore_training_render_dispatch;
        restore_training_render_dispatch(site,target,&child);
        memcpy(f->r,child.r,sizeof(f->r));f->flags=child.flags;return;
    }
    default:abort();
    }
    /* Actual cdecl cleanup is owned by each native caller. Only genuine
       full EAX queries and the resource gate's AL cross typed boundaries. */
    f->r[4]=sp+4u;
#undef RAW
#undef A
}
static uint32_t invoke(uint32_t entry,const uint32_t *args,uint32_t count)
{
    uint32_t stack[4096],sp=(uint32_t)(uintptr_t)(stack+4000),i;
    struct restore_tooltip_frame f={{0},2,restore_tooltip_dispatch};
    /* No incoming receiver/FPU or consumed caller scratch. Native producers
       initialize their byte flags and text buffers before relevant reads. */
    stack[4000]=0;f.r[4]=sp;
    for(i=0;i<count;++i)stack[4001+i]=args[i];
    restore_tooltip_dispatch(0,entry,&f);
    if(f.r[4]!=sp+4u)abort();
    return f.r[0];
}
uint32_t FUN_004c8750(void){return invoke(0x4c8750u,0,0);}
uint32_t FUN_004ca930(void){return invoke(0x4ca930u,0,0);}
void FUN_004ca980(void){(void)invoke(0x4ca980u,0,0);}
void FUN_004cabd0(void){(void)invoke(0x4cabd0u,0,0);}
void FUN_004caf10(void){(void)invoke(0x4caf10u,0,0);}
void FUN_004ca9a0(void){(void)invoke(0x4ca9a0u,0,0);}
void FUN_004cb0b0(void){(void)invoke(0x4cb0b0u,0,0);}
void FUN_004cb300(void){(void)invoke(0x4cb300u,0,0);}
int32_t FUN_004c92a0(void){return (int32_t)invoke(0x4c92a0u,0,0);}
int32_t FUN_004c9340(uint8_t *expanded)
{const uint32_t a[]={(uint32_t)(uintptr_t)expanded};return (int32_t)invoke(0x4c9340u,a,1);}
int32_t FUN_004c95d0(uint8_t *expanded)
{const uint32_t a[]={(uint32_t)(uintptr_t)expanded};return (int32_t)invoke(0x4c95d0u,a,1);}

