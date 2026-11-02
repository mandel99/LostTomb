#include "restore_types.h"
#define DAT_005d1ff8 (*(undefined4 *)0x005d1ff8u)
#define DAT_005d1ffc (*(undefined4 *)0x005d1ffcu)
#define DAT_0070e488 (*(undefined *)0x0070e488u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a10 (*(undefined4 *)0x00799a10u)
#define DAT_00799a20 (*(undefined4 *)0x00799a20u)
#define DAT_00799a82 (*(undefined4 *)0x00799a82u)
#define DAT_00b67d1c (*(undefined4 *)0x00b67d1cu)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
extern void FUN_00436180();
extern int FUN_00488030();
extern void FUN_004f0ba0();

void FUN_00435290(int param_1,undefined4 param_2,undefined4 param_3)

{
  short sVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  undefined4 local_4;
  
  iVar3 = param_1;
  iVar4 = param_1 * 0x108;
  if (*(short *)(&DAT_00799a10 + iVar4) != 0) {
    return;
  }
  cVar2 = FUN_00488030(param_1);
  if (cVar2 != '\0') {
    cVar2 = (&DAT_00b67d1c)[DAT_0070e488];
    if (DAT_00bca2e0 == 0) {
      if (cVar2 != 'P') {
        return;
      }
    }
    else if (DAT_00bca2e0 == 2) {
      if (cVar2 != '@') {
        return;
      }
    }
    else if (DAT_00bca2e0 == 4) {
      if (cVar2 != 'B') {
        return;
      }
    }
    else if ((DAT_00bca2e0 == 6) && (cVar2 != 'R')) {
      return;
    }
  }
  sVar1 = *(short *)(&DAT_00799a20 + iVar4);
  param_1 = param_3;
  local_4 = param_2;
  if ((0x40 < (short)(&DAT_007999e8)[iVar3 * 0x84]) && ((short)(&DAT_007999e8)[iVar3 * 0x84] < 0x46)
     ) {
    iVar3 = (uint)(*(byte *)(&DAT_00799a82 + iVar3 * 0x84) >> 1) * 8;
    FUN_004f0ba0((int)*(short *)((int)&DAT_007999dc + iVar4 + 2) + *(int *)(&DAT_005d1ff8 + iVar3),
                 (int)*(short *)(&DAT_007999e0 + iVar4) + *(int *)(&DAT_005d1ffc + iVar3),&local_4,
                 &param_1);
  }
  FUN_00436180((int)sVar1 / 100,local_4,param_1,1);
  return;
}
