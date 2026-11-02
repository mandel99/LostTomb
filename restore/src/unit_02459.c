#include "restore_types.h"
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00ea41ec (*(undefined4 *)0x00ea41ecu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00f53f08 (*(undefined *)0x00f53f08u)
#define DAT_00f53f0c (*(undefined *)0x00f53f0cu)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define DAT_00f56fb4 (*(undefined *)0x00f56fb4u)
#define DAT_00f571fc (*(undefined *)0x00f571fcu)
extern void FUN_004d6250();
extern void FUN_004d65b0();
extern void FUN_00536820();
extern int FUN_0053e590();

void FUN_00537940(void)

{
  int iVar1;
  
  iVar1 = *(int *)(&DAT_00ea41ec + DAT_00eaac28 * 0x49d8);
  FUN_00536820(DAT_00f53f0c + 0x60,DAT_00f53f08 + 0x90,0x1e,0xf);
  FUN_0053e590(DAT_00f571fc + iVar1,DAT_00f53f0c + 0x80,DAT_00f53f08 + 0xa0);
  FUN_004d6250(199,10,DAT_00f53f0c + 0x80,DAT_00f53f08 + 0xa0,0x1b0,DAT_00f56fb4,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d65b0(DAT_00f53f0c + 0x70,DAT_00f53f08 + 0xd0,0x1c,5,DAT_00e38da8,DAT_00e38da4);
  FUN_004d6250(0xd,4,DAT_00f53f0c + 400,DAT_00f53f08 + 0x155,0xa0,DAT_00f56f8c,0,DAT_00e38da8,
               DAT_00e38da4);
  return;
}
