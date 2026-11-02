#include "menu_building_overlay.h"
#include "chief_advisor_render.h"
#include "population_advisor_render.h"
#include "ratings_advisor_render.h"
#include "map_hover_geometry.h"
#include "menu_text_glyph.h"
#include "sprite_render.h"
#include "editor_brush_validation.h"
#include "camera_navigation.h"
#include "camera_bounds.h"
#include "viewport_controller.h"
#include <stdlib.h>
#include "menu_training_render.h"
#include "menu_resource_render.h"
#include "menu_actor_message.h"
#include "pyramid_summary.h"
#include "placement_queries.h"
#include "monument_base_queries.h"
#include "monument_complex_queries.h"
#include "campaign_score_numeric.h"
#include "city_updates.h"
#include "menu_prepare_integer.h"
#include "menu_resource_neighbors.h"
#include "command_migration.h"
#include "actor_predicate.h"
#include "sound_initialization.h"
#include "dropdown_menu.h"
#include "town_menu.h"
#include "training_queries.h"
#include "terrain_diamond.h"
#include "menu_tail_ui.h"
#include "video_player.h"
#include "sound_streams.h"
#include "campaign_score.h"
#include "input_monument_summary.h"
#include "campaign_map_input.h"
#include "menu_table_queries.h"
#include "message_panel.h"
#include "actor_render.h"
#include "legacy_format.h"
#include "monument_progress.h"
#include "event_publisher.h"
#include "remaining_selection_actions.h"
#include "redraw_dispatch_model.h"
#include "town_menu.h"
#include "monument_unlink.h"
#include "map_commit_population.h"
#include "flood_simulation.h"
extern int32_t FUN_0040cc40(int32_t);
extern void FUN_004cd1a0(uint32_t,int16_t *,uint32_t,int32_t,int32_t);
extern void FUN_00448330(int32_t,char *,int32_t);
extern void FUN_004ccb30(uint32_t,uint32_t,int32_t,int32_t,uint32_t,uint32_t,uint32_t);
extern void FUN_004ccfd0(uint32_t,uint32_t,int32_t,int32_t,uint32_t,uint32_t,uint32_t);
extern void FUN_004ccbb0(uint32_t,uint32_t,int32_t,int32_t,uint32_t,uint32_t);
extern unsigned char FUN_00413470(RestoreVideoPlayer *player);
extern void FUN_004138d0(void);
extern void FUN_00414c40(void);
extern int FUN_00417e80(void);
extern int FUN_00417eb0(void);
extern int FUN_00417ee0(void);
extern void FUN_00417f10(void);
extern void FUN_004180c0(void);
extern void FUN_00418f70(void);
extern void FUN_00419140(void);
extern void FUN_004191f0(void);
extern void FUN_004194c0(void);
extern void FUN_00419db0(void);
extern void FUN_0041a180(void);
extern void FUN_0041a850(void);
extern void FUN_0041aaa0(void);
extern uint8_t FUN_0041b8f0(uint32_t self);
extern uint FUN_0041bb30(void);
extern void FUN_0041be10(void);
extern void FUN_0041c2d0(void);
extern void FUN_0041d210(void);
extern undefined ** FUN_0041e100(void);
extern undefined * FUN_0041e130(void);
extern undefined4 FUN_0041e140(void);
extern char FUN_0041e170(void);
extern void FUN_0041e5c0(void);
extern void FUN_0041f220(void);
extern void FUN_00425430(void);
extern void FUN_00425570(void);
extern void FUN_00425640(void);
extern void FUN_0043e1a0(void);
extern void FUN_00448640(void);
extern void FUN_0044aaf0(void);
extern void FUN_004c5660(void);
extern void FUN_004c5cc0(void);
extern void FUN_004c7340(void);
extern void FUN_004caf10(void);
extern void FUN_004cb4d0(void);
extern void FUN_004cb5a0(void);
extern void FUN_004e1320(void);
extern void FUN_004e1870(void);
extern void FUN_004e1a50(void);
extern void FUN_004e61f0(void);
extern int32_t FUN_005027b0(void);
extern undefined4 FUN_005165b0(void);
extern void FUN_00516630(void);
extern void FUN_00516720(void);
extern void FUN_00516bb0(void);
extern void FUN_0051e990(void);
extern void FUN_0051ec90(void);
extern void FUN_0051ed30(int param_1,undefined4 param_2);
extern void FUN_0051efb0(int param_1);
extern void FUN_0051f790(void);
extern void FUN_0051fd60(void);
extern void FUN_0051ffd0(void);
extern void FUN_00520000(void);
extern void FUN_005208c0(void);
extern void FUN_005208f0(void);
extern void FUN_00520ab0(int param_1);
extern void FUN_00520af0(void);
extern void FUN_00520b60(void);
extern void FUN_00520b90(void);
extern void FUN_00520d00(void);
extern void FUN_00520e00(void);
extern void FUN_00520ea0(void);
extern void FUN_00520f40(void);
extern void FUN_00521060(void);
extern void FUN_00521080(void);
extern void FUN_005215d0(void);
extern void FUN_00521800(void);
extern void FUN_005219c0(void);
extern void FUN_00522a60(void);
extern void FUN_00523bb0(void);
extern void FUN_00524840(void);
extern void FUN_00524e20(void);
extern void FUN_005250f0(void);
extern void FUN_005253c0(void);
extern void FUN_005255a0(void);
extern void FUN_00525720(void);
extern void FUN_00525b90(void);
#include "religion_advisor_render.h"
extern void FUN_0052b6d0(void);
extern void FUN_0052d4d0(void);
extern void FUN_0052d4e0(void);
extern void FUN_0052d940(void);
extern void FUN_0052e6f0(void);
extern void FUN_0052ec50(void);
extern void FUN_0052f8b0(void);
extern void FUN_005305e0(void);
extern void FUN_00530780(void);
extern void FUN_005309b0(int param_1,int param_2);
extern void FUN_00530d70(int x,int y,int rows);
extern void FUN_00531190(void);
extern void FUN_00531480(void);
extern void FUN_00531be0(void);
extern void FUN_00531cc0(void);
extern void FUN_00531ef0(void);
extern void FUN_00532340(void);
extern void FUN_005325f0(void);
extern void FUN_00532920(void);
extern void FUN_00532fe0(void);
extern void FUN_00533470(void);
extern void FUN_00533e70(void);
extern void FUN_00534320(void);
extern void FUN_00534540(void);
extern void FUN_005345a0(void);
extern void FUN_00534610(void);
extern void FUN_00534df0(void);
extern void FUN_00535080(void);
extern void FUN_00536120(void);
extern void FUN_00537260(void);
extern void FUN_005374a0(void);
extern void FUN_00537940(void);
extern void FUN_00537a50(void);
extern void FUN_00537cf0(void);
extern void FUN_00538050(void);
extern void FUN_00538850(void);
extern uint32_t FUN_00538fa0(void);
extern void FUN_00559e10(void);
extern void FUN_0055a0e0(void);
extern void FUN_0055ad30(void);
extern uint32_t FUN_0055f449(RestoreSound *self,const char *filename,int channel,uint32_t loops);
extern uint32_t FUN_0055f5a1(RestoreSound *self,int channel);
#include "message_panel_input.h"
#include "selection_popup.h"
#include "menu_list_scroll.h"
extern void FUN_00406720(void);
extern void FUN_004d62f0(uint32_t,uint32_t,int32_t,int32_t,uint32_t,uint32_t,int32_t,int32_t);
extern void FUN_004ccc30(uint32_t,int32_t,int32_t,int32_t,int32_t,uint32_t,uint32_t);
extern void FUN_004cc110(int32_t,int32_t);
extern void FUN_00448370(const char *,char *,int32_t);
extern void FUN_004cd010(char *,uint32_t,int32_t,int32_t,uint32_t,uint32_t);
#include "pixel_access.h"
extern int32_t FUN_004883d0(int32_t),FUN_00489110(int32_t),FUN_0053d280(int32_t);
extern uint8_t FUN_004a4300(int32_t);
extern uint32_t FUN_00488030(uint32_t);
extern int32_t FUN_00489050(int32_t,int32_t,int32_t,int32_t);
extern void FUN_004256a0(int32_t,int32_t,const RestoreSquareButton *,int32_t);
extern void FUN_004ce310(uint32_t,uint32_t),FUN_0053f790(uint32_t *,uint8_t);
extern int32_t FUN_004ce480(void *,uint32_t,uint32_t,uint32_t);
extern int32_t restore_sprite_fetch_clipped(RestoreSpriteLibraries *,char *,int32_t,int32_t,int32_t,uint32_t);
extern void FUN_004cdeb0(void *,int32_t),FUN_004cdf20(void *,int32_t);
extern void FUN_0040fc40(const void *),FUN_0040fcf0(const void *),FUN_0040fdd0(const void *);
extern void FUN_00410700(const void *,uint16_t),FUN_004109d0(const void *,uint16_t),FUN_00410a70(const void *,uint16_t);
extern void FUN_00410f70(void *),FUN_00410fb0(const void *),FUN_004114a0(void *),FUN_00411fd0(void *);
extern uint32_t FUN_00410de0(const void *),FUN_00410e80(const void *);
#define OVERLAY_CHILD(frame_type,dispatch) do { \
 struct frame_type child;uint32_t k; \
 for(k=0;k<8;++k)child.r[k]=f->r[k];child.flags=f->flags;child.boundary=dispatch; \
 dispatch(site,target,&child); \
 for(k=0;k<8;++k)f->r[k]=child.r[k];f->flags=child.flags;return; \
} while(0)
/* These existing complete C layout wrappers use the real layout engine and
 * glyph/number providers. Their deeper dialog_loc loading is real owned I/O,
 * not a synthesized result or controlled production endpoint. */
