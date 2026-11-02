#include "restore_types.h"
#define DAT_01266a88 (*(undefined *)0x01266a88u)
#define DAT_01266a8c (*(undefined *)0x01266a8cu)
extern void * FUN_005645a0();
extern SIZE_T FUN_005647c2();

void FUN_005618e6(undefined4 param_1)

{
  uint uVar1;
  int iVar2;
  
  uVar1 = FUN_005647c2(DAT_01266a8c);
  if (uVar1 < (uint)((int)DAT_01266a88 + (4 - DAT_01266a8c))) {
    iVar2 = FUN_005647c2(DAT_01266a8c);
    iVar2 = FUN_005645a0(DAT_01266a8c,iVar2 + 0x10);
    if (iVar2 == 0) {
      return;
    }
    DAT_01266a88 = (undefined4 *)(iVar2 + ((int)DAT_01266a88 - DAT_01266a8c >> 2) * 4);
    DAT_01266a8c = iVar2;
  }
  *(undefined4 *)(DAT_01266a88) = param_1;
  DAT_01266a88 = DAT_01266a88 + 1;
  return;
}
