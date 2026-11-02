#include "restore_types.h"
extern bool FUN_0055d760();
extern undefined4 FUN_0055d780();
extern bool FUN_0055d7b0();
extern undefined4 FUN_0055d810();

int FUN_0055d6d0(void)

{
  int iVar1;
  int in_ECX;
  
  if (*(int *)(in_ECX + 0x38) == 0) {
    return 0;
  }
  switch(**(byte **)(in_ECX + 0x40) & 7) {
  case 0: ;
    iVar1 = FUN_0055d760();
    break;
  case 1: ;
    iVar1 = FUN_0055d7b0();
    return iVar1;
  case 2: ;
    iVar1 = FUN_0055d780();
    break;
  case 3: ;
    iVar1 = FUN_0055d810();
    break;
  case 4: ;
    iVar1 = FUN_0055d760();
    break;
default: ;
    goto switchD_0055d6eb_default;
  }
  if (iVar1 != 0) {
    *(uint *)(in_ECX + 0x34) = *(uint *)(in_ECX + 0x34) | 0x100;
    return iVar1;
  }
switchD_0055d6eb_default: ;
  *(uint *)(in_ECX + 0x34) = *(uint *)(in_ECX + 0x34) & 0xfffffeff;
  return 0;
}
