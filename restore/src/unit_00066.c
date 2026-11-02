#include "restore_types.h"
#define DAT_005d2a18 (*(undefined *)0x005d2a18u)
#define DAT_005d2a1c (*(undefined *)0x005d2a1cu)
#define DAT_0077590c (*(undefined *)0x0077590cu)
#define DAT_00775910 (*(undefined *)0x00775910u)
#define DAT_0077f75c (*(undefined4 *)0x0077f75cu)
#define DAT_00784cb4 (*(undefined *)0x00784cb4u)
#define DAT_00d3b4fc (*(undefined *)0x00d3b4fcu)
#define DAT_00e38e30 (*(undefined *)0x00e38e30u)
#define DAT_00e38e34 (*(undefined *)0x00e38e34u)
#define DAT_00f535d4 (*(undefined4 *)0x00f535d4u)
#define DAT_00f53fdc (*(undefined *)0x00f53fdcu)
#define _DAT_00d3b4f8 (*(undefined4 *)0x00d3b4f8u)

void FUN_00407a10(int *param_1,int *param_2)

{
  short sVar1;
  short sVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int *piVar7;
  int local_10;
  int local_8;
  short *local_4;
  
  iVar4 = (DAT_00e38e34 - DAT_005d2a18) + DAT_00775910;
  iVar3 = (DAT_00e38e30 - DAT_005d2a1c) + DAT_0077590c;
  local_10 = 0x1e;
  *param_1 = 0;
  local_8 = 0;
  local_4 = &DAT_0077f75c;
  do {
    if (*(char *)((int)local_4 + -0x13) == '\0') break;
    if (DAT_00784cb4 == 0) {
      sVar1 = local_4[-8];
      sVar2 = local_4[-7];
    }
    else {
      sVar1 = *local_4;
      sVar2 = local_4[1];
    }
    iVar6 = (int)sVar2;
    iVar5 = (int)sVar1;
    if ((((iVar5 + -8 <= iVar4) && (iVar4 < local_4[-6] + 8 + iVar5)) && (iVar6 + -8 <= iVar3)) &&
       (iVar3 < local_4[-5] + 8 + iVar6)) {
      iVar5 = (int)local_4[-6] / 2 + iVar5;
      iVar6 = (int)local_4[-5] / 2 + iVar6;
      if (iVar5 < iVar4) {
        DAT_00d3b4fc = iVar4 - iVar5;
      }
      else if (iVar4 < iVar5) {
        DAT_00d3b4fc = iVar5 - iVar4;
      }
      else {
        DAT_00d3b4fc = 0;
      }
      if (iVar6 < iVar3) {
        _DAT_00d3b4f8 = iVar3 - iVar6;
      }
      else if (iVar3 < iVar6) {
        _DAT_00d3b4f8 = iVar6 - iVar3;
      }
      else {
        _DAT_00d3b4f8 = 0;
      }
      iVar6 = _DAT_00d3b4f8;
      if (_DAT_00d3b4f8 <= DAT_00d3b4fc) {
        iVar6 = DAT_00d3b4fc;
      }
      if (iVar6 < local_10) {
        *param_1 = local_8 + 1;
        local_10 = iVar6;
      }
    }
    local_4 = local_4 + 0x31;
    local_8 = local_8 + 1;
  } while ((int)local_4 < 0x7843ec);
  iVar3 = 0;
  *param_2 = -1;
  if (0 < DAT_00f53fdc) {
    piVar7 = &DAT_00f535d4;
    iVar4 = DAT_00e38e34;
    iVar6 = DAT_00e38e30;
    do {
      iVar5 = piVar7[2] / 2 + *piVar7;
      _DAT_00d3b4f8 = piVar7[3] / 2 + piVar7[1];
      if (iVar5 < iVar4) {
        DAT_00d3b4fc = iVar4 - iVar5;
      }
      else if (iVar4 < iVar5) {
        DAT_00d3b4fc = iVar5 - iVar4;
      }
      else {
        DAT_00d3b4fc = 0;
      }
      if (_DAT_00d3b4f8 < iVar6) {
        _DAT_00d3b4f8 = iVar6 - _DAT_00d3b4f8;
      }
      else if (iVar6 < _DAT_00d3b4f8) {
        _DAT_00d3b4f8 = _DAT_00d3b4f8 - iVar6;
      }
      else {
        _DAT_00d3b4f8 = 0;
      }
      iVar5 = _DAT_00d3b4f8;
      if (_DAT_00d3b4f8 <= DAT_00d3b4fc) {
        iVar5 = DAT_00d3b4fc;
      }
      if (iVar5 < local_10) {
        *param_1 = 0;
        *param_2 = iVar3;
        iVar4 = DAT_00e38e34;
        iVar6 = DAT_00e38e30;
        local_10 = iVar5;
      }
      iVar3 = iVar3 + 1;
      piVar7 = piVar7 + 5;
    } while (iVar3 < DAT_00f53fdc);
  }
  return;
}
