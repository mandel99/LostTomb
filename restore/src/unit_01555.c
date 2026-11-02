#include "restore_types.h"
#include "flood_simulation.h"
#define DAT_007999d8 (*(undefined4 *)0x007999d8u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a18 (*(undefined4 *)0x00799a18u)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00799a7e (*(undefined4 *)0x00799a7eu)
#define DAT_00799a97 (*(undefined4 *)0x00799a97u)
#define DAT_00a5d4cc (*(undefined4 *)0x00a5d4ccu)
#define DAT_00adc36c (*(undefined4 *)0x00adc36cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b285cc (*(undefined4 *)0x00b285ccu)
#define DAT_00b350dc (*(undefined4 *)0x00b350dcu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bc97c0 (*(undefined *)0x00bc97c0u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bcd600 (*(undefined4 *)0x00bcd600u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd618 (*(undefined4 *)0x00bcd618u)
#define DAT_00bcd642 (*(undefined4 *)0x00bcd642u)
#define DAT_00bcd644 (*(undefined4 *)0x00bcd644u)
#define DAT_00ea6208 (*(undefined4 *)0x00ea6208u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern void FUN_00475cc0();
extern void FUN_004777c0();
extern void FUN_0047b590();
extern void FUN_0047bae0();
extern void FUN_0047bd50();
extern void FUN_0047db00();
extern int FUN_00488030();
extern void FUN_00488e60();
extern uint FUN_004894d0();
extern void FUN_004b52f0();
extern void FUN_004b5420();
extern void FUN_004bd250();
extern void FUN_00516f90();
extern void FUN_005173a0();
extern void FUN_00517690();
extern longlong FUN_0056133c();

void FUN_004bd950(uintptr_t receiver,uint8_t param_1)

