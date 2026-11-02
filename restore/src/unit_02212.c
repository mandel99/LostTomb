#include "restore_types.h"
#define DAT_00f56a04 (*(undefined4 *)0x00f56a04u)
#define DAT_010d6a0e (*(undefined4 *)0x010d6a0eu)
#define DAT_0126517c (*(undefined *)0x0126517cu)
extern int FUN_0053e590();

void FUN_004f77e0(int param_1,int param_2,int param_3,int param_4)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar1 = DAT_00f56a04;
  if (DAT_0126517c <= DAT_00f56a04) {
    iVar1 = 0;
  }
  iVar3 = param_2 - (short)(&DAT_010d6a0e)[iVar1 * 0x20];
  FUN_0053e590(DAT_00f56a04,param_1,iVar3);
  iVar1 = DAT_00f56a04 + 1;
  param_2 = param_3 / 2 + param_3;
  if (0 < param_2) {
    do {
      iVar3 = iVar3 + -1;
      FUN_0053e590(iVar1,param_1 + 0xf,iVar3);
      param_2 = param_2 + -1;
    } while (param_2 != 0);
  }
  if (param_4 != 0) {
    iVar1 = DAT_00f56a04 + 2;
    iVar2 = iVar1;
    if (DAT_0126517c <= iVar1) {
      iVar2 = 0;
    }
    FUN_0053e590(iVar1,param_1 + -2,iVar3 - (short)(&DAT_010d6a0e)[iVar2 * 0x20]);
  }
  return;
}
