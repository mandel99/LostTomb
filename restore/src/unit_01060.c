#include "restore_types.h"
#define DAT_00ab623c (*(undefined4 *)0x00ab623cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define _DAT_00bc972c (*(undefined4 *)0x00bc972cu)

void FUN_00477bd0(int param_1,int param_2)

{
  DAT_00bc9768 = DAT_00bca2d8 + param_2 * 0xe4 + param_1;
  if ((&DAT_00ab623c)[DAT_00bc9768] == '\0') {
    _DAT_00bc972c = 1;
    (&DAT_00af598c)[DAT_00bc9768] = (&DAT_00af598c)[DAT_00bc9768] & 0x99cbf680 | 0x10000;
  }
  return;
}
