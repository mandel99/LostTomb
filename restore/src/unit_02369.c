#include "restore_types.h"
#define DAT_0057c048 (*(undefined4 *)0x0057c048u)
#define DAT_00605988 (*(undefined *)0x00605988u)
#define DAT_007056c8 (*(undefined *)0x007056c8u)
#define DAT_00e20778 (*(undefined *)0x00e20778u)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00e38dda (*(char *)0x00e38ddau)
#define DAT_00ea439c (*(undefined4 *)0x00ea439cu)
#define DAT_00ea43a0 (*(undefined4 *)0x00ea43a0u)
#define DAT_00ea43ac (*(undefined4 *)0x00ea43acu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00f53f08 (*(undefined *)0x00f53f08u)
#define DAT_00f53f0c (*(undefined *)0x00f53f0cu)
#define DAT_00f53f48 (*(undefined *)0x00f53f48u)
#define DAT_00f5670c (*(undefined *)0x00f5670cu)
#define DAT_00f56f9c (*(undefined *)0x00f56f9cu)
#define DAT_00f56fa4 (*(undefined *)0x00f56fa4u)
extern void FUN_004d62f0();
extern void FUN_004d63d0();
extern void FUN_004d65b0();
extern void FUN_004d6860();
extern int FUN_0053e590();

void FUN_005215d0(void)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  if (5 < DAT_00f53f48) {
    FUN_004d65b0(DAT_00f53f0c + 0x20,DAT_00f53f08 + 0x46,0x24,0xf,DAT_00e38da8,DAT_00e38da4);
    iVar3 = 0;
    iVar5 = 0;
    iVar4 = 0;
    do {
      iVar1 = DAT_00605988;
      if (DAT_00e38dda == '\x01') {
        iVar1 = DAT_007056c8 + -1;
      }
      FUN_004d63d0(0,DAT_00f53f0c + 0x28,iVar4 + 0x4d + DAT_00f53f08,0x230,0x16,iVar3 == iVar1,
                   DAT_00e38da8,DAT_00e38da4);
      if (*(int *)(&DAT_00ea43ac + iVar5 + DAT_00eaac28 * 0x49d8) != 0) {
        FUN_0053e590(DAT_00f5670c,DAT_00f53f0c + 0x46,iVar4 + 0x50 + DAT_00f53f08);
        FUN_004d6860(*(undefined4 *)(&DAT_00ea43ac + iVar5 + DAT_00eaac28 * 0x49d8),0x40,
                     &DAT_0057c048,DAT_00f53f0c + 0x5a,iVar4 + 0x52 + DAT_00f53f08,DAT_00f56f9c,0,
                     DAT_00e38da8,DAT_00e38da4);
      }
      iVar3 = iVar3 + 1;
      DAT_00e20778 = 0;
      FUN_004d62f0(0x32,iVar3,DAT_00f53f0c + 0xaa,iVar4 + 0x52 + DAT_00f53f08,DAT_00f56f9c,0,
                   DAT_00e38da8,DAT_00e38da4);
      FUN_004d6860(*(undefined4 *)((int)&DAT_00ea439c + iVar5 + DAT_00eaac28 * 0x49d8),0x40,
                   &DAT_0057c048,DAT_00f53f0c + 0x19a,iVar4 + 0x52 + DAT_00f53f08,DAT_00f56f9c,0,
                   DAT_00e38da8,DAT_00e38da4);
      uVar2 = DAT_00f56f9c;
      if (*(int *)(&DAT_00ea43a0 + iVar5 + DAT_00eaac28 * 0x49d8) !=
          *(int *)((int)&DAT_00ea439c + DAT_00eaac28 * 0x49d8 + iVar5)) {
        uVar2 = DAT_00f56fa4;
      }
      FUN_004d6860(*(int *)(&DAT_00ea43a0 + iVar5 + DAT_00eaac28 * 0x49d8),0x40,&DAT_0057c048,
                   DAT_00f53f0c + 0x1fe,iVar4 + 0x52 + DAT_00f53f08,uVar2,0,DAT_00e38da8,
                   DAT_00e38da4);
      iVar4 = iVar4 + 0x19;
      iVar5 = iVar5 + 0x14;
    } while (iVar4 < 0xe1);
  }
  return;
}
