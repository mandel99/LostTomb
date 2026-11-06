#ifndef RESTORE_FINAL_STATE_ACTIONS_H
#define RESTORE_FINAL_STATE_ACTIONS_H
#include "view_state_actions.h"
void restore_building_list_adjust_offset(void);
void FUN_0040c6b0(void);
void restore_input_capture_close(void);
void FUN_00416bc0(void);
/* Only AL is written; the recovered window caller discards the result. */
uint8_t restore_window_handle_debug_key(uint32_t message,uint32_t key);
uint8_t FUN_0041d110(uint32_t message,uint32_t key);
void restore_view_draw_text_entry(void);
void FUN_0041d210(void);
void restore_map_run_refresh(uint32_t parameter);
void FUN_0046e180(uint32_t parameter);
void restore_world_initialize_subsystems(void);
void FUN_0047d6e0(void);
uint32_t restore_select_highest_priority_group(void);
uint32_t FUN_004c41b0(void);
void restore_status_append_log(uint32_t count);
void FUN_004ce860(uint32_t count);
uint32_t restore_mission_load_file(const char *path);
uint32_t FUN_004d51b0(const char *path);
void restore_map_dispatch_reset(void);
void FUN_004e6eb0(void);
#endif
