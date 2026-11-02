#include "restore_types.h"
#define DAT_00605a3c (*(undefined *)0x00605a3cu)
#define DAT_0078d67c (*(undefined *)0x0078d67cu)
#define DAT_0078d6ac (*(undefined *)0x0078d6acu)
#define DAT_0078d6b0 (*(undefined *)0x0078d6b0u)
#define DAT_00ea40ae (*(undefined4 *)0x00ea40aeu)
#define DAT_00ea40c6 (*(undefined4 *)0x00ea40c6u)
#define DAT_00ea40c8 (*(undefined4 *)0x00ea40c8u)
#define DAT_00ea6190 (*(undefined4 *)0x00ea6190u)
#define DAT_00ea61c0 (*(undefined4 *)0x00ea61c0u)
#define DAT_00ea61c4 (*(undefined4 *)0x00ea61c4u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern undefined4 FUN_00443950();
extern undefined4 FUN_00443b90();
extern void FUN_004e2970();
extern int FUN_004f44e0();

void FUN_004e47d0(int param_1)

{
  undefined4 uVar1;
  int iVar2;
  
  if (DAT_00605a3c == 1) {
    return;
  }
  if (param_1 != 0xb7) {
    return;
  }
  uVar1 = FUN_004f44e0();
  switch((int)(uVar1)) {
  case 1: ;
  case 4: ;
    if ((0 < DAT_0078d6ac) && (0 < DAT_0078d6b0)) {
      return;
    }
    iVar2 = *(int *)(&DAT_00ea61c0 + DAT_00eaac28 * 0x24ec);
    if ((0 < iVar2) && (0 < *(int *)(&DAT_00ea61c4 + DAT_00eaac28 * 0x24ec))) {
      return;
    }
    if ((DAT_0078d6ac < 1) && (iVar2 < 1)) {
      if ((DAT_0078d6b0 < 1) && (*(int *)(&DAT_00ea61c4 + DAT_00eaac28 * 0x24ec) < 1)) {
        FUN_004e2970(0x96,0,0);
        iVar2 = FUN_00443b90(0x18);
        if (iVar2 == 0) {
          iVar2 = FUN_00443950(0x18);
          if (iVar2 == 0) {
            uVar1 = 0x98;
            goto LAB_004e48cc;
          }
          if ((*(short *)(&DAT_00ea40c6 + DAT_00eaac28 * 0x49d8) != 1) &&
             (*(short *)(&DAT_00ea40c6 + DAT_00eaac28 * 0x49d8) != 3)) {
            uVar1 = 0x99;
            goto LAB_004e48cc;
          }
        }
        else {
          uVar1 = 0x97;
LAB_004e48cc: ;
          FUN_004e2970(uVar1,0,0);
        }
        iVar2 = FUN_00443b90(0x19);
        if (iVar2 != 0) {
          FUN_004e2970(0x9a,0,0);
          return;
        }
        iVar2 = FUN_00443950(0x19);
        if (iVar2 != 0) {
          if (*(short *)(&DAT_00ea40c8 + DAT_00eaac28 * 0x49d8) == 1) {
            return;
          }
          if (*(short *)(&DAT_00ea40c8 + DAT_00eaac28 * 0x49d8) == 3) {
            return;
          }
          FUN_004e2970(0x9c,0,0);
          return;
        }
LAB_004e4b8c: ;
        FUN_004e2970(0x9b,0,0);
        return;
      }
      if (iVar2 < 1) {
        FUN_004e2970(0x9d,0,0);
        iVar2 = FUN_00443b90(0x18);
        if (iVar2 != 0) {
          FUN_004e2970(0x97,0,0);
          return;
        }
        iVar2 = FUN_00443950(0x18);
        if (iVar2 != 0) {
          if (*(short *)(&DAT_00ea40c6 + DAT_00eaac28 * 0x49d8) == 1) {
            return;
          }
          if (*(short *)(&DAT_00ea40c6 + DAT_00eaac28 * 0x49d8) == 3) {
            return;
          }
          FUN_004e2970(0x20,0,0);
          return;
        }
        break;
      }
    }
    if (0 < DAT_0078d6b0) {
      return;
    }
    if (0 < *(int *)(&DAT_00ea61c4 + DAT_00eaac28 * 0x24ec)) {
      return;
    }
    FUN_004e2970(0x9e,0,0);
    iVar2 = FUN_00443b90(0x19);
    if (iVar2 != 0) {
      FUN_004e2970(0x9a,0,0);
      return;
    }
    iVar2 = FUN_00443950(0x19);
    if (iVar2 != 0) {
      if (*(short *)(&DAT_00ea40c8 + DAT_00eaac28 * 0x49d8) == 1) {
        return;
      }
      if (*(short *)(&DAT_00ea40c8 + DAT_00eaac28 * 0x49d8) == 3) {
        return;
      }
      FUN_004e2970(0x20,0,0);
      return;
    }
    break;
  case 2: ;
    if ((0 < DAT_0078d67c) && (0 < DAT_0078d6b0)) {
      return;
    }
    iVar2 = *(int *)(&DAT_00ea6190 + DAT_00eaac28 * 0x49d8);
    if ((0 < iVar2) && (0 < *(int *)(&DAT_00ea61c4 + DAT_00eaac28 * 0x24ec))) {
      return;
    }
    if ((DAT_0078d67c < 1) && (iVar2 < 1)) {
      if ((DAT_0078d6b0 < 1) && (*(int *)(&DAT_00ea61c4 + DAT_00eaac28 * 0x24ec) < 1)) {
        FUN_004e2970(0x9f,0,0);
        iVar2 = FUN_00443b90(0xc);
        if (iVar2 == 0) {
          iVar2 = FUN_00443950(0xc);
          if (iVar2 == 0) {
            uVar1 = 0xa1;
            goto LAB_004e4b19;
          }
          if ((*(short *)(&DAT_00ea40ae + DAT_00eaac28 * 0x49d8) != 1) &&
             (*(short *)(&DAT_00ea40ae + DAT_00eaac28 * 0x49d8) != 3)) {
            uVar1 = 0xa2;
            goto LAB_004e4b19;
          }
        }
        else {
          uVar1 = 0xa0;
LAB_004e4b19: ;
          FUN_004e2970(uVar1,0,0);
        }
        iVar2 = FUN_00443b90(0x19);
        if (iVar2 != 0) {
          FUN_004e2970(0x9a,0,0);
          return;
        }
        iVar2 = FUN_00443950(0x19);
        if (iVar2 != 0) {
          if (*(short *)(&DAT_00ea40c8 + DAT_00eaac28 * 0x49d8) == 1) {
            return;
          }
          if (*(short *)(&DAT_00ea40c8 + DAT_00eaac28 * 0x49d8) == 3) {
            return;
          }
          FUN_004e2970(0x9c,0,0);
          return;
        }
        goto LAB_004e4b8c;
      }
      if (iVar2 < 1) {
        FUN_004e2970(0xa3,0,0);
        iVar2 = FUN_00443b90(0xc);
        if (iVar2 != 0) {
          FUN_004e2970(0xa0,0,0);
          return;
        }
        iVar2 = FUN_00443950(0xc);
        if (iVar2 != 0) {
          if (*(short *)(&DAT_00ea40ae + DAT_00eaac28 * 0x49d8) == 1) {
            return;
          }
          if (*(short *)(&DAT_00ea40ae + DAT_00eaac28 * 0x49d8) == 3) {
            return;
          }
          FUN_004e2970(0x20,0,0);
          return;
        }
        break;
      }
    }
    if (0 < DAT_0078d6b0) {
      return;
    }
    if (0 < *(int *)(&DAT_00ea61c4 + DAT_00eaac28 * 0x24ec)) {
      return;
    }
    FUN_004e2970(0x9e,0,0);
    iVar2 = FUN_00443b90(0x19);
    if (iVar2 != 0) {
      FUN_004e2970(0x9a,0,0);
      return;
    }
    iVar2 = FUN_00443950(0x19);
    if (iVar2 != 0) {
      if (*(short *)(&DAT_00ea40c8 + DAT_00eaac28 * 0x49d8) == 1) {
        return;
      }
      if (*(short *)(&DAT_00ea40c8 + DAT_00eaac28 * 0x49d8) == 3) {
        return;
      }
      FUN_004e2970(0x20,0,0);
      return;
    }
    break;
  case 3: ;
    if (0 < DAT_0078d6ac) {
      return;
    }
    if (0 < *(int *)(&DAT_00ea61c0 + DAT_00eaac28 * 0x24ec)) {
      return;
    }
    FUN_004e2970(0x9d,0,0);
    iVar2 = FUN_00443b90(0x18);
    if (iVar2 != 0) {
      FUN_004e2970(0x97,0,0);
      return;
    }
    iVar2 = FUN_00443950(0x18);
    if (iVar2 != 0) {
      if (*(short *)(&DAT_00ea40c6 + DAT_00eaac28 * 0x49d8) == 1) {
        return;
      }
      if (*(short *)(&DAT_00ea40c6 + DAT_00eaac28 * 0x49d8) == 3) {
        return;
      }
      FUN_004e2970(0x20,0,0);
      return;
    }
    break;
  case 5: ;
    if (0 < DAT_0078d67c) {
      return;
    }
    if (0 < *(int *)(&DAT_00ea6190 + DAT_00eaac28 * 0x49d8)) {
      return;
    }
    FUN_004e2970(0xa3,0,0);
    iVar2 = FUN_00443b90(0xc);
    if (iVar2 != 0) {
      FUN_004e2970(0xa0,0,0);
      return;
    }
    iVar2 = FUN_00443950(0xc);
    if (iVar2 != 0) {
      if (*(short *)(&DAT_00ea40ae + DAT_00eaac28 * 0x49d8) == 1) {
        return;
      }
      if (*(short *)(&DAT_00ea40ae + DAT_00eaac28 * 0x49d8) == 3) {
        return;
      }
      FUN_004e2970(0x20,0,0);
      return;
    }
    break;
default: ;
    goto switchD_004e47fd_default;
  }
  FUN_004e2970(0x1f,0,0);
switchD_004e47fd_default: ;
  return;
}
