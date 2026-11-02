#include "restore_types.h"
#define DAT_005cc8c0 (*(undefined *)0x005cc8c0u)
#define DAT_005cc8c8 (*(undefined *)0x005cc8c8u)
#define DAT_005cc8d0 (*(undefined *)0x005cc8d0u)
#define DAT_005cc8d8 (*(undefined *)0x005cc8d8u)
#define DAT_005cc8e0 (*(undefined *)0x005cc8e0u)
#define DAT_005cc8f0 (*(undefined *)0x005cc8f0u)
#define DAT_005cc8f8 (*(undefined *)0x005cc8f8u)
#define DAT_005cc900 (*(undefined *)0x005cc900u)
#define DAT_005cc908 (*(undefined *)0x005cc908u)
#define DAT_005cc910 (*(undefined *)0x005cc910u)
#define DAT_005cc920 (*(undefined *)0x005cc920u)
#define DAT_005cc928 (*(undefined *)0x005cc928u)
#define DAT_005cc930 (*(undefined *)0x005cc930u)
#define DAT_005cc938 (*(undefined *)0x005cc938u)
#define DAT_005cc940 (*(undefined *)0x005cc940u)
#define DAT_005cc950 (*(undefined *)0x005cc950u)
#define DAT_005cc958 (*(undefined *)0x005cc958u)
#define DAT_005cc960 (*(undefined *)0x005cc960u)
#define DAT_005cc968 (*(undefined *)0x005cc968u)
#define DAT_005cc970 (*(undefined *)0x005cc970u)
#define DAT_005cc980 (*(undefined *)0x005cc980u)
#define DAT_005cc988 (*(undefined *)0x005cc988u)
#define DAT_005cc990 (*(undefined *)0x005cc990u)
#define DAT_005cc998 (*(undefined *)0x005cc998u)
#define DAT_005cc9a0 (*(undefined *)0x005cc9a0u)
#define DAT_007999db (*(undefined4 *)0x007999dbu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a30 (*(undefined4 *)0x00799a30u)
#define DAT_00799a40 (*(undefined4 *)0x00799a40u)
#define DAT_00799a68 (*(undefined4 *)0x00799a68u)
#define DAT_00acf85c (*(undefined4 *)0x00acf85cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b285cc (*(undefined4 *)0x00b285ccu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00b67d1c (*(undefined4 *)0x00b67d1cu)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00e94e0c (*(undefined *)0x00e94e0cu)
#define DAT_00f56b54 (*(undefined *)0x00f56b54u)
#define DAT_00f572ec (*(undefined *)0x00f572ecu)
extern int FUN_00475fd0();
extern void FUN_0047bf50();
extern undefined4 FUN_00547ba0();

void FUN_005468d0(int param_1)

