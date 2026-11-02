#include "restore_types.h"
#include "../runtime/city_history.h"

int FUN_004e9760(int history_offset,int ring_index)

{
  uint32_t selector = *(uint32_t *)(uintptr_t)0x00eaac28u;
  const void *city = (const void *)(uintptr_t)(0x00ea0000u + selector * 0x49d8u);
  return restore_city_history_get(city,history_offset,ring_index);
}
