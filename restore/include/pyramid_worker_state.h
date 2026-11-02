#ifndef RESTORE_PYRAMID_WORKER_STATE_H
#define RESTORE_PYRAMID_WORKER_STATE_H
#include "pyramid_work_selection.h"
void restore_pyramid_worker_update_travel(int32_t actor,int32_t mode,uint8_t failed_state,uint8_t ready_state);
void restore_pyramid_sort_work_list(void);
void restore_pyramid_sort_entries(uint32_t count);
uint8_t restore_pyramid_linked_worker_active(int32_t actor,int32_t owner);
uint8_t restore_pyramid_is_second_work_piece(int32_t building);
int32_t restore_pyramid_find_available_worker(int32_t building,uint8_t detach_waiting);
void FUN_004f04d0(int32_t actor,int32_t mode,uint8_t failed_state,uint8_t ready_state);
void FUN_004f25c0(void);
uint8_t FUN_004f26a0(int32_t actor,int32_t owner);
uint8_t FUN_004f28b0(int32_t building);
int32_t FUN_004f29e0(int32_t building,uint8_t detach_waiting);
#endif
