#include "restore_types.h"
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_00799af8 (*(undefined4 *)0x00799af8u)

void FUN_004558b0(void)

{
  short sVar1;
  short *psVar2;
  int iVar3;
  
  if (0 < DAT_0078d790) {
    psVar2 = &DAT_00799af8;
    iVar3 = DAT_0078d790;
    do {
      if (((((char)psVar2[-0xc] != '\0') && (sVar1 = psVar2[-4], sVar1 != 0x3b)) && (sVar1 != 0xac))
         && (sVar1 != 0xad)) {
        if (*psVar2 < 2) {
          *psVar2 = 0;
        }
        else {
          *psVar2 = *psVar2 + -1;
        }
      }
      psVar2 = psVar2 + 0x84;
      iVar3 = iVar3 + -1;
    } while (iVar3 != 0);
  }
  return;
}
