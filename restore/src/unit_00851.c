#include "restore_types.h"
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_00799b2e (*(undefined4 *)0x00799b2eu)

void FUN_004599a0(void)

{
  char *pcVar1;
  int iVar2;
  
  if (0 < DAT_0078d790) {
    pcVar1 = &DAT_00799b2e;
    iVar2 = DAT_0078d790;
    do {
      if ((pcVar1[-0x4e] == '\x01') && (*pcVar1 != '\0')) {
        *pcVar1 = *pcVar1 + -1;
      }
      pcVar1 = pcVar1 + 0x108;
      iVar2 = iVar2 + -1;
    } while (iVar2 != 0);
  }
  return;
}
