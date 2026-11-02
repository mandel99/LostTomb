#include "restore_types.h"
#define DAT_007756f0 (*(undefined4 *)0x007756f0u)
#define DAT_007756f4 (*(undefined4 *)0x007756f4u)
#define DAT_00775780 (*(undefined4 *)0x00775780u)
#define DAT_00775784 (*(undefined4 *)0x00775784u)
#define DAT_007900c1 (*(undefined4 *)0x007900c1u)
#define DAT_007900c2 (*(undefined4 *)0x007900c2u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_007999fe (*(undefined4 *)0x007999feu)
#define DAT_00799a00 (*(undefined4 *)0x00799a00u)
#define DAT_00799a12 (*(undefined4 *)0x00799a12u)
#define DAT_00799a14 (*(undefined4 *)0x00799a14u)
#define DAT_00799a1c (*(undefined4 *)0x00799a1cu)
#define DAT_00799af0 (*(undefined4 *)0x00799af0u)
#define DAT_00bcd607 (*(undefined4 *)0x00bcd607u)
#define DAT_00bcd666 (*(undefined4 *)0x00bcd666u)
#define DAT_00bcd667 (*(undefined4 *)0x00bcd667u)
#define DAT_00bcd66c (*(undefined4 *)0x00bcd66cu)
#define DAT_00ea5b34 (*(undefined4 *)0x00ea5b34u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)
extern undefined4 FUN_004434e0();
extern bool FUN_004435f0();
extern undefined4 FUN_0045cf50();
extern undefined4 FUN_0045d180();
extern int FUN_0045dc50();
extern undefined4 FUN_00480180();

int FUN_004450c0(int param_1,undefined4 param_2,undefined4 param_3,int param_4,undefined4 param_5)

