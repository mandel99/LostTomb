#include "restore_types.h"

int FUN_0055efdc(int param_1)

{
  int in_ECX;
  int local_8;
  
  local_8 = 0;
  while( true ) {
    if (*(int *)(in_ECX + 0x198) <= local_8) {
      return -1;
    }
    if (*(int *)(*(int *)(in_ECX + 0x1a8) + 4 + local_8 * 0x28) == param_1) break;
    local_8 = local_8 + 1;
  }
  return local_8;
}
