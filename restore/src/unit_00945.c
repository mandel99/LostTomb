#include "restore_types.h"
#define DAT_007999d8 (*(undefined4 *)0x007999d8u)
#define DAT_007999db (*(undefined4 *)0x007999dbu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_00799a10 (*(undefined4 *)0x00799a10u)
#define DAT_00799a12 (*(undefined4 *)0x00799a12u)
extern void FUN_0046d100();
extern void FUN_0047c2b0();

void FUN_004695d0(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar3 = 0;
  iVar1 = param_1;
  do {
    iVar1 = (int)*(short *)(&DAT_00799a10 + iVar1 * 0x108);
    if (iVar1 < 1) break;
    if (param_2 == 0) {
      iVar2 = iVar1 * 0x108;
      FUN_0047c2b0(iVar1,(int)*(short *)((int)&DAT_007999dc + iVar2 + 2),
                   (int)*(short *)(&DAT_007999e0 + iVar2),(&DAT_007999db)[iVar2],
                   (&DAT_007999db)[iVar2]);
      (&DAT_007999d8)[iVar2] = 4;
    }
    else {
      FUN_0046d100(iVar1,0);
    }
    iVar3 = iVar3 + 1;
  } while (iVar3 < 0x20d);
  iVar1 = 0;
  do {
    param_1 = (int)*(short *)(&DAT_00799a12 + param_1 * 0x108);
    if (param_1 < 1) {
      return;
    }
    if (param_2 == 0) {
      iVar3 = param_1 * 0x108;
      FUN_0047c2b0(param_1,(int)*(short *)((int)&DAT_007999dc + iVar3 + 2),
                   (int)*(short *)(&DAT_007999e0 + iVar3),(&DAT_007999db)[iVar3],
                   (&DAT_007999db)[iVar3]);
      (&DAT_007999d8)[iVar3] = 4;
    }
    else {
      FUN_0046d100(param_1,0);
    }
    iVar1 = iVar1 + 1;
  } while (iVar1 < 0x20d);
  return;
}
