#ifndef RESTORE_STORAGE_TRANSACTIONS_H
#define RESTORE_STORAGE_TRANSACTIONS_H
#include <stdint.h>
int32_t restore_storage_export(int32_t player,int32_t building,uint32_t trader);
int32_t restore_storage_import(int32_t player,int32_t building,uint32_t trader);
void restore_trade_charge_import(int32_t player,uint32_t building,int32_t resource);
void restore_storage_rank_gathering(int32_t building,int32_t *first_resource,int32_t *second_resource,int32_t *first_percent,int32_t *second_percent,int32_t *first_source,int32_t *second_source);
int32_t restore_storage_find_source(int32_t building,int32_t resource,char require_stock);
int32_t restore_storage_distance(int32_t x1,int32_t y1,int32_t x2,int32_t y2,int32_t region1,int32_t region2);
#endif
