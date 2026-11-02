#ifndef RESTORE_EARTHQUAKE_EVENT_H
#define RESTORE_EARTHQUAKE_EVENT_H
#include <stdint.h>
void restore_earthquake_list_tick(int32_t *list);
uint32_t restore_earthquake_tile_allowed(int32_t x,int32_t y);
void restore_earthquake_destroy_tile(int32_t x,int32_t y);
uint8_t restore_earthquake_tick(uintptr_t record);
uint32_t FUN_0044bd20(int32_t x,int32_t y);
void FUN_0044bd50(int32_t x,int32_t y);
#endif
