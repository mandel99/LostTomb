#include "restore_types.h"
#define DAT_00784dc4 (*(undefined *)0x00784dc4u)
#define DAT_00784dd0 (*(undefined *)0x00784dd0u)
extern uint FUN_004467a0();

undefined4 FUN_004aeab0(undefined4 *param_1,undefined4 *param_2)

{
  int *piVar1;
  int iVar2;
  
  iVar2 = 0;
  piVar1 = &DAT_00784dd0;
  while ((piVar1[-3] == -1 || (*piVar1 == -1))) {
    piVar1 = piVar1 + 1;
    iVar2 = iVar2 + 1;
    if (0x784ddb < (int)piVar1) {
      return 0;
    }
  }
  *param_1 = (&DAT_00784dc4)[iVar2];
  *param_2 = (&DAT_00784dd0)[iVar2];
  FUN_004467a0(param_1,param_2);
  return 1;
}
