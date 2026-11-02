#include "restore_types.h"
#define DAT_005e4e0c (*(undefined4 *)0x005e4e0cu)
#define DAT_005e4e30 (*(undefined4 *)0x005e4e30u)
#define DAT_005e4e34 (*(undefined4 *)0x005e4e34u)
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
extern void FUN_004ec990();

uint FUN_004ec4a0(int param_1,uint param_2)

{
  short sVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  int iVar8;
  
  iVar4 = param_1;
  iVar8 = param_1 * 0x184;
  (&DAT_00bcd630)[iVar8] = (char)param_2;
  if (param_2 == 0x12) {
    (&DAT_00d3b1c8)[DAT_00d3b1c4] = DAT_00d3b374;
    DAT_00d3b1c4 = DAT_00d3b1c4 + 1;
    if (99 < DAT_00d3b1c4) {
      DAT_00d3b1c4 = 0;
    }
    iVar4 = 0x1f;
    do {
      uVar2 = DAT_00d3b384 >> 4;
      uVar6 = DAT_00d3b384 & 1;
      uVar7 = DAT_00d3b380 >> 4;
      uVar5 = DAT_00d3b380 & 1;
      DAT_00d3b384 = DAT_00d3b384 >> 1;
      DAT_00d3b380 = DAT_00d3b380 >> 1;
      if ((uVar2 & 1) != uVar6) {
        DAT_00d3b384 = DAT_00d3b384 | 0x40000000;
      }
      if ((uVar7 & 1) != uVar5) {
        DAT_00d3b380 = DAT_00d3b380 | 0x40000000;
      }
      iVar4 = iVar4 + -1;
    } while (iVar4 != 0);
    _DAT_00d3b378 = DAT_00d3b384 & 7;
    _DAT_00d3b364 = DAT_00d3b380 & 0x7fff;
    _DAT_00d3b370 = DAT_00d3b384 & 0x7fff;
    _DAT_00d3b368 = DAT_00d3b380 & 0x7f;
    _DAT_00d3b36c = DAT_00d3b380 & 7;
    DAT_00d3b374 = DAT_00d3b384 & 0x7f;
    uVar2 = _DAT_00d3b370 / 100;
    *(short *)(&DAT_00bcd62e + iVar8) = (short)((ulonglong)_DAT_00d3b370 % 100) + 0xfa;
    *(undefined4 *)(&DAT_00bcd73c + iVar8) = 0xc;
    *(undefined4 *)(&DAT_00bcd740 + iVar8) = 0;
    return uVar2;
  }
  if (param_2 != 0x13) {
    if (param_2 != 0x1a) {
      uVar2 = (FUN_004ec990(param_1), restore_unresolved_return(0x004ec990u));
      *(undefined4 *)(&DAT_00bcd73c + iVar8) = 7;
      *(undefined2 *)(&DAT_00bcd62e + iVar8) = 200;
      *(undefined4 *)(&DAT_00bcd740 + iVar8) = 0;
      return uVar2;
    }
    *(undefined4 *)(&DAT_00bcd73c + iVar8) = 0xc;
    *(undefined2 *)(&DAT_00bcd62e + iVar8) = 0x14;
    *(undefined4 *)(&DAT_00bcd740 + iVar8) = 0;
    return 0;
  }
  *(undefined4 *)(&DAT_00bcd73c + iVar8) = 0xc;
  (&DAT_00d3b1c8)[DAT_00d3b1c4] = DAT_00d3b374;
  DAT_00d3b1c4 = DAT_00d3b1c4 + 1;
  if (99 < DAT_00d3b1c4) {
    DAT_00d3b1c4 = 0;
  }
  iVar3 = 0x1f;
  do {
    uVar2 = DAT_00d3b384 >> 4;
    uVar6 = DAT_00d3b384 & 1;
    uVar7 = DAT_00d3b380 >> 4;
    uVar5 = DAT_00d3b380 & 1;
    DAT_00d3b384 = DAT_00d3b384 >> 1;
    DAT_00d3b380 = DAT_00d3b380 >> 1;
    if ((uVar2 & 1) != uVar6) {
      DAT_00d3b384 = DAT_00d3b384 | 0x40000000;
    }
    if ((uVar7 & 1) != uVar5) {
      DAT_00d3b380 = DAT_00d3b380 | 0x40000000;
    }
    iVar3 = iVar3 + -1;
  } while (iVar3 != 0);
  _DAT_00d3b378 = DAT_00d3b384 & 7;
  _DAT_00d3b364 = DAT_00d3b380 & 0x7fff;
  _DAT_00d3b36c = DAT_00d3b380 & 7;
  _DAT_00d3b370 = DAT_00d3b384 & 0x7fff;
  DAT_00d3b374 = DAT_00d3b384 & 0x7f;
  _DAT_00d3b368 = DAT_00d3b380 & 0x7f;
  uVar2 = DAT_00d3b384 & 7;
  iVar3 = 0;
  do {
    if ((((&DAT_00af598c)[(&DAT_005e4e0c)[uVar2] + *(int *)(&DAT_00bcd618 + iVar8)] & 4) != 0) &&
       (((&DAT_00af598c)[(&DAT_005e4e0c)[uVar2] + *(int *)(&DAT_00bcd618 + iVar8)] & 0x80000) == 0))
    {
      sVar1 = (&DAT_005e4e34)[uVar2 * 4];
      *(undefined2 *)(&DAT_00bcd61c + iVar8) =
           (&DAT_005e4e30)[uVar2 * 4] + (&DAT_00bcd60c)[param_1 * 0xc2];
      uVar2 = (uint)(ushort)(sVar1 + (&DAT_00bcd60e)[param_1 * 0xc2]);
      *(short *)(&DAT_00bcd61e + iVar8) = sVar1 + (&DAT_00bcd60e)[param_1 * 0xc2];
      break;
    }
    uVar2 = uVar2 + 1;
    if (7 < (int)uVar2) {
      uVar2 = 0;
    }
    iVar3 = iVar3 + 1;
  } while (iVar3 < 8);
  if (iVar3 == 8) {
    iVar3 = FUN_004eb800((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                         (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2],0,0,4,&param_1,&param_2,5,
                         param_1);
    if (iVar3 != 0) {
      *(undefined2 *)(&DAT_00bcd61c + iVar8) = (undefined2)param_1;
      *(undefined2 *)(&DAT_00bcd61e + iVar8) = (undefined2)param_2;
      *(undefined4 *)(&DAT_00bcd740 + iVar8) = 0;
      return param_2 & 0xffff;
    }
    uVar2 = (uint)(ushort)(&DAT_00bcd60c)[iVar4 * 0xc2];
    *(undefined2 *)(&DAT_00bcd61c + iVar8) = (&DAT_00bcd60c)[iVar4 * 0xc2];
    *(undefined2 *)(&DAT_00bcd61e + iVar8) = (&DAT_00bcd60e)[iVar4 * 0xc2];
  }
  *(undefined4 *)(&DAT_00bcd740 + iVar8) = 0;
  return uVar2;
}
