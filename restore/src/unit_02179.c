#include "restore_types.h"
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00eaac30 (*(undefined4 *)0x00eaac30u)
#define DAT_00eaaf64 (*(undefined *)0x00eaaf64u)

void FUN_004f60d0(int param_1)

{
  int iVar1;
  int *piVar2;
  
  if ((&DAT_007999e8)[param_1 * 0x84] == 0xb7) {
    iVar1 = 0;
    if (0 < DAT_00eaaf64) {
      piVar2 = &DAT_00eaac30;
      do {
        if (*piVar2 == param_1 - (short)(&DAT_007999ea)[param_1 * 0x84]) {
          return;
        }
        iVar1 = iVar1 + 1;
        piVar2 = piVar2 + 1;
      } while (iVar1 < DAT_00eaaf64);
    }
    if (DAT_00eaaf64 < 4) {
      (&DAT_00eaac30)[DAT_00eaaf64] = param_1 - (short)(&DAT_007999ea)[param_1 * 0x84];
      DAT_00eaaf64 = DAT_00eaaf64 + 1;
    }
  }
  return;
}