extern void FUN_004d6250(uint32_t,uint32_t,int32_t,int32_t,uint32_t,uint32_t,uint32_t,int32_t,int32_t);
extern void FUN_004d6860(uint32_t,uint32_t,const char *,int32_t,int32_t,uint32_t,uint32_t,int32_t,int32_t);
extern void FUN_004d6050(void);
extern void FUN_00536c30(int32_t,int32_t,int32_t,uint32_t,uint32_t,int32_t);
extern void FUN_00430e60(uint32_t,uint32_t,uint32_t,uint32_t);
extern void FUN_004ddec0(void),FUN_004ddf20(void),FUN_004ddf70(void);
extern void FUN_004fe300(void),FUN_004fe730(void),FUN_004fe3c0(void);
extern void FUN_004fe490(void),FUN_004fe560(void),FUN_004fe630(void);
extern void FUN_005058f0(void),FUN_0053d0b0(void),FUN_004fa780(void);
extern void FUN_00536820(int32_t,int32_t,int32_t,int32_t);
extern void FUN_004d6160(uint32_t,uint32_t,int32_t,int32_t,uint32_t,uint32_t,uint32_t,int32_t,int32_t);
extern uint32_t FUN_0047d350(void);
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define A(n) U(sp+4u*(n))
void restore_menu_overlay_dispatch(uint32_t site,uint32_t target,struct restore_menu_overlay_frame *f)
{
 uint32_t sp=f->r[4],cleanup=0;(void)site;
 switch(target){
 case 0x52c570u:FUN_0052c570();break;
 case 0x4d6200u:restore_menu_overlay_C_004d6200(f);return;
 case 0x4cd290u:restore_menu_overlay_C_004cd290(f);return;
 case 0x446610u:restore_menu_overlay_C_00446610(f);return;
 case 0x40cc40u:f->r[0]=(uint32_t)FUN_0040cc40((int32_t)A(1));break;
 case 0x447530u:f->r[0]=(uint32_t)FUN_00447530();break;
 case 0x4bd590u:f->r[0]=(uint32_t)FUN_004bd590();break;
 case 0x4f52b0u:f->r[0]=(uint32_t)FUN_004f52b0((int32_t)A(1));break;
 case 0x4cd1a0u:FUN_004cd1a0(A(1),(int16_t *)(uintptr_t)A(2),A(3),(int32_t)A(4),(int32_t)A(5));break;
 case 0x44d4c0u:f->r[0]=(f->r[0]&0xffffff00u)|(uint8_t)FUN_0044d4c0((const RestoreTownRecord *)(uintptr_t)f->r[1]);break;
 case 0x44baf0u:f->r[0]=(uint32_t)(uintptr_t)restore_town_trade_next((RestoreTownList *)(uintptr_t)f->r[1],(const int16_t *)(uintptr_t)A(1));cleanup=4;break;
 case 0x44bb90u:OVERLAY_CHILD(restore_city_frame,restore_city_dispatch);
 case 0x44d700u:OVERLAY_CHILD(restore_actor_message_frame,restore_actor_message_dispatch);
 case 0x4cd680u:OVERLAY_CHILD(restore_training_render_frame,restore_training_render_dispatch);
 case 0x46f200u:restore_menu_overlay_C_0046f200(f);return;
 case 0x4f6020u:restore_menu_overlay_C_004f6020(f);return;
 case 0x417050u:f->r[0]=(uint32_t)restore_monument_base_00417050((void *)(uintptr_t)f->r[1],A(1),(int32_t *)(uintptr_t)A(2));cleanup=8;break;
 case 0x4e6710u:f->r[0]=(uint32_t)restore_monument_base_004e6710((void *)(uintptr_t)f->r[1],A(1),(int32_t *)(uintptr_t)A(2));cleanup=8;break;
 case 0x545740u:f->r[0]=(uint32_t)restore_monument_progress_00545740((void *)(uintptr_t)f->r[1],A(1),(int32_t *)(uintptr_t)A(2));cleanup=8;break;
 case 0x558be0u:f->r[0]=(uint32_t)restore_monument_progress_00558be0((void *)(uintptr_t)f->r[1],A(1),(int32_t *)(uintptr_t)A(2));cleanup=8;break;
 case 0x555e80u:f->r[0]=(uint32_t)restore_monument_progress_00555e80((void *)(uintptr_t)f->r[1],A(1),(int32_t *)(uintptr_t)A(2));cleanup=8;break;
 case 0x54f470u:f->r[0]=(uint32_t)restore_monument_progress_0054f470((void *)(uintptr_t)f->r[1],A(1),(int32_t *)(uintptr_t)A(2));cleanup=8;break;
 case 0x4ecea0u:f->r[0]=(f->r[0]&0xffffff00u)|FUN_004ecea0(A(1));break;
 case 0x4ed360u:FUN_004ed360((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),A(4));break;
 case 0x547a60u:restore_remaining_selection_00547a60();break;
 case 0x4dea50u:FUN_004dea50();break;
 case 0x4e0cf0u:FUN_004e0cf0((int32_t)A(1),A(2),A(3),(const int16_t *)(uintptr_t)A(4),(uint8_t)A(5));break;
 case 0x00447800u:restore_menu_overlay_C_00447800(f);return;
 case 0x00447840u:restore_menu_overlay_C_00447840(f);return;
 case 0x00447890u:restore_menu_overlay_C_00447890(f);return;
 case 0x00447910u:restore_menu_overlay_C_00447910(f);return;
 case 0x00447a30u:restore_menu_overlay_C_00447a30(f);return;
 case 0x00447a80u:restore_menu_overlay_C_00447a80(f);return;
 case 0x00447ab0u:restore_menu_overlay_C_00447ab0(f);return;
 case 0x00447ae0u:restore_menu_overlay_C_00447ae0(f);return;
 case 0x00447b10u:restore_menu_overlay_C_00447b10(f);return;
 case 0x00447b50u:restore_menu_overlay_C_00447b50(f);return;
 case 0x004479b0u:restore_menu_overlay_C_004479b0(f);return;
 case 0x00447b80u:restore_menu_overlay_C_00447b80(f);return;
 case 0x00447bb0u:restore_menu_overlay_C_00447bb0(f);return;
 case 0x00447c20u:restore_menu_overlay_C_00447c20(f);return;
 case 0x00447c30u:restore_menu_overlay_C_00447c30(f);return;
 case 0x00447d60u:restore_menu_overlay_C_00447d60(f);return;
 case 0x00447d80u:restore_menu_overlay_C_00447d80(f);return;
 case 0x00447de0u:restore_menu_overlay_C_00447de0(f);return;
 case 0x00447e30u:restore_menu_overlay_C_00447e30(f);return;
 case 0x00447f70u:restore_menu_overlay_C_00447f70(f);return;
 case 0x00448000u:restore_menu_overlay_C_00448000(f);return;
 case 0x00448040u:restore_menu_overlay_C_00448040(f);return;
 case 0x00448070u:restore_menu_overlay_C_00448070(f);return;
 case 0x00448180u:restore_menu_overlay_C_00448180(f);return;
 case 0x004481b0u:restore_menu_overlay_C_004481b0(f);return;
 case 0x00448270u:restore_menu_overlay_C_00448270(f);return;
 case 0x00447fd0u:restore_menu_overlay_C_00447fd0(f);return;
 case 0x00510dc0u:restore_menu_overlay_C_00510dc0(f);return;
 case 0x00510f90u:restore_menu_overlay_C_00510f90(f);return;
 case 0x448330u:FUN_00448330((int32_t)A(1),(char *)(uintptr_t)A(2),(int32_t)A(3));break;
 case 0x561a36u:{
  char *output=(char *)(uintptr_t)A(1);const char *format=(const char *)(uintptr_t)A(2);
  /* All formats used by the authenticated template callback closure. Preserve
     the actual one- or two-value native vararg contract, including pointers. */
  switch(A(2)){
  case 0x5d9a10u:f->r[0]=(uint32_t)FUN_00561a36(output,format,(const char *)(uintptr_t)A(3),(const char *)(uintptr_t)A(4));break;
  case 0x5d9a24u:f->r[0]=(uint32_t)FUN_00561a36(output,format,(int32_t)A(3),(const char *)(uintptr_t)A(4));break;
  case 0x5d9a18u:case 0x5d9a1cu:f->r[0]=(uint32_t)FUN_00561a36(output,format,(int32_t)A(3));break;
  default:abort();
  }
  break;
 }
 case 0x4181a0u:restore_menu_overlay_C_004181a0(f);return;
 case 0x536710u:restore_menu_overlay_C_00536710(f);return;
 case 0x41c380u:f->r[0]=(uint32_t)FUN_0041c380(A(1));break;
 case 0x4192b0u:f->r[0]=FUN_004192b0();break;
 case 0x4c72b0u:FUN_004c72b0();break;
 case 0x4ea880u:f->r[0]=FUN_004ea880();break;
 case 0x4cf620u:f->r[0]=(uint32_t)FUN_004cf620(A(1),A(2),A(3),A(4));break;
 case 0x4ccbb0u:FUN_004ccbb0(A(1),A(2),(int32_t)A(3),(int32_t)A(4),A(5),A(6));break;
 case 0x4ccb30u:FUN_004ccb30(A(1),A(2),(int32_t)A(3),(int32_t)A(4),A(5),A(6),A(7));break;
 case 0x4ccfd0u:FUN_004ccfd0(A(1),A(2),(int32_t)A(3),(int32_t)A(4),A(5),A(6),A(7));break;
 case 0x4cce00u:FUN_004cce00(A(1),(uint8_t)A(2),(const char *)(uintptr_t)A(3),A(4),A(5),A(6),A(7));break;
 case 0x536a30u:FUN_00536a30((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4));break;
 case 0x004d71c0u:restore_menu_overlay_C_004d71c0(f);return;
 case 0x004d75d0u:restore_menu_overlay_C_004d75d0(f);return;
 case 0x00515bf0u:restore_menu_overlay_C_00515bf0(f);return;
 case 0x00516080u:restore_menu_overlay_C_00516080(f);return;
 case 0x00516210u:restore_menu_overlay_C_00516210(f);return;
 case 0x00413470u:f->r[0]=(f->r[0]&0xffffff00u)|FUN_00413470((RestoreVideoPlayer *)(uintptr_t)f->r[1]);break;
 case 0x004138d0u:FUN_004138d0();break;
 case 0x00414c40u:FUN_00414c40();break;
 case 0x00417e80u:f->r[0]=(uint32_t)FUN_00417e80();break;
 case 0x00417eb0u:f->r[0]=(uint32_t)FUN_00417eb0();break;
 case 0x00417ee0u:f->r[0]=(uint32_t)FUN_00417ee0();break;
 case 0x00417f10u:FUN_00417f10();break;
 case 0x004180c0u:FUN_004180c0();break;
 case 0x00418f70u:restore_menu_overlay_C_00418f70(f);return;
 case 0x00419140u:restore_menu_overlay_C_00419140(f);return;
 case 0x004191f0u:restore_menu_overlay_C_004191f0(f);return;
 case 0x004194c0u:restore_menu_overlay_C_004194c0(f);return;
 case 0x00419db0u:restore_menu_overlay_C_00419db0(f);return;
 case 0x0041a180u:FUN_0041a180();break;
 case 0x0041a850u:FUN_0041a850();break;
 case 0x0041aaa0u:restore_menu_overlay_C_0041aaa0(f);return;
 case 0x0041b8f0u:f->r[0]=(f->r[0]&0xffffff00u)|FUN_0041b8f0(f->r[1]);break;
 case 0x0041bb30u:f->r[0]=(uint32_t)FUN_0041bb30();break;
 case 0x0041be10u:FUN_0041be10();break;
 case 0x0041c2d0u:FUN_0041c2d0();break;
 case 0x0041d210u:FUN_0041d210();break;
 case 0x0041e100u:f->r[0]=(uint32_t)(uintptr_t)FUN_0041e100();break;
 case 0x0041e130u:f->r[0]=(uint32_t)(uintptr_t)FUN_0041e130();break;
 case 0x0041e140u:f->r[0]=(uint32_t)FUN_0041e140();break;
 case 0x0041e170u:f->r[0]=(uint32_t)(uint8_t)FUN_0041e170();break;
 case 0x0041e5c0u:FUN_0041e5c0();break;
 case 0x0041f220u:FUN_0041f220();break;
 case 0x00425430u:FUN_00425430();break;
 case 0x00425570u:FUN_00425570();break;
 case 0x00425640u:FUN_00425640();break;
 case 0x0043e1a0u:FUN_0043e1a0();break;
 case 0x00448640u:FUN_00448640();break;
 case 0x0044aaf0u:FUN_0044aaf0();break;
 case 0x004c5660u:FUN_004c5660();break;
 case 0x004c5cc0u:FUN_004c5cc0();break;
 case 0x004c7340u:FUN_004c7340();break;
 case 0x004caf10u:FUN_004caf10();break;
 case 0x004cb4d0u:FUN_004cb4d0();break;
 case 0x004cb5a0u:FUN_004cb5a0();break;
 case 0x004e1320u:FUN_004e1320();break;
 case 0x004e1870u:FUN_004e1870();break;
 case 0x004e1a50u:FUN_004e1a50();break;
 case 0x004e61f0u:FUN_004e61f0();break;
 case 0x004f7770u:FUN_004f7770();break;
 case 0x005027b0u:f->r[0]=(uint32_t)FUN_005027b0();break;
 case 0x005165b0u:f->r[0]=(uint32_t)FUN_005165b0();break;
 case 0x00516630u:FUN_00516630();break;
 case 0x00516720u:FUN_00516720();break;
 case 0x00516bb0u:FUN_00516bb0();break;
 case 0x0051e990u:FUN_0051e990();break;
 case 0x0051ec90u:FUN_0051ec90();break;
 case 0x0051ed30u:FUN_0051ed30((int32_t)A(1),A(2));break;
 case 0x0051efb0u:FUN_0051efb0((int32_t)A(1));break;
 case 0x0051f790u:FUN_0051f790();break;
 case 0x0051fd60u:FUN_0051fd60();break;
 case 0x0051ffd0u:FUN_0051ffd0();break;
 case 0x00520000u:FUN_00520000();break;
 case 0x005208c0u:FUN_005208c0();break;
 case 0x005208f0u:FUN_005208f0();break;
 case 0x00520ab0u:FUN_00520ab0((int32_t)A(1));break;
 case 0x00520af0u:FUN_00520af0();break;
 case 0x00520b60u:FUN_00520b60();break;
 case 0x00520b90u:FUN_00520b90();break;
 case 0x00520d00u:FUN_00520d00();break;
 case 0x00520e00u:FUN_00520e00();break;
 case 0x00520ea0u:FUN_00520ea0();break;
 case 0x00520f40u:FUN_00520f40();break;
 case 0x00521060u:FUN_00521060();break;
 case 0x00521080u:FUN_00521080();break;
 case 0x005215d0u:FUN_005215d0();break;
 case 0x00521800u:FUN_00521800();break;
 case 0x005219c0u:FUN_005219c0();break;
 case 0x00522a60u:FUN_00522a60();break;
 case 0x00523bb0u:FUN_00523bb0();break;
 case 0x00524840u:FUN_00524840();break;
 case 0x00524e20u:FUN_00524e20();break;
 case 0x005250f0u:FUN_005250f0();break;
 case 0x005253c0u:FUN_005253c0();break;
 case 0x005255a0u:FUN_005255a0();break;
 case 0x00525720u:FUN_00525720();break;
 case 0x00525b90u:FUN_00525b90();break;
 case 0x005271f0u:FUN_005271f0();break;
 case 0x005292b0u:FUN_005292b0();break;
 case 0x00529320u:FUN_00529320();break;
 case 0x00529530u:FUN_00529530();break;
 case 0x0052b6d0u:restore_menu_overlay_C_0052b6d0(f);return;
 case 0x0052d4d0u:FUN_0052d4d0();break;
 case 0x0052d4e0u:FUN_0052d4e0();break;
 case 0x0052d940u:FUN_0052d940();break;
 case 0x0052e6f0u:FUN_0052e6f0();break;
 case 0x0052ec50u:FUN_0052ec50();break;
 case 0x0052f8b0u:FUN_0052f8b0();break;
 case 0x005305e0u:FUN_005305e0();break;
 case 0x00530780u:FUN_00530780();break;
 case 0x005309b0u:FUN_005309b0((int32_t)A(1),(int32_t)A(2));break;
 case 0x00530d70u:FUN_00530d70((int32_t)A(1),(int32_t)A(2),(int32_t)A(3));break;
 case 0x00531190u:FUN_00531190();break;
 case 0x00531480u:FUN_00531480();break;
 case 0x00531be0u:FUN_00531be0();break;
 case 0x00531cc0u:FUN_00531cc0();break;
 case 0x00531ef0u:FUN_00531ef0();break;
 case 0x00532340u:FUN_00532340();break;
 case 0x005325f0u:FUN_005325f0();break;
 case 0x00532920u:FUN_00532920();break;
 case 0x00532fe0u:FUN_00532fe0();break;
 case 0x00533470u:FUN_00533470();break;
 case 0x00533e70u:FUN_00533e70();break;
 case 0x00534320u:FUN_00534320();break;
 case 0x00534540u:FUN_00534540();break;
 case 0x005345a0u:FUN_005345a0();break;
 case 0x00534610u:FUN_00534610();break;
 case 0x00534df0u:FUN_00534df0();break;
 case 0x00535080u:restore_menu_overlay_C_00535080(f);return;
 case 0x00536120u:FUN_00536120();break;
 case 0x00537260u:FUN_00537260();break;
 case 0x005374a0u:FUN_005374a0();break;
 case 0x00537940u:FUN_00537940();break;
 case 0x00537a50u:FUN_00537a50();break;
 case 0x00537cf0u:FUN_00537cf0();break;
 case 0x00538050u:FUN_00538050();break;
 case 0x00538850u:FUN_00538850();break;
 case 0x00538fa0u:f->r[0]=(uint32_t)FUN_00538fa0();break;
 case 0x00559e10u:FUN_00559e10();break;
 case 0x0055a0e0u:FUN_0055a0e0();break;
 case 0x0055ad30u:FUN_0055ad30();break;
 case 0x0055f449u:f->r[0]=FUN_0055f449((RestoreSound *)(uintptr_t)f->r[1],(const char *)(uintptr_t)A(1),(int32_t)A(2),A(3));cleanup=12;break;
 case 0x0055f5a1u:f->r[0]=FUN_0055f5a1((RestoreSound *)(uintptr_t)f->r[1],(int32_t)A(1));cleanup=4;break;
case 0x004f94a0u:restore_menu_overlay_C_004f94a0(f);return;
 case 0x004f96e0u:restore_menu_overlay_C_004f96e0(f);return;
 case 0x00501140u:restore_menu_overlay_C_00501140(f);return;
 case 0x005011d0u:restore_menu_overlay_C_005011d0(f);return;
 case 0x00501a40u:restore_menu_overlay_C_00501a40(f);return;
 case 0x00501b60u:restore_menu_overlay_C_00501b60(f);return;
 case 0x005026f0u:restore_menu_overlay_C_005026f0(f);return;
 case 0x00502990u:restore_menu_overlay_C_00502990(f);return;
 case 0x00503e10u:restore_menu_overlay_C_00503e10(f);return;
 case 0x00504630u:restore_menu_overlay_C_00504630(f);return;
 case 0x00510b00u:restore_menu_overlay_C_00510b00(f);return;
 case 0x00502840u:restore_menu_overlay_C_00502840(f);return;
 case 0x425e80u:case 0x425c40u:case 0x4cde40u:
  OVERLAY_CHILD(restore_training_render_frame,restore_training_render_dispatch);
 case 0x425bc0u:FUN_00425bc0((int32_t)A(1),(int32_t)A(2),(const void *)(uintptr_t)A(3),(int32_t)A(4),(int32_t)A(5));break;
 case 0x425990u:f->r[0]=(uint32_t)FUN_00425990((int32_t)A(1),(int32_t)A(2),(const void *)(uintptr_t)A(3),(int32_t)A(4));break;
 case 0x426450u:f->r[0]=(uint32_t)FUN_00426450((int32_t)A(1),(int32_t)A(2),(const RestoreMenuButton *)(uintptr_t)A(3),(int32_t)A(4));break;
 case 0x4261d0u:f->r[0]=(uint32_t)FUN_004261d0((int32_t)A(1),(int32_t)A(2),(RestoreMenuButton *)(uintptr_t)A(3),(int32_t)A(4));break;
 case 0x4d6740u:f->r[0]=(uint32_t)FUN_004d6740((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),A(4),(int32_t)A(5));break;
 case 0x515c60u:FUN_00515c60();break;
 case 0x516020u:FUN_00516020();break;
 case 0x40d1b0u:f->r[0]=FUN_0040d1b0();break;
 case 0x406720u:FUN_00406720();break;
case 0x004f8e60u:restore_menu_overlay_C_004f8e60(f);return;
 case 0x004fa4a0u:restore_menu_overlay_C_004fa4a0(f);return;
 case 0x004fc820u:restore_menu_overlay_C_004fc820(f);return;
 case 0x004fc8e0u:restore_menu_overlay_C_004fc8e0(f);return;
 case 0x004fcc90u:restore_menu_overlay_C_004fcc90(f);return;
 case 0x004fcf10u:restore_menu_overlay_C_004fcf10(f);return;
 case 0x004fd120u:restore_menu_overlay_C_004fd120(f);return;
 case 0x004fd4b0u:restore_menu_overlay_C_004fd4b0(f);return;
 case 0x004fda00u:restore_menu_overlay_C_004fda00(f);return;
 case 0x004fdc40u:restore_menu_overlay_C_004fdc40(f);return;
 case 0x004fde80u:restore_menu_overlay_C_004fde80(f);return;
 case 0x004fe0c0u:restore_menu_overlay_C_004fe0c0(f);return;
 case 0x004fe800u:restore_menu_overlay_C_004fe800(f);return;
 case 0x004fe8d0u:restore_menu_overlay_C_004fe8d0(f);return;
 case 0x004fe9a0u:restore_menu_overlay_C_004fe9a0(f);return;
 case 0x004feb00u:restore_menu_overlay_C_004feb00(f);return;
 case 0x004fed40u:restore_menu_overlay_C_004fed40(f);return;
 case 0x004feea0u:restore_menu_overlay_C_004feea0(f);return;
 case 0x004ff000u:restore_menu_overlay_C_004ff000(f);return;
 case 0x004ff200u:restore_menu_overlay_C_004ff200(f);return;
 case 0x004ff400u:restore_menu_overlay_C_004ff400(f);return;
 case 0x004ff740u:restore_menu_overlay_C_004ff740(f);return;
 case 0x004ffa20u:restore_menu_overlay_C_004ffa20(f);return;
 case 0x004ffb30u:restore_menu_overlay_C_004ffb30(f);return;
 case 0x004ffbf0u:restore_menu_overlay_C_004ffbf0(f);return;
 case 0x004ffcc0u:restore_menu_overlay_C_004ffcc0(f);return;
 case 0x004ffd90u:restore_menu_overlay_C_004ffd90(f);return;
 case 0x004ffe60u:restore_menu_overlay_C_004ffe60(f);return;
 case 0x00500000u:restore_menu_overlay_C_00500000(f);return;
 case 0x005001a0u:restore_menu_overlay_C_005001a0(f);return;
 case 0x00500380u:restore_menu_overlay_C_00500380(f);return;
 case 0x00500530u:restore_menu_overlay_C_00500530(f);return;
 case 0x00500a40u:restore_menu_overlay_C_00500a40(f);return;
 case 0x005013e0u:restore_menu_overlay_C_005013e0(f);return;
 case 0x005014a0u:restore_menu_overlay_C_005014a0(f);return;
 case 0x00501ad0u:restore_menu_overlay_C_00501ad0(f);return;
 case 0x00501fd0u:restore_menu_overlay_C_00501fd0(f);return;
 case 0x005028f0u:restore_menu_overlay_C_005028f0(f);return;
 case 0x00502f70u:restore_menu_overlay_C_00502f70(f);return;
 case 0x00503360u:restore_menu_overlay_C_00503360(f);return;
 case 0x005035f0u:restore_menu_overlay_C_005035f0(f);return;
 case 0x00503940u:restore_menu_overlay_C_00503940(f);return;
 case 0x00503aa0u:restore_menu_overlay_C_00503aa0(f);return;
 case 0x00503c00u:restore_menu_overlay_C_00503c00(f);return;
 case 0x00503f00u:restore_menu_overlay_C_00503f00(f);return;
 case 0x00504070u:restore_menu_overlay_C_00504070(f);return;
 case 0x00504300u:restore_menu_overlay_C_00504300(f);return;
 case 0x005043c0u:restore_menu_overlay_C_005043c0(f);return;
 case 0x00504490u:restore_menu_overlay_C_00504490(f);return;
 case 0x00504560u:restore_menu_overlay_C_00504560(f);return;
 case 0x00504680u:restore_menu_overlay_C_00504680(f);return;
 case 0x005048d0u:restore_menu_overlay_C_005048d0(f);return;
 case 0x00504c20u:restore_menu_overlay_C_00504c20(f);return;
 case 0x00505050u:restore_menu_overlay_C_00505050(f);return;
 case 0x005051e0u:restore_menu_overlay_C_005051e0(f);return;
 case 0x00505320u:restore_menu_overlay_C_00505320(f);return;
 case 0x00505460u:restore_menu_overlay_C_00505460(f);return;
 case 0x00505560u:restore_menu_overlay_C_00505560(f);return;
 case 0x005057f0u:restore_menu_overlay_C_005057f0(f);return;
 case 0x00505a00u:restore_menu_overlay_C_00505a00(f);return;
 case 0x00505dc0u:restore_menu_overlay_C_00505dc0(f);return;
 case 0x00506250u:restore_menu_overlay_C_00506250(f);return;
 case 0x005066c0u:restore_menu_overlay_C_005066c0(f);return;
 case 0x00506b30u:restore_menu_overlay_C_00506b30(f);return;
 case 0x00506fc0u:restore_menu_overlay_C_00506fc0(f);return;
 case 0x00507430u:restore_menu_overlay_C_00507430(f);return;
 case 0x005078a0u:restore_menu_overlay_C_005078a0(f);return;
 case 0x00507d30u:restore_menu_overlay_C_00507d30(f);return;
 case 0x00508120u:restore_menu_overlay_C_00508120(f);return;
 case 0x005085b0u:restore_menu_overlay_C_005085b0(f);return;
 case 0x00508a60u:restore_menu_overlay_C_00508a60(f);return;
 case 0x00508e00u:restore_menu_overlay_C_00508e00(f);return;
 case 0x005091c0u:restore_menu_overlay_C_005091c0(f);return;
 case 0x00509570u:restore_menu_overlay_C_00509570(f);return;
 case 0x00509920u:restore_menu_overlay_C_00509920(f);return;
 case 0x00509cd0u:restore_menu_overlay_C_00509cd0(f);return;
 case 0x0050a060u:restore_menu_overlay_C_0050a060(f);return;
 case 0x0050a400u:restore_menu_overlay_C_0050a400(f);return;
 case 0x0050a870u:restore_menu_overlay_C_0050a870(f);return;
 case 0x0050ace0u:restore_menu_overlay_C_0050ace0(f);return;
 case 0x0050b160u:restore_menu_overlay_C_0050b160(f);return;
 case 0x0050b6d0u:restore_menu_overlay_C_0050b6d0(f);return;
 case 0x0050bb50u:restore_menu_overlay_C_0050bb50(f);return;
 case 0x0050bfd0u:restore_menu_overlay_C_0050bfd0(f);return;
 case 0x0050c540u:restore_menu_overlay_C_0050c540(f);return;
 case 0x0050c9b0u:restore_menu_overlay_C_0050c9b0(f);return;
 case 0x0050ce20u:restore_menu_overlay_C_0050ce20(f);return;
 case 0x0050d290u:restore_menu_overlay_C_0050d290(f);return;
 case 0x0050d6c0u:restore_menu_overlay_C_0050d6c0(f);return;
 case 0x0050d930u:restore_menu_overlay_C_0050d930(f);return;
 case 0x0050dbe0u:restore_menu_overlay_C_0050dbe0(f);return;
 case 0x0050e030u:restore_menu_overlay_C_0050e030(f);return;
 case 0x0050e570u:restore_menu_overlay_C_0050e570(f);return;
 case 0x0050f320u:restore_menu_overlay_C_0050f320(f);return;
 case 0x0050fca0u:restore_menu_overlay_C_0050fca0(f);return;
 case 0x005101b0u:restore_menu_overlay_C_005101b0(f);return;
 case 0x00511040u:restore_menu_overlay_C_00511040(f);return;
 case 0x00511c80u:restore_menu_overlay_C_00511c80(f);return;
 case 0x00512140u:restore_menu_overlay_C_00512140(f);return;
 case 0x005124a0u:restore_menu_overlay_C_005124a0(f);return;
 case 0x00512be0u:restore_menu_overlay_C_00512be0(f);return;
 case 0x005132a0u:restore_menu_overlay_C_005132a0(f);return;
 case 0x005135e0u:restore_menu_overlay_C_005135e0(f);return;
 case 0x00513d40u:restore_menu_overlay_C_00513d40(f);return;
 case 0x00514400u:restore_menu_overlay_C_00514400(f);return;
 case 0x00514a40u:restore_menu_overlay_C_00514a40(f);return;
 case 0x00515d10u:restore_menu_overlay_C_00515d10(f);return;
 case 0x004fc150u:restore_menu_overlay_C_004fc150(f);return;
 case 0x004fc2d0u:restore_menu_overlay_C_004fc2d0(f);return;
 case 0x004fc3b0u:restore_menu_overlay_C_004fc3b0(f);return;
 case 0x004fc4f0u:restore_menu_overlay_C_004fc4f0(f);return;
 case 0x004fc5c0u:restore_menu_overlay_C_004fc5c0(f);return;
 case 0x004cde80u:restore_menu_overlay_C_004cde80(f);return;
 case 0x004f5580u:restore_menu_overlay_C_004f5580(f);return;
 case 0x004f56d0u:restore_menu_overlay_C_004f56d0(f);return;
 case 0x004f5770u:restore_menu_overlay_C_004f5770(f);return;
 case 0x004f5bd0u:restore_menu_overlay_C_004f5bd0(f);return;
 case 0x004f5d10u:restore_menu_overlay_C_004f5d10(f);return;
 case 0x004e6070u:restore_menu_overlay_C_004e6070(f);return;
 case 0x004e60f0u:restore_menu_overlay_C_004e60f0(f);return;
 case 0x004e6150u:restore_menu_overlay_C_004e6150(f);return;
 case 0x004f56a0u:restore_menu_overlay_C_004f56a0(f);return;
 case 0x004ef100u:restore_menu_overlay_C_004ef100(f);return;
 case 0x0046f1c0u:restore_menu_overlay_C_0046f1c0(f);return;
 case 0x004894a0u:restore_menu_overlay_C_004894a0(f);return;
 case 0x004e5a30u:restore_menu_overlay_C_004e5a30(f);return;
 case 0x005013a0u:restore_menu_overlay_C_005013a0(f);return;
 case 0x00502780u:restore_menu_overlay_C_00502780(f);return;
 case 0x00505c40u:restore_menu_overlay_C_00505c40(f);return;
 case 0x0050efb0u:restore_menu_overlay_C_0050efb0(f);return;
 case 0x0050e3c0u:restore_menu_overlay_C_0050e3c0(f);return;
 case 0x0050f890u:restore_menu_overlay_C_0050f890(f);return;
 case 0x0050fc10u:restore_menu_overlay_C_0050fc10(f);return;
 case 0x0050fc70u:restore_menu_overlay_C_0050fc70(f);return;
 case 0x0050fe50u:restore_menu_overlay_C_0050fe50(f);return;
 case 0x004204d0u:restore_menu_overlay_C_004204d0(f);return;
 case 0x00420500u:restore_menu_overlay_C_00420500(f);return;
 case 0x00420550u:restore_menu_overlay_C_00420550(f);return;
 case 0x00420600u:restore_menu_overlay_C_00420600(f);return;
 case 0x004205b0u:restore_menu_overlay_C_004205b0(f);return;
 case 0x00420990u:restore_menu_overlay_C_00420990(f);return;
 case 0x00420cc0u:restore_menu_overlay_C_00420cc0(f);return;
 case 0x00420fc0u:restore_menu_overlay_C_00420fc0(f);return;
 case 0x004212a0u:restore_menu_overlay_C_004212a0(f);return;
 case 0x00460fb0u:restore_menu_overlay_C_00460fb0(f);return;
 case 0x004fa780u:restore_menu_overlay_C_004fa780(f);return;
 case 0x004fe300u:restore_menu_overlay_C_004fe300(f);return;
 case 0x004fe3c0u:restore_menu_overlay_C_004fe3c0(f);return;
 case 0x004fe490u:restore_menu_overlay_C_004fe490(f);return;
 case 0x004fe560u:restore_menu_overlay_C_004fe560(f);return;
 case 0x004fe630u:restore_menu_overlay_C_004fe630(f);return;
 case 0x004fe730u:restore_menu_overlay_C_004fe730(f);return;
 case 0x005058f0u:restore_menu_overlay_C_005058f0(f);return;
 case 0x004fab20u:restore_menu_overlay_C_004fab20(f);return;
 case 0x004fa8c0u:restore_menu_overlay_C_004fa8c0(f);return;
 case 0x004fbf00u:restore_menu_overlay_C_004fbf00(f);return;
 case 0x004fbf30u:restore_menu_overlay_C_004fbf30(f);return;
 case 0x004fc040u:restore_menu_overlay_C_004fc040(f);return;
 case 0x00488080u:restore_menu_overlay_C_00488080(f);return;
 case 0x004890f0u:restore_menu_overlay_C_004890f0(f);return;
 case 0x00489240u:restore_menu_overlay_C_00489240(f);return;
 case 0x0053fa40u:restore_menu_overlay_C_0053fa40(f);return;
 case 0x4fa530u:FUN_004fa530();break;
 case 0x4fa3c0u:FUN_004fa3c0();break;
 case 0x4dddc0u:FUN_004dddc0();break;
 case 0x55f0e8u:f->r[0]=(uint32_t)FUN_0055f0e8((RestoreSound *)(uintptr_t)f->r[1],(const int32_t *)(uintptr_t)A(1));cleanup=4;break;
 case 0x4d65b0u:case 0x4ccc30u:case 0x4cde60u:
 case 0x4256a0u:case 0x443d90u:case 0x447790u:case 0x4d63d0u:
  OVERLAY_CHILD(restore_training_render_frame,restore_training_render_dispatch);
 case 0x47d350u:case 0x4d6160u:case 0x4d62f0u:case 0x536820u:case 0x425730u:
  OVERLAY_CHILD(restore_resource_render_frame,restore_resource_render_dispatch);
 case 0x45d510u:case 0x489470u:
  OVERLAY_CHILD(restore_actor_message_frame,restore_actor_message_dispatch);
 case 0x4f1310u:OVERLAY_CHILD(restore_pyramid_summary_frame,restore_pyramid_summary_dispatch);
 case 0x547ba0u:case 0x545fb0u:OVERLAY_CHILD(restore_menu_input_frame,restore_placement_queries_dispatch);
 case 0x401540u:f->r[0]=(uint32_t)FUN_00401540(A(1),(int32_t *)(uintptr_t)A(2));cleanup=8;break;
 case 0x40d940u:f->r[0]=(uint32_t)restore_monument_base_0040d940((void *)(uintptr_t)f->r[1],A(1),(int32_t *)(uintptr_t)A(2));cleanup=8;break;
 case 0x4df040u:f->r[0]=(uint32_t)FUN_004df040(A(1),(int32_t *)(uintptr_t)A(2));break;
 case 0x542ad0u:f->r[0]=(uint32_t)FUN_00542ad0(A(1),(int32_t *)(uintptr_t)A(2));break;
 /* Complete native constant-return entries, not approximations. */
 case 0x545f60u:f->r[0]=9;break;case 0x559460u:f->r[0]=10;break;
 case 0x5567b0u:f->r[0]=21;break;case 0x54fed0u:f->r[0]=31;break;
 case 0x457af0u:FUN_00457af0();break;
 case 0x475b50u:f->r[0]=FUN_00475b50(A(1));break;
 case 0x47fd30u:f->r[0]=FUN_0047fd30((int32_t)A(1),(int32_t)A(2));break;
 case 0x480180u:f->r[0]=FUN_00480180((int32_t)A(1),(int32_t)A(2),A(3));break;
 case 0x47fe60u:f->r[0]=FUN_0047fe60((int32_t)A(1),(int32_t)A(2),A(3),(int32_t)A(4));break;
 case 0x004883d0u:restore_menu_overlay_C_004883d0(f);return;
 case 0x00489110u:restore_menu_overlay_C_00489110(f);return;
 case 0x004a4300u:restore_menu_overlay_C_004a4300(f);return;
 case 0x4c5120u:f->r[0]=FUN_004c5120((int32_t)A(1));break;
 case 0x488030u:f->r[0]=FUN_00488030(A(1));cleanup=4;break;
 case 0x0053d280u:restore_menu_overlay_C_0053d280(f);return;
 case 0x489050u:restore_menu_overlay_C_00489050(f);return;
 case 0x56133cu:{
  uint8_t raw80[10];uint64_t value;
  __asm { lea eax,raw80 }
  __asm { fstp tbyte ptr[eax] }
  value=restore_campaign_score_trunc80(raw80);
  f->r[0]=(uint32_t)value;f->r[2]=(uint32_t)(value>>32);break;
 }
 case 0x4cc110u:FUN_004cc110((int32_t)A(1),(int32_t)A(2));break;
 case 0x448370u:FUN_00448370((const char *)(uintptr_t)A(1),(char *)(uintptr_t)A(2),(int32_t)A(3));break;
 case 0x4cd010u:FUN_004cd010((char *)(uintptr_t)A(1),A(2),(int32_t)A(3),(int32_t)A(4),A(5),A(6));break;
 case 0x4cf090u:FUN_004cf090((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),A(5));break;
 case 0x40f9a0u:FUN_0040f9a0((const uint32_t *)(uintptr_t)A(1),(int32_t)A(2),(int32_t)A(3));break;
 case 0x4ce310u:FUN_004ce310(A(1),A(2));break;
 case 0x53f790u:FUN_0053f790((uint32_t *)(uintptr_t)A(1),(uint8_t)A(2));break;
 case 0x4ce480u:f->r[0]=(uint32_t)FUN_004ce480((void *)(uintptr_t)A(1),A(2),A(3),A(4));break;
 case 0x4cdeb0u:FUN_004cdeb0((void *)(uintptr_t)A(1),(int32_t)A(2));break;
 case 0x4cdf20u:FUN_004cdf20((void *)(uintptr_t)A(1),(int32_t)A(2));break;
 case 0x4cdad0u:FUN_004cdad0();break;
 case 0x4cdb80u:FUN_004cdb80();break;
 case 0x53f7d0u:f->r[0]=(uint32_t)restore_sprite_fetch_clipped((RestoreSpriteLibraries *)(uintptr_t)f->r[1],(char *)(uintptr_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),A(5));cleanup=20;break;
 case 0x51f780u:break; /* Actual native RET-only entry. */
 case 0x40fdd0u:FUN_0040fdd0((const void *)(uintptr_t)A(1));break;
 case 0x40fcf0u:FUN_0040fcf0((const void *)(uintptr_t)A(1));break;
 case 0x40fc40u:FUN_0040fc40((const void *)(uintptr_t)A(1));break;
 case 0x40ff50u:FUN_0040ff50((const void *)(uintptr_t)A(1));break;
 case 0x40fed0u:FUN_0040fed0((const void *)(uintptr_t)A(1));break;
 case 0x40fb60u:FUN_0040fb60((const void *)(uintptr_t)A(1));break;
 case 0x410090u:FUN_00410090((const void *)(uintptr_t)A(1));break;
 case 0x410b10u:FUN_00410b10((const void *)(uintptr_t)A(1));break;
 case 0x410fb0u:FUN_00410fb0((const void *)(uintptr_t)A(1));break;
 case 0x4109d0u:FUN_004109d0((const void *)(uintptr_t)A(1),(uint16_t)A(2));break;
 case 0x410a70u:FUN_00410a70((const void *)(uintptr_t)A(1),(uint16_t)A(2));break;
 case 0x410700u:FUN_00410700((const void *)(uintptr_t)A(1),(uint16_t)A(2));break;
 case 0x410f70u:FUN_00410f70((void *)(uintptr_t)A(1));break;
 case 0x4114a0u:FUN_004114a0((void *)(uintptr_t)A(1));break;
 case 0x411fd0u:FUN_00411fd0((void *)(uintptr_t)A(1));break;
 case 0x410de0u:f->r[0]=FUN_00410de0((const void *)(uintptr_t)A(1));break;
 case 0x410e80u:f->r[0]=FUN_00410e80((const void *)(uintptr_t)A(1));break;
 case 0x42c740u:f->r[0]=(uint32_t)FUN_0042c740();break;
 case 0x42c9a0u:f->r[0]=(uint32_t)FUN_0042c9a0();break;
 case 0x42cee0u:f->r[0]=(uint32_t)FUN_0042cee0();break;
 case 0x42d810u:f->r[0]=(uint32_t)FUN_0042d810();break;
 case 0x42e680u:f->r[0]=(uint32_t)FUN_0042e680();break;
 case 0x42a160u:FUN_0042a160((const void *)(uintptr_t)A(1),(int32_t)A(2));break;
 case 0x42b440u:FUN_0042b440((const void *)(uintptr_t)A(1),(int32_t)A(2));break;
 case 0x427a20u:FUN_00427a20((const void *)(uintptr_t)A(1),(int32_t)A(2));break;
 case 0x4d6090u:restore_menu_overlay_C_004d6090(f);return;
 case 0x536dc0u:restore_menu_overlay_C_00536dc0(f);return;
 case 0x430790u:f->r[0]=(uint32_t)FUN_00430790(A(1));break;
 case 0x4d6250u:FUN_004d6250(A(1),A(2),(int32_t)A(3),(int32_t)A(4),A(5),A(6),A(7),(int32_t)A(8),(int32_t)A(9));break;
 case 0x4d6860u:FUN_004d6860(A(1),A(2),(const char *)(uintptr_t)A(3),(int32_t)A(4),(int32_t)A(5),A(6),A(7),(int32_t)A(8),(int32_t)A(9));break;
 case 0x4cc720u:FUN_004cc720((const uint8_t *)(uintptr_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),(int32_t)A(5),(uint16_t)A(6));break;
 case 0x4cc770u:FUN_004cc770((const uint8_t *)(uintptr_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),(uint16_t)A(5));break;
 case 0x53e590u:f->r[0]=(uint32_t)restore_sprite_draw((RestoreSpriteLibraries *)(uintptr_t)f->r[1],(int32_t)A(1),(int32_t)A(2),(int32_t)A(3));cleanup=12;break;
 case 0x53f4f0u:f->r[0]=(uint32_t)FUN_0053f4f0((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),A(4));cleanup=16;break;
 case 0x4d6050u:FUN_004d6050();break;
 case 0x536c30u:FUN_00536c30((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),A(4),A(5),(int32_t)A(6));break;
 default:abort();
 }
 f->r[4]=sp+4u+cleanup;
}
#undef A
static uint32_t invoke(uint32_t entry,const uint32_t *arguments,uint32_t count)
{
 uint32_t stack[16384],sp,i,cleanup=entry==0x53fa40u?12u:entry==0x489050u?16u:(entry==0x4883d0u||entry==0x489110u)?4u:0u;
 struct restore_menu_overlay_frame f={{0},2,restore_menu_overlay_dispatch};
 /* Native scratch is shared by the complete dispatcher/children. Initialize
  * only guard regions, the return token and actual outgoing arguments. */
 for(i=0;i<32;++i)stack[i]=0x53504c11u;
 for(i=16256;i<16384;++i)stack[i]=0x53504c11u;
 sp=(uint32_t)(uintptr_t)(stack+16000);f.r[4]=sp;stack[16000]=0;
 for(i=0;i<count;++i)stack[16001+i]=arguments[i];
 if(entry==0x53fa40u)f.r[1]=(uint32_t)(uintptr_t)RESTORE_SPRITE_LIBRARIES;
 restore_menu_overlay_dispatch(0,entry,&f);
 if(f.r[4]!=sp+4u+cleanup||f.r[3]||f.r[5]||f.r[6]||f.r[7])abort();
 for(i=0;i<32;++i)if(stack[i]!=0x53504c11u)abort();
 for(i=16256;i<16384;++i)if(stack[i]!=0x53504c11u)abort();
 return f.r[0];
}
void FUN_00535080(void){invoke(0x535080u,0,0);}
void FUN_00536dc0(int32_t index,int32_t x,int32_t y,int32_t columns,int32_t rows,int32_t style)
{
 const uint32_t a[]={(uint32_t)index,(uint32_t)x,(uint32_t)y,(uint32_t)columns,(uint32_t)rows,(uint32_t)style};invoke(0x536dc0u,a,6);
}
void FUN_004d6090(int32_t kind,int32_t x,int32_t y,int32_t width,int32_t height,int32_t relative_x,int32_t relative_y,int32_t origin_x,int32_t origin_y)
{
 const uint32_t a[]={(uint32_t)kind,(uint32_t)x,(uint32_t)y,(uint32_t)width,(uint32_t)height,(uint32_t)relative_x,(uint32_t)relative_y,(uint32_t)origin_x,(uint32_t)origin_y};invoke(0x4d6090u,a,9);
}

