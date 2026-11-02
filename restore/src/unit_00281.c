#include "restore_types.h"
#define DAT_00702984 (*(undefined4 *)0x00702984u)

void FUN_0041d6f0(void)

{
  int *piVar1;
  int *piVar2;
  
  piVar1 = &DAT_00702984;
  do {
    piVar2 = piVar1 + 0x20;
    piVar1[2] = 0;
    piVar1[3] = 0;
    piVar1[4] = 0;
    piVar1[5] = 0;
    piVar1[6] = 0;
    *piVar1 = *piVar1 / 2;
    piVar1 = piVar2;
  } while ((int)piVar2 < 0x704c04);
  return;
}
