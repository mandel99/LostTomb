#include "restore_types.h"
#define _DAT_00784c8c (*(undefined4 *)0x00784c8cu)
#define _DAT_00784c8e (*(undefined4 *)0x00784c8eu)
extern undefined4 FUN_00481420();
extern undefined4 FUN_0051ce90();

bool FUN_00457ac0(undefined4 param_1,undefined4 param_2)

{
  int iVar1;
  
  FUN_0051ce90((int)_DAT_00784c8c,(int)_DAT_00784c8e);
  iVar1 = FUN_00481420(param_1,param_2,3);
  return iVar1 != 0;
}
