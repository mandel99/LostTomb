#include "restore_types.h"
#define DAT_007056c8 (*(undefined *)0x007056c8u)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00ea5d68 (*(undefined4 *)0x00ea5d68u)
#define DAT_00ea6082 (*(undefined4 *)0x00ea6082u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00f53f08 (*(undefined *)0x00f53f08u)
#define DAT_00f53f0c (*(undefined *)0x00f53f0cu)
#define DAT_00f53f48 (*(undefined *)0x00f53f48u)
extern void FUN_004d63d0();

void FUN_005292b0(void)

{
  if ((5 < DAT_00f53f48) && ((&DAT_00ea5d68)[DAT_00eaac28 * 0x1276] == 0)) {
    FUN_004d63d0(0,DAT_00f53f0c + 0x66,DAT_00f53f08 + 0x15c,300,0x14,
                 *(short *)((int)&DAT_00ea6082 + DAT_00eaac28 * 0x49d8 + 2) != 0 &&
                 DAT_007056c8 == 1,DAT_00e38da8,DAT_00e38da4);
  }
  return;
}
