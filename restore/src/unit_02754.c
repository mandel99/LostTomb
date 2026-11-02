#include "restore_types.h"
extern void FUN_00560ccf();

undefined4 FUN_00560bfd(void)

{
  int iVar1;
  uint uVar2;
  int *in_ECX;
  
  iVar1 = *(int *)(*in_ECX + 4);
  uVar2 = *(uint *)((int)in_ECX + iVar1 + 8);
  if (uVar2 != 0) {
    *(uint *)((int)in_ECX + iVar1 + 8) = uVar2 | 2;
    return 0;
  }
  if (*(int *)((int)in_ECX + iVar1 + 0x20) != 0) {
    FUN_00560ccf();
  }
  return 1;
}
