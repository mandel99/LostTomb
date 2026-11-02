#include "restore_types.h"
#define DAT_00799aa0 (*(undefined4 *)0x00799aa0u)
#define DAT_009783c0 (*(undefined4 *)0x009783c0u)
#define DAT_009783d8 (*(undefined4 *)0x009783d8u)

void FUN_0046a2e0(int param_1)

{
  int iVar1;
  
  iVar1 = *(byte *)((int)&DAT_00799aa0 + param_1 * 0x108 + 3) - 1;
  if ((-1 < iVar1) && (iVar1 < 10)) {
    (&DAT_009783c0)[iVar1 * 0x1c] = 0;
    (&DAT_009783d8)[iVar1 * 7] = 0;
  }
  return;
}
