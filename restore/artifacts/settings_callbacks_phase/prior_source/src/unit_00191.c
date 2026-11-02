#include "restore_types.h"
#define DAT_00d3b198 (*(undefined *)0x00d3b198u)
#define DAT_00e38dda (*(char *)0x00e38ddau)
extern void FUN_00403320();

void FUN_00416ba0(undefined4 param_1)

{
  if (DAT_00e38dda == '\x01') {
    DAT_00d3b198 = param_1;
    FUN_00403320();
    return;
  }
  return;
}
