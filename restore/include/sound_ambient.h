#ifndef RESTORE_SOUND_AMBIENT_H
#define RESTORE_SOUND_AMBIENT_H
#include <stdint.h>
/* Twelve signed weights come from the visible map. Zero is not a candidate;
   ties keep the first slot. The selection cache is separate from the total. */
void restore_sound_reset_ambient_weights(void);
uint32_t restore_sound_building_ambient_class(uint32_t building_type);
void restore_sound_update_ambient(void);
void FUN_0053b670(void);
uint32_t FUN_0053b830(uint32_t building_type);
void FUN_0053b960(void);
#endif
