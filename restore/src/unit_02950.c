#include "restore_types.h"

float10 FUN_00568448(undefined8 param_1,short param_2)

{
  undefined8 local_c;
  
  local_c = (double)CONCAT26((param_2 + 0x3fe) * 0x10 | RESTORE_SLICE2(param_1,6) & 0x800f,(undefined6)param_1);
  return (float10)local_c;
}
