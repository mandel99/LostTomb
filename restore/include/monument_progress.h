#ifndef RESTORE_MONUMENT_PROGRESS_H
#define RESTORE_MONUMENT_PROGRESS_H
#include <stdint.h>
uint8_t restore_monument_worker_active(uint32_t building);
int32_t restore_monument_progress_percent(int32_t sum,int32_t count);
int32_t restore_monument_progress(void *self,uint32_t building,int32_t count,int32_t *output);
int32_t restore_monument_progress_00545740(void *self,uint32_t building,int32_t *output);
int32_t restore_monument_progress_00558be0(void *self,uint32_t building,int32_t *output);
int32_t restore_monument_progress_00555e80(void *self,uint32_t building,int32_t *output);
int32_t restore_monument_progress_0054f470(void *self,uint32_t building,int32_t *output);
#endif
