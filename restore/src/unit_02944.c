#include "restore_types.h"
#define CRT_ENVIRONMENT_MODE (*(volatile uint32_t *)0x0126669cu)
extern void * __cdecl FUN_00561b68(size_t bytes);
extern void FUN_00561bdc(void *allocation);
extern undefined4 * FUN_00563400(undefined4 *destination,undefined4 *source,uint32_t bytes);

/* Preserve the recovered acquisition, conversion and release branches. Native
   mode selection is a DWORD; both successful paths return an owned byte block. */
LPSTR restore_crt_acquire_environment_block(void)

{
  uint32_t mode = CRT_ENVIRONMENT_MODE;
  char cVar1;
  WCHAR WVar2;
  WCHAR *pWVar3;
  WCHAR *pWVar4;
  int iVar5;
  size_t _Size;
  LPSTR pCVar6;
  char *pcVar7;
  LPWCH lpWideCharStr;
  LPCH pCVar9;
  LPSTR local_8;
  char *pcVar8;
  
  lpWideCharStr = (LPWCH)0x0;
  pCVar9 = (LPCH)0x0;
  if (mode == 0) {
    lpWideCharStr = GetEnvironmentStringsW();
    if (lpWideCharStr != (LPWCH)0x0) {
      CRT_ENVIRONMENT_MODE = 1;
LAB_00567cf1: ;
      if ((lpWideCharStr == (LPWCH)0x0) &&
         (lpWideCharStr = GetEnvironmentStringsW(), lpWideCharStr == (LPWCH)0x0)) {
        return (LPSTR)0x0;
      }
      WVar2 = *lpWideCharStr;
      pWVar4 = lpWideCharStr;
      while (WVar2 != L'\0') {
        do {
          pWVar3 = pWVar4;
          pWVar4 = pWVar3 + 1;
        } while (*pWVar4 != L'\0');
        pWVar4 = pWVar3 + 2;
        WVar2 = *pWVar4;
      }
      iVar5 = ((int32_t)((uint32_t)(uintptr_t)pWVar4 -
                        (uint32_t)(uintptr_t)lpWideCharStr) >> 1) + 1;
      _Size = WideCharToMultiByte(0,0,lpWideCharStr,iVar5,(LPSTR)0x0,0,(LPCSTR)0x0,(LPBOOL)0x0);
      local_8 = (LPSTR)0x0;
      if (((_Size != 0) && (pCVar6 = FUN_00561b68(_Size), pCVar6 != (LPSTR)0x0)) &&
         (iVar5 = WideCharToMultiByte(0,0,lpWideCharStr,iVar5,pCVar6,_Size,(LPCSTR)0x0,(LPBOOL)0x0),
         local_8 = pCVar6, iVar5 == 0)) {
        FUN_00561bdc(pCVar6);
        local_8 = (LPSTR)0x0;
      }
      FreeEnvironmentStringsW(lpWideCharStr);
      return local_8;
    }
    pCVar9 = GetEnvironmentStrings();
    if (pCVar9 == (LPCH)0x0) {
      return (LPSTR)0x0;
    }
    CRT_ENVIRONMENT_MODE = 2;
  }
  else {
    if (mode == 1) goto LAB_00567cf1;
    if (mode != 2) {
      return (LPSTR)0x0;
    }
  }
  if ((pCVar9 == (LPCH)0x0) && (pCVar9 = GetEnvironmentStrings(), pCVar9 == (LPCH)0x0)) {
    return (LPSTR)0x0;
  }
  cVar1 = *pCVar9;
  pcVar7 = pCVar9;
  while (cVar1 != '\0') {
    do {
      pcVar8 = pcVar7;
      pcVar7 = pcVar8 + 1;
    } while (*pcVar7 != '\0');
    pcVar7 = pcVar8 + 2;
    cVar1 = *pcVar7;
  }
  _Size = (uint32_t)(uintptr_t)pcVar7 - (uint32_t)(uintptr_t)pCVar9 + 1u;
  pCVar6 = FUN_00561b68(_Size);
  if (pCVar6 == (LPSTR)0x0) {
    pCVar6 = (LPSTR)0x0;
  }
  else {
    (void)FUN_00563400((undefined4 *)pCVar6,(undefined4 *)pCVar9,(uint32_t)_Size);
  }
  FreeEnvironmentStringsA(pCVar9);
  return pCVar6;
}

LPSTR FUN_00567c9a(void)
{
  return restore_crt_acquire_environment_block();
}
