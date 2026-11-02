#include "restore_types.h"
extern void FUN_005606e2();
extern void FUN_00561a2b();

void FUN_005606c6(byte param_1)

{
  FUN_005606e2();
  if ((param_1 & 1) != 0) {
    FUN_00561a2b();
  }
  return;
}
