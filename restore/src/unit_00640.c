#include "restore_types.h"
#define DAT_0077de04 (*(undefined4 *)0x0077de04u)
#define DAT_0077de05 (*(undefined4 *)0x0077de05u)

bool FUN_00444980(uint param_1)

{
  byte *pbVar1;
  int iVar2;
  
  iVar2 = 0;
  pbVar1 = &DAT_0077de04;
  while ((pbVar1[-4] == 0 || (*pbVar1 != param_1))) {
    pbVar1 = pbVar1 + 0x6a;
    iVar2 = iVar2 + 1;
    if (0x77f745 < (int)pbVar1) {
      return false;
    }
  }
  return (&DAT_0077de05)[iVar2 * 0x6a] != '\0';
}
