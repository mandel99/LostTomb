#include "restore_types.h"
extern undefined4 FUN_0047feb0();

undefined4 FUN_0047fe60(undefined4 param_1,undefined4 param_2,undefined4 param_3,int param_4)

{
  int iVar1;
  int iVar2;
  
  iVar2 = 1;
  if (0 < param_4) {
    do {
      iVar1 = FUN_0047feb0(param_1,param_2,param_3,iVar2,0);
      if (iVar1 != 0) {
        return 1;
      }
      iVar2 = iVar2 + 1;
    } while (iVar2 <= param_4);
  }
  return 0;
}
