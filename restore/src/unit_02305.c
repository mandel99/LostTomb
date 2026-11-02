#include "restore_types.h"
#define DAT_00af5988 (*(undefined4 *)0x00af5988u)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00af5990 (*(undefined4 *)0x00af5990u)
#define DAT_00af5d1c (*(undefined4 *)0x00af5d1cu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00eadfd4 (*(undefined *)0x00eadfd4u)
#define DAT_00eae004 (*(undefined *)0x00eae004u)
#define DAT_00eae008 (*(undefined *)0x00eae008u)
#define DAT_00eae00c (*(undefined *)0x00eae00cu)
#define DAT_00ee0b6c (*(undefined4 *)0x00ee0b6cu)
#define DAT_00ee0c4f (*(undefined4 *)0x00ee0c4fu)
#define DAT_00ee0c50 (*(undefined4 *)0x00ee0c50u)
#define DAT_00ee0d34 (*(undefined4 *)0x00ee0d34u)
#define DAT_00eed75c (*(undefined4 *)0x00eed75cu)
#define DAT_00eed760 (*(undefined4 *)0x00eed760u)
#define DAT_00f203a0 (*(undefined *)0x00f203a0u)
extern void FUN_0040f8f0();
extern undefined4 FUN_0051aff0();

int FUN_005190a0(int param_1,int param_2,int *param_3,int param_4)

{
  int iVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  
  FUN_0040f8f0(&DAT_00eed760,0x32c40,0);
  if (0 < param_4) {
    iVar4 = -1;
    do {
      iVar7 = *param_3;
      param_3 = param_3 + 1;
      (&DAT_00eed760)[iVar7] = iVar4;
      iVar4 = iVar4 + -1;
      param_4 = param_4 + -1;
    } while (param_4 != 0);
    DAT_00eae004 = DAT_00bca2d8 + param_2 * 0xe4 + param_1;
    DAT_00eadfd4 = DAT_00eadfd4 + 1;
    (&DAT_00eed760)[DAT_00eae004] = 1;
    DAT_00f203a0 = DAT_00eae004;
    iVar4 = 1;
    DAT_00eae00c = 0;
    DAT_00eae008 = 1;
    iVar7 = -1;
    do {
      iVar3 = (&DAT_00f203a0)[DAT_00eae00c];
      iVar1 = iVar3 * 4;
      iVar6 = iVar3 + -0xe4;
      iVar8 = (&DAT_00eed760)[iVar3] + 1;
      if (((((int)(&DAT_00eed760)[iVar6] < 1) &&
           (iVar5 = FUN_0051aff0(iVar6), iVar4 = DAT_00eae008, iVar5 != 0)) &&
          (((cVar2 = (&DAT_00ee0b6c)[iVar3], -1 < cVar2 || (cVar2 == -3)) ||
           ((cVar2 == -1 && (((&DAT_00af598c)[iVar6] & 0x1000) != 0)))))) &&
         ((cVar2 < '\x05' && (cVar2 != '\x03')))) {
        iVar7 = -1 - (&DAT_00eed760)[iVar6];
        (&DAT_00eed760)[iVar6] = iVar8;
        if (-1 < iVar7) goto LAB_00519377;
        (&DAT_00f203a0)[DAT_00eae008] = iVar6;
        DAT_00eae008 = DAT_00eae008 + 1;
        iVar4 = DAT_00eae008;
        if (0xcb0f < DAT_00eae008) {
          DAT_00eae008 = 0;
          iVar4 = 0;
        }
      }
      iVar6 = iVar3 + 1;
      if (((((int)(&DAT_00eed760)[iVar6] < 1) &&
           (iVar5 = FUN_0051aff0(iVar6), iVar4 = DAT_00eae008, iVar5 != 0)) &&
          (((cVar2 = *(char *)((int)&DAT_00ee0c50 + iVar3 + 1), -1 < cVar2 || (cVar2 == -3)) ||
           ((cVar2 == -1 && ((*(uint *)(&DAT_00af5990 + iVar3 * 4) & 0x1000) != 0)))))) &&
         ((cVar2 < '\x05' && (cVar2 != '\x03')))) {
        iVar7 = -1 - (&DAT_00eed760)[iVar6];
        (&DAT_00eed760)[iVar6] = iVar8;
        if (-1 < iVar7) goto LAB_00519377;
        (&DAT_00f203a0)[DAT_00eae008] = iVar6;
        DAT_00eae008 = DAT_00eae008 + 1;
        iVar4 = DAT_00eae008;
        if (0xcb0f < DAT_00eae008) {
          DAT_00eae008 = 0;
          iVar4 = 0;
        }
      }
      iVar6 = iVar3 + 0xe4;
      if (((((int)(&DAT_00eed760)[iVar6] < 1) &&
           (iVar5 = FUN_0051aff0(iVar6), iVar4 = DAT_00eae008, iVar5 != 0)) &&
          (((cVar2 = (&DAT_00ee0d34)[iVar3], -1 < cVar2 || (cVar2 == -3)) ||
           ((cVar2 == -1 && ((*(uint *)(&DAT_00af5d1c + iVar3 * 4) & 0x1000) != 0)))))) &&
         ((cVar2 < '\x05' && (cVar2 != '\x03')))) {
        iVar7 = -1 - (&DAT_00eed760)[iVar6];
        (&DAT_00eed760)[iVar6] = iVar8;
        if (-1 < iVar7) goto LAB_00519377;
        (&DAT_00f203a0)[DAT_00eae008] = iVar6;
        DAT_00eae008 = DAT_00eae008 + 1;
        iVar4 = DAT_00eae008;
        if (0xcb0f < DAT_00eae008) {
          DAT_00eae008 = 0;
          iVar4 = 0;
        }
      }
      if (*(int *)(&DAT_00eed75c + iVar1) < 1) {
        iVar6 = FUN_0051aff0(iVar3 + -1);
        iVar4 = DAT_00eae008;
        if ((iVar6 != 0) &&
           ((((cVar2 = (&DAT_00ee0c4f)[iVar3], -1 < cVar2 || (cVar2 == -3)) ||
             ((cVar2 == -1 && ((*(uint *)(&DAT_00af5988 + iVar3 * 4) & 0x1000) != 0)))) &&
            ((cVar2 < '\x05' && (cVar2 != '\x03')))))) {
          iVar7 = -1 - *(int *)(&DAT_00eed75c + iVar1);
          if (-1 < iVar7) goto LAB_00519377;
          *(int *)(&DAT_00eed75c + iVar1) = iVar8;
          (&DAT_00f203a0)[DAT_00eae008] = iVar3 + -1;
          DAT_00eae008 = DAT_00eae008 + 1;
          iVar4 = DAT_00eae008;
          if (0xcb0f < DAT_00eae008) {
            DAT_00eae008 = 0;
            iVar4 = 0;
          }
        }
      }
      DAT_00eae00c = DAT_00eae00c + 1;
      if (0xcb0f < DAT_00eae00c) {
        DAT_00eae00c = 0;
      }
    } while (DAT_00eae00c != iVar4);
    if (-1 < iVar7) {
LAB_00519377: ;
      return iVar7 + 1;
    }
  }
  return 0;
}
