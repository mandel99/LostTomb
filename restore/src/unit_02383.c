#include "restore_types.h"
#define DAT_007056c8 (*(int32_t *)0x007056c8u)
#define DAT_00e38da4 (*(int32_t *)0x00e38da4u)
#define DAT_00e38da8 (*(int32_t *)0x00e38da8u)
#define DAT_00f53f08 (*(int32_t *)0x00f53f08u)
#define DAT_00f53f0c (*(int32_t *)0x00f53f0cu)
extern void FUN_004d63d0();

void FUN_005253c0(void)

{
  FUN_004d63d0(0,DAT_00f53f0c + 0x80,DAT_00f53f08 + 0xd8,0x40,0x14,DAT_007056c8 == 3,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d63d0(0,DAT_00f53f0c + 0xd0,DAT_00f53f08 + 0xd8,0x40,0x14,DAT_007056c8 == 4,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d63d0(0,DAT_00f53f0c + 0x120,DAT_00f53f08 + 0xd8,0x40,0x14,DAT_007056c8 == 5,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d63d0(0,DAT_00f53f0c + 0x170,DAT_00f53f08 + 0xd8,0x40,0x14,DAT_007056c8 == 6,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d63d0(0,DAT_00f53f0c + 0x1c0,DAT_00f53f08 + 0xd8,0x40,0x14,DAT_007056c8 == 7,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d63d0(0,DAT_00f53f0c + 0x150,DAT_00f53f08 + 0x11b,0xa0,0x14,DAT_007056c8 == 1,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d63d0(0,DAT_00f53f0c + 0x90,DAT_00f53f08 + 0x11b,0xa0,0x14,DAT_007056c8 == 2,DAT_00e38da8,
               DAT_00e38da4);
  return;
}
