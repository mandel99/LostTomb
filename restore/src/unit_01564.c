#include "restore_types.h"
#define DAT_00ae8e7c (*(undefined4 *)0x00ae8e7cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
extern void FUN_00468590();
extern void FUN_004becc0();

void FUN_004beb90(void)

{
  int *piVar1;
  int iVar2;
  int in_ECX;
  undefined4 *puVar3;
  
  iVar2 = *(int *)(in_ECX + 0x32fac);
  if (iVar2 < 0x1e) {
    if (*(int *)(in_ECX + 0x32c64 + iVar2 * 0x1c) != 0) {
      puVar3 = *(undefined4 **)(in_ECX + iVar2 * 0x1c + 0x32c74);
      while (puVar3 != (undefined4 *)0x0) {
        piVar1 = puVar3 + 3;
        puVar3 = (undefined4 *)*puVar3;
        if (*piVar1 == 0) break;
        iVar2 = *(int *)(*piVar1 + 8);
        if ((((&DAT_00af598c)[iVar2] & 0x100) != 0) && ((&DAT_00ae8e7c)[iVar2] == '\0')) {
          FUN_00468590(iVar2);
        }
      }
    }
  }
  else if (*(int *)(in_ECX + 0x32f90) != 0) {
    puVar3 = *(undefined4 **)(in_ECX + 0x32fa0);
    while (puVar3 != (undefined4 *)0x0) {
      piVar1 = puVar3 + 3;
      puVar3 = (undefined4 *)*puVar3;
      if (*piVar1 == 0) break;
      iVar2 = *(int *)(*piVar1 + 8);
      if ((((&DAT_00af598c)[iVar2] & 0x100) != 0) && ((&DAT_00ae8e7c)[iVar2] == '\0')) {
        FUN_00468590(iVar2);
      }
    }
  }
  iVar2 = *(int *)(in_ECX + 0x32fac);
  if ((iVar2 + 1 < 0x1e) && (*(int *)(in_ECX + 0x32c80 + iVar2 * 0x1c) != 0)) {
    puVar3 = *(undefined4 **)(in_ECX + iVar2 * 0x1c + 0x32c90);
    while ((puVar3 != (undefined4 *)0x0 &&
           (piVar1 = puVar3 + 3, puVar3 = (undefined4 *)*puVar3, *piVar1 != 0))) {
      FUN_004becc0();
    }
  }
  iVar2 = *(int *)(in_ECX + 0x32fac);
  if ((iVar2 + 2 < 0x1e) && (*(int *)(in_ECX + 0x32c9c + iVar2 * 0x1c) != 0)) {
    puVar3 = *(undefined4 **)(in_ECX + iVar2 * 0x1c + 0x32cac);
    while ((puVar3 != (undefined4 *)0x0 &&
           (piVar1 = puVar3 + 3, puVar3 = (undefined4 *)*puVar3, *piVar1 != 0))) {
      FUN_004becc0();
    }
  }
  return;
}
