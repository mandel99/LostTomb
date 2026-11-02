#include "restore_types.h"
#define DAT_005e4dd0 (*(undefined4 *)0x005e4dd0u)
#define DAT_005e4df0 (*(undefined4 *)0x005e4df0u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd606 (*(undefined4 *)0x00bcd606u)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd68a (*(undefined4 *)0x00bcd68au)
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
extern void FUN_004b3250();
extern undefined4 FUN_004ea9a0();
extern undefined4 FUN_004eabf0();
extern int FUN_004eb800();
extern int FUN_004ebc90();
extern void FUN_0056233e();

undefined4 FUN_004eb000(int param_1)

{
  short sVar1;
  char cVar2;
  DWORD DVar3;
  int iVar4;
  uint uVar5;
  uint uVar6;
  int iVar7;
  uint uVar8;
  uint uVar9;
  undefined1 local_10 [4];
  undefined1 local_c [4];
  int local_8;
  undefined2 local_4 [2];
  
  iVar7 = param_1 * 0x184;
  cVar2 = FUN_004ea9a0((&DAT_00bcd602)[iVar7]);
  if ((((cVar2 != '\0') && ((&DAT_00bcd606)[iVar7] != '\0')) &&
      (cVar2 = (&DAT_00bcd630)[iVar7], cVar2 != '\f')) && (cVar2 != '\x04')) {
    if ((cVar2 == '\x10') || (cVar2 == '\x11')) {
      return 1;
    }
    DVar3 = timeGetTime();
    FUN_0056233e(DVar3);
    iVar4 = FUN_004ebc90((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                         (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2],3,&DAT_005e4df0,0,local_c,
                         local_10,1,param_1,0x10);
    if (iVar4 != 0) {
      FUN_004b3250(param_1);
      if (((&DAT_00bcd630)[iVar7] != '\x0e') && ((&DAT_00bcd630)[iVar7] != '\x0f')) {
        (&DAT_00bcd630)[iVar7] = 0xf;
        (&DAT_00bcd68a)[iVar7] = 0;
        return 1;
      }
      (&DAT_00bcd630)[iVar7] = 0x10;
      (&DAT_00d3b1c8)[DAT_00d3b1c4] = DAT_00d3b374;
      DAT_00d3b1c4 = DAT_00d3b1c4 + 1;
      if (99 < DAT_00d3b1c4) {
        DAT_00d3b1c4 = 0;
      }
      param_1 = 0x1f;
      do {
        uVar5 = DAT_00d3b384 >> 4;
        uVar8 = DAT_00d3b384 & 1;
        uVar9 = DAT_00d3b380 >> 4;
        uVar6 = DAT_00d3b380 & 1;
        DAT_00d3b384 = DAT_00d3b384 >> 1;
        DAT_00d3b380 = DAT_00d3b380 >> 1;
        if ((uVar5 & 1) != uVar8) {
          DAT_00d3b384 = DAT_00d3b384 | 0x40000000;
        }
        if ((uVar9 & 1) != uVar6) {
          DAT_00d3b380 = DAT_00d3b380 | 0x40000000;
        }
        param_1 = param_1 + -1;
      } while (param_1 != 0);
      sVar1 = *(short *)(&DAT_00bcd61c + iVar4 * 0x184);
      (&DAT_00d3b1c8)[DAT_00d3b1c4] = DAT_00d3b384 & 0x7f;
      DAT_00d3b1c4 = DAT_00d3b1c4 + 1;
      *(ushort *)(&DAT_00bcd61c + iVar7) = sVar1 + ((ushort)DAT_00d3b384 & 3) + -2;
      if (99 < DAT_00d3b1c4) {
        DAT_00d3b1c4 = 0;
      }
      param_1 = 0x1f;
      do {
        uVar5 = DAT_00d3b384 >> 4;
        uVar6 = DAT_00d3b384 & 1;
        uVar8 = DAT_00d3b380 >> 4;
        uVar9 = DAT_00d3b380 & 1;
        DAT_00d3b384 = DAT_00d3b384 >> 1;
        DAT_00d3b380 = DAT_00d3b380 >> 1;
        if ((uVar5 & 1) != uVar6) {
          DAT_00d3b384 = DAT_00d3b384 | 0x40000000;
        }
        if ((uVar8 & 1) != uVar9) {
          DAT_00d3b380 = DAT_00d3b380 | 0x40000000;
        }
        param_1 = param_1 + -1;
      } while (param_1 != 0);
      _DAT_00d3b378 = DAT_00d3b384 & 7;
      _DAT_00d3b364 = DAT_00d3b380 & 0x7fff;
      _DAT_00d3b370 = DAT_00d3b384 & 0x7fff;
      _DAT_00d3b36c = DAT_00d3b380 & 7;
      DAT_00d3b374 = DAT_00d3b384 & 0x7f;
      _DAT_00d3b368 = DAT_00d3b380 & 0x7f;
      *(ushort *)(&DAT_00bcd61e + iVar7) =
           *(short *)(&DAT_00bcd61e + iVar4 * 0x184) + ((ushort)DAT_00d3b384 & 3) + -2;
      return 1;
    }
    if ((&DAT_00bcd630)[iVar7] == '\x0e') {
      iVar4 = FUN_004eb800((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                           (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2],4,&DAT_005e4dd0,0,local_c,
                           local_10,2,param_1);
      if (iVar4 != 0) {
        (&DAT_00d3b1c8)[DAT_00d3b1c4] = DAT_00d3b374;
        DAT_00d3b1c4 = DAT_00d3b1c4 + 1;
        if (99 < DAT_00d3b1c4) {
          DAT_00d3b1c4 = 0;
        }
        local_8 = 0x1f;
        do {
          uVar5 = DAT_00d3b384 >> 4;
          uVar6 = DAT_00d3b384 & 1;
          uVar9 = DAT_00d3b380 >> 4;
          uVar8 = DAT_00d3b380 & 1;
          DAT_00d3b384 = DAT_00d3b384 >> 1;
          DAT_00d3b380 = DAT_00d3b380 >> 1;
          if ((uVar5 & 1) != uVar6) {
            DAT_00d3b384 = DAT_00d3b384 | 0x40000000;
          }
          if ((uVar9 & 1) != uVar8) {
            DAT_00d3b380 = DAT_00d3b380 | 0x40000000;
          }
          local_8 = local_8 + -1;
        } while (local_8 != 0);
        _DAT_00d3b378 = DAT_00d3b384 & 7;
        _DAT_00d3b364 = DAT_00d3b380 & 0x7fff;
        _DAT_00d3b370 = DAT_00d3b384 & 0x7fff;
        DAT_00d3b374 = DAT_00d3b384 & 0x7f;
        _DAT_00d3b368 = DAT_00d3b380 & 0x7f;
        _DAT_00d3b36c = DAT_00d3b380 & 7;
        if (((DAT_00d3b384 & 3) == 0) &&
           (cVar2 = FUN_004eabf0((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                                 (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2],
                                 (int)(short)(&DAT_00bcd60c)[iVar4 * 0xc2],
                                 (int)(short)(&DAT_00bcd60e)[iVar4 * 0xc2],&local_8,local_4),
           cVar2 != '\0')) {
          *(undefined2 *)(&DAT_00bcd61c + iVar7) = (undefined2)local_8;
          *(undefined2 *)(&DAT_00bcd61e + iVar7) = local_4[0];
          (&DAT_00bcd630)[iVar7] = 0x10;
          return 1;
        }
      }
      if ((((&DAT_00bcd630)[iVar7] == '\x0e') || ((&DAT_00bcd630)[iVar7] == '\x0f')) &&
         (cVar2 = (&DAT_00bcd68a)[iVar7], (&DAT_00bcd68a)[iVar7] = cVar2 + 1U,
         0x14 < (byte)(cVar2 + 1U))) {
        FUN_004b3250(param_1);
        (&DAT_00bcd630)[iVar7] = 8;
      }
    }
    else {
      iVar4 = FUN_004ebc90((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                           (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2],3,&DAT_005e4df0,0,local_c,
                           local_10,3,param_1,0xe);
      if (iVar4 != 0) {
        (&DAT_00bcd630)[iVar7] = 0xf;
        (&DAT_00bcd68a)[iVar7] = 0;
        return 1;
      }
      iVar4 = FUN_004eb800((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                           (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2],4,&DAT_005e4dd0,0,local_c,
                           local_10,6,param_1);
      if (iVar4 != 0) {
        (&DAT_00bcd630)[iVar7] = 0xe;
        (&DAT_00bcd68a)[iVar7] = 0;
        return 1;
      }
    }
  }
  return 0;
}
