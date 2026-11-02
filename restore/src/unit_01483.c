#include "restore_types.h"
#define DAT_00bcd786 (*(undefined4 *)0x00bcd786u)

void FUN_004b7920(void)

{
  byte *pbVar1;
  
  pbVar1 = &DAT_00bcd786;
  do {
    if ((((pbVar1[4] == 1) && (10 < *pbVar1)) && (*pbVar1 < 0xe)) &&
       ((pbVar1[0x2e] == 8 && (pbVar1[0x13] != 0)))) {
      pbVar1[0x13] = pbVar1[0x13] - 1;
    }
    pbVar1 = pbVar1 + 0x184;
  } while ((int)pbVar1 < 0xc8ad42);
  return;
}
