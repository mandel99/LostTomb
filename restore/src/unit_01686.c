#include "restore_types.h"
#define DAT_00e2079c (*(uint8_t *)0x00e2079cu)
#define DAT_00e208a0 (*(uint8_t *)0x00e208a0u)
#define DAT_00e208f0 (*(uint8_t *)0x00e208f0u)
#define DAT_00e20990 (*(int32_t *)0x00e20990u)

void FUN_004cc500(int param_1)

{
  int *piVar1;
  int iVar2;
  
  iVar2 = 0;
  piVar1 = (int *)(&DAT_00e208a0 + param_1 * 4);
  DAT_00e20990 = param_1;
  *piVar1 = 0;
  if (-1 < *(int *)(&DAT_00e208f0 + param_1 * 4)) {
    do {
      if (*(char *)(*(int *)(&DAT_00e2079c + param_1 * 4) + iVar2) == '\0') {
        return;
      }
      iVar2 = iVar2 + 1;
      *piVar1 = *piVar1 + 1;
    } while (iVar2 <= *(int *)(&DAT_00e208f0 + param_1 * 4));
  }
  return;
}
