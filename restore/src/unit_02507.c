#include "restore_types.h"
#define DAT_00ea5ba8 (*(undefined4 *)0x00ea5ba8u)
#define DAT_00eaac28 (*(uint32_t *)0x00eaac28u)
extern void FUN_0053d850();

void FUN_0053d820(void)

{
  short sVar1;
  
  sVar1 = *(short *)((int)&DAT_00ea5ba8 + DAT_00eaac28 * 0x49d8 + 2);
  if ((sVar1 != 0) && (sVar1 < 0x10)) {
    *(short *)((int)&DAT_00ea5ba8 + DAT_00eaac28 * 0x49d8 + 2) = sVar1 + 1;
    FUN_0053d850();
    return;
  }
  return;
}
