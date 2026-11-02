#include "restore_types.h"
#include "legacy_member_calls.h"
#define LAB_00561a90 (*(undefined1 *)0x00561a90u)
#define DAT_005706a0 (*(undefined4 *)0x005706a0u)

void FUN_0056400a(int param_1)

{
  int iVar1;
  void *local_14;
  undefined1 *puStack_10;
  undefined *puStack_c;
  undefined4 local_8;
  
  puStack_c = &DAT_005706a0;
  puStack_10 = &LAB_00561a90;
  local_14 = ExceptionList;
  if ((param_1 != 0) && (iVar1 = *(int *)(*(int *)(param_1 + 0x1c) + 4), iVar1 != 0)) {
    local_8 = 0;
    ExceptionList = &local_14;
    restore_call_member0((void *)*(undefined4 *)(param_1 + 0x18),iVar1);
  }
  ExceptionList = local_14;
  return;
}