/* Native 4FA330/34 instructions. Preserve full camera coordinates around the
 * actual preview render; these typed providers retain their existing native
 * caller contracts. Wider world-render scratch/pixel fidelity is separate. */
void FUN_004fa330(uint32_t tile,uint32_t actor)
{
 uint32_t old_x=U(0xbca2bcu),old_y=U(0xbca2c0u);
 FUN_004304c0(tile);
 U(0xbca2bcu)=U(0xd3b4e4u)-2u;U(0xbca2c0u)=U(0xd3b4e0u)-6u;
 FUN_004de080();FUN_004ddf70();
 FUN_00430e60(U(0xbca2bcu),U(0xbca2c0u),U(0xbca2d8u),actor);
 U(0xbca2c0u)=old_y;U(0xbca2bcu)=old_x;
 if(U(0xf53f78u)==1u)FUN_004ddec0();else FUN_004ddf20();
}
/* Whole 4FA3C0/55 instructions. Width/height are WORD stores, geometry and
 * both framebuffer pointers are DWORDs; animation records have a64-byte stride. */
void FUN_004fa3c0(void)
{
 uint32_t index=0,frame,destination;int32_t minimum_y;
 if((int32_t)U(0xeadd40u)<=0)return;
 frame=U(0xf567e4u)+8u;if((int32_t)frame>=(int32_t)U(0x126517cu))frame=0;
 minimum_y=(int16_t)*(volatile uint16_t *)(uintptr_t)(0x10d6a0eu+frame*64u);
 while((int32_t)index<(int32_t)U(0xeadd40u)){
  uint32_t actor=U(0xeadd24u+index*4u);
  FUN_004fa330(U(0xbcd618u+actor*388u),actor);
  U(0xd3b5e0u)=U(0x70e3e4u);U(0xd3b5dcu)=U(0x70e3e0u);
  *(volatile uint16_t *)(uintptr_t)0xd3b5beu=12;
  *(volatile uint16_t *)(uintptr_t)0xd3b5bcu=48;
  if((int32_t)U(0x70e3e4u)<0)U(0xd3b5e0u)=0;
  if((int32_t)U(0x70e3e0u)<minimum_y)U(0xd3b5dcu)=(uint32_t)minimum_y;
  destination=U(0x5f76e8u+index*4u)+U(0xe92e1cu);
  FUN_0040f9f0((uint32_t *)(uintptr_t)destination,(int32_t)(U(0xe38d94u)-96u),0);
  ++index;
 }
 FUN_004dddc0();
}
/* Whole4FA530/171 instructions. The generic detail panel retains its native
 * specialized branches and full-width selection, dimensions and font values. */
