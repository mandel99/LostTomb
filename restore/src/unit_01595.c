#include "restore_types.h"
#define DAT_00705af0 (*(undefined4 *)0x00705af0u)
#define DAT_0078d64c (*(undefined4 *)0x0078d64cu)
#define DAT_00799af0 (*(undefined4 *)0x00799af0u)
#define DAT_00e950e0 (*(undefined *)0x00e950e0u)
extern void FUN_004695d0();
extern void FUN_0046d100();
extern void FUN_00517370();
extern void FUN_00517660();
extern void FUN_0053b090();
extern uint FUN_00562348();

undefined4 FUN_004c3ed0(void)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  int iVar4;
  int iVar5;
  short *psVar6;
  int local_30 [12];
  
  iVar5 = 0;
  iVar4 = 0;
  local_30[0] = 0xa1;
  local_30[1] = 0xa2;
  local_30[2] = 0x6d;
  local_30[3] = 0x4a;
  local_30[4] = 0x6f;
  local_30[5] = 0x71;
  local_30[6] = 0x15;
  local_30[7] = 0x12;
  local_30[8] = 0xb;
  local_30[9] = 0xffffffff;
  local_30[10] = 0x11;
  local_30[0xb] = 0x13;
  iVar1 = 0;
  do {
    if ((int)(&DAT_00705af0)[*(int *)((int)local_30 + iVar1)] < 1) {
      iVar2 = *(int *)((int)local_30 + iVar1 + 0x18);
      if ((iVar2 < 1) || ((int)(&DAT_0078d64c)[iVar2] < 1)) {
        *(undefined4 *)((int)local_30 + iVar1) = 0;
      }
      else {
        iVar5 = iVar5 + 1;
      }
    }
    else {
      iVar5 = iVar5 + 1;
    }
    iVar1 = iVar1 + 4;
  } while (iVar1 < 0x18);
  if (iVar5 == 0) {
    return 0;
  }
  iVar1 = FUN_00562348();
  iVar2 = 0;
  piVar3 = local_30;
  do {
    if (*piVar3 != 0) {
      if (iVar4 == iVar1 % iVar5) {
        iVar1 = local_30[iVar2];
        if (iVar1 == -1) {
          return 0;
        }
        psVar6 = &DAT_00799af0;
        iVar4 = 1;
        do {
          if (((char)psVar6[-8] == '\x01') && (*psVar6 == iVar1)) {
            FUN_0046d100(iVar4,0);
            FUN_004695d0(iVar4,1);
          }
          psVar6 = psVar6 + 0x84;
          iVar4 = iVar4 + 1;
        } while ((int)psVar6 < 0x89b6e8);
        DAT_00e950e0 = 0;
        FUN_0053b090(0x11);
        FUN_00517370();
        FUN_00517660();
        return 1;
      }
      iVar4 = iVar4 + 1;
    }
    iVar2 = iVar2 + 1;
    piVar3 = piVar3 + 1;
  } while (iVar2 < 6);
  return 0;
}
