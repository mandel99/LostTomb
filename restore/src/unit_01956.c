#include "restore_types.h"
#define DAT_00605a3c (*(undefined *)0x00605a3cu)
#define DAT_0078d71c (*(undefined *)0x0078d71cu)
#define DAT_0078d720 (*(undefined *)0x0078d720u)
#define DAT_00ea40b6 (*(undefined4 *)0x00ea40b6u)
#define DAT_00ea40b8 (*(undefined4 *)0x00ea40b8u)
#define DAT_00ea61a4 (*(undefined4 *)0x00ea61a4u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern undefined4 FUN_00443900();
extern undefined4 FUN_00443950();
extern undefined4 FUN_004439c0();
extern void FUN_004e2970();

void FUN_004e42d0(int param_1)

{
  int iVar1;
  
  if (DAT_00605a3c == 1) {
    return;
  }
  if (param_1 != 0x2f) {
    return;
  }
  if (0 < *(int *)(&DAT_00ea61a4 + DAT_00eaac28 * 0x49d8)) {
    return;
  }
  FUN_004e2970(0x8d,0,0);
  if (0 < DAT_0078d71c) {
    return;
  }
  iVar1 = FUN_00443900(0x10);
  if (((iVar1 == 0) && (iVar1 = FUN_004439c0(0x10), iVar1 == 0)) || (DAT_0078d720 != 0)) {
    iVar1 = FUN_004439c0(0x10);
    if ((iVar1 == 0) || (DAT_0078d720 == 0)) {
      iVar1 = FUN_004439c0(0x11);
      if (iVar1 != 0) {
        iVar1 = FUN_00443950(0x11);
        if (iVar1 == 0) {
          FUN_004e2970(0x1f,0,0);
        }
        else if ((*(short *)(&DAT_00ea40b8 + DAT_00eaac28 * 0x49d8) != 1) &&
                (*(short *)(&DAT_00ea40b8 + DAT_00eaac28 * 0x49d8) != 3)) {
          FUN_004e2970(0x20,0,0);
          return;
        }
      }
    }
    else {
      iVar1 = FUN_00443950(0x10);
      if (iVar1 == 0) {
        FUN_004e2970(0xaf,0,0);
        return;
      }
      if ((*(short *)(&DAT_00ea40b6 + DAT_00eaac28 * 0x49d8) != 1) &&
         (*(short *)(&DAT_00ea40b6 + DAT_00eaac28 * 0x49d8) != 3)) {
        FUN_004e2970(0xae,0,0);
        return;
      }
    }
    return;
  }
  iVar1 = FUN_004439c0(0x11);
  if (iVar1 == 0) {
    FUN_004e2970(0x8e,0,0);
    return;
  }
  iVar1 = FUN_00443950(0x11);
  if (iVar1 == 0) {
    FUN_004e2970(0xc4,0,0);
    return;
  }
  if (*(short *)(&DAT_00ea40b8 + DAT_00eaac28 * 0x49d8) == 1) {
    return;
  }
  if (*(short *)(&DAT_00ea40b8 + DAT_00eaac28 * 0x49d8) == 3) {
    return;
  }
  FUN_004e2970(0xc3,0,0);
  return;
}