void FUN_004fa530(void)
{
 uint32_t category=U(0xeadd4cu),actor,kind;
 switch(category){case 6:U(0xeadd54u)=57;break;case 7:U(0xeadd54u)=59;break;
 case 9:case 23:case 24:U(0xeadd54u)=85;break;case 11:U(0xeadd54u)=58;break;
 default:U(0xeadd54u)=0;break;}
 if(category==12){FUN_004fe300();return;}
 if(category==13){if((int32_t)U(0xeadd40u)<1){FUN_004fe730();return;}}
 else switch(category){
 case 9:FUN_004fe3c0();return;case 23:FUN_004fe490();return;
 case 24:FUN_004fe560();return;case 7:FUN_004fe630();return;case 8:FUN_005058f0();return;
 default:break;}
 if(U(0xeadd94u)){
  U(0xeadd94u)=0;
  if((int32_t)U(0xeadd40u)<1)(void)FUN_0047d350();else FUN_0053d0b0();
 }
 if((int32_t)U(0xeadd40u)>0&&(actor=U(0xeadd24u+U(0xeadce4u)*4u))!=0){
  kind=*(volatile uint8_t *)(uintptr_t)(0xbcd602u+actor*388u);
  U(0xeadd54u)=kind<67u?42u:330u;
 }
 FUN_004fa3c0();
 FUN_00536820((int32_t)U(0xeadd90u),(int32_t)U(0xeadd8cu),(int32_t)U(0xeadd88u),(int32_t)U(0xeadd84u));
 if(U(0xeadd40u)==0)FUN_004d6250(70,U(0xeadd4cu)+10u,(int32_t)U(0xeadd90u),(int32_t)(U(0xeadd8cu)+10u),U(0xeadd88u)*16u,U(0xf56fb4u),0,(int32_t)U(0xeadd90u),(int32_t)U(0xeadd8cu));
 if(U(0xeadd4cu)==3u){
  FUN_004d6160(70,39,(int32_t)(U(0xeadd90u)+36u),(int32_t)(U(0xeadd84u)*16u-113u+U(0xeadd8cu)),(U(0xeadd88u)-4u)*16u,U(0xf56f8cu),0,(int32_t)U(0xeadd90u),(int32_t)U(0xeadd8cu));
  FUN_004fa780();return;
 }
 if(U(0xeadd4cu)!=6u&&U(0xeadd4cu)!=13u)FUN_004d6160(70,U(0xeadd4cu)+36u,(int32_t)(U(0xeadd90u)+36u),(int32_t)(U(0xeadd84u)*16u-113u+U(0xeadd8cu)),(U(0xeadd88u)-4u)*16u,U(0xf56f8cu),0,(int32_t)U(0xeadd90u),(int32_t)U(0xeadd8cu));
 FUN_004fa780();
}

