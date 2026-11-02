#include "restore_types.h"
#include "file_list.h"
#define DAT_00eadfac (*(int32_t *)0x00eadfacu)
#define DAT_00eadfb0 (*(int32_t *)0x00eadfb0u)
#define DAT_00eadfb8 (*(int32_t *)0x00eadfb8u)
extern void FUN_004ce3a0();
extern int FUN_00562370();

void FUN_00516b50(undefined4 param_1)

{
  int iVar1;
  int iVar2;
  
  FUN_0041c740(RESTORE_FAMILY_FILES,(const char *)(uintptr_t)param_1,0);
  iVar2 = 0;
  if (0 < DAT_00eadfb0) {
    do {
      if ((iVar2 < 0) || (DAT_00eadfb0 <= iVar2)) {
        iVar1 = 0;
      }
      else {
        iVar1 = *(int *)(DAT_00eadfac + iVar2 * 4) + DAT_00eadfb8;
      }
      FUN_004ce3a0(iVar1);
      iVar2 = iVar2 + 1;
    } while (iVar2 < DAT_00eadfb0);
  }
  FUN_0041ca10(RESTORE_FAMILY_FILES,(RestoreFileCompare)FUN_00562370);
  return;
}
