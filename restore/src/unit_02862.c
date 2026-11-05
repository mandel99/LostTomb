#include "restore_types.h"
#include "crt_exception_runtime.h"
#define LAB_00561a90 (*(undefined1 *)0x00561a90u)
#define DAT_00570668 (*(undefined4 *)0x00570668u)
extern void FUN_00563844();

void FUN_00563be1(int param_1,undefined4 param_2,int param_3,int param_4)

{
  int iVar1;
  int iVar2;
  void *local_14;
  undefined1 *puStack_10;
  undefined *puStack_c;
  undefined4 local_8;
  
  puStack_c = &DAT_00570668;
  puStack_10 = &LAB_00561a90;
  local_14 = ExceptionList;
  ExceptionList = &local_14;
  for (iVar2 = *(int *)(param_1 + 8); local_8 = 0xffffffff, iVar2 != param_4;
      iVar2 = *(int *)(*(int *)(param_3 + 8) + iVar2 * 8)) {
    if ((iVar2 < 0) || (*(int *)(param_3 + 4) <= iVar2)) {
      FUN_00563844();
    }
    local_8 = 0;
    iVar1 = *(int *)(*(int *)(param_3 + 8) + 4 + iVar2 * 8);
    if (iVar1 != 0) {
      (void)restore_crt_invoke_frame((uint32_t)iVar1,(uint32_t)param_1,0x103u);
    }
  }
  *(int *)(param_1 + 8) = iVar2;
  ExceptionList = local_14;
  return;
}
