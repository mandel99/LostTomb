#include "restore_types.h"
#define DAT_00601de4 (*(undefined4 *)0x00601de4u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a30 (*(undefined4 *)0x00799a30u)
#define DAT_00799a34 (*(undefined4 *)0x00799a34u)
#define DAT_00799a3c (*(undefined4 *)0x00799a3cu)
#define DAT_00bcd62e (*(undefined4 *)0x00bcd62eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
extern void FUN_00420240();
extern void FUN_00541920();

void FUN_00541de0(int param_1,int param_2,undefined4 param_3)

{
  int iVar1;
  bool bVar2;
  bool bVar3;
  int *piVar4;
  int iVar5;
  int *piVar6;
  int iVar7;
  
  iVar7 = param_1 * 0x108;
  switch((&DAT_00601de4)[(short)(&DAT_007999ea)[param_1 * 0x84] * 0xd]) {
  case 0: ;
    FUN_00420240(param_1,param_2,param_3);
    return;
  case 1: ;
    iVar5 = (&DAT_00799a30)[param_1 * 0x42];
    param_2 = param_2 * 0x184;
    if (*(short *)(&DAT_00bcd62e + param_2) < 0) {
      *(undefined2 *)(&DAT_00bcd62e + param_2) = 0;
    }
    *(short *)(&DAT_00bcd62e + param_2) = *(short *)(&DAT_00bcd62e + param_2) + 1;
    if (0x31 < *(short *)(&DAT_00bcd62e + param_2)) {
      (&DAT_00799a30)[param_1 * 0x42] = iVar5 + 1;
      FUN_00541920(param_1);
      if ((&DAT_00799a30)[param_1 * 0x42] == 2) {
        *(uint *)(&DAT_00799a3c + (param_1 - (short)(&DAT_007999ea)[param_1 * 0x84]) * 0x84) =
             *(uint *)(&DAT_00799a3c + (param_1 - (short)(&DAT_007999ea)[param_1 * 0x84]) * 0x84) |
             4;
      }
      *(undefined4 *)(&DAT_00799a34 + iVar7) = 0;
      (&DAT_00bcd630)[param_2] = (undefined1)param_3;
      return;
    }
    break;
  case 2: ;
  case 3: ;
  case 4: ;
  case 5: ;
  case 6: ;
  case 7: ;
    iVar5 = (&DAT_00799a30)[param_1 * 0x42];
    param_2 = param_2 * 0x184;
    *(short *)(&DAT_00bcd62e + param_2) = *(short *)(&DAT_00bcd62e + param_2) + 1;
    if (0x31 < *(short *)(&DAT_00bcd62e + param_2)) {
      (&DAT_00799a30)[param_1 * 0x42] = iVar5 + 1;
      FUN_00541920(param_1);
      if ((&DAT_00799a30)[param_1 * 0x42] == 2) {
        param_1 = param_1 - (short)(&DAT_007999ea)[param_1 * 0x84];
        iVar5 = 0;
        piVar4 = &DAT_00601de4;
        piVar6 = &DAT_00799a30 + param_1 * 0x42;
        do {
          iVar1 = *piVar4;
          if (((((iVar1 == 2) || (iVar1 == 3)) || ((iVar1 == 4 || ((iVar1 == 5 || (iVar1 == 6))))))
              || (iVar1 == 7)) && (*piVar6 < 2)) break;
          piVar4 = piVar4 + 0xd;
          iVar5 = iVar5 + 1;
          piVar6 = piVar6 + 0x42;
        } while ((int)piVar4 < 0x603234);
        if (iVar5 == 100) {
          *(uint *)(&DAT_00799a3c + param_1 * 0x84) = *(uint *)(&DAT_00799a3c + param_1 * 0x84) | 8;
        }
      }
      *(undefined4 *)(&DAT_00799a34 + iVar7) = 0;
      (&DAT_00bcd630)[param_2] = (undefined1)param_3;
      return;
    }
    break;
  case 8: ;
  case 9: ;
  case 10: ;
  case 0xb: ;
    iVar5 = (&DAT_00799a30)[param_1 * 0x42];
    param_2 = param_2 * 0x184;
    *(short *)(&DAT_00bcd62e + param_2) = *(short *)(&DAT_00bcd62e + param_2) + 1;
    if (0x31 < *(short *)(&DAT_00bcd62e + param_2)) {
      (&DAT_00799a30)[param_1 * 0x42] = iVar5 + 1;
      FUN_00541920(param_1);
      bVar2 = true;
      bVar3 = true;
      param_1 = param_1 - (short)(&DAT_007999ea)[param_1 * 0x84];
      piVar4 = &DAT_00601de4;
      piVar6 = &DAT_00799a30 + param_1 * 0x42;
      do {
        iVar5 = *piVar4;
        if ((((iVar5 == 8) || (iVar5 == 9)) || (iVar5 == 10)) || (iVar5 == 0xb)) {
          if (*piVar6 == 0) {
            bVar2 = false;
          }
          if (*piVar6 < 2) {
            bVar3 = false;
          }
        }
        piVar4 = piVar4 + 0xd;
        piVar6 = piVar6 + 0x42;
      } while ((int)piVar4 < 0x603234);
      if (bVar2) {
        *(uint *)(&DAT_00799a3c + param_1 * 0x84) = *(uint *)(&DAT_00799a3c + param_1 * 0x84) | 2;
      }
      if (bVar3) {
        *(uint *)(&DAT_00799a3c + param_1 * 0x84) = *(uint *)(&DAT_00799a3c + param_1 * 0x84) | 0x10
        ;
      }
      *(undefined4 *)(&DAT_00799a34 + iVar7) = 0;
      (&DAT_00bcd630)[param_2] = (undefined1)param_3;
      return;
    }
    break;
  case 0xc: ;
    iVar5 = (&DAT_00799a30)[param_1 * 0x42];
    param_2 = param_2 * 0x184;
    *(short *)(&DAT_00bcd62e + param_2) = *(short *)(&DAT_00bcd62e + param_2) + 1;
    if (0x31 < *(short *)(&DAT_00bcd62e + param_2)) {
      (&DAT_00799a30)[param_1 * 0x42] = iVar5 + 1;
      FUN_00541920(param_1);
      if ((&DAT_00799a30)[param_1 * 0x42] == 4) {
        *(uint *)(&DAT_00799a3c + (param_1 - (short)(&DAT_007999ea)[param_1 * 0x84]) * 0x84) =
             *(uint *)(&DAT_00799a3c + (param_1 - (short)(&DAT_007999ea)[param_1 * 0x84]) * 0x84) |
             0x20;
      }
      *(undefined4 *)(&DAT_00799a34 + iVar7) = 0;
      (&DAT_00bcd630)[param_2] = (undefined1)param_3;
    }
  }
  return;
}
