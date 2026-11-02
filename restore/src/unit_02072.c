#include "restore_types.h"
#include <stdint.h>

int FUN_004eada0(int param_1)

{
  uint32_t upper = ((uint32_t)param_1 - 0x19u) & 0xffffff00u;
  switch((int)(param_1)) {
  case 0x19: ;
  case 0x4d: ;
  case 0x4e: ;
  case 0x5c: ;
  case 0x5d: ;
  case 100: ;
  case 0x65: ;
    return (int)(upper | 1u);
default: ;
    return (int)upper;
  }
}
