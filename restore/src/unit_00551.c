#include "restore_types.h"
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd606 (*(undefined4 *)0x00bcd606u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00f55bb4 (*(undefined4 *)0x00f55bb4u)
#define DAT_00f56c0c (*(undefined *)0x00f56c0cu)
#define DAT_010d6a1a (*(undefined4 *)0x010d6a1au)
#define DAT_010d6a1c (*(undefined4 *)0x010d6a1cu)
#define DAT_0126517c (*(undefined *)0x0126517cu)
extern int FUN_0053e590();

void FUN_00438210(int param_1,int param_2,int param_3,int param_4,uint param_5)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  int local_20 [8];
  
  if (param_5 == 0) {
    return;
  }
  uVar1 = param_5 & 0xfff;
  if ((param_5 & 0xf000) == 0x1000) {
    if (uVar1 == 0x19) {
      iVar2 = 0x277;
      goto LAB_00438252;
    }
    if (uVar1 == 0x4d) {
LAB_00438342: ;
      iVar2 = 0x275;
      goto LAB_00438252;
    }
  }
  else if ((param_5 & 0xf000) == 0x2000) {
    iVar2 = uVar1 * 0x184;
    if ((&DAT_00bcd606)[iVar2] != '\x01') {
      return;
    }
    if ((&DAT_00bcd630)[iVar2] == '\x04') {
      return;
    }
    if ((&DAT_00bcd602)[iVar2] == 'M') goto LAB_00438342;
  }
  iVar2 = 0x26e;
LAB_00438252: ;
  local_20[2] = 0x55;
  local_20[4] = 0x55;
  param_2 = (&DAT_00f55bb4)[iVar2 * 2] + -1 + param_2;
  local_20[0] = 0x6e;
  local_20[1] = 0x14;
  local_20[3] = 0x14;
  local_20[5] = 0x14;
  local_20[6] = 100;
  local_20[7] = 10;
  iVar2 = param_2;
  if (DAT_0126517c <= param_2) {
    iVar2 = 0;
  }
  iVar3 = param_2;
  if (DAT_0126517c <= param_2) {
    iVar3 = 0;
  }
  FUN_0053e590(param_2,(local_20[(param_1 - DAT_00f56c0c) * 2] -
                       (int)(short)(&DAT_010d6a1a)[iVar2 * 0x20]) + param_3,
               (local_20[(param_1 - DAT_00f56c0c) * 2 + 1] -
               (int)(short)(&DAT_010d6a1c)[iVar3 * 0x20]) + param_4);
  return;
}
