#include "restore_types.h"
#define DAT_0078d790 (*(int32_t *)0x0078d790u)
#define DAT_00799af0 (*(undefined4 *)0x00799af0u)

int FUN_0046f190(void)

{
  int iVar1;
  short *psVar2;
  int iVar3;
  
  iVar1 = 0;
  if (0 < DAT_0078d790) {
    psVar2 = &DAT_00799af0;
    iVar3 = DAT_0078d790;
    do {
      if (((char)psVar2[-8] == '\x01') && (*psVar2 == 0xb6)) {
        iVar1 = iVar1 + 1;
      }
      psVar2 = psVar2 + 0x84;
      iVar3 = iVar3 + -1;
    } while (iVar3 != 0);
  }
  return iVar1;
}
