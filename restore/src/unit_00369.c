#include "restore_types.h"
#define DAT_005be508 (*(undefined4 *)0x005be508u)
#define DAT_005be538 (*(undefined4 *)0x005be538u)
#define DAT_005be568 (*(undefined4 *)0x005be568u)
#define DAT_005be598 (*(undefined4 *)0x005be598u)
#define DAT_005be5c8 (*(undefined4 *)0x005be5c8u)
#define DAT_005be5f8 (*(undefined4 *)0x005be5f8u)
#define DAT_005be628 (*(undefined4 *)0x005be628u)
#define DAT_005be658 (*(undefined4 *)0x005be658u)
#define DAT_005be688 (*(undefined4 *)0x005be688u)
#define DAT_005be6b8 (*(undefined4 *)0x005be6b8u)
#define DAT_005be6e8 (*(undefined4 *)0x005be6e8u)
#define DAT_005be718 (*(undefined4 *)0x005be718u)
#define DAT_005be748 (*(undefined4 *)0x005be748u)
#define DAT_005be778 (*(undefined4 *)0x005be778u)
#define DAT_005be7a8 (*(undefined4 *)0x005be7a8u)
#define DAT_005be7d8 (*(undefined4 *)0x005be7d8u)
#define DAT_005be808 (*(undefined4 *)0x005be808u)
#define DAT_005be838 (*(undefined4 *)0x005be838u)
#define DAT_005be868 (*(undefined4 *)0x005be868u)
#define DAT_005be898 (*(undefined4 *)0x005be898u)
#define DAT_005be8c8 (*(undefined4 *)0x005be8c8u)
#define DAT_005be8f8 (*(undefined4 *)0x005be8f8u)
#define DAT_005be928 (*(undefined4 *)0x005be928u)
#define DAT_005be958 (*(undefined4 *)0x005be958u)
#define DAT_005be988 (*(undefined4 *)0x005be988u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a38 (*(undefined4 *)0x00799a38u)
#define DAT_00799a40 (*(undefined4 *)0x00799a40u)
#define DAT_00799a58 (*(undefined4 *)0x00799a58u)
extern void FUN_00545bc0();
extern void FUN_0054fb20();
extern void FUN_00556410();
extern void FUN_005590a0();

void FUN_004223f0(int param_1)

