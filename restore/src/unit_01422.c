#include "restore_types.h"
#define DAT_00adc36c (*(undefined4 *)0x00adc36cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bcd5f9 (*(undefined4 *)0x00bcd5f9u)
#define DAT_00bcd609 (*(undefined4 *)0x00bcd609u)
#define DAT_00bcd60a (*(undefined4 *)0x00bcd60au)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd610 (*(undefined4 *)0x00bcd610u)
#define DAT_00bcd612 (*(undefined4 *)0x00bcd612u)
#define DAT_00bcd618 (*(undefined4 *)0x00bcd618u)
#define DAT_00bcd624 (*(undefined4 *)0x00bcd624u)
#define DAT_00bcd626 (*(undefined4 *)0x00bcd626u)
#define DAT_00bcd631 (*(undefined4 *)0x00bcd631u)
#define DAT_00bcd639 (*(undefined4 *)0x00bcd639u)
#define DAT_00bcd642 (*(undefined4 *)0x00bcd642u)
#define DAT_00bcd644 (*(undefined4 *)0x00bcd644u)
#define DAT_00bcd646 (*(undefined4 *)0x00bcd646u)
#define DAT_00bcd648 (*(undefined4 *)0x00bcd648u)
#define DAT_00bcd64a (*(undefined4 *)0x00bcd64au)
#define DAT_00bcd64c (*(undefined4 *)0x00bcd64cu)
#define DAT_00bcd65d (*(undefined4 *)0x00bcd65du)
#define DAT_00bcd66f (*(undefined4 *)0x00bcd66fu)
#define DAT_00bcd670 (*(undefined4 *)0x00bcd670u)
#define DAT_00bcd671 (*(undefined4 *)0x00bcd671u)
#define DAT_00bcd718 (*(undefined4 *)0x00bcd718u)
#define s_follow_figure_closely_following_o_005df72c (*(char (*)[46])0x005df72cu)
extern void FUN_004b52f0();
extern void FUN_004b5420();
extern bool FUN_004f1d10();

void FUN_004b23c0(int param_1,int param_2,undefined4 param_3,int param_4)

