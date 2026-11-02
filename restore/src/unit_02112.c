#include "restore_types.h"
#define DAT_005e4ef4 (*(undefined *)0x005e4ef4u)
#define DAT_005f4b38 (*(undefined4 *)0x005f4b38u)
#define DAT_005f5498 (*(undefined4 *)0x005f5498u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00799a2e (*(undefined4 *)0x00799a2eu)
#define DAT_00799a3c (*(undefined4 *)0x00799a3cu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00bcd5f9 (*(undefined4 *)0x00bcd5f9u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd606 (*(undefined4 *)0x00bcd606u)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd618 (*(undefined4 *)0x00bcd618u)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd620 (*(undefined4 *)0x00bcd620u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd646 (*(undefined4 *)0x00bcd646u)
#define DAT_00bcd648 (*(undefined4 *)0x00bcd648u)
#define DAT_00bcd64a (*(undefined4 *)0x00bcd64au)
#define DAT_00bcd64c (*(undefined4 *)0x00bcd64cu)
#define DAT_00bcd718 (*(undefined4 *)0x00bcd718u)
#define DAT_00d056f8 (*(undefined *)0x00d056f8u)
#define DAT_00eaac44 (*(undefined *)0x00eaac44u)
#define DAT_00eaaf58 (*(undefined *)0x00eaaf58u)
extern void FUN_004b52f0();
extern void FUN_004b5420();
extern undefined4 FUN_004ef100();
extern void FUN_004ef450();
extern void FUN_004f02f0();
extern bool FUN_004f1d10();
extern int FUN_004f2190();

void FUN_004f04d0(int param_1,int param_2,undefined1 param_3,undefined1 param_4)

