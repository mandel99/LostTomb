#include "restore_types.h"
#include <stdint.h>

undefined4 FUN_004ea9a0(int param_1)

{
  uint32_t upper = (uint32_t)param_1 & 0xffffff00u;
  return upper | ((param_1 >= 0x44 && param_1 <= 0x46) ? 1u : 0u);
}
