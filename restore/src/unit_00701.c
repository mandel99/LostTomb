#include "restore_types.h"
extern undefined4 FUN_004493d0();
extern undefined4 FUN_004493e0();
extern bool FUN_00449440();
extern undefined4 FUN_00449530();

undefined4 FUN_00449550(int param_1)

{
  char cVar1;
  int iVar2;
  undefined4 uVar3;
  int in_ECX;
  
  iVar2 = FUN_004493d0();
  if (iVar2 <= param_1) {
    iVar2 = FUN_004493e0();
    if (param_1 < iVar2) {
      cVar1 = *(char *)(in_ECX + 2);
      if (cVar1 == '\x01') {
        uVar3 = FUN_00449440(param_1);
        return uVar3;
      }
      if (cVar1 == '\x17') {
        uVar3 = FUN_00449530(param_1);
        return uVar3;
      }
      if ((((cVar1 != '\r') && (cVar1 != '\x0e')) && (cVar1 != '\x0f')) && (cVar1 != '\x10')) {
        return 1;
      }
      if ((param_1 != 0x1b) && (param_1 != 0x15)) {
        return 1;
      }
    }
  }
  return 0;
}
