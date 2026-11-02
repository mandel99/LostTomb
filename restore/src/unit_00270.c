#include "restore_types.h"
#include "file_list.h"

undefined1 FUN_0041c8f0(RestoreFileList *receiver,LPCSTR param_1,int *param_2,int *param_3)

{
  char cVar1;
  HANDLE hFindFile;
  BOOL BVar2;
  int in_ECX=(int)(uintptr_t)receiver;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  CHAR *pCVar6;
  CHAR *pCVar7;
  char *pcVar8;
  undefined1 local_149;
  _WIN32_FIND_DATAA local_140;
  
  *param_2 = 0;
  local_149 = 1;
  *param_3 = 0;
  hFindFile = restore_file_find_first(param_1,&local_140);
  if (hFindFile == (HANDLE)0xffffffff) {
    return 0;
  }
  do {
    uVar3 = 0xffffffff;
    pCVar6 = local_140.cFileName;
    do {
      if (uVar3 == 0) break;
      uVar3 = uVar3 - 1;
      cVar1 = *pCVar6;
      pCVar6 = pCVar6 + 1;
    } while (cVar1 != '\0');
    uVar3 = ~uVar3;
    *param_3 = *param_3 + 1;
    *param_2 = *param_2 + uVar3;
    if ((*(int *)(in_ECX + 8) < *(int *)(in_ECX + 0xc)) &&
       ((int)(*(int *)(in_ECX + 0x18) + uVar3) < *(int *)(in_ECX + 0x14))) {
      *(int *)(*(int *)(in_ECX + 4) + *(int *)(in_ECX + 8) * 4) = *(int *)(in_ECX + 0x18);
      uVar4 = 0xffffffff;
      pCVar6 = local_140.cFileName;
      do {
        pCVar7 = pCVar6;
        if (uVar4 == 0) break;
        uVar4 = uVar4 - 1;
        pCVar7 = pCVar6 + 1;
        cVar1 = *pCVar6;
        pCVar6 = pCVar7;
      } while (cVar1 != '\0');
      uVar4 = ~uVar4;
      pCVar6 = pCVar7 + -uVar4;
      pcVar8 = (char *)(*(int *)(*(int *)(in_ECX + 4) + *(int *)(in_ECX + 8) * 4) +
                       *(int *)(in_ECX + 0x10));
      for (uVar5 = uVar4 >> 2; uVar5 != 0; uVar5 = uVar5 - 1) {
        *(undefined4 *)pcVar8 = *(undefined4 *)pCVar6;
        pCVar6 = pCVar6 + 4;
        pcVar8 = pcVar8 + 4;
      }
      for (uVar4 = uVar4 & 3; uVar4 != 0; uVar4 = uVar4 - 1) {
        *pcVar8 = *pCVar6;
        pCVar6 = pCVar6 + 1;
        pcVar8 = pcVar8 + 1;
      }
      *(int *)(in_ECX + 8) = *(int *)(in_ECX + 8) + 1;
      *(uint *)(in_ECX + 0x18) = *(int *)(in_ECX + 0x18) + uVar3;
    }
    else {
      local_149 = 0;
    }
    BVar2 = restore_file_find_next(hFindFile,&local_140);
    if (BVar2 == 0) {
      restore_file_find_close(hFindFile);
      return local_149;
    }
  } while( true );
}
