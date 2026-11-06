#include "restore_types.h"
#define DAT_00e20778 (*(int32_t *)0x00e20778u)
#define DAT_00e38da4 (*(int32_t *)0x00e38da4u)
#define DAT_00e38da8 (*(int32_t *)0x00e38da8u)
#define DAT_00ea5dcc (*(undefined4 *)0x00ea5dccu)
#define DAT_00eaac28 (*(uint32_t *)0x00eaac28u)
#define DAT_00f53f08 (*(int32_t *)0x00f53f08u)
#define DAT_00f53f0c (*(int32_t *)0x00f53f0cu)
#define DAT_00f56f8c (*(uint32_t *)0x00f56f8cu)
#define DAT_00f56fb4 (*(uint32_t *)0x00f56fb4u)
#define DAT_00f571fc (*(uint32_t *)0x00f571fcu)
extern void FUN_004ccc30();
extern void FUN_004d6250();
extern void FUN_004d62f0();
extern void FUN_004d65b0();
extern void FUN_00536820();
extern int FUN_0053e590();

void FUN_005255a0(void)

{
  FUN_00536820(DAT_00f53f0c + 0x60,DAT_00f53f08 + 0x90,0x1e,0xf);
  FUN_0053e590(DAT_00f571fc + 0x15,DAT_00f53f0c + 0x80,DAT_00f53f08 + 0xa0);
  FUN_004d6250(0x34,0x45,DAT_00f53f0c + 0x80,DAT_00f53f08 + 0xa0,0x1b0,DAT_00f56fb4,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d65b0(DAT_00f53f0c + 0x70,DAT_00f53f08 + 0xd0,0x1c,5,DAT_00e38da8,DAT_00e38da4);
  DAT_00e20778 = 0;
  FUN_004d62f0(0x34,0x32,DAT_00f53f0c + 0x90,DAT_00f53f08 + 0x130,DAT_00f56f8c,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004ccc30(8,4,(&DAT_00ea5dcc)[DAT_00eaac28 * 0x1276],DAT_00e20778 + 0x90 + DAT_00f53f0c,
               DAT_00f53f08 + 0x130,DAT_00f56f8c,0);
  FUN_004d6250(0xd,4,DAT_00f53f0c + 400,DAT_00f53f08 + 0x155,0xa0,DAT_00f56f8c,0,DAT_00e38da8,
               DAT_00e38da4);
  return;
}
