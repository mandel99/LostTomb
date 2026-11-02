#include "restore_types.h"
#define DAT_00775930 (*(undefined4 *)0x00775930u)
#define DAT_0077de05 (*(undefined4 *)0x0077de05u)

void FUN_004448e0(void)

{
  int iVar1;
  char *pcVar2;
  undefined4 *puVar3;
  
  pcVar2 = &DAT_0077de05;
  do {
    if ((pcVar2[-5] != '\0') && (*pcVar2 != '\0')) {
      puVar3 = (undefined4 *)(&DAT_00775930 + (uint)(byte)pcVar2[-1] * 0x90);
      for (iVar1 = 0x23; iVar1 != 0; iVar1 = iVar1 + -1) {
        *puVar3 = 0;
        puVar3 = puVar3 + 1;
      }
    }
    pcVar2 = pcVar2 + 0x6a;
  } while ((int)pcVar2 < 0x77f747);
  return;
}
