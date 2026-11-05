#ifndef RESTORE_CITY_MAINTENANCE_H
#define RESTORE_CITY_MAINTENANCE_H
#include <stdint.h>
/* Complete logical void contracts. The existing physical RAW entries remain
   available for callers that observe incidental EAX, flags or registers. */
void restore_city_rebuild_road_regions(void);       /* 00468860 */
void restore_city_update_building_hazards(void);    /* 00468b70 */
void restore_city_decay_overlay_counters(void);    /* 0047e290 */
/* Reused original-owner C children; logical memory effects only. */
#include "city_region_children.h"
#endif
