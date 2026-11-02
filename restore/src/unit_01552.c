#include "restore_types.h"
#include "city_prepare.h"
extern void FUN_0051f780();
extern void FUN_00561a2b();

void FUN_004bd6a0(RestoreWorldRegion *region)

{
  int iVar1;
  int *piVar2;
  int *piVar3;
  int iVar4;
  int iVar5;
  uint32_t *in_ECX=region->words;
  undefined4 *puVar6;
  
  if (*in_ECX != 0) {
    puVar6 = (undefined4 *)in_ECX[4];
    if (puVar6 == (undefined4 *)0x0) {
      iVar5 = 0;
      puVar6 = (undefined4 *)0x0;
    }
    else {
      iVar5 = puVar6[3];
      puVar6 = (undefined4 *)*puVar6;
    }
    in_ECX[6] = iVar5;
    while (iVar5 != 0) {
      if (puVar6 == (undefined4 *)0x0) {
        iVar5 = 0;
        puVar6 = (undefined4 *)0x0;
      }
      else {
        iVar5 = puVar6[3];
        puVar6 = (undefined4 *)*puVar6;
      }
      iVar1 = in_ECX[6];
      piVar2 = *(int **)(iVar1 + 4);
      piVar3 = (int *)piVar2[1];
      iVar4 = *piVar2;
      if (piVar3 == (int *)0x0) {
        in_ECX[4] = iVar4;
      }
      else {
        *piVar3 = iVar4;
      }
      if (iVar4 == 0) {
        in_ECX[5] = (int)piVar3;
      }
      else {
        *(int **)(iVar4 + 4) = piVar3;
      }
      piVar2[1] = 0;
      *piVar2 = 0;
      FUN_00561a2b(piVar2);
      *(undefined4 *)(iVar1 + 4) = 0;
      iVar1 = in_ECX[6];
      if (iVar1 != 0) {
        FUN_0051f780();
        FUN_00561a2b(iVar1);
      }
      in_ECX[6] = iVar5;
    }
    *in_ECX = 0;
  }
  return;
}
