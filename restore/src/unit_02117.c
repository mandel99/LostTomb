#include "restore_types.h"
#define DAT_005e4ef4 (*(undefined *)0x005e4ef4u)
#define DAT_005f4b38 (*(undefined4 *)0x005f4b38u)
#define DAT_005f5498 (*(undefined4 *)0x005f5498u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00799a2e (*(undefined4 *)0x00799a2eu)
#define DAT_00799a3c (*(undefined4 *)0x00799a3cu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd618 (*(undefined4 *)0x00bcd618u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd644 (*(undefined4 *)0x00bcd644u)
#define DAT_00bcd648 (*(undefined4 *)0x00bcd648u)
#define DAT_00bcd718 (*(undefined4 *)0x00bcd718u)
#define DAT_00eaac44 (*(undefined *)0x00eaac44u)
#define DAT_00eaaf58 (*(undefined *)0x00eaaf58u)
extern uint FUN_004eea10();
extern void FUN_004ef450();
extern int FUN_004efd90();
extern void FUN_004f02f0();
extern int FUN_004f27b0();
extern uint FUN_004f28e0();

void FUN_004f0ef0(int param_1,char param_2,undefined1 param_3,undefined1 param_4,undefined1 param_5)

{
  short sVar1;
  bool bVar2;
  bool bVar3;
  bool bVar4;
  char cVar5;
  int *piVar6;
  uint uVar7;
  int iVar8;
  int iVar9;
  uint uVar10;
  int iVar11;
  int iVar12;
  int iVar13;
  int iVar14;
  undefined1 local_1d;
  int local_1c;
  int local_18;
  int *local_14;
  int local_10;
  int local_c;
  int local_8;
  int local_4;
  
  iVar11 = param_1 * 0x184;
  iVar12 = (int)*(short *)(&DAT_00b4e6fc + *(int *)(&DAT_00bcd618 + iVar11) * 2);
  if ((&DAT_007999e8)[iVar12 * 0x84] == 0xb7) {
    iVar12 = iVar12 - (short)(&DAT_007999ea)[iVar12 * 0x84];
    local_1c = iVar11;
    FUN_004ef450(iVar12);
    iVar8 = DAT_00eaac44;
    if (DAT_00eaaf58 < 1) {
      (&DAT_00bcd630)[iVar11] = param_3;
      *(undefined2 *)(&DAT_00bcd718 + iVar11) = 0;
      return;
    }
    iVar14 = DAT_00eaac44 - (short)(&DAT_007999ea)[DAT_00eaac44 * 0x84];
    iVar9 = (uint)*(byte *)(&DAT_00799a2e + DAT_00eaac44 * 0x84) +
            (uint)*(byte *)((int)&DAT_00799a2e + DAT_00eaac44 * 0x108 + 1) * 6;
    if (param_2 != '\0') {
      *(short *)(&DAT_00bcd648 + iVar11) = (short)iVar9;
    }
    if (iVar9 == *(short *)(&DAT_00bcd648 + iVar11)) {
      iVar11 = 0;
      local_8 = 0;
      bVar2 = false;
      bVar4 = false;
      if (DAT_005e4ef4 == 5) {
        iVar13 = 0x10;
        piVar6 = (int *)(&DAT_005f5498 + (uint)*(byte *)(&DAT_00799a2c + iVar12 * 0x84) * 0x80);
      }
      else {
        iVar13 = 0x3c;
        piVar6 = (int *)(&DAT_005f4b38 + (uint)*(byte *)(&DAT_00799a2c + iVar12 * 0x84) * 0x1e0);
      }
      iVar12 = 0;
      if (iVar13 != 0) {
        do {
          iVar8 = iVar14 + *piVar6;
          if (iVar9 <= piVar6[1]) break;
          iVar12 = iVar12 + 1;
          piVar6 = piVar6 + 2;
        } while (iVar12 < iVar13);
      }
      local_18 = 0;
      local_4 = 0;
      local_10 = (int)*(short *)((int)&DAT_007999dc + iVar8 * 0x108 + 2);
      local_c = (int)*(short *)(&DAT_007999e0 + iVar8 * 0x108);
      if (0 < DAT_00eaaf58) {
        local_14 = &DAT_00eaac44;
        do {
          iVar12 = *local_14;
          cVar5 = FUN_004efd90(iVar12);
          switch((int)(DAT_005e4ef4)) {
          case 1: ;
          case 3: ;
          case 4: ;
          case 5: ;
switchD_004f10a3_caseD_1: ;
            uVar7 = local_10 - *(short *)((int)&DAT_007999dc + iVar12 * 0x108 + 2);
            uVar10 = (int)uVar7 >> 0x1f;
            iVar9 = (uVar7 ^ uVar10) - uVar10;
            uVar7 = local_c - *(short *)(&DAT_007999e0 + iVar12 * 0x108) >> 0x1f;
            iVar8 = (local_c - *(short *)(&DAT_007999e0 + iVar12 * 0x108) ^ uVar7) - uVar7;
            iVar8 = iVar8 * iVar8 + iVar9 * iVar9;
            iVar9 = FUN_004f27b0(iVar12,&local_1d);
            if (iVar9 < 1) {
              bVar3 = false;
            }
            else {
              uVar7 = local_10 - *(short *)((int)&DAT_007999dc + iVar9 * 0x108 + 2);
              uVar10 = (int)uVar7 >> 0x1f;
              iVar14 = (uVar7 ^ uVar10) - uVar10;
              uVar7 = local_c - *(short *)(&DAT_007999e0 + iVar9 * 0x108) >> 0x1f;
              iVar11 = (local_c - *(short *)(&DAT_007999e0 + iVar9 * 0x108) ^ uVar7) - uVar7;
              iVar9 = iVar14 * iVar14 + iVar11 * iVar11;
              if (iVar9 < iVar8) {
                iVar9 = iVar8;
              }
              bVar3 = true;
              iVar11 = local_18;
              iVar8 = iVar9;
            }
            iVar8 = iVar8 * 2;
            iVar9 = FUN_004f28e0(iVar12);
            if (iVar11 != 0) {
              sVar1 = (&DAT_00799a3c)[iVar11 * 0x84];
              if (sVar1 < 1) {
LAB_004f11ca: ;
                if (((&DAT_00799a3c)[iVar12 * 0x84] != 0) &&
                   ((short)(&DAT_00799a3c)[iVar12 * 0x84] != param_1)) break;
              }
              else {
                if ((sVar1 != param_1) &&
                   (((&DAT_00799a3c)[iVar12 * 0x84] == 0 ||
                    ((short)(&DAT_00799a3c)[iVar12 * 0x84] == param_1)))) goto LAB_004f1207;
                iVar11 = local_18;
                if ((sVar1 < 1) || (sVar1 == param_1)) goto LAB_004f11ca;
              }
              if ((iVar8 <= local_8) &&
                 ((iVar8 != local_8 || (((0 >= iVar9 || (bVar2)) && ((!bVar3 || (bVar4))))))))
              break;
            }
LAB_004f1207: ;
            iVar11 = iVar12;
            local_18 = iVar12;
            local_8 = iVar8;
            bVar2 = 0 < iVar9;
            bVar4 = bVar3;
            break;
          case 2: ;
            if ((&DAT_00bcd602)[local_1c] == 'P') {
              if (cVar5 != '\0') goto switchD_004f10a3_caseD_1;
            }
            else if (cVar5 == '\0') goto switchD_004f10a3_caseD_1;
          }
          local_4 = local_4 + 1;
          local_14 = local_14 + 1;
        } while (local_4 < DAT_00eaaf58);
        if (iVar11 != 0) {
          if ((((&DAT_00799a3c)[iVar11 * 0x84] == 0) ||
              ((short)(&DAT_00799a3c)[iVar11 * 0x84] == param_1)) ||
             (cVar5 = FUN_004eea10(iVar11), cVar5 == '\0')) {
            if (*(char *)((int)&DAT_00799a2c + iVar11 * 0x108 + 1) != '\x01') {
              (&DAT_00799a3c)[iVar11 * 0x84] = (short)param_1;
              *(undefined2 *)(&DAT_00bcd718 + local_1c) = 4;
              (&DAT_00bcd630)[local_1c] = param_5;
              FUN_004f02f0(param_1,iVar11,(int)*(short *)((int)&DAT_00bcd644 + local_1c),0);
              return;
            }
            (&DAT_00799a3c)[iVar11 * 0x84] = 0;
            (&DAT_00bcd630)[local_1c] = param_4;
            return;
          }
        }
      }
      (&DAT_00bcd630)[local_1c] = param_3;
      *(undefined2 *)(&DAT_00bcd718 + local_1c) = 0;
      return;
    }
  }
  (&DAT_00bcd630)[iVar11] = param_3;
  *(undefined2 *)(&DAT_00bcd718 + iVar11) = 0;
  return;
}
