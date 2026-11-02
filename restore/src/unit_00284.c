#include "restore_types.h"
#include "file_list.h"
#define DAT_00700994 (*(int32_t *)0x00700994u)
#define DAT_00704db0 (*(int32_t *)0x00704db0u)

uint8_t FUN_0041df70(RestoreFileList *receiver,const char *param_1,char param_2)

{
  int iVar1;
  undefined4 uVar2;
  int in_ECX=(int)(uintptr_t)receiver;
  
  iVar1 = 0;
  if (DAT_00704db0 == 0) {
    uVar2 = FUN_0041c740(receiver,param_1,param_2);
    return uVar2;
  }
  *(undefined4 *)(in_ECX + 8) = 0;
  if (0 < DAT_00700994) {
    do {
      *(int *)(in_ECX + 0x1c + *(int *)(in_ECX + 8) * 4) = iVar1;
      iVar1 = iVar1 + 1;
      *(int *)(in_ECX + 8) = *(int *)(in_ECX + 8) + 1;
    } while (iVar1 < DAT_00700994);
  }
  return CONCAT31((int3)((uint)iVar1 >> 8),1);
}
