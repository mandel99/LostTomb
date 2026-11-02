#include "restore_types.h"
#define DAT_007999db (*(undefined4 *)0x007999dbu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_00bcb9d0 (*(undefined4 *)0x00bcb9d0u)
#define DAT_00bcbad2 (*(undefined4 *)0x00bcbad2u)
#define DAT_00bcd606 (*(undefined4 *)0x00bcd606u)
#define DAT_00e950e0 (*(undefined *)0x00e950e0u)
extern void FUN_004695d0();
extern void FUN_0046d100();
extern void FUN_00516f90();
extern void FUN_005173a0();
extern void FUN_0053b090();

undefined4 FUN_004c41b0(void)

{
  short *psVar1;
  uint uVar2;
  short sVar3;
  int iVar4;
  int iVar5;
  
  sVar3 = 0;
  iVar5 = 0;
  psVar1 = &DAT_00bcbad2;
  do {
    if (((char)psVar1[-0x3d] == '\x01') && ((char)psVar1[-0x1c] != '\0')) {
      if (sVar3 <= *psVar1) {
        iVar5 = (int)psVar1[-0x3a];
        sVar3 = *psVar1;
      }
    }
    psVar1 = psVar1 + 0x48;
  } while ((int)psVar1 < 0xbcd662);
  if (iVar5 == 0) {
    return 0;
  }
  iVar4 = 0;
  DAT_00e950e0 = 0;
  psVar1 = (short *)(&DAT_00bcb9d0 + iVar5 * 0x90);
  do {
    sVar3 = *psVar1;
    if (sVar3 == 0) break;
    psVar1 = psVar1 + 1;
    iVar4 = iVar4 + 1;
    (&DAT_00bcd606)[sVar3 * 0x184] = 2;
  } while (iVar4 < 0x10);
  FUN_0046d100(iVar5,0);
  FUN_004695d0(iVar5,1);
  FUN_0053b090(0x11);
  iVar5 = iVar5 * 0x108;
  uVar2 = (uint)(byte)(&DAT_007999db)[iVar5];
  iVar4 = (int)*(short *)((int)&DAT_007999dc + iVar5 + 2);
  FUN_00516f90(iVar4 - uVar2,(int)*(short *)(&DAT_007999e0 + iVar5) - uVar2,iVar4 + uVar2,
               uVar2 + (int)*(short *)(&DAT_007999e0 + iVar5),1);
  iVar4 = (int)*(short *)((int)&DAT_007999dc + iVar5 + 2);
  uVar2 = (uint)(byte)(&DAT_007999db)[iVar5];
  FUN_005173a0(iVar4 - uVar2,(int)*(short *)(&DAT_007999e0 + iVar5) - uVar2,iVar4 + uVar2,
               uVar2 + (int)*(short *)(&DAT_007999e0 + iVar5));
  return 1;
}
