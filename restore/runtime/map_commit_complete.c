/* Production4728A0 routes to its ordinary C owner; the original model is retained as inactive evidence. */
#include "map_commit_complete.h"
#include "drag_leaves.h"
#include "editor_brush_validation.h"
#include "map_commit_state.h"
#include "map_commit_warnings.h"
#include "map_commit_costs.h"
#include "map_commit_selection.h"
#include "map_commit_connect.h"
#include "map_commit_proximity.h"
#include "map_commit_resource_warnings.h"
#include "map_commit_resource_gate.h"
#include "map_commit_town_warning.h"
#include "menu_prepare_integer.h"
#include "terrain_redraw.h"
#include "footprint.h"
#include "actor_predicate.h"
#include "region_validation.h"
#include "animation_state.h"
#include "pending_message.h"
#include <stdio.h>
#include <stdlib.h>

#include "placement_context.h"
#include "placement_support.h"
#include "building_allocator.h"
#include "building_rect_commit.h"
#include "small_constructor_model.h"
#include "construction_controller.h"
#include "map_commit_route_actions.h"
#include "map_commit_placement.h"
#include "paving_region_model.h"
#include "city_footprint_validation.h"
#include "map_commit_population.h"
#include "monument_placement_queries.h"
#include "remaining_warnings.h"
#include "map_commit_refresh.h"
#include "map_commit_fields.h"
extern code *restore_placement_rule(uint32_t token);
extern void FUN_0047bae0(int32_t,int32_t,int32_t,int32_t,uint32_t);
extern void FUN_0051f780(void);
/* Exact original47D350 body is33c0c3: no arguments are consumed. */
extern uint32_t FUN_0047d350(void);
/* Supplied by the separate complete470990 construction phase. */
extern void restore_placement_create_00470990(uint32_t,int32_t,int32_t,int32_t,int32_t,int32_t,uint32_t,uint32_t);
#define MI_U32(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define MI_B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))

void FUN_004728a0(int32_t player,int32_t mode,int32_t x1,int32_t y1,int32_t x2,int32_t y2,int32_t tool)
{restore_map_commit((uint32_t)player,mode,x1,y1,x2,y2,tool);}
void FUN_00405e90(void)
{
 int32_t tool,player,mode,x1,y1,x2,y2;uint32_t selected;
 if(MI_U32(0x605a3cu)==1u)FUN_005178b0();
 tool=(int32_t)MI_U32(0x6059e8u);
 if(tool==123||tool==124||tool==129){FUN_004795e0();MI_U32(0x6059e4u)=0;return;}
 if(!MI_U32(0x6059e4u))return;
 selected=MI_U32(0xbca2c4u);MI_U32(0x6059e4u)=0;
 if(!selected){if(!MI_B(0x605798u))return;selected=MI_U32(0xbca2e4u);MI_U32(0xbca2c4u)=selected;}
 if(tool>0){(void)FUN_0047d350();tool=(int32_t)MI_U32(0x6059e8u);}
 y2=(int32_t)MI_U32(0x6059d4u);y1=(int32_t)MI_U32(0x6059d8u);x2=(int32_t)MI_U32(0x6059dcu);x1=(int32_t)MI_U32(0x6059e0u);
 mode=(int32_t)MI_U32(0xbca2e0u);player=(int32_t)MI_U32(0xeaac28u);
 FUN_004728a0(player,mode,x1,y1,x2,y2,tool);
}
