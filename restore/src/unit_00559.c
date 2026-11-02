#include "restore_types.h"
#define DAT_00f56dec (*(undefined *)0x00f56decu)
#define DAT_010d6a1a (*(undefined4 *)0x010d6a1au)
#define DAT_010d6a1c (*(undefined4 *)0x010d6a1cu)
#define DAT_0126517c (*(undefined *)0x0126517cu)
extern int FUN_0053e590();

int FUN_00438770(undefined4 param_1,int param_2,int param_3,int param_4)

{
  int iVar1;
  int iVar2;
  
  param_2 = DAT_00f56dec + -1 + param_2;
  iVar1 = param_2;
  if (DAT_0126517c <= param_2) {
    iVar1 = 0;
  }
  iVar2 = param_2;
  if (DAT_0126517c <= param_2) {
    iVar2 = 0;
  }
  return FUN_0053e590(param_2,(0x6c - (short)(&DAT_010d6a1a)[iVar1 * 0x20]) + param_3,
               (param_4 - (short)(&DAT_010d6a1c)[iVar2 * 0x20]) + 1);
}
