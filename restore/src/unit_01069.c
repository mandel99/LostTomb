#include "restore_types.h"
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca2dc (*(undefined *)0x00bca2dcu)
extern void FUN_00477dd0();

undefined4 FUN_00478090(int param_1,uint param_2)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  uint *puVar5;
  uint uVar6;
  undefined1 local_9;
  
  uVar3 = ((int)(param_1 - 1U) < 0) - 1 & param_1 - 1U;
  iVar4 = param_1 + 1;
  local_9 = 0;
  param_1 = DAT_00bca2d0;
  if (iVar4 <= DAT_00bca2d0) {
    param_1 = iVar4;
  }
  uVar1 = param_2 - 1 & ((int)(param_2 - 1) < 0) - 1;
  uVar6 = param_2 + 1;
  param_2 = DAT_00bca2d4;
  if ((int)uVar6 <= (int)DAT_00bca2d4) {
    param_2 = uVar6;
  }
  iVar2 = (DAT_00bca2dc - param_1) + DAT_00bca2d0;
  iVar4 = DAT_00bca2d8 + uVar1 * 0xe4 + uVar3;
  uVar6 = uVar1;
  for (; (int)uVar1 <= (int)param_2; uVar1 = uVar1 + 1) {
    if ((int)uVar3 <= param_1) {
      puVar5 = &DAT_00af598c + iVar4;
      iVar4 = iVar4 + 1 + (param_1 - uVar3);
      uVar6 = uVar3;
      do {
        if ((*puVar5 & 0x10000) != 0) {
          *puVar5 = *puVar5 & 0xfffeffff;
          local_9 = 1;
          FUN_00477dd0(uVar6,uVar1,1);
          if ((*puVar5 & 0x22341a33) != 0) {
            FUN_00478090(uVar6,uVar1);
          }
        }
        uVar6 = uVar6 + 1;
        puVar5 = puVar5 + 1;
      } while ((int)uVar6 <= param_1);
    }
    iVar4 = iVar4 + iVar2 + -1 + uVar3;
    uVar6 = param_2;
  }
  return CONCAT31((int3)(uVar6 >> 8),local_9);
}
