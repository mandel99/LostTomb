#ifndef RESTORE_MENU_ECONOMY_ACTIONS_H
#define RESTORE_MENU_ECONOMY_ACTIONS_H
#include <stdint.h>
/* Whole nullary cdecl callbacks, RET0. The button dispatcher discards their
   incidental volatile-register/flags results; no incoming GPR/FPU inputs. */
int restore_menu_economy_action(uint32_t token);
/* Existing complete nullary refresh providers. Advisor40A160 consumes
   their global publications, with no incoming or returned GPR contract. */
void restore_economy_refresh_00459d20(void);
void restore_economy_refresh_0045a990(void);
void restore_economy_refresh_0045a7b0(void);
uint32_t restore_economy_trade_validate_005599c0(uint32_t resource);
#endif
