#include "restore_types.h"
#define DAT_007056c8 (*(undefined *)0x007056c8u)
#define DAT_00ea5d68 (*(undefined4 *)0x00ea5d68u)
#define DAT_00ea6082 (*(undefined4 *)0x00ea6082u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00eadd8c (*(undefined *)0x00eadd8cu)
#define DAT_00eadd90 (*(undefined *)0x00eadd90u)
extern void FUN_004d63d0();

void FUN_00516020(void)

{
  if ((&DAT_00ea5d68)[DAT_00eaac28 * 0x1276] == 0) {
    FUN_004d63d0(0,DAT_00eadd90 + 0x52,DAT_00eadd8c + 0x99,300,0x14,
                 *(short *)((int)&DAT_00ea6082 + DAT_00eaac28 * 0x49d8 + 2) != 0 &&
                 DAT_007056c8 == 1,DAT_00eadd90,DAT_00eadd8c);
  }
  return;
}
