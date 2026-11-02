#include "restore_types.h"
#define DAT_007770f0 (*(undefined4 *)0x007770f0u)

bool FUN_00444950(int param_1)

{
  if ((-1 < param_1) && (param_1 < 0x32)) {
    return (&DAT_007770f0)[param_1 * 0x144] == '\x02';
  }
  return false;
}
