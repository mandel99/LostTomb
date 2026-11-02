#include "restore_types.h"
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00f55bb4 (*(undefined4 *)0x00f55bb4u)
#define DAT_00f56bcc (*(undefined *)0x00f56bccu)
#define DAT_010d6a1a (*(undefined4 *)0x010d6a1au)
#define DAT_010d6a1c (*(undefined4 *)0x010d6a1cu)
#define DAT_0126517c (*(undefined *)0x0126517cu)
extern int FUN_0053e590();

int FUN_00438350(int param_1,int param_2,int param_3,int param_4,int param_5)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int local_40 [16];
  
  param_1 = param_1 - DAT_00f56bcc;
  local_40[7] = 8;
  local_40[0xf] = 8;
  local_40[6] = 0x30;
  local_40[0xe] = 0x30;
  uVar1 = param_1 - 1;
  local_40[5] = 0x12;
  local_40[0xd] = 0x12;
  local_40[0] = 0x4b;
  local_40[1] = 10;
  local_40[2] = 0x4e;
  local_40[3] = 7;
  local_40[4] = 0x3e;
  local_40[8] = 0x4b;
  local_40[9] = 10;
  local_40[10] = 0x4e;
  local_40[0xb] = 7;
  local_40[0xc] = 0x3e;
  iVar2 = 0x278;
  if ((param_5 != 0) && ((&DAT_00bcd630)[param_5 * 0x184] == '\f')) {
    iVar2 = 0x26f;
    param_1 = param_1 + 4;
  }
  iVar2 = (&DAT_00f55bb4)[iVar2 * 2] + param_2 * 4 + -4 + (uVar1 & 3);
  iVar3 = iVar2;
  if (DAT_0126517c <= iVar2) {
    iVar3 = 0;
  }
  iVar4 = iVar2;
  if (DAT_0126517c <= iVar2) {
    iVar4 = 0;
  }
  return FUN_0053e590(iVar2,(local_40[param_1 * 2] - (int)(short)(&DAT_010d6a1a)[iVar3 * 0x20]) + param_3,
               (local_40[param_1 * 2 + 1] - (int)(short)(&DAT_010d6a1c)[iVar4 * 0x20]) + param_4);
}
