#include "restore_types.h"
#define DAT_007056c8 (*(undefined *)0x007056c8u)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00f53f08 (*(undefined *)0x00f53f08u)
#define DAT_00f53f0c (*(undefined *)0x00f53f0cu)
extern void FUN_004d63d0();

void FUN_004f7720(int param_1)

{
  FUN_004d63d0(0,DAT_00f53f0c + -0x28 + param_1 * 0x78,DAT_00f53f08 + 0x11e,0x78,0x42,
               DAT_007056c8 == param_1,DAT_00e38da8,DAT_00e38da4);
  return;
}