{
  byte bVar1;
  short sVar2;
  uint uVar3;
  int *in_ECX;
  int iVar4;
  int iVar5;
  uint uVar6;
  int *piVar7;
  int iVar8;
  int iVar9;
  int *piVar10;
  int *local_10;
  uint local_c;
  int local_4;
  
  iVar8 = param_1 * 0x108;
  sVar2 = (&DAT_007999ea)[param_1 * 0x84];
  if (sVar2 == 0) {
    uVar3 = FUN_00547ba0(param_1);
    in_ECX[0x15] = uVar3 & 0xff;
  }
  uVar3 = (uint)(byte)(&DAT_007999db)[iVar8];
  if (((&DAT_00799a30)[(param_1 - sVar2) * 0x42] == 1) && (DAT_00e94e0c != 0x33)) {
    iVar8 = DAT_00bca2d8 + *(short *)((int)&DAT_007999dc + iVar8 + 2) +
            *(short *)(&DAT_007999e0 + iVar8) * 0xe4;
    if ((int)(short)(&DAT_007999ea)[param_1 * 0x84] == in_ECX[0x14]) {
      iVar8 = iVar8 + DAT_005cc8c0;
      (&DAT_00b285cc)[iVar8] = (&DAT_00b285cc)[iVar8] & 0xe0;
      iVar5 = ((byte)(&DAT_00acf85c)[iVar8] & 7) + DAT_00f56b54;
      (&DAT_00b67d1c)[iVar8] = (&DAT_00b67d1c)[iVar8] | 0x40;
      (&DAT_00b7482c)[iVar8] = iVar5;
      return;
    }
    if (uVar3 == 0) {
      return;
    }
    local_10 = &DAT_005cc8c0;
    uVar6 = uVar3;
    piVar7 = local_10;
    local_c = uVar3;
    do {
      do {
        iVar9 = *piVar7 + iVar8;
        iVar5 = FUN_00475fd0(iVar9);
        (&DAT_00b285cc)[iVar9] = (&DAT_00b285cc)[iVar9] & 0xe0;
        iVar5 = DAT_00f572ec + iVar5;
        uVar6 = uVar6 - 1;
        (&DAT_00b67d1c)[iVar9] = (&DAT_00b67d1c)[iVar9] | 0x40;
        (&DAT_00b7482c)[iVar9] = iVar5;
        piVar7 = piVar7 + 2;
      } while (uVar6 != 0);
      piVar7 = local_10 + 0xc;
      local_c = local_c - 1;
      uVar6 = uVar3;
      local_10 = piVar7;
    } while (local_c != 0);
    return;
  }
  iVar5 = (**(code **)(*in_ECX + 0x1c))(param_1,&local_4);
  if ((local_c & 1) != 0) {
    sVar2 = *(short *)(&DAT_007999e0 + iVar8);
    iVar8 = DAT_00bca2d8 + *(short *)((int)&DAT_007999dc + iVar8 + 2);
    if (local_4 < 1) {
      return;
    }
    local_10 = (int *)local_4;
    piVar10 = &DAT_005cc8c0;
    iVar9 = local_4;
    piVar7 = piVar10;
    do {
      do {
        iVar4 = iVar8 + sVar2 * 0xe4 + *piVar10;
        bVar1 = (&DAT_00b285cc)[iVar4];
        (&DAT_00af598c)[iVar4] = 8;
        *(short *)(&DAT_00b4e6fc + iVar4 * 2) = (short)param_1;
        (&DAT_00b285cc)[iVar4] = bVar1 & 0xe0;
        (&DAT_00b67d1c)[iVar4] = (&DAT_00b67d1c)[iVar4] | 0x40;
        if (iVar5 != 0) {
          if (iVar5 == DAT_00f56b54) {
            (&DAT_00b7482c)[iVar4] = ((byte)(&DAT_00acf85c)[iVar4] & 7) + iVar5;
          }
          else {
            (&DAT_00b7482c)[iVar4] = iVar5;
          }
        }
        piVar10 = piVar10 + 2;
        iVar9 = iVar9 + -1;
      } while (iVar9 != 0);
      piVar10 = piVar7 + 0xc;
      local_10 = (int *)((int)local_10 + -1);
      iVar9 = local_4;
      piVar7 = piVar10;
    } while (local_10 != (int *)0x0);
    return;
  }
  FUN_0047bf50(param_1,(int)*(short *)((int)&DAT_007999dc + iVar8 + 2),
               (int)*(short *)(&DAT_007999e0 + iVar8),local_4,local_4,iVar5,0x40000000);
  iVar4 = DAT_005cc928;
  iVar9 = DAT_005cc900;
  iVar5 = DAT_00bca2d8 + *(short *)((int)&DAT_007999dc + iVar8 + 2) +
          *(short *)(&DAT_007999e0 + iVar8) * 0xe4;
  switch(*(undefined4 *)(&DAT_00799a40 + param_1 * 0x84)) {
  case 1: ;
  case 0x1d: ;
    switch(*(undefined4 *)(&DAT_00799a68 + iVar8)) {
    case 0: ;
      iVar8 = DAT_005cc8f8 + iVar5;
      (&DAT_00af598c)[DAT_005cc8c8 + iVar5] = 8;
      (&DAT_00af598c)[iVar8] = 8;
      return;
    case 1: ;
      iVar8 = DAT_005cc8f8 + iVar5;
      (&DAT_00af598c)[DAT_005cc8f0 + iVar5] = 8;
      (&DAT_00af598c)[iVar8] = 8;
      return;
    case 2: ;
      iVar8 = DAT_005cc8f0 + iVar5;
      (&DAT_00af598c)[DAT_005cc8c0 + iVar5] = 8;
      (&DAT_00af598c)[iVar8] = 8;
      return;
    case 3: ;
      iVar8 = DAT_005cc8c8 + iVar5;
      (&DAT_00af598c)[DAT_005cc8c0 + iVar5] = 8;
      (&DAT_00af598c)[iVar8] = 8;
      return;
    }
    break;
  case 2: ;
    switch(*(undefined4 *)(&DAT_00799a68 + iVar8)) {
    case 0: ;
      iVar9 = DAT_005cc8c8 + iVar5;
      (&DAT_00af598c)[DAT_005cc8c0 + iVar5] = 8;
      iVar8 = DAT_005cc8d0;
      (&DAT_00af598c)[iVar9] = 8;
      iVar9 = DAT_005cc8f0 + iVar5;
      (&DAT_00af598c)[iVar8 + iVar5] = 8;
      iVar8 = DAT_005cc8f8;
      (&DAT_00af598c)[iVar9] = 8;
      iVar9 = DAT_005cc900 + iVar5;
      (&DAT_00af598c)[iVar8 + iVar5] = 8;
      (&DAT_00af598c)[iVar9] = 8;
      return;
    case 1: ;
      iVar8 = DAT_005cc8d0 + iVar5;
      (&DAT_00af598c)[DAT_005cc8c8 + iVar5] = 8;
      iVar9 = DAT_005cc8f8;
      (&DAT_00af598c)[iVar8] = 8;
      iVar8 = DAT_005cc900 + iVar5;
      (&DAT_00af598c)[iVar9 + iVar5] = 8;
      break;
    case 2: ;
      iVar8 = DAT_005cc8f8 + iVar5;
      (&DAT_00af598c)[DAT_005cc8f0 + iVar5] = 8;
      iVar9 = DAT_005cc900;
      (&DAT_00af598c)[iVar8] = 8;
      iVar8 = DAT_005cc920 + iVar5;
      (&DAT_00af598c)[iVar9 + iVar5] = 8;
      break;
    case 3: ;
      iVar9 = DAT_005cc8c8 + iVar5;
      (&DAT_00af598c)[DAT_005cc8c0 + iVar5] = 8;
      iVar8 = DAT_005cc8f0;
      (&DAT_00af598c)[iVar9] = 8;
      iVar9 = DAT_005cc8f8 + iVar5;
      (&DAT_00af598c)[iVar8 + iVar5] = 8;
      iVar8 = DAT_005cc920;
      (&DAT_00af598c)[iVar9] = 8;
      iVar9 = DAT_005cc928 + iVar5;
      (&DAT_00af598c)[iVar8 + iVar5] = 8;
      (&DAT_00af598c)[iVar9] = 8;
      return;
default: ;
      goto switchD_00546b89_caseD_3;
    }
LAB_00546f22: ;
    iVar9 = DAT_005cc928;
    (&DAT_00af598c)[iVar8] = 8;
    iVar8 = DAT_005cc930 + iVar5;
    (&DAT_00af598c)[iVar9 + iVar5] = 8;
    (&DAT_00af598c)[iVar8] = 8;
    return;
  case 9: ;
  case 10: ;
  case 0xd: ;
  case 0xe: ;
    switch(*(undefined4 *)(&DAT_00799a68 + iVar8)) {
    case 0: ;
      iVar8 = DAT_005cc8c8 + iVar5;
      (&DAT_00af598c)[DAT_005cc8c0 + iVar5] = 8;
      iVar5 = DAT_005cc8d0 + iVar5;
      (&DAT_00af598c)[iVar8] = 8;
      (&DAT_00af598c)[iVar5] = 8;
      return;
    case 1: ;
      iVar8 = DAT_005cc900 + iVar5;
      (&DAT_00af598c)[DAT_005cc8d0 + iVar5] = 8;
      iVar5 = DAT_005cc930 + iVar5;
      (&DAT_00af598c)[iVar8] = 8;
      (&DAT_00af598c)[iVar5] = 8;
      return;
    case 2: ;
      (&DAT_00af598c)[DAT_005cc920 + iVar5] = 8;
      iVar9 = iVar4;
LAB_00546ef3: ;
      iVar8 = DAT_005cc930 + iVar5;
      (&DAT_00af598c)[iVar9 + iVar5] = 8;
      (&DAT_00af598c)[iVar8] = 8;
      return;
    case 3: ;
      iVar8 = DAT_005cc8f0 + iVar5;
      (&DAT_00af598c)[DAT_005cc8c0 + iVar5] = 8;
      iVar5 = DAT_005cc920 + iVar5;
      (&DAT_00af598c)[iVar8] = 8;
      (&DAT_00af598c)[iVar5] = 8;
      return;
    }
    break;
  case 0xb: ;
    switch(*(undefined4 *)(&DAT_00799a68 + iVar8)) {
    case 0: ;
      (&DAT_00af598c)[DAT_005cc8d0 + iVar5] = 8;
      goto LAB_00546ef3;
    case 1: ;
      iVar8 = DAT_005cc920 + iVar5;
      goto LAB_00546f22;
    case 2: ;
      iVar8 = DAT_005cc8f0 + iVar5;
      (&DAT_00af598c)[DAT_005cc8c0 + iVar5] = 8;
      iVar5 = DAT_005cc920 + iVar5;
      (&DAT_00af598c)[iVar8] = 8;
      (&DAT_00af598c)[iVar5] = 8;
      return;
    case 3: ;
      iVar8 = DAT_005cc8c8 + iVar5;
      (&DAT_00af598c)[DAT_005cc8c0 + iVar5] = 8;
      iVar5 = DAT_005cc8d0 + iVar5;
      (&DAT_00af598c)[iVar8] = 8;
      (&DAT_00af598c)[iVar5] = 8;
      return;
    }
    break;
  case 0x13: ;
    switch(*(undefined4 *)(&DAT_00799a68 + iVar8)) {
    case 0: ;
      iVar8 = DAT_005cc8e0 + iVar5;
      (&DAT_00af598c)[DAT_005cc8d8 + iVar5] = 8;
      iVar9 = DAT_005cc908 + iVar5;
      (&DAT_00af598c)[iVar8] = 8;
      iVar8 = DAT_005cc910;
      (&DAT_00af598c)[iVar9] = 8;
      iVar9 = DAT_005cc938 + iVar5;
      (&DAT_00af598c)[iVar8 + iVar5] = 8;
      iVar8 = DAT_005cc940;
      (&DAT_00af598c)[iVar9] = 8;
      iVar9 = DAT_005cc968 + iVar5;
      (&DAT_00af598c)[iVar8 + iVar5] = 8;
      iVar8 = DAT_005cc970;
      (&DAT_00af598c)[iVar9] = 8;
      goto LAB_00547639;
    case 1: ;
switchD_00547474_caseD_2: ;
      iVar8 = DAT_005cc958 + iVar5;
      (&DAT_00af598c)[DAT_005cc950 + iVar5] = 8;
      iVar9 = DAT_005cc960 + iVar5;
      (&DAT_00af598c)[iVar8] = 8;
      iVar8 = DAT_005cc968;
      (&DAT_00af598c)[iVar9] = 8;
      iVar9 = DAT_005cc970 + iVar5;
      (&DAT_00af598c)[iVar8 + iVar5] = 8;
      iVar8 = DAT_005cc980;
      (&DAT_00af598c)[iVar9] = 8;
      iVar9 = DAT_005cc988 + iVar5;
      (&DAT_00af598c)[iVar8 + iVar5] = 8;
      iVar8 = DAT_005cc990;
      (&DAT_00af598c)[iVar9] = 8;
LAB_00547639: ;
      iVar9 = DAT_005cc998 + iVar5;
      (&DAT_00af598c)[iVar8 + iVar5] = 8;
      iVar5 = DAT_005cc9a0 + iVar5;
      (&DAT_00af598c)[iVar9] = 8;
      (&DAT_00af598c)[iVar5] = 8;
      return;
    case 2: ;
switchD_00547474_caseD_3: ;
      iVar9 = DAT_005cc8f0 + iVar5;
      (&DAT_00af598c)[DAT_005cc8c0 + iVar5] = 8;
      iVar8 = DAT_005cc920;
      (&DAT_00af598c)[iVar9] = 8;
      iVar9 = DAT_005cc950 + iVar5;
      (&DAT_00af598c)[iVar8 + iVar5] = 8;
      iVar8 = DAT_005cc980;
      (&DAT_00af598c)[iVar9] = 8;
      iVar9 = DAT_005cc8c8 + iVar5;
      (&DAT_00af598c)[iVar8 + iVar5] = 8;
      iVar8 = DAT_005cc8f8;
      (&DAT_00af598c)[iVar9] = 8;
      iVar9 = DAT_005cc928 + iVar5;
      (&DAT_00af598c)[iVar8 + iVar5] = 8;
      iVar8 = DAT_005cc958;
      (&DAT_00af598c)[iVar9] = 8;
      iVar9 = DAT_005cc988 + iVar5;
      (&DAT_00af598c)[iVar8 + iVar5] = 8;
      (&DAT_00af598c)[iVar9] = 8;
      break;
    case 3: ;
switchD_00547474_caseD_0: ;
      iVar9 = DAT_005cc8c8 + iVar5;
      (&DAT_00af598c)[DAT_005cc8c0 + iVar5] = 8;
      iVar8 = DAT_005cc8d0;
      (&DAT_00af598c)[iVar9] = 8;
      iVar9 = DAT_005cc8d8 + iVar5;
      (&DAT_00af598c)[iVar8 + iVar5] = 8;
      iVar8 = DAT_005cc8e0;
      (&DAT_00af598c)[iVar9] = 8;
      iVar9 = DAT_005cc8f0 + iVar5;
      (&DAT_00af598c)[iVar8 + iVar5] = 8;
      iVar8 = DAT_005cc8f8;
      (&DAT_00af598c)[iVar9] = 8;
      iVar9 = DAT_005cc900 + iVar5;
      (&DAT_00af598c)[iVar8 + iVar5] = 8;
      iVar8 = DAT_005cc908;
      (&DAT_00af598c)[iVar9] = 8;
      iVar9 = DAT_005cc910 + iVar5;
      (&DAT_00af598c)[iVar8 + iVar5] = 8;
      (&DAT_00af598c)[iVar9] = 8;
      return;
default: ;
      break;
    }
    break;
  case 0x16: ;
    switch(*(undefined4 *)(&DAT_00799a68 + iVar8)) {
    case 0: ;
      iVar8 = DAT_005cc8c8 + iVar5;
      (&DAT_00af598c)[DAT_005cc8c0 + iVar5] = 8;
      iVar9 = DAT_005cc8d0 + iVar5;
      (&DAT_00af598c)[iVar8] = 8;
      iVar8 = DAT_005cc8d8;
      (&DAT_00af598c)[iVar9] = 8;
      iVar9 = DAT_005cc8e0 + iVar5;
      (&DAT_00af598c)[iVar8 + iVar5] = 8;
      iVar8 = DAT_005cc8f0;
      (&DAT_00af598c)[iVar9] = 8;
      iVar9 = DAT_005cc8f8 + iVar5;
      (&DAT_00af598c)[iVar8 + iVar5] = 8;
      iVar8 = DAT_005cc900;
      (&DAT_00af598c)[iVar9] = 8;
      iVar9 = DAT_005cc908 + iVar5;
      (&DAT_00af598c)[iVar8 + iVar5] = 8;
      iVar8 = DAT_005cc910;
      (&DAT_00af598c)[iVar9] = 8;
      iVar9 = DAT_005cc920 + iVar5;
      (&DAT_00af598c)[iVar8 + iVar5] = 8;
      iVar8 = DAT_005cc928;
      (&DAT_00af598c)[iVar9] = 8;
      iVar9 = DAT_005cc930 + iVar5;
      (&DAT_00af598c)[iVar8 + iVar5] = 8;
      iVar8 = DAT_005cc938;
      (&DAT_00af598c)[iVar9] = 8;
      iVar9 = DAT_005cc940 + iVar5;
      (&DAT_00af598c)[iVar8 + iVar5] = 8;
      (&DAT_00af598c)[iVar9] = 8;
      return;
    case 1: ;
      iVar9 = DAT_005cc900 + iVar5;
      (&DAT_00af598c)[DAT_005cc8d0 + iVar5] = 8;
      iVar8 = DAT_005cc930;
      (&DAT_00af598c)[iVar9] = 8;
      iVar4 = DAT_005cc960 + iVar5;
      (&DAT_00af598c)[iVar8 + iVar5] = 8;
      iVar9 = DAT_005cc990;
      (&DAT_00af598c)[iVar4] = 8;
      iVar8 = DAT_005cc8d8;
      (&DAT_00af598c)[iVar9 + iVar5] = 8;
LAB_0054752d: ;
      iVar9 = DAT_005cc908;
      (&DAT_00af598c)[iVar8 + iVar5] = 8;
      iVar8 = DAT_005cc938 + iVar5;
      (&DAT_00af598c)[iVar9 + iVar5] = 8;
      iVar9 = DAT_005cc968 + iVar5;
      (&DAT_00af598c)[iVar8] = 8;
      iVar8 = DAT_005cc998;
      (&DAT_00af598c)[iVar9] = 8;
      iVar9 = DAT_005cc8e0 + iVar5;
      (&DAT_00af598c)[iVar8 + iVar5] = 8;
      iVar8 = DAT_005cc910;
      (&DAT_00af598c)[iVar9] = 8;
      iVar9 = DAT_005cc940 + iVar5;
      (&DAT_00af598c)[iVar8 + iVar5] = 8;
      iVar8 = DAT_005cc970;
      (&DAT_00af598c)[iVar9] = 8;
      iVar9 = DAT_005cc9a0 + iVar5;
      (&DAT_00af598c)[iVar8 + iVar5] = 8;
      (&DAT_00af598c)[iVar9] = 8;
      return;
    case 2: ;
      iVar8 = DAT_005cc928 + iVar5;
      (&DAT_00af598c)[DAT_005cc920 + iVar5] = 8;
      iVar9 = DAT_005cc930 + iVar5;
      (&DAT_00af598c)[iVar8] = 8;
      iVar8 = DAT_005cc938;
      (&DAT_00af598c)[iVar9] = 8;
      iVar9 = DAT_005cc940 + iVar5;
      (&DAT_00af598c)[iVar8 + iVar5] = 8;
      iVar8 = DAT_005cc950;
      (&DAT_00af598c)[iVar9] = 8;
      iVar9 = DAT_005cc958 + iVar5;
      (&DAT_00af598c)[iVar8 + iVar5] = 8;
      iVar8 = DAT_005cc960;
      (&DAT_00af598c)[iVar9] = 8;
      iVar9 = DAT_005cc968 + iVar5;
      (&DAT_00af598c)[iVar8 + iVar5] = 8;
      iVar8 = DAT_005cc970;
      (&DAT_00af598c)[iVar9] = 8;
      iVar9 = DAT_005cc980 + iVar5;
      (&DAT_00af598c)[iVar8 + iVar5] = 8;
      iVar8 = DAT_005cc988;
      (&DAT_00af598c)[iVar9] = 8;
      iVar9 = DAT_005cc990 + iVar5;
      (&DAT_00af598c)[iVar8 + iVar5] = 8;
      iVar8 = DAT_005cc998;
      (&DAT_00af598c)[iVar9] = 8;
      iVar9 = DAT_005cc9a0 + iVar5;
      (&DAT_00af598c)[iVar8 + iVar5] = 8;
      (&DAT_00af598c)[iVar9] = 8;
      return;
    case 3: ;
      iVar8 = DAT_005cc8f0 + iVar5;
      (&DAT_00af598c)[DAT_005cc8c0 + iVar5] = 8;
      iVar9 = DAT_005cc920 + iVar5;
      (&DAT_00af598c)[iVar8] = 8;
      iVar8 = DAT_005cc950;
      (&DAT_00af598c)[iVar9] = 8;
      iVar9 = DAT_005cc980 + iVar5;
      (&DAT_00af598c)[iVar8 + iVar5] = 8;
      iVar8 = DAT_005cc8c8;
      (&DAT_00af598c)[iVar9] = 8;
      iVar9 = DAT_005cc8f8 + iVar5;
      (&DAT_00af598c)[iVar8 + iVar5] = 8;
      iVar8 = DAT_005cc928;
      (&DAT_00af598c)[iVar9] = 8;
      iVar9 = DAT_005cc958 + iVar5;
      (&DAT_00af598c)[iVar8 + iVar5] = 8;
      iVar8 = DAT_005cc988;
      (&DAT_00af598c)[iVar9] = 8;
      iVar9 = DAT_005cc8d0 + iVar5;
      (&DAT_00af598c)[iVar8 + iVar5] = 8;
      iVar8 = DAT_005cc900;
      (&DAT_00af598c)[iVar9] = 8;
      iVar9 = DAT_005cc930 + iVar5;
      (&DAT_00af598c)[iVar8 + iVar5] = 8;
      iVar8 = DAT_005cc960;
      (&DAT_00af598c)[iVar9] = 8;
      iVar9 = DAT_005cc990 + iVar5;
      (&DAT_00af598c)[iVar8 + iVar5] = 8;
      (&DAT_00af598c)[iVar9] = 8;
      return;
    }
    break;
  case 0x18: ;
    switch(*(undefined4 *)(&DAT_00799a68 + iVar8)) {
    case 0: ;
      iVar8 = DAT_005cc930 + iVar5;
      (&DAT_00af598c)[DAT_005cc8c0 + iVar5] = 8;
      (&DAT_00af598c)[iVar8] = 8;
      return;
    case 1: ;
      iVar8 = DAT_005cc930 + iVar5;
      (&DAT_00af598c)[DAT_005cc920 + iVar5] = 8;
      (&DAT_00af598c)[iVar8] = 8;
      return;
    case 2: ;
      iVar8 = DAT_005cc930 + iVar5;
      (&DAT_00af598c)[DAT_005cc8d0 + iVar5] = 8;
      (&DAT_00af598c)[iVar8] = 8;
      return;
    case 3: ;
      iVar8 = DAT_005cc8d0 + iVar5;
      (&DAT_00af598c)[DAT_005cc8c0 + iVar5] = 8;
      (&DAT_00af598c)[iVar8] = 8;
      return;
    }
    break;
  case 0x1a: ;
    switch(*(undefined4 *)(&DAT_00799a68 + iVar8)) {
    case 0: ;
      goto switchD_00547474_caseD_0;
    case 1: ;
      iVar8 = DAT_005cc8d8;
      goto LAB_0054752d;
    case 2: ;
      goto switchD_00547474_caseD_2;
    case 3: ;
      goto switchD_00547474_caseD_3;
default: ;
      break;
    }
  case 0x1b: ;
  case 0x1c: ;
    (&DAT_00af598c)[DAT_005cc8c0 + iVar5] = 8;
    return;
  }
switchD_00546b89_caseD_3: ;
  return;
}
