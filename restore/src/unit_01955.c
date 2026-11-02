#include "restore_types.h"
#define DAT_00605a3c (*(undefined *)0x00605a3cu)
#define DAT_0078d734 (*(undefined *)0x0078d734u)
#define DAT_0078d738 (*(undefined *)0x0078d738u)
#define DAT_00ea40c2 (*(undefined4 *)0x00ea40c2u)
#define DAT_00ea40c4 (*(undefined4 *)0x00ea40c4u)
#define DAT_00ea61bc (*(undefined4 *)0x00ea61bcu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern undefined4 FUN_00443900();
extern undefined4 FUN_00443950();
extern undefined4 FUN_004439c0();
extern void FUN_004e2970();

void FUN_004e4120(int param_1)

{
  int iVar1;
  
  if (DAT_00605a3c == 1) {
    return;
  }
  if ((param_1 != 0x33) && (param_1 != 0x35)) {
    return;
  }
  if (0 < (int)(&DAT_00ea61bc)[DAT_00eaac28 * 0x1276]) {
    return;
  }
  FUN_004e2970(0x8b,0,0);
  if (DAT_0078d734 < 1) {
    iVar1 = FUN_00443900(0x16);
    if (((iVar1 != 0) || (iVar1 = FUN_004439c0(0x16), iVar1 != 0)) && (DAT_0078d738 == 0)) {
      iVar1 = FUN_004439c0(0x17);
      if (iVar1 == 0) {
        FUN_004e2970(0x8c,0,0);
        return;
      }
      iVar1 = FUN_00443950(0x17);
      if (iVar1 != 0) {
        if (*(short *)(&DAT_00ea40c4 + DAT_00eaac28 * 0x49d8) == 1) {
          return;
        }
        FUN_004e2970(0xc1,0,0);
        return;
      }
      FUN_004e2970(0xc2,0,0);
      return;
    }
    iVar1 = FUN_004439c0(0x16);
    if ((iVar1 == 0) || (DAT_0078d738 == 0)) {
      iVar1 = FUN_004439c0(0x17);
      if (iVar1 != 0) {
        iVar1 = FUN_00443950(0x17);
        if (iVar1 == 0) {
          FUN_004e2970(0x1f,0,0);
        }
        else if ((*(short *)(&DAT_00ea40c4 + DAT_00eaac28 * 0x49d8) != 1) &&
                (*(short *)(&DAT_00ea40c4 + DAT_00eaac28 * 0x49d8) != 3)) {
          FUN_004e2970(0x20,0,0);
          return;
        }
      }
    }
    else {
      iVar1 = FUN_00443950(0x16);
      if (iVar1 == 0) {
        FUN_004e2970(0xab,0,0);
        return;
      }
      if ((*(short *)(&DAT_00ea40c2 + DAT_00eaac28 * 0x49d8) != 1) &&
         (*(short *)(&DAT_00ea40c2 + DAT_00eaac28 * 0x49d8) != 3)) {
        FUN_004e2970(0xaa,0,0);
        return;
      }
    }
    return;
  }
  return;
}
