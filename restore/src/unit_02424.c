#include "restore_types.h"
/* Native dialog geometry, font tokens and selection state are DWORDs. */
#define DAT_005c8ee8 (*(undefined4 *)0x005c8ee8u)
#define DAT_00e38da4 (*(int32_t *)0x00e38da4u)
#define DAT_00e38da8 (*(int32_t *)0x00e38da8u)
#define DAT_00e39080 (*(uint32_t *)0x00e39080u)
#define DAT_00f56f8c (*(uint32_t *)0x00f56f8cu)
#define DAT_00f56fb4 (*(uint32_t *)0x00f56fb4u)
extern void FUN_004256a0();
extern void FUN_004d6250();
extern void FUN_00536820();

void FUN_00531be0(void)

{
  FUN_00536820(DAT_00e38da8 + 0x30,DAT_00e38da4 + 0x50,0x18,0xc);
  FUN_004d6250(0x99,0,DAT_00e38da8 + 0x30,DAT_00e38da4 + 0x5e,0x180,DAT_00f56fb4,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d6250(0x99,DAT_00e39080 + 1,DAT_00e38da8 + 0x50,DAT_00e38da4 + 0xae,0xe0,DAT_00f56f8c,0,
               DAT_00e38da8,DAT_00e38da4);
  FUN_004256a0(DAT_00e38da8 + 0x120,DAT_00e38da4 + 0x70,&DAT_005c8ee8,2);
  FUN_004d6250(0x99,8,DAT_00e38da8 + 0x30,DAT_00e38da4 + 0xf6,0x180,DAT_00f56f8c,0,DAT_00e38da8,
               DAT_00e38da4);
  return;
}
