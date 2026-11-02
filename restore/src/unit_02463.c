#include "restore_types.h"
#define DAT_00d3b198 (*(undefined *)0x00d3b198u)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00f53f08 (*(undefined *)0x00f53f08u)
#define DAT_00f53f0c (*(undefined *)0x00f53f0cu)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
extern void FUN_004cf090();
extern void FUN_004d6160();
extern void FUN_004d6250();
extern void FUN_00536820();

void FUN_00538850(void)

{
  int iVar1;
  
  iVar1 = DAT_00d3b198 + 0x20;
  if (-1 < iVar1) {
    FUN_004cf090(DAT_00f53f0c + 0x20,DAT_00f53f08 + 0x3c,0x240,0x110,0);
    FUN_00536820(DAT_00f53f0c + 0x10,DAT_00f53f08 + 0x90,0x26,0xd);
    FUN_004d6160(0x3b,iVar1,DAT_00f53f0c + 0x20,DAT_00f53f08 + 0xa0,0x264,DAT_00f56f8c,0,
                 DAT_00e38da8,DAT_00e38da4);
    FUN_004d6250(0xd,1,DAT_00f53f0c + 0x10,DAT_00f53f08 + 0x148,0x260,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
  }
  return;
}
