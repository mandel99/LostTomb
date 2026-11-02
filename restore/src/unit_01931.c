#include "restore_types.h"
#define DAT_00605a3c (*(undefined *)0x00605a3cu)
#define DAT_00e38ea8 (*(undefined1 *)0x00e38ea8u)
#define DAT_00e38ebc (*(undefined *)0x00e38ebcu)
#define DAT_00ea1894 (*(undefined4 *)0x00ea1894u)
#define DAT_00ea446c (*(undefined4 *)0x00ea446cu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern void FUN_004e2970();

void FUN_004e2d80(void)

{
  if (((DAT_00605a3c != 1) &&
      (((DAT_00e38ebc != 0 || (DAT_00e38ea8 != '\0')) &&
       (299 < *(int *)(&DAT_00ea1894 + DAT_00eaac28 * 0x49d8))))) &&
     (10 < (int)(&DAT_00ea446c)[DAT_00eaac28 * 0x1276])) {
    FUN_004e2970(0x5c,0,0);
  }
  return;
}
