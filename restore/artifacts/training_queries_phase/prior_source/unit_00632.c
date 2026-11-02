#include "restore_types.h"
#define DAT_0077de02 (*(undefined4 *)0x0077de02u)
#define DAT_0077de2a (*(undefined4 *)0x0077de2au)
extern undefined4 FUN_004439c0();
extern undefined4 FUN_00443b90();

undefined4 FUN_00443d90(int param_1)

{
  int iVar1;
  char *pcVar2;
  char *pcVar3;
  
  if (param_1 == 0xf) {
    param_1 = 0xe;
    iVar1 = 0xe;
LAB_00443e59: ;
    iVar1 = FUN_004439c0(iVar1);
  }
  else {
    if (param_1 == 0x11) {
      param_1 = 0x10;
      iVar1 = 0x10;
      goto LAB_00443e59;
    }
    if (param_1 == 0x21) {
      param_1 = 0x20;
      iVar1 = 0x20;
      goto LAB_00443e59;
    }
    if (param_1 == 0x13) {
      param_1 = 0x12;
      iVar1 = 0x12;
      goto LAB_00443e59;
    }
    if (param_1 == 0xd) {
      param_1 = 0xb;
      iVar1 = 0xb;
      goto LAB_00443e59;
    }
    if (param_1 == 10) {
      param_1 = 0x1d;
      iVar1 = 0x1d;
      goto LAB_00443e59;
    }
    if (param_1 == 0x1c) {
      param_1 = 0x14;
      iVar1 = 0x14;
      goto LAB_00443e59;
    }
    if (param_1 != 0xc) {
      if (param_1 == 0x22) {
        iVar1 = FUN_00443b90(0xd);
        if (iVar1 == 0) goto LAB_00443e6c;
        iVar1 = 0x1f;
      }
      else {
        iVar1 = param_1;
        if (param_1 == 0x17) {
          param_1 = 0x16;
          iVar1 = 0x16;
        }
      }
      goto LAB_00443e59;
    }
    iVar1 = FUN_00443b90(1);
    if ((iVar1 == 0) && (iVar1 = FUN_00443d90(9), iVar1 == 0)) goto LAB_00443e6c;
    iVar1 = FUN_00443b90(0xb);
    if (iVar1 != 0) {
      return 1;
    }
    iVar1 = FUN_00443d90(0xb);
  }
  if (iVar1 != 0) {
    return 1;
  }
LAB_00443e6c: ;
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
