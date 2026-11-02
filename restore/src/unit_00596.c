#include "restore_types.h"
#define DAT_00775614 (*(undefined4 *)0x00775614u)

undefined4 FUN_00441040(int param_1,int param_2,int param_3,int *param_4,int param_5)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  int *piVar5;
  int iVar6;
  int iVar7;
  int local_8;
  int local_4;
  
  iVar4 = 0;
  if (param_5 == 0) {
    return 0;
  }
  local_8 = 0;
  if (param_3 < 2) {
    local_4 = param_1;
  }
  else {
    local_4 = (param_1 - param_3) + 1;
  }
  iVar1 = param_1;
  if (1 < param_2) {
    iVar1 = (param_1 - param_2) + 1;
  }
  param_1 = 0;
  if (0 < local_4) {
    do {
      iVar6 = 0;
      if (0 < iVar1) {
        iVar7 = 0;
        piVar5 = (int *)(param_5 + iVar4 * 8);
        do {
          iVar4 = 0;
          if (0 < param_3) {
            do {
              iVar3 = 0;
              if (0 < param_2) {
                piVar2 = &DAT_00775614 + iVar7 + iVar4 + param_1;
                do {
                  if (*piVar2 != 0) goto LAB_004410f4;
                  iVar3 = iVar3 + 1;
                  piVar2 = piVar2 + 5;
                } while (iVar3 < param_2);
              }
              iVar4 = iVar4 + 1;
            } while (iVar4 < param_3);
          }
          *piVar5 = iVar6;
          piVar5[1] = param_1;
          local_8 = local_8 + 1;
          piVar5 = piVar5 + 2;
LAB_004410f4: ;
          iVar6 = iVar6 + 1;
          iVar7 = iVar7 + 5;
          iVar4 = local_8;
        } while (iVar6 < iVar1);
      }
      param_1 = param_1 + 1;
    } while (param_1 < local_4);
  }
  *param_4 = iVar4;
  return 1;
}
