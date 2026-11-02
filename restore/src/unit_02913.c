#include "restore_types.h"
#define DAT_01266434 (*(undefined *)0x01266434u)
#define DAT_01266960 (*(uint32_t *)0x01266960u)
#define DAT_01266a60 (*(uint32_t *)0x01266a60u)
#define _DAT_01266430 (*(undefined4 *)0x01266430u)
extern undefined4 FUN_005669c6();
extern void FUN_005672c8();

DWORD FUN_005662e3(uint param_1,LONG param_2,DWORD param_3)

{
  byte *pbVar1;
  HANDLE hFile;
  DWORD DVar2;
  DWORD DVar3;
  int iVar4;
  
  if (param_1 < DAT_01266a60) {
    iVar4 = (param_1 & 0x1f) * 8;
    if ((*(byte *)((&DAT_01266960)[(int)param_1 >> 5] + 4 + iVar4) & 1) != 0) {
      hFile = (HANDLE)FUN_005669c6(param_1);
      if (hFile == (HANDLE)0xffffffff) {
        _DAT_01266430 = 9;
        return 0xffffffff;
      }
      DVar2 = SetFilePointer(hFile,param_2,(PLONG)0x0,param_3);
      if (DVar2 == 0xffffffff) {
        DVar3 = GetLastError();
      }
      else {
        DVar3 = 0;
      }
      if (DVar3 != 0) {
        FUN_005672c8(DVar3);
        return 0xffffffff;
      }
      pbVar1 = (byte *)((&DAT_01266960)[(int)param_1 >> 5] + 4 + iVar4);
      *pbVar1 = *pbVar1 & 0xfd;
      return DVar2;
    }
  }
  DAT_01266434 = 0;
  _DAT_01266430 = 9;
  return 0xffffffff;
}