{
  byte bVar1;
  short sVar2;
  short sVar3;
  short sVar4;
  int iVar5;
  char cVar6;
  uint uVar7;
  int iVar8;
  int iVar9;
  
  iVar8 = param_1 * 0x184;
  while( true ) {
    if (((&DAT_00bcd60c)[param_1 * 0xc2] == *(short *)(&DAT_00bcd624 + iVar8)) &&
       ((&DAT_00bcd60e)[param_1 * 0xc2] == *(short *)(&DAT_00bcd626 + iVar8))) {
      (&DAT_00bcd65d)[iVar8] = 1;
    }
    if (*(short *)(&DAT_00bcd718 + iVar8) == 0) break;
    if (*(short *)(&DAT_00bcd718 + param_2 * 0x184) != 0) {
      if (*(short *)(&DAT_00bcd646 + iVar8) == *(short *)(&DAT_00bcd646 + param_2 * 0x184)) {
        if ((short)(&DAT_00bcd64a)[param_1 * 0xc2] + param_4 <
            (int)(short)(&DAT_00bcd64a)[param_2 * 0xc2]) {
          FUN_004f1d10(param_1,param_3);
          (&DAT_00bcd5f9)[iVar8] = (&DAT_00bcd5f9)[param_2 * 0x184];
          return;
        }
      }
      else if ((*(short *)(&DAT_00bcd646 + iVar8) != (&DAT_00bcd642)[param_2 * 0xc2]) ||
              (cVar6 = FUN_004f1d10(param_1,param_3), cVar6 != '\0')) {
LAB_004b24ae: ;
        iVar9 = param_2 * 0x184;
        *(undefined2 *)(&DAT_00bcd718 + iVar8) = *(undefined2 *)(&DAT_00bcd718 + iVar9);
        (&DAT_00bcd64c)[param_1 * 0xc2] = (&DAT_00bcd64c)[param_2 * 0xc2];
        (&DAT_00bcd64a)[param_1 * 0xc2] =
             (ushort)((short)(&DAT_00bcd64a)[param_2 * 0xc2] - param_4) &
             ((short)(&DAT_00bcd64a)[param_2 * 0xc2] - param_4 < 0) - 1;
        (&DAT_00bcd642)[param_1 * 0xc2] = (&DAT_00bcd642)[param_2 * 0xc2];
        *(undefined2 *)(&DAT_00bcd646 + iVar8) = *(undefined2 *)(&DAT_00bcd646 + iVar9);
        (&DAT_00bcd644)[param_1 * 0xc2] = (&DAT_00bcd644)[param_2 * 0xc2];
        *(undefined2 *)(&DAT_00bcd648 + iVar8) = *(undefined2 *)(&DAT_00bcd648 + iVar9);
        (&DAT_00bcd609)[iVar8] = (&DAT_00bcd609)[iVar9];
        FUN_004f1d10(param_1,0);
        (&DAT_00bcd5f9)[iVar8] = (&DAT_00bcd5f9)[iVar9];
        return;
      }
LAB_004b27ab: ;
      (&DAT_00bcd5f9)[iVar8] = (&DAT_00bcd5f9)[param_2 * 0x184];
      return;
    }
    OutputDebugStringA(s_follow_figure_closely_following_o_005df72c);
    *(undefined2 *)(&DAT_00bcd718 + iVar8) = 0;
  }
  iVar9 = param_2 * 0x184;
  if (*(short *)(&DAT_00bcd718 + iVar9) != 0) {
    cVar6 = (&DAT_00bcd631)[iVar8];
    (&DAT_00bcd631)[iVar8] = cVar6 + '\x01';
    if ((char)(cVar6 + '\x01') < '\t') goto LAB_004b27ab;
    (&DAT_00bcd631)[iVar8] = 0;
    goto LAB_004b24ae;
  }
  uVar7 = (char)(&DAT_00bcd631)[iVar9] - param_4;
  if ((int)uVar7 < 0) {
    uVar7 = (uint)(byte)((char)uVar7 + 0xf);
  }
  (&DAT_00bcd631)[iVar8] = (char)uVar7;
  if ((char)uVar7 == '\b') {
    FUN_004b5420(param_1);
    iVar5 = DAT_00bca2d8;
    sVar2 = (&DAT_00bcd60c)[param_1 * 0xc2];
    sVar3 = (&DAT_00bcd60e)[param_1 * 0xc2];
    (&DAT_00bcd60c)[param_1 * 0xc2] = (&DAT_00bcd60c)[param_2 * 0xc2];
    sVar4 = (&DAT_00bcd60e)[param_2 * 0xc2];
    (&DAT_00bcd60e)[param_1 * 0xc2] = sVar4;
    *(int *)(&DAT_00bcd618 + iVar8) = iVar5 + (short)(&DAT_00bcd60c)[param_1 * 0xc2] + sVar4 * 0xe4;
    if (((&DAT_00bcd60c)[param_1 * 0xc2] != sVar2) || ((&DAT_00bcd60e)[param_1 * 0xc2] != sVar3)) {
      *(short *)(&DAT_00bcd610 + iVar8) = sVar2;
      *(short *)(&DAT_00bcd612 + iVar8) = sVar3;
    }
    FUN_004b52f0(param_1);
    uVar7 = (&DAT_00af598c)[*(int *)(&DAT_00bcd618 + iVar8)];
    if (((uVar7 & 0x40) == 0) && ((uVar7 & 0x40000000) == 0)) {
      (&DAT_00bcd639)[iVar8] = 0;
    }
    else {
      (&DAT_00bcd639)[iVar8] = 1;
      if ((uVar7 & 4) != 0) {
        bVar1 = (&DAT_00adc36c)[*(int *)(&DAT_00bcd618 + iVar8)];
        (&DAT_00bcd66f)[iVar8] = 0x12;
        if (bVar1 < 7) {
          switch((int)(bVar1)) {
          case 1: ;
          case 4: ;
            (&DAT_00bcd671)[iVar8] = 0xf;
            break;
          case 2: ;
            (&DAT_00bcd671)[iVar8] = 0x1b;
            break;
          case 3: ;
          case 6: ;
            (&DAT_00bcd671)[iVar8] = 0xe;
            break;
          case 5: ;
            (&DAT_00bcd671)[iVar8] = 0x1e;
            break;
default: ;
            (&DAT_00bcd671)[iVar8] = 0;
          }
        }
        else if (bVar1 == 0xd) {
          (&DAT_00bcd671)[iVar8] = 0x1e;
        }
        else if (bVar1 == 9) {
          (&DAT_00bcd671)[iVar8] = 0xe;
        }
        else if (bVar1 == 8) {
          (&DAT_00bcd671)[iVar8] = 0xe;
        }
        else if (bVar1 == 7) {
          (&DAT_00bcd671)[iVar8] = 0xe;
        }
        else if (bVar1 == 10) {
          (&DAT_00bcd671)[iVar8] = 0xe;
        }
        else {
          (&DAT_00bcd671)[iVar8] = 0x24;
        }
      }
    }
  }
  if ((&DAT_00bcd631)[iVar8] == '\0') {
    (&DAT_00bcd60a)[iVar8] = (&DAT_00bcd609)[iVar8];
    (&DAT_00bcd609)[iVar8] = (&DAT_00bcd609)[iVar9];
  }
  if ((&DAT_00bcd66f)[iVar8] == '\0') {
    if ((&DAT_00bcd670)[iVar8] != '\0') {
      (&DAT_00bcd670)[iVar8] = (&DAT_00bcd670)[iVar8] + -1;
      (&DAT_00bcd5f9)[iVar8] = (&DAT_00bcd5f9)[iVar9];
      return;
    }
  }
  else {
    cVar6 = (&DAT_00bcd66f)[iVar8] + -1;
    (&DAT_00bcd66f)[iVar8] = cVar6;
    if (cVar6 != '\0') {
      (&DAT_00bcd65d)[iVar8] = 1;
      if ((byte)(&DAT_00bcd670)[iVar8] < (byte)(&DAT_00bcd671)[iVar8]) {
        (&DAT_00bcd670)[iVar8] = (&DAT_00bcd670)[iVar8] + 1;
      }
      if ((byte)(&DAT_00bcd671)[iVar8] < (byte)(&DAT_00bcd670)[iVar8]) {
        (&DAT_00bcd670)[iVar8] = (&DAT_00bcd670)[iVar8] - 1;
        (&DAT_00bcd5f9)[iVar8] = (&DAT_00bcd5f9)[iVar9];
        return;
      }
      goto LAB_004b27ab;
    }
  }
  (&DAT_00bcd65d)[iVar8] = 0;
  goto LAB_004b27ab;
}
