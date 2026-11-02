#include "restore_types.h"
#define DAT_0057c048 (*(undefined4 *)0x0057c048u)
#define DAT_00d3b550 (*(undefined *)0x00d3b550u)
#define DAT_00e20778 (*(undefined *)0x00e20778u)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00e38dac (*(undefined *)0x00e38dacu)
#define DAT_00ea1800 (*(undefined4 *)0x00ea1800u)
#define DAT_00ea1804 (*(undefined4 *)0x00ea1804u)
#define DAT_00ea1808 (*(undefined4 *)0x00ea1808u)
#define DAT_00ea180c (*(undefined4 *)0x00ea180cu)
#define DAT_00ea3e30 (*(undefined4 *)0x00ea3e30u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00f53dcc (*(undefined4 *)0x00f53dccu)
#define DAT_00f56904 (*(undefined4 *)0x00f56904u)
#define DAT_00f56f84 (*(undefined *)0x00f56f84u)
#define _DAT_00f53dc8 (*(undefined4 *)0x00f53dc8u)
extern void FUN_004cccb0();
extern void FUN_004cce90();
extern void FUN_004cd510();
extern void FUN_004cd9d0();
extern void FUN_004d62f0();
extern undefined4 FUN_004e9760();
extern int FUN_004e97c0();
extern void FUN_00527ad0();
extern int FUN_0053e590();

void FUN_00527360(int param_1,int param_2,int param_3)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int local_4;
  
  iVar5 = param_3;
  iVar3 = param_2;
  iVar6 = *(int *)(&DAT_00ea3e30 + DAT_00eaac28 * 0x49d8);
  if (iVar6 < 0x15) {
    iVar6 = 0x14;
  }
  else if (iVar6 < 0x29) {
    iVar6 = 0x28;
  }
  else if (iVar6 < 0x65) {
    iVar6 = 100;
  }
  else {
    iVar6 = ((200 < iVar6) - 1 & 0xffffff38) + 400;
  }
  if (param_1 == 0) {
    iVar6 = ((0x28 < iVar6) - 1 & 0xffffffb0) + 100;
  }
  iVar4 = 0;
  iVar2 = 0;
  if (0 < iVar6) {
    do {
      iVar1 = FUN_004e9760(iVar6,iVar4);
      if (iVar2 < iVar1) {
        iVar2 = iVar1;
      }
      iVar4 = iVar4 + 1;
    } while (iVar4 < iVar6);
  }
  FUN_00527ad0(iVar2);
  if (param_1 == 0) {
    iVar5 = 0;
    _DAT_00f53dc8 = _DAT_00f53dc8 + 2;
    if (0 < iVar6) {
      do {
        iVar2 = FUN_004e9760(iVar6,iVar5);
        iVar2 = iVar2 >> ((byte)_DAT_00f53dc8 & 0x1f);
        if (0 < iVar2) {
          if (iVar6 == 0x14) {
            FUN_004cd9d0(iVar3,(param_3 - iVar2) + 0x32,4,iVar2 + 1,0xf800);
          }
          else {
            FUN_004cd510(param_2 + iVar5,(param_3 - iVar2) + 0x32,param_2 + iVar5,param_3 + 0x32,
                         0xf800);
          }
        }
        iVar5 = iVar5 + 1;
        iVar3 = iVar3 + 5;
      } while (iVar5 < iVar6);
    }
    return;
  }
  iVar2 = param_2 + -0x42;
  FUN_004cce90(DAT_00f53dcc,0x40,&DAT_0057c048,iVar2,param_3 + -3,0x3c,DAT_00f56f84,0);
  FUN_004cce90(DAT_00f53dcc / 2,0x40,&DAT_0057c048,iVar2,param_3 + 0x60,0x3c,DAT_00f56f84,0);
  FUN_004cce90(0,0x40,&DAT_0057c048,iVar2,param_3 + 0xc4,0x3c,DAT_00f56f84,0);
  FUN_004e97c0(iVar6);
  iVar2 = param_3 + 0xd2;
  DAT_00e20778 = 0;
  FUN_004d62f0(0x19,DAT_00ea180c,param_2 + -0x14,iVar2,DAT_00f56f84,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004cccb0(0,DAT_00ea1808,DAT_00e20778 + -0x14 + param_2,iVar2,DAT_00f56f84,0);
  DAT_00e20778 = 0;
  FUN_004d62f0(0x19,DAT_00ea1804,param_2 + 0x17c,iVar2,DAT_00f56f84,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004cccb0(0,DAT_00ea1800,DAT_00e20778 + 0x17c + param_2,iVar2,DAT_00f56f84,0);
  DAT_00d3b550 = param_3 + 200;
  iVar2 = 0;
  if (0 < iVar6) {
    local_4 = param_2;
    param_1 = param_2;
    param_3 = param_2;
    do {
      iVar4 = FUN_004e9760(iVar6,iVar2);
      if (_DAT_00f53dc8 == -1) {
        iVar4 = iVar4 << 1;
      }
      else {
        iVar4 = iVar4 >> ((byte)_DAT_00f53dc8 & 0x1f);
      }
      if (0 < iVar4) {
        if (iVar6 == 0x14) {
          FUN_0053e590(DAT_00f56904,param_2,(iVar5 - iVar4) + 200);
        }
        else if (iVar6 == 0x28) {
          FUN_0053e590(DAT_00f56904 + 1,param_3,(iVar5 - iVar4) + 200);
        }
        else if (iVar6 == 100) {
          FUN_0053e590(DAT_00f56904 + 2,param_1,(iVar5 - iVar4) + 200);
        }
        else if (iVar6 == 200) {
          FUN_0053e590(DAT_00f56904 + 3,local_4,(iVar5 - iVar4) + 200);
        }
        else {
          FUN_004cd510(iVar2 + iVar3,(iVar5 - iVar4) + 200,iVar2 + iVar3,iVar5 + 199,0xf800);
        }
      }
      param_2 = param_2 + 0x14;
      iVar2 = iVar2 + 1;
      param_3 = param_3 + 10;
      param_1 = param_1 + 4;
      local_4 = local_4 + 2;
    } while (iVar2 < iVar6);
  }
  DAT_00d3b550 = DAT_00e38dac;
  return;
}
