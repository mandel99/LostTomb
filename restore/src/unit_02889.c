#include "restore_types.h"
#define DAT_01266434 (*(undefined *)0x01266434u)
#define DAT_01266960 (*(uint32_t *)0x01266960u)
#define DAT_01266a60 (*(uint32_t *)0x01266a60u)
#define _DAT_01266430 (*(undefined4 *)0x01266430u)
extern undefined4 FUN_0056694c();
extern undefined4 FUN_005669c6();
extern void FUN_005672c8();

undefined4 FUN_005647eb(uint param_1)

{
  int iVar1;
  int iVar2;
  HANDLE hObject;
  BOOL BVar3;
  DWORD DVar4;
  int iVar5;
  
  if (DAT_01266a60 <= param_1) {
    DAT_01266434 = 0;
    _DAT_01266430 = 9;
    return 0xffffffff;
  }
  iVar5 = (param_1 & 0x1f) * 8;
  if ((*(byte *)((&DAT_01266960)[(int)param_1 >> 5] + 4 + iVar5) & 1) == 0) {
    _DAT_01266430 = 9;
    DAT_01266434 = 0;
    return 0xffffffff;
  }
  iVar1 = FUN_005669c6(param_1);
  if (iVar1 != -1) {
    if ((param_1 == 1) || (param_1 == 2)) {
      iVar1 = FUN_005669c6(2);
      iVar2 = FUN_005669c6(1);
      if (iVar2 == iVar1) goto LAB_00564864;
    }
    hObject = (HANDLE)FUN_005669c6(param_1);
    BVar3 = CloseHandle(hObject);
    if (BVar3 == 0) {
      DVar4 = GetLastError();
      goto LAB_00564866;
    }
  }
LAB_00564864: ;
  DVar4 = 0;
LAB_00564866: ;
  FUN_0056694c(param_1);
  *(undefined1 *)((&DAT_01266960)[(int)param_1 >> 5] + 4 + iVar5) = 0;
  if (DVar4 == 0) {
    return 0;
  }
  FUN_005672c8(DVar4);
  return 0xffffffff;
}
