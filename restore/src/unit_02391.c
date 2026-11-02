#include "restore_types.h"
#define DAT_0057c048 (*(undefined4 *)0x0057c048u)
#define DAT_00d3b550 (*(undefined *)0x00d3b550u)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00e38dac (*(undefined *)0x00e38dacu)
#define DAT_00ea3efc (*(undefined4 *)0x00ea3efcu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00f53dcc (*(undefined4 *)0x00f53dccu)
#define DAT_00f56904 (*(undefined4 *)0x00f56904u)
#define DAT_00f56f84 (*(undefined *)0x00f56f84u)
#define _DAT_00f53dc8 (*(undefined4 *)0x00f53dc8u)
extern void FUN_004cce90();
extern void FUN_004cd9d0();
extern void FUN_004d6250();
extern void FUN_00527ad0();
extern int FUN_0053e590();

void FUN_005278e0(int param_1,int param_2,int param_3)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  iVar3 = 0;
  piVar1 = (int *)(&DAT_00ea3efc + DAT_00eaac28 * 0x49d8);
  iVar4 = 0x14;
  do {
    if (iVar3 < *piVar1) {
      iVar3 = *piVar1;
    }
    piVar1 = piVar1 + 1;
    iVar4 = iVar4 + -1;
  } while (iVar4 != 0);
  FUN_00527ad0(iVar3);
  if (param_1 != 0) {
    iVar3 = param_2 + -0x42;
    FUN_004cce90(DAT_00f53dcc,0x40,&DAT_0057c048,iVar3,param_3 + -3,0x3c,DAT_00f56f84,0);
    FUN_004cce90(DAT_00f53dcc / 2,0x40,&DAT_0057c048,iVar3,param_3 + 0x60,0x3c,DAT_00f56f84,0);
    FUN_004cce90(0,0x40,&DAT_0057c048,iVar3,param_3 + 0xc4,0x3c,DAT_00f56f84,0);
    FUN_004d6250(0x37,9,param_2 + -0x50,param_3 + 0xd2,200,DAT_00f56f84,0,DAT_00e38da8,DAT_00e38da4)
    ;
    FUN_004d6250(0x37,10,param_2 + 0x118,param_3 + 0xd2,200,DAT_00f56f84,0,DAT_00e38da8,DAT_00e38da4
                );
    DAT_00d3b550 = param_3 + 200;
    iVar4 = 0;
    iVar3 = _DAT_00f53dc8;
    do {
      if (iVar3 == -1) {
        iVar2 = *(int *)(&DAT_00ea3efc + (DAT_00eaac28 * 0x1276 + iVar4) * 4) << 1;
      }
      else {
        iVar2 = *(int *)(&DAT_00ea3efc + (DAT_00eaac28 * 0x1276 + iVar4) * 4) >>
                ((byte)iVar3 & 0x1f);
      }
      if (0 < iVar2) {
        FUN_0053e590(DAT_00f56904,param_2,(param_3 - iVar2) + 200);
        iVar3 = _DAT_00f53dc8;
      }
      iVar4 = iVar4 + 1;
      param_2 = param_2 + 0x14;
    } while (iVar4 < 0x14);
    DAT_00d3b550 = DAT_00e38dac;
    return;
  }
  _DAT_00f53dc8 = _DAT_00f53dc8 + 2;
  iVar3 = 0;
  do {
    iVar4 = *(int *)(&DAT_00ea3efc + (DAT_00eaac28 * 0x1276 + iVar3) * 4) >>
            ((byte)_DAT_00f53dc8 & 0x1f);
    if (0 < iVar4) {
      FUN_004cd9d0(param_2,(param_3 - iVar4) + 0x32,4,iVar4 + 1,0xf800);
    }
    iVar3 = iVar3 + 1;
    param_2 = param_2 + 5;
  } while (iVar3 < 0x14);
  return;
}
