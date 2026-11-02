#include "restore_types.h"
#define DAT_00bcd694 (*(undefined4 *)0x00bcd694u)
#define DAT_00bcd6b4 (*(undefined4 *)0x00bcd6b4u)
#define DAT_00d056f8 (*(undefined *)0x00d056f8u)
extern undefined4 FUN_00496de0();

undefined4 FUN_00496010(undefined4 param_1,undefined4 param_2,undefined4 param_3,int param_4)

{
  int iVar1;
  
  iVar1 = (param_4 + DAT_00d056f8 * 0x61) * 4;
  return FUN_00496de0(param_1,param_2,param_3,*(undefined4 *)(&DAT_00bcd694 + iVar1),
               *(undefined4 *)(&DAT_00bcd6b4 + iVar1),param_4);
}
