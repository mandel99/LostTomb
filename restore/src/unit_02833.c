#include "restore_types.h"
#define DAT_00604e78 (*(undefined *)0x00604e78u)
#define DAT_00604e7c (*(undefined *)0x00604e7cu)
#define DAT_00604e80 (*(undefined *)0x00604e80u)
extern void FUN_00566bae();
extern bool FUN_00566e21();
extern undefined * FUN_0056710d();

int * FUN_00562644(int *param_1)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  
  piVar1 = param_1;
  if (*param_1 < 0) {
    piVar1 = (int *)0x0;
  }
  else {
    FUN_00566bae();
    iVar2 = *piVar1;
    if ((iVar2 < 0x3f481) || (0x7ffc0b7e < iVar2)) {
      piVar1 = (int *)FUN_0056710d(piVar1);
      iVar3 = FUN_00566e21(piVar1);
      iVar2 = *piVar1;
      if (iVar3 != 0) {
        iVar2 = iVar2 - DAT_00604e80;
      }
      param_1 = (int *)(iVar2 - DAT_00604e78);
      iVar2 = (int)param_1 % 0x3c;
      *piVar1 = iVar2;
      if (iVar2 < 0) {
        *piVar1 = iVar2 + 0x3c;
        param_1 = (int *)((int)param_1 + -0x3c);
      }
      param_1 = (int *)((int)param_1 / 0x3c + piVar1[1]);
      iVar2 = (int)param_1 % 0x3c;
      piVar1[1] = iVar2;
      if (iVar2 < 0) {
        piVar1[1] = iVar2 + 0x3c;
        param_1 = (int *)((int)param_1 + -0x3c);
      }
      param_1 = (int *)((int)param_1 / 0x3c + piVar1[2]);
      iVar2 = (int)param_1 % 0x18;
      piVar1[2] = iVar2;
      if (iVar2 < 0) {
        piVar1[2] = iVar2 + 0x18;
        param_1 = (int *)((int)param_1 + -0x18);
      }
      iVar2 = (int)param_1 / 0x18;
      if (iVar2 < 1) {
        if (-1 < iVar2) {
          return piVar1;
        }
        piVar1[6] = (piVar1[6] + 7 + iVar2) % 7;
        piVar1[3] = piVar1[3] + iVar2;
        if (piVar1[3] < 1) {
          piVar1[5] = piVar1[5] + -1;
          piVar1[3] = piVar1[3] + 0x1f;
          piVar1[7] = 0x16c;
          piVar1[4] = 0xb;
          return piVar1;
        }
      }
      else {
        piVar1[6] = (piVar1[6] + iVar2) % 7;
        piVar1[3] = piVar1[3] + iVar2;
      }
      piVar1[7] = piVar1[7] + iVar2;
    }
    else {
      param_1 = (int *)(iVar2 - DAT_00604e78);
      piVar1 = (int *)FUN_0056710d(&param_1);
      if ((DAT_00604e7c != 0) && (iVar2 = FUN_00566e21(piVar1), iVar2 != 0)) {
        param_1 = (int *)((int)param_1 - DAT_00604e80);
        piVar1 = (int *)FUN_0056710d(&param_1);
        piVar1[8] = 1;
      }
    }
  }
  return piVar1;
}
