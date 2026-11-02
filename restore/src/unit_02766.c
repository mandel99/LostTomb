#include "restore_types.h"

undefined4 FUN_00561090(void)

{
  int in_ECX;
  
  if ((*(uint *)(in_ECX + 0x2c) <= *(uint *)(in_ECX + 0x28)) &&
     (*(uint *)(in_ECX + 0x1c) <= *(uint *)(in_ECX + 0x18))) {
    return 0;
  }
  return 0xffffffff;
}
