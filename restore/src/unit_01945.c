#include "restore_types.h"
#define DAT_00605a3c (*(undefined *)0x00605a3cu)
#define DAT_0078d68c (*(undefined *)0x0078d68cu)
#define DAT_00ea40b6 (*(undefined4 *)0x00ea40b6u)
#define DAT_00ea61a0 (*(undefined4 *)0x00ea61a0u)
#define DAT_00ea61a4 (*(undefined4 *)0x00ea61a4u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern undefined4 FUN_00443950();
extern undefined4 FUN_00443b90();
extern void FUN_004e2970();

void FUN_004e3460(int param_1)

{
  int iVar1;
  
  if ((((DAT_00605a3c != 1) && (param_1 == 0x6f)) && (DAT_0078d68c < 1)) &&
     ((*(int *)(&DAT_00ea61a4 + DAT_00eaac28 * 0x49d8) < 1 &&
      (*(int *)(&DAT_00ea61a0 + DAT_00eaac28 * 0x49d8) < 1)))) {
    FUN_004e2970(0x1c,0,0);
    iVar1 = FUN_00443b90(0x10);
    if (iVar1 != 0) {
      FUN_004e2970(0x23,0,0);
      return;
    }
    iVar1 = FUN_00443950(0x10);
    if (iVar1 == 0) {
      FUN_004e2970(0x1f,0,0);
    }
    else if ((*(short *)(&DAT_00ea40b6 + DAT_00eaac28 * 0x49d8) != 1) &&
            (*(short *)(&DAT_00ea40b6 + DAT_00eaac28 * 0x49d8) != 3)) {
      FUN_004e2970(0x20,0,0);
      return;
    }
  }
  return;
}
