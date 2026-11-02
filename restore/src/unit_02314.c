#include "restore_types.h"
#define DAT_00b4e6fa (*(undefined4 *)0x00b4e6fau)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00b4e6fe (*(undefined4 *)0x00b4e6feu)
#define DAT_00b4e8c4 (*(undefined4 *)0x00b4e8c4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00eadfd4 (*(undefined *)0x00eadfd4u)
#define DAT_00eadfd8 (*(undefined *)0x00eadfd8u)
#define DAT_00eae004 (*(undefined *)0x00eae004u)
#define DAT_00eae008 (*(undefined *)0x00eae008u)
#define DAT_00eae00c (*(undefined *)0x00eae00cu)
#define DAT_00ec754c (*(undefined4 *)0x00ec754cu)
#define DAT_00ec762f (*(undefined4 *)0x00ec762fu)
#define DAT_00ec7631 (*(undefined4 *)0x00ec7631u)
#define DAT_00ec7714 (*(undefined4 *)0x00ec7714u)
#define DAT_00eed75c (*(undefined4 *)0x00eed75cu)
#define DAT_00eed760 (*(undefined4 *)0x00eed760u)
#define DAT_00f203a0 (*(undefined *)0x00f203a0u)
extern void FUN_0040f8f0();
extern undefined4 FUN_0051afa0();

bool FUN_0051a420(int param_1,int param_2,int param_3,int param_4,int param_5,int param_6)

