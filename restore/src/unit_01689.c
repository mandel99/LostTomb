#include "restore_types.h"
#define DAT_00e34238 (*(undefined *)0x00e34238u)

int FUN_004cc5c0(int param_1,int param_2,int param_3)

{
  int iVar1;
  uint uVar2;
  undefined4 *puVar3;
  
  puVar3 = (undefined4 *)&DAT_00e34238;
  for (iVar1 = 0x40; iVar1 != 0; iVar1 = iVar1 + -1) {
    *puVar3 = 0;
    puVar3 = puVar3 + 1;
  }
  if (param_1 < 0) {
    param_1 = (int32_t)(0u-(uint32_t)param_1);
    (&DAT_00e34238)[param_2] = 0x2d;
  }
  else {
    if (param_3 == 0) goto LAB_004cc5fc;
    (&DAT_00e34238)[param_2] = 0x2b;
  }
  param_2 = param_2 + 1;
LAB_004cc5fc: ;
  if (param_1 < 10) {
    uVar2 = 1;
  }
  else if (param_1 < 100) {
    uVar2 = 2;
  }
  else if (param_1 < 1000) {
    uVar2 = 3;
  }
  else if (param_1 < 10000) {
    uVar2 = 4;
  }
  else if (param_1 < 100000) {
    uVar2 = 5;
  }
  else if (param_1 < 1000000) {
    uVar2 = 6;
  }
  else if (param_1 < 10000000) {
    uVar2 = 7;
  }
  else if (param_1 < 100000000) {
    uVar2 = 8;
  }
  else {
    uVar2 = (999999999 < param_1) - 1 & 9;
  }
  iVar1 = uVar2 - 1;
  if (iVar1 < 0) {
    return uVar2 + param_2;
  }
  do {
    (&DAT_00e34238)[iVar1 + param_2] = (char)(param_1 % 10) + '0';
    param_1 = param_1 / 10;
    iVar1 = iVar1 + -1;
  } while (-1 < iVar1);
  return uVar2 + param_2;
}
