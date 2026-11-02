#include "restore_types.h"
#define DAT_00ea89dc (*(undefined *)0x00ea89dcu)

void FUN_004e7e40(void)

{
  if (0 < (short)DAT_00ea89dc) {
    DAT_00ea89dc = CONCAT22(RESTORE_SLICE2(DAT_00ea89dc,2),(short)DAT_00ea89dc + -1);
  }
  return;
}
