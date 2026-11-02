#include "restore_types.h"
#define DAT_00e95124 (*(undefined4 *)0x00e95124u)

void FUN_004e1850(void)

{
  int *piVar1;
  
  piVar1 = &DAT_00e95124;
  do {
    if (0 < *piVar1) {
      *piVar1 = *piVar1 + -1;
    }
    piVar1 = piVar1 + 1;
  } while ((int)piVar1 < 0xe95174);
  return;
}
