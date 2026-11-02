#include "restore_types.h"

undefined2 FUN_0040cb40(int param_1,undefined4 param_2)

{
  if (param_1 == 0) {
    return 0;
  }
  if (((short)param_2 != 2) ||
     ((((RESTORE_SLICE2(param_2,2) != 1 && (RESTORE_SLICE2(param_2,2) != 4)) && (RESTORE_SLICE2(param_2,2) != 2)) &&
      (RESTORE_SLICE2(param_2,2) != 3)))) {
    if (param_1 == 3) {
      if (((short)param_2 == 1) ||
         (((RESTORE_SLICE2(param_2,2) != 6 && (RESTORE_SLICE2(param_2,2) != 7)) && (RESTORE_SLICE2(param_2,2) != 5)))) {
        return 0;
      }
    }
    else if (param_1 == 2) {
      if (((short)param_2 == 1) ||
         ((((RESTORE_SLICE2(param_2,2) != 10 && (RESTORE_SLICE2(param_2,2) != 0xb)) && (RESTORE_SLICE2(param_2,2) != 0xc)) &&
          (RESTORE_SLICE2(param_2,2) != 0xd)))) {
        return 0;
      }
    }
    else if (param_1 == 4) {
      if (((short)param_2 == 1) || (RESTORE_SLICE2(param_2,2) != 0xe)) {
        return 0;
      }
    }
    else if ((param_1 == 1) && ((short)param_2 != 1)) {
      return 0;
    }
  }
  return 1;
}
