#include "restore_types.h"

undefined4 FUN_0055d080(int param_1,uint param_2)

{
  int *piVar1;
  byte bVar2;
  int iVar3;
  uint uVar4;
  
  uVar4 = *(uint *)(param_1 + 0x18);
  if (param_2 <= uVar4) {
    *(uint *)(param_1 + 0x18) = uVar4 - param_2;
    *(uint *)(param_1 + 0x14) = *(uint *)(param_1 + 0x14) >> ((byte)param_2 & 0x1f);
    return 0;
  }
  piVar1 = (int *)(param_1 + 0x1c);
  *(uint *)(param_1 + 0x14) = *(uint *)(param_1 + 0x14) >> ((byte)uVar4 & 0x1f);
  if (*piVar1 == *(int *)(param_1 + 0x20)) {
    *piVar1 = 0x800;
    iVar3 = (**(code **)(param_1 + 0x28))(param_1 + 0x2234,piVar1,*(undefined4 *)(param_1 + 0x24));
    *(int *)(param_1 + 0x20) = iVar3;
    if (iVar3 == 0) {
      return 1;
    }
    *piVar1 = 0;
  }
  bVar2 = *(byte *)(param_1 + 0x2234 + *piVar1);
  *piVar1 = *piVar1 + 1;
  uVar4 = (uint)bVar2 << 8 | *(uint *)(param_1 + 0x14);
  *(uint *)(param_1 + 0x14) = uVar4;
  *(uint *)(param_1 + 0x14) = uVar4 >> ((byte)param_2 - (char)*(int *)(param_1 + 0x18) & 0x1f);
  *(uint *)(param_1 + 0x18) = (*(int *)(param_1 + 0x18) - param_2) + 8;
  return 0;
}
