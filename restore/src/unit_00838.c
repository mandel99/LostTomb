#include "restore_types.h"
#define DAT_00799a06 (*(undefined4 *)0x00799a06u)
#define DAT_00799a25 (*(undefined4 *)0x00799a25u)
#define DAT_00bcd62e (*(undefined4 *)0x00bcd62eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd654 (*(undefined4 *)0x00bcd654u)
#define DAT_00bcd65c (*(undefined4 *)0x00bcd65cu)
#define DAT_00ea3fe8 (*(undefined4 *)0x00ea3fe8u)
#define DAT_00ea3fea (*(undefined4 *)0x00ea3feau)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern int FUN_004b4800();

void FUN_00458d00(int param_1,undefined1 param_2)

{
  byte bVar1;
  int iVar2;
  int iVar3;
  
  iVar2 = FUN_004b4800(DAT_00eaac28,1,(int)*(short *)(&DAT_00ea3fe8 + DAT_00eaac28 * 0x49d8),
                       (int)*(short *)(&DAT_00ea3fea + DAT_00eaac28 * 0x49d8),0);
  (&DAT_00799a06)[param_1 * 0x84] = (short)iVar2;
  bVar1 = (&DAT_00799a25)[param_1 * 0x108];
  iVar3 = iVar2 * 0x184;
  (&DAT_00bcd630)[iVar3] = 8;
  (&DAT_00bcd654)[iVar2 * 0xc2] = (short)param_1;
  *(ushort *)(&DAT_00bcd62e + iVar3) = (bVar1 & 0xff7f) + 10;
  (&DAT_00bcd65c)[iVar3] = param_2;
  return;
}
