#include "restore_types.h"
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)

int FUN_004ec0f0(int param_1,int param_2,int *param_3,int *param_4,int param_5,undefined4 param_6,
                code *param_7)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  iVar3 = param_2;
  iVar4 = 1;
  param_2 = 0;
  iVar2 = DAT_00bca2d8 + iVar3 * 0xe4 + param_1;
  *param_3 = param_1;
  *param_4 = iVar3;
  do {
    iVar3 = 0;
    DAT_00bc9768 = iVar2;
    if (0 < iVar4) {
      do {
        if ((((-1 < *param_3) && (-1 < *param_4)) && (*param_3 < DAT_00bca2d0)) &&
           (((*param_4 < DAT_00bca2d4 && (((&DAT_00af598c)[DAT_00bc9768] & 0x80000) == 0)) &&
            (iVar1 = (*param_7)(DAT_00bc9768,param_6), iVar1 != 0)))) {
          return iVar1;
        }
        iVar3 = iVar3 + 1;
        DAT_00bc9768 = DAT_00bc9768 + 1;
        *param_3 = *param_3 + 1;
      } while (iVar3 < iVar4);
    }
    iVar3 = 0;
    DAT_00bc9768 = DAT_00bc9768 + -1;
    *param_3 = *param_3 + -1;
    if (0 < iVar4) {
      do {
        if (((-1 < *param_3) && (-1 < *param_4)) &&
           ((*param_3 < DAT_00bca2d0 &&
            (((*param_4 < DAT_00bca2d4 && (((&DAT_00af598c)[DAT_00bc9768] & 0x80000) == 0)) &&
             (iVar1 = (*param_7)(DAT_00bc9768,param_6), iVar1 != 0)))))) {
          return iVar1;
        }
        iVar3 = iVar3 + 1;
        DAT_00bc9768 = DAT_00bc9768 + 0xe4;
        *param_4 = *param_4 + 1;
      } while (iVar3 < iVar4);
    }
    iVar3 = 0;
    DAT_00bc9768 = DAT_00bc9768 + -0xe4;
    *param_4 = *param_4 + -1;
    if (0 < iVar4) {
      do {
        if (((-1 < *param_3) && (-1 < *param_4)) &&
           (((*param_3 < DAT_00bca2d0 &&
             ((*param_4 < DAT_00bca2d4 && (((&DAT_00af598c)[DAT_00bc9768] & 0x80000) == 0)))) &&
            (iVar1 = (*param_7)(DAT_00bc9768,param_6), iVar1 != 0)))) {
          return iVar1;
        }
        iVar3 = iVar3 + 1;
        DAT_00bc9768 = DAT_00bc9768 + -1;
        *param_3 = *param_3 + -1;
      } while (iVar3 < iVar4);
    }
    iVar3 = 0;
    DAT_00bc9768 = DAT_00bc9768 + 1;
    *param_3 = *param_3 + 1;
    if (0 < iVar4) {
      do {
        if (((((-1 < *param_3) && (-1 < *param_4)) && (*param_3 < DAT_00bca2d0)) &&
            ((*param_4 < DAT_00bca2d4 && (((&DAT_00af598c)[DAT_00bc9768] & 0x80000) == 0)))) &&
           (iVar1 = (*param_7)(DAT_00bc9768,param_6), iVar1 != 0)) {
          return iVar1;
        }
        iVar3 = iVar3 + 1;
        DAT_00bc9768 = DAT_00bc9768 + -0xe4;
        *param_4 = *param_4 + -1;
      } while (iVar3 < iVar4);
    }
    DAT_00bc9768 = DAT_00bc9768 + 0xe4;
    *param_4 = *param_4 + 1;
    if (param_5 == 0) {
      return 0;
    }
    param_2 = param_2 + 1;
    iVar4 = iVar4 + 2;
    iVar2 = iVar2 + -0xe5;
    *param_3 = *param_3 + -1;
    param_5 = param_5 + -1;
    *param_4 = *param_4 + -1;
  } while (param_2 < 0xe4);
  return 0;
}
