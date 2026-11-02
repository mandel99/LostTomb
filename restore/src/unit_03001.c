#include "restore_types.h"
#define DAT_012666e8 (*(undefined *)0x012666e8u)
#define DAT_012666ec (*(undefined *)0x012666ecu)
#define DAT_012666f0 (*(undefined *)0x012666f0u)

int FUN_00569c11(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  HMODULE hModule;
  int iVar1;
  
  iVar1 = 0;
  if (DAT_012666e8 == (FARPROC)0x0) {
    hModule = LoadLibraryA("user32.dll");
    if (hModule != (HMODULE)0x0) {
      DAT_012666e8 = GetProcAddress(hModule,"MessageBoxA");
      if (DAT_012666e8 != (FARPROC)0x0) {
        DAT_012666ec = GetProcAddress(hModule,"GetActiveWindow");
        DAT_012666f0 = GetProcAddress(hModule,"GetLastActivePopup");
        goto LAB_00569c60;
      }
    }
    iVar1 = 0;
  }
  else {
LAB_00569c60: ;
    if (DAT_012666ec != (FARPROC)0x0) {
      iVar1 = ((code *)DAT_012666ec)();
      if ((iVar1 != 0) && (DAT_012666f0 != (FARPROC)0x0)) {
        iVar1 = ((code *)DAT_012666f0)(iVar1);
      }
    }
    iVar1 = ((code *)DAT_012666e8)(iVar1,param_1,param_2,param_3);
  }
  return iVar1;
}
