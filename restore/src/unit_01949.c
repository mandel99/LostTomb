#include "restore_types.h"
#define DAT_00605a3c (*(undefined *)0x00605a3cu)
#define DAT_0078d650 (*(undefined *)0x0078d650u)
#define DAT_0078d678 (*(undefined *)0x0078d678u)
#define DAT_00ea40a8 (*(undefined4 *)0x00ea40a8u)
#define DAT_00ea40ac (*(undefined4 *)0x00ea40acu)
#define DAT_00ea6184 (*(undefined4 *)0x00ea6184u)
#define DAT_00ea618c (*(undefined4 *)0x00ea618cu)
#define DAT_00ea6190 (*(undefined4 *)0x00ea6190u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern undefined4 FUN_00443950();
extern undefined4 FUN_00443b90();
extern void FUN_004e2970();

void FUN_004e3760(int param_1)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  
  if (DAT_00605a3c == 1) {
    return;
  }
  if (param_1 != 0xcc) {
    return;
  }
  if ((0 < DAT_0078d678) && (0 < DAT_0078d650)) {
    return;
  }
  iVar2 = DAT_00eaac28 * 0x49d8;
  if (0 < *(int *)(&DAT_00ea6190 + iVar2)) {
    return;
  }
  iVar1 = *(int *)(&DAT_00ea618c + iVar2);
  if ((0 < iVar1) && (0 < *(int *)(&DAT_00ea6184 + iVar2))) {
    return;
  }
  if ((DAT_0078d678 < 1) && (iVar1 < 1)) {
    if ((DAT_0078d650 < 1) && (*(int *)(&DAT_00ea6184 + iVar2) < 1)) {
      FUN_004e2970(0x91,0,0);
      iVar2 = FUN_00443b90(0xb);
      if (iVar2 == 0) {
        iVar2 = FUN_00443950(0xb);
        if (iVar2 == 0) {
          uVar3 = 0x92;
        }
        else {
          if ((*(short *)(&DAT_00ea40ac + DAT_00eaac28 * 0x49d8) == 1) ||
             (*(short *)(&DAT_00ea40ac + DAT_00eaac28 * 0x49d8) == 3)) goto LAB_004e385a;
          uVar3 = 0x93;
        }
      }
      else {
        uVar3 = 0x24;
      }
      FUN_004e2970(uVar3,0,0);
LAB_004e385a: ;
      iVar2 = FUN_00443b90(1);
      if (iVar2 != 0) {
        FUN_004e2970(0x53,0,0);
        return;
      }
      iVar2 = FUN_00443950(9);
      if (iVar2 == 0) {
        FUN_004e2970(0x94,0,0);
        return;
      }
      if (*(short *)(&DAT_00ea40a8 + DAT_00eaac28 * 0x49d8) == 1) {
        return;
      }
      if (*(short *)(&DAT_00ea40a8 + DAT_00eaac28 * 0x49d8) == 3) {
        return;
      }
      FUN_004e2970(0x95,0,0);
      return;
    }
    if (iVar1 < 1) {
      FUN_004e2970(0x1d,0,0);
      iVar2 = FUN_00443b90(0xb);
      if (iVar2 != 0) {
        FUN_004e2970(0x24,0,0);
        return;
      }
      iVar2 = FUN_00443950(0xb);
      if (iVar2 != 0) {
        if (*(short *)(&DAT_00ea40ac + DAT_00eaac28 * 0x49d8) == 1) {
          return;
        }
        if (*(short *)(&DAT_00ea40ac + DAT_00eaac28 * 0x49d8) == 3) {
          return;
        }
        FUN_004e2970(0x20,0,0);
        return;
      }
      goto LAB_004e39bc;
    }
  }
  if (0 < DAT_0078d650) {
    return;
  }
  if (0 < *(int *)(&DAT_00ea6184 + iVar2)) {
    return;
  }
  FUN_004e2970(0x52,0,0);
  iVar2 = FUN_00443b90(1);
  if (iVar2 != 0) {
    FUN_004e2970(0x53,0,0);
    return;
  }
  iVar2 = FUN_00443950(9);
  if (iVar2 != 0) {
    if (*(short *)(&DAT_00ea40a8 + DAT_00eaac28 * 0x49d8) == 1) {
      return;
    }
    if (*(short *)(&DAT_00ea40a8 + DAT_00eaac28 * 0x49d8) == 3) {
      return;
    }
    FUN_004e2970(0x20,0,0);
    return;
  }
LAB_004e39bc: ;
  FUN_004e2970(0x1f,0,0);
  return;
}
