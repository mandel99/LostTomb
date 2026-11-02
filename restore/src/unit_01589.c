#include "restore_types.h"
#define DAT_005cfce0 (*(undefined *)0x005cfce0u)
#define DAT_005cfcf4 (*(undefined4 *)0x005cfcf4u)
#define DAT_005cfe10 (*(undefined4 *)0x005cfe10u)
#define DAT_0070e120 (*(undefined *)0x0070e120u)
#define DAT_0070e44c (*(undefined *)0x0070e44cu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a12 (*(undefined4 *)0x00799a12u)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b350dc (*(undefined4 *)0x00b350dcu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
#define DAT_00bca300 (*(undefined *)0x00bca300u)
#define DAT_00bca304 (*(undefined *)0x00bca304u)
#define DAT_00bca308 (*(undefined *)0x00bca308u)
#define DAT_00ea187c (*(undefined4 *)0x00ea187cu)
#define DAT_00ea5ce8 (*(undefined4 *)0x00ea5ce8u)
#define DAT_00ea5cfa (*(undefined4 *)0x00ea5cfau)
#define DAT_00ea5f9c (*(undefined4 *)0x00ea5f9cu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00f56be4 (*(undefined *)0x00f56be4u)
#define DAT_00f56d84 (*(undefined *)0x00f56d84u)
#define DAT_00f56d8c (*(undefined *)0x00f56d8cu)
#define DAT_00f56dac (*(undefined *)0x00f56dacu)
extern int FUN_00475b50();
extern bool FUN_004c52d0();
extern void FUN_004f0ba0();
extern int FUN_0053e590();
extern undefined4 FUN_0053f250();
extern int FUN_0053f4f0();

void FUN_004c2f50(int param_1)

