#include "restore_types.h"
#define DAT_00adc36c (*(undefined4 *)0x00adc36cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b350dc (*(undefined4 *)0x00b350dcu)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd609 (*(undefined4 *)0x00bcd609u)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd610 (*(undefined4 *)0x00bcd610u)
#define DAT_00bcd612 (*(undefined4 *)0x00bcd612u)
#define DAT_00bcd618 (*(undefined4 *)0x00bcd618u)
#define DAT_00bcd639 (*(undefined4 *)0x00bcd639u)
#define DAT_00bcd66f (*(undefined4 *)0x00bcd66fu)
#define DAT_00bcd671 (*(undefined4 *)0x00bcd671u)
extern void FUN_004b1210();
extern void FUN_004b52f0();
extern void FUN_004b5420();

void FUN_004b3c60(int param_1)

{
  byte bVar1;
  undefined2 uVar2;
  undefined2 uVar3;
  uint uVar4;
  int iVar5;
  int iVar6;
  
  iVar6 = param_1 * 0x184;
  uVar2 = (&DAT_00bcd60c)[param_1 * 0xc2];
  uVar3 = (&DAT_00bcd60e)[param_1 * 0xc2];
  if ((char)(&DAT_00bcd609)[iVar6] < '\b') {
    FUN_004b5420(param_1);
    switch((&DAT_00bcd609)[iVar6]) {
    case 0: ;
      (&DAT_00bcd60e)[param_1 * 0xc2] = (&DAT_00bcd60e)[param_1 * 0xc2] + -1;
      iVar5 = *(int *)(&DAT_00bcd618 + iVar6) + -0xe4;
      break;
    case 1: ;
      (&DAT_00bcd60e)[param_1 * 0xc2] = (&DAT_00bcd60e)[param_1 * 0xc2] + -1;
      (&DAT_00bcd60c)[param_1 * 0xc2] = (&DAT_00bcd60c)[param_1 * 0xc2] + 1;
      iVar5 = *(int *)(&DAT_00bcd618 + iVar6) + -0xe3;
      break;
    case 2: ;
      (&DAT_00bcd60c)[param_1 * 0xc2] = (&DAT_00bcd60c)[param_1 * 0xc2] + 1;
      iVar5 = *(int *)(&DAT_00bcd618 + iVar6) + 1;
      break;
    case 3: ;
      (&DAT_00bcd60e)[param_1 * 0xc2] = (&DAT_00bcd60e)[param_1 * 0xc2] + 1;
      (&DAT_00bcd60c)[param_1 * 0xc2] = (&DAT_00bcd60c)[param_1 * 0xc2] + 1;
      iVar5 = *(int *)(&DAT_00bcd618 + iVar6) + 0xe5;
      break;
    case 4: ;
      (&DAT_00bcd60e)[param_1 * 0xc2] = (&DAT_00bcd60e)[param_1 * 0xc2] + 1;
      iVar5 = *(int *)(&DAT_00bcd618 + iVar6) + 0xe4;
      break;
    case 5: ;
      (&DAT_00bcd60e)[param_1 * 0xc2] = (&DAT_00bcd60e)[param_1 * 0xc2] + 1;
      (&DAT_00bcd60c)[param_1 * 0xc2] = (&DAT_00bcd60c)[param_1 * 0xc2] + -1;
      iVar5 = *(int *)(&DAT_00bcd618 + iVar6) + 0xe3;
      break;
    case 6: ;
      (&DAT_00bcd60c)[param_1 * 0xc2] = (&DAT_00bcd60c)[param_1 * 0xc2] + -1;
      iVar5 = *(int *)(&DAT_00bcd618 + iVar6) + -1;
      break;
    case 7: ;
      (&DAT_00bcd60e)[param_1 * 0xc2] = (&DAT_00bcd60e)[param_1 * 0xc2] + -1;
      (&DAT_00bcd60c)[param_1 * 0xc2] = (&DAT_00bcd60c)[param_1 * 0xc2] + -1;
      iVar5 = *(int *)(&DAT_00bcd618 + iVar6) + -0xe5;
      break;
default: ;
      goto switchD_004b3ca8_default;
    }
    *(int *)(&DAT_00bcd618 + iVar6) = iVar5;
    FUN_004b52f0(param_1);
    uVar4 = (&DAT_00af598c)[*(int *)(&DAT_00bcd618 + iVar6)];
    if (((uVar4 & 0x40) == 0) && ((uVar4 & 0x40000000) == 0)) {
      (&DAT_00bcd639)[iVar6] = 0;
    }
    else {
      (&DAT_00bcd639)[iVar6] = 1;
      if (((uVar4 & 4) != 0) &&
         (bVar1 = (&DAT_00adc36c)[*(int *)(&DAT_00bcd618 + iVar6)], bVar1 != 0)) {
        (&DAT_00bcd66f)[iVar6] = 0x12;
        if (bVar1 < 7) {
          switch((int)(bVar1)) {
          case 1: ;
          case 4: ;
            (&DAT_00bcd671)[iVar6] = 0xf;
            break;
          case 2: ;
            (&DAT_00bcd671)[iVar6] = 0x1b;
            break;
          case 3: ;
          case 6: ;
            (&DAT_00bcd671)[iVar6] = 0xe;
            break;
          case 5: ;
            (&DAT_00bcd671)[iVar6] = 0x1e;
            break;
default: ;
            (&DAT_00bcd671)[iVar6] = 0;
          }
        }
        else if (bVar1 == 0xd) {
          (&DAT_00bcd671)[iVar6] = 0x1e;
        }
        else if (bVar1 == 9) {
          (&DAT_00bcd671)[iVar6] = 0xe;
        }
        else if (bVar1 == 8) {
          (&DAT_00bcd671)[iVar6] = 0xe;
        }
        else if (bVar1 == 7) {
          (&DAT_00bcd671)[iVar6] = 0xe;
        }
        else if (bVar1 == 10) {
          (&DAT_00bcd671)[iVar6] = 0xe;
        }
        else {
          (&DAT_00bcd671)[iVar6] = 0x24;
        }
      }
    }
    switch((&DAT_00bcd602)[iVar6]) {
    case 0x4d: ;
    case 0x4e: ;
    case 0x5c: ;
    case 0x5d: ;
    case 100: ;
    case 0x65: ;
      break;
default: ;
      FUN_004b1210(param_1,(int)*(short *)(&DAT_00b350dc + *(int *)(&DAT_00bcd618 + iVar6) * 2));
    }
    *(undefined2 *)(&DAT_00bcd610 + iVar6) = uVar2;
    *(undefined2 *)(&DAT_00bcd612 + iVar6) = uVar3;
  }
switchD_004b3ca8_default: ;
  return;
}
