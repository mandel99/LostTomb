#include "restore_types.h"
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_00799afc (*(undefined4 *)0x00799afcu)

int FUN_0042fca0(void)

{
  int iVar1;
  short *psVar2;
  int iVar3;
  int iVar4;
  
  iVar3 = 1;
  iVar4 = 0;
  iVar1 = 0;
  if (0 < DAT_0078d790) {
    psVar2 = &DAT_00799afc;
    do {
      if (((((char)psVar2[-0xe] != '\0') && (*psVar2 != 0)) &&
          (*(char *)((int)psVar2 + -0x17) != '\0')) && (iVar4 < *(int *)(psVar2 + 0x70))) {
        iVar1 = iVar3;
        iVar4 = *(int *)(psVar2 + 0x70);
      }
      iVar3 = iVar3 + 1;
      psVar2 = psVar2 + 0x84;
    } while (iVar3 <= DAT_0078d790);
  }
  return iVar1;
}
