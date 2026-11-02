#include "restore_types.h"
#define DAT_00605a3c (*(undefined *)0x00605a3cu)
#define DAT_0078d714 (*(undefined *)0x0078d714u)
#define DAT_0078d718 (*(undefined *)0x0078d718u)
#define DAT_00ea40b2 (*(undefined4 *)0x00ea40b2u)
#define DAT_00ea40b4 (*(undefined4 *)0x00ea40b4u)
#define DAT_00ea619c (*(undefined4 *)0x00ea619cu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern undefined4 FUN_00443900();
extern undefined4 FUN_00443950();
extern undefined4 FUN_004439c0();
extern void FUN_004e2970();

void FUN_004e3da0(int param_1)

{
  int iVar1;
  
  if (DAT_00605a3c == 1) {
    return;
  }
  if (param_1 != 0x20) {
    return;
  }
  if (0 < *(int *)(&DAT_00ea619c + DAT_00eaac28 * 0x49d8)) {
    return;
  }
  FUN_004e2970(0x89,0,0);
  if (0 < DAT_0078d714) {
    return;
  }
  iVar1 = FUN_00443900(0xe);
  if (((iVar1 == 0) && (iVar1 = FUN_004439c0(0xe), iVar1 == 0)) || (DAT_0078d718 != 0)) {
    iVar1 = FUN_004439c0(0xe);
    if ((iVar1 == 0) || (DAT_0078d718 == 0)) {
      iVar1 = FUN_004439c0(0xf);
      if (iVar1 != 0) {
        iVar1 = FUN_00443950(0xf);
        if (iVar1 == 0) {
          FUN_004e2970(0x1f,0,0);
        }
        else if ((*(short *)(&DAT_00ea40b4 + DAT_00eaac28 * 0x49d8) != 1) &&
                (*(short *)(&DAT_00ea40b4 + DAT_00eaac28 * 0x49d8) != 3)) {
          FUN_004e2970(0x20,0,0);
          return;
        }
      }
    }
    else {
      iVar1 = FUN_00443950(0xe);
      if (iVar1 == 0) {
        FUN_004e2970(0xa9,0,0);
        return;
      }
      if ((*(short *)(&DAT_00ea40b2 + DAT_00eaac28 * 0x49d8) != 1) &&
         (*(short *)(&DAT_00ea40b2 + DAT_00eaac28 * 0x49d8) != 3)) {
        FUN_004e2970(0xa8,0,0);
        return;
      }
    }
    return;
  }
  iVar1 = FUN_004439c0(0xf);
  if (iVar1 == 0) {
    FUN_004e2970(0x8a,0,0);
    return;
  }
  iVar1 = FUN_00443950(0xf);
  if (iVar1 == 0) {
    FUN_004e2970(0xc0,0,0);
    return;
  }
  if (*(short *)(&DAT_00ea40b4 + DAT_00eaac28 * 0x49d8) == 1) {
    return;
  }
  if (*(short *)(&DAT_00ea40b4 + DAT_00eaac28 * 0x49d8) == 3) {
    return;
  }
  FUN_004e2970(0xbf,0,0);
  return;
}
