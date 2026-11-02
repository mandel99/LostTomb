#include "restore_types.h"
#define DAT_005f80fc (*(undefined4 *)0x005f80fcu)
#define DAT_005f811c (*(undefined4 *)0x005f811cu)
#define DAT_005f8120 (*(undefined4 *)0x005f8120u)
#define DAT_005f815c (*(undefined4 *)0x005f815cu)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00bc971c (*(undefined *)0x00bc971cu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00eae000 (*(undefined *)0x00eae000u)
#define DAT_00eed760 (*(undefined4 *)0x00eed760u)
extern void FUN_0047b310();
extern void FUN_0047b960();
extern undefined4 FUN_0051c390();
extern undefined4 FUN_0051c630();
extern undefined4 FUN_0051cba0();

undefined4
FUN_0051c930(undefined4 param_1,undefined4 param_2,undefined4 param_3,int param_4,int param_5,
            int param_6)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  int *piVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int local_8;
  
  DAT_00bc971c = 0;
  local_8 = 1;
  iVar7 = DAT_00bca2d8 + param_5 * 0xe4 + param_4;
  do {
    iVar1 = (&DAT_00eed760)[iVar7];
    if ((iVar1 < 1) && ((&DAT_007999e8)[*(short *)(&DAT_00b4e6fc + iVar7 * 2) * 0x84] != 0x8a)) {
      DAT_00eae000 = iVar7;
      return 0;
    }
    DAT_00eae000 = iVar7;
    if (param_6 == 0) {
      if ((((&DAT_00af598c)[iVar7] & 0x100) != 0) && (iVar7 = FUN_0051c390(iVar7), iVar7 == 0)) {
        return 0;
      }
      FUN_0047b310(param_4,param_5,1);
    }
    else if (param_6 == 2) {
      DAT_00bc971c = DAT_00bc971c + 1;
    }
    else if (param_6 == 3) {
      if (((*(byte *)(&DAT_00af598c + iVar7) & 0x40) != 0) &&
         (iVar7 = FUN_0051c630(iVar7), iVar7 == 0)) {
        return 0;
      }
      FUN_0047b960(param_4,param_5,1,1);
    }
    uVar3 = FUN_0051cba0(param_4,param_5,param_2,param_3);
    if ((uVar3 & 0xff) == 8) {
      return 1;
    }
    iVar6 = 0;
    piVar4 = (int *)(&DAT_005f815c + (uVar3 & 0xff) * 0x10);
    while( true ) {
      iVar2 = *piVar4;
      iVar7 = (&DAT_005f80fc)[iVar2] + DAT_00eae000;
      if (((0 < (int)(&DAT_00eed760)[iVar7]) && ((int)(&DAT_00eed760)[iVar7] < iVar1)) &&
         ((((uint)(&DAT_00af598c)[iVar7] >> 8 & 1) == 0 || (iVar5 = FUN_0051c390(iVar7), iVar5 != 0)
          ))) break;
      iVar6 = iVar6 + 1;
      piVar4 = piVar4 + 1;
      if (3 < iVar6) {
        return 0;
      }
    }
    param_4 = param_4 + *(int *)(&DAT_005f811c + iVar2 * 8);
    param_5 = param_5 + *(int *)(&DAT_005f8120 + iVar2 * 8);
    if (3 < iVar6) {
      DAT_00eae000 = iVar7;
      return 0;
    }
    local_8 = local_8 + 1;
  } while (local_8 < 400);
  DAT_00eae000 = iVar7;
  return 0;
}
