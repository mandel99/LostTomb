#ifndef RESTORE_CITY_HISTORY_H
#define RESTORE_CITY_HISTORY_H
#include <stdint.h>

/* A city record is caller-owned; these functions need no original executable. */
int32_t restore_city_sum_bins(const void *city, uint32_t first, uint32_t count);
void restore_city_history_append(void *city);
int32_t restore_city_history_get(const void *city, int32_t history_offset, int32_t ring_index);
#endif
