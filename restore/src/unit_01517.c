#include "restore_types.h"
#define DAT_005cd978 (*(undefined4 *)0x005cd978u)
#define DAT_005cd97c (*(undefined4 *)0x005cd97cu)
#define DAT_005cdb78 (*(undefined4 *)0x005cdb78u)
#define DAT_005cdb7c (*(undefined4 *)0x005cdb7cu)
#define DAT_005e4c20 (*(undefined4 *)0x005e4c20u)
#define DAT_005e4c28 (*(undefined4 *)0x005e4c28u)
#define DAT_005e4c4c (*(undefined4 *)0x005e4c4cu)
#define DAT_005e4cb0 (*(undefined4 *)0x005e4cb0u)
#define DAT_005e4cb8 (*(undefined4 *)0x005e4cb8u)
#define DAT_005e4cdc (*(undefined4 *)0x005e4cdcu)
#define DAT_00605a3c (*(undefined *)0x00605a3cu)
#define DAT_00784b64 (*(undefined *)0x00784b64u)
#define DAT_00784b94 (*(undefined *)0x00784b94u)
#define DAT_00784cc0 (*(undefined *)0x00784cc0u)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bcb9f1 (*(undefined4 *)0x00bcb9f1u)
#define DAT_00bcba02 (*(undefined4 *)0x00bcba02u)
#define DAT_00bcba04 (*(undefined4 *)0x00bcba04u)
#define DAT_00bcba16 (*(undefined4 *)0x00bcba16u)
#define DAT_00bcba31 (*(undefined4 *)0x00bcba31u)
#define DAT_00bcba3c (*(undefined4 *)0x00bcba3cu)
#define DAT_00bcd62e (*(undefined4 *)0x00bcd62eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd658 (*(undefined4 *)0x00bcd658u)
#define DAT_00bcd65a (*(undefined4 *)0x00bcd65au)
#define DAT_00d3b1c4 (*(undefined *)0x00d3b1c4u)
#define DAT_00d3b1c8 (*(undefined4 *)0x00d3b1c8u)
#define DAT_00d3b374 (*(undefined *)0x00d3b374u)
#define DAT_00d3b380 (*(undefined *)0x00d3b380u)
#define DAT_00d3b384 (*(undefined *)0x00d3b384u)
#define _DAT_00784cb0 (*(undefined4 *)0x00784cb0u)
#define _DAT_00d3b364 (*(undefined4 *)0x00d3b364u)
#define _DAT_00d3b368 (*(undefined4 *)0x00d3b368u)
#define _DAT_00d3b36c (*(undefined4 *)0x00d3b36cu)
#define _DAT_00d3b370 (*(undefined4 *)0x00d3b370u)
#define _DAT_00d3b378 (*(undefined4 *)0x00d3b378u)
extern int FUN_004b4800();
extern int FUN_004b6ff0();
extern uint FUN_004ec410();

void FUN_004bb3f0(void)

