#include "restore_types.h"
#define DAT_006049c0 (*(undefined4 *)0x006049c0u)
#define DAT_006049e0 (*(undefined4 *)0x006049e0u)
extern int FUN_00562bea();

int FUN_00560c26(void)

{
  uint *puVar1;
  int iVar2;
  int *in_ECX;
  
  *(undefined4 *)(*(int *)(*in_ECX + 4) + 0x30 + (int)in_ECX) = 0;
  if ((*(byte *)((int)in_ECX + *(int *)(*in_ECX + 4) + 0x25) & 0x20) != 0) {
    iVar2 = (**(code **)(**(int **)((int)in_ECX + *(int *)(*in_ECX + 4) + 4) + 4))();
    if (iVar2 == -1) {
      *(undefined4 *)(*(int *)(*in_ECX + 4) + 8 + (int)in_ECX) = 6;
    }
  }
  iVar2 = *(int *)(*in_ECX + 4);
  if ((*(byte *)(iVar2 + 0x25 + (int)in_ECX) & 0x40) != 0) {
    iVar2 = FUN_00562bea(&DAT_006049c0);
    if (iVar2 == -1) {
      puVar1 = (uint *)(*(int *)(*in_ECX + 4) + 8 + (int)in_ECX);
      *puVar1 = *puVar1 | 2;
    }
    iVar2 = FUN_00562bea(&DAT_006049e0);
    if (iVar2 == -1) {
      iVar2 = *(int *)(*in_ECX + 4);
      puVar1 = (uint *)(iVar2 + 8 + (int)in_ECX);
      *puVar1 = *puVar1 | 2;
      iVar2 = iVar2 + 8 + (int)in_ECX;
    }
  }
  return iVar2;
}
