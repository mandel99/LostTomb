#include "restore_types.h"
extern uint FUN_00560b2a();
extern size_t FUN_00563380();

void FUN_00560d96(char *param_1,char *param_2)

{
  uint *puVar1;
  uint uVar2;
  bool bVar3;
  size_t sVar4;
  size_t sVar5;
  int iVar6;
  size_t sVar7;
  int *in_ECX;
  int iVar8;
  int local_8;
  
  sVar4 = FUN_00563380(param_1);
  sVar5 = FUN_00563380(param_2);
  uVar2 = *(uint *)(*(int *)(*in_ECX + 4) + 0x30 + (int)in_ECX);
  if (sVar5 + sVar4 < uVar2) {
    local_8 = (uVar2 - sVar5) - sVar4;
  }
  else {
    local_8 = 0;
  }
  if (((*(byte *)((int)in_ECX + *(int *)(*in_ECX + 4) + 0x24) & 10) == 0) &&
     (iVar6 = local_8 + -1, bVar3 = 0 < local_8, iVar8 = local_8, local_8 = iVar6, bVar3)) {
    do {
      iVar6 = FUN_00560b2a(*(undefined1 *)((int)in_ECX + *(int *)(*in_ECX + 4) + 0x2c));
      if (iVar6 == -1) {
        puVar1 = (uint *)(*(int *)(*in_ECX + 4) + 8 + (int)in_ECX);
        *puVar1 = *puVar1 | 6;
      }
      iVar8 = iVar8 + -1;
      local_8 = local_8 + -1;
    } while (iVar8 != 0);
  }
  if ((sVar4 != 0) &&
     (sVar7 = (**(code **)(**(int **)(*(int *)(*in_ECX + 4) + 4 + (int)in_ECX) + 0x14))
                        (param_1,sVar4), sVar7 != sVar4)) {
    puVar1 = (uint *)(*(int *)(*in_ECX + 4) + 8 + (int)in_ECX);
    *puVar1 = *puVar1 | 6;
  }
  if (((*(byte *)(*(int *)(*in_ECX + 4) + 0x24 + (int)in_ECX) & 8) != 0) &&
     (iVar8 = local_8 + -1, bVar3 = 0 < local_8, iVar6 = local_8, local_8 = iVar8, bVar3)) {
    do {
      iVar8 = FUN_00560b2a(*(undefined1 *)((int)in_ECX + *(int *)(*in_ECX + 4) + 0x2c));
      if (iVar8 == -1) {
        puVar1 = (uint *)(*(int *)(*in_ECX + 4) + 8 + (int)in_ECX);
        *puVar1 = *puVar1 | 6;
      }
      iVar6 = iVar6 + -1;
      local_8 = local_8 + -1;
    } while (iVar6 != 0);
  }
  sVar4 = (**(code **)(**(int **)(*(int *)(*in_ECX + 4) + 4 + (int)in_ECX) + 0x14))(param_2,sVar5);
  if (sVar4 != sVar5) {
    puVar1 = (uint *)(*(int *)(*in_ECX + 4) + 8 + (int)in_ECX);
    *puVar1 = *puVar1 | 6;
  }
  if (((*(byte *)(*(int *)(*in_ECX + 4) + 0x24 + (int)in_ECX) & 2) != 0) && (0 < local_8)) {
    do {
      iVar6 = FUN_00560b2a(*(undefined1 *)((int)in_ECX + *(int *)(*in_ECX + 4) + 0x2c));
      if (iVar6 == -1) {
        puVar1 = (uint *)(*(int *)(*in_ECX + 4) + 8 + (int)in_ECX);
        *puVar1 = *puVar1 | 6;
      }
      local_8 = local_8 + -1;
    } while (local_8 != 0);
  }
  return;
}
