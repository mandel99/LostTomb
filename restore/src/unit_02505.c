#include "restore_types.h"
#define DAT_005e4c20 (*(undefined4 *)0x005e4c20u)
#define DAT_006012ec (*(undefined4 *)0x006012ecu)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd603 (*(undefined4 *)0x00bcd603u)
#define DAT_00bcd652 (*(undefined4 *)0x00bcd652u)
#define _DAT_00784cb0 (*(undefined4 *)0x00784cb0u)
extern undefined4 FUN_004c5120();

int FUN_0053d280(int param_1)

{
  undefined4 uVar1;
  int iVar2;
  
  iVar2 = *(int *)(&DAT_006012ec + (uint)(byte)(&DAT_00bcd602)[param_1 * 0x184] * 4);
  if (iVar2 < 1) {
    if (iVar2 == -4) {
      iVar2 = *(int *)(&DAT_006012ec + (uint)(byte)(&DAT_00bcd603)[param_1 * 0x184] * 4);
      switch((int)(iVar2)) {
      case -4: ;
      case -2: ;
      case -1: ;
        break;
      case -3: ;
        uVar1 = FUN_004c5120((int)(short)(&DAT_00bcd652)[param_1 * 0xc2]);
        switch((int)(uVar1)) {
        case 0: ;
switchD_0053d334_caseD_0: ;
          return 0xe6;
        case 1: ;
switchD_0053d334_caseD_1: ;
          return 0xe7;
        case 2: ;
switchD_0053d334_caseD_2: ;
          return 0xe8;
        case 3: ;
switchD_0053d334_caseD_3: ;
          return 0xe9;
        case 4: ;
switchD_0053d334_caseD_4: ;
          return 0xea;
        }
        break;
default: ;
        goto switchD_0053d2cd_default;
      }
    }
    else if (iVar2 == -2) {
      iVar2 = *(int *)(&DAT_005e4c20 + (_DAT_00784cb0 & 0xff) * 0x30);
      if (iVar2 == 0x44) {
        return 0xd2;
      }
      if (iVar2 == 0x45) {
        return 0xd1;
      }
      if (iVar2 == 0x46) {
        return 0xd0;
      }
    }
    else if (iVar2 == -3) {
      uVar1 = FUN_004c5120((int)(short)(&DAT_00bcd652)[param_1 * 0xc2]);
      switch((int)(uVar1)) {
      case 0: ;
        goto switchD_0053d334_caseD_0;
      case 1: ;
        goto switchD_0053d334_caseD_1;
      case 2: ;
        goto switchD_0053d334_caseD_2;
      case 3: ;
        goto switchD_0053d334_caseD_3;
      case 4: ;
        goto switchD_0053d334_caseD_4;
      }
    }
    iVar2 = -1;
  }
switchD_0053d2cd_default: ;
  return iVar2;
}
