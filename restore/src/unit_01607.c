#include "restore_types.h"
#define DAT_00ea5ce8 (*(undefined4 *)0x00ea5ce8u)
#define DAT_00ea5f9c (*(undefined4 *)0x00ea5f9cu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern undefined4 FUN_004c5120();
extern void FUN_004e96c0();

void FUN_004c5300(void)

{
  int iVar1;
  
  if ((&DAT_00ea5ce8)[DAT_00eaac28 * 0x24ec] != 0) {
    iVar1 = FUN_004c5120((&DAT_00ea5f9c)[DAT_00eaac28 * 0x1276]);
    if (iVar1 == 1) {
      FUN_004e96c0(1);
    }
  }
  return;
}
