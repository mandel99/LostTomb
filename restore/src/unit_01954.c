#include "restore_types.h"
#define DAT_00605a3c (*(undefined *)0x00605a3cu)
#define DAT_0078d650 (*(undefined *)0x0078d650u)
#define DAT_0078d6fc (*(undefined *)0x0078d6fcu)
#define DAT_00ea409a (*(undefined4 *)0x00ea409au)
#define DAT_00ea40a6 (*(undefined4 *)0x00ea40a6u)
#define DAT_00ea40a8 (*(undefined4 *)0x00ea40a8u)
#define DAT_00ea6180 (*(undefined4 *)0x00ea6180u)
#define DAT_00ea6184 (*(undefined4 *)0x00ea6184u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern undefined4 FUN_00443900();
extern undefined4 FUN_00443950();
extern undefined4 FUN_004439c0();
extern undefined4 FUN_00443b90();
extern void FUN_004e2970();

void FUN_004e3f60(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  if (DAT_00605a3c == 1) {
    return;
  }
  if (param_1 != 0xe2) {
    return;
  }
  if (0 < *(int *)(&DAT_00ea6180 + DAT_00eaac28 * 0x49d8)) {
    return;
  }
  FUN_004e2970(0xed,0,0);
  if (0 < DAT_0078d6fc) {
    return;
  }
  iVar1 = FUN_00443900(8);
  if (((iVar1 == 0) && (iVar1 = FUN_004439c0(8), iVar1 == 0)) || (DAT_0078d6fc != 0)) {
    iVar1 = FUN_004439c0(8);
    if (iVar1 == 0) goto LAB_004e4082;
    iVar1 = FUN_00443950(8);
    if (iVar1 == 0) {
      uVar2 = 0x1f;
    }
    else {
      if ((*(short *)(&DAT_00ea40a6 + DAT_00eaac28 * 0x49d8) == 1) ||
         (*(short *)(&DAT_00ea40a6 + DAT_00eaac28 * 0x49d8) == 3)) goto LAB_004e4082;
      uVar2 = 0x20;
    }
  }
  else {
    iVar1 = FUN_004439c0(8);
    if (iVar1 == 0) {
      uVar2 = 0xee;
    }
    else {
      iVar1 = FUN_00443950(8);
      if (iVar1 == 0) {
        uVar2 = 0xf0;
      }
      else {
        if ((*(short *)(&DAT_00ea409a + DAT_00eaac28 * 0x49d8) == 1) ||
           (*(short *)(&DAT_00ea409a + DAT_00eaac28 * 0x49d8) == 3)) goto LAB_004e4082;
        uVar2 = 0xef;
      }
    }
  }
  FUN_004e2970(uVar2,0,0);
LAB_004e4082: ;
  if ((DAT_0078d650 < 1) && (*(int *)(&DAT_00ea6184 + DAT_00eaac28 * 0x49d8) < 1)) {
    FUN_004e2970(0x52,0,0);
    iVar1 = FUN_00443b90(1);
    if (iVar1 != 0) {
      FUN_004e2970(0x53,0,0);
      return;
    }
    iVar1 = FUN_00443950(9);
    if (iVar1 == 0) {
      FUN_004e2970(0x94,0,0);
    }
    else if ((*(short *)(&DAT_00ea40a8 + DAT_00eaac28 * 0x49d8) != 1) &&
            (*(short *)(&DAT_00ea40a8 + DAT_00eaac28 * 0x49d8) != 3)) {
      FUN_004e2970(0x95,0,0);
      return;
    }
  }
  return;
}
