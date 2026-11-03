#ifndef RESTORE_POPULATION_DEMOGRAPHY_H
#define RESTORE_POPULATION_DEMOGRAPHY_H
#include <stdint.h>
int32_t restore_population_add_births(int32_t player,int32_t requested);
int32_t restore_population_remove_deaths(int32_t player,int32_t requested);
int32_t restore_population_age_group(int32_t player,int32_t group);
void restore_population_remove_age_group(int32_t player,int32_t group,int32_t amount);
void restore_population_age_and_deaths(void);
void restore_population_births(void);
void restore_population_rebuild_totals(int32_t player);
int32_t restore_population_working_age(void);
void restore_population_rebuild_housing_mix(void);
void FUN_004e87a0(int32_t player);
int32_t FUN_004e9460(void);
void FUN_00458880(void);
#endif
