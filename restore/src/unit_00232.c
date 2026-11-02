#include "restore_types.h"
#define DAT_00d3b568 (*(undefined *)0x00d3b568u)
#define DAT_00d3b56c (*(undefined *)0x00d3b56cu)
#define DAT_00d3b570 (*(undefined *)0x00d3b570u)
#define DAT_00d3b574 (*(undefined *)0x00d3b574u)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00e38dac (*(undefined *)0x00e38dacu)
#define DAT_00e38db0 (*(undefined *)0x00e38db0u)
#define DAT_00f53db4 (*(undefined *)0x00f53db4u)
#define DAT_00f53db8 (*(undefined *)0x00f53db8u)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define DAT_00f56fb4 (*(undefined *)0x00f56fb4u)
extern void FUN_004ccb30();
extern void FUN_00536820();

void FUN_004191f0(void)

{
  DAT_00f53db8 = DAT_00e38da8 + 0x30;
  DAT_00f53db4 = DAT_00e38da4 + 0x80;
  DAT_00d3b56c = (0x400 - DAT_00e38db0) / 2;
  DAT_00d3b568 = (0x300 - DAT_00e38dac) / 2;
  DAT_00d3b574 = DAT_00e38db0;
  DAT_00d3b570 = DAT_00e38dac;
  FUN_00536820(DAT_00f53db8,DAT_00f53db4,0x22,0xf);
  FUN_004ccb30(0x3e,0,DAT_00f53db8,DAT_00f53db4 + 0x10,0x220,DAT_00f56fb4,0);
  FUN_004ccb30(0x3e,2,DAT_00f53db8,DAT_00f53db4 + 0x2f,0x220,DAT_00f56f8c,0);
  return;
}
