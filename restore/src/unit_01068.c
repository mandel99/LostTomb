#include "restore_types.h"
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca2dc (*(undefined *)0x00bca2dcu)
extern void FUN_00477dd0();
extern undefined4 FUN_00477eb0();

undefined4 FUN_00477f70(int param_1,int param_2)

{
  char cVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  uint uVar6;
  uint *puVar7;
  undefined1 local_9;
  
  uVar3 = ((int)(param_1 - 1U) < 0) - 1 & param_1 - 1U;
  iVar5 = param_1 + 1;
  local_9 = 0;
  param_1 = DAT_00bca2d0;
  if (iVar5 <= DAT_00bca2d0) {
    param_1 = iVar5;
  }
  uVar4 = ((int)(param_2 - 1U) < 0) - 1 & param_2 - 1U;
  iVar5 = param_2 + 1;
  param_2 = DAT_00bca2d4;
  if (iVar5 <= DAT_00bca2d4) {
    param_2 = iVar5;
  }
  iVar2 = (DAT_00bca2dc - param_1) + DAT_00bca2d0;
  iVar5 = DAT_00bca2d8 + uVar4 * 0xe4 + uVar3;
  for (; (int)uVar4 <= param_2; uVar4 = uVar4 + 1) {
    if ((int)uVar3 <= param_1) {
      puVar7 = &DAT_00af598c + iVar5;
      uVar6 = uVar3;
      do {
        if (((*puVar7 & 0x10004) == 0) && (cVar1 = FUN_00477eb0(iVar5,0x10004,1), cVar1 == '\0')) {
          *puVar7 = *puVar7 & 0xd9cbf680 | 0x10000;
          local_9 = 1;
          FUN_00477dd0(uVar6,uVar4,2);
          FUN_00477f70(uVar6,uVar4);
        }
        uVar6 = uVar6 + 1;
        iVar5 = iVar5 + 1;
        puVar7 = puVar7 + 1;
      } while ((int)uVar6 <= param_1);
    }
    iVar5 = iVar5 + iVar2 + -1 + uVar3;
  }
  return CONCAT31((int3)((uint)param_2 >> 8),local_9);
}