/* Canonical C facades for the replaced complete native detail bodies. */
void FUN_004f8e60(void){(void)invoke(0x004f8e60u,0,0);}
void FUN_004fa4a0(void){(void)invoke(0x004fa4a0u,0,0);}
void FUN_004fc150(int32_t y){const uint32_t a[]={(uint32_t)y};(void)invoke(0x004fc150u,a,1);}
void FUN_004fc2d0(int32_t y){const uint32_t a[]={(uint32_t)y};(void)invoke(0x004fc2d0u,a,1);}
void FUN_004fc3b0(int32_t y){const uint32_t a[]={(uint32_t)y};(void)invoke(0x004fc3b0u,a,1);}
void FUN_004fc4f0(int32_t y){const uint32_t a[]={(uint32_t)y};(void)invoke(0x004fc4f0u,a,1);}
void FUN_004fc5c0(int32_t y){const uint32_t a[]={(uint32_t)y};(void)invoke(0x004fc5c0u,a,1);}
void FUN_004fa780(void){(void)invoke(0x004fa780u,0,0);}
void FUN_004fab20(int32_t actor){const uint32_t a[]={(uint32_t)actor};(void)invoke(0x004fab20u,a,1);}
int32_t FUN_004fa8c0(int32_t actor){const uint32_t a[]={(uint32_t)actor};return (int32_t)invoke(0x004fa8c0u,a,1);}
void FUN_004fbf00(int32_t actor,int32_t x,int32_t y){const uint32_t a[]={(uint32_t)actor,(uint32_t)x,(uint32_t)y};(void)invoke(0x004fbf00u,a,3);}
void FUN_004fbf30(int32_t actor,int32_t x,int32_t y){const uint32_t a[]={(uint32_t)actor,(uint32_t)x,(uint32_t)y};(void)invoke(0x004fbf30u,a,3);}
void FUN_004fc040(int32_t actor,int32_t x,int32_t y){const uint32_t a[]={(uint32_t)actor,(uint32_t)x,(uint32_t)y};(void)invoke(0x004fc040u,a,3);}
void FUN_004fe300(void){(void)invoke(0x004fe300u,0,0);}
void FUN_004fe3c0(void){(void)invoke(0x004fe3c0u,0,0);}
void FUN_004fe490(void){(void)invoke(0x004fe490u,0,0);}
void FUN_004fe560(void){(void)invoke(0x004fe560u,0,0);}
void FUN_004fe630(void){(void)invoke(0x004fe630u,0,0);}
void FUN_004fe730(void){(void)invoke(0x004fe730u,0,0);}
void FUN_005058f0(void){(void)invoke(0x005058f0u,0,0);}
void FUN_0050efb0(void){(void)invoke(0x0050efb0u,0,0);}
void FUN_00505c40(uint32_t group){const uint32_t a[]={group};(void)invoke(0x00505c40u,a,1);}
void FUN_0050f890(int32_t actor){const uint32_t a[]={(uint32_t)actor};(void)invoke(0x0050f890u,a,1);}
void FUN_0050fe50(int32_t actor){const uint32_t a[]={(uint32_t)actor};(void)invoke(0x0050fe50u,a,1);}
void FUN_0050f320(int32_t actor){const uint32_t a[]={(uint32_t)actor};(void)invoke(0x0050f320u,a,1);}
void FUN_0050fca0(int32_t actor){const uint32_t a[]={(uint32_t)actor};(void)invoke(0x0050fca0u,a,1);}
void FUN_005101b0(int32_t actor){const uint32_t a[]={(uint32_t)actor};(void)invoke(0x005101b0u,a,1);}
char FUN_0050fc10(int32_t actor){const uint32_t a[]={(uint32_t)actor};return (char)invoke(0x0050fc10u,a,1);}
char FUN_0050fc70(int32_t actor){const uint32_t a[]={(uint32_t)actor};return (char)invoke(0x0050fc70u,a,1);}
int32_t FUN_0050e3c0(int32_t counts[16],char alternative){const uint32_t a[]={(uint32_t)(uintptr_t)counts,(uint32_t)(uint8_t)alternative};return (int32_t)invoke(0x0050e3c0u,a,2);}
int32_t FUN_0053fa40(int32_t image,int32_t x,int32_t y){const uint32_t a[]={(uint32_t)image,(uint32_t)x,(uint32_t)y};return (int32_t)invoke(0x0053fa40u,a,3);}

