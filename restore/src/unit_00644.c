#include "restore_types.h"
#define DAT_0077f748 (*(undefined4 *)0x0077f748u)
extern undefined4 FUN_00443000();
extern undefined4 FUN_00443040();
extern byte FUN_00444a00();
extern void FUN_00444a40();

void FUN_00444a80(void)

{
  int iVar1;
  int iVar2;
  char *pcVar3;
  int iVar4;
  int iVar5;
  
  iVar5 = 0;
  pcVar3 = &DAT_0077f748;
  do {
    if ((pcVar3[1] != '\0') && (*pcVar3 == '\x01')) {
      iVar2 = 0;
      iVar4 = 1;
      do {
        iVar1 = FUN_00444a00(iVar5,iVar4);
        iVar2 = iVar2 + iVar1;
        iVar4 = iVar4 + 1;
      } while (iVar4 < 0x24);
      if (iVar2 == 0) {
        iVar2 = 1;
        do {
          FUN_00444a40(iVar5,iVar2,2);
          iVar2 = iVar2 + 1;
        } while (iVar2 < 0x24);
      }
      iVar2 = 1;
      do {
        iVar4 = FUN_00443000(iVar5,iVar2);
        if (iVar4 == 0) {
          iVar4 = FUN_00443040(iVar5,iVar2);
          if (iVar4 == 0) {
            FUN_00444a40(iVar5,iVar2,0);
          }
        }
        iVar2 = iVar2 + 1;
      } while (iVar2 < 0x24);
    }
    pcVar3 = pcVar3 + 0x62;
    iVar5 = iVar5 + 1;
  } while ((int)pcVar3 < 0x7843d8);
  return;
}
