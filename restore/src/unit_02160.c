#include "restore_types.h"
#include <stdint.h>

int FUN_004f5020(void)
{
  uint32_t selector = *(uint32_t *)(uintptr_t)0x00eaac28u;
  const uint32_t *slots = (const uint32_t *)(uintptr_t)(0x00ea5fa0u + selector * 0x49d8u);
  return (slots[0] != 0) + (slots[1] != 0) + (slots[2] != 0);
}
