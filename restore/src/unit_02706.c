#include "restore_types.h"

void FUN_0055f3eb(undefined4 param_1)

{
  int iVar1;
  int in_ECX;
  
  *(undefined4 *)(in_ECX + 0x158) = param_1;
  if (((*(int *)(in_ECX + 0x200) == 0) && (*(int *)(in_ECX + 0x1e0) != 0)) &&
     (iVar1 = _AIL_stream_status_4(*(undefined4 *)(in_ECX + 0x1e0)), iVar1 == 4)) {
    _AIL_set_stream_volume_8(*(undefined4 *)(in_ECX + 0x1e0),param_1);
  }
  return;
}
