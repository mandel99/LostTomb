#include "restore_types.h"
#define DAT_00605a3c (*(undefined *)0x00605a3cu)
#define DAT_0078d650 (*(undefined *)0x0078d650u)
#define DAT_00ea40a8 (*(undefined4 *)0x00ea40a8u)
#define DAT_00ea6184 (*(undefined4 *)0x00ea6184u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern undefined4 FUN_00443950();
extern undefined4 FUN_00443b90();
extern void FUN_004e2970();

void FUN_004e3cf0(int param_1)

{
  int iVar1;
  
  if ((((DAT_00605a3c != 1) && (param_1 == 0xc2)) && (DAT_0078d650 < 1)) &&
     (*(int *)(&DAT_00ea6184 + DAT_00eaac28 * 0x49d8) < 1)) {
    FUN_004e2970(0x52,0,0);
    iVar1 = FUN_00443b90(1);
    if (iVar1 != 0) {
      FUN_004e2970(0x53,0,0);
      return;
    }
    iVar1 = FUN_00443950(9);
    if (iVar1 == 0) {
      FUN_004e2970(0x1f,0,0);
    }
    else if ((*(short *)(&DAT_00ea40a8 + DAT_00eaac28 * 0x49d8) != 1) &&
            (*(short *)(&DAT_00ea40a8 + DAT_00eaac28 * 0x49d8) != 3)) {
      FUN_004e2970(0x20,0,0);
      return;
    }
  }
  return;
}
