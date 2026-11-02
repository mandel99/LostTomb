#include "restore_types.h"
#define DAT_005cc8c0 (*(undefined *)0x005cc8c0u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)

undefined4 FUN_00486bf0(int param_1,int param_2)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  
  if (DAT_00bca2e0 != 0) {
    if (DAT_00bca2e0 == 6) {
      param_2 = param_2 + -1;
    }
    else if (DAT_00bca2e0 == 4) {
      param_1 = param_1 + -1;
      param_2 = param_2 + -1;
    }
    else if (DAT_00bca2e0 == 2) {
      param_1 = param_1 + -1;
    }
  }
  if (param_1 < 0) {
    return 999;
  }
  if (param_2 < 0) {
    return 999;
  }
  if (param_1 + 1 < DAT_00bca2d0) {
    if (DAT_00bca2d4 <= param_2 + 1) {
      return 999;
    }
    param_1 = DAT_00bca2d8 + param_2 * 0xe4 + param_1;
    iVar3 = (int)*(short *)(&DAT_00b4e6fc + param_1 * 2);
    if ((iVar3 != 0) && ((&DAT_007999e8)[iVar3 * 0x84] == 0x88)) {
      piVar4 = &DAT_005cc8c0;
      do {
        iVar2 = 0;
        piVar1 = piVar4;
        do {
          if (iVar3 != *(short *)(&DAT_00b4e6fc + (*piVar1 + param_1) * 2)) {
            return 0;
          }
          iVar2 = iVar2 + 1;
          piVar1 = piVar1 + 2;
        } while (iVar2 < 2);
        piVar4 = piVar4 + 0xc;
      } while ((int)piVar4 < 0x5cc920);
      return 1;
    }
    return 0;
  }
  return 999;
}
