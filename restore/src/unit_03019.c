#include "restore_types.h"
#include "crt_environment.h"
#include "host_allocator.h"
extern void * __cdecl FUN_00561b68(size_t bytes);
extern size_t FUN_00563380(const char *text);
extern uint32_t *FUN_00566750(void *destination,const void *source);
/* Preserve the existing duplicate-vector owner and its actual C declaration.
   Its DWORD entries carry full pointer bits; no duplicate algorithm is added. */
extern uint32_t *FUN_0056b09d(int32_t *vector);
typedef char RestoreCrtEnvironmentSetterPointerLayout[(sizeof(char *)==4 &&
 sizeof(WCHAR *)==4 && sizeof(uint32_t)==4)?1:-1];

/* Preserve the native ownership, append/replace/delete branches and callbacks. */
uint32_t restore_crt_set_environment_entry(char *param_1,int32_t param_2)

{
  char *pcVar1;
  int iVar2;
  char **piVar3;
  size_t sVar4;
  char *lpName;
  uint32_t name_length;
  char **piVar5;
  bool bVar6;
  
  if (param_1 == (char *)0x0) {
    return 0xffffffff;
  }
  pcVar1 = (char *)FUN_0056b104((uint8_t *)param_1,0x3d);
  if (pcVar1 == (char *)0x0) {
    return 0xffffffff;
  }
  if (param_1 == pcVar1) {
    return 0xffffffff;
  }
  bVar6 = pcVar1[1] == '\0';
  if (RESTORE_CRT_ENVIRONMENT_VECTOR == RESTORE_CRT_INITIAL_ENVIRONMENT_VECTOR) {
    RESTORE_CRT_ENVIRONMENT_VECTOR = (char **)FUN_0056b09d((int32_t *)RESTORE_CRT_ENVIRONMENT_VECTOR);
  }
  if (RESTORE_CRT_ENVIRONMENT_VECTOR == (char **)0x0) {
    if ((param_2 == 0) || (RESTORE_CRT_WIDE_ENVIRONMENT_VECTOR == (WCHAR **)0x0)) {
      if (bVar6) {
        return 0;
      }
      RESTORE_CRT_ENVIRONMENT_VECTOR = FUN_00561b68(4);
      if (RESTORE_CRT_ENVIRONMENT_VECTOR == (char **)0x0) {
        return 0xffffffff;
      }
      *RESTORE_CRT_ENVIRONMENT_VECTOR = 0;
      if (RESTORE_CRT_WIDE_ENVIRONMENT_VECTOR == (WCHAR **)0x0) {
        RESTORE_CRT_WIDE_ENVIRONMENT_VECTOR = FUN_00561b68(4);
        if (RESTORE_CRT_WIDE_ENVIRONMENT_VECTOR == (WCHAR **)0x0) {
          return 0xffffffff;
        }
        *RESTORE_CRT_WIDE_ENVIRONMENT_VECTOR = 0;
      }
    }
    else {
      iVar2 = FUN_0056a896();
      if (iVar2 != 0) {
        return 0xffffffff;
      }
    }
  }
  /* Native EDI keeps this vector across the name-lookup callback. */
  piVar3 = RESTORE_CRT_ENVIRONMENT_VECTOR;
  name_length = (uint32_t)(uintptr_t)pcVar1 - (uint32_t)(uintptr_t)param_1;
  iVar2 = FUN_0056b045((const uint8_t *)param_1,name_length);
  if ((iVar2 < 0) || (*piVar3 == 0)) {
    if (bVar6) {
      return 0;
    }
    if (iVar2 < 0) {
      iVar2 = (int32_t)(0u - (uint32_t)iVar2);
    }
    piVar3 = (char **)FUN_005645a0(piVar3,(uint32_t)iVar2 * 4u + 8u);
    if (piVar3 == (char **)0x0) {
      return 0xffffffff;
    }
    piVar3[iVar2] = param_1;
    piVar3[(uint32_t)iVar2 + 1u] = 0;
  }
  else {
    if (!bVar6) {
      piVar3[iVar2] = param_1;
      goto LAB_0056aff2;
    }
    piVar5 = piVar3 + iVar2;
    FUN_00561bdc(piVar3[iVar2]);
    for (; *piVar5 != 0; piVar5 = piVar5 + 1) {
      iVar2 = (int32_t)((uint32_t)iVar2 + 1u);
      *piVar5 = piVar5[1];
    }
    piVar3 = (char **)FUN_005645a0(piVar3,(uint32_t)iVar2 << 2);
    if (piVar3 == (char **)0x0) goto LAB_0056aff2;
  }
  RESTORE_CRT_ENVIRONMENT_VECTOR = piVar3;
LAB_0056aff2: ;
  if (param_2 != 0) {
    sVar4 = FUN_00563380(param_1);
    lpName = FUN_00561b68(sVar4 + 2);
    if (lpName != (char *)0x0) {
      FUN_00566750(lpName,param_1);
      lpName[name_length] = '\0';
      (void)SetEnvironmentVariableA(lpName,bVar6 ? (LPCSTR)0 : lpName + name_length + 1u);
      FUN_00561bdc(lpName);
    }
  }
  return 0;
}

uint32_t FUN_0056aebe(char *entry,int32_t update_os)
{
  return restore_crt_set_environment_entry(entry,update_os);
}
