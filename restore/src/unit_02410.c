#include "restore_types.h"
#define DAT_00784f18 (*(undefined *)0x00784f18u)
extern undefined4 FUN_00443000();
extern undefined4 FUN_00443040();
extern bool FUN_00447790();

undefined4 FUN_0052dac0(undefined4 param_1,undefined4 param_2)

{
  char cVar1;
  int iVar2;
  
  iVar2 = FUN_00443000(param_1,param_2);
  if (iVar2 != 0) {
    iVar2 = FUN_00443040(param_1,param_2);
    if (iVar2 != 0) {
      cVar1 = FUN_00447790(DAT_00784f18,param_2);
      if (cVar1 != '\0') {
        return 1;
      }
    }
  }
  return 0;
}
