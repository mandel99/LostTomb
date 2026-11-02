#include "restore_types.h"
#define DAT_01266434 (*(undefined *)0x01266434u)
#define DAT_01266960 (*(uint32_t *)0x01266960u)
#define DAT_01266a60 (*(uint32_t *)0x01266a60u)
#define _DAT_01266430 (*(undefined4 *)0x01266430u)

undefined4 FUN_005669c6(uint param_1)

{
  if ((param_1 < DAT_01266a60) &&
     ((*(byte *)((&DAT_01266960)[(int)param_1 >> 5] + 4 + (param_1 & 0x1f) * 8) & 1) != 0)) {
    return *(undefined4 *)((&DAT_01266960)[(int)param_1 >> 5] + (param_1 & 0x1f) * 8);
  }
  DAT_01266434 = 0;
  _DAT_01266430 = 9;
  return 0xffffffff;
}
