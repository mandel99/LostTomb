#include "restore_types.h"
#define DAT_00bcd694 (*(undefined4 *)0x00bcd694u)
#define DAT_00e20778 (*(undefined *)0x00e20778u)
#define DAT_00f571fc (*(undefined *)0x00f571fcu)
extern int FUN_0053e590();

void FUN_004fbf30(int param_1,int param_2,undefined4 param_3)

{
  undefined4 uVar1;
  int iVar2;
  int *piVar3;
  int iVar4;
  
  iVar4 = param_2;
  uVar1 = DAT_00e20778;
  param_2 = 4;
  piVar3 = (int *)(&DAT_00bcd694 + param_1 * 0x184);
  do {
    if (*piVar3 != -1) {
      switch(*piVar3) {
      case 0: ;
        iVar2 = 1;
        break;
      case 1: ;
        iVar2 = 2;
        break;
      case 2: ;
        iVar2 = 3;
        break;
      case 3: ;
        iVar2 = 4;
        break;
      case 4: ;
        iVar2 = 5;
        break;
      case 5: ;
        iVar2 = 6;
        break;
      case 6: ;
        iVar2 = 7;
        break;
      case 7: ;
        iVar2 = 8;
        break;
      case 8: ;
        iVar2 = 0xf;
        break;
      case 9: ;
        iVar2 = 0xd;
        break;
      case 10: ;
        iVar2 = 0x11;
        break;
      case 0xb: ;
        iVar2 = 0x13;
        break;
default: ;
        iVar2 = -1;
      }
      DAT_00e20778 = uVar1;
      FUN_0053e590(DAT_00f571fc + iVar2,iVar4,param_3);
      iVar4 = iVar4 + 0x20;
    }
    piVar3 = piVar3 + 1;
    param_2 = param_2 + -1;
  } while (param_2 != 0);
  DAT_00e20778 = uVar1;
  return;
}
