#include "restore_types.h"
#define DAT_00605a3c (*(undefined *)0x00605a3cu)
#define DAT_0078d678 (*(undefined *)0x0078d678u)
#define DAT_00ea40ac (*(undefined4 *)0x00ea40acu)
#define DAT_00ea618c (*(undefined4 *)0x00ea618cu)
#define DAT_00ea6194 (*(undefined4 *)0x00ea6194u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern undefined4 FUN_00443950();
extern undefined4 FUN_00443b90();
extern void FUN_004e2970();

void FUN_004e35e0(int param_1)

{
  int iVar1;
  
  if ((((DAT_00605a3c != 1) && (param_1 == 0x72)) && (DAT_0078d678 < 1)) &&
     ((*(int *)(&DAT_00ea6194 + DAT_00eaac28 * 0x49d8) < 1 &&
      (*(int *)(&DAT_00ea618c + DAT_00eaac28 * 0x49d8) < 1)))) {
    FUN_004e2970(0x1d,0,0);
    iVar1 = FUN_00443b90(0xb);
    if (iVar1 != 0) {
      FUN_004e2970(0x24,0,0);
      return;
    }
    iVar1 = FUN_00443950(0xb);
    if (iVar1 == 0) {
      FUN_004e2970(0x1f,0,0);
    }
    else if ((*(short *)(&DAT_00ea40ac + DAT_00eaac28 * 0x49d8) != 1) &&
            (*(short *)(&DAT_00ea40ac + DAT_00eaac28 * 0x49d8) != 3)) {
      FUN_004e2970(0x20,0,0);
      return;
    }
  }
  return;
}
