#include "restore_types.h"
#include "production_updates.h"

void FUN_004611d0(int param_1,int param_2,int param_3,undefined1 *param_4,undefined1 *param_5,
                 undefined4 *param_6)

{
  if (param_3 == 0) {
    *param_4 = 0;
    *param_5 = 0;
    *param_6 = 0;
  }
  else {
    if (param_3 == 1) {
      *param_5 = 1;
      if (param_1 < 0x32) {
        *param_4 = 1;
        *param_6 = 2;
        return;
      }
      if (param_1 < 100) {
        *param_4 = 0;
        *param_6 = 1;
        return;
      }
      *param_4 = 0;
      *param_6 = 0;
      return;
    }
    if ((100 < param_1) && (100 < param_2)) {
      *param_6 = 0;
      *param_5 = 0;
      *param_4 = 0;
    }
    if ((param_1 < 0x32) && (100 < param_2)) {
      *param_6 = 2;
      *param_5 = 1;
      *param_4 = 1;
    }
    if (param_1 < 100) {
      if (100 < param_2) {
        *param_6 = 1;
        *param_5 = 1;
        *param_4 = 0;
      }
      if (param_2 < 100) {
        *param_6 = 2;
        *param_5 = 0;
        *param_4 = 1;
        return;
      }
    }
  }
  return;
}
