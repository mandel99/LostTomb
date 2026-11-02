#include "restore_types.h"
#define DAT_00e38da4 (*(int32_t *)0x00e38da4u)
#define DAT_00e38da8 (*(int32_t *)0x00e38da8u)
#define DAT_00e38dc9 (*(char *)0x00e38dc9u)
#define DAT_00f55cbc (*(int32_t *)0x00f55cbcu)
#define _DAT_00f53f28 (*(undefined4 *)0x00f53f28u)
extern int FUN_0053e590();

void FUN_0041c2d0(void)

{
  FUN_0053e590(DAT_00f55cbc,DAT_00e38da8 + -0xc0,DAT_00e38da4 + -0x90);
  _DAT_00f53f28 = 100;
  DAT_00e38dc9 = 1;
  return;
}
