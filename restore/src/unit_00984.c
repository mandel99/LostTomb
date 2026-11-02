#include "restore_types.h"
#define DAT_007999db (*(undefined4 *)0x007999dbu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999e4 (*(undefined4 *)0x007999e4u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_007999f4 (*(undefined4 *)0x007999f4u)
#define DAT_007999f8 (*(undefined4 *)0x007999f8u)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00f5653c (*(undefined *)0x00f5653cu)
extern int FUN_0046a400();
extern void FUN_0047bf50();
extern void FUN_0047c480();

void FUN_0046cf40(int param_1)

{
  short sVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  uVar2 = DAT_00f5653c;
  iVar5 = param_1 * 0x108;
  (&DAT_007999e8)[param_1 * 0x84] = 10;
  (&DAT_007999ea)[param_1 * 0x84] = 0;
  if (*(char *)(&DAT_007999dc + param_1 * 0x42) == '\0') {
    (&DAT_00b7482c)[*(int *)(&DAT_007999e4 + iVar5)] = uVar2;
    return;
  }
  FUN_0047c480(param_1,(int)*(short *)((int)&DAT_007999dc + iVar5 + 2),
               (int)*(short *)(&DAT_007999e0 + iVar5));
  *(undefined1 *)(&DAT_007999dc + param_1 * 0x42) = 0;
  (&DAT_007999db)[iVar5] = 1;
  *(undefined1 *)((int)&DAT_007999dc + iVar5 + 1) = 1;
  FUN_0047bf50(param_1,(int)*(short *)((int)&DAT_007999dc + iVar5 + 2),
               (int)*(short *)(&DAT_007999e0 + iVar5),1,1,uVar2,8);
  iVar4 = (int)*(short *)((int)&DAT_007999dc + iVar5 + 2);
  sVar1 = *(short *)(&DAT_007999e0 + iVar5);
  iVar5 = iVar4 + 1;
  iVar3 = FUN_0046a400(DAT_00eaac28,(int)(short)(&DAT_007999e8)[param_1 * 0x84],iVar5,(int)sVar1,0,0
                      );
  (&DAT_007999f4)[iVar3 * 0x84] = 0;
  (&DAT_007999f8)[iVar3 * 0x84] = 0;
  FUN_0047bf50(iVar3,(int)*(short *)((int)&DAT_007999dc + iVar3 * 0x108 + 2),
               (int)*(short *)(&DAT_007999e0 + iVar3 * 0x108),1,1,uVar2,8);
  iVar3 = sVar1 + 1;
  iVar4 = FUN_0046a400(DAT_00eaac28,(int)(short)(&DAT_007999e8)[param_1 * 0x84],iVar4,iVar3,0,0);
  (&DAT_007999f4)[iVar4 * 0x84] = 0;
  (&DAT_007999f8)[iVar4 * 0x84] = 0;
  FUN_0047bf50(iVar4,(int)*(short *)((int)&DAT_007999dc + iVar4 * 0x108 + 2),
               (int)*(short *)(&DAT_007999e0 + iVar4 * 0x108),1,1,uVar2,8);
  iVar5 = FUN_0046a400(DAT_00eaac28,(int)(short)(&DAT_007999e8)[param_1 * 0x84],iVar5,iVar3,0,0);
  (&DAT_007999f4)[iVar5 * 0x84] = 0;
  (&DAT_007999f8)[iVar5 * 0x84] = 0;
  FUN_0047bf50(iVar5,(int)*(short *)((int)&DAT_007999dc + iVar5 * 0x108 + 2),
               (int)*(short *)(&DAT_007999e0 + iVar5 * 0x108),1,1,uVar2,8);
  return;
}
