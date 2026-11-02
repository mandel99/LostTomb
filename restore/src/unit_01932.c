#include "restore_types.h"
#define DAT_00605a3c (*(undefined *)0x00605a3cu)
#define DAT_00ea1894 (*(undefined4 *)0x00ea1894u)
#define DAT_00ea41dc (*(undefined4 *)0x00ea41dcu)
#define DAT_00ea5e1c (*(undefined4 *)0x00ea5e1cu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern void FUN_004e2970();

void FUN_004e2dd0(void)

{
  if (((DAT_00605a3c != 1) && (299 < *(int *)(&DAT_00ea1894 + DAT_00eaac28 * 0x49d8))) &&
     (*(int *)(&DAT_00ea41dc + DAT_00eaac28 * 0x49d8) <=
      (int)(&DAT_00ea5e1c)[DAT_00eaac28 * 0x1276] / 2)) {
    FUN_004e2970(0x5d,0,0);
  }
  return;
}
