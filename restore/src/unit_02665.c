#include "restore_types.h"

bool FUN_0055d480(float param_1)

{
  int in_ECX;
  
  param_1 = param_1 + *(float *)(in_ECX + 0x48);
  *(float *)(in_ECX + 0x48) = param_1;
  return (float)*(ushort *)(*(int *)(in_ECX + 0x44) + 2) <= param_1;
}
