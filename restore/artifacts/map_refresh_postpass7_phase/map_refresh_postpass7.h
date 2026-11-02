#ifndef RESTORE_MAP_REFRESH_POSTPASS7_H
#define RESTORE_MAP_REFRESH_POSTPASS7_H
#include "map_refresh_postpass6.h"
#ifdef MAP_REFRESH_POSTPASS7_TRACE
uint32_t map_post7_read(uint32_t,uint32_t);
void map_post7_write(uint32_t,uint32_t,uint32_t);
void map_post7_phase(uint32_t);
#define P7_R(a,n) map_post7_read((uint32_t)(a),(n))
#define P7_W(a,n,v) map_post7_write((uint32_t)(a),(n),(uint32_t)(v))
#define P7_PHASE(p) map_post7_phase((p))
#else
#define P7_R(a,n) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define P7_W(a,n,v) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a)=(uint32_t)(v))
#define P7_PHASE(p) ((void)0)
#endif
void FUN_004877f0(int32_t,int32_t,int32_t,int32_t);
#endif
