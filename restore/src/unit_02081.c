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
extern uint FUN_004ec4a0();

void FUN_004ec7c0(int param_1)

{
  uint uVar1;
  int *piVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  int iVar7;
  int local_14;
  int local_10 [4];
  
  switch((&DAT_00bcd630)[param_1 * 0x184]) {
  case 8: ;
  case 0x19: ;
    local_10[0] = 0x32;
    local_10[1] = 0x32;
    local_10[2] = 0;
    local_10[3] = 0;
    break;
default: ;
    local_10[0] = 0x1d;
    local_10[1] = 0x46;
    local_10[2] = 0;
    local_10[3] = 1;
    break;
  case 0x13: ;
    local_10[0] = 0x4b;
    local_10[1] = 5;
    local_10[2] = 0x14;
    local_10[3] = 0;
    break;
  case 0x1a: ;
    local_10[0] = 0x5a;
    local_10[1] = 1;
    local_10[2] = 9;
    local_10[3] = 0;
  }
  iVar7 = 0;
  (&DAT_00d3b1c8)[DAT_00d3b1c4] = DAT_00d3b374;
  DAT_00d3b1c4 = DAT_00d3b1c4 + 1;
  if (99 < DAT_00d3b1c4) {
    DAT_00d3b1c4 = 0;
  }
  local_14 = 0x1f;
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
    local_14 = local_14 + -1;
  } while (local_14 != 0);
  _DAT_00d3b378 = DAT_00d3b384 & 7;
  _DAT_00d3b364 = DAT_00d3b380 & 0x7fff;
  _DAT_00d3b370 = DAT_00d3b384 & 0x7fff;
  _DAT_00d3b368 = DAT_00d3b380 & 0x7f;
  _DAT_00d3b36c = DAT_00d3b380 & 7;
  DAT_00d3b374 = DAT_00d3b384 & 0x7f;
  iVar3 = 0;
  piVar2 = local_10;
  do {
    iVar7 = iVar7 + *piVar2;
    if ((int)(_DAT_00d3b370 % 100) < iVar7) break;
    iVar3 = iVar3 + 1;
    piVar2 = piVar2 + 1;
  } while (iVar3 < 4);
  FUN_004ec4a0(param_1,*(undefined4 *)(iVar3 * 4 + 0x5e4e70));
  return;
}
