#include "restore_types.h"
#define DAT_00bcba9a (*(undefined4 *)0x00bcba9au)

int FUN_004b78b0(int param_1)

{
  int iVar1;
  char *pcVar2;
  int iVar3;
  
  iVar3 = 1;
  iVar1 = 1;
  pcVar2 = &DAT_00bcba9a;
  do {
    if ((pcVar2[-0x42] == '\x01') && (*pcVar2 != '\0')) {
      if (iVar3 == param_1) {
        return iVar1;
      }
      iVar3 = iVar3 + 1;
    }
    pcVar2 = pcVar2 + 0x90;
    iVar1 = iVar1 + 1;
  } while ((int)pcVar2 < 0xbcd62a);
  return 0;
}
