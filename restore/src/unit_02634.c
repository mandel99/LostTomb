#include "restore_types.h"
#define DAT_005d2a3c (*(undefined4 *)0x005d2a3cu)
#define DAT_0077de6f (*(undefined4 *)0x0077de6fu)
#define DAT_00ea40ba (*(undefined4 *)0x00ea40bau)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern bool FUN_00447790();

int FUN_00559fc0(int *param_1)

{
  short sVar1;
  char cVar2;
  int iVar3;
  char *pcVar4;
  int iVar5;
  
  iVar3 = 0;
  iVar5 = 1;
  pcVar4 = &DAT_0077de6f;
  do {
    if ((((pcVar4[-5] != '\0') && (*pcVar4 != '\0')) &&
        ((sVar1 = *(short *)((int)&DAT_00ea40ba + DAT_00eaac28 * 0x49d8 + 2), sVar1 == 1 ||
         (sVar1 == 3)))) && (pcVar4[0x38] != '\0')) {
      cVar2 = FUN_00447790(iVar5,0x13);
      if (cVar2 != '\0') {
        param_1[*(int *)(&DAT_005d2a3c + (uint)(byte)pcVar4[-2] * 8) + -0x26] =
             param_1[*(int *)(&DAT_005d2a3c + (uint)(byte)pcVar4[-2] * 8) + -0x26] + 1;
      }
    }
    pcVar4 = pcVar4 + 0x6a;
    iVar5 = iVar5 + 1;
  } while ((int)pcVar4 < 0x77f747);
  iVar5 = 0x10;
  do {
    if (*param_1 != 0) {
      iVar3 = iVar3 + 1;
    }
    param_1 = param_1 + 1;
    iVar5 = iVar5 + -1;
  } while (iVar5 != 0);
  return iVar3;
}
