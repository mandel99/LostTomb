#include "restore_types.h"
#define DAT_0056f7d8 (*(undefined4 *)0x0056f7d8u)
#define DAT_0056f7ec (*(undefined4 *)0x0056f7ecu)
#define DAT_005700f4 (*(undefined4 *)0x005700f4u)
#define DAT_005e4ef4 (*(undefined *)0x005e4ef4u)
#define DAT_005e4ef8 (*(undefined4 *)0x005e4ef8u)
#define DAT_005e6578 (*(undefined4 *)0x005e6578u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999e4 (*(undefined4 *)0x007999e4u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
#define DAT_00bcd609 (*(undefined4 *)0x00bcd609u)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd618 (*(undefined4 *)0x00bcd618u)
#define DAT_00bcd642 (*(undefined4 *)0x00bcd642u)
#define DAT_00bcd646 (*(undefined4 *)0x00bcd646u)
#define DAT_00bcd64a (*(undefined4 *)0x00bcd64au)
#define DAT_00bcd64c (*(undefined4 *)0x00bcd64cu)
#define DAT_00bcd718 (*(undefined4 *)0x00bcd718u)
#define DAT_00d056f8 (*(undefined *)0x00d056f8u)
extern void FUN_004b52f0();
extern void FUN_004b5420();
extern undefined1 FUN_0051cd80();

bool FUN_004f1d10(int param_1,short param_2)

