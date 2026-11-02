/* Actual100-target binding over the unchanged, authenticated2160-instruction
 * restore_map_commit_C_004728a0 model. No original code-address dispatch.
 * The separate470990 implementation is a required link prerequisite. */
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

void restore_map_commit_complete_dispatch(uint32_t site,uint32_t target,struct restore_map_commit_frame *f)
{
 uint32_t sp=f->r[4];(void)site;
#define A(n) MI_U32(sp+(n)*4u)
 switch(target){
 case 0x401400u:f->r[0]=restore_placement_check_00401400(f->r[1],A(1));f->r[4]=sp+8u;return;
 case 0x401430u:f->r[0]=restore_placement_existing_00401430(f->r[1],f->r[0]);break;
 case 0x40d850u:f->r[0]=restore_placement_check_0040d850(f->r[1],A(1));f->r[4]=sp+8u;return;
 case 0x40d880u:f->r[0]=restore_placement_existing_0040d880(f->r[1],f->r[0]);break;
 case 0x40f860u:FUN_0040f860((void *)(uintptr_t)A(1),(void *)(uintptr_t)A(2),(int32_t)A(3));break;
 case 0x40f920u:FUN_0040f920((void *)(uintptr_t)A(1),(int32_t)A(2),(uint8_t)A(3));break;
 case 0x416f70u:f->r[0]=restore_placement_check_00416f70(f->r[1],A(1));f->r[4]=sp+8u;return;
 case 0x416fa0u:f->r[0]=restore_placement_existing_00416fa0(f->r[1],f->r[0]);break;
 case 0x440700u:f->r[0]=FUN_00440700(A(1),A(2),A(3),(int32_t)A(4),(int32_t)A(5));break;
 case 0x440850u:f->r[0]=FUN_00440850(A(1),A(2),A(3),(int32_t)A(4),(int32_t)A(5));break;
 case 0x440b10u:f->r[0]=FUN_00440b10(A(1),A(2),A(3),(int32_t)A(4),(int32_t)A(5));break;
 case 0x440ed0u:f->r[0]=FUN_00440ed0(A(1),A(2),A(3),(int32_t)A(4),(int32_t)A(5));break;
 case 0x457ac0u:f->r[0]=FUN_00457ac0((int32_t)A(1),(int32_t)A(2));break;
 case 0x459fa0u:f->r[0]=FUN_00459fa0();break;
 case 0x45b4d0u:f->r[0]=(uint32_t)FUN_0045b4d0((int32_t)A(1),(int32_t)A(2),(int32_t)A(3));break;
 case 0x46a400u:f->r[0]=(uint32_t)restore_building_allocate_0046a400(A(1),A(2),(int32_t)A(3),(int32_t)A(4),(int32_t)A(5),(int32_t)A(6));break;
 case 0x46fde0u:f->r[0]=FUN_0046fde0(A(1));break;
 case 0x470900u:
  /* Housing checks protected entry cells inside474130, for preview and commit.
   * Other construction tools retain their whole-footprint entry check. */
  f->r[0]=site==0x472eaeu?0u:FUN_00470900((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4));break;
 case 0x470990u:restore_placement_create_00470990(A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),(int32_t)A(5),(int32_t)A(6),A(7),A(8));break;
 case 0x474130u:restore_building_rect_00474130(A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),(int32_t)A(5));break;
 case 0x4743f0u:FUN_004743f0((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),(int32_t)A(5));break;
 case 0x474f50u:FUN_00474f50((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),(int32_t)A(5));break;
 case 0x475090u:FUN_00475090((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),(int32_t)A(5));break;
 case 0x475220u:FUN_00475220((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),(int32_t)A(5),A(6));break;
 case 0x4754a0u:FUN_004754a0(A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),(int32_t)A(5));break;
 case 0x4759e0u:FUN_004759e0(A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),(int32_t)A(5));break;
 case 0x475b50u:f->r[0]=FUN_00475b50(A(1));break;
 case 0x47bae0u:FUN_0047bae0((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),A(5));break;
 case 0x47c880u:f->r[0]=(uint32_t)FUN_0047c880((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),(int32_t)A(5),(int32_t)A(6),(int32_t)A(7));break;
 case 0x47cc70u:f->r[0]=(uint32_t)FUN_0047cc70((int32_t)A(1),(int32_t)A(2),A(3),(uint8_t)A(4));break;
 case 0x47d060u:f->r[0]=(uint32_t)FUN_0047d060((int32_t)A(1),(int32_t)A(2),A(3));break;
 case 0x47f530u:FUN_0047f530((int32_t)A(1),(int32_t)A(2),(int32_t)A(3));break;
 case 0x482650u:f->r[0]=FUN_00482650((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),A(5));break;
 case 0x482720u:f->r[0]=FUN_00482720((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),restore_placement_rule(A(5)));break;
 case 0x4828e0u:f->r[0]=FUN_004828e0((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),A(5));break;
 case 0x4829b0u:f->r[0]=FUN_004829b0((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),A(5),(int32_t)A(6));break;
 case 0x485100u:f->r[0]=FUN_00485100((int32_t)A(1));break;
 case 0x485200u:FUN_00485200();break;
 case 0x485830u:FUN_00485830();break;
 case 0x486020u:FUN_00486020();break;
 case 0x4b5830u:f->r[0]=FUN_004b5830((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4));break;
 case 0x4b9ee0u:FUN_004b9ee0((int32_t)A(1),(int32_t)A(2));break;
 case 0x4bff60u:f->r[0]=FUN_004bff60((int32_t)A(1),(int32_t)A(2),(int32_t)A(3));break;
 case 0x4c3730u:f->r[0]=(uint32_t)FUN_004c3730((int32_t)A(1),(int32_t)A(2));break;
 case 0x4c52d0u:f->r[0]=(f->r[0]&0xffffff00u)|FUN_004c52d0((int32_t)A(1),A(2));break;
 case 0x4def00u:f->r[0]=(f->r[0]&0xffffff00u)|FUN_004def00((int32_t)A(1));break;
 case 0x4e2970u:FUN_004e2970((int32_t)A(1));break;
 case 0x4e2b10u:FUN_004e2b10((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),A(4));break;
 case 0x4e2d30u:FUN_004e2d30((int32_t)A(1));break;
 case 0x4e2e20u:FUN_004e2e20((int32_t)A(1));break;
 case 0x4e3100u:FUN_004e3100((int32_t)A(1));break;
 case 0x4e3140u:FUN_004e3140((int32_t)A(1));break;
 case 0x4e3180u:FUN_004e3180((int32_t)A(1));break;
 case 0x4e31c0u:FUN_004e31c0((int32_t)A(1));break;
 case 0x4e3200u:FUN_004e3200((int32_t)A(1));break;
 case 0x4e3240u:FUN_004e3240((int32_t)A(1));break;
 case 0x4e3280u:FUN_004e3280((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4));break;
 case 0x4e32e0u:FUN_004e32e0((int32_t)A(1),(int32_t)A(2),(int32_t)A(3));break;
 case 0x4e3330u:FUN_004e3330((int32_t)A(1));break;
 case 0x4e33a0u:FUN_004e33a0((int32_t)A(1));break;
 case 0x4e3460u:FUN_004e3460((int32_t)A(1));break;
 case 0x4e3520u:FUN_004e3520((int32_t)A(1));break;
 case 0x4e35e0u:FUN_004e35e0((int32_t)A(1));break;
 case 0x4e36a0u:FUN_004e36a0((int32_t)A(1));break;
 case 0x4e3760u:FUN_004e3760((int32_t)A(1));break;
 case 0x4e39d0u:FUN_004e39d0((int32_t)A(1));break;
 case 0x4e3aa0u:FUN_004e3aa0((int32_t)A(1));break;
 case 0x4e3cf0u:FUN_004e3cf0((int32_t)A(1));break;
 case 0x4e3da0u:FUN_004e3da0((int32_t)A(1));break;
 case 0x4e3f60u:FUN_004e3f60((int32_t)A(1));break;
 case 0x4e4120u:FUN_004e4120((int32_t)A(1));break;
 case 0x4e42d0u:FUN_004e42d0((int32_t)A(1));break;
 case 0x4e4490u:FUN_004e4490((int32_t)A(1));break;
 case 0x4e4550u:FUN_004e4550((int32_t)A(1));break;
 case 0x4e47d0u:FUN_004e47d0((int32_t)A(1));break;
 case 0x4e4e10u:FUN_004e4e10((int32_t)A(1));break;
 case 0x4e4ed0u:FUN_004e4ed0((int32_t)A(1));break;
 case 0x4e5070u:FUN_004e5070((int32_t)A(1));break;
 case 0x4e59c0u:f->r[0]=(uint32_t)FUN_004e59c0();break;
 case 0x4e65f0u:f->r[0]=restore_placement_check_004e65f0(f->r[1],A(1));f->r[4]=sp+8u;return;
 case 0x4e6650u:f->r[0]=restore_placement_existing_004e6650(f->r[1],f->r[0]);break;
 case 0x4e9680u:FUN_004e9680((int32_t)A(1),(int32_t)A(2));break;
 case 0x4ecec0u:f->r[0]=FUN_004ecec0((int32_t)A(1),(int32_t)A(2),(uint32_t *)(uintptr_t)A(3));break;
 case 0x4f61e0u:FUN_004f61e0();break;
 case 0x516f90u:FUN_00516f90((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),(uint8_t)A(5));break;
 case 0x517370u:FUN_00517370();break;
 case 0x5173a0u:FUN_005173a0((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4));break;
 case 0x517660u:FUN_00517660();break;
 case 0x5178e0u:FUN_005178e0();break;
 case 0x51f780u:FUN_0051f780();break;
 case 0x541ae0u:f->r[0]=FUN_00541ae0(A(1));break;
 case 0x542ee0u:f->r[0]=restore_monument_ready_00542ee0(f->r[0]);break;
 case 0x5456c0u:f->r[0]=restore_placement_check_005456c0(f->r[1],A(1));f->r[4]=sp+8u;return;
 case 0x545700u:f->r[0]=restore_placement_existing_00545700(f->r[1],f->r[0]);break;
 case 0x54f3f0u:f->r[0]=restore_placement_check_0054f3f0(f->r[1],A(1));f->r[4]=sp+8u;return;
 case 0x54f430u:f->r[0]=restore_placement_existing_0054f430(f->r[1],f->r[0]);break;
 case 0x555e00u:f->r[0]=restore_placement_check_00555e00(f->r[1],A(1));f->r[4]=sp+8u;return;
 case 0x555e40u:f->r[0]=restore_placement_existing_00555e40(f->r[1],f->r[0]);break;
 case 0x558b60u:f->r[0]=restore_placement_check_00558b60(f->r[1],A(1));f->r[4]=sp+8u;return;
 case 0x558ba0u:f->r[0]=restore_placement_existing_00558ba0(f->r[1],f->r[0]);break;
 default:abort();
 }
#undef A
 f->r[4]=sp+4u;
}

void FUN_004728a0(int32_t player,int32_t mode,int32_t x1,int32_t y1,int32_t x2,int32_t y2,int32_t tool)
{
 uint32_t stack[256],i,sp;struct restore_map_commit_frame f={{0},2,restore_map_commit_complete_dispatch};
 for(i=0;i<256;++i)stack[i]=0x4728ca11u;
 stack[241]=(uint32_t)player;stack[242]=(uint32_t)mode;stack[243]=(uint32_t)x1;stack[244]=(uint32_t)y1;
 stack[245]=(uint32_t)x2;stack[246]=(uint32_t)y2;stack[247]=(uint32_t)tool;sp=(uint32_t)(uintptr_t)(stack+240);f.r[4]=sp;
 restore_map_commit_C_004728a0(&f);
 if(f.r[4]!=sp+4u||f.r[3]||f.r[5]||f.r[6]||f.r[7])abort();
 for(i=0;i<16;++i)if(stack[i]!=0x4728ca11u)abort();for(i=248;i<256;++i)if(stack[i]!=0x4728ca11u)abort();
}
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