{
  int iVar1;
  bool bVar2;
  int *piVar3;
  
  bVar2 = false;
  iVar1 = *(int *)(&DAT_00799a38 + param_1 * 0x108);
  switch(*(undefined4 *)(&DAT_00799a40 + param_1 * 0x84)) {
  case 0: ;
    piVar3 = &DAT_005be5f8;
    while ((int)piVar3 < 0x5be628) {
      if ((*piVar3 == iVar1) && (0 < *(int *)(&DAT_00799a58 + (iVar1 + param_1 * 0x42) * 4))) {
        bVar2 = true;
      }
      piVar3 = piVar3 + 4;
      if (bVar2) {
        return;
      }
    }
    break;
  case 1: ;
  case 0x1d: ;
    piVar3 = &DAT_005be508;
    while ((int)piVar3 < 0x5be538) {
      if ((*piVar3 == iVar1) && (0 < *(int *)(&DAT_00799a58 + (iVar1 + param_1 * 0x42) * 4))) {
        bVar2 = true;
      }
      piVar3 = piVar3 + 4;
      if (bVar2) {
        return;
      }
    }
    break;
  case 2: ;
    piVar3 = &DAT_005be538;
    while ((int)piVar3 < 0x5be568) {
      if ((*piVar3 == iVar1) && (0 < *(int *)(&DAT_00799a58 + (iVar1 + param_1 * 0x42) * 4))) {
        bVar2 = true;
      }
      piVar3 = piVar3 + 4;
      if (bVar2) {
        return;
      }
    }
    break;
  case 3: ;
    piVar3 = &DAT_005be568;
    while ((int)piVar3 < 0x5be598) {
      if ((*piVar3 == iVar1) && (0 < *(int *)(&DAT_00799a58 + (iVar1 + param_1 * 0x42) * 4))) {
        bVar2 = true;
      }
      piVar3 = piVar3 + 4;
      if (bVar2) {
        return;
      }
    }
    break;
  case 4: ;
    piVar3 = &DAT_005be748;
    while ((int)piVar3 < 0x5be778) {
      if ((*piVar3 == iVar1) && (0 < *(int *)(&DAT_00799a58 + (iVar1 + param_1 * 0x42) * 4))) {
        bVar2 = true;
      }
      piVar3 = piVar3 + 4;
      if (bVar2) {
        return;
      }
    }
    break;
  case 5: ;
    piVar3 = &DAT_005be778;
    while ((int)piVar3 < 0x5be7a8) {
      if ((*piVar3 == iVar1) && (0 < *(int *)(&DAT_00799a58 + (iVar1 + param_1 * 0x42) * 4))) {
        bVar2 = true;
      }
      piVar3 = piVar3 + 4;
      if (bVar2) {
        return;
      }
    }
    break;
  case 6: ;
    piVar3 = &DAT_005be628;
    while ((int)piVar3 < 0x5be658) {
      if ((*piVar3 == iVar1) && (0 < *(int *)(&DAT_00799a58 + (iVar1 + param_1 * 0x42) * 4))) {
        bVar2 = true;
      }
      piVar3 = piVar3 + 4;
      if (bVar2) {
        return;
      }
    }
    break;
  case 7: ;
    piVar3 = &DAT_005be7a8;
    while ((int)piVar3 < 0x5be7d8) {
      if ((*piVar3 == iVar1) && (0 < *(int *)(&DAT_00799a58 + (iVar1 + param_1 * 0x42) * 4))) {
        bVar2 = true;
      }
      piVar3 = piVar3 + 4;
      if (bVar2) {
        return;
      }
    }
    break;
  case 8: ;
    piVar3 = &DAT_005be7d8;
    while ((int)piVar3 < 0x5be808) {
      if ((*piVar3 == iVar1) && (0 < *(int *)(&DAT_00799a58 + (iVar1 + param_1 * 0x42) * 4))) {
        bVar2 = true;
      }
      piVar3 = piVar3 + 4;
      if (bVar2) {
        return;
      }
    }
    break;
  case 9: ;
    piVar3 = &DAT_005be658;
    while ((int)piVar3 < 0x5be688) {
      if ((*piVar3 == iVar1) && (0 < *(int *)(&DAT_00799a58 + (iVar1 + param_1 * 0x42) * 4))) {
        bVar2 = true;
      }
      piVar3 = piVar3 + 4;
      if (bVar2) {
        return;
      }
    }
    break;
  case 10: ;
    piVar3 = &DAT_005be688;
    while ((int)piVar3 < 0x5be6b8) {
      if ((*piVar3 == iVar1) && (0 < *(int *)(&DAT_00799a58 + (iVar1 + param_1 * 0x42) * 4))) {
        bVar2 = true;
      }
      piVar3 = piVar3 + 4;
      if (bVar2) {
        return;
      }
    }
    break;
  case 0xb: ;
    piVar3 = &DAT_005be898;
    while ((int)piVar3 < 0x5be8c8) {
      if ((*piVar3 == iVar1) && (0 < *(int *)(&DAT_00799a58 + (iVar1 + param_1 * 0x42) * 4))) {
        bVar2 = true;
      }
      piVar3 = piVar3 + 4;
      if (bVar2) {
        return;
      }
    }
    break;
  case 0xc: ;
    piVar3 = &DAT_005be808;
    while ((int)piVar3 < 0x5be838) {
      if ((*piVar3 == iVar1) && (0 < *(int *)(&DAT_00799a58 + (iVar1 + param_1 * 0x42) * 4))) {
        bVar2 = true;
      }
      piVar3 = piVar3 + 4;
      if (bVar2) {
        return;
      }
    }
    break;
  case 0xd: ;
    piVar3 = &DAT_005be8c8;
    while ((int)piVar3 < 0x5be8f8) {
      if ((*piVar3 == iVar1) && (0 < *(int *)(&DAT_00799a58 + (iVar1 + param_1 * 0x42) * 4))) {
        bVar2 = true;
      }
      piVar3 = piVar3 + 4;
      if (bVar2) {
        return;
      }
    }
    break;
  case 0xe: ;
    piVar3 = &DAT_005be8f8;
    while ((int)piVar3 < 0x5be928) {
      if ((*piVar3 == iVar1) && (0 < *(int *)(&DAT_00799a58 + (iVar1 + param_1 * 0x42) * 4))) {
        bVar2 = true;
      }
      piVar3 = piVar3 + 4;
      if (bVar2) {
        return;
      }
    }
    break;
  case 0xf: ;
    piVar3 = &DAT_005be598;
    while ((int)piVar3 < 0x5be5c8) {
      if ((*piVar3 == iVar1) && (0 < *(int *)(&DAT_00799a58 + (iVar1 + param_1 * 0x42) * 4))) {
        bVar2 = true;
      }
      piVar3 = piVar3 + 4;
      if (bVar2) {
        return;
      }
    }
    break;
  case 0x10: ;
    piVar3 = &DAT_005be5c8;
    while ((int)piVar3 < 0x5be5f8) {
      if ((*piVar3 == iVar1) && (0 < *(int *)(&DAT_00799a58 + (iVar1 + param_1 * 0x42) * 4))) {
        bVar2 = true;
      }
      piVar3 = piVar3 + 4;
      if (bVar2) {
        return;
      }
    }
    break;
default: ;
    goto switchD_00422419_caseD_11;
  case 0x13: ;
    piVar3 = &DAT_005be6b8;
    while ((int)piVar3 < 0x5be6e8) {
      if ((*piVar3 == iVar1) && (0 < *(int *)(&DAT_00799a58 + (iVar1 + param_1 * 0x42) * 4))) {
        bVar2 = true;
      }
      piVar3 = piVar3 + 4;
      if (bVar2) {
        return;
      }
    }
    break;
  case 0x14: ;
    piVar3 = &DAT_005be6e8;
    while ((int)piVar3 < 0x5be718) {
      if ((*piVar3 == iVar1) && (0 < *(int *)(&DAT_00799a58 + (iVar1 + param_1 * 0x42) * 4))) {
        bVar2 = true;
      }
      piVar3 = piVar3 + 4;
      if (bVar2) {
        return;
      }
    }
    break;
  case 0x15: ;
    piVar3 = &DAT_005be718;
    while ((int)piVar3 < 0x5be748) {
      if ((*piVar3 == iVar1) && (0 < *(int *)(&DAT_00799a58 + (iVar1 + param_1 * 0x42) * 4))) {
        bVar2 = true;
      }
      piVar3 = piVar3 + 4;
      if (bVar2) {
        return;
      }
    }
    break;
  case 0x16: ;
    piVar3 = &DAT_005be838;
    while ((int)piVar3 < 0x5be868) {
      if ((*piVar3 == iVar1) && (0 < *(int *)(&DAT_00799a58 + (iVar1 + param_1 * 0x42) * 4))) {
        bVar2 = true;
      }
      piVar3 = piVar3 + 4;
      if (bVar2) {
        return;
      }
    }
    break;
  case 0x17: ;
    piVar3 = &DAT_005be868;
    while ((int)piVar3 < 0x5be898) {
      if ((*piVar3 == iVar1) && (0 < *(int *)(&DAT_00799a58 + (iVar1 + param_1 * 0x42) * 4))) {
        bVar2 = true;
      }
      piVar3 = piVar3 + 4;
      if (bVar2) {
        return;
      }
    }
    break;
  case 0x18: ;
    piVar3 = &DAT_005be928;
    while ((int)piVar3 < 0x5be958) {
      if ((*piVar3 == iVar1) && (0 < *(int *)(&DAT_00799a58 + (iVar1 + param_1 * 0x42) * 4))) {
        bVar2 = true;
      }
      piVar3 = piVar3 + 4;
      if (bVar2) {
        return;
      }
    }
    break;
  case 0x19: ;
    piVar3 = &DAT_005be958;
    while ((int)piVar3 < 0x5be988) {
      if ((*piVar3 == iVar1) && (0 < *(int *)(&DAT_00799a58 + (iVar1 + param_1 * 0x42) * 4))) {
        bVar2 = true;
      }
      piVar3 = piVar3 + 4;
      if (bVar2) {
        return;
      }
    }
    break;
  case 0x1a: ;
    piVar3 = &DAT_005be988;
    while ((int)piVar3 < 0x5be9b8) {
      if ((*piVar3 == iVar1) && (0 < *(int *)(&DAT_00799a58 + (iVar1 + param_1 * 0x42) * 4))) {
        bVar2 = true;
      }
      piVar3 = piVar3 + 4;
      if (bVar2) {
        return;
      }
    }
  }
  if (!bVar2) {
switchD_00422419_caseD_11: ;
    switch((&DAT_007999e8)[param_1 * 0x84]) {
    case 0xe5: ;
      FUN_00545bc0(param_1);
      return;
    case 0xea: ;
      FUN_005590a0(param_1);
      return;
    case 0xeb: ;
      FUN_00556410(param_1);
      return;
    case 0xec: ;
      FUN_0054fb20(param_1);
    }
  }
  return;
}
