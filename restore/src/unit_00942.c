#include "restore_types.h"
#define DAT_007027b8 (*(char *)0x007027b8u)
#define DAT_007027bb (*(char *)0x007027bbu)
#define DAT_00705ebc (*(undefined4 *)0x00705ebcu)
#define DAT_00705ec0 (*(undefined4 *)0x00705ec0u)
#define DAT_00784cb0 (*(undefined1 *)0x00784cb0u)
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_00799b26 (*(undefined4 *)0x00799b26u)
#define DAT_00978d70 (*(undefined *)0x00978d70u)
#define DAT_00acf85c (*(undefined4 *)0x00acf85cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00bc9694 (*(undefined *)0x00bc9694u)
#define DAT_00d3b1c4 (*(undefined *)0x00d3b1c4u)
#define DAT_00d3b1c8 (*(undefined4 *)0x00d3b1c8u)
#define DAT_00d3b374 (*(undefined *)0x00d3b374u)
#define DAT_00d3b380 (*(undefined *)0x00d3b380u)
#define DAT_00d3b384 (*(undefined *)0x00d3b384u)
#define DAT_00e92e0c (*(undefined *)0x00e92e0cu)
#define DAT_00e950dc (*(undefined *)0x00e950dcu)
#define DAT_00e950e0 (*(undefined *)0x00e950e0u)
#define DAT_00e950ec (*(undefined *)0x00e950ecu)
#define DAT_00e950f0 (*(undefined *)0x00e950f0u)
#define DAT_00e95128 (*(undefined *)0x00e95128u)
#define DAT_00e9512c (*(undefined *)0x00e9512cu)
#define DAT_00e95178 (*(undefined *)0x00e95178u)
#define DAT_00e9517c (*(undefined *)0x00e9517cu)
#define DAT_00ea5c84 (*(undefined4 *)0x00ea5c84u)
#define DAT_00ea5c88 (*(undefined4 *)0x00ea5c88u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define _DAT_00d3b364 (*(undefined4 *)0x00d3b364u)
#define _DAT_00d3b368 (*(undefined4 *)0x00d3b368u)
#define _DAT_00d3b36c (*(undefined4 *)0x00d3b36cu)
#define _DAT_00d3b370 (*(undefined4 *)0x00d3b370u)
#define _DAT_00d3b378 (*(undefined4 *)0x00d3b378u)
extern void FUN_00417ae0();
extern void FUN_00468b20();
extern void FUN_004695d0();
extern void FUN_0046d100();
extern int FUN_00475b50();
extern void FUN_0047c2b0();
extern void FUN_0048c0e0();
extern void FUN_004c03d0();
extern void FUN_004e0cf0();
extern void FUN_00517370();
extern void FUN_00517660();

void FUN_00468b70(void)

