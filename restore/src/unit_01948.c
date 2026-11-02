#include "restore_types.h"
#define DAT_00605a3c (*(undefined *)0x00605a3cu)
#define DAT_0078d6a4 (*(undefined *)0x0078d6a4u)
#define DAT_00ea40c2 (*(undefined4 *)0x00ea40c2u)
#define DAT_00ea61b8 (*(undefined4 *)0x00ea61b8u)
#define DAT_00ea61bc (*(undefined4 *)0x00ea61bcu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern undefined4 FUN_00443950();
extern undefined4 FUN_00443b90();
extern void FUN_004e2970();

void FUN_004e36a0(int param_1)

{
  int iVar1;
  
  if ((((DAT_00605a3c != 1) && (param_1 == 0xcb)) && (DAT_0078d6a4 < 1)) &&
     (((int)(&DAT_00ea61bc)[DAT_00eaac28 * 0x1276] < 1 &&
      (*(int *)(&DAT_00ea61b8 + DAT_00eaac28 * 0x49d8) < 1)))) {
    FUN_004e2970(0x50,0,0);
    iVar1 = FUN_00443b90(0x16);
    if (iVar1 != 0) {
      FUN_004e2970(0x51,0,0);
      return;
    }
    iVar1 = FUN_00443950(0x16);
    if (iVar1 == 0) {
      FUN_004e2970(0x1f,0,0);
    }
    else if ((*(short *)(&DAT_00ea40c2 + DAT_00eaac28 * 0x49d8) != 1) &&
            (*(short *)(&DAT_00ea40c2 + DAT_00eaac28 * 0x49d8) != 3)) {
      FUN_004e2970(0x20,0,0);
      return;
    }
  }
  return;
}
