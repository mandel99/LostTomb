#include "restore_types.h"
#define DAT_007999d9 (*(undefined4 *)0x007999d9u)
#define DAT_007999db (*(undefined4 *)0x007999dbu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_00799a14 (*(undefined4 *)0x00799a14u)
#define DAT_00799aa4 (*(undefined4 *)0x00799aa4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bcb9cc (*(undefined4 *)0x00bcb9ccu)
#define DAT_00bcb9f0 (*(undefined4 *)0x00bcb9f0u)
#define DAT_00bcba42 (*(undefined4 *)0x00bcba42u)
#define DAT_00bcba9e (*(undefined4 *)0x00bcba9eu)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd620 (*(undefined4 *)0x00bcd620u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd658 (*(undefined4 *)0x00bcd658u)
#define DAT_00bcd65b (*(undefined4 *)0x00bcd65bu)
#define DAT_00bcd66a (*(undefined4 *)0x00bcd66au)
#define DAT_00d3b4e0 (*(undefined *)0x00d3b4e0u)
#define DAT_00d3b4e4 (*(undefined *)0x00d3b4e4u)
#define DAT_00d3b4fc (*(undefined *)0x00d3b4fcu)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)
#define _DAT_00d3b4f8 (*(undefined4 *)0x00d3b4f8u)
extern undefined4 FUN_00480180();
extern int FUN_004b4800();
extern int FUN_004b75c0();
extern void FUN_004b7c80();
extern byte FUN_004c4160();
extern int FUN_004cde80();

undefined4 FUN_004b70c0(int param_1)

