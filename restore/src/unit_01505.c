#include "restore_types.h"
#define DAT_005cd978 (*(undefined4 *)0x005cd978u)
#define DAT_005cd97c (*(undefined4 *)0x005cd97cu)
#define DAT_005cdb78 (*(undefined4 *)0x005cdb78u)
#define DAT_005cdb7c (*(undefined4 *)0x005cdb7cu)
#define DAT_005e4c20 (*(undefined4 *)0x005e4c20u)
#define DAT_005e4c28 (*(undefined4 *)0x005e4c28u)
#define DAT_005e4c30 (*(undefined4 *)0x005e4c30u)
#define DAT_005e4c34 (*(undefined4 *)0x005e4c34u)
#define DAT_005e4c38 (*(undefined4 *)0x005e4c38u)
#define DAT_005e4cb0 (*(undefined4 *)0x005e4cb0u)
#define DAT_005e4cb8 (*(undefined4 *)0x005e4cb8u)
#define DAT_005e4cbc (*(undefined4 *)0x005e4cbcu)
#define DAT_005e4cc0 (*(undefined4 *)0x005e4cc0u)
#define DAT_005e4cc4 (*(undefined4 *)0x005e4cc4u)
#define DAT_005e4cc8 (*(undefined4 *)0x005e4cc8u)
#define DAT_00784b94 (*(undefined *)0x00784b94u)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bcb9a4 (*(undefined *)0x00bcb9a4u)
#define DAT_00bcb9a8 (*(undefined *)0x00bcb9a8u)
#define DAT_00bcb9c8 (*(undefined4 *)0x00bcb9c8u)
#define DAT_00bcb9cc (*(undefined4 *)0x00bcb9ccu)
#define DAT_00bcb9d0 (*(undefined4 *)0x00bcb9d0u)
#define DAT_00bcb9f0 (*(undefined4 *)0x00bcb9f0u)
#define DAT_00bcb9f1 (*(undefined4 *)0x00bcb9f1u)
#define DAT_00bcb9fe (*(undefined4 *)0x00bcb9feu)
#define DAT_00bcba00 (*(undefined4 *)0x00bcba00u)
#define DAT_00bcba02 (*(undefined4 *)0x00bcba02u)
#define DAT_00bcba04 (*(undefined4 *)0x00bcba04u)
#define DAT_00bcba0a (*(undefined4 *)0x00bcba0au)
#define DAT_00bcba31 (*(undefined4 *)0x00bcba31u)
#define DAT_00bcba3c (*(undefined4 *)0x00bcba3cu)
#define DAT_00bcd62e (*(undefined4 *)0x00bcd62eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd658 (*(undefined4 *)0x00bcd658u)
#define DAT_00d3b1c4 (*(undefined *)0x00d3b1c4u)
#define DAT_00d3b1c8 (*(undefined4 *)0x00d3b1c8u)
#define DAT_00d3b374 (*(undefined *)0x00d3b374u)
#define DAT_00d3b380 (*(undefined *)0x00d3b380u)
#define DAT_00d3b384 (*(undefined *)0x00d3b384u)
#define DAT_00ea5ce8 (*(undefined4 *)0x00ea5ce8u)
#define DAT_00ea5f9c (*(undefined4 *)0x00ea5f9cu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define _DAT_00784cb0 (*(undefined4 *)0x00784cb0u)
#define _DAT_00d3b364 (*(undefined4 *)0x00d3b364u)
#define _DAT_00d3b368 (*(undefined4 *)0x00d3b368u)
#define _DAT_00d3b36c (*(undefined4 *)0x00d3b36cu)
#define _DAT_00d3b370 (*(undefined4 *)0x00d3b370u)
#define _DAT_00d3b378 (*(undefined4 *)0x00d3b378u)
extern int FUN_004b4800();
extern undefined4 FUN_004ba7e0();
extern undefined4 FUN_004c5120();
extern bool FUN_004c52d0();
extern int FUN_004eaa10();
extern int FUN_004eaa60();
extern undefined4 FUN_004eb4c0();
extern uint FUN_004ec410();
extern uint FUN_00562348();

