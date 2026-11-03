#ifndef RESTORE_HOUSING_CAPACITY_H
#define RESTORE_HOUSING_CAPACITY_H
#include <stdint.h>
void restore_building_decay_timers(void);
void restore_housing_rebuild_list(void);
void restore_migration_reset_monthly(void);
void restore_housing_rebuild_capacity(void);
void restore_migration_calculate_demand(int32_t player);
void FUN_004583f0(void);
void FUN_004e8610(void);
void FUN_004e8f80(int32_t player);
void FUN_004e2fb0(int32_t tool);
#endif
