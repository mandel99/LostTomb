#include "restore_types.h"
#include "file_list.h"
#define DAT_00704c08 (*(int32_t *)0x00704c08u)
#define DAT_00704db0 (*(int32_t *)0x00704db0u)
#define DAT_00704db4 (*(int32_t *)0x00704db4u)
extern void FUN_004ce3a0();
extern int FUN_00562370();

void FUN_0041f810(undefined4 param_1)

{
  undefined4 uVar1;
  int iVar2;
  
  FUN_0041df70(RESTORE_MISSION_FILES,(const char *)(uintptr_t)param_1,0);
  DAT_00704db4 = DAT_00704c08;
  if (DAT_00704db0 == 0) {
    iVar2 = 0;
    if (0 < DAT_00704c08) {
      do {
        uVar1 = (uint32_t)(uintptr_t)FUN_0041e010(RESTORE_MISSION_FILES,iVar2);
        FUN_004ce3a0(uVar1);
        iVar2 = iVar2 + 1;
      } while (iVar2 < DAT_00704db4);
    }
    FUN_0041ca10(RESTORE_MISSION_FILES,(RestoreFileCompare)FUN_00562370);
  }
  return;
}
