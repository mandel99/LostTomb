#include "restore_types.h"
#define DAT_00ae8e7c (*(undefined4 *)0x00ae8e7cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
extern void FUN_00468590();

void FUN_004becc0(void)

{
  int iVar1;
  int in_ECX;
  
  iVar1 = *(int *)(in_ECX + 8);
  if ((((&DAT_00af598c)[iVar1] & 0x100) != 0) && ((&DAT_00ae8e7c)[iVar1] == '\0')) {
    FUN_00468590(iVar1);
  }
  return;
}