{
  char cVar1;
  int iVar2;
  int *piVar3;
  int iVar4;
  int *piVar5;
  bool bVar6;
  undefined4 uVar7;
  int local_18;
  int local_14;
  int local_10;
  int local_c;
  int local_8;
  int local_4;
  
  local_14 = DAT_005cfce0;
  iVar4 = DAT_00bca2e0 / 2;
  FUN_004f0ba0(DAT_00bca304,DAT_00bca308,&local_c,&local_10);
  local_10 = local_10 + DAT_0070e44c;
  bVar6 = (&DAT_00ea5ce8)[DAT_00eaac28 * 0x24ec] == 0;
  local_4 = DAT_00bca300;
  local_18 = local_c;
  local_8 = local_10;
  iVar2 = FUN_00475b50((int)*(short *)(&DAT_00b4e6fc + DAT_00bca300 * 2));
  if (((short)(&DAT_007999e8)[iVar2 * 0x84] < 0x41) || (0x45 < (short)(&DAT_007999e8)[iVar2 * 0x84])
     ) {
    bVar6 = true;
  }
  if ((short)(&DAT_00ea5cfa)[DAT_00eaac28 * 0x24ec] < 2) {
    bVar6 = true;
  }
  if (param_1 == 0xd3) {
    cVar1 = FUN_004c52d0((&DAT_00ea5f9c)[DAT_00eaac28 * 0x1276],1);
  }
  else {
    cVar1 = FUN_004c52d0((&DAT_00ea5f9c)[DAT_00eaac28 * 0x1276],2);
  }
  if (cVar1 != '\0') {
    bVar6 = true;
  }
  if (*(int *)(&DAT_00ea187c + DAT_00eaac28 * 0x49d8) < -4999) {
    bVar6 = true;
  }
  else if (!bVar6) {
    local_14 = 0;
    local_18 = 0;
    if (DAT_00bca2e0 == 2) {
      local_14 = 2;
    }
    else if (DAT_00bca2e0 == 4) {
      local_14 = 2;
      local_18 = 2;
    }
    else if (DAT_00bca2e0 == 6) {
      local_18 = 2;
    }
    DAT_0070e120 = 0x1fe3;
    if (param_1 == 0xd3) {
      if (iVar2 == 0) {
        DAT_0070e120 = 0;
        return;
      }
      while( true ) {
        iVar4 = iVar2 * 0x108;
        if ((&DAT_00b7482c)
            [DAT_00bca2d8 + *(short *)((int)&DAT_007999dc + iVar4 + 2) +
             *(short *)(&DAT_007999e0 + iVar4) * 0xe4] == DAT_00f56d8c) {
          FUN_004f0ba0(*(short *)((int)&DAT_007999dc + iVar2 * 0x108 + 2) + local_14,
                       *(short *)(&DAT_007999e0 + iVar2 * 0x108) + local_18,&local_18,&param_1);
          FUN_0053e590(DAT_00f56dac + 2,local_18,param_1);
          iVar2 = DAT_00f56dac + 2;
          goto LAB_004c3400;
        }
        if ((&DAT_00b7482c)
            [DAT_00bca2d8 + *(short *)((int)&DAT_007999dc + iVar4 + 2) +
             *(short *)(&DAT_007999e0 + iVar4) * 0xe4] == DAT_00f56d8c + 3) break;
        iVar2 = (int)*(short *)(&DAT_00799a12 + iVar4);
        if (iVar2 == 0) {
          DAT_0070e120 = *(short *)(&DAT_00799a12 + iVar4);
          return;
        }
      }
      FUN_004f0ba0(*(short *)((int)&DAT_007999dc + iVar2 * 0x108 + 2) + local_14,
                   *(short *)(&DAT_007999e0 + iVar2 * 0x108) + local_18,&local_18,&param_1);
      FUN_0053e590(DAT_00f56dac + 3,local_18,param_1);
      iVar2 = DAT_00f56dac + 3;
    }
    else {
      if (iVar2 == 0) {
        DAT_0070e120 = 0;
        return;
      }
      while( true ) {
        iVar4 = iVar2 * 0x108;
        if ((&DAT_00b7482c)
            [DAT_00bca2d8 + *(short *)((int)&DAT_007999dc + iVar4 + 2) +
             *(short *)(&DAT_007999e0 + iVar4) * 0xe4] == DAT_00f56d84) {
          FUN_004f0ba0(*(short *)((int)&DAT_007999dc + iVar2 * 0x108 + 2) + local_14,
                       *(short *)(&DAT_007999e0 + iVar2 * 0x108) + local_18,&local_18,&param_1);
          FUN_0053e590(DAT_00f56dac,local_18,param_1);
          iVar2 = DAT_00f56dac;
          goto LAB_004c3400;
        }
        if ((&DAT_00b7482c)
            [DAT_00bca2d8 + *(short *)((int)&DAT_007999dc + iVar4 + 2) +
             *(short *)(&DAT_007999e0 + iVar4) * 0xe4] == DAT_00f56d84 + 3) break;
        iVar2 = (int)*(short *)(&DAT_00799a12 + iVar4);
        if (iVar2 == 0) {
          DAT_0070e120 = *(short *)(&DAT_00799a12 + iVar4);
          return;
        }
      }
      FUN_004f0ba0(*(short *)((int)&DAT_007999dc + iVar2 * 0x108 + 2) + local_14,
                   *(short *)(&DAT_007999e0 + iVar2 * 0x108) + local_18,&local_18,&param_1);
      FUN_0053e590(DAT_00f56dac + 1,local_18,param_1);
      iVar2 = DAT_00f56dac + 1;
    }
LAB_004c3400: ;
    FUN_0053f250(iVar2,local_18,param_1);
    DAT_0070e120 = 0;
    return;
  }
  if (local_14 < 1) {
    DAT_0070e120 = 0;
    return;
  }
  piVar5 = &DAT_005cfcf4;
  piVar3 = (int *)(&DAT_005cfe10 + iVar4 * 0x90);
  param_1 = local_14;
  do {
    if (bVar6) {
      uVar7 = 0xfffff863;
    }
    else if (*(short *)(&DAT_00b350dc + (local_4 + *piVar3) * 2) == 0 &&
             ((&DAT_00af598c)[local_4 + *piVar3] & 0xeefed77f) == 0) {
      uVar7 = 0x1fe3;
    }
    else {
      uVar7 = 0xfffff863;
    }
    FUN_0053f4f0(DAT_00f56be4,piVar5[-1] + local_18,(*piVar5 - DAT_0070e44c) + local_8,uVar7);
    piVar3 = piVar3 + 1;
    piVar5 = piVar5 + 2;
    param_1 = param_1 + -1;
  } while (param_1 != 0);
  DAT_0070e120 = 0;
  return;
}