{
  byte bVar1;
  short sVar2;
  bool bVar3;
  int iVar4;
  short sVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  uint uVar9;
  short *psVar10;
  int iVar11;
  uint uVar12;
  int local_10;
  int local_c;
  
  iVar11 = DAT_00eaac28;
  (&DAT_00ea5c84)[DAT_00eaac28 * 0x1276] = 0;
  (&DAT_00ea5c88)[iVar11 * 0x1276] = 0;
  bVar3 = false;
  if (DAT_00978d70 != 1) {
    (&DAT_00d3b1c8)[DAT_00d3b1c4] = DAT_00d3b374;
    DAT_00d3b1c4 = DAT_00d3b1c4 + 1;
    if (99 < DAT_00d3b1c4) {
      DAT_00d3b1c4 = 0;
    }
    iVar11 = 0x1f;
    do {
      uVar6 = DAT_00d3b384 >> 4;
      uVar8 = DAT_00d3b384 & 1;
      uVar9 = DAT_00d3b380 >> 4;
      uVar7 = DAT_00d3b380 & 1;
      DAT_00d3b384 = DAT_00d3b384 >> 1;
      DAT_00d3b380 = DAT_00d3b380 >> 1;
      if ((uVar6 & 1) != uVar8) {
        DAT_00d3b384 = DAT_00d3b384 | 0x40000000;
      }
      if ((uVar9 & 1) != uVar7) {
        DAT_00d3b380 = DAT_00d3b380 | 0x40000000;
      }
      iVar11 = iVar11 + -1;
    } while (iVar11 != 0);
    _DAT_00d3b378 = DAT_00d3b384 & 7;
    _DAT_00d3b364 = DAT_00d3b380 & 0x7fff;
    _DAT_00d3b36c = DAT_00d3b380 & 7;
    _DAT_00d3b370 = DAT_00d3b384 & 0x7fff;
    DAT_00d3b374 = DAT_00d3b384 & 0x7f;
    _DAT_00d3b368 = DAT_00d3b380 & 0x7f;
    uVar6 = DAT_00d3b384 & 7;
    local_10 = 1;
    if (0 < DAT_0078d790) {
      psVar10 = &DAT_00799b26;
      do {
        if ((((((char)psVar10[-0x23] == '\x01') && (sVar5 = psVar10[-0x1b], sVar5 != 99)) &&
             (psVar10[2] == 0)) &&
            ((((sVar5 != 0x20 || (psVar10[-7] == 0)) && ((sVar5 != 0xe2 || (psVar10[-7] == 0)))) &&
             ((sVar5 != 0xd2 || (psVar10[-7] == 0)))))) &&
           ((sVar5 != 0x49 &&
            ((((sVar5 < 0x41 || (0x45 < sVar5)) || (psVar10[-7] == 0)) &&
             (iVar11 = FUN_00475b50(local_10), local_10 == iVar11)))))) {
          bVar1 = (&DAT_00acf85c)[*(int *)(psVar10 + -0x1d)];
          sVar5 = *(short *)(&DAT_00705ec0 + psVar10[-0x1b] * 0x28);
          sVar2 = *psVar10;
          *psVar10 = sVar5 + sVar2;
          if (psVar10[2] != 0) {
            *psVar10 = *(short *)(&DAT_00705ec0 + psVar10[-0x1b] * 0x28) * 10 + sVar5 + sVar2;
          }
          if ((*(char *)((int)psVar10 + -0x41) != '\0') && (psVar10[-0x1a] < 2)) {
            *psVar10 = 0;
          }
          if (((&DAT_00af598c)[*(int *)(psVar10 + -0x1d)] & 0x10000) != 0) {
            *psVar10 = 0;
          }
          if (*psVar10 < 1000) {
            (&DAT_00d3b1c8)[DAT_00d3b1c4] = DAT_00d3b374;
            DAT_00d3b1c4 = DAT_00d3b1c4 + 1;
            if (99 < DAT_00d3b1c4) {
              DAT_00d3b1c4 = 0;
            }
            local_c = 0x1f;
            do {
              uVar7 = DAT_00d3b384 >> 4;
              uVar9 = DAT_00d3b384 & 1;
              uVar12 = DAT_00d3b380 >> 4;
              uVar8 = DAT_00d3b380 & 1;
              DAT_00d3b384 = DAT_00d3b384 >> 1;
              DAT_00d3b380 = DAT_00d3b380 >> 1;
              if ((uVar7 & 1) != uVar9) {
                DAT_00d3b384 = DAT_00d3b384 | 0x40000000;
              }
              if ((uVar12 & 1) != uVar8) {
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
            sVar5 = (short)(_DAT_00d3b370 % 5) + 1;
            if (((uint)bVar1 + local_10 & 7) == uVar6) {
              if (*(char *)((int)psVar10 + -0x41) == '\0') {
                if (((DAT_00784cb0 != '\0') && (DAT_00784cb0 == '\x02')) &&
                   (*(int *)(&DAT_00705ebc + psVar10[-0x1b] * 0x28) != 0)) {
                  psVar10[1] = psVar10[1] + 3;
                }
                psVar10[1] = psVar10[1] + *(short *)(&DAT_00705ebc + psVar10[-0x1b] * 0x28) * sVar5;
                if (*(int *)(&DAT_00705ebc + psVar10[-0x1b] * 0x28) == 0) goto LAB_00468f9e;
              }
              else if (psVar10[-0x15] < 1) {
LAB_00468f9e: ;
                psVar10[1] = 0;
              }
              else {
                psVar10[1] = psVar10[1] + *(short *)(&DAT_00705ebc + psVar10[-0x1b] * 0x28) * sVar5;
              }
              iVar4 = DAT_00e950f0;
              iVar11 = DAT_00e92e0c;
              if (999 < psVar10[1]) {
                psVar10[2] = 1;
                if (iVar11 - iVar4 < 0x3a99) {
                  DAT_00e950e0 = 0;
                }
                else {
                  DAT_00e950f0 = iVar11;
                  iVar4 = iVar11;
                }
                if (DAT_007027b8 == '\0') {
                  FUN_00417ae0(0);
                }
                else {
                  if (iVar11 - iVar4 < 0x3a99) {
                    DAT_00e950e0 = 0;
                  }
                  else {
                    DAT_00e950f0 = iVar11;
                  }
                  if (DAT_00e95128 < 1) {
                    DAT_00e950dc = 1;
                    DAT_00e95128 = 0xc;
                  }
                  FUN_004e0cf0(0x6f,(int)psVar10[-0x1b],*(undefined4 *)(psVar10 + -0x1d),0,0);
                  DAT_00e95178 = DAT_00e95178 + 1;
                }
                sVar5 = psVar10[-0x1b];
                if ((((sVar5 == 0x1f) || (sVar5 == 0x1e)) || (sVar5 == 0x21)) ||
                   ((sVar5 == 0xd1 || ((0x40 < sVar5 && (sVar5 < 0x46)))))) {
                  FUN_00468b20(local_10);
                }
                else {
                  psVar10[2] = 1;
                  FUN_0046d100(local_10,0);
                  FUN_004695d0(local_10,1);
                }
              }
            }
          }
          else {
            if (DAT_00e92e0c - DAT_00e950ec < 0x3a99) {
              DAT_00e950e0 = 0;
            }
            else {
              DAT_00e950ec = DAT_00e92e0c;
            }
            if (DAT_007027bb == '\0') {
              FUN_00417ae0(3);
            }
            else {
              if (DAT_00e9512c < 1) {
                DAT_00e950dc = 1;
                DAT_00e9512c = 0xc;
              }
              FUN_004e0cf0(0x70,(int)psVar10[-0x1b],*(undefined4 *)(psVar10 + -0x1d),0,0);
              DAT_00e9517c = DAT_00e9517c + 1;
            }
            *(undefined1 *)(psVar10 + -0x23) = 4;
            DAT_00bc9694 = 0;
            if (psVar10[-0x1b] == 0xca) {
              FUN_004c03d0(local_10);
            }
            else {
              FUN_0047c2b0(local_10,(int)psVar10[-0x20],(int)psVar10[-0x1f],
                           *(undefined1 *)((int)psVar10 + -0x43),
                           *(undefined1 *)((int)psVar10 + -0x43));
            }
            FUN_0048c0e0((int)psVar10[-0x20],(int)psVar10[-0x1f],
                         *(undefined1 *)((int)psVar10 + -0x43),1);
            FUN_004695d0(local_10,0);
            bVar3 = true;
          }
        }
        local_10 = local_10 + 1;
        psVar10 = psVar10 + 0x84;
      } while (local_10 <= DAT_0078d790);
      if (bVar3) {
        FUN_00517370();
        FUN_00517660();
      }
    }
  }
  return;
}
