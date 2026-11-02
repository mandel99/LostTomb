#include "restore_types.h"
#include "sound_initialization.h"

int FUN_0055f02d(uintptr_t in_ECX)

{
  int iVar1;
  int local_6c;
  
  local_6c = 0;
  while( true ) {
    if (*(int *)(in_ECX + 0x198) <= local_6c) {
      return -1;
    }
    iVar1 = _AIL_sample_status_4(*(undefined4 *)(*(int *)(in_ECX + 0x19c) + local_6c * 4));
    if (iVar1 != 4) break;
    local_6c = local_6c + 1;
  }
  return local_6c;
}
