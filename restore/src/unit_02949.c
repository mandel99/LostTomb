#include "restore_types.h"
#define _DAT_01266430 (*(undefined4 *)0x01266430u)

void FUN_00568422(int param_1)

{
  if (param_1 == 1) {
    _DAT_01266430 = 0x21;
  }
  else if ((1 < param_1) && (param_1 < 4)) {
    _DAT_01266430 = 0x22;
    return;
  }
  return;
}
