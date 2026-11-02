#include "restore_types.h"
extern void FUN_005672c8();

undefined4 FUN_00562bb5(LPCSTR param_1)

{
  BOOL BVar1;
  DWORD DVar2;
  
  BVar1 = DeleteFileA(param_1);
  if (BVar1 == 0) {
    DVar2 = GetLastError();
  }
  else {
    DVar2 = 0;
  }
  if (DVar2 != 0) {
    FUN_005672c8(DVar2);
    return 0xffffffff;
  }
  return 0;
}
