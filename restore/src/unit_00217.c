#include "restore_types.h"
#define DAT_00e38da4 (*(int32_t *)0x00e38da4u)
#define DAT_00e38da8 (*(int32_t *)0x00e38da8u)
#define DAT_00f55bd4 (*(uint32_t *)0x00f55bd4u)
extern void FUN_004cf1c0();
extern int FUN_0053e590();

int FUN_00417ee0(void)

{
  FUN_004cf1c0();
  return FUN_0053e590(DAT_00f55bd4,DAT_00e38da8,DAT_00e38da4);
}
