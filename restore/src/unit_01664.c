#include "restore_types.h"

void FUN_004cbec0(byte *param_1)

{
  byte *pbVar1;
  byte bVar2;
  
  bVar2 = *param_1;
  while (bVar2 != 0) {
    if ((0x60 < bVar2) && (bVar2 < 0x7b)) {
      bVar2 = bVar2 - 0x20;
    }
    *param_1 = bVar2;
    pbVar1 = param_1 + 1;
    param_1 = param_1 + 1;
    bVar2 = *pbVar1;
  }
  return;
}
