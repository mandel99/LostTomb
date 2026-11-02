#include "restore_types.h"
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd618 (*(undefined4 *)0x00bcd618u)
#define DAT_00bcd642 (*(undefined4 *)0x00bcd642u)
#define DAT_00bcd644 (*(undefined4 *)0x00bcd644u)
#define DAT_00ed4140 (*(undefined4 *)0x00ed4140u)
extern uint FUN_004467a0();
extern void FUN_004b52f0();
extern void FUN_004b5420();

void FUN_004be4a0(int param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int local_4;
  
  iVar1 = param_1;
  iVar3 = param_1 * 0x184;
  if ((char)(&DAT_00ed4140)[*(int *)(&DAT_00bcd618 + iVar3)] < '\0') {
    local_4 = (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2];
    param_1 = (int)(short)(&DAT_00bcd60c)[param_1 * 0xc2];
    FUN_004467a0(&param_1,&local_4);
    iVar2 = param_1 - (short)(&DAT_00bcd60c)[iVar1 * 0xc2];
    iVar4 = local_4 - (short)(&DAT_00bcd60e)[iVar1 * 0xc2];
    if ((iVar2 != 0) || (iVar4 != 0)) {
      FUN_004b5420(iVar1);
      (&DAT_00bcd642)[iVar1 * 0xc2] = (&DAT_00bcd642)[iVar1 * 0xc2] + (short)iVar2 * 0x10;
      (&DAT_00bcd60c)[iVar1 * 0xc2] = (undefined2)param_1;
      (&DAT_00bcd644)[iVar1 * 0xc2] = (&DAT_00bcd644)[iVar1 * 0xc2] + (short)iVar4 * 0x10;
      (&DAT_00bcd60e)[iVar1 * 0xc2] = (undefined2)local_4;
      *(int *)(&DAT_00bcd618 + iVar3) = *(int *)(&DAT_00bcd618 + iVar3) + iVar2 + iVar4 * 0xe4;
      FUN_004b52f0(iVar1);
    }
  }
  return;
}