int32_t FUN_004883d0(int32_t id){const uint32_t a[]={(uint32_t)id};return (int32_t)invoke(0x004883d0u,a,1);}

int32_t FUN_00489110(int32_t id){const uint32_t a[]={(uint32_t)id};return (int32_t)invoke(0x00489110u,a,1);}

uint8_t FUN_004a4300(int32_t id){const uint32_t a[]={(uint32_t)id};return (uint8_t)invoke(0x004a4300u,a,1);}

int32_t FUN_0053d280(int32_t id){const uint32_t a[]={(uint32_t)id};return (int32_t)invoke(0x0053d280u,a,1);}

int32_t FUN_00489050(int32_t x,int32_t y,int32_t columns,int32_t rows)
{const uint32_t a[]={(uint32_t)x,(uint32_t)y,(uint32_t)columns,(uint32_t)rows};return (int32_t)invoke(0x489050u,a,4);}

void FUN_004f94a0(void){(void)invoke(0x004f94a0u,0,0);}

void FUN_004f96e0(void){(void)invoke(0x004f96e0u,0,0);}

void FUN_00501140(void){(void)invoke(0x00501140u,0,0);}

void FUN_005011d0(void){(void)invoke(0x005011d0u,0,0);}

void FUN_00501a40(void){(void)invoke(0x00501a40u,0,0);}

