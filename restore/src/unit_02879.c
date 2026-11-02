#include "restore_types.h"
#define DAT_01266424 (*(undefined4 *)0x01266424u)
extern undefined1 * FUN_00564367();

void FUN_00564507(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  DAT_01266424 = 1;
  FUN_00564367(param_1,param_2,param_3);
  DAT_01266424 = 0;
  return;
}
