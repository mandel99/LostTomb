#include "restore_types.h"
#define DAT_005cc8c0 (*(undefined *)0x005cc8c0u)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00bc9738 (*(undefined *)0x00bc9738u)
#define DAT_00bc973c (*(undefined *)0x00bc973cu)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
#define DAT_00d05720 (*(undefined4 *)0x00d05720u)
extern bool FUN_0046f9a0();

undefined4 FUN_0047d530(int param_1,int *param_2,int param_3,uint param_4,char param_5)

{
  char cVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  
  DAT_00bc9738 = param_1;
  DAT_00bc973c = param_1;
  iVar2 = param_3;
  if (DAT_00bca2e0 != 0) {
    if (DAT_00bca2e0 == 6) {
      iVar2 = param_3 + (1 - param_1);
    }
    else if (DAT_00bca2e0 == 4) {
      param_2 = (int *)((int)param_2 + (1 - param_1));
      iVar2 = param_3 + (1 - param_1);
    }
    else if (DAT_00bca2e0 == 2) {
      param_2 = (int *)((int)param_2 + (1 - param_1));
    }
  }
  if ((int)param_2 < 0) {
    return 0;
  }
  if (iVar2 < 0) {
    return 0;
  }
  if (DAT_00bca2d0 <= param_1 + -1 + (int)param_2) {
    return 0;
  }
  if (DAT_00bca2d4 <= param_1 + -1 + iVar2) {
    return 0;
  }
  param_3 = 0;
  DAT_00bc9768 = DAT_00bca2d8 + iVar2 * 0xe4 + (int)param_2;
  if (0 < param_1) {
    param_2 = &DAT_005cc8c0;
    do {
      iVar2 = 0;
      piVar4 = param_2;
      if (0 < param_1) {
        do {
          iVar3 = *piVar4 + DAT_00bc9768;
          cVar1 = FUN_0046f9a0(iVar3 % 0xe4,iVar3 / 0xe4,3);
          if (cVar1 != '\0') {
            return 0;
          }
          if ((((&DAT_00af598c)[iVar3] & param_4 & 0x10000) != 0) && (param_5 == '\0')) {
            return 0;
          }
          if (((&DAT_00af598c)[iVar3] & param_4 & 0xeefed77f) != 0) {
            return 0;
          }
          if ((*(int *)(&DAT_00d05720 + iVar3 * 4) != 0) &&
             (*(char *)(*(int *)(&DAT_00d05720 + iVar3 * 4) + 0x18) == '\0')) {
            return 0;
          }
          iVar2 = iVar2 + 1;
          piVar4 = piVar4 + 2;
          param_1 = DAT_00bc973c;
        } while (iVar2 < DAT_00bc973c);
      }
      param_3 = param_3 + 1;
      param_2 = param_2 + 0xc;
    } while (param_3 < DAT_00bc9738);
  }
  return 1;
}
