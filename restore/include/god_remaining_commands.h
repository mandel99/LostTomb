#ifndef RESTORE_GOD_REMAINING_COMMANDS_H
#define RESTORE_GOD_REMAINING_COMMANDS_H
#include <stdint.h>
void FUN_0041cbc0(void);
void FUN_0041cbf0(void);
void FUN_0041cc20(void);
void FUN_0041cc50(void);
void FUN_0041ccb0(void);
int restore_god_remaining_command_action(uint32_t token);
/* Logical cdecl one-DWORD/RET0, no incoming ECX. All inspected native
   callers discard EAX; these are genuine unique C providers, not legacy aliases. */
void restore_god_command_004e7200(int32_t duration);
void restore_god_command_004e7ac0(int32_t duration);
/* Root exports this SAME existing73-instruction core from god_helpers.c;
   this phase does not duplicate it or call old global FUN_004b1df0. */
void restore_god_actor_patrol_004b1df0(int32_t actor);
#endif
