#include "restore_types.h"
#define DAT_007056c8 (*(undefined *)0x007056c8u)
#define DAT_00eadd84 (*(undefined *)0x00eadd84u)
#define DAT_00eadd88 (*(undefined *)0x00eadd88u)
#define DAT_00eadd8c (*(undefined *)0x00eadd8cu)
#define DAT_00eadd90 (*(undefined *)0x00eadd90u)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
extern void FUN_004d6250();
extern void FUN_004d63d0();

void FUN_00501140(void)

{
  FUN_004d63d0(0,DAT_00eadd90 + 100,DAT_00eadd84 * 0x10 + -0x22 + DAT_00eadd8c,
               (DAT_00eadd88 + -0xc) * 0x10,0x14,DAT_007056c8 == 1,DAT_00eadd90,DAT_00eadd8c);
  FUN_004d6250(0x61,6,DAT_00eadd90 + 0x50,DAT_00eadd84 * 0x10 + -0x1e + DAT_00eadd8c,
               (DAT_00eadd88 + -10) * 0x10,DAT_00f56f8c,0,DAT_00eadd90,DAT_00eadd8c);
  return;
}
