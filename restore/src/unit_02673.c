#include "restore_types.h"
extern void FUN_0055d4f0();

undefined4 FUN_0055d780(void)

{
  int iVar1;
  int in_ECX;
  
  iVar1 = (FUN_0055d4f0(), restore_unresolved_return(0x0055d4f0u));
  if (iVar1 != 0) {
    *(uint *)(in_ECX + 0x34) = *(uint *)(in_ECX + 0x34) | 8;
    return 1;
  }
  return 0;
}
