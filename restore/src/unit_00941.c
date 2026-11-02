#include "restore_types.h"
#define DAT_00799a12 (*(undefined4 *)0x00799a12u)
#define DAT_00799a20 (*(undefined4 *)0x00799a20u)
#define DAT_00799a22 (*(undefined4 *)0x00799a22u)
extern void FUN_0046d100();
extern int FUN_00475b50();

void FUN_00468b20(undefined4 param_1)

{
  int iVar1;
  int iVar2;
  
  iVar1 = FUN_00475b50(param_1);
  while( true ) {
    iVar2 = iVar1 * 0x108;
    *(undefined2 *)(&DAT_00799a22 + iVar2) = 1;
    *(undefined2 *)(&DAT_00799a20 + iVar2) = 1000;
    FUN_0046d100(iVar1,0);
    if (*(short *)(&DAT_00799a12 + iVar2) < 1) break;
    iVar1 = (int)*(short *)(&DAT_00799a12 + iVar2);
  }
  return;
}
