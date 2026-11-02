#include "restore_types.h"
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
extern int FUN_004a59c0();
extern int FUN_004a5a20();

int FUN_004a5560(int param_1,int param_2,int param_3)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int local_10;
  int local_c;
  int local_4;
  
  local_10 = 0;
  iVar5 = 500000;
  iVar1 = (int)(short)(&DAT_00bcd60c)[param_1 * 0xc2] +
          DAT_00bca2d8 + (short)(&DAT_00bcd60e)[param_1 * 0xc2] * 0xe4 + (param_2 / 2) * -0xe5;
  if (0 < param_2) {
    local_4 = param_2;
    do {
      local_c = param_2;
      iVar3 = iVar1;
      do {
        if (param_3 == 0) {
          iVar2 = FUN_004a5a20(iVar3,param_1);
        }
        else {
          iVar2 = FUN_004a59c0();
        }
        if ((iVar2 != 0) &&
           (iVar4 = ((int)(short)(&DAT_00bcd60c)[iVar2 * 0xc2] -
                    (int)(short)(&DAT_00bcd60c)[param_1 * 0xc2]) *
                    ((int)(short)(&DAT_00bcd60c)[iVar2 * 0xc2] -
                    (int)(short)(&DAT_00bcd60c)[param_1 * 0xc2]) +
                    ((int)(short)(&DAT_00bcd60e)[iVar2 * 0xc2] -
                    (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2]) *
                    ((int)(short)(&DAT_00bcd60e)[iVar2 * 0xc2] -
                    (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2]), iVar4 < iVar5)) {
          iVar5 = iVar4;
          local_10 = iVar2;
        }
        iVar3 = iVar3 + 1;
        local_c = local_c + -1;
      } while (local_c != 0);
      iVar1 = iVar1 + 0xe4;
      local_4 = local_4 + -1;
    } while (local_4 != 0);
  }
  return local_10;
}
