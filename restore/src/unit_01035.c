#include "restore_types.h"
#define DAT_00799a10 (*(undefined4 *)0x00799a10u)

int FUN_00475b50(int param_1)

{
  unsigned char *record;
  int iVar1;
  
  iVar1 = 0;
  do {
    record = (unsigned char *)(0x00799a10u + (unsigned int)param_1 * 0x108u);
    if (*(short *)record < 1) {
      return param_1;
    }
    iVar1 = iVar1 + 1;
    param_1 = (int)*(short *)record;
  } while (iVar1 < 0x20d);
  return 0;
}