{
  bool bVar1;
  undefined1 uVar2;
  char cVar3;
  int iVar4;
  uint *puVar5;
  uintptr_t in_ECX=receiver;
  int iVar6;
  uint *puVar7;
  uint uVar8;
  undefined2 *puVar9;
  int iVar10;
  uint uVar11;
  uint uVar12;
  short sVar13;
  int iVar14;
  uint uVar15;
  short sVar16;
  int iVar17;
  int iVar18;
  int iVar19;
  
  iVar4 = *(int *)(in_ECX + 8);
  if (param_1 == '\0') {
    iVar6 = (int)*(short *)(&DAT_00b4e6fc + iVar4 * 2);
    uVar8 = (&DAT_00af598c)[iVar4];
    if ((uVar8 & 4) == 0) {
      FUN_004777c0(*(int *)(in_ECX + 0xc),*(int *)(in_ECX + 0x10),*(int *)(in_ECX + 0xc) + 1,
                   *(int *)(in_ECX + 0x10) + 1);
      FUN_00475cc0(*(int *)(in_ECX + 0xc) + -1,*(int *)(in_ECX + 0x10) + -1,
                   *(int *)(in_ECX + 0xc) + 1,*(int *)(in_ECX + 0x10) + 1,0);
      FUN_0047b590(*(int *)(in_ECX + 0xc) + -2,*(int *)(in_ECX + 0x10) + -2,
                   *(int *)(in_ECX + 0xc) + 2,*(int *)(in_ECX + 0x10) + 2);
      FUN_0047bae0(*(int *)(in_ECX + 0xc) + -2,*(int *)(in_ECX + 0x10) + -2,
                   *(int *)(in_ECX + 0xc) + 2,*(int *)(in_ECX + 0x10) + 2,0);
    }
    else {
      (&DAT_00af598c)[iVar4] = uVar8 & 0xfbfffffb;
      if ((uVar8 & 0x8000000) != 0) {
        (&DAT_00af598c)[iVar4] = uVar8 & 0xf3fffffb | 0x40;
      }
      FUN_0047bd50(*(int *)(in_ECX + 0xc) + -3,*(int *)(in_ECX + 0x10) + -3,
                   *(int *)(in_ECX + 0xc) + 3,*(int *)(in_ECX + 0x10) + 3);
      FUN_00517690(*(int *)(in_ECX + 0xc) + -1,*(int *)(in_ECX + 0x10) + -1,
                   *(int *)(in_ECX + 0xc) + 1,*(int *)(in_ECX + 0x10) + 1);
      FUN_00516f90(*(int *)(in_ECX + 0xc) + -1,*(int *)(in_ECX + 0x10) + -1,
                   *(int *)(in_ECX + 0xc) + 1,*(int *)(in_ECX + 0x10) + 1,0);
      FUN_005173a0(*(int *)(in_ECX + 0xc) + -1,*(int *)(in_ECX + 0x10) + -1,
                   *(int *)(in_ECX + 0xc) + 1,*(int *)(in_ECX + 0x10) + 1);
      FUN_004777c0(*(int *)(in_ECX + 0xc) + -2,*(int *)(in_ECX + 0x10) + -2,
                   *(int *)(in_ECX + 0xc) + 2,*(int *)(in_ECX + 0x10) + 2);
      FUN_00475cc0(*(int *)(in_ECX + 0xc) + -2,*(int *)(in_ECX + 0x10) + -2,
                   *(int *)(in_ECX + 0xc) + 2,*(int *)(in_ECX + 0x10) + 2,0);
      FUN_0047b590(*(int *)(in_ECX + 0xc) + -3,*(int *)(in_ECX + 0x10) + -3,
                   *(int *)(in_ECX + 0xc) + 3,*(int *)(in_ECX + 0x10) + 3);
      FUN_0047bae0(*(int *)(in_ECX + 0xc) + -3,*(int *)(in_ECX + 0x10) + -3,
                   *(int *)(in_ECX + 0xc) + 3,*(int *)(in_ECX + 0x10) + 3,0);
      iVar18 = (int)*(short *)(&DAT_00b350dc + iVar4 * 2);
      iVar10 = iVar18 * 0x184;
      if (*(short *)(&DAT_00bcd600 + iVar10) != iVar18) {
        do {
          if (iVar18 == 0) break;
          switch((&DAT_00bcd602)[iVar10]) {
          case 0x14: ;
          case 0x19: ;
          case 0x4d: ;
          case 0x4e: ;
          case 100: ;
          case 0x65: ;
            DAT_00bc9768 = iVar4;
            FUN_0047db00(4);
            iVar14 = 0;
            uVar15 = 0;
            uVar8 = 0xffffffff;
            uVar11 = 0xffffffff;
            iVar17 = 0;
            do {
              if ((&DAT_00bc97c0)[uVar15 & 7] == 0) {
LAB_004bde1f: ;
                uVar12 = uVar11;
                iVar19 = iVar17;
                if (iVar14 < iVar17) {
                  uVar12 = 0xffffffff;
                  iVar19 = 0;
                  iVar14 = iVar17;
                  uVar8 = uVar11;
                }
              }
              else {
                if ((int)uVar11 < 0) {
                  uVar11 = uVar15;
                }
                iVar17 = iVar17 + 1;
                if (((&DAT_00bc97c0)[uVar15 & 7] == 0) ||
                   (uVar12 = uVar11, iVar19 = iVar17, uVar15 == 0xf)) goto LAB_004bde1f;
              }
              uVar15 = uVar15 + 1;
              uVar11 = uVar12;
              iVar17 = iVar19;
            } while ((int)uVar15 < 0x10);
            sVar13 = 0;
            sVar16 = 0;
            iVar17 = 0;
            switch(iVar14 / 2 + uVar8 & 7) {
            case 0: ;
              sVar16 = -1;
              iVar17 = -0xe4;
              break;
            case 1: ;
              sVar16 = -1;
              sVar13 = 1;
              iVar17 = -0xe3;
              break;
            case 2: ;
              sVar13 = 1;
              iVar17 = 1;
              break;
            case 3: ;
              sVar16 = 1;
              iVar17 = 0xe5;
              sVar13 = 1;
              break;
            case 4: ;
              sVar16 = 1;
              iVar17 = 0xe4;
              break;
            case 5: ;
              sVar16 = 1;
              sVar13 = -1;
              iVar17 = 0xe3;
              break;
            case 6: ;
              sVar13 = -1;
              iVar17 = -1;
              break;
            case 7: ;
              sVar16 = -1;
              sVar13 = -1;
              iVar17 = -0xe5;
            }
            FUN_004b5420(iVar18);
            *(short *)((int)&DAT_00bcd642 + iVar10) =
                 *(short *)((int)&DAT_00bcd642 + iVar10) + sVar13 * 0x20;
            *(short *)((int)&DAT_00bcd60c + iVar10) =
                 *(short *)((int)&DAT_00bcd60c + iVar10) + sVar13 * 2;
            *(short *)((int)&DAT_00bcd644 + iVar10) =
                 *(short *)((int)&DAT_00bcd644 + iVar10) + sVar16 * 0x20;
            *(short *)((int)&DAT_00bcd60e + iVar10) =
                 *(short *)((int)&DAT_00bcd60e + iVar10) + sVar16 * 2;
            *(int *)(&DAT_00bcd618 + iVar10) = *(int *)(&DAT_00bcd618 + iVar10) + iVar17 * 2;
            FUN_004b52f0(iVar18);
          }
          iVar18 = (int)*(short *)(&DAT_00bcd600 + iVar10);
          iVar10 = iVar18 * 0x184;
        } while (*(short *)(&DAT_00bcd600 + iVar10) != iVar18);
      }
    }
    cVar3 = FUN_00488030(iVar6);
    if ((cVar3 != '\0') && (cVar3 = FUN_004894d0(iVar6), cVar3 == '\0')) {
      iVar18 = 3;
      iVar4 = DAT_00bca2d8 + *(short *)((int)&DAT_007999dc + iVar6 * 0x108 + 2) +
              *(short *)(&DAT_007999e0 + iVar6 * 0x108) * 0xe4;
      puVar9 = (undefined2 *)(&DAT_00b285cc + iVar4);
      puVar7 = &DAT_00af598c + iVar4;
      do {
        *puVar9 = 0x202;
        *(undefined1 *)(puVar9 + 1) = 2;
        iVar4 = 3;
        puVar5 = puVar7;
        do {
          iVar4 = iVar4 + -1;
          *puVar5 = *puVar5 | 8;
          puVar5 = puVar5 + 1;
        } while (iVar4 != 0);
        puVar9 = puVar9 + 0x72;
        puVar7 = puVar7 + 0xe4;
        iVar18 = iVar18 + -1;
      } while (iVar18 != 0);
      FUN_00488e60(iVar6,0);
    }
  }
  else {
    uVar8 = (&DAT_00af598c)[iVar4];
    (&DAT_00adc36c)[iVar4] = 0;
    (&DAT_00af598c)[iVar4] = uVar8 & 0xffffefff | 4;
    if (*(char *)(in_ECX + 0x18) == '\0') {
      uVar2 = 0;
    }
    else {
      uVar2 = FUN_0056133c();
    }
    iVar6 = (int)*(short *)(&DAT_00b4e6fc + iVar4 * 2);
    (&DAT_00a5d4cc)[iVar4] = uVar2;
    if ((uVar8 & 0x40) != 0) {
      (&DAT_00af598c)[iVar4] = uVar8 & 0xffffefbf | 0x8000004;
    }
    FUN_00517690(*(int *)(in_ECX + 0xc) + -1,*(int *)(in_ECX + 0x10) + -1,*(int *)(in_ECX + 0xc) + 1
                 ,*(int *)(in_ECX + 0x10) + 1);
    FUN_00516f90(*(int *)(in_ECX + 0xc) + -1,*(int *)(in_ECX + 0x10) + -1,*(int *)(in_ECX + 0xc) + 1
                 ,*(int *)(in_ECX + 0x10) + 1,0);
    FUN_005173a0(*(int *)(in_ECX + 0xc) + -1,*(int *)(in_ECX + 0x10) + -1,*(int *)(in_ECX + 0xc) + 1
                 ,*(int *)(in_ECX + 0x10) + 1);
    FUN_004777c0(*(int *)(in_ECX + 0xc) + -1,*(int *)(in_ECX + 0x10) + -1,*(int *)(in_ECX + 0xc) + 1
                 ,*(int *)(in_ECX + 0x10) + 1);
    FUN_00475cc0(*(int *)(in_ECX + 0xc) + -2,*(int *)(in_ECX + 0x10) + -2,*(int *)(in_ECX + 0xc) + 2
                 ,*(int *)(in_ECX + 0x10) + 2,0);
    FUN_0047b590(*(int *)(in_ECX + 0xc) + -2,*(int *)(in_ECX + 0x10) + -2,*(int *)(in_ECX + 0xc) + 2
                 ,*(int *)(in_ECX + 0x10) + 2);
    FUN_0047bae0(*(int *)(in_ECX + 0xc) + -2,*(int *)(in_ECX + 0x10) + -2,*(int *)(in_ECX + 0xc) + 2
                 ,*(int *)(in_ECX + 0x10) + 2,0);
    if (((uVar8 & 8) != 0) && (iVar6 != 0)) {
      iVar18 = iVar6 * 0x108;
      sVar13 = (&DAT_007999e8)[iVar6 * 0x84];
      if ((((sVar13 != 99) &&
           ((((sVar13 != 0x58 && (sVar13 != 0x59)) && (sVar13 != 0x5d)) &&
            ((sVar13 != 0x51 && (sVar13 != 0x5a)))))) && (sVar13 != 0x8a)) &&
         ((&DAT_007999d8)[iVar18] != '\x06')) {
        cVar3 = FUN_00488030(iVar6);
        if (cVar3 == '\0') {
          (&DAT_007999d8)[iVar18] = 6;
          (&DAT_00799a97)[iVar18] = 1;
        }
        else if (*(int *)(&DAT_00ea6208 + DAT_00eaac28 * 0x49d8) == 0) {
          bVar1 = true;
          iVar14 = 0;
          iVar10 = DAT_00bca2d8 + *(short *)((int)&DAT_007999dc + iVar18 + 2) +
                   *(short *)(&DAT_007999e0 + iVar18) * 0xe4;
          do {
            if (!bVar1) break;
            iVar14 = iVar14 + 1;
            puVar7 = &DAT_00af598c + iVar10;
            iVar17 = 3;
            do {
              iVar19 = iVar10;
              uVar8 = *puVar7;
              if ((uVar8 & 8) == 0) {
                bVar1 = false;
              }
              else {
                (&DAT_00b285cc)[iVar19] = 0;
                *puVar7 = uVar8 & 0xfffffff7;
              }
              puVar7 = puVar7 + 1;
              iVar17 = iVar17 + -1;
              iVar10 = iVar19 + 1;
            } while (iVar17 != 0);
            iVar10 = iVar19 + 0xe2;
          } while (iVar14 < 3);
          (&DAT_00799a2c)[iVar6 * 0x84] = 0;
          (&DAT_00799a18)[iVar6 * 0x84] = 0;
          *(undefined1 *)(&DAT_00799a7e + iVar6 * 0x84) = 0;
          *(undefined1 *)((int)&DAT_00799a7e + iVar18 + 1) = 0;
          FUN_004777c0(*(int *)(in_ECX + 0xc) + -2,*(int *)(in_ECX + 0x10) + -2,
                       *(int *)(in_ECX + 0xc) + 2,*(int *)(in_ECX + 0x10) + 2);
          FUN_00475cc0(*(int *)(in_ECX + 0xc) + -2,*(int *)(in_ECX + 0x10) + -2,
                       *(int *)(in_ECX + 0xc) + 2,*(int *)(in_ECX + 0x10) + 2,0);
        }
        else {
          *(undefined4 *)(&DAT_00ea6208 + DAT_00eaac28 * 0x49d8) = 2;
          (&DAT_007999d8)[iVar18] = 6;
        }
      }
    }
    iVar4 = (int)*(short *)(&DAT_00b350dc + iVar4 * 2);
    if (iVar4 != 0) {
      while (iVar6 = iVar4 * 0x184, *(short *)(&DAT_00bcd600 + iVar6) != iVar4) {
        if (((&DAT_00bcd602)[iVar6] != '\x0e') && ((&DAT_00bcd602)[iVar6] != '\x1c')) {
          FUN_004bd250(iVar4);
        }
        iVar4 = (int)*(short *)(&DAT_00bcd600 + iVar6);
        if (iVar4 == 0) {
          return;
        }
      }
    }
  }
  return;
}
