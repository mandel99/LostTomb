#include "restore_types.h"
#define DAT_00605a3c (*(undefined *)0x00605a3cu)
#define DAT_0078d680 (*(undefined *)0x0078d680u)
#define DAT_0078d6c8 (*(undefined *)0x0078d6c8u)
#define DAT_00ea40b0 (*(undefined4 *)0x00ea40b0u)
#define DAT_00ea40d4 (*(undefined4 *)0x00ea40d4u)
#define DAT_00ea6194 (*(undefined4 *)0x00ea6194u)
#define DAT_00ea61dc (*(undefined4 *)0x00ea61dcu)
#define DAT_00ea61e8 (*(undefined4 *)0x00ea61e8u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern undefined4 FUN_00443950();
extern undefined4 FUN_00443b90();
extern void FUN_004e2970();

void FUN_004e3aa0(int param_1)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  
  if (DAT_00605a3c == 1) {
    return;
  }
  if (param_1 != 0xe8) {
    return;
  }
  if ((0 < DAT_0078d6c8) && (0 < DAT_0078d680)) {
    return;
  }
  iVar2 = DAT_00eaac28 * 0x49d8;
  if (0 < *(int *)(&DAT_00ea61e8 + iVar2)) {
    return;
  }
  iVar1 = *(int *)(&DAT_00ea61dc + iVar2);
  if ((0 < iVar1) && (0 < *(int *)(&DAT_00ea6194 + iVar2))) {
    return;
  }
  if ((DAT_0078d6c8 < 1) && (iVar1 < 1)) {
    if ((DAT_0078d680 < 1) && (*(int *)(&DAT_00ea6194 + iVar2) < 1)) {
      FUN_004e2970(0xff,0,0);
      iVar2 = FUN_00443950(0x1f);
      if (iVar2 == 0) {
        uVar3 = 0x104;
      }
      else {
        if ((*(short *)(&DAT_00ea40d4 + DAT_00eaac28 * 0x49d8) == 1) ||
           (*(short *)(&DAT_00ea40d4 + DAT_00eaac28 * 0x49d8) == 3)) goto LAB_004e3b84;
        uVar3 = 0x103;
      }
      FUN_004e2970(uVar3,0,0);
LAB_004e3b84: ;
      iVar2 = FUN_00443b90(0xd);
      if (iVar2 != 0) {
        FUN_004e2970(0x100,0,0);
        return;
      }
      iVar2 = FUN_00443950(0xd);
      if (iVar2 == 0) {
        FUN_004e2970(0x102,0,0);
        return;
      }
      if (*(short *)(&DAT_00ea40b0 + DAT_00eaac28 * 0x49d8) == 1) {
        return;
      }
      if (*(short *)(&DAT_00ea40b0 + DAT_00eaac28 * 0x49d8) == 3) {
        return;
      }
      FUN_004e2970(0x101,0,0);
      return;
    }
    if (iVar1 < 1) {
      FUN_004e2970(0x109,0,0);
      iVar2 = FUN_00443950(0x1f);
      if (iVar2 != 0) {
        if (*(short *)(&DAT_00ea40d4 + DAT_00eaac28 * 0x49d8) == 1) {
          return;
        }
        if (*(short *)(&DAT_00ea40d4 + DAT_00eaac28 * 0x49d8) == 3) {
          return;
        }
        FUN_004e2970(0x20,0,0);
        return;
      }
      goto LAB_004e3cd8;
    }
  }
  if (0 < DAT_0078d680) {
    return;
  }
  if (0 < *(int *)(&DAT_00ea6194 + iVar2)) {
    return;
  }
  FUN_004e2970(0x10a,0,0);
  iVar2 = FUN_00443b90(0xd);
  if (iVar2 != 0) {
    FUN_004e2970(0x100,0,0);
    return;
  }
  iVar2 = FUN_00443950(0xd);
  if (iVar2 != 0) {
    if (*(short *)(&DAT_00ea40b0 + DAT_00eaac28 * 0x49d8) == 1) {
      return;
    }
    if (*(short *)(&DAT_00ea40b0 + DAT_00eaac28 * 0x49d8) == 3) {
      return;
    }
    FUN_004e2970(0x20,0,0);
    return;
  }
LAB_004e3cd8: ;
  FUN_004e2970(0x1f,0,0);
  return;
}
