#ifndef RESTORE_UNIT_BUTTON_ACTIONS_H
#define RESTORE_UNIT_BUTTON_ACTIONS_H
#include <stdint.h>
/* Four native nullary cdecl/RET0 button callbacks. */
void restore_unit_button_00406b40(void);
void restore_unit_button_00406e80(void);
void restore_unit_button_00406fb0(void);
void restore_unit_button_00407110(void);
int restore_unit_button_action(uint32_t token);
/* Genuine 4A4300 AL query and 4A45D0 command, native cdecl1/RET0. */
uint8_t restore_unit_actor_is_linked(uint32_t id);
void restore_unit_actor_assign_building(uint32_t id);
int32_t restore_unit_building_rank(uint32_t id);
uint32_t restore_unit_nearest_building(uint32_t id);
#endif
