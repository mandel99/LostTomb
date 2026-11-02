#include "restore_types.h"
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd614 (*(undefined4 *)0x00bcd614u)
#define DAT_00bcd618 (*(undefined4 *)0x00bcd618u)
#define DAT_00bcd638 (*(undefined4 *)0x00bcd638u)
#define DAT_00bcd642 (*(undefined4 *)0x00bcd642u)
#define DAT_00bcd644 (*(undefined4 *)0x00bcd644u)
#define DAT_00bcd64a (*(undefined4 *)0x00bcd64au)
#define DAT_00bcd64c (*(undefined4 *)0x00bcd64cu)
#define DAT_00bcd64e (*(undefined4 *)0x00bcd64eu)
#define DAT_00bcd650 (*(undefined4 *)0x00bcd650u)
extern void FUN_004b4180();
extern void FUN_004b43a0();
extern void FUN_004b43e0();
extern void FUN_004b52f0();
extern void FUN_004b5420();

undefined4 FUN_004b3ff0(int param_1,int param_2)

{
  short sVar1;
  int iVar2;
  int iVar3;
  undefined4 local_4;
  
  iVar2 = 0;
  local_4 = 0;
  FUN_004b5420(param_1);
  if (0 < param_2) {
    iVar3 = param_1 * 0x184;
    do {
      if ((&DAT_00bcd614)[iVar3] == '\0') {
        (&DAT_00bcd614)[iVar3] = 0;
      }
      else {
        (&DAT_00bcd614)[iVar3] = (&DAT_00bcd614)[iVar3] + -1;
      }
      if ((int)(short)(&DAT_00bcd64c)[param_1 * 0xc2] + (int)(short)(&DAT_00bcd64a)[param_1 * 0xc2]
          < 1) {
        local_4 = 1;
        break;
      }
      FUN_004b4180(param_1);
      if ((&DAT_00bcd650)[iVar3] == '\x02') {
        FUN_004b43a0();
        if (-1 < (short)(&DAT_00bcd64e)[param_1 * 0xc2]) {
          (&DAT_00bcd64a)[param_1 * 0xc2] = (&DAT_00bcd64a)[param_1 * 0xc2] + -1;
          FUN_004b43e0(param_1);
        }
      }
      else {
        FUN_004b43e0(param_1);
        if (-1 < (short)(&DAT_00bcd64e)[param_1 * 0xc2]) {
          (&DAT_00bcd64c)[param_1 * 0xc2] = (&DAT_00bcd64c)[param_1 * 0xc2] + -1;
          FUN_004b43a0(param_1);
        }
      }
      iVar2 = iVar2 + 1;
    } while (iVar2 < param_2);
  }
  iVar3 = param_1 * 0x184;
  sVar1 = (&DAT_00bcd642)[param_1 * 0xc2];
  (&DAT_00bcd60c)[param_1 * 0xc2] =
       (sVar1 / 0xf + (sVar1 >> 0xf)) - (short)((longlong)(int)sVar1 * 0x88888889 >> 0x3f);
  iVar2 = DAT_00bca2d8;
  sVar1 = (&DAT_00bcd644)[param_1 * 0xc2];
  sVar1 = (sVar1 / 0xf + (sVar1 >> 0xf)) - (short)((longlong)(int)sVar1 * 0x88888889 >> 0x3f);
  (&DAT_00bcd60e)[param_1 * 0xc2] = sVar1;
  iVar2 = iVar2 + (short)(&DAT_00bcd60c)[param_1 * 0xc2] + sVar1 * 0xe4;
  *(int *)(&DAT_00bcd618 + iVar3) = iVar2;
  if ((*(byte *)(&DAT_00af598c + iVar2) & 8) == 0) {
    if ((&DAT_00bcd638)[iVar3] != '\0') {
      (&DAT_00bcd638)[iVar3] = (&DAT_00bcd638)[iVar3] + -1;
    }
    FUN_004b52f0(param_1);
    return local_4;
  }
  (&DAT_00bcd638)[iVar3] = 8;
  FUN_004b52f0(param_1);
  return local_4;
}
