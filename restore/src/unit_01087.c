#include "restore_types.h"
#define DAT_00ab623c (*(undefined4 *)0x00ab623cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b285cc (*(undefined4 *)0x00b285ccu)
#define DAT_00b67d1c (*(undefined4 *)0x00b67d1cu)
#define DAT_00bc96cc (*(undefined *)0x00bc96ccu)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)

void FUN_00479460(int param_1,int param_2)

{
  byte bVar1;
  
  param_1 = DAT_00bca2d8 + param_2 * 0xe4 + param_1;
  DAT_00bc9768 = param_1;
  if (((&DAT_00af598c)[param_1] & 0x400) != 0) {
    bVar1 = (&DAT_00b285cc)[param_1];
    (&DAT_00af598c)[param_1] = (&DAT_00af598c)[param_1] & 0xfffffbff | 0x200;
    (&DAT_00b285cc)[param_1] = bVar1 & 0xf0;
    (&DAT_00b67d1c)[param_1] = (&DAT_00b67d1c)[param_1] | 0x40;
  }
  bVar1 = (&DAT_00ab623c)[param_1];
  if (bVar1 == 0) {
    (&DAT_00af598c)[param_1] = (&DAT_00af598c)[param_1] & 0xfffffdff;
    return;
  }
  if (bVar1 == DAT_00bc96cc) {
    (&DAT_00af598c)[param_1] = (&DAT_00af598c)[param_1] & 0xfffff9ff;
    (&DAT_00ab623c)[param_1] = bVar1 - 1;
  }
  return;
}
