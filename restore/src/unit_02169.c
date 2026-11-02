#include "restore_types.h"
#define DAT_005d19ce (*(undefined4 *)0x005d19ceu)
#define DAT_005d19d0 (*(undefined4 *)0x005d19d0u)
#define DAT_005d19d2 (*(undefined4 *)0x005d19d2u)
extern int FUN_004f5770();
extern undefined4 FUN_004f5bd0();

undefined4 FUN_004f56d0(int param_1)

{
  char cVar1;
  int iVar2;
  int iVar3;
  undefined4 in_stack_00000070;
  undefined4 in_stack_00000074;
  undefined4 in_stack_00000078;
  
  param_1 = param_1 * 0x10;
  iVar3 = 0;
  iVar2 = 0;
  do {
    if (iVar2 == 0) {
      iVar3 = (int)*(short *)(&DAT_005d19ce + param_1);
    }
    else if (iVar2 == 1) {
      iVar3 = (int)*(short *)(&DAT_005d19d0 + param_1);
    }
    else if (iVar2 == 2) {
      iVar3 = (int)*(short *)(&DAT_005d19d2 + param_1);
    }
    if (((iVar3 != 0) && (iVar3 != 0x14)) &&
       (cVar1 = FUN_004f5bd0(in_stack_00000078,iVar3), cVar1 != '\0')) {
      if (iVar3 == 0x19) {
        cVar1 = FUN_004f5770(0x19,in_stack_00000074);
      }
      else {
        cVar1 = FUN_004f5770(iVar3,in_stack_00000070);
      }
      if (cVar1 != '\0') {
        return 1;
      }
    }
    iVar2 = iVar2 + 1;
    if (2 < iVar2) {
      return 0;
    }
  } while( true );
}
