#include "restore_types.h"
extern undefined4 FUN_0055d080();

int FUN_0055d000(int param_1,int param_2)

{
  int iVar1;
  uint uVar2;
  
  uVar2 = (uint)*(byte *)((*(uint *)(param_1 + 0x14) & 0xff) + 0x2a34 + param_1);
  iVar1 = FUN_0055d080(param_1,*(undefined1 *)(param_1 + 0x30b4 + uVar2));
  if (iVar1 != 0) {
    return 0;
  }
  if (param_2 == 2) {
    uVar2 = uVar2 << 2 | *(uint *)(param_1 + 0x14) & 3;
    iVar1 = FUN_0055d080(param_1,2);
    if (iVar1 != 0) {
      return 0;
    }
  }
  else {
    uVar2 = uVar2 << ((byte)*(undefined4 *)(param_1 + 0xc) & 0x1f) |
            *(uint *)(param_1 + 0x10) & *(uint *)(param_1 + 0x14);
    iVar1 = FUN_0055d080(param_1,*(undefined4 *)(param_1 + 0xc));
    if (iVar1 != 0) {
      return 0;
    }
  }
  return uVar2 + 1;
}
