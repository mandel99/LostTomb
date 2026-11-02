#include "restore_types.h"
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca2dc (*(undefined *)0x00bca2dcu)
extern void FUN_0047ab10();

void FUN_004776f0(int param_1,int param_2,int param_3,int param_4)

{
  int iVar1;
  uint *puVar2;
  int iVar3;
  int iVar4;
  
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
  iVar1 = DAT_00bca2d0 - param_3;
  iVar4 = DAT_00bca2d8 + param_2 * 0xe4 + param_1;
  for (; param_2 <= param_4; param_2 = param_2 + 1) {
    if (param_1 <= param_3) {
      puVar2 = &DAT_00af598c + iVar4;
      iVar4 = iVar4 + 1 + (param_3 - param_1);
      iVar3 = param_1;
      do {
        if (((*puVar2 & 0x40000) != 0) && ((*puVar2 & 8) == 0)) {
          FUN_0047ab10(iVar3,param_2,1);
        }
        iVar3 = iVar3 + 1;
        puVar2 = puVar2 + 1;
      } while (iVar3 <= param_3);
    }
    iVar4 = iVar4 + DAT_00bca2dc + iVar1 + -1 + param_1;
  }
  return;
}
