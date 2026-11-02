#include "restore_types.h"
#include "../runtime/city_history.h"

void FUN_004e9700(void)

{
  uint32_t selector = *(uint32_t *)(uintptr_t)0x00eaac28u;
  void *city = (void *)(uintptr_t)(0x00ea0000u + selector * 0x49d8u);
  restore_city_history_append(city);
}
