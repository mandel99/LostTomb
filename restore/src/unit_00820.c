#include "restore_types.h"
#define DAT_00799a3e (*(undefined4 *)0x00799a3eu)
#define DAT_00799a40 (*(undefined4 *)0x00799a40u)
#define DAT_00799a42 (*(undefined4 *)0x00799a42u)
#define DAT_00799a44 (*(undefined4 *)0x00799a44u)
extern uint FUN_00455ca0();

void FUN_00456070(int param_1,int param_2,undefined4 *param_3)

{
  char cVar1;
  uint uVar2;
  
  *param_3 = 0xffffffff;
  param_3[1] = 0xffffffff;
  param_3[2] = 0xffffffff;
  uVar2 = 0;
  param_3[3] = 0xffffffff;
  if (((&DAT_00799a3e)[param_1 * 0x84] == 0) && (*(int *)(param_2 + 0x20) != 0)) {
    *param_3 = 8;
    uVar2 = 1;
  }
  if (((&DAT_00799a40)[param_1 * 0x84] == 0) && (*(int *)(param_2 + 0x24) != 0)) {
    param_3[uVar2] = 9;
    uVar2 = uVar2 + 1;
  }
  if (((&DAT_00799a42)[param_1 * 0x84] == 0) && (*(int *)(param_2 + 0x28) != 0)) {
    param_3[uVar2] = 10;
    uVar2 = uVar2 + 1;
  }
  if (((&DAT_00799a44)[param_1 * 0x84] == 0) && (*(int *)(param_2 + 0x2c) != 0)) {
    param_3[uVar2] = 0xb;
    uVar2 = uVar2 + 1;
    if (3 < uVar2) {
      return;
    }
  }
  if (((*(int *)(param_2 + 0x20) != 0) && ((short)(&DAT_00799a3e)[param_1 * 0x84] < 100)) &&
     (cVar1 = FUN_00455ca0(8,param_1,param_3), cVar1 == '\0')) {
    param_3[uVar2] = 8;
    uVar2 = uVar2 + 1;
    if (3 < uVar2) {
      return;
    }
  }
  if (((*(int *)(param_2 + 0x24) != 0) && ((short)(&DAT_00799a40)[param_1 * 0x84] < 100)) &&
     (cVar1 = FUN_00455ca0(9,param_1,param_3), cVar1 == '\0')) {
    param_3[uVar2] = 9;
    uVar2 = uVar2 + 1;
    if (3 < uVar2) {
      return;
    }
  }
  if (((*(int *)(param_2 + 0x28) != 0) && ((short)(&DAT_00799a42)[param_1 * 0x84] < 100)) &&
     (cVar1 = FUN_00455ca0(10,param_1,param_3), cVar1 == '\0')) {
    param_3[uVar2] = 10;
    uVar2 = uVar2 + 1;
    if (3 < uVar2) {
      return;
    }
  }
  if (((*(int *)(param_2 + 0x2c) != 0) && ((short)(&DAT_00799a44)[param_1 * 0x84] < 100)) &&
     (cVar1 = FUN_00455ca0(0xb,param_1,param_3), cVar1 == '\0')) {
    param_3[uVar2] = 0xb;
  }
  return;
}
