#include "restore_types.h"
#define DAT_0078d7c0 (*(undefined1 *)0x0078d7c0u)
extern void FUN_0046e1c0();
extern void FUN_0046e1e0();
extern void FUN_0046e7c0();
extern void FUN_0046e970();
extern void FUN_0046ef50();

void FUN_0046e180(undefined4 param_1)

{
  FUN_0046e1c0();
  FUN_0046e970(param_1);
  FUN_0046ef50();
  FUN_0046e7c0();
  if ((((DAT_0078d7c0 != 0) && (DAT_0078d7c0 != 3)) && (DAT_0078d7c0 != 6)) && (DAT_0078d7c0 != 9))
  {
    return;
  }
  FUN_0046e1e0();
  return;
}
