#include "menu_input_boundary.h"
#include "menu_input_entry.h"
#include "actor_panel_native_context.h"
#include "map_commit_complete.h"
#include "preview_parent_model.h"
#include "menu_004f96e0.h"
#include "menu_004f7e50.h"
#include "menu_inline_capacity.h"
#include "menu_full_input_numeric.h"
#include "menu_list_getter.h"
#include "menu_record_queries.h"
#include "menu_cursor_queries.h"
#include "map_record_refresh.h"
#include "map_record_decay.h"
#include "map_validation.h"
#include "map_view_commit.h"
#include "map_file_load.h"
#include "tooltip_timing.h"
#include "menu_timers.h"
#include "menu_table_queries.h"
#include "load_save_menu.h"
#include "display_confirmation.h"
#include "tool_selector_input.h"
#include "menu_tool_return.h"
#include "menu_list_scroll.h"
#include "menu_group_selection.h"
#include "menu_tail_ui.h"
#include "campaign_score_numeric.h"
#include "monument_progress.h"
#include "monument_base_queries.h"
#include "monument_complex_queries.h"
#include "nearest_rect.h"
#include "editor_brush_validation.h"
#include "three_way_button.h"
#include "order_input.h"
#include "list_panel_input.h"
#include "order_branch.h"
#include "video_open.h"
#include "pyramid_summary.h"
#include "minimap_input.h"
#include "menu_prepare_integer.h"
#include "menu_monument_search.h"
#include "pending_message.h"
#include "record_place.h"
#include "menu_building_kind.h"
#include "menu_resource_neighbors.h"
#include "order_actions_finish.h"
#include "building_options.h"
#include "resource_availability.h"
#include "route_seed.h"
#include "mission_unlock.h"
#include "menu_actor_message.h"
#include "menu_window_cursor_consumer.h"
#include "campaign_menu.h"
#include "campaign_entry.h"
#include "campaign_actions.h"
#include "campaign_map_input.h"
#include "map_menu_scroll.h"
#include "mission_menu.h"
#include "family_menu.h"
#include "high_scores_menu.h"
#include "high_score_submission.h"
#include "message_panel_input.h"
#include "selection_popup.h"
#include "camera_pan.h"
#include "menu_keyboard_modal.h"
#include "dropdown_menu.h"
#include "video_player.h"
#include <windows.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
void FUN_00402bb0(void);
uint8_t **FUN_0041e100(void);uint8_t *FUN_0041e120(void);uint8_t *FUN_0041e130(void);
uint32_t FUN_0041e140(void);char FUN_0041e170(void);uint8_t **FUN_0041e190(void);int32_t FUN_0041f210(void);
void FUN_004256a0(int32_t,int32_t,int16_t *,int32_t);void FUN_00426530(void);
void FUN_004cc500(int32_t);uint32_t FUN_005165b0(void);void FUN_0051f780(void);
/* The guarded physical entry links only after all 103 native targets have
 * genuine C providers, including the complete 405E90/470990 construction graph. */
