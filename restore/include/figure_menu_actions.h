#ifndef RESTORE_FIGURE_MENU_ACTIONS_H
#define RESTORE_FIGURE_MENU_ACTIONS_H
#include <stdint.h>
void restore_figure_selection_0053d0b0(void);
uint32_t restore_figure_selection_result_0053d0b0(void);
int32_t restore_actor_figure_code(int32_t actor);
uint32_t restore_actor_voice_variant_result(int32_t bank,int32_t variant,uint32_t incoming_eax);
void restore_actor_voice_variant(int32_t bank,int32_t variant);
void FUN_0053d0b0(void);
void FUN_0053bfe0(int32_t bank,int32_t variant);
int32_t FUN_0053d280(int32_t actor);
uint32_t restore_figure_voice_00539a70(const char *name);
int restore_figure_menu_action(uint32_t token);
#endif
