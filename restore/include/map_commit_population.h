#ifndef RESTORE_MAP_COMMIT_POPULATION_H
#define RESTORE_MAP_COMMIT_POPULATION_H
#include <stdint.h>
/* Numeric contract: all x87 exceptions masked, PC24/53/64 and the four RC
 * modes; explicit raw80 truncation and actual binary64 spill/reload. Values,
 * CW and balanced live x87 stack are covered; FPSW/traps are not emulated.
 * Native quality callers request3x3 from the immutable6x6 offset matrix.
 * Records, tile offsets and player tables must remain mapped and stable. */
/* Logical cdecl facades: the native member entries have unused incoming ECX
 * and RET4/RET8/RET16; raw frame callers must still apply those cleanups. */
uint32_t restore_building_flag_00488080(int32_t building);
uint32_t restore_building_flag_00489880(int32_t building);
uint32_t restore_building_active_004890f0(int32_t building);
int32_t restore_building_area_00489050(int32_t x,int32_t y,int32_t columns,int32_t rows);
int32_t restore_building_quality_00489110(int32_t building);
int32_t restore_building_capacity_00488140(int32_t building,uint8_t configured);
uint32_t FUN_004c5340(int32_t player);
int32_t FUN_00447530(void);
void FUN_004e3330(int32_t tool);
#endif
