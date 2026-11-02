#include "restore_types.h"
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
extern int FUN_0040c180();
extern uint FUN_004467a0();

void FUN_004a4850(int *param_1,int *param_2,int param_3,int param_4,int param_5,int param_6,
                 int param_7,int param_8,int param_9)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar2 = param_9;
  iVar1 = FUN_0040c180((int)(short)(&DAT_00bcd60c)[param_9 * 0xc2],
                       (int)(short)(&DAT_00bcd60e)[param_9 * 0xc2],
                       param_3 + (short)(&DAT_00bcd60c)[param_9 * 0xc2],
                       param_4 + (short)(&DAT_00bcd60e)[param_9 * 0xc2],&param_9,&param_4);
  iVar3 = 0;
  if (-1 < iVar1) {
    *param_1 = param_9;
    *param_2 = param_4;
    iVar3 = iVar1;
  }
  iVar1 = FUN_0040c180((int)(short)(&DAT_00bcd60c)[iVar2 * 0xc2],
                       (int)(short)(&DAT_00bcd60e)[iVar2 * 0xc2],
                       param_5 + (short)(&DAT_00bcd60c)[iVar2 * 0xc2],
                       param_6 + (short)(&DAT_00bcd60e)[iVar2 * 0xc2],&param_9,&param_4);
  if (iVar3 <= iVar1) {
    *param_1 = param_9;
    *param_2 = param_4;
    iVar3 = iVar1;
  }
  iVar2 = FUN_0040c180((int)(short)(&DAT_00bcd60c)[iVar2 * 0xc2],
                       (int)(short)(&DAT_00bcd60e)[iVar2 * 0xc2],
                       param_7 + (short)(&DAT_00bcd60c)[iVar2 * 0xc2],
                       param_8 + (short)(&DAT_00bcd60e)[iVar2 * 0xc2],&param_9,&param_4);
  if (iVar3 <= iVar2) {
    *param_1 = param_9;
    *param_2 = param_4;
  }
  FUN_004467a0(param_1,param_2);
  return;
}
