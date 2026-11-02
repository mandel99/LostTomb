#include "restore_types.h"
#define DAT_0077de04 (*(undefined4 *)0x0077de04u)

int FUN_004449d0(uint param_1)

{
  int iVar1;
  byte *pbVar2;
  
  iVar1 = 0;
  pbVar2 = &DAT_0077de04;
  while ((pbVar2[-4] == 0 || (*pbVar2 != param_1))) {
    pbVar2 = pbVar2 + 0x6a;
    iVar1 = iVar1 + 1;
    if (0x77f745 < (int)pbVar2) {
      return 0;
    }
  }
  return iVar1;
}
