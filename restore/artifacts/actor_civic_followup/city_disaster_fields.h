#ifndef RESTORE_CITY_DISASTER_FIELDS_H
#define RESTORE_CITY_DISASTER_FIELDS_H
#include <stdint.h>

/* Logical cdecl APIs. Void APIs expose owned memory effects; incidental
   native volatile outputs remain available through existing RAW entries. */
void restore_city_apply_disaster_at_cell(int32_t x,int32_t y,uint32_t cell);
void restore_city_tick_burning_fields(void);

/* The native selector overwrites incoming EAX. Its upper 24 result bits are
   ((uint32_t)(int32_t)signed_type - 183) & 0xffffff00, with only AL set0/1.
   No incoming-register argument is required for this full result. */
uint32_t restore_building_disaster_spread_result(int32_t building);
uint8_t restore_building_allows_disaster_spread(int32_t building);

void FUN_00469140(int32_t x,int32_t y,int32_t cell);
uint32_t FUN_004696b0(int32_t building);
void FUN_00469730(void);

/* Existing invasion RAW models, supplied by the shared root integration.
   Both are cdecl/RET0 and no volatile output is consumed by these callers. */
void restore_city_disaster_actors_004b7510(int32_t x,int32_t y);
void restore_city_disaster_refresh_00497b90(void);
#endif
