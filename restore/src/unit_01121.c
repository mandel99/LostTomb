#include "restore_types.h"
#define DAT_00b285cc (*(undefined4 *)0x00b285ccu)
#define DAT_00b67d1c (*(undefined4 *)0x00b67d1cu)
#define DAT_00bc9738 (*(undefined *)0x00bc9738u)
#define DAT_00bc973c (*(undefined *)0x00bc973cu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)

void FUN_0047c7b0(int param_1,int param_2)

{
  byte bVar1;
  int iVar2;
  int iVar3;
  
  param_1 = DAT_00bca2d8 + param_2 * 0xe4 + param_1;
  bVar1 = (&DAT_00b285cc)[param_1] & 0xf;
  if (bVar1 == 1) {
    iVar2 = 2;
    DAT_00bc973c = 2;
    DAT_00bc9738 = 2;
  }
  else if (bVar1 == 2) {
    iVar2 = 3;
    DAT_00bc973c = 3;
    DAT_00bc9738 = 3;
  }
  else if (bVar1 == 3) {
    iVar2 = 4;
    DAT_00bc973c = 4;
    DAT_00bc9738 = 4;
  }
  else if (bVar1 == 4) {
    iVar2 = 5;
    DAT_00bc973c = 5;
    DAT_00bc9738 = 5;
  }
  else if (bVar1 == 5) {
    iVar2 = 6;
    DAT_00bc973c = 6;
    DAT_00bc9738 = 6;
  }
  else {
    iVar2 = 1;
    DAT_00bc973c = 1;
    DAT_00bc9738 = 1;
  }
  iVar3 = 0;
  if (iVar2 != 0) {
    do {
      if (((&DAT_00b67d1c)[param_1] & 7) == 0) break;
      param_1 = param_1 + -1;
      iVar3 = iVar3 + 1;
    } while (iVar3 < iVar2);
  }
  iVar2 = 0;
  if (DAT_00bc9738 != 0) {
    do {
      if (((&DAT_00b67d1c)[param_1] & 0x38) == 0) {
        return;
      }
      param_1 = param_1 + -0xe4;
      iVar2 = iVar2 + 1;
    } while (iVar2 < DAT_00bc9738);
  }
  return;
}
