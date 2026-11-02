#include "restore_types.h"

void FUN_0041acd0(byte *param_1)

{
  byte bVar1;
  byte *pbVar2;
  
  bVar1 = *param_1;
  while ((bVar1 != 0 && (pbVar2 = param_1, bVar1 < 0x21))) {
    while (bVar1 != 0) {
      *pbVar2 = pbVar2[1];
      bVar1 = pbVar2[1];
      pbVar2 = pbVar2 + 1;
    }
    bVar1 = *param_1;
  }
  bVar1 = *param_1;
  pbVar2 = (byte *)0x0;
  if (bVar1 != 0) {
    do {
      if (0x20 < bVar1) {
        pbVar2 = param_1;
      }
      bVar1 = param_1[1];
      param_1 = param_1 + 1;
    } while (bVar1 != 0);
    if (pbVar2 != (byte *)0x0) {
      pbVar2[1] = 0;
    }
  }
  return;
}
