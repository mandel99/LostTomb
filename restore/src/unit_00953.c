#include "restore_types.h"
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_007999d8 (*(undefined4 *)0x007999d8u)
#define DAT_007999db (*(undefined4 *)0x007999dbu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999e4 (*(undefined4 *)0x007999e4u)
#define DAT_00799af0 (*(undefined4 *)0x00799af0u)
#define DAT_00bc9694 (*(undefined *)0x00bc9694u)
extern void FUN_0047c2b0();
extern void FUN_00516f90();
extern void FUN_005173a0();
extern void FUN_0053b090();

undefined4 FUN_00469f80(int param_1)

{
  undefined4 uVar1;
  int iVar2;
  uint uVar3;
  short *psVar4;
  int iVar5;
  
  iVar2 = 1;
  if (0 < DAT_0078d790) {
    psVar4 = &DAT_00799af0;
    do {
      if (((char)psVar4[-8] == '\x01') && (*psVar4 == param_1)) {
        DAT_00bc9694 = 0;
        iVar5 = iVar2 * 0x108;
        uVar1 = *(undefined4 *)(&DAT_007999e4 + iVar5);
        (&DAT_007999d8)[iVar5] = 4;
        FUN_0047c2b0(iVar2,(int)*(short *)((int)&DAT_007999dc + iVar5 + 2),
                     (int)*(short *)(&DAT_007999e0 + iVar5),(&DAT_007999db)[iVar5],
                     (&DAT_007999db)[iVar5]);
        FUN_0053b090(0x11);
        iVar2 = (int)*(short *)((int)&DAT_007999dc + iVar5 + 2);
        uVar3 = (uint)(byte)(&DAT_007999db)[iVar5];
        FUN_00516f90(iVar2 - uVar3,(int)*(short *)(&DAT_007999e0 + iVar5) - uVar3,iVar2 + uVar3,
                     (int)*(short *)(&DAT_007999e0 + iVar5) + uVar3,1);
        iVar2 = (int)*(short *)((int)&DAT_007999dc + iVar5 + 2);
        uVar3 = (uint)(byte)(&DAT_007999db)[iVar5];
        FUN_005173a0(iVar2 - uVar3,(int)*(short *)(&DAT_007999e0 + iVar5) - uVar3,iVar2 + uVar3,
                     (int)*(short *)(&DAT_007999e0 + iVar5) + uVar3);
        return uVar1;
      }
      iVar2 = iVar2 + 1;
      psVar4 = psVar4 + 0x84;
    } while (iVar2 <= DAT_0078d790);
  }
  return 0;
}
