#include "restore_types.h"
extern void FUN_004bd5e0();
extern uint FUN_00562348();

void FUN_004bed00(void)

{
  uint uVar1;
  
  uVar1 = FUN_00562348();
  uVar1 = uVar1 & 0x80000007;
  if ((int)uVar1 < 0) {
    uVar1 = (uVar1 - 1 | 0xfffffff8) + 1;
  }
  FUN_004bd5e0((-3 - uVar1) * 10);
  return;
}
