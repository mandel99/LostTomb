#ifndef RESTORE_STORAGE_DELIVERY_H
#define RESTORE_STORAGE_DELIVERY_H
#include <stdint.h>
int32_t restore_storage_resource_amount(int32_t building,int32_t resource);
uint32_t restore_storage_limit_exceeded(int32_t building,int32_t amount,int32_t resource);
int32_t restore_storage_pile_frame(int32_t amount);
int32_t restore_storage_deliver(int32_t player,int32_t building,int32_t resource,int32_t amount);
uint32_t FUN_0045b230(int32_t building,int32_t amount,int32_t resource);
int32_t FUN_0045d510(int32_t building,int32_t resource);
int32_t FUN_0045b490(int32_t amount);
int32_t FUN_0045b2a0(int32_t player,int32_t building,int32_t resource,int32_t amount);
int32_t restore_storage_withdraw(int32_t player,int32_t building,int32_t resource,int32_t amount);
void restore_storage_remove_any(int32_t building,int32_t amount);
int32_t restore_storage_withdraw_city(int32_t player,int32_t resource,int32_t amount);
int32_t FUN_0045b570(int32_t player,int32_t building,int32_t resource,int32_t amount);
void FUN_0045b700(int32_t building,int32_t amount);
int32_t FUN_0045b4d0(int32_t player,int32_t resource,int32_t amount);
#endif