{
  char cVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int *piVar7;
  int local_4;
  
  local_4 = 0;
  FUN_0040f8f0(&DAT_00eed760,0x32c40,0);
  DAT_00eae004 = DAT_00bca2d8 + param_2 * 0xe4 + param_1;
  DAT_00eadfd4 = DAT_00eadfd4 + 1;
  DAT_00eadfd8 = DAT_00eadfd8 + 1;
  (&DAT_00eed760)[DAT_00eae004] = 1;
  DAT_00f203a0 = DAT_00eae004;
  iVar2 = 1;
  DAT_00eae00c = 0;
  DAT_00eae008 = 1;
  param_1 = 0;
  param_3 = DAT_00bca2d8 + param_4 * 0xe4 + param_3;
  if (param_5 == 0) {
    do {
      iVar4 = (&DAT_00eed760)[(&DAT_00f203a0)[DAT_00eae00c]];
      iVar5 = DAT_00eae00c;
      iVar3 = local_4;
      if (local_4 < iVar4) {
        if (DAT_00eae00c < iVar2) {
          iVar6 = DAT_00eae00c + 1;
          iVar3 = iVar4;
          if (iVar6 < iVar2) {
            piVar7 = &DAT_00f203a0 + iVar6;
            do {
              iVar3 = (&DAT_00eed760)[*piVar7];
              if ((iVar3 < iVar4) && (iVar4 = iVar3, iVar5 = iVar6, iVar3 <= local_4)) break;
              iVar6 = iVar6 + 1;
              piVar7 = piVar7 + 1;
              iVar3 = iVar4;
            } while (iVar6 < iVar2);
          }
        }
        else {
          iVar6 = DAT_00eae00c + 1;
          if (iVar6 < 0xcb10) {
            piVar7 = &DAT_00f203a0 + iVar6;
            do {
              iVar3 = (&DAT_00eed760)[*piVar7];
              if ((iVar3 < iVar4) && (iVar4 = iVar3, iVar5 = iVar6, iVar3 <= local_4))
              goto LAB_0051a58d;
              piVar7 = piVar7 + 1;
              iVar6 = iVar6 + 1;
            } while ((int)piVar7 < 0xf52fdd);
          }
          iVar3 = iVar4;
          if ((local_4 < iVar4) && (iVar6 = 0, 0 < iVar2)) {
            piVar7 = &DAT_00f203a0;
            do {
              if ((int)(&DAT_00eed760)[*piVar7] < iVar4) {
                iVar4 = (&DAT_00eed760)[*piVar7];
                iVar5 = iVar6;
              }
              iVar6 = iVar6 + 1;
              piVar7 = piVar7 + 1;
              iVar3 = iVar4;
            } while (iVar6 < iVar2);
          }
        }
      }
LAB_0051a58d: ;
      local_4 = iVar3;
      iVar4 = (&DAT_00f203a0)[iVar5];
      if (iVar5 != DAT_00eae00c) {
        (&DAT_00f203a0)[iVar5] = (&DAT_00f203a0)[DAT_00eae00c];
      }
      if ((iVar4 == param_3) || (param_1 = param_1 + 1, param_6 < param_1)) break;
      iVar3 = iVar4 + -0xe4;
      iVar5 = (&DAT_00eed760)[iVar4] + 1;
      if (((&DAT_00eed760)[iVar3] == 0) &&
         (((iVar6 = FUN_0051afa0(iVar3), iVar2 = DAT_00eae008, iVar6 != 0 &&
           (cVar1 = (&DAT_00ec754c)[iVar4], -1 < cVar1)) && (cVar1 < '\x05')))) {
        if (cVar1 == '\x03') {
          iVar2 = 0x28;
        }
        else {
          iVar2 = (-(uint)(cVar1 != '\x01') & 0xfffffff6) + 10;
        }
        (&DAT_00eed760)[iVar3] = iVar2 + iVar5;
        (&DAT_00f203a0)[DAT_00eae008] = iVar3;
        DAT_00eae008 = DAT_00eae008 + 1;
        iVar2 = DAT_00eae008;
        if (0xcb0f < DAT_00eae008) {
          DAT_00eae008 = 0;
          iVar2 = 0;
        }
      }
      iVar3 = iVar4 + 1;
      if ((((&DAT_00eed760)[iVar3] == 0) &&
          (iVar6 = FUN_0051afa0(iVar3), iVar2 = DAT_00eae008, iVar6 != 0)) &&
         ((cVar1 = (&DAT_00ec7631)[iVar4], -1 < cVar1 && (cVar1 < '\x05')))) {
        if (cVar1 == '\x03') {
          iVar2 = 0x28;
        }
        else {
          iVar2 = (-(uint)(cVar1 != '\x01') & 0xfffffff6) + 10;
        }
        (&DAT_00eed760)[iVar3] = iVar2 + iVar5;
        (&DAT_00f203a0)[DAT_00eae008] = iVar3;
        DAT_00eae008 = DAT_00eae008 + 1;
        iVar2 = DAT_00eae008;
        if (0xcb0f < DAT_00eae008) {
          DAT_00eae008 = 0;
          iVar2 = 0;
        }
      }
      iVar3 = iVar4 + 0xe4;
      if ((((&DAT_00eed760)[iVar3] == 0) &&
          (iVar6 = FUN_0051afa0(iVar3), iVar2 = DAT_00eae008, iVar6 != 0)) &&
         ((cVar1 = (&DAT_00ec7714)[iVar4], -1 < cVar1 && (cVar1 < '\x05')))) {
        if (cVar1 == '\x03') {
          iVar2 = 0x28;
        }
        else {
          iVar2 = (-(uint)(cVar1 != '\x01') & 0xfffffff6) + 10;
        }
        (&DAT_00eed760)[iVar3] = iVar2 + iVar5;
        (&DAT_00f203a0)[DAT_00eae008] = iVar3;
        DAT_00eae008 = DAT_00eae008 + 1;
        iVar2 = DAT_00eae008;
        if (0xcb0f < DAT_00eae008) {
          DAT_00eae008 = 0;
          iVar2 = 0;
        }
      }
      if (*(int *)(&DAT_00eed75c + iVar4 * 4) == 0) {
        iVar3 = FUN_0051afa0(iVar4 + -1);
        iVar2 = DAT_00eae008;
        if (((iVar3 != 0) && (cVar1 = (&DAT_00ec762f)[iVar4], -1 < cVar1)) && (cVar1 < '\x05')) {
          if (cVar1 == '\x03') {
            iVar2 = 0x28;
          }
          else {
            iVar2 = (-(uint)(cVar1 != '\x01') & 0xfffffff6) + 10;
          }
          *(int *)(&DAT_00eed75c + iVar4 * 4) = iVar2 + iVar5;
          (&DAT_00f203a0)[DAT_00eae008] = iVar4 + -1;
          DAT_00eae008 = DAT_00eae008 + 1;
          iVar2 = DAT_00eae008;
          if (0xcb0f < DAT_00eae008) {
            DAT_00eae008 = 0;
            iVar2 = 0;
          }
        }
      }
      DAT_00eae00c = DAT_00eae00c + 1;
      if (0xcb0f < DAT_00eae00c) {
        DAT_00eae00c = 0;
      }
    } while (DAT_00eae00c != iVar2);
  }
  else {
    do {
      iVar4 = (&DAT_00f203a0)[DAT_00eae00c];
      if (iVar4 == param_3) break;
      iVar3 = iVar4 + -0xe4;
      iVar5 = (&DAT_00eed760)[iVar4] + 1;
      if ((((&DAT_00eed760)[iVar3] == 0) &&
          (iVar6 = FUN_0051afa0(iVar3), iVar2 = DAT_00eae008, iVar6 != 0)) &&
         (((cVar1 = (&DAT_00ec754c)[iVar4], cVar1 == '\0' || (cVar1 == '\x02')) ||
          ((cVar1 == '\x01' && (*(short *)(&DAT_00b4e6fc + iVar3 * 2) == param_5)))))) {
        (&DAT_00eed760)[iVar3] = iVar5;
        (&DAT_00f203a0)[DAT_00eae008] = iVar3;
        DAT_00eae008 = DAT_00eae008 + 1;
        iVar2 = DAT_00eae008;
        if (0xcb0f < DAT_00eae008) {
          DAT_00eae008 = 0;
          iVar2 = 0;
        }
      }
      iVar3 = iVar4 + 1;
      if ((((&DAT_00eed760)[iVar3] == 0) &&
          (iVar6 = FUN_0051afa0(iVar3), iVar2 = DAT_00eae008, iVar6 != 0)) &&
         (((cVar1 = (&DAT_00ec7631)[iVar4], cVar1 == '\0' || (cVar1 == '\x02')) ||
          ((cVar1 == '\x01' && (*(short *)(&DAT_00b4e6fe + iVar4 * 2) == param_5)))))) {
        (&DAT_00eed760)[iVar3] = iVar5;
        (&DAT_00f203a0)[DAT_00eae008] = iVar3;
        DAT_00eae008 = DAT_00eae008 + 1;
        iVar2 = DAT_00eae008;
        if (0xcb0f < DAT_00eae008) {
          DAT_00eae008 = 0;
          iVar2 = 0;
        }
      }
      iVar3 = iVar4 + 0xe4;
      if ((((&DAT_00eed760)[iVar3] == 0) &&
          (iVar6 = FUN_0051afa0(iVar3), iVar2 = DAT_00eae008, iVar6 != 0)) &&
         (((cVar1 = (&DAT_00ec7714)[iVar4], cVar1 == '\0' || (cVar1 == '\x02')) ||
          ((cVar1 == '\x01' && (*(short *)(&DAT_00b4e8c4 + iVar4 * 2) == param_5)))))) {
        (&DAT_00eed760)[iVar3] = iVar5;
        (&DAT_00f203a0)[DAT_00eae008] = iVar3;
        DAT_00eae008 = DAT_00eae008 + 1;
        iVar2 = DAT_00eae008;
        if (0xcb0f < DAT_00eae008) {
          DAT_00eae008 = 0;
          iVar2 = 0;
        }
      }
      if (*(int *)(&DAT_00eed75c + iVar4 * 4) == 0) {
        iVar3 = FUN_0051afa0(iVar4 + -1);
        iVar2 = DAT_00eae008;
        if ((iVar3 != 0) &&
           (((cVar1 = (&DAT_00ec762f)[iVar4], cVar1 == '\0' || (cVar1 == '\x02')) ||
            ((cVar1 == '\x01' && (*(short *)(&DAT_00b4e6fa + iVar4 * 2) == param_5)))))) {
          *(int *)(&DAT_00eed75c + iVar4 * 4) = iVar5;
          (&DAT_00f203a0)[DAT_00eae008] = iVar4 + -1;
          DAT_00eae008 = DAT_00eae008 + 1;
          iVar2 = DAT_00eae008;
          if (0xcb0f < DAT_00eae008) {
            DAT_00eae008 = 0;
            iVar2 = 0;
          }
        }
      }
      DAT_00eae00c = DAT_00eae00c + 1;
      if (0xcb0f < DAT_00eae00c) {
        DAT_00eae00c = 0;
      }
    } while (DAT_00eae00c != iVar2);
  }
  return (&DAT_00eed760)[param_3] != 0;
}
