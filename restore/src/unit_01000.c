#include "restore_types.h"
#define DAT_00ea5b44 (*(undefined4 *)0x00ea5b44u)
#define DAT_00ea5cac (*(undefined4 *)0x00ea5cacu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern float10 FUN_004f78a0();
extern longlong FUN_0056133c();

void FUN_0046ef50(void)

{
  int *piVar1;
  int iVar2;
  
  FUN_004f78a0(0);
  iVar2 = FUN_0056133c();
  piVar1 = &DAT_00ea5b44 + DAT_00eaac28 * 0x1276;
  *piVar1 = iVar2;
  if (iVar2 < 0) {
    *piVar1 = 0;
  }
  if (100 < *piVar1) {
    *piVar1 = 100;
  }
  (&DAT_00ea5cac)[DAT_00eaac28 * 0x1276] = 0;
  return;
}
