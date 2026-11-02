#include "restore_types.h"
extern void FUN_0055d8b0();
extern void FUN_00561a2b();

void FUN_0055d960(int *param_1)

{
  int iVar1;
  
  iVar1 = *param_1;
  if (iVar1 != 0) {
    FUN_0055d8b0();
    FUN_00561a2b(iVar1);
    *param_1 = 0;
  }
  return;
}
