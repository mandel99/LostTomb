#include "restore_types.h"
#define DAT_00bcba9a (*(undefined4 *)0x00bcba9au)

int FUN_004b78f0(void)

{
  int iVar1;
  char *pcVar2;
  
  iVar1 = 0;
  pcVar2 = &DAT_00bcba9a;
  do {
    if ((pcVar2[-0x42] == '\x01') && (*pcVar2 != '\0')) {
      iVar1 = iVar1 + 1;
    }
    pcVar2 = pcVar2 + 0x90;
  } while ((int)pcVar2 < 0xbcd62a);
  return iVar1;
}
