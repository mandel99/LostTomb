#include "restore_types.h"
extern undefined4 FUN_00562517();
extern undefined4 FUN_00562c25();
extern int FUN_00562c8a();

int FUN_00562bea(int param_1)

{
  int iVar1;
  
  if (param_1 == 0) {
    iVar1 = FUN_00562c8a(0);
    return iVar1;
  }
  iVar1 = FUN_00562c25(param_1);
  if (iVar1 != 0) {
    return -1;
  }
  if ((*(byte *)(param_1 + 0xd) & 0x40) != 0) {
    iVar1 = FUN_00562517(*(undefined4 *)(param_1 + 0x10));
    return -(uint)(iVar1 != 0);
  }
  return 0;
}
