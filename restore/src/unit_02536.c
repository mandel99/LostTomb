#include "restore_types.h"

void FUN_00540590(int *param_1,int param_2)

{
  *param_1 = param_2;
  param_1[1] = *(int *)(param_2 + 4);
  **(int **)(param_2 + 4) = (int)param_1;
  *(int **)(param_2 + 4) = param_1;
  return;
}
