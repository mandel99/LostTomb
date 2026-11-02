#include "restore_types.h"
#define DAT_00ea5d40 (*(undefined4 *)0x00ea5d40u)
#define DAT_00ea5d44 (*(undefined4 *)0x00ea5d44u)
#define DAT_00eaac28 (*(undefined4 *)0x00eaac28u)

void FUN_004ea740(void)

{
  if ((&DAT_00ea5d40)[DAT_00eaac28 * 0x1276] != 0) {
    (&DAT_00ea5d44)[DAT_00eaac28 * 0x1276] = (&DAT_00ea5d44)[DAT_00eaac28 * 0x1276] + -1;
  }
  return;
}
