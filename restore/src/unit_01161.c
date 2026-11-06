#include "restore_types.h"
#define DAT_005f80fc (*(undefined4 *)0x005f80fcu)
#define DAT_00af55fc (*(undefined4 *)0x00af55fcu)
#define DAT_00af5988 (*(undefined4 *)0x00af5988u)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00af5990 (*(undefined4 *)0x00af5990u)
#define DAT_00af5d1c (*(undefined4 *)0x00af5d1cu)
#define DAT_00bc96f4 (*(undefined *)0x00bc96f4u)
#define DAT_00bc96f8 (*(undefined *)0x00bc96f8u)
#define DAT_00bc96fc (*(undefined *)0x00bc96fcu)
#define DAT_00bc9700 (*(undefined *)0x00bc9700u)
#define DAT_00bca2d8 (*(uint32_t *)0x00bca2d8u)
#define DAT_00ee0c50 (*(undefined4 *)0x00ee0c50u)
#define _DAT_00bc9704 (*(undefined4 *)0x00bc9704u)
extern char FUN_0047ec80();
extern char FUN_0047ecc0();

void FUN_0047f200(int param_1,int param_2)

{
  char cVar1;
  uint uVar2;
  int iVar3;
  int *piVar4;
  int iVar5;
  
  DAT_00bc96fc = 0;
  DAT_00bc96f8 = 0;
  DAT_00bc9700 = 0;
  DAT_00bc96f4 = 0;
  param_1 = DAT_00bca2d8 + param_2 * 0xe4 + param_1;
  _DAT_00bc9704 = param_1;
  if ((((((&DAT_00af598c)[param_1] & 4) != 0) && (((&DAT_00af598c)[param_1] & 0x48) == 0)) &&
      (iVar3 = FUN_0047ec80(param_1,4), iVar3 == 3)) &&
     (iVar3 = FUN_0047ec80(param_1,0x10000), iVar3 == 0)) {
    if (((&DAT_00af55fc)[param_1 * 4] & 4) == 0) {
      DAT_00bc96f4 = 4;
      DAT_00bc9700 = 0xe4;
      DAT_00bc96f8 = 1;
      iVar3 = 0;
    }
    else if (((&DAT_00af5990)[param_1 * 4] & 4) == 0) {
      DAT_00bc96f4 = 6;
      DAT_00bc9700 = -1;
      DAT_00bc96f8 = 1;
      iVar3 = 0;
    }
    else if (((&DAT_00af5d1c)[param_1 * 4] & 4) == 0) {
      DAT_00bc96f4 = 0;
      DAT_00bc9700 = -0xe4;
      DAT_00bc96f8 = 1;
      iVar3 = 0;
    }
    else {
      if (((&DAT_00af5988)[param_1 * 4] & 4) != 0) {
        return;
      }
      DAT_00bc96f4 = 2;
      DAT_00bc9700 = 1;
      DAT_00bc96f8 = 1;
      iVar3 = 0;
    }
    while( true ) {
      param_1 = param_1 + DAT_00bc9700;
      DAT_00bc96f8 = DAT_00bc96f8 + 1;
      uVar2 = (&DAT_00af598c)[DAT_00bc9700 + param_1];
      if ((uVar2 & 1) != 0) {
        return;
      }
      if ((((&DAT_00af598c)[param_1] & 0x4000000) != 0) && (((&DAT_00af598c)[param_1] & 0x40) != 0))
      {
        return;
      }
      piVar4 = &DAT_005f80fc;
      do {
        cVar1 = *(char *)((int)&DAT_00ee0c50 + *piVar4 + param_1);
        if (cVar1 == '\x01') {
          return;
        }
        if (cVar1 == '\x03') {
          return;
        }
        piVar4 = piVar4 + 1;
      } while ((int)piVar4 < 0x5f811c);
      if ((uVar2 & 4) == 0) break;
      if ((uVar2 & 0x48) != 0) {
        return;
      }
      iVar5 = FUN_0047ecc0(param_1,4);
      if (iVar5 != 4) {
        return;
      }
      iVar3 = iVar3 + 1;
      if (3 < iVar3) {
        return;
      }
    }
    DAT_00bc96fc = param_1;
    iVar3 = FUN_0047ec80(param_1,4);
    if ((iVar3 != 3) || (iVar3 = FUN_0047ec80(param_1,0x10000), iVar3 != 0)) {
      DAT_00bc96fc = 0;
    }
  }
  return;
}
