#include "restore_types.h"
#define DAT_00f56a34 (*(undefined *)0x00f56a34u)
extern int FUN_0053e590();

void FUN_00435de0(uint param_1,int param_2,int param_3)

{
  uint uVar1;
  int iVar2;
  
  if (199 < (int)param_1) {
    param_1 = 200;
  }
  uVar1 = ((int)param_1 < 1) - 1 & 3;
  if ((int)param_1 <= (int)uVar1) {
    param_1 = uVar1;
  }
  param_1 = (int)param_1 / 3;
  if (0 < (int)param_1) {
    param_3 = param_3 + 10;
    do {
      if ((int)param_1 < 4) {
        uVar1 = param_1 & 0x80000003;
        if ((int)uVar1 < 0) {
          uVar1 = (uVar1 - 1 | 0xfffffffc) + 1;
        }
        iVar2 = uVar1 - 1;
      }
      else {
        iVar2 = 3;
      }
      FUN_0053e590(DAT_00f56a34 + iVar2,param_2 + 0xf,param_3);
      param_1 = param_1 - 4;
      param_3 = param_3 + -0xc;
    } while (0 < (int)param_1);
  }
  return;
}
