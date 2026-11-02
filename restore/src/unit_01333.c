#include "restore_types.h"
#define DAT_00799a2e (*(undefined4 *)0x00799a2eu)
#define DAT_0089b6d8 (*(undefined4 *)0x0089b6d8u)
#define DAT_00bcd68b (*(undefined4 *)0x00bcd68bu)
#define DAT_00bcd694 (*(undefined4 *)0x00bcd694u)
#define DAT_00bcd6b4 (*(undefined4 *)0x00bcd6b4u)
extern void FUN_00456760();
extern undefined4 FUN_00495c10();
extern int FUN_00495da0();
extern void FUN_00495de0();

void FUN_00495f00(int param_1,int param_2,undefined4 param_3)

{
  short sVar1;
  int iVar2;
  int *piVar3;
  int *piVar4;
  int iVar5;
  int iVar6;
  int local_10 [4];
  
  if ((byte)(&DAT_00bcd68b)[param_1 * 0x184] < 3) {
    piVar4 = local_10;
    piVar3 = (int *)(&DAT_00bcd6b4 + param_1 * 0x184);
    iVar6 = 4;
    do {
      iVar2 = piVar3[-8];
      *piVar4 = 0;
      if (iVar2 != -1) {
        if (iVar2 == 0) {
          iVar5 = 800;
          sVar1 = (&DAT_00799a2e)[param_2 * 0x84];
        }
        else {
          iVar5 = 600;
          sVar1 = (&DAT_00799a2e)[iVar2 + param_2 * 0x84];
        }
        *piVar4 = (iVar5 - sVar1) - *piVar3;
      }
      piVar3 = piVar3 + 1;
      piVar4 = piVar4 + 1;
      iVar6 = iVar6 + -1;
    } while (iVar6 != 0);
    iVar6 = FUN_00495da0(local_10);
    FUN_00456760(param_1,param_3);
    if (iVar6 != -1) {
      do {
        if (0 < (int)(&DAT_0089b6d8)[*(int *)(&DAT_00bcd694 + (param_1 * 0x61 + iVar6) * 4)]) {
          FUN_00495c10(param_1,(&DAT_0089b6d8)
                               [*(int *)(&DAT_00bcd694 + (param_1 * 0x61 + iVar6) * 4)]);
          return;
        }
        local_10[iVar6] = 0;
        iVar6 = FUN_00495da0(local_10);
      } while (iVar6 != -1);
    }
  }
  FUN_00495de0(param_1);
  return;
}
