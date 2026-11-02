#include "restore_types.h"
extern void FUN_00560648();
extern void FUN_00561a2b();

void FUN_0056062c(byte param_1)

{
  FUN_00560648();
  if ((param_1 & 1) != 0) {
    FUN_00561a2b();
  }
  return;
}
