#include "restore_types.h"
#define DAT_00605a3c (*(undefined *)0x00605a3cu)
#define DAT_0078d678 (*(undefined *)0x0078d678u)
#define DAT_0078d6d0 (*(undefined *)0x0078d6d0u)
#define DAT_0078d760 (*(undefined *)0x0078d760u)
#define DAT_00ea40ac (*(undefined4 *)0x00ea40acu)
#define DAT_00ea40d6 (*(undefined4 *)0x00ea40d6u)
#define DAT_00ea40d8 (*(undefined4 *)0x00ea40d8u)
#define DAT_00ea618c (*(undefined4 *)0x00ea618cu)
#define DAT_00ea61e4 (*(undefined4 *)0x00ea61e4u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern undefined4 FUN_00443900();
extern undefined4 FUN_00443950();
extern undefined4 FUN_004439c0();
extern undefined4 FUN_00443b90();
extern void FUN_004e2970();

void FUN_004e4550(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  if (DAT_00605a3c == 1) {
    return;
  }
  if (param_1 != 0xe7) {
    return;
  }
  if ((0 < DAT_0078d678) || (0 < *(int *)(&DAT_00ea618c + DAT_00eaac28 * 0x49d8))) {
    if (0 < DAT_0078d6d0) {
      return;
    }
    if (0 < *(int *)(&DAT_00ea61e4 + DAT_00eaac28 * 0x49d8)) {
      return;
    }
  }
  FUN_004e2970(0x10e,0,0);
  if ((DAT_0078d678 == 0) && (*(int *)(&DAT_00ea618c + DAT_00eaac28 * 0x49d8) == 0)) {
    iVar1 = FUN_00443b90(0xb);
    if (iVar1 == 0) {
      iVar1 = FUN_00443950(0xb);
      if (iVar1 == 0) {
        uVar2 = 0x92;
      }
      else {
        if ((*(short *)(&DAT_00ea40ac + DAT_00eaac28 * 0x49d8) == 1) ||
           (*(short *)(&DAT_00ea40ac + DAT_00eaac28 * 0x49d8) == 3)) goto LAB_004e463b;
        uVar2 = 0x93;
      }
    }
    else {
      uVar2 = 0x24;
    }
    FUN_004e2970(uVar2,0,0);
  }
LAB_004e463b: ;
  if (DAT_0078d6d0 != 0) {
    return;
  }
  if (*(int *)(&DAT_00ea61e4 + DAT_00eaac28 * 0x49d8) != 0) {
    return;
  }
  iVar1 = FUN_00443900(0x20);
  if (((iVar1 == 0) && (iVar1 = FUN_004439c0(0x20), iVar1 == 0)) || (DAT_0078d760 != 0)) {
    iVar1 = FUN_004439c0(0x20);
    if ((iVar1 == 0) || (DAT_0078d760 == 0)) {
      iVar1 = FUN_004439c0(0x21);
      if (iVar1 != 0) {
        iVar1 = FUN_00443950(0x21);
        if (iVar1 == 0) {
          FUN_004e2970(0x1f,0,0);
        }
        else if ((*(short *)(&DAT_00ea40d8 + DAT_00eaac28 * 0x49d8) != 1) &&
                (*(short *)(&DAT_00ea40d8 + DAT_00eaac28 * 0x49d8) != 3)) {
          FUN_004e2970(0x20,0,0);
          return;
        }
      }
    }
    else {
      iVar1 = FUN_00443950(0x20);
      if (iVar1 == 0) {
        FUN_004e2970(0x108,0,0);
        return;
      }
      if ((*(short *)(&DAT_00ea40d6 + DAT_00eaac28 * 0x49d8) != 1) &&
         (*(short *)(&DAT_00ea40d6 + DAT_00eaac28 * 0x49d8) != 3)) {
        FUN_004e2970(0x107,0,0);
        return;
      }
    }
    return;
  }
  iVar1 = FUN_004439c0(0x21);
  if (iVar1 == 0) {
    FUN_004e2970(0x10d,0,0);
    return;
  }
  iVar1 = FUN_00443950(0x21);
  if (iVar1 == 0) {
    FUN_004e2970(0x10b,0,0);
    return;
  }
  if (*(short *)(&DAT_00ea40d8 + DAT_00eaac28 * 0x49d8) == 1) {
    return;
  }
  if (*(short *)(&DAT_00ea40d8 + DAT_00eaac28 * 0x49d8) == 3) {
    return;
  }
  FUN_004e2970(0x10c,0,0);
  return;
}
