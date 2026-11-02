#include "restore_types.h"
#define DAT_00bcd660 (*(undefined4 *)0x00bcd660u)

void FUN_00496640(int param_1)

{
  if (*(short *)(&DAT_00bcd660 + param_1 * 0x184) != 0) {
    do {
                    /* WARNING: Do nothing block with infinite loop */
    } while( true );
  }
  return;
}
