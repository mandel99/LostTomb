#include "restore_types.h"
extern void FUN_0055d2e0();

void FUN_0055d300(undefined4 param_1,short param_2)

{
  int in_ECX;
  
  if (*(int *)(in_ECX + 0x40) != 0) {
    FUN_0055d2e0(param_1);
    *(float *)(in_ECX + 0x48) = (float)(int)param_2;
  }
  return;
}
