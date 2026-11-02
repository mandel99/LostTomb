#include "restore_types.h"
#include "simulation_tick.h"
#define DAT_00605a3c (*(int32_t *)0x00605a3cu)
#define _DAT_00784c60 (*(int16_t *)0x00784c60u)
#define _DAT_00784c62 (*(int16_t *)0x00784c62u)

void FUN_0044bca0(int32_t *in_ECX)

{
  char cVar1;
  int *piVar2;
  int iVar3;
  
  if ((((DAT_00605a3c != 1) && (_DAT_00784c60 != -1)) && (_DAT_00784c62 != -1)) &&
     (iVar3 = 0, 0 < *in_ECX)) {
    piVar2 = in_ECX + 1;
    while ((*(char *)((int)piVar2 + 2) != '\x03' || (cVar1 = FUN_0044be30((uintptr_t)piVar2), cVar1 == '\0'))) {
      iVar3 = iVar3 + 1;
      piVar2 = piVar2 + 0x1f;
      if (*in_ECX <= iVar3) {
        return;
      }
    }
    *(undefined1 *)(in_ECX + 0x122d) = 1;
    return;
  }
  return;
}
