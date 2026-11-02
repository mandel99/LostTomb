#include "restore_types.h"
extern uint FUN_0055ce50();
extern int FUN_0055d000();
extern void FUN_0055d250();

uint FUN_0055cd50(int param_1)

{
  undefined1 uVar1;
  int iVar2;
  undefined1 *puVar3;
  undefined1 *puVar4;
  uint local_8;
  int local_4;
  
  *(undefined4 *)(param_1 + 8) = 0x1000;
  do {
    local_8 = FUN_0055ce50(param_1);
    if (0x304 < local_8) {
LAB_0055ce1c: ;
      local_4 = *(int *)(param_1 + 8) + -0x1000;
      (**(code **)(param_1 + 0x2c))(param_1 + 0x1030,&local_4,*(undefined4 *)(param_1 + 0x24));
      return local_8;
    }
    if (local_8 < 0x100) {
      *(char *)(*(int *)(param_1 + 8) + 0x30 + param_1) = (char)local_8;
      *(int *)(param_1 + 8) = *(int *)(param_1 + 8) + 1;
    }
    else {
      local_8 = local_8 - 0xfe;
      iVar2 = FUN_0055d000(param_1,local_8);
      if (iVar2 == 0) {
        local_8 = 0x306;
        goto LAB_0055ce1c;
      }
      puVar4 = (undefined1 *)(*(int *)(param_1 + 8) + 0x30 + param_1);
      puVar3 = puVar4 + -iVar2;
      *(uint *)(param_1 + 8) = local_8 + *(int *)(param_1 + 8);
      do {
        uVar1 = *puVar3;
        puVar3 = puVar3 + 1;
        *puVar4 = uVar1;
        puVar4 = puVar4 + 1;
        local_8 = local_8 - 1;
      } while (local_8 != 0);
    }
    if (0x1fff < *(uint *)(param_1 + 8)) {
      local_8 = 0x1000;
      (**(code **)(param_1 + 0x2c))(param_1 + 0x1030,&local_8,*(undefined4 *)(param_1 + 0x24));
      FUN_0055d250(param_1 + 0x30,param_1 + 0x1030,*(int *)(param_1 + 8) + -0x1000);
      *(int *)(param_1 + 8) = *(int *)(param_1 + 8) + -0x1000;
    }
  } while( true );
}
