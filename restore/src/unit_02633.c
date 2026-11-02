#include "restore_types.h"
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00ea623c (*(undefined4 *)0x00ea623cu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00f53f00 (*(undefined *)0x00f53f00u)
#define DAT_00f53f08 (*(undefined *)0x00f53f08u)
#define DAT_00f53f0c (*(undefined *)0x00f53f0cu)
#define DAT_00f53f48 (*(undefined *)0x00f53f48u)
#define DAT_00f5681c (*(undefined *)0x00f5681cu)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define DAT_00f56fb4 (*(undefined *)0x00f56fb4u)
#define _DAT_00f53f04 (*(undefined4 *)0x00f53f04u)
extern void FUN_004cd680();
extern void FUN_004d62f0();
extern void FUN_00536820();
extern int FUN_0053e590();
extern void FUN_005596d0();

void FUN_00559e10(void)

{
  int iVar1;
  
  FUN_005596d0();
  iVar1 = 5 - DAT_00f53f48;
  DAT_00f53f0c = DAT_00e38da8;
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
    FUN_0053e590(DAT_00f5681c + 4,DAT_00f53f0c + 10,DAT_00f53f08 + 10);
    FUN_004d62f0(0x36,0,DAT_00f53f0c + 0x3c,DAT_00f53f08 + 0xc,DAT_00f56fb4,0,DAT_00e38da8,
                 DAT_00e38da4);
    if ((&DAT_00ea623c)[DAT_00eaac28 * 0x49d8] == '\0') {
      FUN_004d62f0(0x36,1,DAT_00f53f0c + 100,DAT_00f53f08 + 0x26,DAT_00f56f8c,0,DAT_00e38da8,
                   DAT_00e38da4);
      return;
    }
    FUN_004d62f0(0x36,0x16,DAT_00f53f0c + 0xaf,DAT_00f53f08 + 0x26,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
    FUN_004d62f0(0x36,0x17,DAT_00f53f0c + 0x113,DAT_00f53f08 + 0x26,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
  }
  return;
}
