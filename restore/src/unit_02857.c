#include "restore_types.h"
#define LAB_00561a90 (*(undefined1 *)0x00561a90u)
#define DAT_00570650 (*(undefined4 *)0x00570650u)
#define PTR_FUN_00604c24 (*(unsigned char * *)0x00604c24u)
extern void FUN_005637ee();

void FUN_00563844(void)

{
  void *pvStack_14;
  undefined1 *puStack_10;
  undefined *puStack_c;
  undefined4 local_8;
  
  puStack_c = &DAT_00570650;
  puStack_10 = &LAB_00561a90;
  pvStack_14 = ExceptionList;
  ExceptionList = &pvStack_14;
  if (PTR_FUN_00604c24 != (undefined *)0x0) {
    local_8 = 1;
    ExceptionList = &pvStack_14;
    (*(code *)PTR_FUN_00604c24)();
  }
  local_8 = 0xffffffff;
  FUN_005637ee();
  return;
}
