#include "restore_types.h"
#define DAT_00bcd6b4 (*(undefined4 *)0x00bcd6b4u)
#define DAT_00ea42f8 (*(undefined4 *)0x00ea42f8u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern uint FUN_004963b0();
extern void FUN_00496640();
extern undefined4 FUN_00496660();

undefined4
FUN_00496de0(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,undefined4 param_5
            ,int param_6)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  
  switch((int)(param_4)) {
  case 0: ;
    iVar3 = 1;
    break;
  case 1: ;
    iVar3 = 2;
    break;
  case 2: ;
    iVar3 = 3;
    break;
  case 3: ;
    iVar3 = 4;
    break;
  case 4: ;
    iVar3 = 5;
    break;
  case 5: ;
    iVar3 = 6;
    break;
  case 6: ;
    iVar3 = 7;
    break;
  case 7: ;
    iVar3 = 8;
    break;
  case 8: ;
    iVar3 = 0xf;
    break;
  case 9: ;
    iVar3 = 0xd;
    break;
  case 10: ;
    iVar3 = 0x11;
    break;
  case 0xb: ;
    iVar3 = 0x13;
    break;
default: ;
    iVar3 = -1;
  }
  if (*(int *)(&DAT_00ea42f8 + (DAT_00eaac28 * 0x1276 + iVar3) * 4) == 0) {
    iVar1 = FUN_004963b0(iVar3,param_1,param_2,param_3,param_5,param_4);
    if (0 < iVar1) {
      uVar2 = (FUN_00496640(param_1), restore_unresolved_return(0x00496640u));
      FUN_00496660(iVar3,iVar1,param_1,param_2,param_3,uVar2);
      param_6 = param_6 + param_1 * 0x61;
      *(int *)(&DAT_00bcd6b4 + param_6 * 4) = *(int *)(&DAT_00bcd6b4 + param_6 * 4) + iVar1 * 100;
      return 1;
    }
    return 0;
  }
  return 0;
}
