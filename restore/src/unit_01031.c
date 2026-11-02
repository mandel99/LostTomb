#include "restore_types.h"
#define DAT_009ab9ec (*(undefined4 *)0x009ab9ecu)
#define DAT_009b84fc (*(undefined4 *)0x009b84fcu)
#define DAT_009d1b1c (*(undefined4 *)0x009d1b1cu)
#define DAT_009de62c (*(undefined4 *)0x009de62cu)
#define DAT_00ae8e7c (*(undefined4 *)0x00ae8e7cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b285cc (*(undefined4 *)0x00b285ccu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00b67d1c (*(undefined4 *)0x00b67d1cu)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00bc971c (*(undefined *)0x00bc971cu)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca2dc (*(undefined *)0x00bca2dcu)
extern void FUN_0040f860();
extern void FUN_00475720();
extern uint FUN_0047b8e0();
extern void FUN_00485830();

void FUN_004754a0(undefined4 param_1,int param_2,int param_3,int param_4,int param_5)

{
  byte bVar1;
  int iVar2;
  int iVar3;
  short *psVar4;
  int iVar5;
  int iVar6;
  
  iVar5 = param_2;
  if (param_4 < param_2) {
    param_2 = param_4;
    param_4 = iVar5;
  }
  iVar5 = param_5;
  if (param_5 < param_3) {
    iVar5 = param_3;
    param_3 = param_5;
  }
  FUN_0040f860(&DAT_009de62c,&DAT_00af598c,0x32c40);
  FUN_0040f860(&DAT_009d1b1c,&DAT_00ae8e7c,0xcb10);
  FUN_0040f860(&DAT_009b84fc,&DAT_00b285cc,0xcb10);
  FUN_0040f860(&DAT_009ab9ec,&DAT_00b67d1c,0xcb10);
  FUN_00485830(0);
  if (param_2 < 0) {
    param_2 = 0;
  }
  if (param_3 < 0) {
    param_3 = 0;
  }
  if (DAT_00bca2d0 <= param_4) {
    param_4 = DAT_00bca2d0 + -1;
  }
  if (DAT_00bca2d4 <= iVar5) {
    iVar5 = DAT_00bca2d4 + -1;
  }
  DAT_00bc971c = 0;
  iVar2 = DAT_00bca2d0 - param_4;
  iVar6 = DAT_00bca2d8 + param_3 * 0xe4 + param_2;
  iVar3 = DAT_00bca2d0;
  if (param_3 <= iVar5) {
    param_3 = (iVar5 - param_3) + 1;
    do {
      if (param_2 <= param_4) {
        psVar4 = (short *)(&DAT_00b4e6fc + iVar6 * 2);
        iVar5 = (param_4 - param_2) + 1;
        do {
          if ((((((&DAT_00af598c)[iVar6] & 0x40) != 0) && (((&DAT_00af598c)[iVar6] & 0xc) == 0)) &&
              (*psVar4 == 0)) && (iVar3 = FUN_0047b8e0(iVar6), iVar3 != 0)) {
            bVar1 = (&DAT_00b285cc)[iVar6];
            if ((bVar1 & 0x80) == 0) {
              DAT_00bc971c = DAT_00bc971c + 1;
            }
            (&DAT_00b7482c)[iVar6] = 0;
            (&DAT_00b285cc)[iVar6] = bVar1 & 0xf0 | 0x80;
            (&DAT_00b67d1c)[iVar6] = (&DAT_00b67d1c)[iVar6] | 0x40;
          }
          iVar6 = iVar6 + 1;
          psVar4 = psVar4 + 1;
          iVar5 = iVar5 + -1;
          iVar3 = DAT_00bca2d0;
        } while (iVar5 != 0);
      }
      iVar6 = iVar6 + DAT_00bca2dc + iVar2 + -1 + param_2;
      param_3 = param_3 + -1;
    } while (param_3 != 0);
  }
  FUN_00475720(0,0,iVar3 + -1,DAT_00bca2d4 + -1,1);
  return;
}
