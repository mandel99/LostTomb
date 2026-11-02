#include "restore_types.h"
#define DAT_01266960 (*(uint32_t *)0x01266960u)
#define DAT_01266a60 (*(uint32_t *)0x01266a60u)

byte FUN_005691bf(uint param_1)

{
  if (DAT_01266a60 <= param_1) {
    return 0;
  }
  return *(byte *)((&DAT_01266960)[(int)param_1 >> 5] + 4 + (param_1 & 0x1f) * 8) & 0x40;
}
