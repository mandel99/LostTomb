#include "restore_types.h"
#define DAT_00e20778 (*(undefined *)0x00e20778u)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00f53f00 (*(undefined *)0x00f53f00u)
#define DAT_00f53f08 (*(undefined *)0x00f53f08u)
#define DAT_00f53f0c (*(undefined *)0x00f53f0cu)
#define DAT_00f53f48 (*(undefined *)0x00f53f48u)
#define DAT_00f5681c (*(undefined *)0x00f5681cu)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define DAT_00f56f9c (*(undefined *)0x00f56f9cu)
#define DAT_00f56fb4 (*(undefined *)0x00f56fb4u)
#define _DAT_00f53f04 (*(undefined4 *)0x00f53f04u)
extern int FUN_0040cc40();
extern void FUN_004cd680();
extern void FUN_004d62f0();
extern void FUN_004d65b0();
extern void FUN_00536820();
extern int FUN_0053e590();

void FUN_0052b520(void)

{
  int iVar1;
  
  DAT_00f53f0c = DAT_00e38da8;
  iVar1 = 5 - DAT_00f53f48;
  DAT_00f53f08 = DAT_00e38da4;
  _DAT_00f53f04 = 0x28;
  DAT_00f53f00 = 0x1b;
  if (iVar1 < 5) {
    if (0 < iVar1) {
      FUN_004cd680(iVar1 * 0x10 + DAT_00e38da8,DAT_00e38da4 + iVar1 * 8,(iVar1 * -2 + 0x28) * 0x10,
                   (iVar1 * -2 + 0x1b) * 0x10,0);
      return;
    }
    FUN_00536820(DAT_00e38da8,DAT_00e38da4,0x28,0x1b);
    FUN_004d65b0(DAT_00f53f0c + 0x20,DAT_00f53f08 + 0x35,0x24,0xe,DAT_00e38da8,DAT_00e38da4);
    FUN_0053e590(DAT_00f5681c + 0xc,DAT_00f53f0c + 10,DAT_00f53f08 + 10);
    DAT_00e20778 = 0;
    FUN_004d62f0(199,0,DAT_00f53f0c + 0x3c,DAT_00f53f08 + 0xc,DAT_00f56fb4,0,DAT_00e38da8,
                 DAT_00e38da4);
    iVar1 = FUN_0040cc40(0);
    if (iVar1 != 0) {
      FUN_004d62f0(199,3,DAT_00e20778 + 0x3c + DAT_00f53f0c,DAT_00f53f08 + 0x1e,DAT_00f56f8c,0,
                   DAT_00e38da8,DAT_00e38da4);
      return;
    }
    FUN_004d62f0(199,0xc,DAT_00f53f0c + 0x54,DAT_00f53f08 + 0x8c,DAT_00f56f9c,0,DAT_00e38da8,
                 DAT_00e38da4);
  }
  return;
}
