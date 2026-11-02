#include "restore_types.h"
#define DAT_00605a3c (*(undefined *)0x00605a3cu)
#define DAT_00e94e20 (*(undefined *)0x00e94e20u)
extern undefined4 FUN_00480770();
extern void FUN_004e2970();

void FUN_004e3280(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  
  if (((DAT_00605a3c != 1) && (DAT_00e94e20 == 0)) &&
     ((param_1 == 0x3b || ((param_1 == 0xac || (param_1 == 0xad)))))) {
    iVar1 = FUN_00480770(param_2,param_3,param_4);
    if (iVar1 == 0) {
      FUN_004e2970(0x29,0,0);
    }
  }
  return;
}
