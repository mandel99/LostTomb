#include "restore_types.h"
#define DAT_00e20778 (*(undefined *)0x00e20778u)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00e38dac (*(undefined *)0x00e38dacu)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
extern void FUN_004cd930();
extern void FUN_004d6250();

void FUN_0052f850(void)

{
  DAT_00e20778 = 0;
  FUN_004cd930(0xe7,DAT_00e38dac + -0x22,0x96,0x14);
  FUN_004d6250(0x2c,0xcc,0xe7,DAT_00e38dac + -0x1e,0x78,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  return;
}
