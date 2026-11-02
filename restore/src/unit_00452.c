#include "restore_types.h"
#define DAT_0070e128 (*(undefined *)0x0070e128u)
#define DAT_0070e134 (*(undefined *)0x0070e134u)
extern void FUN_0042fb30();

void FUN_00430040(undefined4 param_1)

{
  FUN_0042fb30(param_1);
  DAT_0070e128 = param_1;
  DAT_0070e134 = 1;
  return;
}
