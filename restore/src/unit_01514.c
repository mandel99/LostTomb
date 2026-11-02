#include "restore_types.h"
#define DAT_005cd6f8 (*(undefined4 *)0x005cd6f8u)
#define DAT_005cd6fc (*(undefined4 *)0x005cd6fcu)
#define DAT_005cd700 (*(undefined4 *)0x005cd700u)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b350dc (*(undefined4 *)0x00b350dcu)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca360 (*(undefined *)0x00bca360u)
#define DAT_00bca364 (*(undefined4 *)0x00bca364u)
#define DAT_00bcb9f0 (*(undefined4 *)0x00bcb9f0u)
#define DAT_00bcb9f2 (*(undefined4 *)0x00bcb9f2u)
#define DAT_00bcd658 (*(undefined4 *)0x00bcd658u)
#define DAT_00eed760 (*(undefined4 *)0x00eed760u)
#define _DAT_00bcb9a4 (*(undefined4 *)0x00bcb9a4u)
#define _DAT_00bcb9a8 (*(undefined4 *)0x00bcb9a8u)
extern bool FUN_0051a420();

undefined4 FUN_004ba7e0(int param_1,int param_2,int param_3)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  int iVar5;
  uint uVar6;
  int iVar7;
  int *piVar8;
  int iVar9;
  int local_1c;
  int local_18;
  int local_14;
  int local_10;
  
  uVar6 = (uint)(byte)(&DAT_00bcb9f0)[param_1 * 0x90];
  DAT_00bca360 = 0;
  iVar1 = *(short *)(&DAT_00bcb9f2 + param_1 * 0x90) * 0x80;
  iVar3 = *(int *)(&DAT_005cd6fc + iVar1);
  iVar5 = *(int *)(&DAT_005cd6f8 + iVar1);
  if (1 < (byte)(&DAT_00bcb9f0)[param_1 * 0x90]) {
    iVar7 = uVar6 - 1;
    piVar4 = (int *)(&DAT_005cd700 + iVar1);
    piVar8 = &DAT_00bca364;
    do {
      iVar7 = iVar7 + -1;
      *piVar8 = (piVar4[1] * 0xe4 - (iVar5 + iVar3 * 0xe4)) + *piVar4;
      piVar4 = piVar4 + 2;
      piVar8 = piVar8 + 1;
    } while (iVar7 != 0);
  }
  FUN_0051a420(param_2,param_3,0xffffffff,0xffffffff,0,600);
  iVar1 = param_2 - param_3;
  local_10 = 0;
  iVar3 = param_3;
  iVar5 = param_3;
  do {
    param_2 = iVar3 + iVar1;
    param_3 = iVar5 + iVar1;
    if (param_2 < 0) {
      param_2 = 0;
    }
    local_14 = iVar3;
    if (iVar3 < 0) {
      local_14 = 0;
    }
    if (DAT_00bca2d0 <= param_3) {
      param_3 = DAT_00bca2d0 + -1;
    }
    local_18 = iVar5;
    if (DAT_00bca2d4 <= iVar5) {
      local_18 = DAT_00bca2d4 + -1;
    }
    if (local_14 <= local_18) {
      local_1c = DAT_00bca2d8 + local_14 * 0xe4;
      iVar7 = param_2;
      do {
        for (; iVar7 <= param_3; iVar7 = iVar7 + 1) {
          iVar9 = 0;
          if (uVar6 == 0) {
            _DAT_00bcb9a8 = iVar7;
            _DAT_00bcb9a4 = local_14;
            return 1;
          }
          piVar4 = &DAT_00bca360;
          while( true ) {
            iVar2 = local_1c + iVar7 + *piVar4;
            if ((((((&DAT_00af598c)[iVar2] & 4) != 0) || (((&DAT_00af598c)[iVar2] & 0x1200) != 0))
                || ((int)(&DAT_00eed760)[iVar2] < 1)) ||
               ((*(short *)(&DAT_00b350dc + iVar2 * 2) != 0 &&
                ((short)(&DAT_00bcd658)[*(short *)(&DAT_00b350dc + iVar2 * 2) * 0xc2] != param_1))))
            break;
            iVar9 = iVar9 + 1;
            piVar4 = piVar4 + 1;
            if ((int)uVar6 <= iVar9) {
              _DAT_00bcb9a4 = local_14;
              _DAT_00bcb9a8 = iVar7;
              return 1;
            }
          }
        }
        local_1c = local_1c + 0xe4;
        local_14 = local_14 + 1;
        iVar7 = param_2;
      } while (local_14 <= local_18);
    }
    local_10 = local_10 + 1;
    iVar3 = iVar3 + -1;
    iVar5 = iVar5 + 1;
  } while (local_10 < 0xb);
  return 0;
}
