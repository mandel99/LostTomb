#include "restore_types.h"
#define DAT_00e38da4 (*(int32_t *)0x00e38da4u)
#define DAT_00e38da8 (*(int32_t *)0x00e38da8u)
#define DAT_00f55c24 (*(int32_t *)0x00f55c24u)
#define _DAT_00f53f28 (*(undefined4 *)0x00f53f28u)
extern void FUN_004cf1c0();
extern int FUN_0053e590();

void FUN_00520ab0(int param_1)

{
  if (param_1 == 0) {
    FUN_004cf1c0();
  }
  FUN_0053e590(DAT_00f55c24,DAT_00e38da8,DAT_00e38da4);
  _DAT_00f53f28 = 100;
  return;
}
