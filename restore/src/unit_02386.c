#include "restore_types.h"
#define DAT_0077de58 (*(undefined4 *)0x0077de58u)
#define DAT_00784f18 (*(int32_t *)0x00784f18u)
#define DAT_00e38da4 (*(int32_t *)0x00e38da4u)
#define DAT_00e38da8 (*(int32_t *)0x00e38da8u)
#define DAT_00f56f8c (*(uint32_t *)0x00f56f8cu)
#define DAT_00f56fb4 (*(uint32_t *)0x00f56fb4u)
extern void FUN_004d6160();
extern void FUN_004d6250();
extern void FUN_004d62f0();
extern void FUN_00536820();

void FUN_00525b90(void)

{
  FUN_00536820(DAT_00e38da8 + 0x50,DAT_00e38da4 + 0x40,0x1e,0xe);
  FUN_004d6250(0x8e,0,DAT_00e38da8 + 0x50,DAT_00e38da4 + 0x50,0x1e0,DAT_00f56fb4,0,DAT_00e38da8,
               DAT_00e38da4);
  if (*(char *)((int)&DAT_0077de58 + DAT_00784f18 * 0x6a) == '\0') {
    FUN_004d6160(0x8e,1,DAT_00e38da8 + 0x70,DAT_00e38da4 + 0x98,0x1a0,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
  }
  else {
    FUN_004d6160(0x8e,1,DAT_00e38da8 + 0x70,DAT_00e38da4 + 0x78,0x1a0,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
    FUN_004d6160(0x8e,3,DAT_00e38da8 + 0x70,DAT_00e38da4 + 0xb8,0x1a0,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
  }
  FUN_004d62f0(0x8e,2,DAT_00e38da8 + 0x80,DAT_00e38da4 + 0x100,DAT_00f56f8c,0,DAT_00e38da8,
               DAT_00e38da4);
  return;
}
