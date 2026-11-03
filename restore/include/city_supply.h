#ifndef RESTORE_CITY_SUPPLY_H
#define RESTORE_CITY_SUPPLY_H
#include <stdint.h>
uint32_t restore_storage_delivery_destination(int32_t source,uint32_t actor,int32_t resource,uint32_t network);
void restore_city_update_food(void);
void restore_city_rebuild_food(int32_t player);
void restore_city_update_industry(void);
void restore_city_rebuild_industry(int32_t player);
void FUN_0045e6e0(int32_t player);
#endif
