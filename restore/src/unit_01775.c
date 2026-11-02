#include "restore_types.h"
#define DAT_00e38dac (*(int32_t *)0x00e38dacu)
#define DAT_00e38db0 (*(int32_t *)0x00e38db0u)

void FUN_004cfd60(int param_1)

{
  if (param_1 == 1) {
    DAT_00e38db0 = 0x280;
    DAT_00e38dac = 0x1e0;
    return;
  }
  if (param_1 == 2) {
    DAT_00e38db0 = 800;
    DAT_00e38dac = 600;
    return;
  }
  if (param_1 == 3) {
    DAT_00e38db0 = 0x400;
    DAT_00e38dac = 0x300;
  }
  return;
}
