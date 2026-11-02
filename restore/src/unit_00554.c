#include "restore_types.h"
#define DAT_00f56f44 (*(undefined *)0x00f56f44u)
#define DAT_00f56f4c (*(undefined *)0x00f56f4cu)
#define DAT_010d6a1a (*(undefined4 *)0x010d6a1au)
#define DAT_010d6a1c (*(undefined4 *)0x010d6a1cu)
#define DAT_0126517c (*(undefined *)0x0126517cu)
extern int FUN_0053e590();

int FUN_004384d0(undefined4 param_1,int param_2,int param_3,int param_4)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar3 = DAT_00f56f44 + -1 + param_2;
  iVar1 = iVar3;
  if (DAT_0126517c <= iVar3) {
    iVar1 = 0;
  }
  iVar2 = iVar3;
  if (DAT_0126517c <= iVar3) {
    iVar2 = 0;
  }
  FUN_0053e590(iVar3,(0x5a - (short)(&DAT_010d6a1a)[iVar1 * 0x20]) + param_3,
               (param_4 - (short)(&DAT_010d6a1c)[iVar2 * 0x20]) + 0x11);
  param_2 = DAT_00f56f4c + -1 + param_2;
  iVar3 = param_2;
  if (DAT_0126517c <= param_2) {
    iVar3 = 0;
  }
  iVar1 = param_2;
  if (DAT_0126517c <= param_2) {
    iVar1 = 0;
  }
  return FUN_0053e590(param_2,(0x28 - (short)(&DAT_010d6a1a)[iVar3 * 0x20]) + param_3,
               (param_4 - (short)(&DAT_010d6a1c)[iVar1 * 0x20]) + 0x14);
}
