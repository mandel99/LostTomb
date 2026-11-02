#include "restore_types.h"
#define DAT_005c9298 (*(undefined4 *)0x005c9298u)
extern int FUN_00562370();

undefined4 FUN_004265d0(LPCSTR param_1,char *param_2,DWORD param_3)

{
  char cVar1;
  UINT UVar2;
  HANDLE hFile;
  DWORD DVar3;
  undefined4 uVar4;
  int iVar5;
  uint uVar6;
  uint uVar7;
  char *pcVar8;
  CHAR *pCVar9;
  char *pcVar10;
  char *pcVar11;
  char local_21c [4];
  DWORD local_218;
  DWORD local_214;
  DWORD local_210;
  DWORD local_20c [4];
  CHAR local_1fc [52];
  char local_1c8 [256];
  CHAR local_c8 [200];
  
  pcVar8 = local_1c8;
  for (iVar5 = 0x40; iVar5 != 0; iVar5 = iVar5 + -1) {
    pcVar8[0] = '\0';
    pcVar8[1] = '\0';
    pcVar8[2] = '\0';
    pcVar8[3] = '\0';
    pcVar8 = pcVar8 + 4;
  }
  pCVar9 = local_c8;
  for (iVar5 = 0x32; iVar5 != 0; iVar5 = iVar5 + -1) {
    pCVar9[0] = '\0';
    pCVar9[1] = '\0';
    pCVar9[2] = '\0';
    pCVar9[3] = '\0';
    pCVar9 = pCVar9 + 4;
  }
  pCVar9 = local_1fc;
  for (iVar5 = 0xc; iVar5 != 0; iVar5 = iVar5 + -1) {
    pCVar9[0] = '\0';
    pCVar9[1] = '\0';
    pCVar9[2] = '\0';
    pCVar9[3] = '\0';
    pCVar9 = pCVar9 + 4;
  }
  local_20c[3] = 0xffffffff;
  local_20c[2] = 0xffffffff;
  local_20c[1] = 0xffffffff;
  local_20c[0] = 0xffffffff;
  local_210 = 0xffffffff;
  local_218 = 0xffffffff;
  local_214 = 0xffffffff;
  local_21c[0] = 'n';
  local_21c[1] = 0x35;
  local_21c[2] = 0;
  pCVar9[0] = '\0';
  pCVar9[1] = '\0';
  UVar2 = GetDriveTypeA(param_1);
  if (UVar2 != 5) {
    return 0xffffffff;
  }
  uVar6 = 0xffffffff;
  pcVar8 = local_21c;
  do {
    pcVar11 = pcVar8;
    if (uVar6 == 0) break;
    uVar6 = uVar6 - 1;
    pcVar11 = pcVar8 + 1;
    cVar1 = *pcVar8;
    pcVar8 = pcVar11;
  } while (cVar1 != '\0');
  uVar6 = ~uVar6;
  pcVar8 = pcVar11 + -uVar6;
  pcVar11 = local_1fc;
  for (uVar7 = uVar6 >> 2; uVar7 != 0; uVar7 = uVar7 - 1) {
    *(undefined4 *)pcVar11 = *(undefined4 *)pcVar8;
    pcVar8 = pcVar8 + 4;
    pcVar11 = pcVar11 + 4;
  }
  for (uVar6 = uVar6 & 3; uVar6 != 0; uVar6 = uVar6 - 1) {
    *pcVar11 = *pcVar8;
    pcVar8 = pcVar8 + 1;
    pcVar11 = pcVar11 + 1;
  }
  GetVolumeInformationA
            (param_1,local_c8,200,local_20c + 3,local_20c + 2,local_20c + 1,local_1fc,0x32);
  iVar5 = FUN_00562370(&DAT_005c9298,local_1fc);
  if (iVar5 != 0) {
    return 0;
  }
  local_218 = 4;
  GetDiskFreeSpaceA(param_1,local_20c,&local_210,&local_218,&local_214);
  if (local_218 != 0) {
    return 1;
  }
  uVar6 = 0xffffffff;
  do {
    pcVar8 = param_1;
    if (uVar6 == 0) break;
    uVar6 = uVar6 - 1;
    pcVar8 = param_1 + 1;
    cVar1 = *param_1;
    param_1 = pcVar8;
  } while (cVar1 != '\0');
  uVar6 = ~uVar6;
  pcVar8 = pcVar8 + -uVar6;
  pcVar11 = local_1c8;
  for (uVar7 = uVar6 >> 2; uVar7 != 0; uVar7 = uVar7 - 1) {
    *(undefined4 *)pcVar11 = *(undefined4 *)pcVar8;
    pcVar8 = pcVar8 + 4;
    pcVar11 = pcVar11 + 4;
  }
  for (uVar6 = uVar6 & 3; uVar6 != 0; uVar6 = uVar6 - 1) {
    *pcVar11 = *pcVar8;
    pcVar8 = pcVar8 + 1;
    pcVar11 = pcVar11 + 1;
  }
  uVar6 = 0xffffffff;
  do {
    pcVar8 = param_2;
    if (uVar6 == 0) break;
    uVar6 = uVar6 - 1;
    pcVar8 = param_2 + 1;
    cVar1 = *param_2;
    param_2 = pcVar8;
  } while (cVar1 != '\0');
  uVar6 = ~uVar6;
  iVar5 = -1;
  pcVar11 = local_1c8;
  do {
    pcVar10 = pcVar11;
    if (iVar5 == 0) break;
    iVar5 = iVar5 + -1;
    pcVar10 = pcVar11 + 1;
    cVar1 = *pcVar11;
    pcVar11 = pcVar10;
  } while (cVar1 != '\0');
  pcVar8 = pcVar8 + -uVar6;
  pcVar11 = pcVar10 + -1;
  for (uVar7 = uVar6 >> 2; uVar7 != 0; uVar7 = uVar7 - 1) {
    *(undefined4 *)pcVar11 = *(undefined4 *)pcVar8;
    pcVar8 = pcVar8 + 4;
    pcVar11 = pcVar11 + 4;
  }
  for (uVar6 = uVar6 & 3; uVar6 != 0; uVar6 = uVar6 - 1) {
    *pcVar11 = *pcVar8;
    pcVar8 = pcVar8 + 1;
    pcVar11 = pcVar11 + 1;
  }
  hFile = CreateFileA(local_1c8,0x80000000,1,(LPSECURITY_ATTRIBUTES)0x0,3,0x60000001,(HANDLE)0x0);
  if (hFile == (HANDLE)0xffffffff) {
    CloseHandle((HANDLE)0xffffffff);
    return 4;
  }
  DVar3 = GetFileSize(hFile,(LPDWORD)0x0);
  if (DVar3 == 0xffffffff) {
    CloseHandle(hFile);
    return 5;
  }
  if (DVar3 != param_3) {
    CloseHandle(hFile);
    return 6;
  }
  CloseHandle(hFile);
  uVar6 = local_214 * local_210 * local_20c[0];
  if ((0x29dfffff < uVar6) || (uVar4 = 10, uVar6 < 0x2ad00001)) {
    uVar4 = 7;
  }
  return uVar4;
}
