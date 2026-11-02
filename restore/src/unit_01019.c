#include "restore_types.h"
#define DAT_00784b6c (*(undefined *)0x00784b6cu)
#define DAT_00784cd0 (*(undefined *)0x00784cd0u)

uint FUN_00470900(int param_1,int param_2,int param_3,int param_4)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  
  iVar3 = param_1;
  if (param_3 <= param_1) {
    iVar3 = param_3;
  }
  if (param_1 <= param_3) {
    param_1 = param_3;
  }
  iVar2 = param_2;
  if (param_4 <= param_2) {
    iVar2 = param_4;
  }
  if (param_2 <= param_4) {
    param_2 = param_4;
  }
  piVar1 = (int *)&DAT_00784b6c;
  do {
    if ((((iVar3 <= (short)piVar1[-2]) && ((short)piVar1[-2] <= param_1)) &&
        (iVar2 <= (short)*piVar1)) && ((short)*piVar1 <= param_2)) goto LAB_00470983;
    piVar1 = (int *)((int)piVar1 + 2);
  } while ((int)piVar1 < 0x784b74);
  piVar1 = &DAT_00784cd0;
  while (((piVar1[-4] < iVar3 || (param_1 < piVar1[-4])) ||
         ((*piVar1 < iVar2 || (param_2 < *piVar1))))) {
    piVar1 = piVar1 + 1;
    if (0x784cdf < (int)piVar1) {
      return (uint)piVar1 & 0xffffff00;
    }
  }
LAB_00470983: ;
  return CONCAT31((int3)((uint)piVar1 >> 8),1);
}
