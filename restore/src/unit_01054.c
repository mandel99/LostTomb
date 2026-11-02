#include "restore_types.h"
#define DAT_00a9cc1c (*(undefined4 *)0x00a9cc1cu)
#define DAT_00adc36c (*(undefined4 *)0x00adc36cu)
#define DAT_00ae8e7c (*(undefined4 *)0x00ae8e7cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b285cc (*(undefined4 *)0x00b285ccu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00b67d1c (*(undefined4 *)0x00b67d1cu)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define _DAT_00bc972c (*(undefined4 *)0x00bc972cu)
#define _DAT_00bc9730 (*(undefined4 *)0x00bc9730u)

void FUN_00477960(int param_1,int param_2)

{
  byte bVar1;
  uint uVar2;
  
  param_1 = DAT_00bca2d8 + param_2 * 0xe4 + param_1;
  DAT_00bc9768 = param_1;
  uVar2 = (&DAT_00af598c)[param_1];
  if (uVar2 == 0) {
    _DAT_00bc9730 = 1;
    return;
  }
  (&DAT_00ae8e7c)[param_1] = 0;
  (&DAT_00af598c)[param_1] = uVar2 & 0x99caf680;
  bVar1 = (&DAT_00b285cc)[param_1];
  *(undefined2 *)(&DAT_00b4e6fc + param_1 * 2) = 0;
  (&DAT_00b285cc)[param_1] = bVar1 & 0xe0;
  bVar1 = (&DAT_00b67d1c)[param_1];
  *(undefined2 *)(&DAT_00a9cc1c + param_1 * 2) = 0;
  (&DAT_00adc36c)[param_1] = 0;
  (&DAT_00b67d1c)[param_1] = bVar1 & 0xc0 | 0x40;
  _DAT_00bc972c = 1;
  return;
}
