#ifndef RESTORE_GAME_DIAGNOSTICS_H
#define RESTORE_GAME_DIAGNOSTICS_H
#include <stdint.h>
void restore_game_diagnostics_initialize(void);
struct _EXCEPTION_POINTERS;
int restore_game_startup_exception_filter(struct _EXCEPTION_POINTERS *exception);
void restore_game_trace(const char *format,...);
void restore_game_trace_save_complete(void);
/* Progress only; no per-frame file writes. Stage0 marks an idle main loop. */
void restore_game_progress(uint32_t stage);
int restore_game_trace_post_save_frame(void);
void restore_game_actor_grid_event(uint32_t site,int32_t actor,int inserting);
__declspec(noreturn) void restore_game_actor_grid_failure(uint32_t tile,int32_t actor);
#endif
