#include "restore_types.h"
extern void FUN_0055d290();
extern void FUN_0055d2e0();
extern void FUN_0055d300();
extern undefined2 FUN_0055d750();
extern uint FUN_0055d840();

void FUN_0055d330(ushort param_1,undefined4 param_2,undefined4 param_3)

{
  ushort uVar1;
  undefined1 *puVar2;
  int iVar3;
  undefined4 uVar4;
  int in_ECX;
  int iVar5;
  undefined4 extraout_ECX;
  undefined4 extraout_ECX_00;
  uint uVar6;
  undefined2 uVar7;
  undefined2 extraout_var;
  short sVar8;
  
  if (*(int *)(in_ECX + 0x38) != 0) {
    if ((param_1 != 0) &&
       (iVar5 = *(int *)(*(int *)(in_ECX + 0x38) + 8), *(ushort *)(iVar5 + 0x1a) <= param_1)) {
      param_1 = *(short *)(iVar5 + 0x1a) - 1;
    }
    *(uint *)(in_ECX + 0x34) = *(uint *)(in_ECX + 0x34) & 0xfffffeff;
    *(ushort *)(in_ECX + 0x3c) = param_1;
    iVar5 = 0;
    if (*(int *)(in_ECX + 0x38) != 0) {
      iVar5 = *(int *)(*(int *)(in_ECX + 0x38) + 8);
    }
    sVar8 = (short)param_3;
    *(uint *)(in_ECX + 0x40) = (uint)param_1 * 0x10 + *(int *)(iVar5 + 0x1c);
    if (sVar8 == 1000) {
      uVar4 = FUN_0055d840();
      FUN_0055d2e0(uVar4);
      uVar4 = extraout_ECX_00;
    }
    else {
      uVar4 = FUN_0055d840(param_3);
      FUN_0055d300(uVar4,param_3);
      uVar4 = extraout_ECX;
    }
    puVar2 = *(undefined1 **)(in_ECX + 0x40);
    uVar6 = CONCAT31((int3)((uint)uVar4 >> 8),*puVar2) & 0xffffff07;
    uVar7 = (undefined2)(uVar6 >> 0x10);
    *(ushort *)(in_ECX + 0x78) = (ushort)(byte)puVar2[3] << ((char)uVar6 == '\x04');
    if (sVar8 == 0) {
      uVar4 = FUN_0055d750(puVar2,*(undefined4 *)(in_ECX + 0x44));
      FUN_0055d290(uVar4);
      uVar7 = extraout_var;
    }
    uVar1 = *(ushort *)(*(int *)(in_ECX + 0x44) + 2);
    if ((float)uVar1 <= *(float *)(in_ECX + 0x48)) {
      FUN_0055d290(CONCAT22(uVar7,uVar1) + -1);
    }
    iVar5 = *(int *)(in_ECX + 0x40);
    *(float *)(in_ECX + 0x50) = (float)*(byte *)(iVar5 + 2);
    if (1 < *(byte *)(iVar5 + 0xb)) {
      iVar3 = *(int *)(*(int *)(iVar5 + 0xc) + 4 + (uint)(*(byte *)(iVar5 + 0xb) >> 1) * 8);
      *(uint *)(in_ECX + 0x18) = (uint)*(ushort *)(*(int *)(*(int *)(iVar5 + 0xc) + 4) + 2);
      *(uint *)(in_ECX + 0x1c) = (uint)*(ushort *)(iVar3 + 2);
      *(uint *)(in_ECX + 0x20) = (uint)*(ushort *)(iVar3 + 4) - *(int *)(in_ECX + 0x18) / 2;
    }
  }
  return;
}
