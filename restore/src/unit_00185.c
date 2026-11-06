#include "restore_types.h"
#define DAT_00e38da4 (*(int32_t *)0x00e38da4u)
#define DAT_00e38da8 (*(int32_t *)0x00e38da8u)
#define DAT_00f56f94 (*(undefined *)0x00f56f94u)
#define DAT_00f56fb4 (*(undefined *)0x00f56fb4u)
extern void FUN_004ccb30();
extern void FUN_00536820();

void FUN_00414c40(void)

{
  FUN_00536820(DAT_00e38da8 + 0x80,DAT_00e38da4 + 0xa0,0x18,7);
  FUN_004ccb30(0xb,5,DAT_00e38da8 + 0x80,DAT_00e38da4 + 0xbc,0x180,DAT_00f56fb4,0);
  FUN_004ccb30(0xb,6,DAT_00e38da8 + 0x80,DAT_00e38da4 + 0xdc,0x180,DAT_00f56f94,0);
  return;
}
