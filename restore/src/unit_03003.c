#include "restore_types.h"
#define DAT_006050d0 (*(undefined *)0x006050d0u)
#define DAT_006050d4 (*(undefined4 *)0x006050d4u)
#define DAT_00605150 (*(undefined *)0x00605150u)

undefined4 * FUN_00569dc7(int param_1)

{
  int *piVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  
  puVar2 = &DAT_006050d0;
  if (DAT_006050d4 != param_1) {
    puVar3 = puVar2;
    do {
      puVar2 = puVar3 + 3;
      if (&DAT_006050d0 + DAT_00605150 * 3 <= puVar2) break;
      piVar1 = puVar3 + 4;
      puVar3 = puVar2;
    } while (*piVar1 != param_1);
  }
  if ((&DAT_006050d0 + DAT_00605150 * 3 <= puVar2) || (puVar2[1] != param_1)) {
    puVar2 = (undefined4 *)0x0;
  }
  return puVar2;
}
