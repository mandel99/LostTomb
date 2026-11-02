#include "restore_types.h"
#define DAT_00bcd694 (*(undefined4 *)0x00bcd694u)
#define DAT_00bcd6b4 (*(undefined4 *)0x00bcd6b4u)
extern undefined4 FUN_00496c70();

undefined4 FUN_00495cf0(int param_1,undefined4 param_2,undefined4 param_3,int param_4)

{
  int iVar1;
  
  iVar1 = (param_4 + param_1 * 0x61) * 4;
  return FUN_00496c70(param_1,param_2,param_3,*(undefined4 *)(&DAT_00bcd694 + iVar1),
               *(undefined4 *)(&DAT_00bcd6b4 + iVar1),param_4);
}
