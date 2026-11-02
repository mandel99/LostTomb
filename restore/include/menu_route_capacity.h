#ifndef RESTORE_MENU_ROUTE_CAPACITY_H
#define RESTORE_MENU_ROUTE_CAPACITY_H
#include <stdint.h>
/* Six-byte signed-WORD point records, at most50 vertices. stop_after is the
   original route generator's limit, checked before each segment. This helper
   computes required sample slots without writing any point/sample buffer. */
int restore_menu_route_required_samples(const uint8_t *points,uint32_t count,
                                       uint8_t step,uint32_t stop_after,uint32_t *required);
#endif
