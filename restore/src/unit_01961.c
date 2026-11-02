#include "restore_types.h"
#define DAT_00605a3c (*(undefined *)0x00605a3cu)
#define DAT_0078d6b4 (*(undefined *)0x0078d6b4u)
#define DAT_0078d6c0 (*(undefined *)0x0078d6c0u)
#define DAT_00ea40ca (*(undefined4 *)0x00ea40cau)
#define DAT_00ea40d0 (*(undefined4 *)0x00ea40d0u)
#define DAT_00ea40dc (*(undefined4 *)0x00ea40dcu)
#define DAT_00ea61c8 (*(undefined4 *)0x00ea61c8u)
#define DAT_00ea61d4 (*(undefined4 *)0x00ea61d4u)
#define DAT_00ea61ec (*(undefined4 *)0x00ea61ecu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern undefined4 FUN_00443950();
extern void FUN_004e2970();

void FUN_004e4ed0(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  if (DAT_00605a3c == 1) {
    return;
  }
  if (((param_1 != 0xe4) && (param_1 != 0xe3)) && (param_1 != 0xe1)) {
    return;
  }
  if (*(int *)(&DAT_00ea61ec + DAT_00eaac28 * 0x49d8) == 0) {
    FUN_004e2970(0xf2,0,0);
    iVar1 = FUN_00443950(0x23);
    if (iVar1 == 0) {
      uVar2 = 0x1f;
    }
    else {
      if ((*(short *)(&DAT_00ea40dc + DAT_00eaac28 * 0x49d8) == 1) ||
         (*(short *)(&DAT_00ea40dc + DAT_00eaac28 * 0x49d8) == 3)) goto LAB_004e4f6d;
      uVar2 = 0x20;
    }
    FUN_004e2970(uVar2,0,0);
  }
LAB_004e4f6d: ;
  if (param_1 == 0xe1) {
    if (DAT_0078d6c0 != 0) {
      return;
    }
    if (*(int *)(&DAT_00ea61d4 + DAT_00eaac28 * 0x49d8) != 0) {
      return;
    }
    FUN_004e2970(0xf1,0,0);
    iVar1 = FUN_00443950(0x1d);
    if (iVar1 != 0) {
      if (*(short *)(&DAT_00ea40d0 + DAT_00eaac28 * 0x49d8) == 1) {
        return;
      }
      if (*(short *)(&DAT_00ea40d0 + DAT_00eaac28 * 0x49d8) == 3) {
        return;
      }
      FUN_004e2970(0x20,0,0);
      return;
    }
  }
  else {
    if (param_1 != 0xe3) {
      return;
    }
    if (DAT_0078d6b4 != 0) {
      return;
    }
    if (*(int *)(&DAT_00ea61c8 + DAT_00eaac28 * 0x49d8) != 0) {
      return;
    }
    FUN_004e2970(0xf8,0,0);
    iVar1 = FUN_00443950(0x1a);
    if (iVar1 != 0) {
      if (*(short *)(&DAT_00ea40ca + DAT_00eaac28 * 0x49d8) == 1) {
        return;
      }
      if (*(short *)(&DAT_00ea40ca + DAT_00eaac28 * 0x49d8) == 3) {
        return;
      }
      FUN_004e2970(0x20,0,0);
      return;
    }
  }
  FUN_004e2970(0x1f,0,0);
  return;
}
