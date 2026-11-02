#include "restore_types.h"
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a34 (*(undefined4 *)0x00799a34u)
#define DAT_00799a38 (*(undefined4 *)0x00799a38u)
#define DAT_00799a6e (*(undefined4 *)0x00799a6eu)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
extern longlong FUN_0056133c();

undefined4 FUN_00546790(int param_1,int param_2,int param_3)

{
  int iVar1;
  int iVar2;
  char cVar3;
  int iVar4;
  int *in_ECX;
  int iVar5;
  int *piVar6;
  int iVar7;
  int iVar8;
  int local_10;
  int local_c;
  int local_8;
  
  iVar2 = param_2;
  iVar7 = 0;
  iVar8 = param_1 - (short)(&DAT_007999ea)[param_1 * 0x84];
  iVar5 = 0;
  local_10 = 0;
  local_c = 0;
  local_8 = 0;
  param_1 = 0;
  if (iVar8 < iVar8 + param_2) {
    iVar4 = (iVar8 + param_2) - iVar8;
    piVar6 = (int *)(&DAT_00799a38 + iVar8 * 0x108);
    do {
      iVar1 = *piVar6;
      piVar6 = piVar6 + 0x42;
      iVar5 = iVar5 + iVar1;
      iVar4 = iVar4 + -1;
      param_1 = iVar5;
    } while (iVar4 != 0);
  }
  if ((100.0 / ((float)(int)param_2 * 4.0)) * (float)(int)param_1 <= 99.9) {
    param_2 = FUN_0056133c();
  }
  else {
    param_2 = 100;
  }
  if (0 < iVar2) {
    piVar6 = (int *)(&DAT_00799a34 + iVar8 * 0x108);
    do {
      cVar3 = (**(code **)(*in_ECX + 0x24))(iVar8);
      if (cVar3 != '\0') {
        if (*piVar6 == 0) break;
        cVar3 = (&DAT_00bcd602)[*piVar6 * 0x184];
        if (cVar3 == 'Q') {
          local_10 = local_10 + 1;
        }
        else if (cVar3 == 'U') {
          local_c = local_c + 1;
        }
        else if (cVar3 == 'l') {
          local_8 = local_8 + 1;
        }
      }
      iVar7 = iVar7 + 1;
      piVar6 = piVar6 + 0x42;
    } while (iVar7 < iVar2);
  }
  if (param_3 != 0) {
    *(int *)(param_3 + 0x14) = param_2;
    *(int *)(param_3 + 4) = local_10;
    *(int *)(param_3 + 8) = local_c;
    *(int *)(param_3 + 0x10) = local_8;
    *(undefined4 *)(param_3 + 0x18) = *(undefined4 *)((int)&DAT_00799a6e + iVar8 * 0x108 + 2);
  }
  return param_2;
}
