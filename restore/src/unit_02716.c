#include "restore_types.h"

int FUN_0055fb6f(int param_1)

{
  int in_ECX;
  int local_8;
  
  local_8 = 0;
  while( true ) {
    if (*(int *)(in_ECX + 0x1e4) <= local_8) {
      return -1;
    }
    if ((*(int *)(*(int *)(in_ECX + 0x1f8) + local_8 * 0xc) == *(int *)(param_1 + 4)) &&
       (*(int *)(*(int *)(in_ECX + 0x1f8) + 4 + local_8 * 0xc) != 0)) break;
    local_8 = local_8 + 1;
  }
  return local_8;
}
