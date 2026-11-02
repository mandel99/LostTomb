#include "restore_types.h"
#define DAT_00604e68 (*(undefined4 *)0x00604e68u)
#define DAT_01266420 (*(undefined4 *)0x01266420u)
#define DAT_01266424 (*(undefined4 *)0x01266424u)
extern void FUN_0056457b();
extern uint * FUN_00566750();
extern void FUN_00568d11();
extern undefined * FUN_00568d88();

undefined1 * FUN_00564263(undefined8 *param_1,undefined1 *param_2,int param_3,int param_4)

{
  int *piVar1;
  undefined1 *puVar2;
  int iVar3;
  
  piVar1 = DAT_01266420;
  if (DAT_01266424 == '\0') {
    piVar1 = (int *)FUN_00568d88(*param_1);
    FUN_00568d11(param_2 + (uint)(0 < param_3) + (uint)(*piVar1 == 0x2d),param_3 + 1,piVar1);
  }
  else {
    FUN_0056457b(param_2 + (*(undefined4 *)(DAT_01266420) == 0x2d));
  }
  puVar2 = param_2;
  if (*piVar1 == 0x2d) {
    *param_2 = 0x2d;
    puVar2 = param_2 + 1;
  }
  if (0 < param_3) {
    *puVar2 = puVar2[1];
    puVar2[1] = DAT_00604e68;
  }
  puVar2 = (undefined1 *)FUN_00566750();
  if (param_4 != 0) {
    *puVar2 = 0x45;
  }
  if (*(char *)piVar1[3] != '0') {
    iVar3 = piVar1[1] + -1;
    if (iVar3 < 0) {
      iVar3 = -iVar3;
      puVar2[1] = 0x2d;
    }
    if (99 < iVar3) {
      puVar2[2] = puVar2[2] + (char)(iVar3 / 100);
      iVar3 = iVar3 % 100;
    }
    if (9 < iVar3) {
      puVar2[3] = puVar2[3] + (char)(iVar3 / 10);
      iVar3 = iVar3 % 10;
    }
    puVar2[4] = puVar2[4] + (char)iVar3;
  }
  return param_2;
}
