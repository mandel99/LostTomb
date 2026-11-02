#include "restore_types.h"
#define DAT_005e4e80 (*(undefined4 *)0x005e4e80u)
#define DAT_005e4ea0 (*(undefined4 *)0x005e4ea0u)
#define DAT_005e4ea4 (*(undefined4 *)0x005e4ea4u)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd618 (*(undefined4 *)0x00bcd618u)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd62e (*(undefined4 *)0x00bcd62eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd73c (*(undefined4 *)0x00bcd73cu)
#define DAT_00bcd740 (*(undefined4 *)0x00bcd740u)
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
extern int FUN_004eb800();

uint FUN_004eca70(int param_1,uint param_2)

{
  short sVar1;
  ushort uVar2;
  int iVar3;
  uint uVar4;
  int iVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  int iVar9;
  
  iVar3 = param_1;
  iVar9 = param_1 * 0x184;
  (&DAT_00bcd630)[iVar9] = (char)param_2;
  if (param_2 != 0x13) {
    (&DAT_00d3b1c8)[DAT_00d3b1c4] = DAT_00d3b374;
    DAT_00d3b1c4 = DAT_00d3b1c4 + 1;
    if (99 < DAT_00d3b1c4) {
      DAT_00d3b1c4 = 0;
    }
    param_1 = 0x1f;
    do {
      uVar4 = DAT_00d3b384 >> 4;
      uVar7 = DAT_00d3b384 & 1;
      uVar8 = DAT_00d3b380 >> 4;
      uVar6 = DAT_00d3b380 & 1;
      DAT_00d3b384 = DAT_00d3b384 >> 1;
      DAT_00d3b380 = DAT_00d3b380 >> 1;
      if ((uVar4 & 1) != uVar7) {
        DAT_00d3b384 = DAT_00d3b384 | 0x40000000;
      }
      if ((uVar8 & 1) != uVar6) {
        DAT_00d3b380 = DAT_00d3b380 | 0x40000000;
      }
      param_1 = param_1 + -1;
    } while (param_1 != 0);
    _DAT_00d3b378 = DAT_00d3b384 & 7;
    _DAT_00d3b364 = DAT_00d3b380 & 0x7fff;
    _DAT_00d3b370 = DAT_00d3b384 & 0x7fff;
    _DAT_00d3b368 = DAT_00d3b380 & 0x7f;
    _DAT_00d3b36c = DAT_00d3b380 & 7;
    DAT_00d3b374 = DAT_00d3b384 & 0x7f;
    uVar4 = _DAT_00d3b370 / 100;
    *(short *)(&DAT_00bcd62e + iVar9) = (short)((ulonglong)_DAT_00d3b370 % 100) + 0xfa;
    *(undefined4 *)(&DAT_00bcd73c + iVar9) = 0xc;
    *(undefined4 *)(&DAT_00bcd740 + iVar9) = 0;
    return uVar4;
  }
  *(undefined4 *)(&DAT_00bcd73c + iVar9) = 0xb;
  (&DAT_00d3b1c8)[DAT_00d3b1c4] = DAT_00d3b374;
  DAT_00d3b1c4 = DAT_00d3b1c4 + 1;
  if (99 < DAT_00d3b1c4) {
    DAT_00d3b1c4 = 0;
  }
  iVar5 = 0x1f;
  do {
    uVar4 = DAT_00d3b384 >> 4;
    uVar7 = DAT_00d3b384 & 1;
    uVar8 = DAT_00d3b380 >> 4;
    uVar6 = DAT_00d3b380 & 1;
    DAT_00d3b384 = DAT_00d3b384 >> 1;
    DAT_00d3b380 = DAT_00d3b380 >> 1;
    if ((uVar4 & 1) != uVar7) {
      DAT_00d3b384 = DAT_00d3b384 | 0x40000000;
    }
    if ((uVar8 & 1) != uVar6) {
      DAT_00d3b380 = DAT_00d3b380 | 0x40000000;
    }
    iVar5 = iVar5 + -1;
  } while (iVar5 != 0);
  _DAT_00d3b378 = DAT_00d3b384 & 7;
  _DAT_00d3b364 = DAT_00d3b380 & 0x7fff;
  _DAT_00d3b36c = DAT_00d3b380 & 7;
  _DAT_00d3b370 = DAT_00d3b384 & 0x7fff;
  DAT_00d3b374 = DAT_00d3b384 & 0x7f;
  _DAT_00d3b368 = DAT_00d3b380 & 0x7f;
  uVar4 = DAT_00d3b384 & 7;
  iVar5 = 0;
  do {
    if ((((&DAT_00af598c)[(&DAT_005e4e80)[uVar4] + *(int *)(&DAT_00bcd618 + iVar9)] & 4) != 0) &&
       (((&DAT_00af598c)[(&DAT_005e4e80)[uVar4] + *(int *)(&DAT_00bcd618 + iVar9)] & 0x80000) == 0))
    {
      sVar1 = (&DAT_005e4ea4)[uVar4 * 4];
      *(undefined2 *)(&DAT_00bcd61c + iVar9) =
           (&DAT_005e4ea0)[uVar4 * 4] + (&DAT_00bcd60c)[param_1 * 0xc2];
      uVar4 = (uint)(ushort)(sVar1 + (&DAT_00bcd60e)[param_1 * 0xc2]);
      *(short *)(&DAT_00bcd61e + iVar9) = sVar1 + (&DAT_00bcd60e)[param_1 * 0xc2];
      break;
    }
    uVar4 = uVar4 + 1;
    if (7 < (int)uVar4) {
      uVar4 = 0;
    }
    iVar5 = iVar5 + 1;
  } while (iVar5 < 8);
  if (iVar5 == 8) {
    iVar5 = FUN_004eb800((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                         (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2],0,0,4,&param_1,&param_2,5,
                         param_1);
    if (iVar5 == 0) {
      uVar2 = (&DAT_00bcd60c)[iVar3 * 0xc2];
      *(ushort *)(&DAT_00bcd61c + iVar9) = uVar2;
      *(undefined2 *)(&DAT_00bcd61e + iVar9) = (&DAT_00bcd60e)[iVar3 * 0xc2];
      *(undefined4 *)(&DAT_00bcd740 + iVar9) = 0;
      return (uint)uVar2;
    }
    uVar4 = param_2 & 0xffff;
    *(undefined2 *)(&DAT_00bcd61c + iVar9) = (undefined2)param_1;
    *(undefined2 *)(&DAT_00bcd61e + iVar9) = (undefined2)param_2;
  }
  *(undefined4 *)(&DAT_00bcd740 + iVar9) = 0;
  return uVar4;
}
