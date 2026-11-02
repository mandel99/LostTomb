#include "restore_types.h"
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)

int FUN_0043d010(int param_1)

{
  param_1 = param_1 - DAT_00bca2e0;
  if (param_1 < 0) {
    param_1 = param_1 + 8;
  }
  return param_1 / 2;
}
