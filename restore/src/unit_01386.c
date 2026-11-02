#include "restore_types.h"
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bcd609 (*(undefined4 *)0x00bcd609u)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
extern int FUN_004a59c0();

int FUN_004a5c10(int param_1)

{
  int iVar1;
  int iVar2;
  
  iVar1 = 0;
  iVar2 = DAT_00bca2d8 + (short)(&DAT_00bcd60c)[param_1 * 0xc2] +
          (short)(&DAT_00bcd60e)[param_1 * 0xc2] * 0xe4;
  switch((&DAT_00bcd609)[param_1 * 0x184]) {
  case 0: ;
    iVar1 = FUN_004a59c0(iVar2,param_1);
    if (iVar1 == 0) {
      iVar2 = FUN_004a59c0(iVar2 + -0xe4,param_1);
      return iVar2;
    }
    break;
  case 1: ;
    iVar1 = FUN_004a59c0(iVar2,param_1);
    if (iVar1 == 0) {
      iVar2 = FUN_004a59c0(iVar2 + -0xe3,param_1);
      return iVar2;
    }
    break;
  case 2: ;
    iVar1 = FUN_004a59c0(iVar2,param_1);
    if (iVar1 == 0) {
      iVar2 = FUN_004a59c0(iVar2 + 1,param_1);
      return iVar2;
    }
    break;
  case 3: ;
    iVar1 = FUN_004a59c0(iVar2,param_1);
    if (iVar1 == 0) {
      iVar2 = FUN_004a59c0(iVar2 + 0xe5,param_1);
      return iVar2;
    }
    break;
  case 4: ;
    iVar1 = FUN_004a59c0(iVar2,param_1);
    if (iVar1 == 0) {
      iVar2 = FUN_004a59c0(iVar2 + 0xe4,param_1);
      return iVar2;
    }
    break;
  case 5: ;
    iVar1 = FUN_004a59c0(iVar2,param_1);
    if (iVar1 == 0) {
      iVar2 = FUN_004a59c0(iVar2 + 0xe3,param_1);
      return iVar2;
    }
    break;
  case 6: ;
    iVar1 = FUN_004a59c0(iVar2,param_1);
    if (iVar1 == 0) {
      iVar2 = FUN_004a59c0(iVar2 + -1,param_1);
      return iVar2;
    }
    break;
  case 7: ;
    iVar1 = FUN_004a59c0(iVar2,param_1);
    if (iVar1 == 0) {
      iVar1 = FUN_004a59c0(iVar2 + -0xe5,param_1);
    }
  }
  return iVar1;
}
