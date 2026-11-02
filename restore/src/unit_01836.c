#include "restore_types.h"
#define DAT_00605a44 (*(int32_t *)0x00605a44u)

int FUN_004d5d70(int param_1)

{
  if (param_1 == 7) {
    param_1 = DAT_00605a44 + 100;
  }
  return param_1;
}
