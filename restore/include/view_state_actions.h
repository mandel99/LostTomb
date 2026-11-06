#ifndef RESTORE_VIEW_STATE_ACTIONS_H
#define RESTORE_VIEW_STATE_ACTIONS_H
#include <stdint.h>
#define VIEW_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define VIEW_B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define VIEW_C(a) (*(volatile int8_t *)(uintptr_t)(uint32_t)(a))
void restore_view_activate_primary_action(void);
void FUN_00409fa0(void);
void restore_view_toggle_primary_action(void);
void FUN_0040b590(void);
void restore_view_toggle_tool(int32_t tool);
void FUN_004168e0(int32_t tool);
void restore_view_enable_selection_flag(void);
void FUN_00417e70(void);
void restore_view_invalidate_if_pending(void);
void FUN_004d6070(void);
void restore_city_check_notice_92(void);
void FUN_004e2d80(void);
void restore_city_check_notice_93(void);
void FUN_004e2dd0(void);
void restore_view_draw_selected_entry(void);
void FUN_0052d940(void);
void restore_view_draw_setting_toggle(void);
void FUN_005345a0(void);
void restore_view_draw_network_hint(void);
void FUN_00536620(void);
/* Native callers pass through the known EAX0; the canonical API stays void. */
uint32_t restore_view_snapshot_bounds_raw(void);
void FUN_005370d0(void);
void restore_view_close_video(void);
void FUN_005391e0(void);
#endif
