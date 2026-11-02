#include "restore_types.h"
#define DAT_00e38da4 (*(int32_t *)0x00e38da4u)
#define DAT_00e38da8 (*(int32_t *)0x00e38da8u)
#define DAT_00f5732c (*(int32_t *)0x00f5732cu)
#define _DAT_00f53f28 (*(undefined4 *)0x00f53f28u)
extern int FUN_0053e590();

void FUN_00520af0(void)

{
  FUN_0053e590(DAT_00f5732c,DAT_00e38da8,DAT_00e38da4);
  _DAT_00f53f28 = 100;
  return;
}
