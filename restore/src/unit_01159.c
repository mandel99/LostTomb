#include "restore_types.h"
#define DAT_005dc188 (*(undefined4 *)0x005dc188u)
#define DAT_005dc18c (*(undefined4 *)0x005dc18cu)
#define DAT_006059e4 (*(undefined *)0x006059e4u)
#define DAT_00705fe4 (*(undefined *)0x00705fe4u)
#define DAT_00706cb4 (*(undefined *)0x00706cb4u)
#define DAT_009d1b1c (*(undefined4 *)0x009d1b1cu)
#define DAT_009de62c (*(undefined4 *)0x009de62cu)
#define DAT_00ae8e7c (*(undefined4 *)0x00ae8e7cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00bc9708 (*(undefined *)0x00bc9708u)
#define DAT_00bc970c (*(undefined *)0x00bc970cu)
#define DAT_00bc971c (*(undefined *)0x00bc971cu)
#define DAT_00bc9724 (*(undefined *)0x00bc9724u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00d3b4fc (*(undefined *)0x00d3b4fcu)
#define _DAT_00d3b4f8 (*(undefined4 *)0x00d3b4f8u)
extern void FUN_0040f860();
extern undefined4 FUN_0047c880();
extern void FUN_0047ca50();
extern void FUN_0047d360();
extern undefined4 FUN_0047f190();
extern void FUN_00485830();
extern undefined4 FUN_0051b680();
extern undefined4 FUN_0051c930();

undefined4 FUN_0047eda0(int param_1,int param_2,int param_3,int param_4,int *param_5)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  undefined4 uVar7;
  int local_18;
  int *local_14;
  int local_10;
  int local_c;
  int local_8;
  
  iVar4 = (int)param_5;
  iVar3 = param_3;
  iVar2 = DAT_00705fe4;
  DAT_00bc9724 = 0;
  DAT_00bc970c = 0;
  DAT_00bc9708 = 0;
  FUN_0040f860(&DAT_009de62c,&DAT_00af598c,0x32c40);
  FUN_0040f860(&DAT_009d1b1c,&DAT_00ae8e7c,0xcb10);
  FUN_00485830(0);
  if (param_4 < param_2) {
    DAT_00d3b4fc = param_2 - param_4;
  }
  else if (param_2 < param_4) {
    DAT_00d3b4fc = param_4 - param_2;
  }
  else {
    DAT_00d3b4fc = 0;
  }
  if ((int)param_5 < param_3) {
    _DAT_00d3b4f8 = param_3 - (int)param_5;
  }
  else if (param_3 < (int)param_5) {
    _DAT_00d3b4f8 = (int)param_5 - param_3;
  }
  else {
    _DAT_00d3b4f8 = 0;
  }
  param_3 = _DAT_00d3b4f8;
  if (_DAT_00d3b4f8 <= DAT_00d3b4fc) {
    param_3 = DAT_00d3b4fc;
  }
  if ((param_1 == 0) || (DAT_006059e4 != 0)) {
    if (0 < param_3) {
      iVar5 = FUN_0047f190(DAT_00bca2d8 + iVar3 * 0xe4 + -0xe5 + param_2);
      if (iVar5 == 0) {
        iVar5 = FUN_0047c880(param_2 + -1,iVar3 + -1,3,3,0xffff,0,1);
        DAT_00bc970c = (-(uint)(iVar5 != 0) & 2) - 1;
      }
      else {
        DAT_00bc970c = 2;
      }
    }
  }
  else {
    param_3 = 0;
  }
  iVar5 = FUN_0047f190(DAT_00bca2d8 + (int)param_5 * 0xe4 + -0xe5 + param_4);
  if (iVar5 == 0) {
    iVar5 = FUN_0047c880(param_4 + -1,(int)param_5 + -1,3,3,0xffff,0,1);
    DAT_00bc9708 = (-(uint)(iVar5 != 0) & 2) - 1;
  }
  else {
    DAT_00bc9708 = 2;
  }
  if (((DAT_00bc970c != -1) && (DAT_00bc9708 != -1)) &&
     ((DAT_00bc970c != 1 || ((DAT_00bc9708 != 1 || (2 < param_3)))))) {
    if (param_3 == 0) {
      DAT_00bc9724 = DAT_00706cb4;
      return 1;
    }
    iVar5 = FUN_0051b680(param_1,param_2,iVar3,1);
    if (iVar5 != 0) {
      if (DAT_00bc970c != 0) {
        FUN_0047ca50(0,param_2 + -1,iVar3 + -1,3,3);
        FUN_0047d360(3,param_2 + -1,iVar3 + -1,0xffffffff,1,1);
      }
      if (DAT_00bc9708 != 0) {
        FUN_0047ca50(0,param_4 + -1,(int)param_5 + -1,3,3);
        FUN_0047d360(3,param_4 + -1,(int)param_5 + -1,0xffffffff,1,1);
      }
      local_10 = 10000;
      local_c = 0;
      local_8 = 0;
      local_18 = 0;
      local_14 = &DAT_005dc18c;
      do {
        iVar5 = local_14[-1];
        iVar1 = *local_14;
        param_3 = 0;
        param_5 = &DAT_005dc18c;
        do {
          if (param_1 == 0) {
            uVar7 = 2;
          }
          else {
            uVar7 = 1;
          }
          iVar6 = FUN_0051c930(uVar7,iVar5 + param_2,iVar1 + iVar3,param_5[-1] + param_4,
                               *param_5 + iVar4,2);
          if (((iVar6 != 0) && (DAT_00bc971c != 0)) && (DAT_00bc971c < local_10)) {
            local_10 = DAT_00bc971c;
            local_8 = param_3;
            local_c = local_18;
          }
          param_3 = param_3 + 1;
          param_5 = param_5 + 2;
        } while ((int)param_5 < 0x5dc1ac);
        local_14 = local_14 + 2;
        local_18 = local_18 + 1;
      } while ((int)local_14 < 0x5dc1ac);
      if (local_10 != 10000) {
        FUN_0051c930(param_1 != 0,(&DAT_005dc188)[local_c * 2] + param_2,
                     (&DAT_005dc18c)[local_c * 2] + iVar3,(&DAT_005dc188)[local_8 * 2] + param_4,
                     (&DAT_005dc18c)[local_8 * 2] + iVar4,1);
        if (DAT_00bc970c == 1) {
          DAT_00bc9724 = DAT_00bc9724 + DAT_00706cb4;
        }
        if (DAT_00bc9708 == 1) {
          DAT_00bc9724 = DAT_00bc9724 + DAT_00706cb4;
        }
        if (local_10 != 0) {
          DAT_00bc9724 = DAT_00bc9724 + local_10 * iVar2;
        }
        return 1;
      }
    }
  }
  return 0;
}
