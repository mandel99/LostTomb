#include "restore_types.h"

void FUN_00484ee0(int param_1,int param_2)

{
  undefined1 *puVar1;
  
  if (0 < param_2) {
    puVar1 = (undefined1 *)(param_1 + 0xe);
    do {
      *puVar1 = 0;
      puVar1 = puVar1 + 0x10;
      param_2 = param_2 + -1;
    } while (param_2 != 0);
  }
  return;
}
