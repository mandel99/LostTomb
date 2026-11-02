#include "restore_types.h"
#define DAT_0077de02 (*(undefined4 *)0x0077de02u)
#define DAT_0077de2a (*(undefined4 *)0x0077de2au)
#define DAT_00784d32 (*(undefined *)0x00784d32u)
#define DAT_00784d34 (*(undefined *)0x00784d34u)
extern undefined4 FUN_00443950();
extern undefined4 FUN_004439c0();
extern undefined4 FUN_00443d90();

undefined4 FUN_00443b90(int param_1)

{
  int iVar1;
  char *pcVar2;
  char *pcVar3;
  
  if (param_1 == 0xf) {
    param_1 = 0xe;
    iVar1 = FUN_004439c0(0xe);
  }
  else if (param_1 == 0x11) {
    param_1 = 0x10;
    iVar1 = FUN_004439c0(0x10);
  }
  else if (param_1 == 0x21) {
    param_1 = 0x20;
    iVar1 = FUN_004439c0(0x20);
  }
  else if (param_1 == 0x22) {
    iVar1 = FUN_00443b90(0xd);
    if (((iVar1 != 0) || (iVar1 = FUN_00443d90(0xd), iVar1 != 0)) &&
       (iVar1 = FUN_00443950(0x1f), iVar1 != 0)) {
      return 1;
    }
    iVar1 = FUN_004439c0(0xd);
    if (iVar1 == 0) goto LAB_00443d55;
    iVar1 = FUN_00443950(0x1f);
  }
  else if (param_1 == 0x13) {
    param_1 = 0x12;
    iVar1 = FUN_004439c0(0x12);
  }
  else if (param_1 == 0xd) {
    param_1 = 0xb;
    iVar1 = FUN_004439c0(0xb);
  }
  else if (param_1 == 10) {
    param_1 = 0x1d;
    if (DAT_00784d32 == 0) {
      return 0;
    }
    iVar1 = FUN_004439c0(0x1d);
  }
  else if (param_1 == 0x1c) {
    param_1 = 0x14;
    if (DAT_00784d34 == 0) {
      return 0;
    }
    iVar1 = FUN_004439c0(0x14);
  }
  else if (param_1 == 0xc) {
    iVar1 = FUN_00443b90(0xb);
    if (((iVar1 != 0) || (iVar1 = FUN_00443d90(0xb), iVar1 != 0)) &&
       (iVar1 = FUN_00443b90(1), iVar1 != 0)) {
      return 1;
    }
    iVar1 = FUN_00443b90(0xb);
    if (((iVar1 != 0) || (iVar1 = FUN_00443d90(0xb), iVar1 != 0)) &&
       (iVar1 = FUN_004439c0(9), iVar1 != 0)) {
      return 1;
    }
    iVar1 = FUN_00443b90(1);
    if ((iVar1 != 0) && (iVar1 = FUN_004439c0(0xb), iVar1 != 0)) {
      return 1;
    }
    iVar1 = FUN_004439c0(9);
    if (iVar1 == 0) goto LAB_00443d55;
    iVar1 = FUN_004439c0(0xb);
  }
  else if (param_1 == 0x17) {
    param_1 = 0x16;
    iVar1 = FUN_004439c0(0x16);
  }
  else {
    if (param_1 != 9) goto LAB_00443d55;
    iVar1 = FUN_00443b90(1);
  }
  if (iVar1 != 0) {
    return 1;
  }
LAB_00443d55: ;
  pcVar2 = &DAT_0077de02;
  pcVar3 = &DAT_0077de2a + param_1;
  while (((pcVar2[-2] == '\0' || (*pcVar2 != '\0')) || (*pcVar3 != '\x01'))) {
    pcVar2 = pcVar2 + 0x6a;
    pcVar3 = pcVar3 + 0x6a;
    if (0x77f743 < (int)pcVar2) {
      return 0;
    }
  }
  return 1;
}
