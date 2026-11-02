#include "restore_types.h"
#define DAT_00ac2d4c (*(undefined4 *)0x00ac2d4cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
extern uint FUN_004ec410();
extern bool FUN_005181c0();

undefined4
FUN_004eb4c0(int param_1,int param_2,int param_3,int *param_4,int *param_5,undefined4 param_6)

{
  int *piVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  
  piVar1 = param_4;
  iVar6 = param_3 * 2 + 1;
  *param_4 = param_1 - param_3;
  param_4 = (int *)0x0;
  *param_5 = param_2 - param_3;
  iVar3 = *piVar1 + (param_2 - param_3) * 0xe4 + DAT_00bca2d8;
  do {
    iVar5 = 0;
    DAT_00bc9768 = iVar3;
    if (0 < iVar6) {
      do {
        if (((((-1 < *piVar1) && (-1 < *param_5)) && (*piVar1 < DAT_00bca2d0)) &&
            ((*param_5 < DAT_00bca2d4 && ((&DAT_00ac2d4c)[DAT_00bc9768] == '\0')))) &&
           ((((&DAT_00af598c)[DAT_00bc9768] & 0x80000) == 0 &&
            ((cVar2 = FUN_004ec410((&DAT_00af598c)[DAT_00bc9768],param_6), cVar2 != '\0' &&
             (iVar4 = FUN_005181c0(param_1,param_2,*piVar1,*param_5), iVar4 != 0)))))) {
          return 1;
        }
        iVar5 = iVar5 + 1;
        DAT_00bc9768 = DAT_00bc9768 + 1;
        *piVar1 = *piVar1 + 1;
      } while (iVar5 < iVar6);
    }
    iVar5 = 0;
    DAT_00bc9768 = DAT_00bc9768 + -1;
    *piVar1 = *piVar1 + -1;
    if (0 < iVar6) {
      do {
        if ((((-1 < *piVar1) && (-1 < *param_5)) && (*piVar1 < DAT_00bca2d0)) &&
           (((*param_5 < DAT_00bca2d4 && (((&DAT_00af598c)[DAT_00bc9768] & 0x80000) == 0)) &&
            (((&DAT_00ac2d4c)[DAT_00bc9768] == '\0' &&
             ((cVar2 = FUN_004ec410((&DAT_00af598c)[DAT_00bc9768],param_6), cVar2 != '\0' &&
              (iVar4 = FUN_005181c0(param_1,param_2,*piVar1,*param_5), iVar4 != 0)))))))) {
          return 1;
        }
        iVar5 = iVar5 + 1;
        DAT_00bc9768 = DAT_00bc9768 + 0xe4;
        *param_5 = *param_5 + 1;
      } while (iVar5 < iVar6);
    }
    iVar5 = 0;
    DAT_00bc9768 = DAT_00bc9768 + -0xe4;
    *param_5 = *param_5 + -1;
    if (0 < iVar6) {
      do {
        if ((((-1 < *piVar1) && (-1 < *param_5)) && (*piVar1 < DAT_00bca2d0)) &&
           (((*param_5 < DAT_00bca2d4 && ((&DAT_00ac2d4c)[DAT_00bc9768] == '\0')) &&
            ((((&DAT_00af598c)[DAT_00bc9768] & 0x80000) == 0 &&
             ((cVar2 = FUN_004ec410((&DAT_00af598c)[DAT_00bc9768],param_6), cVar2 != '\0' &&
              (iVar4 = FUN_005181c0(param_1,param_2,*piVar1,*param_5), iVar4 != 0)))))))) {
          return 1;
        }
        iVar5 = iVar5 + 1;
        DAT_00bc9768 = DAT_00bc9768 + -1;
        *piVar1 = *piVar1 + -1;
      } while (iVar5 < iVar6);
    }
    iVar5 = 0;
    DAT_00bc9768 = DAT_00bc9768 + 1;
    *piVar1 = *piVar1 + 1;
    if (0 < iVar6) {
      do {
        if (((((-1 < *piVar1) && (-1 < *param_5)) && (*piVar1 < DAT_00bca2d0)) &&
            ((*param_5 < DAT_00bca2d4 && ((&DAT_00ac2d4c)[DAT_00bc9768] == '\0')))) &&
           ((((&DAT_00af598c)[DAT_00bc9768] & 0x80000) == 0 &&
            ((cVar2 = FUN_004ec410((&DAT_00af598c)[DAT_00bc9768],param_6), cVar2 != '\0' &&
             (iVar4 = FUN_005181c0(param_1,param_2,*piVar1,*param_5), iVar4 != 0)))))) {
          return 1;
        }
        iVar5 = iVar5 + 1;
        DAT_00bc9768 = DAT_00bc9768 + -0xe4;
        *param_5 = *param_5 + -1;
      } while (iVar5 < iVar6);
    }
    DAT_00bc9768 = DAT_00bc9768 + 0xe4;
    *param_5 = *param_5 + 1;
    if (param_3 == 0) {
      return 0;
    }
    param_4 = (int *)((int)param_4 + 1);
    iVar6 = iVar6 + -2;
    iVar3 = iVar3 + 0xe5;
    *piVar1 = *piVar1 + 1;
    param_3 = param_3 + -1;
    *param_5 = *param_5 + 1;
  } while ((int)param_4 < 0xe4);
  return 0;
}
