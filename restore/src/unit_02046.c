#include "restore_types.h"
#include "../runtime/city_history.h"

int FUN_004e94a0(int param_1)

{
  const void *city = (const void *)(uintptr_t)(0x00ea0000u + (uint32_t)param_1 * 0x49d8u);
  return restore_city_sum_bins(city, 0u, 14u);
}
