#include "restore_types.h"
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bcb9d0 (*(undefined4 *)0x00bcb9d0u)
#define DAT_00bcd608 (*(undefined4 *)0x00bcd608u)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd610 (*(undefined4 *)0x00bcd610u)
#define DAT_00bcd612 (*(undefined4 *)0x00bcd612u)
#define DAT_00bcd618 (*(undefined4 *)0x00bcd618u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd642 (*(undefined4 *)0x00bcd642u)
#define DAT_00bcd644 (*(undefined4 *)0x00bcd644u)
extern void FUN_004b52f0();

void FUN_004a44f0(int param_1,int param_2,int param_3)

{
  int iVar1;
  int iVar2;
  short *psVar3;
  int local_4;
  
  local_4 = 0;
  psVar3 = (short *)(&DAT_00bcb9d0 + param_1 * 0x90);
  do {
    if (*psVar3 == 0) {
      return;
    }
    iVar2 = (int)*psVar3;
    iVar1 = iVar2 * 0x184;
    if ((&DAT_00bcd630)[iVar1] != '\x04') {
      *(int *)(&DAT_00bcd618 + iVar1) = DAT_00bca2d8 + param_3 * 0xe4 + param_2;
      *(short *)(&DAT_00bcd610 + iVar1) = (short)param_2;
      (&DAT_00bcd60c)[iVar2 * 0xc2] = (short)param_2;
      *(short *)(&DAT_00bcd612 + iVar1) = (short)param_3;
      (&DAT_00bcd60e)[iVar2 * 0xc2] = (short)param_3;
      (&DAT_00bcd630)[iVar1] = (&DAT_00bcd608)[iVar1];
      (&DAT_00bcd642)[iVar2 * 0xc2] = (&DAT_00bcd60c)[iVar2 * 0xc2] * 0xf;
      (&DAT_00bcd644)[iVar2 * 0xc2] = (&DAT_00bcd60e)[iVar2 * 0xc2] * 0xf;
      FUN_004b52f0(iVar2);
    }
    psVar3 = psVar3 + 1;
    local_4 = local_4 + 1;
  } while (local_4 < 0x10);
  return;
}
