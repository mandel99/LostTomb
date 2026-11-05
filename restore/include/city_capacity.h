#ifndef RESTORE_CITY_CAPACITY_H
#define RESTORE_CITY_CAPACITY_H
#include <stdint.h>
/* Whole-function logical interfaces; no incoming GPR/FPU stack operands.
   Service coverage retains the caller's x87 control word. */
void restore_city_rebuild_building_counts(void);
void restore_city_rebuild_service_coverage(void);
void restore_city_rebuild_building_requirements(void);
#endif
