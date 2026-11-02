#ifndef RESTORE_MAP_COMMIT_PROXIMITY_H
#define RESTORE_MAP_COMMIT_PROXIMITY_H
#include <stdint.h>
/* Native callers test AL. The full EAX result retains its point-table address
 * family: WORD hit00784B01, DWORD hit00784C01, no hit00784C00. */
uint32_t FUN_00470900(int32_t x1,int32_t y1,int32_t x2,int32_t y2);
uint32_t FUN_004b5830(int32_t x1,int32_t y1,int32_t x2,int32_t y2);
void FUN_004b9ee0(int32_t x,int32_t y);
#endif
