#include "restore_types.h"
#define DAT_005df778 (*(undefined4 *)0x005df778u)
#define DAT_005df78c (*(undefined4 *)0x005df78cu)
#define DAT_005df814 (*(undefined4 *)0x005df814u)
#define DAT_005df818 (*(undefined4 *)0x005df818u)
#define DAT_005df81c (*(undefined4 *)0x005df81cu)
#define DAT_0070e120 (*(undefined *)0x0070e120u)
#define DAT_007999d8 (*(undefined4 *)0x007999d8u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_00799a82 (*(undefined4 *)0x00799a82u)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
#define DAT_00f55bb4 (*(undefined4 *)0x00f55bb4u)
#define DAT_00f56afc (*(undefined *)0x00f56afcu)
extern int FUN_004c53d0();
extern void FUN_004f0ba0();
extern int FUN_0053e590();
extern undefined4 FUN_0053f250();

void FUN_004c0160(int param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int local_20;
  int local_1c;
  int local_18;
  int local_14;
  int local_10;
  undefined *local_c;
  int local_8;
  int local_4;
  
  iVar2 = param_1;
  iVar8 = param_1 * 0x108;
  iVar5 = 0x40;
  iVar7 = (int)*(short *)((int)&DAT_007999dc + iVar8 + 2);
  local_14 = -0x5a;
  iVar6 = (uint)*(byte *)(&DAT_00799a82 + param_1 * 0x84) + DAT_00bca2e0;
  local_18 = (int)*(short *)(&DAT_007999e0 + iVar8);
  local_10 = iVar6;
  local_8 = iVar7;
  iVar3 = FUN_004c53d0(2);
  if ((iVar3 == -1) && (iVar3 = FUN_004c53d0(1), iVar3 == -1)) {
    iVar3 = 0;
  }
  local_4 = (&DAT_005df778)[iVar3];
  if (7 < iVar6) {
    iVar6 = iVar6 + -8;
    local_10 = iVar6;
  }
  local_1c = 2;
  local_c = &DAT_005df78c + iVar6 * 0x10;
  do {
    iVar3 = *(int *)(local_c + local_1c * 4);
    switch((int)(iVar6)) {
    case 0: ;
    case 3: ;
    case 4: ;
    case 7: ;
      iVar5 = -1;
      local_14 = -0x41;
      iVar6 = 1;
      break;
    case 1: ;
    case 2: ;
    case 5: ;
    case 6: ;
      iVar5 = 0xc;
      local_14 = -0x40;
default: ;
      iVar6 = 0;
    }
    if (*(char *)(&DAT_00799a82 + iVar2 * 0x84) == '\0') {
      iVar1 = *(int *)(&DAT_005df81c + iVar3 * 0x10);
      iVar4 = *(int *)(&DAT_005df818 + iVar3 * 0x10);
LAB_004c027d: ;
      iVar7 = iVar1 + iVar7;
      iVar4 = iVar4 + local_18;
    }
    else {
      iVar4 = local_18;
      if (*(char *)(&DAT_00799a82 + iVar2 * 0x84) == '\x01') {
        iVar1 = *(int *)(&DAT_005df818 + iVar3 * 0x10);
        iVar4 = *(int *)(&DAT_005df81c + iVar3 * 0x10);
        goto LAB_004c027d;
      }
    }
    if (local_1c == 2) {
      iVar6 = DAT_00f56afc + iVar6;
      FUN_004f0ba0(iVar7,iVar4,&local_20,&param_1);
      iVar3 = local_14;
      if ((&DAT_007999d8)[iVar8] == '\x06') {
        DAT_0070e120 = 0xf863;
      }
      FUN_0053e590(iVar6,local_20 + iVar5,param_1 + local_14);
      iVar7 = local_20 + iVar5;
      iVar3 = param_1 + iVar3;
    }
    else {
      if (DAT_00bca2e0 != 0) {
        if (DAT_00bca2e0 == 2) {
          iVar7 = iVar7 + -1 + (&DAT_005df814)[iVar3 * 4];
        }
        else {
          if (DAT_00bca2e0 == 4) {
            iVar3 = (&DAT_005df814)[iVar3 * 4];
            iVar7 = iVar7 + -1 + iVar3;
          }
          else {
            iVar3 = (&DAT_005df814)[iVar3 * 4];
          }
          iVar4 = iVar4 + -1 + iVar3;
        }
      }
      iVar6 = (&DAT_00f55bb4)[local_4 * 2] + iVar6;
      FUN_004f0ba0(iVar7,iVar4,&local_20,&param_1);
      if ((&DAT_007999d8)[iVar8] == '\x06') {
        DAT_0070e120 = 0xf863;
      }
      FUN_0053e590(iVar6,local_20,param_1);
      iVar7 = local_20;
      iVar3 = param_1;
    }
    FUN_0053f250(iVar6,iVar7,iVar3);
    DAT_0070e120 = 0;
    local_1c = local_1c + 1;
    iVar6 = local_10;
    iVar7 = local_8;
    if (3 < local_1c) {
      return;
    }
  } while( true );
}
