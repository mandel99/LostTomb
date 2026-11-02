#include "restore_types.h"
#define DAT_00604e68 (*(undefined4 *)0x00604e68u)
#define DAT_01266420 (*(undefined4 *)0x01266420u)
#define DAT_01266424 (*(undefined4 *)0x01266424u)
#define DAT_01266428 (*(undefined4 *)0x01266428u)
extern void * FUN_00563240();
extern void FUN_0056457b();
extern void FUN_00568d11();
extern undefined * FUN_00568d88();

undefined1 * FUN_00564367(undefined8 *param_1,undefined1 *param_2,size_t param_3)

{
  int *piVar1;
  int iVar2;
  undefined1 *puVar3;
  
  piVar1 = DAT_01266420;
  if (DAT_01266424 == '\0') {
    piVar1 = (int *)FUN_00568d88(*param_1);
    FUN_00568d11(param_2 + (*piVar1 == 0x2d),piVar1[1] + param_3,piVar1);
  }
  else if (DAT_01266428 == param_3) {
    iVar2 = (*(undefined4 *)(DAT_01266420) == 0x2d) + DAT_01266428;
    param_2[iVar2] = 0x30;
    (param_2 + iVar2)[1] = 0;
  }
  puVar3 = param_2;
  if (*piVar1 == 0x2d) {
    *param_2 = 0x2d;
    puVar3 = param_2 + 1;
  }
  if (piVar1[1] < 1) {
    FUN_0056457b(puVar3,1);
    *puVar3 = 0x30;
    puVar3 = puVar3 + 1;
  }
  else {
    puVar3 = puVar3 + piVar1[1];
  }
  if (0 < (int)param_3) {
    FUN_0056457b(puVar3,1);
    *puVar3 = DAT_00604e68;
    iVar2 = piVar1[1];
    if (iVar2 < 0) {
      if ((DAT_01266424 != '\0') || (-iVar2 <= (int)param_3)) {
        param_3 = -iVar2;
      }
      FUN_0056457b(puVar3 + 1,param_3);
      FUN_00563240(puVar3 + 1,0x30,param_3);
    }
  }
  return param_2;
}
