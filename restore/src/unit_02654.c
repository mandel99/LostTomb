#include "restore_types.h"
extern undefined4 FUN_0055d080();

uint FUN_0055ce50(int param_1)

{
  byte bVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  
  if ((*(byte *)(param_1 + 0x14) & 1) == 0) {
    iVar2 = FUN_0055d080(param_1,1);
    if (iVar2 != 0) {
      return 0x306;
    }
    if (*(int *)(param_1 + 4) == 0) {
      uVar4 = *(uint *)(param_1 + 0x14) & 0xff;
      iVar2 = FUN_0055d080(param_1,8);
      if (iVar2 != 0) {
        return 0x306;
      }
    }
    else {
      uVar4 = *(uint *)(param_1 + 0x14) & 0xff;
      if (uVar4 == 0) {
        iVar2 = FUN_0055d080(param_1,8);
        if (iVar2 != 0) {
          return 0x306;
        }
        uVar4 = (uint)*(byte *)((*(uint *)(param_1 + 0x14) & 0xff) + 0x2eb4 + param_1);
      }
      else {
        uVar4 = (uint)*(byte *)(param_1 + 0x2c34 + uVar4);
        if (uVar4 == 0xff) {
          if ((*(uint *)(param_1 + 0x14) & 0x3f) == 0) {
            iVar2 = FUN_0055d080(param_1,6);
            if (iVar2 != 0) {
              return 0x306;
            }
            uVar4 = (uint)*(byte *)((*(uint *)(param_1 + 0x14) & 0x7f) + 0x2e34 + param_1);
          }
          else {
            iVar2 = FUN_0055d080(param_1,4);
            if (iVar2 != 0) {
              return 0x306;
            }
            uVar4 = (uint)*(byte *)((*(uint *)(param_1 + 0x14) & 0xff) + 0x2d34 + param_1);
          }
        }
      }
      iVar2 = FUN_0055d080(param_1,*(undefined1 *)(param_1 + 0x2fb4 + uVar4));
      if (iVar2 != 0) {
        return 0x306;
      }
    }
  }
  else {
    iVar2 = FUN_0055d080(param_1,1);
    if (iVar2 != 0) {
      return 0x306;
    }
    uVar4 = (uint)*(byte *)((*(uint *)(param_1 + 0x14) & 0xff) + 0x2b34 + param_1);
    iVar2 = FUN_0055d080(param_1,*(undefined1 *)(param_1 + 0x30f4 + uVar4));
    if (iVar2 != 0) {
      return 0x306;
    }
    bVar1 = *(byte *)(param_1 + uVar4 + 0x3104);
    if (bVar1 != 0) {
      uVar3 = (1 << (bVar1 & 0x1f)) - 1U & *(uint *)(param_1 + 0x14);
      iVar2 = FUN_0055d080(param_1,bVar1);
      if ((iVar2 != 0) && (uVar4 + uVar3 != 0x10e)) {
        return 0x306;
      }
      uVar4 = *(ushort *)(param_1 + 0x3114 + uVar4 * 2) + uVar3;
    }
    uVar4 = uVar4 + 0x100;
  }
  return uVar4;
}
