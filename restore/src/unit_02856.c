#include "restore_types.h"
#define LAB_00561a90 (*(undefined1 *)0x00561a90u)
#define DAT_00570638 (*(undefined4 *)0x00570638u)
#define DAT_01266414 (*(undefined4 *)0x01266414u)
extern void FUN_005686f4();

void FUN_005637ee(void)

{
  void *pvStack_14;
  undefined1 *puStack_10;
  undefined *puStack_c;
  undefined4 local_8;
  
  puStack_c = &DAT_00570638;
  puStack_10 = &LAB_00561a90;
  pvStack_14 = ExceptionList;
  ExceptionList = &pvStack_14;
  if (DAT_01266414 != (code *)0x0) {
    local_8 = 1;
    ExceptionList = &pvStack_14;
    ((code *)DAT_01266414)();
  }
  local_8 = 0xffffffff;
  FUN_005686f4();
  return;
}
