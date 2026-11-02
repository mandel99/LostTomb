#include "restore_types.h"
#define DAT_00eadd84 (*(undefined *)0x00eadd84u)
#define DAT_00eadd88 (*(undefined *)0x00eadd88u)
#define DAT_00eadd8c (*(undefined *)0x00eadd8cu)
#define DAT_00eadd90 (*(undefined *)0x00eadd90u)
#define DAT_00f56f94 (*(undefined *)0x00f56f94u)
#define DAT_00f56fb4 (*(undefined *)0x00f56fb4u)
extern void FUN_004d6250();
extern void FUN_00536820();

void FUN_004fa4a0(void)

{
  FUN_00536820(DAT_00eadd90,DAT_00eadd8c,DAT_00eadd88,DAT_00eadd84);
  FUN_004d6250(0x46,0,DAT_00eadd90,DAT_00eadd8c + 8,DAT_00eadd88 << 4,DAT_00f56fb4,0,DAT_00eadd90,
               DAT_00eadd8c);
  FUN_004d6250(0xd,1,DAT_00eadd90,DAT_00eadd84 * 0x10 + -0x16 + DAT_00eadd8c,DAT_00eadd88 << 4,
               DAT_00f56f94,0,DAT_00eadd90,DAT_00eadd8c);
  return;
}
