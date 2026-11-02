#include "restore_types.h"
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
#define DAT_00f566fc (*(undefined *)0x00f566fcu)

void FUN_004405c0(int param_1,int *param_2,int *param_3)

{
  switch((int)(DAT_00bca2e0)) {
  case 0: ;
    if (param_1 == 0) {
      *param_3 = DAT_00f566fc;
      *param_2 = DAT_00f566fc + 1;
      return;
    }
    if (param_1 == 1) {
      *param_3 = DAT_00f566fc + 3;
      *param_2 = DAT_00f566fc + 2;
      return;
    }
    break;
  case 2: ;
    if (param_1 == 0) {
      *param_3 = DAT_00f566fc + 3;
      *param_2 = DAT_00f566fc + 2;
      return;
    }
    if (param_1 == 1) {
LAB_00440625: ;
      *param_3 = DAT_00f566fc + 1;
      *param_2 = DAT_00f566fc;
      return;
    }
    break;
  case 4: ;
    if (param_1 == 0) goto LAB_00440625;
    if (param_1 == 1) {
      *param_3 = DAT_00f566fc + 2;
      *param_2 = DAT_00f566fc + 3;
      return;
    }
    break;
  case 6: ;
    if (param_1 == 0) {
      *param_3 = DAT_00f566fc + 2;
      *param_2 = DAT_00f566fc + 3;
      return;
    }
    if (param_1 == 1) {
      *param_3 = DAT_00f566fc;
      *param_2 = DAT_00f566fc + 1;
    }
  }
  return;
}