const uint32_t restore_menu_input_boundary_closed_103=RESTORE_MENU_INPUT_CLOSED_TAG;
typedef char compact_frame_prefix[offsetof(struct restore_menu_input_frame,flags)==32?1:-1];
static __declspec(noreturn) void unsupported(uint32_t site,uint32_t target,const char *reason)
{fprintf(stderr,"menu input: site=%08lx target=%08lx %s\n",(unsigned long)site,(unsigned long)target,reason);fflush(stderr);ExitProcess(0x4d83);}
static uint64_t ftol_raw80(void)
{uint8_t raw[10];__asm{lea eax,raw}__asm{fstp tbyte ptr[eax]}return restore_menu_input_trunc80(raw);}
void restore_menu_input_boundary_dispatch(uint32_t site,uint32_t target,struct restore_menu_input_frame *f)
{
 uint32_t sp,cleanup=0;uint64_t result;
 if(!f)unsupported(site,target,"missing frame");
 /* Seven reviewed route methods, repaired delete and exact inline allocator
    sites live here; their RET4/RET12 cleanup is already complete on return. */
 if(restore_menu_inline_capacity_boundary(site,target,f))return;
 sp=f->r[4];
#define A(n) (*(uint32_t *)(uintptr_t)(sp+(uint32_t)(n)*4u))
 switch(target){
 case 0x480180:case 0x4802c0:case 0x480460:case 0x4806a0:{
  struct restore_resource_neighbors_frame neighbors;
  memcpy(neighbors.r,f->r,sizeof(neighbors.r));neighbors.flags=f->flags;
  neighbors.boundary=restore_resource_neighbors_dispatch;
  switch(target){
  case 0x480180:restore_resource_C_00480180(&neighbors);break;
  case 0x4802c0:restore_resource_C_004802c0(&neighbors);break;
  case 0x480460:restore_resource_C_00480460(&neighbors);break;
  default:restore_resource_C_004806a0(&neighbors);break;
  }
  memcpy(f->r,neighbors.r,sizeof(f->r));f->flags=neighbors.flags;return;
 }
 case 0x53c100:{
  struct restore_actor_message_frame actor;
  memcpy(actor.r,f->r,sizeof(actor.r));actor.flags=f->flags;
  actor.boundary=restore_actor_message_dispatch;
  restore_actor_message_C_0053c100(&actor);
  memcpy(f->r,actor.r,sizeof(f->r));f->flags=actor.flags;return;
 }
 case 0x454190:case 0x45d570:case 0x475b50:case 0x4b7590:case 0x50f0f0:{
  struct restore_prepare_integer_frame prepared;
  memcpy(prepared.r,f->r,sizeof(prepared.r));prepared.flags=f->flags;
  prepared.boundary=restore_prepare_integer_dispatch;
  switch(target){
  case 0x454190:restore_prepare_C_00454190(&prepared);break;
  case 0x45d570:restore_prepare_C_0045d570(&prepared);break;
  case 0x475b50:restore_prepare_C_00475b50(&prepared);break;
  case 0x4b7590:restore_prepare_C_004b7590(&prepared);break;
  default:restore_prepare_C_0050f0f0(&prepared);break;
  }
  memcpy(f->r,prepared.r,sizeof(f->r));f->flags=prepared.flags;return;
 }
 case 0x401540:f->r[0]=(uint32_t)FUN_00401540(A(1),(int32_t *)(uintptr_t)A(2));cleanup=8;break;
 case 0x402720:FUN_00402720();break;
 case 0x4029d0:f->r[0]=FUN_004029d0();break;
 case 0x402bb0:FUN_00402bb0();break;
 case 0x402de0:FUN_00402de0();break;
 case 0x403e00:f->r[0]=(uint32_t)FUN_00403e00();break;
 case 0x405ba0:f->r[0]=FUN_00405ba0();break;
 case 0x405c20:FUN_00405c20();break;
 case 0x405d90:FUN_00405d90();break;
 case 0x405e90:FUN_00405e90();break;
 case 0x405f80:FUN_00405f80();break;
 case 0x406190:FUN_00406190();break;
 case 0x4061e0:f->r[0]=FUN_004061e0();break;
 case 0x406250:FUN_00406250();break;
 case 0x406720:FUN_00406720();break;
 case 0x407870:f->r[0]=(uint32_t)FUN_00407870();break;
 case 0x407a10:FUN_00407a10((int32_t *)(uintptr_t)A(1),(int32_t *)(uintptr_t)A(2));break;
 case 0x4072c0:FUN_004072c0();break;
 case 0x407620:FUN_00407620((int32_t)A(1),(int32_t)A(2));break;
 case 0x409520:FUN_00409520();break;
 case 0x4099b0:FUN_004099b0();break;
 case 0x409e40:f->r[0]=FUN_00409e40();break;
 case 0x40ba40:FUN_0040ba40();break;
 case 0x40b630:f->r[0]=FUN_0040b630();break;
 case 0x40b6d0:f->r[0]=FUN_0040b6d0();break;
 case 0x40b790:f->r[0]=FUN_0040b790();break;
 case 0x40b830:FUN_0040b830();break;
 case 0x40c440:FUN_0040c440();break;
 case 0x40c570:FUN_0040c570();break;
 case 0x40d1b0:f->r[0]=FUN_0040d1b0();break;
 case 0x40d940:f->r[0]=(uint32_t)restore_monument_base_0040d940((void *)(uintptr_t)f->r[1],A(1),(int32_t *)(uintptr_t)A(2));cleanup=8;break;
 case 0x413420:(void)FUN_00413420((RestoreVideoPlayer *)(uintptr_t)f->r[1]);break;
 /* Native caller consumes the same mapped incoming-SP minus32 scratch.
    Its next CALL4CF660 replaces EAX before use; nonvolatile frame stays intact. */
 case 0x417b40:restore_mission_unlock_body((uint8_t *)(uintptr_t)(sp-32u));break;
 case 0x417050:f->r[0]=(uint32_t)restore_monument_base_00417050((void *)(uintptr_t)f->r[1],A(1),(int32_t *)(uintptr_t)A(2));cleanup=8;break;
 case 0x418140:f->r[0]=(uint32_t)FUN_00418140();break;
 case 0x41d310:FUN_0041d310();break;
 case 0x418220:FUN_00418220();break;
 case 0x418640:FUN_00418640();break;
 case 0x418fe0:FUN_00418fe0();break;
 case 0x419090:FUN_00419090();break;
 case 0x41aa20:FUN_0041aa20(A(1),A(2),(int32_t)A(3));break;
 case 0x41aa60:FUN_0041aa60();break;
 case 0x41b8f0:f->r[0]=(f->r[0]&0xffffff00u)|restore_menu_list_can_open(f->r[1]);break;
 case 0x41bb30:f->r[0]=FUN_0041bb30();break;
 case 0x41e100:f->r[0]=(uint32_t)(uintptr_t)FUN_0041e100();break;
 case 0x41e120:f->r[0]=(uint32_t)(uintptr_t)FUN_0041e120();break;
 case 0x41e130:f->r[0]=(uint32_t)(uintptr_t)FUN_0041e130();break;
 case 0x41e140:f->r[0]=FUN_0041e140();break;
 case 0x41e170:f->r[0]=(uint8_t)FUN_0041e170();break; /* Native full-EAX enum3/4. */
 case 0x41e190:f->r[0]=(uint32_t)(uintptr_t)FUN_0041e190();break;
 case 0x41f210:f->r[0]=(uint32_t)FUN_0041f210();break;
 case 0x41f4d0:f->r[0]=FUN_0041f4d0();break;
 case 0x41f650:FUN_0041f650();break;
 case 0x4256a0:FUN_004256a0((int32_t)A(1),(int32_t)A(2),(int16_t *)(uintptr_t)A(3),(int32_t)A(4));break;
 case 0x425840:f->r[0]=(f->r[0]&0xffffff00u)|FUN_00425840();break;
 case 0x425990:
  restore_actor_panel_native_small(site,A(3),(int32_t)A(4));
  f->r[0]=(uint32_t)FUN_00425990((int32_t)A(1),(int32_t)A(2),(const void *)(uintptr_t)A(3),(int32_t)A(4));break;
 case 0x425bc0:FUN_00425bc0((int32_t)A(1),(int32_t)A(2),(const void *)(uintptr_t)A(3),(int32_t)A(4),(int32_t)A(5));break;
 case 0x425c40:f->r[0]=(uint32_t)FUN_00425c40((int32_t)A(1),(int32_t)A(2),(RestoreSquareButton *)(uintptr_t)A(3),(int32_t)A(4));break;
 case 0x425e80:FUN_00425e80((int32_t)A(1),(int32_t)A(2),(const RestoreSquareButton *)(uintptr_t)A(3),(int32_t)A(4),(int32_t)A(5));break;
 case 0x425f20:f->r[0]=(uint32_t)FUN_00425f20((RestoreDropdownBar *)(uintptr_t)A(1),(int32_t)A(2));break;
 case 0x426030:f->r[0]=(uint32_t)FUN_00426030((RestoreDropdownBar *)(uintptr_t)A(1));break;
 case 0x426090:FUN_00426090();break;
 case 0x426100:f->r[0]=(uint32_t)FUN_00426100((const RestoreDropdownBar *)(uintptr_t)A(1),(int32_t)A(2));break;
 case 0x4261d0:{
  uint32_t previous=restore_actor_panel_native_buttons_begin(site,A(3),A(4));
  f->r[0]=(uint32_t)FUN_004261d0((int32_t)A(1),(int32_t)A(2),(RestoreMenuButton *)(uintptr_t)A(3),(int32_t)A(4));
  restore_actor_panel_native_buttons_end(previous);break;
 }
 case 0x426450:f->r[0]=(uint32_t)FUN_00426450((int32_t)A(1),(int32_t)A(2),(const RestoreMenuButton *)(uintptr_t)A(3),(int32_t)A(4));break;
 case 0x426530:FUN_00426530();break;
 case 0x442500:FUN_00442500();break;
 case 0x4426c0:FUN_004426c0();break;
 case 0x442730:FUN_00442730((int32_t)A(1));break;
 case 0x442a40:FUN_00442a40(A(1));break;
 case 0x442ef0:FUN_00442ef0();break;
 case 0x442f80:FUN_00442f80();break;
 case 0x444920:f->r[0]=(uint32_t)FUN_00444920((int32_t)A(1));break;
 case 0x448cf0:FUN_00448cf0();break;
 case 0x4483f0:f->r[0]=FUN_004483f0();break;
 case 0x4c57d0:f->r[0]=(uint32_t)FUN_004c57d0();break;
 case 0x4c5a40:FUN_004c5a40();break;
 case 0x4c7770:FUN_004c7770();break;
 case 0x4cb6a0:FUN_004cb6a0();break;
 case 0x4cb970:FUN_004cb970();break;
 case 0x4cc500:FUN_004cc500((int32_t)A(1));break;
 case 0x4cde40:f->r[0]=(uint32_t)FUN_004cde40((int32_t)A(1),(int32_t)A(2));break;
 case 0x4cde60:f->r[0]=(uint32_t)FUN_004cde60((int32_t)A(1),(int32_t)A(2));break;
 case 0x4cde80:f->r[0]=(uint32_t)FUN_004cde80((int32_t)A(1),(int32_t)A(2));break;
 case 0x4cf1c0:FUN_004cf1c0();break;
 case 0x4cf660:f->r[0]=(uint32_t)FUN_004cf660();break;
 case 0x4d6740:{
  uint32_t previous=restore_actor_panel_native_hover_begin(site,A(3),A(4));
  f->r[0]=(uint32_t)FUN_004d6740((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),A(4),(int32_t)A(5));
  restore_actor_panel_native_hover_end(previous);break;
 }
 case 0x4ddde0:FUN_004ddde0((int32_t)A(1));break;
 case 0x4df040:f->r[0]=(uint32_t)FUN_004df040(A(1),(int32_t *)(uintptr_t)A(2));break;
 case 0x4e2ab0:FUN_004e2ab0();break;
 case 0x4e1e10:FUN_004e1e10();break;
 case 0x4e6710:f->r[0]=(uint32_t)restore_monument_base_004e6710((void *)(uintptr_t)f->r[1],A(1),(int32_t *)(uintptr_t)A(2));cleanup=8;break;
 case 0x4ea880:f->r[0]=FUN_004ea880();break;
 case 0x4ea8d0:f->r[0]=(uint32_t)(uintptr_t)FUN_004ea8d0();break;
 /* The translated callee consumes its own virtual return slot. */
 case 0x4f7e50:restore_C_004f7e50(f);return;
 case 0x4f96e0:restore_C_004f96e0(f);return;
 case 0x4f1310:f->r[0]=FUN_004f1310(A(1),(int32_t *)(uintptr_t)A(2));break;
 case 0x4f5050:f->r[0]=FUN_004f5050(A(1),(uint8_t)A(2));break;
 case 0x4f5d90:f->r[0]=FUN_004f5d90(A(1));break;
 case 0x4f6000:f->r[0]=(uint32_t)FUN_004f6000(A(1));break;
 case 0x4f8960:f->r[0]=FUN_004f8960();break;
 case 0x5013a0:f->r[0]=FUN_005013a0(A(1));break;
 case 0x502780:f->r[0]=FUN_00502780();break;
 case 0x515c60:FUN_00515c60();break;
 case 0x515130:FUN_00515130((uint32_t *)(uintptr_t)A(1));break;
 case 0x516020:FUN_00516020();break;
 case 0x5165b0:f->r[0]=FUN_005165b0();break;
 case 0x5168e0:f->r[0]=FUN_005168e0();break;
 case 0x51f780:FUN_0051f780();break; /* Authenticated native RET body. */
 case 0x53b690:FUN_0053b690((int32_t)A(1));break;
 case 0x539170:FUN_00539170();break;
 case 0x538dc0:f->r[0]=FUN_00538dc0((const char *)(uintptr_t)A(1),A(2),A(3),A(4),A(5));break;
 case 0x542ad0:f->r[0]=(uint32_t)FUN_00542ad0(A(1),(int32_t *)(uintptr_t)A(2));break;
 case 0x545740:f->r[0]=(uint32_t)restore_monument_progress_00545740((void *)(uintptr_t)f->r[1],A(1),(int32_t *)(uintptr_t)A(2));cleanup=8;break;
 case 0x54f470:f->r[0]=(uint32_t)restore_monument_progress_0054f470((void *)(uintptr_t)f->r[1],A(1),(int32_t *)(uintptr_t)A(2));cleanup=8;break;
 case 0x555e80:f->r[0]=(uint32_t)restore_monument_progress_00555e80((void *)(uintptr_t)f->r[1],A(1),(int32_t *)(uintptr_t)A(2));cleanup=8;break;
 case 0x558be0:f->r[0]=(uint32_t)restore_monument_progress_00558be0((void *)(uintptr_t)f->r[1],A(1),(int32_t *)(uintptr_t)A(2));cleanup=8;break;
 case 0x55bb90:f->r[0]=FUN_0055bb90();break;
 case 0x5596d0:FUN_005596d0();break;
 case 0x56133c:result=ftol_raw80();f->r[0]=(uint32_t)result;f->r[2]=(uint32_t)(result>>32);break;
 case 0x56f22c:f->r[0]=restore_display_confirmation_clock();break;
 default:unsupported(site,target,"unverified endpoint");
 }
 f->r[4]=sp+4u+cleanup; /* Typed C calls retain each native RET0/RET8 contract. */
#undef A
}
