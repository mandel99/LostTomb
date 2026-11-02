#include "restore_types.h"
#define DAT_005d9c98 (*(undefined4 *)0x005d9c98u)

undefined4 FUN_004493d0(void)

{
  int in_ECX;
  
  return *(undefined4 *)(&DAT_005d9c98 + (uint)*(byte *)(in_ECX + 2) * 4);
}
