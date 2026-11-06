#include "restore_types.h"
#define DAT_00acf85c (*(uint8_t *)0x00acf85cu)
#include "city_prepare.h"
extern void * FUN_00561a1d();

void *FUN_004bd740(RestoreWorldRegion *region,int32_t param_1,int32_t param_2,int32_t param_3,int32_t param_4,uint8_t param_5)

{
  int *piVar1;
  int iVar2;
  void *pvVar3;
  int iVar4 = 0;
  uint uVar5;
  int *piVar6;
  uint32_t *in_ECX=region->words;
  
  pvVar3 = FUN_00561a1d(0x1c);
  /* Native4BD778 keeps a null item and continues to allocate its list node. */
  if (pvVar3 != (void *)0x0) {
    iVar4 = (int)(uintptr_t)FUN_004bd610(pvVar3,param_1,param_2,param_3,param_4,region,param_5);
  }
  uVar5 = (uint)(byte)(&DAT_00acf85c)[param_1];
  piVar6 = FUN_00561a1d(0x10);
  /* Native4BD790 skips construction on failure and still enters insertion. */
  if (piVar6 != (int *)0x0) {
    piVar6[1] = 0;
    *piVar6 = 0;
    piVar6[3] = iVar4;
    piVar6[2] = uVar5;
  }
  piVar1 = (int *)in_ECX[4];
  if ((piVar1 == (int *)0x0) || ((int)uVar5 <= piVar1[2])) {
    *piVar6 = (int)piVar1;
    if (in_ECX[4] != 0) {
      *(int **)(in_ECX[4] + 4) = piVar6;
    }
    piVar6[1] = 0;
    in_ECX[4] = (int)piVar6;
    if (in_ECX[5] == 0) {
      in_ECX[5] = (int)piVar6;
    }
  }
  else {
    if ((int)uVar5 < *(int *)(in_ECX[5] + 8)) {
      iVar2 = piVar1[2];
      while (iVar2 < (int)uVar5) {
        piVar1 = (int *)*piVar1;
        iVar2 = piVar1[2];
      }
      *piVar6 = (int)piVar1;
      piVar6[1] = piVar1[1];
      *(int **)piVar1[1] = piVar6;
      piVar1[1] = (int)piVar6;
      *(int **)(iVar4 + 4) = piVar6;
      *in_ECX = *in_ECX + 1;
      return (void *)(uintptr_t)iVar4;
    }
    piVar6[1] = in_ECX[5];
    if ((undefined4 *)in_ECX[5] != (undefined4 *)0x0) {
      *(undefined4 *)in_ECX[5] = (uint32_t)(uintptr_t)piVar6;
    }
    *piVar6 = 0;
    in_ECX[5] = (int)piVar6;
    if (in_ECX[4] == 0) {
      in_ECX[4] = (int)piVar6;
      *(int **)(iVar4 + 4) = piVar6;
      *in_ECX = *in_ECX + 1;
      return (void *)(uintptr_t)iVar4;
    }
  }
  *(int **)(iVar4 + 4) = piVar6;
  *in_ECX = *in_ECX + 1;
  return (void *)(uintptr_t)iVar4;
}