void FUN_004b92e0(void)

{
  short sVar1;
  bool bVar2;
  undefined2 uVar3;
  char cVar4;
  int iVar5;
  int iVar6;
  uint uVar7;
  byte bVar8;
  uint uVar9;
  short *psVar10;
  uint uVar11;
  uint uVar12;
  int iVar13;
  uint uVar14;
  int local_28;
  int local_24;
  int local_20;
  int local_1c;
  int local_18;
  int local_14;
  undefined4 local_10;
  undefined4 local_c;
  undefined4 local_8;
  undefined4 local_4;
  
  bVar2 = false;
  iVar6 = (_DAT_00784cb0 & 0xff) * 0x30;
  local_8 = *(undefined4 *)(&DAT_005e4c20 + iVar6);
  local_4 = *(undefined4 *)(&DAT_005e4c28 + iVar6);
  iVar6 = ((int)DAT_00784b94 + (_DAT_00784cb0 & 0xff) * 2) * 0x30;
  local_10 = *(undefined4 *)(&DAT_005e4cb0 + iVar6);
  local_c = *(undefined4 *)(&DAT_005e4cb8 + iVar6);
  if ((((&DAT_00ea5ce8)[DAT_00eaac28 * 0x24ec] != 0) &&
      (iVar6 = FUN_004c5120((&DAT_00ea5f9c)[DAT_00eaac28 * 0x1276]), iVar6 == 0)) &&
     (cVar4 = FUN_004c52d0((&DAT_00ea5f9c)[DAT_00eaac28 * 0x1276],2), cVar4 != '\0')) {
    bVar2 = true;
  }
  local_28 = 1;
  do {
    iVar6 = local_28 * 0x90;
    if ((((&DAT_00bcb9c8)[iVar6] != '\x01') || ((&DAT_00bcba0a)[iVar6] != '\0')) ||
       ((&DAT_00bcba31)[iVar6] == '\0')) goto LAB_004b9ea8;
    if ((&DAT_00bcba31)[iVar6] == '\x01') {
      *(undefined2 *)(&DAT_00bcb9cc + iVar6) = (undefined2)local_10;
      (&DAT_00bcb9f1)[iVar6] = (byte)local_c;
      uVar7 = _DAT_00784cb0;
      if (((byte)(&DAT_00bcb9f0)[iVar6] < (byte)local_c) &&
         ((iVar5 = ((int)DAT_00784b94 + (_DAT_00784cb0 & 0xff) * 2) * 0x30,
          (&DAT_005e4cbc)[iVar5] == '\0' || ((&DAT_00bcb9f0)[iVar6] != 0)))) {
        iVar5 = *(int *)(&DAT_005e4cc0 + iVar5);
        bVar8 = *(char *)(iVar6 + 0xbcba3d) + 1;
        *(byte *)(iVar6 + 0xbcba3d) = bVar8;
        if (iVar5 < (int)(uint)bVar8) {
          *(undefined1 *)(iVar6 + 0xbcba3d) = 0;
          (&DAT_00d3b1c8)[DAT_00d3b1c4] = DAT_00d3b374;
          DAT_00d3b1c4 = DAT_00d3b1c4 + 1;
          if (99 < DAT_00d3b1c4) {
            DAT_00d3b1c4 = 0;
          }
          iVar5 = 0x1f;
          do {
            uVar7 = DAT_00d3b384 >> 4;
            uVar11 = DAT_00d3b384 & 1;
            uVar12 = DAT_00d3b380 >> 4;
            uVar9 = DAT_00d3b380 & 1;
            DAT_00d3b384 = DAT_00d3b384 >> 1;
            DAT_00d3b380 = DAT_00d3b380 >> 1;
            if ((uVar7 & 1) != uVar11) {
              DAT_00d3b384 = DAT_00d3b384 | 0x40000000;
            }
            if ((uVar12 & 1) != uVar9) {
              DAT_00d3b380 = DAT_00d3b380 | 0x40000000;
            }
            iVar5 = iVar5 + -1;
          } while (iVar5 != 0);
          _DAT_00d3b378 = DAT_00d3b384 & 7;
          _DAT_00d3b364 = DAT_00d3b380 & 0x7fff;
          _DAT_00d3b370 = DAT_00d3b384 & 0x7fff;
          DAT_00d3b374 = DAT_00d3b384 & 0x7f;
          _DAT_00d3b368 = DAT_00d3b380 & 0x7f;
          _DAT_00d3b36c = DAT_00d3b380 & 7;
          uVar7 = DAT_00d3b384 & 0xf;
          iVar5 = 0;
          do {
            iVar13 = (int)*(short *)(&DAT_00bcb9fe + iVar6) +
                     DAT_00bca2d8 +
                     (&DAT_005cdb78)[uVar7 * 2] +
                     ((&DAT_005cdb7c)[uVar7 * 2] + (int)*(short *)(&DAT_00bcba00 + iVar6)) * 0xe4;
            cVar4 = FUN_004ec410((&DAT_00af598c)[iVar13],0);
            if ((cVar4 != '\0') && (*(short *)(&DAT_00b4e6fc + iVar13 * 2) == 0)) goto LAB_004b957b;
            uVar7 = uVar7 + 1;
            if (0xf < (int)uVar7) {
              uVar7 = 0;
            }
            iVar5 = iVar5 + 1;
          } while (iVar5 < 0x10);
          if (*(short *)(&DAT_00b4e6fc + iVar13 * 2) == 0) {
LAB_004b957b: ;
            cVar4 = FUN_004ec410((&DAT_00af598c)[iVar13],0);
            if ((cVar4 != '\0') && (iVar5 != 0x10)) {
              (&DAT_00d3b1c8)[DAT_00d3b1c4] = DAT_00d3b374;
              DAT_00d3b1c4 = DAT_00d3b1c4 + 1;
              if (99 < DAT_00d3b1c4) {
                DAT_00d3b1c4 = 0;
              }
              local_20 = 0x1f;
              do {
                uVar9 = DAT_00d3b384 >> 4;
                uVar12 = DAT_00d3b384 & 1;
                uVar14 = DAT_00d3b380 >> 4;
                uVar11 = DAT_00d3b380 & 1;
                DAT_00d3b384 = DAT_00d3b384 >> 1;
                DAT_00d3b380 = DAT_00d3b380 >> 1;
                if ((uVar9 & 1) != uVar12) {
                  DAT_00d3b384 = DAT_00d3b384 | 0x40000000;
                }
                if ((uVar14 & 1) != uVar11) {
                  DAT_00d3b380 = DAT_00d3b380 | 0x40000000;
                }
                local_20 = local_20 + -1;
              } while (local_20 != 0);
              _DAT_00d3b378 = DAT_00d3b384 & 7;
              _DAT_00d3b370 = DAT_00d3b384 & 0x7fff;
              _DAT_00d3b364 = DAT_00d3b380 & 0x7fff;
              _DAT_00d3b368 = DAT_00d3b380 & 0x7f;
              _DAT_00d3b36c = DAT_00d3b380 & 7;
              DAT_00d3b374 = DAT_00d3b384 & 0x7f;
              iVar6 = FUN_004b4800((int)*(short *)(&DAT_00bcb9cc + iVar6),
                                   (int)*(short *)(&DAT_00bcb9cc + iVar6),
                                   (&DAT_005cd978)[uVar7 * 2] +
                                   (int)*(short *)(&DAT_00bcb9fe + iVar6),
                                   (&DAT_005cd97c)[uVar7 * 2] +
                                   (int)*(short *)(&DAT_00bcba00 + iVar6),DAT_00d3b384 & 7);
              (&DAT_00bcd630)[iVar6 * 0x184] = 0x18;
              (&DAT_00bcd658)[iVar6 * 0xc2] = (undefined2)local_28;
              *(undefined2 *)(&DAT_00bcd62e + iVar6 * 0x184) = 10;
            }
          }
        }
      }
      else {
        *(short *)(iVar6 + 0xbcba1c) = *(short *)(iVar6 + 0xbcba1c) + 1;
        iVar5 = ((int)DAT_00784b94 + (uVar7 & 0xff) * 2) * 0x30;
        if (*(int *)(&DAT_005e4cc4 + iVar5) < (int)*(short *)(iVar6 + 0xbcba1c)) {
          iVar5 = *(int *)(&DAT_005e4cc8 + iVar5);
          *(undefined2 *)(iVar6 + 0xbcba1c) = 0;
          *(undefined1 *)(iVar6 + 0xbcba3e) = 0;
          local_20 = (int)(short)(&DAT_00bcba02)[local_28 * 0x48];
          local_24 = (int)(short)(&DAT_00bcba04)[local_28 * 0x48];
          iVar5 = iVar5 >> 1;
          if (iVar5 != 0) {
            iVar13 = FUN_00562348();
            iVar5 = iVar5 + iVar13 % iVar5;
          }
          if (((&DAT_00bcba02)[local_28 * 0x48] == *(short *)(&DAT_00bcb9fe + iVar6)) &&
             ((&DAT_00bcba04)[local_28 * 0x48] == *(short *)(&DAT_00bcba00 + iVar6))) {
LAB_004b982c: ;
            iVar6 = FUN_004eb4c0((int)(short)(&DAT_00bcba02)[local_28 * 0x48],
                                 (int)(short)(&DAT_00bcba04)[local_28 * 0x48],iVar5,&local_20,
                                 &local_24,0);
            if (iVar6 == 0) goto LAB_004b9ea8;
          }
          else {
            (&DAT_00d3b1c8)[DAT_00d3b1c4] = DAT_00d3b374;
            DAT_00d3b1c4 = DAT_00d3b1c4 + 1;
            if (99 < DAT_00d3b1c4) {
              DAT_00d3b1c4 = 0;
            }
            local_1c = 0x1f;
            do {
              uVar7 = DAT_00d3b384 >> 4;
              uVar11 = DAT_00d3b384 & 1;
              uVar12 = DAT_00d3b380 >> 4;
              uVar9 = DAT_00d3b380 & 1;
              DAT_00d3b384 = DAT_00d3b384 >> 1;
              DAT_00d3b380 = DAT_00d3b380 >> 1;
              if ((uVar7 & 1) != uVar11) {
                DAT_00d3b384 = DAT_00d3b384 | 0x40000000;
              }
              if ((uVar12 & 1) != uVar9) {
                DAT_00d3b380 = DAT_00d3b380 | 0x40000000;
              }
              local_1c = local_1c + -1;
            } while (local_1c != 0);
            _DAT_00d3b378 = DAT_00d3b384 & 7;
            _DAT_00d3b364 = DAT_00d3b380 & 0x7fff;
            _DAT_00d3b370 = DAT_00d3b384 & 0x7fff;
            DAT_00d3b374 = DAT_00d3b384 & 0x7f;
            _DAT_00d3b368 = DAT_00d3b380 & 0x7f;
            _DAT_00d3b36c = DAT_00d3b380 & 7;
            local_1c = 0;
            if ((DAT_00d3b384 & 7) != 0) goto LAB_004b982c;
            local_20 = (int)*(short *)(&DAT_00bcb9fe + iVar6);
            local_24 = (int)*(short *)(&DAT_00bcba00 + iVar6);
          }
          iVar6 = FUN_004ba7e0(local_28,local_20,local_24);
          uVar3 = DAT_00bcb9a4;
          if (iVar6 != 0) {
            (&DAT_00bcba02)[local_28 * 0x48] = DAT_00bcb9a8;
            (&DAT_00bcba04)[local_28 * 0x48] = uVar3;
          }
        }
      }
    }
    else {
      *(undefined2 *)(&DAT_00bcb9cc + iVar6) = (undefined2)local_8;
      (&DAT_00bcb9f1)[iVar6] = (undefined1)local_4;
      FUN_004eaa60(local_28,0x10);
      FUN_004eaa10(local_28);
      iVar5 = FUN_004eaa60(local_28,0x11);
      if (iVar5 == 0) {
        (&DAT_00bcba3c)[iVar6] = 0;
      }
      else {
        (&DAT_00bcba3c)[iVar6] = 0x11;
      }
      if ((byte)(&DAT_00bcb9f0)[iVar6] < (byte)(&DAT_00bcb9f1)[iVar6]) {
        if (bVar2) {
          iVar5 = *(int *)(&DAT_005e4c30 + (_DAT_00784cb0 & 0xff) * 0x30) / 2;
        }
        else {
          iVar5 = *(int *)(&DAT_005e4c30 + (_DAT_00784cb0 & 0xff) * 0x30);
        }
        bVar8 = *(char *)(iVar6 + 0xbcba3d) + 1;
        *(byte *)(iVar6 + 0xbcba3d) = bVar8;
        if (iVar5 < (int)(uint)bVar8) {
          *(undefined1 *)(iVar6 + 0xbcba3d) = 0;
          (&DAT_00d3b1c8)[DAT_00d3b1c4] = DAT_00d3b374;
          DAT_00d3b1c4 = DAT_00d3b1c4 + 1;
          if (99 < DAT_00d3b1c4) {
            DAT_00d3b1c4 = 0;
          }
          iVar5 = 0x1f;
          do {
            uVar7 = DAT_00d3b384 >> 4;
            uVar11 = DAT_00d3b384 & 1;
            uVar12 = DAT_00d3b380 >> 4;
            uVar9 = DAT_00d3b380 & 1;
            DAT_00d3b384 = DAT_00d3b384 >> 1;
            DAT_00d3b380 = DAT_00d3b380 >> 1;
            if ((uVar7 & 1) != uVar11) {
              DAT_00d3b384 = DAT_00d3b384 | 0x40000000;
            }
            if ((uVar12 & 1) != uVar9) {
              DAT_00d3b380 = DAT_00d3b380 | 0x40000000;
            }
            iVar5 = iVar5 + -1;
          } while (iVar5 != 0);
          _DAT_00d3b378 = DAT_00d3b384 & 7;
          _DAT_00d3b364 = DAT_00d3b380 & 0x7fff;
          _DAT_00d3b370 = DAT_00d3b384 & 0x7fff;
          DAT_00d3b374 = DAT_00d3b384 & 0x7f;
          _DAT_00d3b368 = DAT_00d3b380 & 0x7f;
          _DAT_00d3b36c = DAT_00d3b380 & 7;
          uVar7 = DAT_00d3b384 & 0xf;
          iVar5 = 0;
          do {
            iVar13 = (int)*(short *)(&DAT_00bcb9fe + iVar6) +
                     DAT_00bca2d8 +
                     (&DAT_005cdb78)[uVar7 * 2] +
                     ((&DAT_005cdb7c)[uVar7 * 2] + (int)*(short *)(&DAT_00bcba00 + iVar6)) * 0xe4;
            cVar4 = FUN_004ec410((&DAT_00af598c)[iVar13],1);
            if ((cVar4 != '\0') && (*(short *)(&DAT_00b4e6fc + iVar13 * 2) == 0)) goto LAB_004b9a9a;
            uVar7 = uVar7 + 1;
            if (0xf < (int)uVar7) {
              uVar7 = 0;
            }
            iVar5 = iVar5 + 1;
          } while (iVar5 < 0x10);
          if (*(short *)(&DAT_00b4e6fc + iVar13 * 2) == 0) {
LAB_004b9a9a: ;
            cVar4 = FUN_004ec410((&DAT_00af598c)[iVar13],1);
            if (cVar4 != '\0') {
              if (iVar5 != 0x10) {
                iVar5 = FUN_004b4800((int)*(short *)(&DAT_00bcb9cc + iVar6),
                                     (int)*(short *)(&DAT_00bcb9cc + iVar6),
                                     (&DAT_005cdb78)[uVar7 * 2] +
                                     (int)*(short *)(&DAT_00bcb9fe + iVar6),
                                     (&DAT_005cdb7c)[uVar7 * 2] +
                                     (int)*(short *)(&DAT_00bcba00 + iVar6),0);
                (&DAT_00bcd630)[iVar5 * 0x184] = 0x18;
                (&DAT_00bcd658)[iVar5 * 0xc2] = (undefined2)local_28;
                *(undefined2 *)(&DAT_00bcd62e + iVar5 * 0x184) = 10;
              }
              goto LAB_004b9b19;
            }
          }
          goto LAB_004b9ea8;
        }
      }
LAB_004b9b19: ;
      uVar7 = _DAT_00784cb0;
      *(short *)(iVar6 + 0xbcba1c) = *(short *)(iVar6 + 0xbcba1c) + 1;
      if (((((int)*(short *)(iVar6 + 0xbcba1c) <= *(int *)(&DAT_005e4c34 + (uVar7 & 0xff) * 0x30))
           || (iVar5 = FUN_004eaa60(local_28,0xe), iVar5 != 0)) ||
          (iVar5 = FUN_004eaa60(local_28,0x10), iVar5 != 0)) || ((&DAT_00bcba3c)[iVar6] == '\x11'))
      goto LAB_004b9ea8;
      uVar7 = _DAT_00784cb0 & 0xff;
      *(undefined2 *)(iVar6 + 0xbcba1c) = 0;
      *(undefined1 *)(iVar6 + 0xbcba3e) = 0;
      local_24 = *(int *)(&DAT_005e4c38 + uVar7 * 0x30) >> 1;
      if (local_24 == 0) {
        local_24 = 0;
      }
      else {
        (&DAT_00d3b1c8)[DAT_00d3b1c4] = DAT_00d3b374;
        DAT_00d3b1c4 = DAT_00d3b1c4 + 1;
        if (99 < DAT_00d3b1c4) {
          DAT_00d3b1c4 = 0;
        }
        iVar5 = 0x1f;
        do {
          uVar7 = DAT_00d3b384 >> 4;
          uVar9 = DAT_00d3b384 & 1;
          uVar11 = DAT_00d3b380 >> 4;
          uVar12 = DAT_00d3b380 & 1;
          DAT_00d3b384 = DAT_00d3b384 >> 1;
          DAT_00d3b380 = DAT_00d3b380 >> 1;
          if ((uVar7 & 1) != uVar9) {
            DAT_00d3b384 = DAT_00d3b384 | 0x40000000;
          }
          if ((uVar11 & 1) != uVar12) {
            DAT_00d3b380 = DAT_00d3b380 | 0x40000000;
          }
          iVar5 = iVar5 + -1;
        } while (iVar5 != 0);
        DAT_00d3b374 = DAT_00d3b384 & 0x7f;
        local_24 = local_24 +
                   (int)((longlong)(ulonglong)(DAT_00d3b384 & 0x7fff) % (longlong)local_24);
      }
      (&DAT_00d3b1c8)[DAT_00d3b1c4] = DAT_00d3b374;
      DAT_00d3b1c4 = DAT_00d3b1c4 + 1;
      if (99 < DAT_00d3b1c4) {
        DAT_00d3b1c4 = 0;
      }
      local_1c = 0x1f;
      do {
        uVar7 = DAT_00d3b384 >> 4;
        uVar9 = DAT_00d3b384 & 1;
        uVar11 = DAT_00d3b380 >> 4;
        uVar12 = DAT_00d3b380 & 1;
        DAT_00d3b384 = DAT_00d3b384 >> 1;
        DAT_00d3b380 = DAT_00d3b380 >> 1;
        if ((uVar7 & 1) != uVar9) {
          DAT_00d3b384 = DAT_00d3b384 | 0x40000000;
        }
        if ((uVar11 & 1) != uVar12) {
          DAT_00d3b380 = DAT_00d3b380 | 0x40000000;
        }
        local_1c = local_1c + -1;
      } while (local_1c != 0);
      _DAT_00d3b378 = DAT_00d3b384 & 7;
      _DAT_00d3b370 = DAT_00d3b384 & 0x7fff;
      _DAT_00d3b364 = DAT_00d3b380 & 0x7fff;
      _DAT_00d3b368 = DAT_00d3b380 & 0x7f;
      _DAT_00d3b36c = DAT_00d3b380 & 7;
      *(byte *)(iVar6 + 0xbcba3e) = (byte)_DAT_00d3b370 & 7;
      sVar1 = (&DAT_00bcba02)[local_28 * 0x48];
      DAT_00d3b374 = DAT_00d3b384 & 0x7f;
      if ((sVar1 == *(short *)(&DAT_00bcb9fe + iVar6)) &&
         ((&DAT_00bcba04)[local_28 * 0x48] == *(short *)(&DAT_00bcba00 + iVar6))) {
LAB_004b9df2: ;
        local_1c = 0;
        iVar5 = FUN_004eb4c0((int)sVar1,(int)(short)(&DAT_00bcba04)[local_28 * 0x48],local_24,
                             &local_14,&local_18,1);
        if (iVar5 == 0) goto LAB_004b9ea8;
      }
      else {
        (&DAT_00d3b1c8)[DAT_00d3b1c4] = DAT_00d3b374;
        DAT_00d3b1c4 = DAT_00d3b1c4 + 1;
        if (99 < DAT_00d3b1c4) {
          DAT_00d3b1c4 = 0;
        }
        local_1c = 0x1f;
        do {
          uVar7 = DAT_00d3b384 >> 4;
          uVar9 = DAT_00d3b384 & 1;
          uVar11 = DAT_00d3b380 >> 4;
          uVar12 = DAT_00d3b380 & 1;
          DAT_00d3b384 = DAT_00d3b384 >> 1;
          DAT_00d3b380 = DAT_00d3b380 >> 1;
          if ((uVar7 & 1) != uVar9) {
            DAT_00d3b384 = DAT_00d3b384 | 0x40000000;
          }
          if ((uVar11 & 1) != uVar12) {
            DAT_00d3b380 = DAT_00d3b380 | 0x40000000;
          }
          local_1c = local_1c + -1;
        } while (local_1c != 0);
        DAT_00d3b374 = DAT_00d3b384 & 0x7f;
        _DAT_00d3b378 = DAT_00d3b384 & 7;
        _DAT_00d3b370 = DAT_00d3b384 & 0x7fff;
        _DAT_00d3b364 = DAT_00d3b380 & 0x7fff;
        _DAT_00d3b368 = DAT_00d3b380 & 0x7f;
        _DAT_00d3b36c = DAT_00d3b380 & 7;
        local_1c = 0;
        if ((DAT_00d3b384 & 7) != 0) goto LAB_004b9df2;
        local_14 = (int)*(short *)(&DAT_00bcb9fe + iVar6);
        local_18 = (int)*(short *)(&DAT_00bcba00 + iVar6);
      }
      iVar5 = FUN_004ba7e0(local_28,local_14,local_18);
      uVar3 = DAT_00bcb9a4;
      if (iVar5 != 0) {
        (&DAT_00bcba02)[local_28 * 0x48] = DAT_00bcb9a8;
        (&DAT_00bcba04)[local_28 * 0x48] = uVar3;
        psVar10 = (short *)(&DAT_00bcb9d0 + iVar6);
        iVar6 = 0x10;
        do {
          if (0 < *psVar10) {
            iVar5 = *psVar10 * 0x184;
            cVar4 = (&DAT_00bcd630)[iVar5];
            if (((cVar4 != '\x05') && (cVar4 != '\x04')) &&
               ((cVar4 != '\f' && (((cVar4 != '\x0e' && (cVar4 != '\x10')) && (cVar4 != '\x11'))))))
            {
              (&DAT_00bcd630)[iVar5] = 8;
              *(undefined2 *)(&DAT_00bcd62e + iVar5) = 0;
            }
          }
          psVar10 = psVar10 + 1;
          iVar6 = iVar6 + -1;
        } while (iVar6 != 0);
      }
    }
LAB_004b9ea8: ;
    local_28 = local_28 + 1;
    if (0x31 < local_28) {
      return;
    }
  } while( true );
}
