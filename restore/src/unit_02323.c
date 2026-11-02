#include "restore_types.h"
#define DAT_00af55fc (*(undefined4 *)0x00af55fcu)
#define DAT_00af5988 (*(undefined4 *)0x00af5988u)
#define DAT_00af5990 (*(undefined4 *)0x00af5990u)
#define DAT_00af5d1c (*(undefined4 *)0x00af5d1cu)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
#define DAT_00f56b84 (*(undefined *)0x00f56b84u)

undefined4 FUN_0051c390(int param_1)

{
  bool bVar1;
  int iVar2;
  
  bVar1 = false;
  iVar2 = (&DAT_00b7482c)[param_1] - DAT_00f56b84;
  if (0x2f < iVar2) {
    iVar2 = iVar2 + -0x30;
  }
  switch((int)(iVar2)) {
  case 0: ;
  case 7: ;
  case 9: ;
  case 0xf: ;
  case 0x11: ;
  case 0x13: ;
  case 0x15: ;
  case 0x1c: ;
  case 0x1e: ;
  case 0x24: ;
  case 0x26: ;
  case 0x28: ;
  case 0x2a: ;
  case 0x2c: ;
  case 0x2e: ;
    bVar1 = true;
    break;
  case 1: ;
  case 6: ;
  case 8: ;
  case 0x10: ;
  case 0x12: ;
  case 0x14: ;
  case 0x16: ;
  case 0x1b: ;
  case 0x1d: ;
  case 0x25: ;
  case 0x27: ;
  case 0x29: ;
  case 0x2b: ;
  case 0x2d: ;
  case 0x2f: ;
    break;
default: ;
    goto switchD_0051c3bb_caseD_2;
  }
  if ((DAT_00bca2e0 == 2) || (DAT_00bca2e0 == 6)) {
    bVar1 = (bool)(bVar1 ^ 1);
  }
  if (bVar1) {
    if (((&DAT_00af5990)[param_1 * 4] & 0x40) != 0) {
      return 0;
    }
    if (((&DAT_00af5988)[param_1 * 4] & 0x40) != 0) {
switchD_0051c3bb_caseD_2: ;
      return 0;
    }
  }
  else {
    if (((&DAT_00af55fc)[param_1 * 4] & 0x40) != 0) {
      return 0;
    }
    if (((&DAT_00af5d1c)[param_1 * 4] & 0x40) != 0) {
      return 0;
    }
  }
  return 1;
}
