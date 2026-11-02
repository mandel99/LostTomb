#include "restore_types.h"
#define DAT_00799a2e (*(undefined4 *)0x00799a2eu)
#define DAT_00799a84 (*(undefined4 *)0x00799a84u)
#define DAT_00f53dd8 (*(undefined *)0x00f53dd8u)
#define DAT_00f53ddc (*(undefined4 *)0x00f53ddcu)

undefined4 FUN_004561e0(int param_1,int param_2,int param_3)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  short sVar4;
  int local_4;
  
  iVar2 = 0;
  sVar4 = 0x32;
  local_4 = -1;
  if (0 < DAT_00f53dd8) {
    piVar3 = &DAT_00f53ddc;
    do {
      if (*piVar3 != 0) {
        switch(*piVar3) {
        case 1: ;
          iVar1 = 0;
          break;
        case 2: ;
          iVar1 = 1;
          break;
        case 3: ;
          iVar1 = 2;
          break;
        case 4: ;
          iVar1 = 3;
          break;
        case 5: ;
          iVar1 = 4;
          break;
        case 6: ;
          iVar1 = 5;
          break;
        case 7: ;
          iVar1 = 6;
          break;
        case 8: ;
          iVar1 = 7;
          break;
default: ;
          iVar1 = -1;
          break;
        case 0xd: ;
          iVar1 = 9;
          break;
        case 0xf: ;
          iVar1 = 8;
          break;
        case 0x11: ;
          iVar1 = 10;
          break;
        case 0x13: ;
          iVar1 = 0xb;
        }
        if (((&DAT_00799a2e)[iVar1 + param_1 * 0x84] == 0) && (*(int *)(param_2 + iVar1 * 4) != 0))
        goto LAB_004564a5;
      }
      iVar2 = iVar2 + 1;
      piVar3 = piVar3 + 1;
    } while (iVar2 < DAT_00f53dd8);
  }
  if (0 < DAT_00f53dd8) {
    piVar3 = &DAT_00f53ddc;
    iVar2 = DAT_00f53dd8;
    do {
      if (*piVar3 != 0) {
        switch(*piVar3) {
        case 1: ;
          iVar1 = 0;
          break;
        case 2: ;
          iVar1 = 1;
          break;
        case 3: ;
          iVar1 = 2;
          break;
        case 4: ;
          iVar1 = 3;
          break;
        case 5: ;
          iVar1 = 4;
          break;
        case 6: ;
          iVar1 = 5;
          break;
        case 7: ;
          iVar1 = 6;
          break;
        case 8: ;
          iVar1 = 7;
          break;
default: ;
          iVar1 = -1;
          break;
        case 0xd: ;
          iVar1 = 9;
          break;
        case 0xf: ;
          iVar1 = 8;
          break;
        case 0x11: ;
          iVar1 = 10;
          break;
        case 0x13: ;
          iVar1 = 0xb;
        }
        if ((*(int *)(param_2 + iVar1 * 4) != 0) &&
           ((short)(&DAT_00799a2e)[iVar1 + param_1 * 0x84] < sVar4)) {
          sVar4 = (&DAT_00799a2e)[iVar1 + param_1 * 0x84];
          local_4 = iVar1;
        }
      }
      piVar3 = piVar3 + 1;
      iVar2 = iVar2 + -1;
    } while (iVar2 != 0);
    if ((local_4 == -1) && (0 < DAT_00f53dd8)) {
      piVar3 = &DAT_00f53ddc;
      iVar2 = DAT_00f53dd8;
      do {
        if (*piVar3 != 0) {
          switch(*piVar3) {
          case 1: ;
            iVar1 = 0;
            break;
          case 2: ;
            iVar1 = 1;
            break;
          case 3: ;
            iVar1 = 2;
            break;
          case 4: ;
            iVar1 = 3;
            break;
          case 5: ;
            iVar1 = 4;
            break;
          case 6: ;
            iVar1 = 5;
            break;
          case 7: ;
            iVar1 = 6;
            break;
          case 8: ;
            iVar1 = 7;
            break;
default: ;
            iVar1 = -1;
            break;
          case 0xd: ;
            iVar1 = 9;
            break;
          case 0xf: ;
            iVar1 = 8;
            break;
          case 0x11: ;
            iVar1 = 10;
            break;
          case 0x13: ;
            iVar1 = 0xb;
          }
          if ((*(int *)(param_2 + iVar1 * 4) != 0) &&
             ((short)(&DAT_00799a2e)[iVar1 + param_1 * 0x84] < 600)) {
            local_4 = iVar1;
          }
        }
        piVar3 = piVar3 + 1;
        iVar2 = iVar2 + -1;
      } while (iVar2 != 0);
    }
  }
  iVar2 = 0;
  if (0 < DAT_00f53dd8) {
    piVar3 = &DAT_00f53ddc;
    do {
      if (*piVar3 != 0) {
        switch(*piVar3) {
        case 1: ;
          iVar1 = 0;
          break;
        case 2: ;
          iVar1 = 1;
          break;
        case 3: ;
          iVar1 = 2;
          break;
        case 4: ;
          iVar1 = 3;
          break;
        case 5: ;
          iVar1 = 4;
          break;
        case 6: ;
          iVar1 = 5;
          break;
        case 7: ;
          iVar1 = 6;
          break;
        case 8: ;
          iVar1 = 7;
          break;
default: ;
          iVar1 = -1;
          break;
        case 0xd: ;
          iVar1 = 9;
          break;
        case 0xf: ;
          iVar1 = 8;
          break;
        case 0x11: ;
          iVar1 = 10;
          break;
        case 0x13: ;
          iVar1 = 0xb;
        }
        if (local_4 == iVar1) {
          *(char *)(&DAT_00799a84 + param_1 * 0x42) = (char)iVar1;
LAB_004564a5: ;
          return *(undefined4 *)(param_3 + iVar1 * 4);
        }
      }
      iVar2 = iVar2 + 1;
      piVar3 = piVar3 + 1;
    } while (iVar2 < DAT_00f53dd8);
  }
  return 0;
}
