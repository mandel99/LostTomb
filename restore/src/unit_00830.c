#include "restore_types.h"
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_007999d8 (*(undefined4 *)0x007999d8u)
#define DAT_007999db (*(undefined4 *)0x007999dbu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999e4 (*(undefined4 *)0x007999e4u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799af6 (*(undefined4 *)0x00799af6u)
#define DAT_00bc9694 (*(undefined *)0x00bc9694u)
#define DAT_00e950dc (*(undefined *)0x00e950dcu)
extern void FUN_0043fd00();
extern void FUN_004695d0();
extern int FUN_00475b50();
extern void FUN_0047c2b0();
extern void FUN_0048c0e0();
extern void FUN_004c03d0();
extern void FUN_004e0cf0();
extern void FUN_00516f90();
extern void FUN_005173a0();

void FUN_00458270(void)

{
  int iVar1;
  uint uVar2;
  short *psVar3;
  short sVar4;
  int iVar5;
  
  iVar1 = 1;
  sVar4 = 0;
  iVar5 = 0;
  if (0 < DAT_0078d790) {
    psVar3 = &DAT_00799af6;
    do {
      if (((char)psVar3[-0xb] == '\x01') || ((char)psVar3[-0xb] == '\x03')) {
        if (sVar4 < *psVar3) {
          iVar5 = iVar1;
          sVar4 = *psVar3;
        }
      }
      iVar1 = iVar1 + 1;
      psVar3 = psVar3 + 0x84;
    } while (iVar1 <= DAT_0078d790);
    if (iVar5 != 0) {
      iVar5 = FUN_00475b50(iVar5);
      DAT_00e950dc = 1;
      iVar1 = iVar5 * 0x108;
      FUN_004e0cf0(0x117,0,*(undefined4 *)(&DAT_007999e4 + iVar1),0,0);
      (&DAT_007999d8)[iVar1] = 4;
      DAT_00bc9694 = 0;
      if ((&DAT_007999e8)[iVar5 * 0x84] == 0xca) {
        FUN_004c03d0(iVar5);
      }
      else {
        FUN_0047c2b0(iVar5,(int)*(short *)((int)&DAT_007999dc + iVar1 + 2),
                     (int)*(short *)(&DAT_007999e0 + iVar1),(&DAT_007999db)[iVar1],
                     (&DAT_007999db)[iVar1]);
      }
      sVar4 = (&DAT_007999e8)[iVar5 * 0x84];
      if ((((sVar4 == 0x1f) || (sVar4 == 0x1e)) || (sVar4 == 0x21)) || (sVar4 == 0xd1)) {
        FUN_0043fd00(iVar5,(int)sVar4);
      }
      FUN_0048c0e0((int)*(short *)((int)&DAT_007999dc + iVar1 + 2),
                   (int)*(short *)(&DAT_007999e0 + iVar1),(&DAT_007999db)[iVar1],1);
      FUN_004695d0(iVar5,0);
      iVar5 = (int)*(short *)((int)&DAT_007999dc + iVar1 + 2);
      uVar2 = (uint)(byte)(&DAT_007999db)[iVar1];
      FUN_00516f90(iVar5 - uVar2,(int)*(short *)(&DAT_007999e0 + iVar1) - uVar2,iVar5 + uVar2,
                   (int)*(short *)(&DAT_007999e0 + iVar1) + uVar2,1);
      iVar5 = (int)*(short *)((int)&DAT_007999dc + iVar1 + 2);
      uVar2 = (uint)(byte)(&DAT_007999db)[iVar1];
      FUN_005173a0(iVar5 - uVar2,(int)*(short *)(&DAT_007999e0 + iVar1) - uVar2,iVar5 + uVar2,
                   (int)*(short *)(&DAT_007999e0 + iVar1) + uVar2);
    }
  }
  return;
}
