#include "restore_types.h"
extern void FUN_0055d290();
extern void FUN_0055d4f0();
extern longlong FUN_0056133c();

bool FUN_0055d7b0(void)

{
  short sVar1;
  int iVar2;
  int in_ECX;
  
  sVar1 = FUN_0056133c();
  if ((int)sVar1 == *(ushort *)(*(int *)(in_ECX + 0x44) + 2) - 1) {
    *(uint *)(in_ECX + 0x34) = *(uint *)(in_ECX + 0x34) | 0x100;
    return true;
  }
  iVar2 = (FUN_0055d4f0(), restore_unresolved_return(0x0055d4f0u));
  if (iVar2 != 0) {
    FUN_0055d290(*(short *)(*(int *)(in_ECX + 0x44) + 2) + -1);
  }
  return iVar2 != 0;
}
