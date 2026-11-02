#include "restore_types.h"
#include "sprite_libraries.h"
#define PTR_s_Obelisk3x3a_005e4588 (*(unsigned char * *)0x005e4588u)
#define PTR_s_Obelisk5x5a_005e4598 (*(unsigned char * *)0x005e4598u)
extern int FUN_004f52b0();

void FUN_004e5950(int param_1)

{
  int iVar1;
  
  iVar1 = FUN_004f52b0(param_1);
  if (iVar1 == 0xcf) {
    FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,PTR_s_Obelisk3x3a_005e4588,param_1 + 5);
    return;
  }
  if (iVar1 == 0xd0) {
    FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,PTR_s_Obelisk5x5a_005e4598,param_1 + 5);
  }
  return;
}
