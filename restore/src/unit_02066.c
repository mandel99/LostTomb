#include "restore_types.h"
#include <stdint.h>

int FUN_004ea9c0(int param_1)

{
  uint32_t upper = ((uint32_t)param_1 - 0x52u) & 0xffffff00u;
  switch((int)(param_1)) {
  case 0x52: ;
  case 0x53: ;
  case 0x54: ;
  case 0x66: ;
  case 0x67: ;
  case 0x68: ;
    return (int)(upper | 1u);
default: ;
    return (int)upper;
  }
}
