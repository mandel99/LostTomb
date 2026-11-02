#include "restore_types.h"
#define DAT_006050d0 (*(undefined *)0x006050d0u)
#define DAT_00605150 (*(undefined *)0x00605150u)

int * FUN_005678f9(int param_1)

{
  int *piVar1;
  
  piVar1 = &DAT_006050d0;
  if (DAT_006050d0 != param_1) {
    do {
      piVar1 = piVar1 + 3;
      if (&DAT_006050d0 + DAT_00605150 * 3 <= piVar1) break;
    } while (*piVar1 != param_1);
  }
  if ((&DAT_006050d0 + DAT_00605150 * 3 <= piVar1) || (*piVar1 != param_1)) {
    piVar1 = (int *)0x0;
  }
  return piVar1;
}
