#include "restore_types.h"
#define DAT_00f566fc (*(undefined *)0x00f566fcu)
#define DAT_00f56e04 (*(undefined *)0x00f56e04u)
#define DAT_00f56e0c (*(undefined *)0x00f56e0cu)
#define DAT_010d6a1a (*(undefined4 *)0x010d6a1au)
#define DAT_010d6a1c (*(undefined4 *)0x010d6a1cu)
#define DAT_0126517c (*(undefined *)0x0126517cu)
extern int FUN_0053e590();

int FUN_00438590(int param_1,int param_2,int param_3,int param_4)

{
  int iVar1;
  int iVar2;
  
  if (param_1 == DAT_00f566fc) {
    param_2 = DAT_00f56e04 + -1 + param_2;
    iVar1 = param_2;
    if (DAT_0126517c <= param_2) {
      iVar1 = 0;
    }
    iVar2 = 0x34 - (short)(&DAT_010d6a1a)[iVar1 * 0x20];
    iVar1 = param_2;
    if (DAT_0126517c <= param_2) {
      iVar1 = 0;
    }
    param_4 = (param_4 - (short)(&DAT_010d6a1c)[iVar1 * 0x20]) + -3;
  }
  else {
    param_2 = DAT_00f56e0c + -1 + param_2;
    iVar1 = param_2;
    if (DAT_0126517c <= param_2) {
      iVar1 = 0;
    }
    iVar2 = 0x14 - (short)(&DAT_010d6a1a)[iVar1 * 0x20];
    iVar1 = param_2;
    if (DAT_0126517c <= param_2) {
      iVar1 = 0;
    }
    param_4 = param_4 - (short)(&DAT_010d6a1c)[iVar1 * 0x20];
  }
  return FUN_0053e590(param_2,iVar2 + param_3,param_4);
}
