#include "restore_types.h"
#define DAT_005cccc8 (*(undefined4 *)0x005cccc8u)
#define DAT_005dbda8 (*(undefined4 *)0x005dbda8u)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)

int FUN_0047cbd0(int param_1,int param_2)

{
  int iVar1;
  int *piVar2;
  int *piVar3;
  int iVar4;
  
  if (DAT_00bca2e0 != 0) {
    if (DAT_00bca2e0 == 6) {
      param_2 = param_2 + -1;
    }
    else if (DAT_00bca2e0 == 4) {
      param_1 = param_1 + -1;
      param_2 = param_2 + -1;
    }
    else if (DAT_00bca2e0 == 2) {
      param_1 = param_1 + -1;
    }
  }
  piVar3 = &DAT_005dbda8;
  param_1 = DAT_00bca2d8 + param_2 * 0xe4 + param_1;
  iVar1 = 0;
  do {
    iVar4 = 0;
    piVar2 = &DAT_005cccc8;
    do {
      if (*piVar3 == 0) {
        if ((*(byte *)(&DAT_00af598c + *piVar2 + param_1) & 4) == 0) goto LAB_0047cc42;
      }
      else if ((*(byte *)(&DAT_00af598c + *piVar2 + param_1) & 4) != 0) {
LAB_0047cc42: ;
        iVar4 = iVar4 + 1;
      }
      piVar2 = piVar2 + 1;
      piVar3 = piVar3 + 1;
    } while ((int)piVar2 < 0x5cccd8);
    if (iVar4 == 4) {
      return iVar1;
    }
    iVar1 = iVar1 + 1;
    if (0x5dbde7 < (int)piVar3) {
      return 999;
    }
  } while( true );
}
