#include "restore_types.h"
#define DAT_0056f7d8 (*(undefined4 *)0x0056f7d8u)
#define DAT_0056f7ec (*(undefined4 *)0x0056f7ecu)
#define DAT_005e4ef4 (*(undefined *)0x005e4ef4u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00799a3c (*(undefined4 *)0x00799a3cu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd620 (*(undefined4 *)0x00bcd620u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd660 (*(undefined4 *)0x00bcd660u)
#define DAT_00bcd672 (*(undefined4 *)0x00bcd672u)
#define DAT_00eaac44 (*(undefined *)0x00eaac44u)
#define DAT_00eaaf58 (*(undefined *)0x00eaaf58u)
extern uint FUN_004eea10();
extern void FUN_004ef450();
extern void FUN_004f25c0();
extern bool FUN_004f28b0();
extern int FUN_004f29e0();

void FUN_004f2cf0(int param_1)

{
  byte bVar1;
  short sVar2;
  char cVar3;
  int iVar4;
  int iVar5;
  int *piVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  int iVar12;
  short *psVar13;
  int local_20;
  int local_c;
  int local_8;
  
  iVar5 = param_1 * 0x184;
  iVar9 = (int)*(short *)(&DAT_00b4e6fc + *(int *)(&DAT_00bcd620 + iVar5) * 2);
  if (iVar9 == 0) {
    return;
  }
  if ((&DAT_007999e8)[iVar9 * 0x84] != 0xb7) {
    return;
  }
  local_c = 0;
  iVar11 = iVar9 - (short)(&DAT_007999ea)[iVar9 * 0x84];
  local_8 = 0;
  bVar1 = *(byte *)(&DAT_00799a2c + iVar11 * 0x84);
  if (*(short *)(&DAT_00bcd660 + iVar5) != 0) {
    iVar12 = *(short *)(&DAT_00bcd660 + iVar5) * 0x184;
    if ((*(short *)(&DAT_00b4e6fc + *(int *)(&DAT_00bcd620 + iVar12) * 2) != 0) &&
       (cVar3 = FUN_004eea10((int)*(short *)(&DAT_00b4e6fc + *(int *)(&DAT_00bcd620 + iVar12) * 2)),
       cVar3 != '\0')) goto LAB_004f2dd2;
  }
  iVar12 = FUN_004f29e0(iVar9,1);
  if (iVar12 == 0) {
    return;
  }
  *(short *)(&DAT_00bcd660 + iVar5) = (short)iVar12;
  iVar12 = iVar12 * 0x184;
  *(undefined2 *)(&DAT_00bcd660 + iVar12) = (undefined2)param_1;
LAB_004f2dd2: ;
  sVar2 = *(short *)(&DAT_00b4e6fc + *(int *)(&DAT_00bcd620 + iVar12) * 2);
  FUN_004ef450(iVar11);
  FUN_004f25c0();
  iVar9 = 0;
  if (0 < DAT_00eaaf58) {
    piVar6 = &DAT_00eaac44;
    while (*piVar6 != (int)sVar2) {
      iVar9 = iVar9 + 1;
      piVar6 = piVar6 + 1;
      if (DAT_00eaaf58 <= iVar9) {
        return;
      }
    }
    if ((iVar9 != 0) && (-1 < iVar9)) {
      psVar13 = &DAT_00799a3c + iVar11 * 0x84;
      iVar12 = iVar11;
      local_20 = iVar9;
      while( true ) {
        if (DAT_005e4ef4 == 5) {
          iVar10 = *(int *)(&DAT_0056f7ec + (uint)bVar1 * 4);
        }
        else {
          iVar10 = *(int *)(&DAT_0056f7d8 + (uint)bVar1 * 4);
        }
        if (iVar10 + iVar11 <= iVar12) break;
        iVar10 = (int)*psVar13;
        if ((iVar10 != 0) && (cVar3 = FUN_004f28b0(iVar12), cVar3 == '\0')) {
          iVar4 = iVar10 * 0x184;
          if (((((&DAT_00bcd602)[iVar4] == 'Q') && ((&DAT_00bcd630)[iVar4] == '\n')) ||
              (((&DAT_00bcd602)[iVar4] == 'P' && ((&DAT_00bcd630)[iVar4] == '\v')))) &&
             (iVar8 = (int)*(short *)(&DAT_00bcd660 + iVar4), iVar8 != param_1)) {
            if (iVar8 != 0) {
              iVar7 = iVar8 * 0x184;
              if (*(short *)(&DAT_00bcd660 + iVar7) == iVar10) {
                if (((&DAT_00bcd602)[iVar7] != 'U') ||
                   ((((cVar3 = (&DAT_00bcd630)[iVar7], cVar3 != '\x0f' && (cVar3 != '\x10')) &&
                     (cVar3 != '\x1e')) && (cVar3 != '\x11')))) goto LAB_004f2f72;
              }
              else {
                iVar8 = 0;
                *(undefined2 *)(&DAT_00bcd660 + iVar4) = 0;
              }
            }
            iVar7 = 0;
            iVar4 = 999;
            if (0 < DAT_00eaaf58) {
              piVar6 = &DAT_00eaac44;
              do {
                iVar4 = iVar7;
                if (*piVar6 == iVar12) break;
                iVar7 = iVar7 + 1;
                piVar6 = piVar6 + 1;
                iVar4 = 999;
              } while (iVar7 < DAT_00eaaf58);
            }
            if (((iVar4 < local_20) && (iVar8 != 0)) &&
               ((&DAT_00bcd672)[iVar5] == (&DAT_00bcd672)[iVar8 * 0x184])) {
              local_20 = iVar4;
              local_c = iVar10;
              local_8 = iVar8;
            }
          }
        }
LAB_004f2f72: ;
        iVar12 = iVar12 + 1;
        psVar13 = psVar13 + 0x84;
      }
      if (local_20 != iVar9) {
        sVar2 = *(short *)(&DAT_00bcd660 + iVar5);
        *(undefined2 *)(&DAT_00bcd660 + local_c * 0x184) = (undefined2)param_1;
        *(short *)(&DAT_00bcd660 + iVar5) = (short)local_c;
        if (local_8 != 0) {
          *(short *)(&DAT_00bcd660 + sVar2 * 0x184) = (short)local_8;
          *(short *)(&DAT_00bcd660 + local_8 * 0x184) = sVar2;
        }
      }
    }
  }
  return;
}