void FUN_00501b60(void){(void)invoke(0x00501b60u,0,0);}

void FUN_005026f0(void){(void)invoke(0x005026f0u,0,0);}

void FUN_00502990(void){(void)invoke(0x00502990u,0,0);}

void FUN_00503e10(void){(void)invoke(0x00503e10u,0,0);}

void FUN_00504630(void){(void)invoke(0x00504630u,0,0);}

void FUN_00510b00(int32_t group){const uint32_t a[]={(uint32_t)group};(void)invoke(0x00510b00u,a,1);}

void FUN_00502840(void){(void)invoke(0x00502840u,0,0);}

void FUN_004d71c0(int32_t force){const uint32_t a[]={(uint32_t)force};(void)invoke(0x004d71c0u,a,1);}

void FUN_004d75d0(void){(void)invoke(0x004d75d0u,0,0);}

void FUN_00515bf0(void){(void)invoke(0x00515bf0u,0,0);}

void FUN_00516080(void){(void)invoke(0x00516080u,0,0);}

void FUN_00516210(void){(void)invoke(0x00516210u,0,0);}
/* These public facades and the enclosing draw routers share the same reviewed
   private models. Dispatch must never call a facade back into itself. */
void FUN_004191f0(void){(void)invoke(0x4191f0u,0,0);}
const char *restore_menu_overlay_text_callback(uint32_t address)
{return (const char *)(uintptr_t)invoke(address,0,0);}
char *FUN_00447840(void){return (char *)(uintptr_t)invoke(0x447840u,0,0);}
void FUN_0046f200(void){(void)invoke(0x46f200u,0,0);}
void FUN_0052b6d0(void){(void)invoke(0x52b6d0u,0,0);}
uint32_t FUN_00446610(void){return invoke(0x446610u,0,0);}
void FUN_004d6200(uint32_t group,uint32_t text,const int16_t *point,uint32_t font,uint32_t color,int32_t dx,int32_t dy,int32_t origin_x,int32_t origin_y)
{const uint32_t a[]={group,text,(uint32_t)(uintptr_t)point,font,color,(uint32_t)dx,(uint32_t)dy,(uint32_t)origin_x,(uint32_t)origin_y};(void)invoke(0x4d6200u,a,9);}
void FUN_004cd290(uint32_t value,uint32_t separator,const char *suffix,const int16_t *point,uint32_t font,uint32_t color,int32_t dx,int32_t dy)
{const uint32_t a[]={value,separator,(uint32_t)(uintptr_t)suffix,(uint32_t)(uintptr_t)point,font,color,(uint32_t)dx,(uint32_t)dy};(void)invoke(0x4cd290u,a,8);}
uint8_t FUN_004f6020(uint32_t building)
{const uint32_t a[]={building};return (uint8_t)invoke(0x4f6020u,a,1);}
void FUN_004194c0(void){(void)invoke(0x4194c0u,0,0);}
void FUN_00419db0(void){(void)invoke(0x419db0u,0,0);}
void FUN_00418f70(void){(void)invoke(0x418f70u,0,0);}
void FUN_00419140(void){(void)invoke(0x419140u,0,0);}
void FUN_0041aaa0(void){(void)invoke(0x41aaa0u,0,0);}
uint32_t FUN_004181a0(void){return invoke(0x4181a0u,0,0);}
void FUN_00536710(int32_t style,int32_t x,int32_t y,int32_t segments,int32_t pressed)
{const uint32_t a[]={(uint32_t)style,(uint32_t)x,(uint32_t)y,(uint32_t)segments,(uint32_t)pressed};(void)invoke(0x536710u,a,5);}
