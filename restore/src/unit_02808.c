#include "restore_types.h"
extern void * FUN_00561ba6();
extern undefined4 FUN_005653d0();

void * __cdecl FUN_00561b7a(size_t _Size,int _NhFlag)

{
  void *pvVar1;
  int iVar2;
  
  if (_Size < 0xffffffe1) {
    do {
      pvVar1 = FUN_00561ba6(_Size);
      if (pvVar1 != (void *)0x0) {
        return pvVar1;
      }
      if (_NhFlag == 0) {
        return (void *)0x0;
      }
      iVar2 = FUN_005653d0(_Size);
    } while (iVar2 != 0);
  }
  return (void *)0x0;
}
