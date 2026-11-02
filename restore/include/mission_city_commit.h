#ifndef RESTORE_MISSION_CITY_COMMIT_H
#define RESTORE_MISSION_CITY_COMMIT_H
#include <stdint.h>

/* Native 4487F0/448A90: ECX receiver, no stack arguments, RET 0.
   Their volatile return registers are discarded by these callers. */
void restore_mission_range_004487f0(uint32_t self);
void restore_mission_record_00448a90(uint32_t self);
void restore_mission_terrain_004771c0(void);
uint32_t restore_mission_building_count_0046f190(void);

/* Native nullary cdecl entry, ending in the real 4D4040 tail dispatch. */
void FUN_004d5980(void);
#endif
