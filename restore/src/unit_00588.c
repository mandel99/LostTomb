#include "restore_types.h"
#define DAT_0077567c (*(undefined *)0x0077567cu)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799aa8 (*(undefined4 *)0x00799aa8u)
#define DAT_00a9010c (*(undefined4 *)0x00a9010cu)
#define DAT_00a9cc1c (*(undefined4 *)0x00a9cc1cu)
#define DAT_00adc36c (*(undefined4 *)0x00adc36cu)
#define DAT_00ae8e7c (*(undefined4 *)0x00ae8e7cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b285cc (*(undefined4 *)0x00b285ccu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00b67d1c (*(undefined4 *)0x00b67d1cu)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00ea6082 (*(undefined4 *)0x00ea6082u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern void FUN_00475720();
extern void FUN_00475cc0();
extern void FUN_004767d0();
extern void FUN_00477500();
extern void FUN_004776f0();
extern void FUN_004777c0();
extern void FUN_00477890();
extern void FUN_0047a120();
extern void FUN_0047b310();
extern void FUN_00516f90();

void FUN_0043fd00(int param_1,undefined4 param_2)

{
  byte bVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int local_20;
  int local_1c;
  int local_18;
  int local_14;
  
  iVar5 = param_1;
  if ((&DAT_007999e8)[param_1 * 0x84] == 0xd1) {
    *(undefined2 *)((int)&DAT_00ea6082 + DAT_00eaac28 * 0x49d8 + 2) = 0;
  }
  switch((int)(param_2)) {
  case 0x1e: ;
    param_1 = 3;
    break;
  case 0x1f: ;
    param_1 = 2;
    break;
default: ;
    param_1 = 0;
    break;
  case 0x21: ;
    param_1 = 4;
    break;
  case 0xd1: ;
    param_1 = 5;
  }
  iVar5 = (&DAT_00799aa8)[iVar5 * 0x42];
  iVar4 = (iVar5 - DAT_00bca2d8) % 0xe4;
  iVar3 = (iVar5 - DAT_00bca2d8) / 0xe4;
  if (param_1 != 0) {
    local_14 = param_1;
    local_1c = iVar3;
    do {
      local_18 = param_1;
      local_20 = iVar4;
      do {
        (&DAT_00adc36c)[iVar5] = 0;
        (&DAT_00ae8e7c)[iVar5] = 0;
        (&DAT_00b285cc)[iVar5] = (&DAT_00b285cc)[iVar5] & 0xe0;
        bVar1 = (&DAT_00b67d1c)[iVar5];
        *(undefined2 *)(&DAT_00b4e6fc + iVar5 * 2) = 0;
        *(undefined2 *)(&DAT_00a9cc1c + iVar5 * 2) = 0;
        (&DAT_00b67d1c)[iVar5] = bVar1 & 0xc0 | 0x40;
        uVar2 = (&DAT_00af598c)[iVar5];
        (&DAT_00b7482c)[iVar5] = 0;
        (&DAT_00af598c)[iVar5] = uVar2 & 0x3052ec4;
        (&DAT_00a9010c)[iVar5] = (undefined1)param_2;
        if ((uVar2 & 0x40) == 0) {
          if (DAT_0077567c != 0) {
            FUN_0047a120(local_20,local_1c);
          }
        }
        else {
          FUN_0047b310(local_20,local_1c,1);
        }
        local_20 = local_20 + 1;
        iVar5 = iVar5 + 1;
        local_18 = local_18 + -1;
      } while (local_18 != 0);
      local_1c = local_1c + 1;
      iVar5 = iVar5 + (0xe4 - param_1);
      local_14 = local_14 + -1;
    } while (local_14 != 0);
  }
  iVar5 = param_1 + iVar3;
  param_1 = param_1 + iVar4;
  DAT_0077567c = 0;
  FUN_004767d0(iVar4,iVar3,param_1,iVar5,0,0);
  FUN_004767d0(iVar4 + -5,iVar3 + -5,param_1 + 5,iVar5 + 5,1,0);
  FUN_00477500(iVar4,iVar3,param_1,iVar5);
  FUN_004777c0(iVar4,iVar3,param_1,iVar5);
  FUN_00475cc0(iVar4,iVar3,param_1,iVar5,0);
  FUN_004776f0(iVar4,iVar3,param_1,iVar5);
  FUN_00477890(iVar4,iVar3,param_1,iVar5);
  FUN_00475720(iVar4,iVar3,param_1,iVar5,1);
  FUN_00516f90(iVar4,iVar3,param_1,iVar5,1);
  return;
}
