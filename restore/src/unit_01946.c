#include "restore_types.h"
#define DAT_00605a3c (*(undefined *)0x00605a3cu)
#define DAT_0078d694 (*(undefined *)0x0078d694u)
#define DAT_00ea40ba (*(undefined4 *)0x00ea40bau)
#define DAT_00ea61a8 (*(undefined4 *)0x00ea61a8u)
#define DAT_00ea61ac (*(undefined4 *)0x00ea61acu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern undefined4 FUN_00443950();
extern undefined4 FUN_00443b90();
extern void FUN_004e2970();

void FUN_004e3520(int param_1)

{
  int iVar1;
  
  if ((((DAT_00605a3c != 1) && (param_1 == 0x71)) && (DAT_0078d694 < 1)) &&
     ((*(int *)(&DAT_00ea61ac + DAT_00eaac28 * 0x49d8) < 1 &&
      (*(int *)(&DAT_00ea61a8 + DAT_00eaac28 * 0x49d8) < 1)))) {
    FUN_004e2970(0x1e,0,0);
    iVar1 = FUN_00443b90(0x12);
    if (iVar1 != 0) {
      FUN_004e2970(0x25,0,0);
      return;
    }
    iVar1 = FUN_00443950(0x12);
    if (iVar1 == 0) {
      FUN_004e2970(0x1f,0,0);
    }
    else if ((*(short *)(&DAT_00ea40ba + DAT_00eaac28 * 0x1276) != 1) &&
            (*(short *)(&DAT_00ea40ba + DAT_00eaac28 * 0x1276) != 3)) {
      FUN_004e2970(0x20,0,0);
      return;
    }
  }
  return;
}
