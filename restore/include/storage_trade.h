#ifndef RESTORE_STORAGE_TRADE_H
#define RESTORE_STORAGE_TRADE_H
#include <stdint.h>
int32_t restore_trade_can_export(int32_t player,int32_t trader,int32_t resource);
int32_t restore_trade_can_import(int32_t player,int32_t trader,int32_t resource);
int32_t restore_storage_can_export(int32_t actor,int32_t player,int32_t building,int32_t trader);
int32_t restore_storage_can_import(int32_t actor,int32_t player,int32_t building,int32_t trader);
int32_t restore_food_delivery_destination(int32_t force,int32_t actor,int32_t resource,uint32_t network);
int32_t FUN_004434e0(int32_t player,int32_t trader,int32_t resource);
int32_t FUN_004435f0(int32_t player,int32_t trader,int32_t resource);
int32_t FUN_0045cf50(int32_t actor,int32_t player,int32_t building,int32_t trader);
int32_t FUN_0045d180(int32_t actor,int32_t player,int32_t building,int32_t trader);
#endif
