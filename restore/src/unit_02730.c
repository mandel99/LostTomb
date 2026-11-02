#include "restore_types.h"
#define DAT_005761d0 (*(undefined4 *)0x005761d0u)
#define s_QuickTime_qts_00604860 (*(char (*)[14])0x00604860u)

HMODULE FUN_005603f0(void)

{
  char cVar1;
  HMODULE pHVar2;
  DWORD DVar3;
  char *pcVar4;
  UINT UVar5;
  uint uVar6;
  int iVar7;
  uint uVar8;
  char *pcVar9;
  char *pcVar10;
  CHAR local_104 [260];
  
  pHVar2 = GetModuleHandleA(s_QuickTime_qts_00604860);
  if ((pHVar2 != (HMODULE)0x0) && (DVar3 = GetModuleFileNameA(pHVar2,local_104,0x104), DVar3 != 0))
  {
    pHVar2 = LoadLibraryA(local_104);
    return pHVar2;
  }
  DVar3 = GetModuleFileNameA((HMODULE)0x0,local_104,0x104);
  if (DVar3 != 0) {
    uVar6 = 0xffffffff;
    pcVar4 = local_104;
    do {
      if (uVar6 == 0) break;
      uVar6 = uVar6 - 1;
      cVar1 = *pcVar4;
      pcVar4 = pcVar4 + 1;
    } while (cVar1 != '\0');
    pcVar4 = local_104 + (~uVar6 - 2);
    if (local_104 <= pcVar4) {
      do {
        if ((*pcVar4 == '\\') || (*pcVar4 == '/')) {
          if (local_104 <= pcVar4) {
            pcVar4[1] = '\0';
            uVar6 = 0xffffffff;
            pcVar4 = s_QuickTime_qts_00604860;
            goto code_r0x00560497;
          }
          break;
        }
        pcVar4 = pcVar4 + -1;
      } while (local_104 <= pcVar4);
    }
  }
  goto LAB_005604cc;
  while( true ) {
    uVar6 = uVar6 - 1;
    pcVar10 = pcVar4 + 1;
    cVar1 = *pcVar4;
    pcVar4 = pcVar10;
    if (cVar1 == '\0') break;
code_r0x00560497: ;
    pcVar10 = pcVar4;
    if (uVar6 == 0) break;
  }
  uVar6 = ~uVar6;
  iVar7 = -1;
  pcVar4 = local_104;
  do {
    pcVar9 = pcVar4;
    if (iVar7 == 0) break;
    iVar7 = iVar7 + -1;
    pcVar9 = pcVar4 + 1;
    cVar1 = *pcVar4;
    pcVar4 = pcVar9;
  } while (cVar1 != '\0');
  pcVar4 = pcVar10 + -uVar6;
  pcVar10 = pcVar9 + -1;
  for (uVar8 = uVar6 >> 2; uVar8 != 0; uVar8 = uVar8 - 1) {
    *(undefined4 *)pcVar10 = *(undefined4 *)pcVar4;
    pcVar4 = pcVar4 + 4;
    pcVar10 = pcVar10 + 4;
  }
  for (uVar6 = uVar6 & 3; uVar6 != 0; uVar6 = uVar6 - 1) {
    *pcVar10 = *pcVar4;
    pcVar4 = pcVar4 + 1;
    pcVar10 = pcVar10 + 1;
  }
  pHVar2 = LoadLibraryA(local_104);
  if (pHVar2 != (HMODULE)0x0) {
    return pHVar2;
  }
LAB_005604cc: ;
  UVar5 = GetSystemDirectoryA(local_104,0x104);
  if (UVar5 == 0) {
    return (HMODULE)0x0;
  }
  uVar6 = 0xffffffff;
  pcVar4 = &DAT_005761d0;
  do {
    pcVar10 = pcVar4;
    if (uVar6 == 0) break;
    uVar6 = uVar6 - 1;
    pcVar10 = pcVar4 + 1;
    cVar1 = *pcVar4;
    pcVar4 = pcVar10;
  } while (cVar1 != '\0');
  uVar6 = ~uVar6;
  iVar7 = -1;
  pcVar4 = local_104;
  do {
    pcVar9 = pcVar4;
    if (iVar7 == 0) break;
    iVar7 = iVar7 + -1;
    pcVar9 = pcVar4 + 1;
    cVar1 = *pcVar4;
    pcVar4 = pcVar9;
  } while (cVar1 != '\0');
  pcVar4 = pcVar10 + -uVar6;
  pcVar10 = pcVar9 + -1;
  for (uVar8 = uVar6 >> 2; uVar8 != 0; uVar8 = uVar8 - 1) {
    *(undefined4 *)pcVar10 = *(undefined4 *)pcVar4;
    pcVar4 = pcVar4 + 4;
    pcVar10 = pcVar10 + 4;
  }
  for (uVar6 = uVar6 & 3; uVar6 != 0; uVar6 = uVar6 - 1) {
    *pcVar10 = *pcVar4;
    pcVar4 = pcVar4 + 1;
    pcVar10 = pcVar10 + 1;
  }
  uVar6 = 0xffffffff;
  pcVar4 = s_QuickTime_qts_00604860;
  do {
    pcVar10 = pcVar4;
    if (uVar6 == 0) break;
    uVar6 = uVar6 - 1;
    pcVar10 = pcVar4 + 1;
    cVar1 = *pcVar4;
    pcVar4 = pcVar10;
  } while (cVar1 != '\0');
  uVar6 = ~uVar6;
  iVar7 = -1;
  pcVar4 = local_104;
  do {
    pcVar9 = pcVar4;
    if (iVar7 == 0) break;
    iVar7 = iVar7 + -1;
    pcVar9 = pcVar4 + 1;
    cVar1 = *pcVar4;
    pcVar4 = pcVar9;
  } while (cVar1 != '\0');
  pcVar4 = pcVar10 + -uVar6;
  pcVar10 = pcVar9 + -1;
  for (uVar8 = uVar6 >> 2; uVar8 != 0; uVar8 = uVar8 - 1) {
    *(undefined4 *)pcVar10 = *(undefined4 *)pcVar4;
    pcVar4 = pcVar4 + 4;
    pcVar10 = pcVar10 + 4;
  }
  for (uVar6 = uVar6 & 3; uVar6 != 0; uVar6 = uVar6 - 1) {
    *pcVar10 = *pcVar4;
    pcVar4 = pcVar4 + 1;
    pcVar10 = pcVar10 + 1;
  }
  pHVar2 = LoadLibraryA(local_104);
  return pHVar2;
}
