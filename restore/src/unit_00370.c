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
#define DAT_00799a38 (*(undefined4 *)0x00799a38u)
#define DAT_00799a3c (*(undefined4 *)0x00799a3cu)
#define DAT_00799a40 (*(undefined4 *)0x00799a40u)
#define DAT_00799a58 (*(undefined4 *)0x00799a58u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)

int FUN_00422a90(int param_1,int param_2)

{
  int iVar1;
  int *piVar2;
  
  iVar1 = *(int *)(&DAT_00799a38 + param_1 * 0x108);
  if (*(int *)(&DAT_00799a3c + param_1 * 0x84) == 0) goto switchD_00422ac4_caseD_11;
  switch(*(undefined4 *)(&DAT_00799a40 + param_1 * 0x84)) {
  case 0: ;
    piVar2 = &DAT_005be5f8;
    while ((((*(short *)(piVar2[2] + 0x12) != 0 || (*piVar2 != iVar1)) ||
            (piVar2[1] != (uint)(byte)(&DAT_00bcd602)[param_2 * 0x184])) ||
           (*(int *)(&DAT_00799a58 + (iVar1 + param_1 * 0x42) * 4) < 1))) {
      piVar2 = piVar2 + 4;
      if (0x5be627 < (int)piVar2) {
        return 0;
      }
    }
    break;
  case 1: ;
  case 0x1d: ;
    piVar2 = &DAT_005be508;
    while (((*(short *)(piVar2[2] + 0x12) != 0 || (*piVar2 != iVar1)) ||
           ((piVar2[1] != (uint)(byte)(&DAT_00bcd602)[param_2 * 0x184] ||
            (*(int *)(&DAT_00799a58 + (iVar1 + param_1 * 0x42) * 4) < 1))))) {
      piVar2 = piVar2 + 4;
      if (0x5be537 < (int)piVar2) {
        return 0;
      }
    }
    break;
  case 2: ;
    piVar2 = &DAT_005be538;
    while (((*(short *)(piVar2[2] + 0x12) != 0 || (*piVar2 != iVar1)) ||
           ((piVar2[1] != (uint)(byte)(&DAT_00bcd602)[param_2 * 0x184] ||
            (*(int *)(&DAT_00799a58 + (iVar1 + param_1 * 0x42) * 4) < 1))))) {
      piVar2 = piVar2 + 4;
      if (0x5be567 < (int)piVar2) {
        return 0;
      }
    }
    break;
  case 3: ;
    piVar2 = &DAT_005be568;
    while ((((*(short *)(piVar2[2] + 0x12) != 0 || (*piVar2 != iVar1)) ||
            (piVar2[1] != (uint)(byte)(&DAT_00bcd602)[param_2 * 0x184])) ||
           (*(int *)(&DAT_00799a58 + (iVar1 + param_1 * 0x42) * 4) < 1))) {
      piVar2 = piVar2 + 4;
      if (0x5be597 < (int)piVar2) {
        return 0;
      }
    }
    break;
  case 4: ;
    piVar2 = &DAT_005be748;
    while (((*(short *)(piVar2[2] + 0x12) != 0 || (*piVar2 != iVar1)) ||
           ((piVar2[1] != (uint)(byte)(&DAT_00bcd602)[param_2 * 0x184] ||
            (*(int *)(&DAT_00799a58 + (iVar1 + param_1 * 0x42) * 4) < 1))))) {
      piVar2 = piVar2 + 4;
      if (0x5be777 < (int)piVar2) {
        return 0;
      }
    }
    break;
  case 5: ;
    piVar2 = &DAT_005be778;
    while (((*(short *)(piVar2[2] + 0x12) != 0 || (*piVar2 != iVar1)) ||
           ((piVar2[1] != (uint)(byte)(&DAT_00bcd602)[param_2 * 0x184] ||
            (*(int *)(&DAT_00799a58 + (iVar1 + param_1 * 0x42) * 4) < 1))))) {
      piVar2 = piVar2 + 4;
      if (0x5be7a7 < (int)piVar2) {
        return 0;
      }
    }
    break;
  case 6: ;
    piVar2 = &DAT_005be628;
    while ((((*(short *)(piVar2[2] + 0x12) != 0 || (*piVar2 != iVar1)) ||
            (piVar2[1] != (uint)(byte)(&DAT_00bcd602)[param_2 * 0x184])) ||
           (*(int *)(&DAT_00799a58 + (iVar1 + param_1 * 0x42) * 4) < 1))) {
      piVar2 = piVar2 + 4;
      if (0x5be657 < (int)piVar2) {
        return 0;
      }
    }
    break;
  case 7: ;
    piVar2 = &DAT_005be7a8;
    while (((*(short *)(piVar2[2] + 0x12) != 0 || (*piVar2 != iVar1)) ||
           ((piVar2[1] != (uint)(byte)(&DAT_00bcd602)[param_2 * 0x184] ||
            (*(int *)(&DAT_00799a58 + (iVar1 + param_1 * 0x42) * 4) < 1))))) {
      piVar2 = piVar2 + 4;
      if (0x5be7d7 < (int)piVar2) {
        return 0;
      }
    }
    break;
  case 8: ;
    piVar2 = &DAT_005be7d8;
    while (((*(short *)(piVar2[2] + 0x12) != 0 || (*piVar2 != iVar1)) ||
           ((piVar2[1] != (uint)(byte)(&DAT_00bcd602)[param_2 * 0x184] ||
            (*(int *)(&DAT_00799a58 + (iVar1 + param_1 * 0x42) * 4) < 1))))) {
      piVar2 = piVar2 + 4;
      if (0x5be807 < (int)piVar2) {
        return 0;
      }
    }
    break;
  case 9: ;
    piVar2 = &DAT_005be658;
    while ((((*(short *)(piVar2[2] + 0x12) != 0 || (*piVar2 != iVar1)) ||
            (piVar2[1] != (uint)(byte)(&DAT_00bcd602)[param_2 * 0x184])) ||
           (*(int *)(&DAT_00799a58 + (iVar1 + param_1 * 0x42) * 4) < 1))) {
      piVar2 = piVar2 + 4;
      if (0x5be687 < (int)piVar2) {
        return 0;
      }
    }
    break;
  case 10: ;
    piVar2 = &DAT_005be688;
    while (((*(short *)(piVar2[2] + 0x12) != 0 || (*piVar2 != iVar1)) ||
           ((piVar2[1] != (uint)(byte)(&DAT_00bcd602)[param_2 * 0x184] ||
            (*(int *)(&DAT_00799a58 + (iVar1 + param_1 * 0x42) * 4) < 1))))) {
      piVar2 = piVar2 + 4;
      if (0x5be6b7 < (int)piVar2) {
        return 0;
      }
    }
    break;
  case 0xb: ;
    piVar2 = &DAT_005be898;
    while (((*(short *)(piVar2[2] + 0x12) != 0 || (*piVar2 != iVar1)) ||
           ((piVar2[1] != (uint)(byte)(&DAT_00bcd602)[param_2 * 0x184] ||
            (*(int *)(&DAT_00799a58 + (iVar1 + param_1 * 0x42) * 4) < 1))))) {
      piVar2 = piVar2 + 4;
      if (0x5be8c7 < (int)piVar2) {
        return 0;
      }
    }
    break;
  case 0xc: ;
    piVar2 = &DAT_005be808;
    while ((((*(short *)(piVar2[2] + 0x12) != 0 || (*piVar2 != iVar1)) ||
            (piVar2[1] != (uint)(byte)(&DAT_00bcd602)[param_2 * 0x184])) ||
           (*(int *)(&DAT_00799a58 + (iVar1 + param_1 * 0x42) * 4) < 1))) {
      piVar2 = piVar2 + 4;
      if (0x5be837 < (int)piVar2) {
        return 0;
      }
    }
    break;
  case 0xd: ;
    piVar2 = &DAT_005be8c8;
    while (((*(short *)(piVar2[2] + 0x12) != 0 || (*piVar2 != iVar1)) ||
           ((piVar2[1] != (uint)(byte)(&DAT_00bcd602)[param_2 * 0x184] ||
            (*(int *)(&DAT_00799a58 + (iVar1 + param_1 * 0x42) * 4) < 1))))) {
      piVar2 = piVar2 + 4;
      if (0x5be8f7 < (int)piVar2) {
        return 0;
      }
    }
    break;
  case 0xe: ;
    piVar2 = &DAT_005be8f8;
    while (((*(short *)(piVar2[2] + 0x12) != 0 || (*piVar2 != iVar1)) ||
           ((piVar2[1] != (uint)(byte)(&DAT_00bcd602)[param_2 * 0x184] ||
            (*(int *)(&DAT_00799a58 + (iVar1 + param_1 * 0x42) * 4) < 1))))) {
      piVar2 = piVar2 + 4;
      if (0x5be927 < (int)piVar2) {
        return 0;
      }
    }
    break;
  case 0xf: ;
    piVar2 = &DAT_005be598;
    while ((((*(short *)(piVar2[2] + 0x12) != 0 || (*piVar2 != iVar1)) ||
            (piVar2[1] != (uint)(byte)(&DAT_00bcd602)[param_2 * 0x184])) ||
           (*(int *)(&DAT_00799a58 + (iVar1 + param_1 * 0x42) * 4) < 1))) {
      piVar2 = piVar2 + 4;
      if (0x5be5c7 < (int)piVar2) {
        return 0;
      }
    }
    break;
  case 0x10: ;
    piVar2 = &DAT_005be5c8;
    while (((*(short *)(piVar2[2] + 0x12) != 0 || (*piVar2 != iVar1)) ||
           ((piVar2[1] != (uint)(byte)(&DAT_00bcd602)[param_2 * 0x184] ||
            (*(int *)(&DAT_00799a58 + (iVar1 + param_1 * 0x42) * 4) < 1))))) {
      piVar2 = piVar2 + 4;
      if (0x5be5f7 < (int)piVar2) {
        return 0;
      }
    }
    break;
  case 0x13: ;
    piVar2 = &DAT_005be6b8;
    while (((*(short *)(piVar2[2] + 0x12) != 0 || (*piVar2 != iVar1)) ||
           ((piVar2[1] != (uint)(byte)(&DAT_00bcd602)[param_2 * 0x184] ||
            (*(int *)(&DAT_00799a58 + (iVar1 + param_1 * 0x42) * 4) < 1))))) {
      piVar2 = piVar2 + 4;
      if (0x5be6e7 < (int)piVar2) {
        return 0;
      }
    }
    break;
  case 0x14: ;
    piVar2 = &DAT_005be6e8;
    while ((((*(short *)(piVar2[2] + 0x12) != 0 || (*piVar2 != iVar1)) ||
            (piVar2[1] != (uint)(byte)(&DAT_00bcd602)[param_2 * 0x184])) ||
           (*(int *)(&DAT_00799a58 + (iVar1 + param_1 * 0x42) * 4) < 1))) {
      piVar2 = piVar2 + 4;
      if (0x5be717 < (int)piVar2) {
        return 0;
      }
    }
    break;
  case 0x15: ;
    piVar2 = &DAT_005be718;
    while (((*(short *)(piVar2[2] + 0x12) != 0 || (*piVar2 != iVar1)) ||
           ((piVar2[1] != (uint)(byte)(&DAT_00bcd602)[param_2 * 0x184] ||
            (*(int *)(&DAT_00799a58 + (iVar1 + param_1 * 0x42) * 4) < 1))))) {
      piVar2 = piVar2 + 4;
      if (0x5be747 < (int)piVar2) {
        return 0;
      }
    }
    break;
  case 0x16: ;
    piVar2 = &DAT_005be838;
    while (((*(short *)(piVar2[2] + 0x12) != 0 || (*piVar2 != iVar1)) ||
           ((piVar2[1] != (uint)(byte)(&DAT_00bcd602)[param_2 * 0x184] ||
            (*(int *)(&DAT_00799a58 + (iVar1 + param_1 * 0x42) * 4) < 1))))) {
      piVar2 = piVar2 + 4;
      if (0x5be867 < (int)piVar2) {
        return 0;
      }
    }
    break;
  case 0x17: ;
    piVar2 = &DAT_005be868;
    while ((((*(short *)(piVar2[2] + 0x12) != 0 || (*piVar2 != iVar1)) ||
            (piVar2[1] != (uint)(byte)(&DAT_00bcd602)[param_2 * 0x184])) ||
           (*(int *)(&DAT_00799a58 + (iVar1 + param_1 * 0x42) * 4) < 1))) {
      piVar2 = piVar2 + 4;
      if (0x5be897 < (int)piVar2) {
        return 0;
      }
    }
    break;
  case 0x18: ;
    piVar2 = &DAT_005be928;
    while (((*(short *)(piVar2[2] + 0x12) != 0 || (*piVar2 != iVar1)) ||
           ((piVar2[1] != (uint)(byte)(&DAT_00bcd602)[param_2 * 0x184] ||
            (*(int *)(&DAT_00799a58 + (iVar1 + param_1 * 0x42) * 4) < 1))))) {
      piVar2 = piVar2 + 4;
      if (0x5be957 < (int)piVar2) {
        return 0;
      }
    }
    break;
  case 0x19: ;
    piVar2 = &DAT_005be958;
    while (((*(short *)(piVar2[2] + 0x12) != 0 || (*piVar2 != iVar1)) ||
           ((piVar2[1] != (uint)(byte)(&DAT_00bcd602)[param_2 * 0x184] ||
            (*(int *)(&DAT_00799a58 + (iVar1 + param_1 * 0x42) * 4) < 1))))) {
      piVar2 = piVar2 + 4;
      if (0x5be987 < (int)piVar2) {
        return 0;
      }
    }
    break;
  case 0x1a: ;
    piVar2 = &DAT_005be988;
    do {
      if ((((*(short *)(piVar2[2] + 0x12) == 0) && (*piVar2 == iVar1)) &&
          (piVar2[1] == (uint)(byte)(&DAT_00bcd602)[param_2 * 0x184])) &&
         (0 < *(int *)(&DAT_00799a58 + (iVar1 + param_1 * 0x42) * 4))) {
        return param_1;
      }
      piVar2 = piVar2 + 4;
    } while ((int)piVar2 < 0x5be9b8);
default: ;
switchD_00422ac4_caseD_11: ;
    param_1 = 0;
  }
  return param_1;
}
