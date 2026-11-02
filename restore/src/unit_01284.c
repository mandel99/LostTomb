#include "restore_types.h"
#define DAT_0070e448 (*(undefined *)0x0070e448u)
#define DAT_0070e44c (*(undefined *)0x0070e44cu)
#define DAT_0070e45c (*(undefined *)0x0070e45cu)
#define DAT_00f56b24 (*(undefined *)0x00f56b24u)
#define DAT_010d6a0e (*(undefined4 *)0x010d6a0eu)
#define DAT_0126517c (*(undefined *)0x0126517cu)
extern void FUN_0047bf50();
extern bool FUN_004890f0();
extern int FUN_0053e590();
#define _param_6 restore_param_6

void FUN_00488690(undefined4 param_1,int param_2,int param_3,int param_4,int param_5,unsigned int restore_param_6)

{
  char cVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int local_64 [25];
  
  iVar3 = DAT_00f56b24;
  if (RESTORE_SLICE1(restore_param_6,0) == '\0') {
    cVar1 = FUN_004890f0(param_1);
    FUN_0047bf50(param_1,param_2,param_3,3,3,(uint)(cVar1 == '\0') + iVar3,8);
    return;
  }
  param_5 = param_5 / 10;
  local_64[2] = 2;
  local_64[6] = 1;
  local_64[0xb] = 2;
  local_64[0xd] = 1;
  local_64[0xf] = 1;
  local_64[0x10] = 1;
  local_64[0x14] = 2;
  local_64[0x17] = 2;
  local_64[0] = -2;
  local_64[1] = 0;
  local_64[3] = 0;
  local_64[4] = 0;
  local_64[5] = 0xffffffff;
  local_64[7] = 0xe6;
  local_64[8] = 4;
  local_64[9] = 0;
  local_64[10] = 0;
  local_64[0xc] = 0x1ca;
  local_64[0xe] = 0;
  local_64[0x11] = 0x1c9;
  local_64[0x12] = 3;
  local_64[0x13] = 0;
  local_64[0x15] = 0;
  local_64[0x16] = 0x1c8;
  local_64[0x18] = 0;
  piVar2 = local_64 + 4;
  iVar3 = 5;
  do {
    iVar5 = param_4 + 5;
    if (param_5 < 4) {
      iVar5 = param_4 + param_5;
    }
    param_5 = param_5 + -4;
    *piVar2 = iVar5;
    if (param_5 < 0) {
      param_5 = 0;
    }
    piVar2 = piVar2 + 5;
    iVar3 = iVar3 + -1;
  } while (iVar3 != 0);
  piVar2 = local_64 + 3;
  _param_6 = 5;
  do {
    iVar3 = *piVar2;
    iVar5 = local_64[iVar3 * 5 + 4];
    iVar4 = iVar5;
    if (DAT_0126517c <= iVar5) {
      iVar4 = 0;
    }
    FUN_0053e590(iVar5,local_64[iVar3 * 5] * DAT_0070e45c + param_2,
                 (DAT_0070e448 * 2 - (int)(short)(&DAT_010d6a0e)[iVar4 * 0x20]) +
                 local_64[iVar3 * 5 + 1] * DAT_0070e44c + param_3);
    piVar2 = piVar2 + 5;
    _param_6 = _param_6 + -1;
  } while (_param_6 != 0);
  return;
}
