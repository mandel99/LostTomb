#include "restore_types.h"
#define DAT_00784d32 (*(undefined *)0x00784d32u)
#define DAT_00784d34 (*(undefined *)0x00784d34u)
#define DAT_00785d70 (*(undefined *)0x00785d70u)
#define DAT_00785d7c (*(undefined4 *)0x00785d7cu)
extern undefined4 FUN_004439c0();
extern undefined4 FUN_00443b90();
extern undefined4 FUN_00443d90();

undefined4 FUN_00559480(int param_1)

{
  char cVar1;
  int iVar2;
  short *psVar3;
  bool bVar4;
  undefined4 uVar5;
  
  if (DAT_00785d70 != 0) {
    psVar3 = &DAT_00785d7c + (DAT_00785d70 + -1) * 0x3e;
    iVar2 = DAT_00785d70 + -1;
    do {
      if ((((-1 < iVar2) && (iVar2 < DAT_00785d70)) && (psVar3 != (short *)0x8)) &&
         ((char)psVar3[-3] == '\x17')) {
        if (psVar3[-1] == param_1) {
          return 1;
        }
        if (*psVar3 == param_1) {
          return 1;
        }
        if (psVar3[1] == param_1) {
          return 1;
        }
      }
      psVar3 = psVar3 + -0x3e;
      bVar4 = iVar2 != 0;
      iVar2 = iVar2 + -1;
    } while (bVar4);
  }
  if (param_1 == 0xc) {
    iVar2 = FUN_00443b90(0xb);
    if (((iVar2 != 0) || (iVar2 = FUN_00443d90(0xb), iVar2 != 0)) &&
       (cVar1 = FUN_00559480(9), cVar1 != '\0')) {
      return 1;
    }
    cVar1 = FUN_00559480(0xb);
    if ((cVar1 != '\0') && (iVar2 = FUN_00443b90(1), iVar2 != 0)) {
      return 1;
    }
    iVar2 = FUN_004439c0(0xb);
    if ((iVar2 != 0) && (cVar1 = FUN_00559480(9), cVar1 != '\0')) {
      return 1;
    }
    cVar1 = FUN_00559480(0xb);
    if ((cVar1 != '\0') && (iVar2 = FUN_004439c0(9), iVar2 != 0)) {
      return 1;
    }
    cVar1 = FUN_00559480(0xb);
    if (cVar1 == '\0') {
      return 0;
    }
    uVar5 = 9;
  }
  else {
    if (param_1 == 0x22) {
      iVar2 = FUN_00443b90(0xb);
      if ((((iVar2 == 0) && (iVar2 = FUN_00443d90(0xb), iVar2 == 0)) &&
          (cVar1 = FUN_00559480(0xb), cVar1 == '\0')) && (cVar1 = FUN_00559480(0xd), cVar1 == '\0'))
      {
        return 0;
      }
      iVar2 = FUN_004439c0(0x1f);
      if (iVar2 == 0) {
        return 0;
      }
      return 1;
    }
    switch((int)(param_1)) {
    case 10: ;
      if (DAT_00784d32 == 0) {
        return 0;
      }
      uVar5 = 0x1d;
      break;
default: ;
      goto switchD_0055950b_caseD_b;
    case 0xd: ;
      uVar5 = 0xb;
      break;
    case 0xf: ;
      uVar5 = 0xe;
      break;
    case 0x11: ;
      uVar5 = 0x10;
      break;
    case 0x13: ;
      uVar5 = 0x12;
      break;
    case 0x17: ;
      uVar5 = 0x16;
      break;
    case 0x1c: ;
      if (DAT_00784d34 == 0) {
        return 0;
      }
      uVar5 = 0x14;
      break;
    case 0x21: ;
      uVar5 = 0x20;
    }
  }
  cVar1 = FUN_00559480(uVar5);
  if (cVar1 != '\0') {
    return 1;
  }
switchD_0055950b_caseD_b: ;
  return 0;
}