{
  short sVar1;
  short sVar2;
  short sVar3;
  short sVar4;
  bool bVar5;
  bool bVar6;
  undefined1 uVar7;
  int *piVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  int iVar12;
  int iVar13;
  int iVar14;
  int iVar15;
  int local_10;
  uint local_c;
  
  iVar11 = param_1 * 0x184;
  if ((short)(&DAT_00bcd64c)[param_1 * 0xc2] < 1) {
    return true;
  }
  (&DAT_00bcd64a)[param_1 * 0xc2] = (&DAT_00bcd64a)[param_1 * 0xc2] + param_2;
  sVar1 = (&DAT_00bcd64a)[param_1 * 0xc2];
  sVar2 = (&DAT_00bcd64c)[param_1 * 0xc2];
  if (sVar2 <= sVar1) {
    (&DAT_00bcd64a)[param_1 * 0xc2] = sVar2;
  }
  iVar12 = (int)(short)(&DAT_00bcd642)[param_1 * 0xc2];
  iVar14 = (int)*(short *)(&DAT_00bcd646 + iVar11);
  if (iVar12 == 0) {
    (&DAT_00bcd609)[iVar11] = 0;
  }
  else {
    uVar7 = FUN_0051cd80((int)*(short *)((int)&DAT_007999dc + iVar12 * 0x108 + 2),
                         (int)*(short *)(&DAT_007999e0 + iVar12 * 0x108),
                         (int)*(short *)((int)&DAT_007999dc + iVar14 * 0x108 + 2),
                         (int)*(short *)(&DAT_007999e0 + iVar14 * 0x108));
    (&DAT_00bcd609)[iVar11] = uVar7;
  }
  iVar10 = DAT_00bca2e0 / 2;
  if (iVar12 == 0) {
    iVar15 = (int)*(short *)(&DAT_007999e0 + iVar14 * 0x108);
    local_10 = (int)*(short *)((int)&DAT_007999dc + iVar14 * 0x108 + 2);
    iVar14 = DAT_00bca2d8 + iVar15 * 0xe4 + local_10;
    goto LAB_004f20fe;
  }
  if (*(short *)(&DAT_00bcd718 + iVar11) == 4) {
    if (DAT_005e4ef4 == 5) {
      iVar15 = *(int *)(&DAT_0056f7ec + (uint)*(byte *)(&DAT_00799a2c + iVar12 * 0x84) * 4);
    }
    else {
      iVar15 = *(int *)(&DAT_0056f7d8 + (uint)*(byte *)(&DAT_00799a2c + iVar12 * 0x84) * 4);
    }
    iVar13 = (int)*(short *)((int)&DAT_007999dc + iVar12 * 0x108 + 2);
    iVar14 = *(short *)(&DAT_00b4e6fc +
                       (iVar13 + ((*(short *)((int)&DAT_007999dc + iVar14 * 0x108 + 2) - iVar13) *
                                 (int)(short)(&DAT_00bcd64a)[param_1 * 0xc2]) /
                                 (int)(short)(&DAT_00bcd64c)[param_1 * 0xc2] +
                        ((((int)*(short *)(&DAT_007999e0 + iVar14 * 0x108) -
                          (int)*(short *)(&DAT_007999e0 + iVar12 * 0x108)) *
                         (int)(short)(&DAT_00bcd64a)[param_1 * 0xc2]) /
                         (int)(short)(&DAT_00bcd64c)[param_1 * 0xc2] +
                        (int)*(short *)(&DAT_007999e0 + iVar12 * 0x108)) * 0xe4 + DAT_00bca2d8) * 2)
             * 0x108;
    iVar13 = 0;
    sVar3 = *(short *)((int)&DAT_007999dc + iVar14 + 2);
    sVar4 = *(short *)(&DAT_007999e0 + iVar14);
    piVar8 = (int *)(&DAT_005700f4 + iVar10 * 0xc);
    do {
      iVar10 = (int)*(short *)(&DAT_00b4e6fc + (*(int *)(&DAT_007999e4 + iVar14) + *piVar8) * 2);
      if ((iVar12 - (short)(&DAT_007999ea)[iVar12 * 0x84] <= iVar10) &&
         (iVar10 <= iVar15 + -1 + (iVar12 - (short)(&DAT_007999ea)[iVar12 * 0x84]))) {
        sVar3 = *(short *)((int)&DAT_007999dc + iVar10 * 0x108 + 2);
        sVar4 = *(short *)(&DAT_007999e0 + iVar10 * 0x108);
        break;
      }
      iVar13 = iVar13 + 1;
      piVar8 = piVar8 + 1;
    } while (iVar13 < 3);
    local_10 = (int)sVar3;
    iVar15 = (int)sVar4;
    iVar14 = DAT_00bca2d8 + iVar15 * 0xe4 + local_10;
    goto LAB_004f20fe;
  }
  if (DAT_005e4ef4 == 5) {
    local_c = (uint)*(byte *)(&DAT_00799a2c + iVar12 * 0x84);
    iVar15 = *(int *)(&DAT_005e6578 +
                     ((int)(short)(&DAT_007999ea)[iVar12 * 0x84] + local_c * 0x24) * 8);
  }
  else {
    local_c = (uint)*(byte *)(&DAT_00799a2c + iVar12 * 0x84);
    iVar15 = *(int *)(&DAT_005e4ef8 +
                     (local_c * 0x90 + (int)(short)(&DAT_007999ea)[iVar12 * 0x84]) * 8);
  }
  if (iVar15 < 4) {
    iVar13 = iVar15 + iVar10;
    if (3 < iVar15 + iVar10) {
LAB_004f1fcc: ;
      iVar13 = iVar15 + iVar10 + -4;
    }
  }
  else {
    iVar13 = iVar15;
    if ((iVar15 < 8) && (iVar13 = iVar15 + iVar10, 7 < iVar13)) goto LAB_004f1fcc;
  }
  bVar6 = false;
  iVar15 = ((char)(&DAT_00bcd609)[DAT_00d056f8 * 0x184] - DAT_00bca2e0) + 1;
  if (iVar15 < 0) {
    iVar15 = ((char)(&DAT_00bcd609)[DAT_00d056f8 * 0x184] - DAT_00bca2e0) + 9;
  }
  bVar5 = false;
  if ((iVar15 < 2) || (5 < iVar15)) {
    bVar5 = true;
  }
  iVar9 = iVar12;
  switch((int)(iVar13)) {
  case 0: ;
  case 7: ;
    bVar6 = true;
  case 2: ;
  case 4: ;
  case 6: ;
  case 8: ;
  case 9: ;
switchD_004f200d_caseD_2: ;
    iVar9 = iVar12;
    break;
  case 1: ;
  case 3: ;
    iVar9 = iVar14;
    break;
  case 5: ;
    iVar9 = iVar14;
    if (bVar5) goto switchD_004f200d_caseD_2;
  }
  iVar9 = iVar9 * 0x108;
  local_10 = (int)*(short *)((int)&DAT_007999dc + iVar9 + 2);
  iVar15 = (int)*(short *)(&DAT_007999e0 + iVar9);
  iVar14 = *(int *)(&DAT_007999e4 + iVar9);
  if (bVar6) {
    if (DAT_005e4ef4 == 5) {
      iVar14 = *(int *)(&DAT_0056f7ec + local_c * 4);
    }
    else {
      iVar14 = *(int *)(&DAT_0056f7d8 + local_c * 4);
    }
    iVar13 = 0;
    piVar8 = (int *)(&DAT_005700f4 + (iVar10 - 2U & 3) * 0xc);
    do {
      iVar10 = (int)*(short *)(&DAT_00b4e6fc +
                              (*(int *)(&DAT_007999e4 + iVar12 * 0x108) + *piVar8) * 2);
      if ((iVar12 - (short)(&DAT_007999ea)[iVar12 * 0x84] <= iVar10) &&
         (iVar10 <= iVar14 + -1 + (iVar12 - (short)(&DAT_007999ea)[iVar12 * 0x84]))) {
        local_10 = (int)*(short *)((int)&DAT_007999dc + iVar10 * 0x108 + 2);
        iVar15 = (int)*(short *)(&DAT_007999e0 + iVar10 * 0x108);
        break;
      }
      iVar13 = iVar13 + 1;
      piVar8 = piVar8 + 1;
    } while (iVar13 < 3);
    iVar14 = DAT_00bca2d8 + iVar15 * 0xe4 + local_10;
  }
LAB_004f20fe: ;
  if (((short)(&DAT_00bcd60c)[param_1 * 0xc2] == local_10) &&
     ((short)(&DAT_00bcd60e)[param_1 * 0xc2] == iVar15)) {
    bVar6 = false;
  }
  else {
    FUN_004b5420(param_1);
    bVar6 = true;
  }
  (&DAT_00bcd60c)[param_1 * 0xc2] = (short)local_10;
  (&DAT_00bcd60e)[param_1 * 0xc2] = (short)iVar15;
  *(int *)(&DAT_00bcd618 + iVar11) = iVar14;
  if (bVar6) {
    FUN_004b52f0(param_1);
  }
  return sVar2 <= sVar1;
}
