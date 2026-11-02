#include "restore_types.h"
#define DAT_005d9a40 (*(undefined *)0x005d9a40u)
#define DAT_0078a634 (*(char *)0x0078a634u)

uint FUN_0044c430(uint param_1)

{
  int *piVar1;
  uint in_EAX;
  undefined3 uVar2;
  int *in_ECX;
  
  if (DAT_005d9a40 < 0) {
    return in_EAX & 0xffffff00;
  }
  if ((int)param_1 < DAT_005d9a40) {
    return param_1 & 0xffffff00;
  }
  if (((-1 < (int)param_1) && ((int)param_1 < *in_ECX)) &&
     (piVar1 = in_ECX + param_1 * 0x1f + 1, param_1 = 0, piVar1 != (int *)0x0)) {
    uVar2 = (undefined3)((uint)piVar1 >> 8);
    if (*(char *)((int)piVar1 + 2) == '\x02') {
      return CONCAT31(uVar2,DAT_0078a634 != '\0');
    }
    return CONCAT31(uVar2,DAT_0078a634 == '\0');
  }
  return param_1 & 0xffffff00;
}
