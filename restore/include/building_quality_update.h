#ifndef RESTORE_BUILDING_QUALITY_UPDATE_H
#define RESTORE_BUILDING_QUALITY_UPDATE_H
#include <stdint.h>
/* Logical RET0/two-DWORD prefix. Only recalculate's low byte is consumed.
   Legacy4E7F00/4EB800/56133C global symbols are not rebound. */
void restore_building_quality_004e7f00(int32_t building,uint32_t recalculate);
/* Exact FILD signed32 -> FSQRT -> raw80 -> signed64 truncation. Exceptions
   masked, inherited PC/RC; CW and x87 stack preserved. FPSW equivalence is
   not claimed: software truncation does not reproduce FISTP exception flags. */
uint64_t restore_building_sqrt_trunc64(int32_t squared,uint8_t raw80[10]);
#endif
