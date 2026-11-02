#include "restore_types.h"
#define DAT_00799a8c (*(undefined4 *)0x00799a8cu)
#define DAT_00bcd652 (*(undefined4 *)0x00bcd652u)

bool FUN_004a4300(int param_1)

{
  if ((short)(&DAT_00bcd652)[param_1 * 0xc2] != 0) {
    return *(short *)(&DAT_00799a8c + (short)(&DAT_00bcd652)[param_1 * 0xc2] * 0x108) == param_1;
  }
  return false;
}
