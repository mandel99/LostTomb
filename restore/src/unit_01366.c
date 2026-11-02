#include "restore_types.h"
#define DAT_00799aa0 (*(undefined4 *)0x00799aa0u)
#define DAT_00bcd656 (*(undefined4 *)0x00bcd656u)
extern undefined4 FUN_004f6950();

undefined4 FUN_004a43a0(int param_1)

{
  byte bVar1;
  
  bVar1 = *(byte *)((int)&DAT_00799aa0 + (short)(&DAT_00bcd656)[param_1 * 0xc2] * 0x108 + 3);
  if ((bVar1 != 0) && (bVar1 < 10)) {
    FUN_004f6950(param_1);
    return 1;
  }
  return 0;
}
