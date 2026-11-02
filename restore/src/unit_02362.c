#include "restore_types.h"
#define DAT_007056c8 (*(int32_t *)0x007056c8u)
#define DAT_00e38da4 (*(int32_t *)0x00e38da4u)
#define DAT_00e38da8 (*(int32_t *)0x00e38da8u)
#define DAT_00e38e74 (*(char *)0x00e38e74u)
#define DAT_00f56f8c (*(int32_t *)0x00f56f8cu)
#define DAT_00f56f9c (*(int32_t *)0x00f56f9cu)
#define DAT_00f56fb4 (*(int32_t *)0x00f56fb4u)
extern void FUN_004cc540();
extern void FUN_004cc770();
extern void FUN_004d6250();
extern void FUN_004d62f0();
extern void FUN_004d63d0();
extern void FUN_004d65b0();
extern void FUN_00536820();
extern void FUN_00537140();

void FUN_00520b90(void)

{
  undefined4 uVar1;
  
  FUN_00536820(DAT_00e38da8 + 0x80,DAT_00e38da4 + 0xa0,0x18,8);
  FUN_004d6250(0x1f,0,DAT_00e38da8 + 0x80,DAT_00e38da4 + 0xac,0x180,DAT_00f56fb4,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d62f0(0xd,5,DAT_00e38da8 + 0x18b,DAT_00e38da4 + 0xff,DAT_00f56f8c,0,DAT_00e38da8,
               DAT_00e38da4);
  if (DAT_007056c8 != 1) {
    uVar1 = 0;
  }
  else {
    uVar1 = 0xf800;
  }
  FUN_004d63d0(0,DAT_00e38da8 + 0x160,DAT_00e38da4 + 0xf6,0x19,0x19,DAT_007056c8 == 1,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d6250(0xd,8,DAT_00e38da8 + 0x91,DAT_00e38da4 + 0x100,0xb4,DAT_00f56f8c,uVar1,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d65b0(DAT_00e38da8 + 0xa0,DAT_00e38da4 + 0xd0,0x14,2,DAT_00e38da8,DAT_00e38da4);
  FUN_004cc540();
  FUN_004cc770(&DAT_00e38e74,DAT_00e38da8 + 0xb0,DAT_00e38da4 + 0xd8,DAT_00f56f9c,0);
  FUN_00537140(DAT_00e38da8 + 0xb0,DAT_00e38da4 + 0xd9,0xffff);
  return;
}
