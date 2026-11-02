#include "restore_types.h"
#define DAT_00799a0d (*(undefined4 *)0x00799a0du)

bool FUN_004890f0(int param_1)

{
  return (&DAT_00799a0d)[param_1 * 0x108] != '\0';
}
