#ifndef RESTORE_ACTOR_CITY_REFRESH_H
#define RESTORE_ACTOR_CITY_REFRESH_H
#include <stdint.h>

/* Logical cdecl memory-effect APIs. Incidental native register/flag results
   remain available through the existing invasion/city RAW implementations. */
void restore_actor_refresh_sentries_on_marked_tiles(void);
void restore_actor_mark_sentries_near_cell(int32_t x,int32_t y);
void restore_city_clear_periodic_counters(void);

void FUN_00497b90(void);
void FUN_004b7510(int32_t x,int32_t y);
void FUN_0046a060(void);
#endif
