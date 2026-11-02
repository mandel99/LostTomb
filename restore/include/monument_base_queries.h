#ifndef RESTORE_MONUMENT_BASE_QUERIES_H
#define RESTORE_MONUMENT_BASE_QUERIES_H
#include <stdint.h>
void restore_monument_base_initialize(void);
uint8_t restore_monument_base_worker(uint32_t token,uint32_t building);
int32_t restore_monument_base_0040d940(void *self,uint32_t building,int32_t *output);
int32_t restore_monument_base_00417050(void *self,uint32_t building,int32_t *output);
int32_t restore_monument_base_004e6710(void *self,uint32_t building,int32_t *output);
#endif
