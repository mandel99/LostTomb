#include "restore_types.h"
#include "sound_initialization.h"

int FUN_0055f07f(uintptr_t in_ECX,const int32_t *request)

{
  int local_8;
  
  local_8 = 0;
  while( true ) {
    if (*(int *)(in_ECX + 0x198) <= local_8) {
      return -1;
    }
    if ((*(int *)(*(int *)(in_ECX + 0x1ac) + local_8 * 0xc) == request[1]) &&
       (*(int *)(*(int *)(in_ECX + 0x1ac) + 4 + local_8 * 0xc) != 0)) break;
    local_8 = local_8 + 1;
  }
  return local_8;
}
