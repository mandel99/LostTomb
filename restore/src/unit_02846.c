#include "restore_types.h"
#define DAT_01266408 (*(undefined *)0x01266408u)
extern void FUN_00567dcc();
extern void FUN_00567e05();

void FUN_00563105(undefined4 param_1)

{
  if (DAT_01266408 == 1) {
    FUN_00567dcc();
  }
  FUN_00567e05(param_1);
                    /* WARNING: Subroutine does not return */
  ExitProcess(0xff);
}