{
  short sVar1;
  short sVar2;
  undefined2 uVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  uint uVar7;
  int iVar8;
  int iVar9;
  short *psVar10;
  int iVar11;
  int iVar12;
  int local_c;
  
  iVar5 = param_1;
  iVar9 = 0;
  iVar12 = param_1 * 0x108;
  sVar2 = 0;
  iVar11 = param_1 * 0x84;
  local_c = 10000;
  param_1 = 1;
  psVar10 = &DAT_00bcba9e;
  do {
    if (((((((char)psVar10[-0x23] == '\x01') && ((char)psVar10[-2] != '\0')) && (psVar10[0x1e] < 0))
         && ((sVar1 = *psVar10, 0 < sVar1 && (*(int *)(psVar10 + 0x1f) == 0)))) &&
        ((sVar1 != 3 || (0 < *(short *)(&DAT_00799aa4 + iVar5 * 0x42))))) &&
       ((sVar1 != 2 || (0 < (short)(&DAT_00799a14)[iVar11])))) {
      iVar6 = (int)*(short *)((int)&DAT_007999dc + iVar12 + 2);
      _DAT_00d3b4f8 = (int)*(short *)(&DAT_007999e0 + iVar12);
      iVar8 = (int)*(short *)(&DAT_007999e0 + psVar10[-0x20] * 0x108);
      iVar4 = (int)*(short *)((int)&DAT_007999dc + psVar10[-0x20] * 0x108 + 2);
      if (iVar4 < iVar6) {
        DAT_00d3b4fc = iVar6 - iVar4;
      }
      else if (iVar6 < iVar4) {
        DAT_00d3b4fc = iVar4 - iVar6;
      }
      else {
        DAT_00d3b4fc = 0;
      }
      if (iVar8 < _DAT_00d3b4f8) {
        _DAT_00d3b4f8 = _DAT_00d3b4f8 - iVar8;
      }
      else if (_DAT_00d3b4f8 < iVar8) {
        _DAT_00d3b4f8 = iVar8 - _DAT_00d3b4f8;
      }
      else {
        _DAT_00d3b4f8 = 0;
      }
      iVar4 = DAT_00d3b4fc;
      if (DAT_00d3b4fc < _DAT_00d3b4f8) {
        iVar4 = _DAT_00d3b4f8;
      }
      sVar1 = *psVar10;
      if ((sVar2 < sVar1) || ((sVar1 == sVar2 && (iVar4 < local_c)))) {
        iVar9 = param_1;
        local_c = iVar4;
        sVar2 = sVar1;
      }
    }
    psVar10 = psVar10 + 0x48;
    param_1 = param_1 + 1;
  } while ((int)psVar10 < 0xbcd62e);
  iVar11 = 0;
  if (0 < iVar9) {
    iVar11 = FUN_004b4800((&DAT_007999d9)[iVar12],(int)*(short *)(&DAT_00bcb9cc + iVar9 * 0x90),
                          _DAT_00d3b4e4,_DAT_00d3b4e0,0);
    sVar2 = *(short *)(&DAT_00bcb9cc + iVar9 * 0x90);
    iVar12 = iVar11 * 0x184;
    (&DAT_00bcd658)[iVar11 * 0xc2] = (short)iVar9;
    (&DAT_00bcd65b)[iVar12] = 1;
    if (sVar2 == 0xd) {
      if (0 < *(short *)(&DAT_00799aa4 + iVar5 * 0x42)) {
        *(short *)(&DAT_00799aa4 + iVar5 * 0x42) = *(short *)(&DAT_00799aa4 + iVar5 * 0x42) + -100;
      }
    }
    else if ((sVar2 == 0xc) && (0 < (short)(&DAT_00799a14)[iVar5 * 0x84])) {
      (&DAT_00799a14)[iVar5 * 0x84] = (&DAT_00799a14)[iVar5 * 0x84] + -100;
    }
    iVar11 = FUN_004b75c0(iVar9);
    param_1 = iVar9;
    if (iVar11 == 0) {
      (&DAT_00bcd630)[iVar12] = 0x16;
    }
    else {
      iVar5 = iVar11 * 0x108;
      iVar5 = FUN_00480180((int)*(short *)((int)&DAT_007999dc + iVar5 + 2),
                           (int)*(short *)(&DAT_007999e0 + iVar5),(&DAT_007999db)[iVar5]);
      if (iVar5 == 0) {
        (&DAT_00bcd630)[iVar12] = 9;
      }
      else {
        sVar2 = DAT_00d3b4e0;
        uVar3 = DAT_00d3b4e4;
        iVar5 = (int)DAT_00d3b4e0;
        (&DAT_00bcd66a)[iVar12] = 10;
        (&DAT_00bcd630)[iVar12] = 0xd;
        *(undefined2 *)(&DAT_00bcd61c + iVar12) = uVar3;
        *(short *)(&DAT_00bcd61e + iVar12) = sVar2;
        *(int *)(&DAT_00bcd620 + iVar12) =
             DAT_00bca2d8 + *(short *)(&DAT_00bcd61c + iVar12) + iVar5 * 0xe4;
      }
    }
  }
  FUN_004b7c80();
  iVar11 = FUN_004c4160(iVar11);
  if (iVar9 != 0) {
    param_1 = param_1 * 0x90;
    iVar9 = (int)*(short *)(&DAT_00bcba42 + param_1);
    uVar7 = (uint)(byte)(&DAT_00bcb9f0)[param_1];
    iVar5 = uVar7 - 1;
    if ((iVar9 < 0x19) && (iVar11 == 0x19)) {
      uVar3 = FUN_004cde80(iVar5 * iVar9 + 0x19,uVar7);
      *(undefined2 *)(&DAT_00bcba42 + param_1) = uVar3;
      return 1;
    }
    iVar12 = uVar7 + 1 + iVar5;
    iVar11 = (uVar7 + iVar5) * iVar9 + iVar11;
    iVar5 = FUN_004cde80(iVar11,iVar12);
    if (iVar5 < 100) {
      uVar3 = FUN_004cde80(iVar11,iVar12);
      *(undefined2 *)(&DAT_00bcba42 + param_1) = uVar3;
      return 1;
    }
    *(undefined2 *)(&DAT_00bcba42 + param_1) = 100;
    return 1;
  }
  return 0;
}
