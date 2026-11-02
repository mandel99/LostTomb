#include "restore_types.h"
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca2dc (*(undefined *)0x00bca2dcu)
extern void FUN_0047a120();

void FUN_00477890(int param_1,int param_2,int param_3,int param_4)

{
  uint uVar1;
  int iVar2;
  uint *puVar3;
  int iVar4;
  int iVar5;
  
  if (param_1 < 0) {
    param_1 = 0;
  }
  if (param_2 < 0) {
    param_2 = 0;
  }
  if (DAT_00bca2d0 <= param_3) {
    param_3 = DAT_00bca2d0 + -1;
  }
  if (DAT_00bca2d4 <= param_4) {
    param_4 = DAT_00bca2d4 + -1;
  }
  iVar2 = DAT_00bca2d0 - param_3;
  iVar5 = DAT_00bca2d8 + param_2 * 0xe4 + param_1;
  for (; param_2 <= param_4; param_2 = param_2 + 1) {
    if (param_1 <= param_3) {
      puVar3 = &DAT_00af598c + iVar5;
      iVar5 = iVar5 + 1 + (param_3 - param_1);
      iVar4 = param_1;
      do {
        uVar1 = *puVar3;
        if ((((uVar1 & 0x1000) != 0) && ((uVar1 & 0x700) == 0)) && ((uVar1 & 0x68) == 0)) {
          FUN_0047a120(iVar4,param_2);
        }
        iVar4 = iVar4 + 1;
        puVar3 = puVar3 + 1;
      } while (iVar4 <= param_3);
    }
    iVar5 = iVar5 + DAT_00bca2dc + iVar2 + -1 + param_1;
  }
  return;
}
