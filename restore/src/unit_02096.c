#include "restore_types.h"
#define DAT_005cc8c0 (*(undefined *)0x005cc8c0u)
#define DAT_007999db (*(undefined4 *)0x007999dbu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)

void FUN_004ee6c0(int *param_1)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int *piVar5;
  uint uVar6;
  int iVar7;
  uint local_4;
  
  iVar2 = (int)param_1;
  iVar3 = (int)param_1 * 0x108;
  iVar7 = (int)*(short *)((int)&DAT_007999dc + iVar3 + 2);
  iVar4 = (int)*(short *)(&DAT_007999e0 + iVar3);
  uVar6 = (uint)(byte)(&DAT_007999db)[iVar3];
  if ((((-1 < iVar7) && (-1 < iVar4)) && ((int)((uVar6 - 1) + iVar7) < DAT_00bca2d0)) &&
     (((int)((uVar6 - 1) + iVar4) < DAT_00bca2d4 &&
      (iVar7 = DAT_00bca2d8 + iVar4 * 0xe4 + iVar7, DAT_00bc9768 = iVar7, uVar6 != 0)))) {
    param_1 = &DAT_005cc8c0;
    uVar1 = uVar6;
    piVar5 = param_1;
    local_4 = uVar6;
    do {
      for (; uVar1 != 0; uVar1 = uVar1 - 1) {
        iVar3 = *piVar5;
        (&DAT_00af598c)[iVar3 + iVar7] = (&DAT_00af598c)[iVar3 + iVar7] | 0x10000008;
        *(short *)(&DAT_00b4e6fc + (iVar3 + iVar7) * 2) = (short)iVar2;
        piVar5 = piVar5 + 2;
      }
      piVar5 = param_1 + 0xc;
      local_4 = local_4 - 1;
      uVar1 = uVar6;
      param_1 = piVar5;
    } while (local_4 != 0);
  }
  return;
}
