#include "restore_types.h"

bool FUN_0055d4c0(float param_1)

{
  int in_ECX;
  
  param_1 = param_1 + *(float *)(in_ECX + 0x48);
  *(float *)(in_ECX + 0x48) = param_1;
  return param_1 < 0.0;
}
