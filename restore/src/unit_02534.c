#include "restore_types.h"

void FUN_00540540(int *param_1)

{
  int *in_ECX;
  
  *param_1 = *in_ECX;
  if (*in_ECX != 0) {
    *(int **)(*in_ECX + 4) = param_1;
  }
  param_1[1] = 0;
  *in_ECX = (int)param_1;
  if (in_ECX[1] == 0) {
    in_ECX[1] = (int)param_1;
  }
  return;
}
