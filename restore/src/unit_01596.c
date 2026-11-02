#include "restore_types.h"
#define DAT_007999db (*(undefined4 *)0x007999dbu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799af0 (*(undefined4 *)0x00799af0u)
#define DAT_00e950e0 (*(undefined *)0x00e950e0u)
extern void FUN_0045b700();
extern int FUN_0045d510();
extern void FUN_004695d0();
extern void FUN_0046d100();
extern void FUN_00516f90();
extern void FUN_005173a0();
extern void FUN_0053b090();

void FUN_004c4020(int param_1)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  short *local_8;
  int local_4;
  
  iVar6 = 0;
  local_4 = 0;
  iVar3 = 1;
  local_8 = &DAT_00799af0;
  do {
    if ((char)local_8[-8] == '\x01') {
      iVar4 = 0;
      if ((*local_8 == 0x48) || (*local_8 == 0xc1)) {
        iVar5 = 1;
        do {
          iVar1 = FUN_0045d510(iVar3,iVar5);
          iVar4 = iVar4 + iVar1;
          iVar5 = iVar5 + 1;
        } while (iVar5 < 0x24);
        if (local_4 <= iVar4) {
          iVar6 = iVar3;
          local_4 = iVar4;
        }
      }
    }
    local_8 = local_8 + 0x84;
    iVar3 = iVar3 + 1;
  } while ((int)local_8 < 0x89b6e8);
  if (iVar6 != 0) {
    if (param_1 == 1) {
      DAT_00e950e0 = 0;
      FUN_0046d100(iVar6,0);
      FUN_004695d0(iVar6,1);
      FUN_0053b090(0x11);
      iVar6 = iVar6 * 0x108;
      uVar2 = (uint)(byte)(&DAT_007999db)[iVar6];
      iVar3 = (int)*(short *)((int)&DAT_007999dc + iVar6 + 2);
      FUN_00516f90(iVar3 - uVar2,(int)*(short *)(&DAT_007999e0 + iVar6) - uVar2,iVar3 + uVar2,
                   uVar2 + (int)*(short *)(&DAT_007999e0 + iVar6),1);
      iVar3 = (int)*(short *)((int)&DAT_007999dc + iVar6 + 2);
      uVar2 = (uint)(byte)(&DAT_007999db)[iVar6];
      FUN_005173a0(iVar3 - uVar2,(int)*(short *)(&DAT_007999e0 + iVar6) - uVar2,iVar3 + uVar2,
                   uVar2 + (int)*(short *)(&DAT_007999e0 + iVar6));
      return;
    }
    if (((&DAT_007999e8)[iVar6 * 0x84] == 0x48) || ((&DAT_007999e8)[iVar6 * 0x84] == 0xc1)) {
      FUN_0045b700(iVar6,0x10);
    }
  }
  return;
}
