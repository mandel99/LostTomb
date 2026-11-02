#include "restore_types.h"
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a30 (*(undefined4 *)0x00799a30u)
#define DAT_00799a34 (*(undefined4 *)0x00799a34u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)

int FUN_00417050(int *param_1)

{
  int iVar1;
  char cVar2;
  int iVar3;
  int *in_ECX;
  int unaff_EBX;
  int iVar4;
  int unaff_retaddr;
  int local_c;
  int local_8;
  
  iVar4 = 0;
  iVar3 = (int)param_1 - (int)(short)(&DAT_007999ea)[(int)param_1 * 0x84];
  local_c = 0;
  local_8 = 0;
  iVar1 = (&DAT_00799a30)[iVar3 * 0x42];
  if (iVar1 == 0x13) {
    param_1 = (int *)0x64;
  }
  else {
    param_1 = (int *)(iVar1 * 5 + 5);
  }
  cVar2 = (**(code **)(*in_ECX + 0x24))(iVar3);
  if (cVar2 != '\0') {
    if (*(int *)(&DAT_00799a34 + iVar3 * 0x108) == 0) {
      return 0;
    }
    iVar3 = *(int *)(&DAT_00799a34 + iVar3 * 0x108) * 0x184;
    cVar2 = (&DAT_00bcd602)[iVar3];
    if (cVar2 == 'O') {
      local_8 = 1;
    }
    else if (cVar2 == 'Q') {
      iVar4 = 1;
      if ((&DAT_00bcd630)[iVar3] == '\n') {
        unaff_EBX = 1;
      }
    }
    else if (cVar2 == 'U') {
      local_c = 1;
    }
  }
  iVar3 = (0x26 - in_ECX[1] * iVar1) * 400;
  if (iVar3 < 0) {
    iVar3 = 0;
  }
  if (param_1 != (int *)0x0) {
    *param_1 = iVar1;
    param_1[2] = unaff_EBX;
    param_1[3] = local_c;
    param_1[5] = iVar3;
    param_1[1] = iVar4;
    param_1[4] = local_8;
    param_1[6] = unaff_retaddr;
    param_1[7] = 0x3b60;
    return unaff_retaddr;
  }
  return unaff_retaddr;
}
