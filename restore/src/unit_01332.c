#include "restore_types.h"
#define DAT_00799a2e (*(undefined4 *)0x00799a2eu)
#define DAT_0089b6d8 (*(undefined4 *)0x0089b6d8u)
#define DAT_00bcd68b (*(undefined4 *)0x00bcd68bu)
#define DAT_00bcd694 (*(undefined4 *)0x00bcd694u)
extern void FUN_00456760();
extern undefined4 FUN_00495c10();
extern int FUN_00495da0();
extern void FUN_00495de0();

void FUN_00495e10(int param_1,int param_2,undefined4 param_3)

{
  int iVar1;
  int *piVar2;
  int *piVar3;
  int iVar4;
  int local_10 [4];
  
  if ((byte)(&DAT_00bcd68b)[param_1 * 0x184] < 3) {
    piVar3 = local_10;
    piVar2 = (int *)(&DAT_00bcd694 + param_1 * 0x184);
    iVar4 = 4;
    do {
      iVar1 = *piVar2;
      *piVar3 = 0;
      if (iVar1 != -1) {
        *piVar3 = (400 - (short)(&DAT_00799a2e)[iVar1 + param_2 * 0x84]) - piVar2[8];
      }
      piVar2 = piVar2 + 1;
      piVar3 = piVar3 + 1;
      iVar4 = iVar4 + -1;
    } while (iVar4 != 0);
    iVar4 = FUN_00495da0(local_10);
    FUN_00456760(param_1,param_3);
    if (iVar4 != -1) {
      do {
        if (0 < (int)(&DAT_0089b6d8)[*(int *)(&DAT_00bcd694 + (param_1 * 0x61 + iVar4) * 4)]) {
          FUN_00495c10(param_1,(&DAT_0089b6d8)
                               [*(int *)(&DAT_00bcd694 + (param_1 * 0x61 + iVar4) * 4)]);
          return;
        }
        local_10[iVar4] = 0;
        iVar4 = FUN_00495da0(local_10);
      } while (iVar4 != -1);
    }
  }
  FUN_00495de0(param_1);
  return;
}
