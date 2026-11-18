#ifndef PHARAOH_CORE_PERCENTAGE_ADJUST_H
#define PHARAOH_CORE_PERCENTAGE_ADJUST_H

#include <stdint.h>

/* Original entry 0x004CDE40. Returns the signed 32-bit product multiplied by
   percentage, divided by 100 with truncation toward zero. */
int32_t pharaoh_adjust_by_percentage(int32_t value, int32_t percentage);

#endif
