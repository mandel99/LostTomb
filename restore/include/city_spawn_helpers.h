#ifndef RESTORE_CITY_SPAWN_HELPERS_H
#define RESTORE_CITY_SPAWN_HELPERS_H
#include <stdint.h>

/* Ordinary C memory-effect APIs; physical register models remain separate. */
void restore_city_ensure_labor_seeker(int32_t building);
void restore_city_refresh_masked_building_image(int32_t building);
/* Native004673C0 defines AL only. This API exposes that logical byte result. */
uint8_t restore_city_refresh_fertility_building_image(int32_t building);
#endif