{
  short sVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  char cVar4;
  int iVar5;
  int iVar6;
  uint uVar7;
  int iVar8;
  uint uVar9;
  int iVar10;
  uint uVar11;
  uint uVar12;
  uint uVar13;
  uint uVar14;
  int iVar15;
  int local_20;
  int *local_1c;
  undefined2 local_18;
  int local_14;
  int local_c;
  
  iVar10 = (_DAT_00784cb0 & 0xff) * 0x30;
  uVar2 = *(undefined4 *)(&DAT_005e4c20 + iVar10);
  iVar10 = *(int *)(&DAT_005e4c28 + iVar10);
  iVar8 = ((int)DAT_00784b94 + (_DAT_00784cb0 & 0xff) * 2) * 0x30;
  uVar3 = *(undefined4 *)(&DAT_005e4cb0 + iVar8);
  iVar8 = *(int *)(&DAT_005e4cb8 + iVar8);
  if (DAT_00605a3c == 0) {
    local_1c = (int *)&DAT_00784b64;
    uVar7 = _DAT_00784cb0;
    do {
      if ((0 < (short)*local_1c) &&
         (iVar5 = FUN_004b6ff0(uVar3,*(undefined4 *)
                                      (&DAT_005e4cdc +
                                      ((int)DAT_00784b94 + (uVar7 & 0xff) * 2) * 0x30),0,
                               (int)(short)*local_1c,(int)*(short *)((int)local_1c + 8)),
         uVar7 = _DAT_00784cb0, 0 < iVar5)) {
        sVar1 = *(short *)((int)local_1c + 8);
        local_20 = 0;
        iVar6 = iVar5 * 0x90;
        (&DAT_00bcba02)[iVar5 * 0x48] = (short)*local_1c;
        (&DAT_00bcba04)[iVar5 * 0x48] = sVar1;
        (&DAT_00bcba3c)[iVar6] = 0;
        (&DAT_00bcba31)[iVar6] = 1;
        *(undefined2 *)(&DAT_00bcba16 + iVar6) = 0x18;
        (&DAT_00bcb9f1)[iVar6] = (char)iVar8;
        uVar7 = _DAT_00784cb0;
        uVar9 = DAT_00d3b380;
        iVar6 = DAT_00d3b1c4;
        uVar14 = DAT_00d3b384;
        if (0 < iVar8) {
          do {
            (&DAT_00d3b1c8)[iVar6] = DAT_00d3b374;
            DAT_00d3b1c4 = iVar6 + 1;
            if (99 < DAT_00d3b1c4) {
              DAT_00d3b1c4 = 0;
            }
            iVar6 = 0x1f;
            do {
              uVar7 = uVar14 >> 4;
              uVar11 = uVar14 & 1;
              uVar12 = uVar9 >> 4;
              uVar13 = uVar9 & 1;
              uVar14 = uVar14 >> 1;
              uVar9 = uVar9 >> 1;
              if ((uVar7 & 1) != uVar11) {
                uVar14 = uVar14 | 0x40000000;
              }
              if ((uVar12 & 1) != uVar13) {
                uVar9 = uVar9 | 0x40000000;
              }
              iVar6 = iVar6 + -1;
            } while (iVar6 != 0);
            _DAT_00d3b370 = uVar14 & 0x7fff;
            DAT_00d3b374 = uVar14 & 0x7f;
            _DAT_00d3b378 = uVar14 & 7;
            _DAT_00d3b364 = uVar9 & 0x7fff;
            _DAT_00d3b368 = uVar9 & 0x7f;
            _DAT_00d3b36c = uVar9 & 7;
            iVar15 = 0;
            iVar6 = local_20;
            DAT_00d3b380 = uVar9;
            DAT_00d3b384 = uVar14;
            do {
              cVar4 = FUN_004ec410((&DAT_00af598c)
                                   [(int)(short)*local_1c +
                                    DAT_00bca2d8 +
                                    (&DAT_005cd978)[iVar6 * 2] +
                                    ((&DAT_005cd97c)[iVar6 * 2] + (int)*(short *)((int)local_1c + 8)
                                    ) * 0xe4],0);
              if (cVar4 != '\0') break;
              iVar6 = iVar6 + 1;
              if (0xf < iVar6) {
                iVar6 = 0;
              }
              iVar15 = iVar15 + 1;
            } while (iVar15 < 0x10);
            (&DAT_00d3b1c8)[DAT_00d3b1c4] = DAT_00d3b374;
            DAT_00d3b1c4 = DAT_00d3b1c4 + 1;
            if (99 < DAT_00d3b1c4) {
              DAT_00d3b1c4 = 0;
            }
            local_14 = 0x1f;
            do {
              uVar7 = DAT_00d3b384 >> 4;
              uVar14 = DAT_00d3b384 & 1;
              uVar11 = DAT_00d3b380 >> 4;
              uVar9 = DAT_00d3b380 & 1;
              DAT_00d3b384 = DAT_00d3b384 >> 1;
              DAT_00d3b380 = DAT_00d3b380 >> 1;
              if ((uVar7 & 1) != uVar14) {
                DAT_00d3b384 = DAT_00d3b384 | 0x40000000;
              }
              if ((uVar11 & 1) != uVar9) {
                DAT_00d3b380 = DAT_00d3b380 | 0x40000000;
              }
              local_14 = local_14 + -1;
            } while (local_14 != 0);
            _DAT_00d3b378 = DAT_00d3b384 & 7;
            _DAT_00d3b370 = DAT_00d3b384 & 0x7fff;
            _DAT_00d3b364 = DAT_00d3b380 & 0x7fff;
            _DAT_00d3b36c = DAT_00d3b380 & 7;
            _DAT_00d3b368 = DAT_00d3b380 & 0x7f;
            DAT_00d3b374 = DAT_00d3b384 & 0x7f;
            iVar6 = FUN_004b4800(1,uVar3,(&DAT_005cd978)[iVar6 * 2] + (int)(short)*local_1c,
                                 (&DAT_005cd97c)[iVar6 * 2] + (int)*(short *)((int)local_1c + 8),
                                 DAT_00d3b384 & 7);
            local_18 = (undefined2)iVar5;
            iVar15 = iVar6 * 0x184;
            (&DAT_00d3b1c8)[DAT_00d3b1c4] = DAT_00d3b374;
            DAT_00d3b1c4 = DAT_00d3b1c4 + 1;
            (&DAT_00bcd630)[iVar15] = 0x18;
            (&DAT_00bcd658)[iVar6 * 0xc2] = local_18;
            *(undefined2 *)(&DAT_00bcd62e + iVar15) = 0;
            if (99 < DAT_00d3b1c4) {
              DAT_00d3b1c4 = 0;
            }
            iVar6 = DAT_00d3b1c4;
            local_14 = 0x1f;
            uVar9 = DAT_00d3b380;
            uVar14 = DAT_00d3b384;
            do {
              uVar7 = uVar14 >> 4;
              uVar11 = uVar14 & 1;
              uVar12 = uVar9 >> 4;
              uVar13 = uVar9 & 1;
              uVar14 = uVar14 >> 1;
              uVar9 = uVar9 >> 1;
              if ((uVar7 & 1) != uVar11) {
                uVar14 = uVar14 | 0x40000000;
              }
              if ((uVar12 & 1) != uVar13) {
                uVar9 = uVar9 | 0x40000000;
              }
              local_14 = local_14 + -1;
            } while (local_14 != 0);
            DAT_00d3b374 = uVar14 & 0x7f;
            _DAT_00d3b370 = uVar14 & 0x7fff;
            _DAT_00d3b378 = uVar14 & 7;
            _DAT_00d3b364 = uVar9 & 0x7fff;
            _DAT_00d3b368 = uVar9 & 0x7f;
            _DAT_00d3b36c = uVar9 & 7;
            DAT_00d3b380 = uVar9;
            DAT_00d3b384 = uVar14;
            *(short *)(&DAT_00bcd62e + iVar15) = (short)(_DAT_00d3b370 % 0x19 << 2);
            (&DAT_00bcd65a)[iVar15] = (char)local_20;
            local_20 = local_20 + 1;
            uVar7 = _DAT_00784cb0;
          } while (local_20 < iVar8);
        }
      }
      local_1c = (int *)((int)local_1c + 2);
    } while ((int)local_1c < 0x784b6c);
    local_1c = &DAT_00784cc0;
    do {
      if ((0 < *local_1c) &&
         (iVar8 = FUN_004b6ff0(uVar2,*(undefined4 *)(&DAT_005e4c4c + (uVar7 & 0xff) * 0x30),0,
                               *local_1c,local_1c[4]), uVar7 = _DAT_00784cb0, 0 < iVar8)) {
        iVar5 = local_1c[4];
        local_20 = 0;
        iVar6 = iVar8 * 0x90;
        (&DAT_00bcba02)[iVar8 * 0x48] = (short)*local_1c;
        (&DAT_00bcba04)[iVar8 * 0x48] = (short)iVar5;
        (&DAT_00bcba3c)[iVar6] = 0;
        (&DAT_00bcba31)[iVar6] = 2;
        *(undefined2 *)(&DAT_00bcba16 + iVar6) = 0x18;
        (&DAT_00bcb9f1)[iVar6] = (char)iVar10;
        uVar7 = _DAT_00784cb0;
        uVar9 = DAT_00d3b380;
        iVar5 = DAT_00d3b1c4;
        uVar14 = DAT_00d3b384;
        if (0 < iVar10) {
          do {
            (&DAT_00d3b1c8)[iVar5] = DAT_00d3b374;
            DAT_00d3b1c4 = iVar5 + 1;
            if (99 < DAT_00d3b1c4) {
              DAT_00d3b1c4 = 0;
            }
            iVar5 = 0x1f;
            do {
              uVar7 = uVar14 >> 4;
              uVar11 = uVar14 & 1;
              uVar12 = uVar9 >> 4;
              uVar13 = uVar9 & 1;
              uVar14 = uVar14 >> 1;
              uVar9 = uVar9 >> 1;
              if ((uVar7 & 1) != uVar11) {
                uVar14 = uVar14 | 0x40000000;
              }
              if ((uVar12 & 1) != uVar13) {
                uVar9 = uVar9 | 0x40000000;
              }
              iVar5 = iVar5 + -1;
            } while (iVar5 != 0);
            _DAT_00d3b370 = uVar14 & 0x7fff;
            DAT_00d3b374 = uVar14 & 0x7f;
            _DAT_00d3b378 = uVar14 & 7;
            _DAT_00d3b364 = uVar9 & 0x7fff;
            _DAT_00d3b368 = uVar9 & 0x7f;
            _DAT_00d3b36c = uVar9 & 7;
            iVar6 = 0;
            iVar5 = local_20;
            DAT_00d3b380 = uVar9;
            DAT_00d3b384 = uVar14;
            do {
              cVar4 = FUN_004ec410((&DAT_00af598c)
                                   [(&DAT_005cdb78)[iVar5 * 2] +
                                    ((&DAT_005cdb7c)[iVar5 * 2] + local_1c[4]) * 0xe4 + *local_1c +
                                    DAT_00bca2d8],1);
              if (cVar4 != '\0') break;
              iVar5 = iVar5 + 1;
              if (0xf < iVar5) {
                iVar5 = 0;
              }
              iVar6 = iVar6 + 1;
            } while (iVar6 < 0x10);
            (&DAT_00d3b1c8)[DAT_00d3b1c4] = DAT_00d3b374;
            DAT_00d3b1c4 = DAT_00d3b1c4 + 1;
            if (99 < DAT_00d3b1c4) {
              DAT_00d3b1c4 = 0;
            }
            local_c = 0x1f;
            do {
              uVar7 = DAT_00d3b384 >> 4;
              uVar14 = DAT_00d3b384 & 1;
              uVar11 = DAT_00d3b380 >> 4;
              uVar9 = DAT_00d3b380 & 1;
              DAT_00d3b384 = DAT_00d3b384 >> 1;
              DAT_00d3b380 = DAT_00d3b380 >> 1;
              if ((uVar7 & 1) != uVar14) {
                DAT_00d3b384 = DAT_00d3b384 | 0x40000000;
              }
              if ((uVar11 & 1) != uVar9) {
                DAT_00d3b380 = DAT_00d3b380 | 0x40000000;
              }
              local_c = local_c + -1;
            } while (local_c != 0);
            _DAT_00d3b378 = DAT_00d3b384 & 7;
            _DAT_00d3b364 = DAT_00d3b380 & 0x7fff;
            _DAT_00d3b370 = DAT_00d3b384 & 0x7fff;
            _DAT_00d3b36c = DAT_00d3b380 & 7;
            _DAT_00d3b368 = DAT_00d3b380 & 0x7f;
            DAT_00d3b374 = DAT_00d3b384 & 0x7f;
            iVar5 = FUN_004b4800(1,uVar2,(&DAT_005cdb78)[iVar5 * 2] + *local_1c,
                                 (&DAT_005cdb7c)[iVar5 * 2] + local_1c[4],DAT_00d3b384 & 7);
            local_18 = (undefined2)iVar8;
            iVar6 = iVar5 * 0x184;
            (&DAT_00d3b1c8)[DAT_00d3b1c4] = DAT_00d3b374;
            DAT_00d3b1c4 = DAT_00d3b1c4 + 1;
            (&DAT_00bcd630)[iVar6] = 0x18;
            (&DAT_00bcd658)[iVar5 * 0xc2] = local_18;
            if (99 < DAT_00d3b1c4) {
              DAT_00d3b1c4 = 0;
            }
            iVar5 = DAT_00d3b1c4;
            local_c = 0x1f;
            uVar9 = DAT_00d3b380;
            uVar14 = DAT_00d3b384;
            do {
              uVar7 = uVar14 >> 4;
              uVar11 = uVar14 & 1;
              uVar12 = uVar9 >> 4;
              uVar13 = uVar9 & 1;
              uVar14 = uVar14 >> 1;
              uVar9 = uVar9 >> 1;
              if ((uVar7 & 1) != uVar11) {
                uVar14 = uVar14 | 0x40000000;
              }
              if ((uVar12 & 1) != uVar13) {
                uVar9 = uVar9 | 0x40000000;
              }
              local_c = local_c + -1;
            } while (local_c != 0);
            DAT_00d3b374 = uVar14 & 0x7f;
            _DAT_00d3b370 = uVar14 & 0x7fff;
            _DAT_00d3b378 = uVar14 & 7;
            _DAT_00d3b364 = uVar9 & 0x7fff;
            _DAT_00d3b368 = uVar9 & 0x7f;
            _DAT_00d3b36c = uVar9 & 7;
            DAT_00d3b380 = uVar9;
            DAT_00d3b384 = uVar14;
            *(short *)(&DAT_00bcd62e + iVar6) = (short)((ulonglong)_DAT_00d3b370 % 0x19);
            (&DAT_00bcd65a)[iVar6] = (char)local_20;
            local_20 = local_20 + 1;
            uVar7 = _DAT_00784cb0;
          } while (local_20 < iVar10);
        }
      }
      local_1c = local_1c + 1;
    } while ((int)local_1c < 0x784cd0);
  }
  return;
}
