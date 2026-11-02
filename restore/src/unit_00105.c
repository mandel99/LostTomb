#include "restore_types.h"
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a30 (*(undefined4 *)0x00799a30u)
#define DAT_00799a34 (*(undefined4 *)0x00799a34u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
extern longlong FUN_0056133c();

int FUN_0040d940(int *param_1)

{
  int iVar1;
  char cVar2;
  int iVar3;
  int *in_ECX;
  int unaff_EBX;
  int iVar4;
  int unaff_retaddr;
  int *local_10;
  int local_c;
  int local_8;
  
  iVar4 = 0;
  iVar3 = (int)param_1 - (int)(short)(&DAT_007999ea)[(int)param_1 * 0x84];
  local_c = 0;
  local_8 = 0;
  iVar1 = (&DAT_00799a30)[iVar3 * 0x42];
  if (iVar1 == 0x15) {
    param_1 = (int *)0x64;
  }
  else {
    param_1 = (int *)FUN_0056133c();
  }
  cVar2 = (**(code **)(*in_ECX + 0x24))(iVar3);
  local_10 = in_ECX;
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
        local_10 = (int *)0x1;
      }
    }
    else if (cVar2 == 'U') {
      local_c = 1;
    }
  }
  iVar3 = 0x3840;
  if ((3 < iVar1) && (iVar3 = (0x24 - (iVar1 + -3) * *(int *)(unaff_EBX + 4)) * 400, iVar3 < 0)) {
    iVar3 = 0;
  }
  if (param_1 != (int *)0x0) {
    param_1[0xb] = iVar3;
    param_1[8] = (int)local_10;
    param_1[9] = local_c;
    param_1[7] = iVar4;
    *param_1 = iVar1;
    param_1[10] = local_8;
    param_1[0xc] = unaff_retaddr;
    param_1[0xd] = 0x3840;
    return unaff_retaddr;
  }
  return unaff_retaddr;
}
