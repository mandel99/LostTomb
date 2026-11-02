#include "restore_types.h"
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00d3b1c4 (*(undefined *)0x00d3b1c4u)
#define DAT_00d3b1c8 (*(undefined4 *)0x00d3b1c8u)
#define DAT_00d3b374 (*(undefined *)0x00d3b374u)
#define DAT_00d3b380 (*(undefined *)0x00d3b380u)
#define DAT_00d3b384 (*(undefined *)0x00d3b384u)
#define _DAT_00d3b364 (*(undefined4 *)0x00d3b364u)
#define _DAT_00d3b368 (*(undefined4 *)0x00d3b368u)
#define _DAT_00d3b36c (*(undefined4 *)0x00d3b36cu)
#define _DAT_00d3b370 (*(undefined4 *)0x00d3b370u)
#define _DAT_00d3b378 (*(undefined4 *)0x00d3b378u)
extern uint FUN_004eca70();

void FUN_004ecd40(int param_1)

{
  uint uVar1;
  int iVar2;
  int *piVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  int iVar7;
  int local_c;
  int local_8 [2];
  
  if ((&DAT_00bcd630)[param_1 * 0x184] == '\b') {
    local_8[0] = 0x32;
    local_8[1] = 0x32;
  }
  else if ((&DAT_00bcd630)[param_1 * 0x184] == '\x13') {
    local_8[0] = 0x5a;
    local_8[1] = 10;
  }
  else {
    local_8[0] = 0x19;
    local_8[1] = 0x4b;
  }
  iVar7 = 0;
  (&DAT_00d3b1c8)[DAT_00d3b1c4] = DAT_00d3b374;
  DAT_00d3b1c4 = DAT_00d3b1c4 + 1;
  if (99 < DAT_00d3b1c4) {
    DAT_00d3b1c4 = 0;
  }
  local_c = 0x1f;
  do {
    uVar1 = DAT_00d3b384 >> 4;
    uVar5 = DAT_00d3b384 & 1;
    uVar6 = DAT_00d3b380 >> 4;
    uVar4 = DAT_00d3b380 & 1;
    DAT_00d3b384 = DAT_00d3b384 >> 1;
    DAT_00d3b380 = DAT_00d3b380 >> 1;
    if ((uVar1 & 1) != uVar5) {
      DAT_00d3b384 = DAT_00d3b384 | 0x40000000;
    }
    if ((uVar6 & 1) != uVar4) {
      DAT_00d3b380 = DAT_00d3b380 | 0x40000000;
    }
    local_c = local_c + -1;
  } while (local_c != 0);
  _DAT_00d3b378 = DAT_00d3b384 & 7;
  _DAT_00d3b364 = DAT_00d3b380 & 0x7fff;
  _DAT_00d3b370 = DAT_00d3b384 & 0x7fff;
  _DAT_00d3b368 = DAT_00d3b380 & 0x7f;
  _DAT_00d3b36c = DAT_00d3b380 & 7;
  DAT_00d3b374 = DAT_00d3b384 & 0x7f;
  iVar2 = 0;
  piVar3 = local_8;
  do {
    iVar7 = iVar7 + *piVar3;
    if ((int)(_DAT_00d3b370 % 100) < iVar7) break;
    iVar2 = iVar2 + 1;
    piVar3 = piVar3 + 1;
  } while (iVar2 < 2);
  FUN_004eca70(param_1,*(undefined4 *)(iVar2 * 4 + 0x5e4ee0));
  return;
}
