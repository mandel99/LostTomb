#include "restore_types.h"
extern void FUN_0055ca20();

void FUN_0055ca90(int param_1)

{
  undefined1 *puVar1;
  undefined1 uVar2;
  undefined1 uVar3;
  undefined4 local_4;
  
  local_4 = 0x800;
  puVar1 = (undefined1 *)(param_1 + 0x1fca);
  (**(code **)(param_1 + 0x9b8))(puVar1,&local_4,*(undefined4 *)(param_1 + 0x9b0));
  uVar2 = *(undefined1 *)(param_1 + 0x27ca);
  uVar3 = *(undefined1 *)(*(int *)(param_1 + 4) + 0x1fca + param_1);
  *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + -0x800;
  FUN_0055ca20(puVar1,0,0x802);
  if (*(int *)(param_1 + 4) != 0) {
    *puVar1 = uVar2;
  }
  if (*(int *)(param_1 + 8) != 0) {
    *(undefined1 *)(*(int *)(param_1 + 4) + 0x1fca + param_1) = uVar3;
  }
  return;
}
