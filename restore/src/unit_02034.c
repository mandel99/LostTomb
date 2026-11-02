#include "restore_types.h"
#include <stdint.h>

void FUN_004e8e80(int8_t delta)

{
  int8_t *value = (int8_t *)(uintptr_t)0x00799ba2u;
  const int8_t *end = (const int8_t *)(uintptr_t)0x0089b79au;

  while (value < end) {
    if (value[-0xc2] == 1 && value[-0xbd] != 0) {
      int8_t updated = (int8_t)(*value + delta);
      if (updated < 0) updated = 0;
      if (updated > 100) updated = 100;
      *value = updated;
    }
    value += 0x108;
  }
}
