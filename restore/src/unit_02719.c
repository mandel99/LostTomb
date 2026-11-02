#include "restore_types.h"
#define s_EAX_environment_selection_006047e8 (*(char (*)[26])0x006047e8u)

void FUN_0055fec8(undefined4 param_1)

{
  int in_ECX;
  
  *(undefined4 *)(in_ECX + 0x1fc) = param_1;
  if (((*(int *)(in_ECX + 0x200) == 0) && (*(int *)(in_ECX + 0x1e8) != 0)) &&
     (*(int *)(in_ECX + 0x1ec) != 0)) {
    _AIL_set_3D_provider_preference_12
              (*(undefined4 *)(in_ECX + 0x1e8),s_EAX_environment_selection_006047e8,&param_1);
  }
  return;
}
