#include "restore_types.h"
#define DAT_00799a2e (*(undefined4 *)0x00799a2eu)
#define DAT_00ea60ac (*(undefined4 *)0x00ea60acu)

int FUN_0045e360(int param_1,int param_2,int param_3,int param_4)

{
  int iVar1;
  short sVar2;
  int iVar3;
  int iVar4;
  
  if (param_4 < 1) {
    return 0;
  }
  iVar1 = param_3 + param_2 * 0x84;
  sVar2 = (&DAT_00799a2e)[iVar1];
  iVar4 = (int)sVar2;
  if (iVar4 <= param_4) {
    param_3 = param_1 * 0x1276 + param_3;
    iVar3 = (&DAT_00ea60ac)[param_3];
    (&DAT_00799a2e)[param_2 * 0x84] = (&DAT_00799a2e)[param_2 * 0x84] + sVar2;
    (&DAT_00799a2e)[iVar1] = 0;
    (&DAT_00ea60ac)[param_3] = iVar3 - iVar4;
    return param_4 - iVar4;
  }
  param_3 = param_1 * 0x1276 + param_3;
  (&DAT_00799a2e)[iVar1] = sVar2 - (short)param_4;
  (&DAT_00ea60ac)[param_3] = (&DAT_00ea60ac)[param_3] - param_4;
  (&DAT_00799a2e)[param_2 * 0x84] = (&DAT_00799a2e)[param_2 * 0x84] + (short)param_4;
  return 0;
}
