#include "restore_types.h"
#define DAT_005cc8c0 (*(undefined *)0x005cc8c0u)
#define DAT_007999e4 (*(undefined4 *)0x007999e4u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00a9010c (*(undefined4 *)0x00a9010cu)
#define DAT_00a9cc1c (*(undefined4 *)0x00a9cc1cu)
#define DAT_00acf85c (*(undefined4 *)0x00acf85cu)
#define DAT_00adc36c (*(undefined4 *)0x00adc36cu)
#define DAT_00ae8e7c (*(undefined4 *)0x00ae8e7cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b285cc (*(undefined4 *)0x00b285ccu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00b67d1c (*(undefined4 *)0x00b67d1cu)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00bc9738 (*(undefined *)0x00bc9738u)
#define DAT_00bc973c (*(undefined *)0x00bc973cu)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00f56b54 (*(undefined *)0x00f56b54u)
#define _DAT_00bc9734 (*(undefined4 *)0x00bc9734u)
extern void FUN_004746d0();
extern void FUN_00475cc0();
extern void FUN_004767d0();
extern void FUN_00477500();
extern void FUN_004776f0();
extern void FUN_004777c0();
extern void FUN_00477890();
extern void FUN_0047a220();
extern void FUN_0047c7b0();
extern int FUN_00488030();
extern undefined4 FUN_00541ab0();

void FUN_0047c480(int param_1,int *param_2,int param_3)

{
  uint *puVar1;
  byte bVar2;
  uint uVar3;
  int iVar4;
  int iVar5;
  char cVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int *local_10;
  uint local_8;
  
  iVar5 = param_3;
  iVar4 = (int)param_2;
  if ((int)param_2 < 0) {
    _DAT_00bc9734 = 1;
    return;
  }
  if (param_3 < 0) {
    _DAT_00bc9734 = 1;
    return;
  }
  if (DAT_00bca2d0 <= (int)param_2) {
    _DAT_00bc9734 = 1;
    return;
  }
  if (DAT_00bca2d4 <= param_3) {
    _DAT_00bc9734 = 1;
    return;
  }
  if ((&DAT_007999e8)[param_1 * 0x84] == 0xb7) {
    iVar7 = *(int *)(&DAT_007999e4 + param_1 * 0x108);
    DAT_00bc973c = 2;
    DAT_00bc9738 = 2;
  }
  else {
    iVar7 = (FUN_0047c7b0(param_2,param_3), restore_unresolved_return(0x0047c7b0u));
  }
  if ((&DAT_00af598c)[iVar7] != 2) {
    cVar6 = FUN_00488030(param_1);
    if (cVar6 == '\0') {
      if ((&DAT_007999e8)[param_1 * 0x84] == 0xd5) {
        DAT_00bc9738 = FUN_00541ab0(param_1);
        DAT_00bc973c = DAT_00bc9738;
      }
    }
    else {
      DAT_00bc973c = 3;
      DAT_00bc9738 = 3;
    }
    local_8 = 0x3052e84;
    if ((&DAT_007999e8)[param_1 * 0x84] == 0x8a) {
      local_8 = (&DAT_00af598c)[iVar7] & 0x140 | 0x3052e84;
    }
    param_3 = 0;
    iVar8 = DAT_00bc973c;
    iVar9 = DAT_00bc9738;
    if (0 < DAT_00bc9738) {
      local_10 = &DAT_005cc8c0;
      do {
        iVar10 = 0;
        if (0 < iVar8) {
          param_2 = local_10;
          do {
            iVar8 = iVar7 + *param_2;
            if (param_1 == 0) {
LAB_0047c62e: ;
              bVar2 = (&DAT_00b285cc)[iVar8];
              puVar1 = &DAT_00af598c + iVar8;
              *(undefined2 *)(&DAT_00b4e6fc + iVar8 * 2) = 0;
              (&DAT_00b285cc)[iVar8] = bVar2 & 0xe0;
              bVar2 = (&DAT_00b67d1c)[iVar8];
              *(undefined2 *)(&DAT_00a9cc1c + iVar8 * 2) = 0;
              (&DAT_00ae8e7c)[iVar8] = 0;
              (&DAT_00b67d1c)[iVar8] = bVar2 & 0xc0 | 0x40;
              uVar3 = *puVar1;
              (&DAT_00adc36c)[iVar8] = 0;
              if (((uVar3 & 4) == 0) || ((uVar3 & 0x10000) != 0)) {
                (&DAT_00b7482c)[iVar8] = ((byte)(&DAT_00acf85c)[iVar8] & 7) + DAT_00f56b54;
                *puVar1 = uVar3 & local_8;
              }
              else {
                *puVar1 = uVar3 & 4;
                FUN_0047a220(iVar10 + iVar4,param_3 + iVar5,1);
              }
            }
            else if (*(short *)(&DAT_00b4e6fc + iVar8 * 2) == param_1) {
              if ((&DAT_007999e8)[param_1 * 0x84] != 99) {
                (&DAT_00a9010c)[iVar8] = *(undefined1 *)(&DAT_007999e8 + param_1 * 0x84);
              }
              goto LAB_0047c62e;
            }
            iVar10 = iVar10 + 1;
            param_2 = param_2 + 2;
            iVar8 = DAT_00bc973c;
            iVar9 = DAT_00bc9738;
          } while (iVar10 < DAT_00bc973c);
        }
        param_3 = param_3 + 1;
        local_10 = local_10 + 0xc;
      } while (param_3 < iVar9);
    }
    FUN_004767d0(iVar4,iVar5,iVar8 + iVar4,iVar9 + iVar5,0,0);
    FUN_004746d0(iVar4,iVar5,DAT_00bc973c);
    FUN_00477500(iVar4,iVar5,DAT_00bc973c + iVar4,DAT_00bc9738 + iVar5);
    FUN_004777c0(iVar4,iVar5,DAT_00bc973c + iVar4,DAT_00bc9738 + iVar5);
    FUN_00475cc0(iVar4,iVar5,DAT_00bc973c + iVar4,DAT_00bc9738 + iVar5,0);
    FUN_004776f0(iVar4,iVar5,DAT_00bc973c + iVar4,DAT_00bc9738 + iVar5);
    FUN_00477890(iVar4,iVar5,DAT_00bc973c + iVar4,DAT_00bc9738 + iVar5);
  }
  return;
}
