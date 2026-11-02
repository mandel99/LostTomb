#include "restore_types.h"
extern void FUN_0056a904();

void FUN_0056ab24(undefined2 *param_1,uint param_2,int param_3)

{
  uint uVar1;
  uint uVar2;
  int iVar3;
  undefined4 *puVar4;
  undefined2 local_10;
  undefined4 local_e;
  undefined2 uStack_a;
  undefined4 uStack_8;
  
  iVar3 = 0x605438;
  if (param_2 != 0) {
    if ((int)param_2 < 0) {
      param_2 = -param_2;
      iVar3 = 0x605598;
    }
    if (param_3 == 0) {
      *param_1 = 0;
    }
    while (param_2 != 0) {
      iVar3 = iVar3 + 0x54;
      uVar1 = (int)param_2 >> 3;
      uVar2 = param_2 & 7;
      param_2 = uVar1;
      if (uVar2 != 0) {
        puVar4 = (undefined4 *)(iVar3 + uVar2 * 0xc);
        if (0x7fff < *(ushort *)(iVar3 + uVar2 * 0xc)) {
          local_10 = (undefined2)*puVar4;
          RESTORE_SLICE2(local_e,0) = (undefined2)((uint)*puVar4 >> 0x10);
          RESTORE_SLICE2(local_e,2) = (undefined2)puVar4[1];
          uStack_a = (undefined2)((uint)puVar4[1] >> 0x10);
          uStack_8 = puVar4[2];
          local_e = CONCAT22(RESTORE_SLICE2(local_e,2),(undefined2)local_e) + -1;
          puVar4 = (undefined4 *)&local_10;
        }
        FUN_0056a904(param_1,puVar4);
      }
    }
  }
  return;
}
