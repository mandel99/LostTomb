#ifndef RESTORE_MONUMENT_LAYOUT_QUERIES_H
#define RESTORE_MONUMENT_LAYOUT_QUERIES_H
#include "monument_worker_motion.h"
void restore_monument_refresh_all(void);
void FUN_004ee560(void);
uint32_t restore_monument_dispatch_work(uint32_t x,uint32_t y,int32_t actor);
uint32_t FUN_004f0190(uint32_t x,uint32_t y,int32_t actor);
int32_t restore_monument_find_layout_pair(int32_t building);
int32_t FUN_004f2720(int32_t building);
void restore_monument_set_layout_target(int32_t actor,int32_t building);
void FUN_004f3000(int32_t actor,int32_t building);
/* Exact EAX bits. Indirect consumers have not established a narrower API. */
uint32_t restore_monument_worker_query_raw(int32_t building);
uint32_t FUN_004f5d10(int32_t building);
#endif