{
  byte bVar1;
  char cVar2;
  short sVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int *piVar7;
  int iVar8;
  int iVar9;
  short *psVar10;
  int iVar11;
  int iVar12;
  int iVar13;
  int local_1c;
  int local_18;
  int local_10;
  int local_c;
  int local_4;
  
  iVar12 = 0;
  iVar11 = param_1 * 0x184;
  local_c = 10000;
  local_4 = 0;
  iVar13 = 1;
  do {
    iVar6 = FUN_004434e0(DAT_00eaac28,param_4,iVar13);
    if (iVar6 == 0) {
      *(undefined4 *)((int)&DAT_00775784 + iVar12) = 0;
    }
    else {
      *(undefined4 *)((int)&DAT_00775784 + iVar12) = 1;
    }
    if (7 < (byte)(&DAT_00bcd667)[iVar11]) {
      *(undefined4 *)((int)&DAT_00775784 + iVar12) = 0;
    }
    if ((param_4 == 0) || (iVar6 = FUN_004435f0(DAT_00eaac28,param_4,iVar13), iVar6 == 0)) {
      *(undefined4 *)((int)&DAT_007756f4 + iVar12) = 0;
    }
    else {
      *(undefined4 *)((int)&DAT_007756f4 + iVar12) = 1;
    }
    if (7 < *(short *)(&DAT_00bcd66c + iVar11)) {
      *(undefined4 *)((int)&DAT_007756f4 + iVar12) = 0;
    }
    iVar12 = iVar12 + 4;
    iVar13 = iVar13 + 1;
  } while (iVar12 < 0x8c);
  iVar12 = 0;
  piVar7 = &DAT_007756f4;
  do {
    if (*piVar7 == 1) {
      iVar12 = iVar12 + 1;
    }
    piVar7 = piVar7 + 1;
  } while ((int)piVar7 < 0x775780);
  local_1c = 1;
  psVar10 = &DAT_00799af0;
  do {
    if (((char)psVar10[-8] == '\x01') &&
       ((((*psVar10 == 0x48 || (*psVar10 == 0xc1)) && ((char)psVar10[0x1a] != '\0')) &&
        ((0 < psVar10[8] && (psVar10[0x18] != 0)))))) {
      bVar1 = *(byte *)((int)psVar10 + 0xb1);
      iVar13 = FUN_0045d180(param_1,(&DAT_00bcd607)[iVar11],local_1c,(&DAT_00bcd666)[iVar11]);
      if ((iVar13 != 0) ||
         (iVar13 = FUN_0045cf50(param_1,(&DAT_00bcd607)[iVar11],local_1c,(&DAT_00bcd666)[iVar11]),
         iVar13 != 0)) {
        local_18 = 0;
        iVar6 = 1;
        iVar13 = (uint)bVar1 * 0xc4;
        do {
          cVar2 = *(char *)((int)&DAT_007900c2 + iVar13 + iVar6);
          if (((cVar2 != '\x01') && (cVar2 != '\x03')) &&
             (iVar8 = FUN_004435f0(DAT_00eaac28,param_4,iVar6), iVar8 != 0)) {
            local_18 = local_18 + 1;
          }
          iVar6 = iVar6 + 1;
        } while (iVar6 < 0x24);
        iVar8 = 0x20;
        local_10 = 8;
        iVar6 = local_1c;
        do {
          iVar5 = DAT_00eaac28;
          iVar6 = (int)*(short *)(&DAT_00799a12 + iVar6 * 0x108);
          if ((iVar6 != 0) &&
             (*(int *)(&DAT_00775780 + (short)(&DAT_007999ea)[iVar6 * 0x84] * 4) != 0)) {
            iVar8 = iVar8 + -4;
          }
          if (((iVar12 != 0) && (local_18 != 0)) && ((&DAT_007900c1)[iVar13] == '\0')) {
            iVar9 = 1;
            do {
              iVar4 = (&DAT_00ea5b34)[iVar5 * 0x1276];
              (&DAT_00ea5b34)[iVar5 * 0x1276] = iVar4 + 1;
              if (0x23 < iVar4 + 1) {
                (&DAT_00ea5b34)[iVar5 * 0x1276] = 1;
              }
            } while ((*(char *)((int)&DAT_007900c2 + iVar13 + (&DAT_00ea5b34)[iVar5 * 0x1276]) ==
                      '\x01') && (iVar9 = iVar9 + 1, iVar9 < 0x24));
            if (*(char *)((int)&DAT_007900c2 + iVar13 + (&DAT_00ea5b34)[iVar5 * 0x1276]) != '\x01')
            {
              sVar3 = (&DAT_007999ea)[iVar6 * 0x84];
              if (sVar3 == 0) {
                iVar8 = iVar8 + -0x10;
              }
              if (((iVar6 != 0) && (*(int *)(&DAT_007756f0 + sVar3 * 4) != 0)) &&
                 (((short)(&DAT_00799a14)[iVar6 * 0x84] < 400 &&
                  ((int)sVar3 == (&DAT_00ea5b34)[iVar5 * 0x1276])))) {
                iVar8 = iVar8 + -8;
              }
            }
          }
          local_10 = local_10 + -1;
        } while (local_10 != 0);
        if (iVar8 < 0x20) {
          iVar13 = FUN_0045dc50((int)psVar10[-5],(int)psVar10[-4],param_2,param_3,param_5,
                                (int)psVar10[8]);
          if (iVar8 + iVar13 < local_c) {
            local_4 = local_1c;
            local_c = iVar8 + iVar13;
          }
        }
      }
    }
    psVar10 = psVar10 + 0x84;
    local_1c = local_1c + 1;
  } while ((int)psVar10 < 0x89b6e8);
  iVar11 = local_4 * 0x108;
  if ((&DAT_00799a1c)[iVar11] == '\x01') {
    _DAT_00d3b4e4 = (int)(short)(&DAT_007999fe)[local_4 * 0x84];
    _DAT_00d3b4e0 = (int)(short)(&DAT_00799a00)[local_4 * 0x84];
  }
  else {
    iVar11 = FUN_00480180((int)*(short *)((int)&DAT_007999dc + iVar11 + 2),
                          (int)*(short *)(&DAT_007999e0 + iVar11),3);
    if (iVar11 == 0) {
      return 0;
    }
  }
  return local_4;
}