{
  short sVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  int *piVar5;
  undefined4 uVar6;
  int iVar7;
  int *piVar8;
  int iVar9;
  int local_c;
  int *local_8;
  int local_4;
  
  iVar9 = param_1 * 0x184;
  iVar3 = (int)*(short *)(&DAT_00b4e6fc + *(int *)(&DAT_00bcd620 + iVar9) * 2);
  if ((&DAT_007999e8)[iVar3 * 0x84] != 0xb7) {
    (&DAT_00bcd606)[DAT_00d056f8 * 0x184] = 2;
    return;
  }
  cVar2 = FUN_004f1d10(param_1,1);
  if (cVar2 != '\x01') {
    return;
  }
  if (DAT_005e4ef4 == 5) {
    local_c = 0x10;
    piVar8 = (int *)(&DAT_005f5498 + (uint)*(byte *)(&DAT_00799a2c + iVar3 * 0x84) * 0x80);
  }
  else {
    local_c = 0x3c;
    piVar8 = (int *)(&DAT_005f4b38 + (uint)*(byte *)(&DAT_00799a2c + iVar3 * 0x84) * 0x1e0);
  }
  FUN_004b5420(param_1);
  (&DAT_00bcd60c)[param_1 * 0xc2] = *(undefined2 *)(&DAT_00bcd61c + iVar9);
  (&DAT_00bcd60e)[param_1 * 0xc2] = *(undefined2 *)(&DAT_00bcd61e + iVar9);
  *(undefined4 *)(&DAT_00bcd618 + iVar9) = *(undefined4 *)(&DAT_00bcd620 + iVar9);
  FUN_004b52f0(param_1);
  if (param_2 == 1) {
    if ((&DAT_00799a3c)[iVar3 * 0x84] == (short)param_1) {
      iVar3 = FUN_004ef100(iVar3);
      if (iVar3 != 0) goto LAB_004f06d6;
    }
    else {
      sVar1 = (&DAT_007999ea)[iVar3 * 0x84];
      local_4 = 0;
      local_8 = piVar8;
      if (local_c != 0) {
        do {
          iVar4 = (iVar3 - sVar1) + *local_8;
          if (((&DAT_00799a3c)[iVar4 * 0x84] == (short)param_1) &&
             (iVar4 = FUN_004ef100(iVar4), iVar4 != 0)) goto LAB_004f0671;
          local_4 = local_4 + 1;
          local_8 = local_8 + 2;
        } while (local_4 < local_c);
      }
    }
  }
  else {
LAB_004f0671: ;
    if (*(short *)(&DAT_00bcd718 + iVar9) == 1) {
      iVar4 = 0;
      FUN_004ef450(iVar3 - (short)(&DAT_007999ea)[iVar3 * 0x84]);
      if (0 < DAT_00eaaf58) {
        if ((int)*(short *)(&DAT_00bcd648 + iVar9) <
            (int)((uint)*(byte *)(&DAT_00799a2e + DAT_00eaac44 * 0x84) +
                 (uint)*(byte *)((int)&DAT_00799a2e + DAT_00eaac44 * 0x108 + 1) * 6)) {
LAB_004f083f: ;
          iVar3 = (piVar8[iVar4 * 2] - (int)(short)(&DAT_007999ea)[iVar3 * 0x84]) + iVar3;
          if ((param_2 == 2) && (iVar7 = FUN_004ef100(iVar3), iVar7 != 0)) {
            (&DAT_00bcd5f9)[iVar9] = 0;
            return;
          }
          sVar1 = *(short *)(&DAT_00bcd646 + iVar9);
          FUN_004f02f0(param_1,iVar3,piVar8[iVar4 * 2 + 1],1);
          if (iVar3 != sVar1) {
            return;
          }
          (&DAT_00bcd64a)[param_1 * 0xc2] = (&DAT_00bcd64c)[param_1 * 0xc2];
          return;
        }
LAB_004f06d6: ;
        *(undefined2 *)(&DAT_00bcd718 + iVar9) = 4;
        (&DAT_00bcd630)[iVar9] = param_4;
        return;
      }
    }
    else {
      iVar4 = 0;
      if ((param_2 == 0) || (param_2 == 2)) {
        FUN_004ef450(iVar3 - (short)(&DAT_007999ea)[iVar3 * 0x84]);
        if (DAT_00eaaf58 < 1) {
          (&DAT_00bcd630)[iVar9] = param_3;
          *(undefined2 *)(&DAT_00bcd718 + iVar9) = 0;
          return;
        }
        local_4 = DAT_00eaac44;
        if ((int)((uint)*(byte *)(&DAT_00799a2e + DAT_00eaac44 * 0x84) +
                 (uint)*(byte *)((int)&DAT_00799a2e + DAT_00eaac44 * 0x108 + 1) * 6) <=
            (int)*(short *)(&DAT_00bcd648 + iVar9)) goto LAB_004f06d6;
      }
      else {
        local_4 = 0;
      }
      piVar5 = piVar8;
      if (local_c != 0) {
        do {
          if ((*piVar5 == (int)(short)(&DAT_007999ea)[iVar3 * 0x84]) &&
             (piVar5[1] == (int)*(short *)(&DAT_00bcd648 + iVar9))) {
            iVar4 = iVar4 + 1;
            if (-1 < iVar4) {
              if (-1 < piVar8[iVar4 * 2]) goto LAB_004f083f;
              if (param_2 == 1) {
                (&DAT_00bcd630)[iVar9] = param_3;
                *(undefined2 *)(&DAT_00bcd718 + iVar9) = 0;
                return;
              }
              if (((param_2 == 0) && ((&DAT_00bcd602)[iVar9] == 'Q')) &&
                 (*(char *)((int)&DAT_00799a2c + local_4 * 0x108 + 1) == '\x01')) {
                (&DAT_00bcd630)[iVar9] = 0x10;
                uVar6 = FUN_004f2190((int)*(short *)(&DAT_00bcd646 + iVar9));
                FUN_004f02f0(param_1,uVar6,(int)*(short *)(&DAT_00bcd648 + iVar9),0);
                return;
              }
              goto LAB_004f06d6;
            }
            break;
          }
          iVar4 = iVar4 + 1;
          piVar5 = piVar5 + 2;
        } while (iVar4 < local_c);
      }
    }
  }
  (&DAT_00bcd630)[iVar9] = param_3;
  *(undefined2 *)(&DAT_00bcd718 + iVar9) = 0;
  return;
}
