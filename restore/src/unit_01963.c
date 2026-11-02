#include "restore_types.h"

void FUN_004e5120(CRect *this,int param_1,int param_2,int param_3,int param_4)

{
  *(int *)this = param_1;
  *(int *)(this + 4) = param_2;
  *(int *)(this + 8) = param_3;
  *(int *)(this + 0xc) = param_4;
  return;
}
