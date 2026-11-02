#include "restore_types.h"
#define DAT_00d3b5c0 (*(undefined *)0x00d3b5c0u)
#define DAT_00f56754 (*(undefined *)0x00f56754u)
#define DAT_010d6a0e (*(undefined4 *)0x010d6a0eu)
#define DAT_0126517c (*(undefined *)0x0126517cu)
extern int FUN_0053e590();

void FUN_004361a0(int param_1,int param_2,int param_3,int param_4)

{
  int iVar1;
  short sVar2;
  int iVar3;
  int iVar4;
  
  iVar4 = param_1;
  if (10 < param_1) {
    iVar4 = 10;
  }
  param_1 = 0;
  if (param_4 == 2) {
    param_1 = 0xc;
  }
  else if (param_4 != 0) {
    if (iVar4 < 9) {
      if (iVar4 < 7) {
        if (4 < iVar4) {
          param_1 = 3;
        }
      }
      else {
        param_1 = 6;
      }
    }
    else {
      param_1 = 9;
    }
  }
  iVar1 = DAT_00f56754 + param_1;
  iVar3 = iVar1;
  if (DAT_0126517c <= iVar1) {
    iVar3 = 0;
  }
  sVar2 = (&DAT_010d6a0e)[iVar3 * 0x20];
  if (iVar4 == 0) {
    DAT_00d3b5c0 = iVar1 + 2;
    FUN_0053e590(DAT_00d3b5c0,param_2 + 9,param_3 + -8);
    return;
  }
  if (iVar4 == 1) {
    FUN_0053e590(iVar1 + 2,param_2 + 9,param_3 + -8);
    FUN_0053e590(iVar1,param_2 + 5,(param_3 - sVar2) + 5);
    return;
  }
  FUN_0053e590(iVar1 + 2,param_2 + 9,param_3 + -8);
  FUN_0053e590(iVar1,param_2 + 5,((param_3 + iVar4 * -10) - (int)sVar2) + 0xf);
  DAT_00d3b5c0 = DAT_00f56754 + 1 + param_1;
  iVar4 = iVar4 + -1;
  if (0 < iVar4) {
    param_3 = param_3 + -5;
    do {
      FUN_0053e590(DAT_00d3b5c0,param_2 + 0x11,param_3);
      param_3 = param_3 + -10;
      iVar4 = iVar4 + -1;
    } while (iVar4 != 0);
  }
  return;
}
