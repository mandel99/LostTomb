#include "restore_types.h"
#define DAT_00576098 (*(undefined4 *)0x00576098u)
#define DAT_005760a0 (*(undefined4 *)0x005760a0u)
#define DAT_005760a4 (*(undefined4 *)0x005760a4u)
#define DAT_005760ac (*(undefined4 *)0x005760acu)
#define DAT_01266470 (*(undefined1 *)0x01266470u)
#define DAT_01266478 (*(undefined *)0x01266478u)
#define DAT_01266a88 (*(undefined *)0x01266a88u)
#define DAT_01266a8c (*(undefined *)0x01266a8cu)
#define _DAT_01266474 (*(undefined4 *)0x01266474u)
extern void FUN_005647a8();

void FUN_0056470f(UINT param_1,int param_2,int param_3)

{
  HANDLE hProcess;
  undefined4 *puVar1;
  UINT uExitCode;
  
  if (DAT_01266478 == 1) {
    uExitCode = param_1;
    hProcess = GetCurrentProcess();
    TerminateProcess(hProcess,uExitCode);
  }
  _DAT_01266474 = 1;
  DAT_01266470 = (undefined1)param_3;
  if (param_2 == 0) {
    if ((DAT_01266a8c != (undefined4 *)0x0) &&
       (puVar1 = (undefined4 *)(DAT_01266a88 - 4), DAT_01266a8c <= puVar1)) {
      do {
        if ((code *)*puVar1 != (code *)0x0) {
          (*(code *)*puVar1)();
        }
        puVar1 = puVar1 + -1;
      } while (DAT_01266a8c <= puVar1);
    }
    FUN_005647a8(&DAT_00576098,&DAT_005760a0);
  }
  FUN_005647a8(&DAT_005760a4,&DAT_005760ac);
  if (param_3 != 0) {
    return;
  }
  DAT_01266478 = 1;
                    /* WARNING: Subroutine does not return */
  ExitProcess(param_1);
}
