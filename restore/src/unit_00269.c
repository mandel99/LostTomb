#include "restore_types.h"
#include "file_list.h"
extern void * FUN_00561a1d();
extern void FUN_00561a2b();

uint8_t FUN_0041c740(RestoreFileList *receiver,const char *param_1,char param_2)

{
  int iVar1;
  char cVar2;
  undefined4 *puVar3;
  void *pvVar4;
  int iVar5;
  int in_ECX=(int)(uintptr_t)receiver;
  uint uVar6;
  uint uVar7;
  undefined4 *puVar8;
  undefined4 *puVar9;
  int local_14;
  int local_10;
  int local_c;
  undefined4 local_8;
  uint local_4;
  
  if (param_2 == '\0') {
    *(undefined4 *)(in_ECX + 8) = 0;
    *(undefined4 *)(in_ECX + 0x18) = 0;
  }
  local_8 = *(undefined4 *)(in_ECX + 8);
  iVar1 = *(int *)(in_ECX + 0x18);
  local_c = iVar1;
  cVar2 = FUN_0041c8f0(receiver,param_1,&local_10,&local_14);
  while( true ) {
    if (cVar2 != '\0') {
      return 1;
    }
    *(undefined4 *)(in_ECX + 8) = local_8;
    *(int *)(in_ECX + 0x18) = iVar1;
    if ((local_10 <= *(int *)(in_ECX + 0x14)) && (local_14 <= *(int *)(in_ECX + 0xc))) break;
    if (*(int *)(in_ECX + 0x14) < local_10 + iVar1) {
      if (param_2 == '\0') {
        if (*(int *)(in_ECX + 0x10) != 0) {
          FUN_00561a2b(*(int *)(in_ECX + 0x10));
          *(undefined4 *)(in_ECX + 0x10) = 0;
        }
        *(undefined4 *)(in_ECX + 0x14) = 0;
      }
      uVar7 = local_10 + 1000 + *(int *)(in_ECX + 0x14);
      if ((int)uVar7 < 1) {
        return 0;
      }
      local_4 = uVar7;
      puVar3 = FUN_00561a1d(uVar7);
      if (puVar3 == (undefined4 *)0x0) {
        return 0;
      }
      if (*(undefined4 **)(in_ECX + 0x10) != (undefined4 *)0x0) {
        uVar7 = *(uint *)(in_ECX + 0x18);
        puVar8 = *(undefined4 **)(in_ECX + 0x10);
        puVar9 = puVar3;
        for (uVar6 = uVar7 >> 2; uVar6 != 0; uVar6 = uVar6 - 1) {
          *puVar9 = *puVar8;
          puVar8 = puVar8 + 1;
          puVar9 = puVar9 + 1;
        }
        for (uVar7 = uVar7 & 3; uVar7 != 0; uVar7 = uVar7 - 1) {
          *(undefined1 *)puVar9 = *(undefined1 *)puVar8;
          puVar8 = (undefined4 *)((int)puVar8 + 1);
          puVar9 = (undefined4 *)((int)puVar9 + 1);
        }
        FUN_00561a2b(*(undefined4 *)(in_ECX + 0x10));
        uVar7 = local_4;
      }
      *(undefined4 **)(in_ECX + 0x10) = puVar3;
      *(uint *)(in_ECX + 0x14) = uVar7;
    }
    if (*(int *)(in_ECX + 0xc) < *(int *)(in_ECX + 8) + local_14) {
      if (param_2 == '\0') {
        if (*(int *)(in_ECX + 4) != 0) {
          FUN_00561a2b(*(int *)(in_ECX + 4));
          /* Native41C859 leaves self+4 unchanged until41C8B2. */
        }
        *(undefined4 *)(in_ECX + 0xc) = 0;
      }
      iVar1 = local_14 + 100 + *(int *)(in_ECX + 0xc);
      pvVar4 = FUN_00561a1d(iVar1 * 4);
      if (pvVar4 == (void *)0x0) {
        return 0;
      }
      if (*(int *)(in_ECX + 4) != 0) {
        iVar5 = 0;
        if (0 < *(int *)(in_ECX + 8)) {
          do {
            iVar5 = iVar5 + 1;
            *(undefined4 *)((int)pvVar4 + iVar5 * 4 + -4) =
                 *(undefined4 *)(*(int *)(in_ECX + 4) + -4 + iVar5 * 4);
          } while (iVar5 < *(int *)(in_ECX + 8));
        }
        FUN_00561a2b(*(undefined4 *)(in_ECX + 4));
      }
      *(void **)(in_ECX + 4) = pvVar4;
      *(int *)(in_ECX + 0xc) = iVar1;
    }
    cVar2 = FUN_0041c8f0(receiver,param_1,&local_10,&local_14);
    iVar1 = local_c;
  }
  return 0;
}
