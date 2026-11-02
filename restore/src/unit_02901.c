#include "restore_types.h"
#define DAT_01266480 (*(undefined *)0x01266480u)

undefined4 FUN_005653d0(undefined4 param_1)

{
  int iVar1;
  
  if (DAT_01266480 != (code *)0x0) {
    iVar1 = ((code *)DAT_01266480)(param_1);
    if (iVar1 != 0) {
      return 1;
    }
  }
  return 0;
}
