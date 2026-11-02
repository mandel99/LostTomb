#include "restore_types.h"
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
extern void FUN_004e5840();
extern void FUN_004e5ae0();

bool FUN_004e5b20(int param_1)

{
  char cVar1;
  byte bVar2;
  int iVar3;
  bool bVar4;
  
  bVar4 = true;
  iVar3 = param_1 * 0x108;
  if (*(char *)((int)&DAT_00799a2c + iVar3 + 1) != 'd') {
    cVar1 = (FUN_004e5ae0(param_1), restore_unresolved_return(0x004e5ae0u));
    bVar2 = *(char *)((int)&DAT_00799a2c + iVar3 + 1) + cVar1;
    *(byte *)((int)&DAT_00799a2c + iVar3 + 1) = bVar2;
    if (100 < bVar2) {
      *(undefined1 *)((int)&DAT_00799a2c + iVar3 + 1) = 100;
    }
    FUN_004e5840(param_1,(int)*(short *)((int)&DAT_007999dc + iVar3 + 2),
                 (int)*(short *)(&DAT_007999e0 + iVar3));
    bVar4 = *(char *)((int)&DAT_00799a2c + iVar3 + 1) == 'd';
  }
  return bVar4;
}
