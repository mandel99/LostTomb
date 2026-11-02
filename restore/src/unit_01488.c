#include "restore_types.h"
#define DAT_00bcbac1 (*(undefined4 *)0x00bcbac1u)
extern void FUN_004b7b50();

void FUN_004b7c00(undefined4 param_1)

{
  char *pcVar1;
  int iVar2;
  
  iVar2 = 1;
  pcVar1 = &DAT_00bcbac1;
  do {
    if (((pcVar1[-0x69] == '\x01') && (*pcVar1 == '\0')) && (pcVar1[-0x27] != '\0')) {
      FUN_004b7b50(iVar2,param_1);
    }
    pcVar1 = pcVar1 + 0x90;
    iVar2 = iVar2 + 1;
  } while ((int)pcVar1 < 0xbcd651);
  return;
}
