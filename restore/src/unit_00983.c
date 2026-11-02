#include "restore_types.h"
#define DAT_005d034c (*(undefined4 *)0x005d034cu)
#define DAT_005d03ec (*(undefined4 *)0x005d03ecu)
#define DAT_005d043c (*(undefined4 *)0x005d043cu)
#define DAT_007999db (*(undefined4 *)0x007999dbu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999e4 (*(undefined4 *)0x007999e4u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00acf85c (*(undefined4 *)0x00acf85cu)
#define DAT_00f55bb4 (*(undefined4 *)0x00f55bb4u)
extern void FUN_0047bf50();

void FUN_0046cea0(int param_1,short param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  iVar2 = param_1 * 0x108;
  (&DAT_007999e8)[param_1 * 0x84] = param_2;
  (&DAT_007999ea)[param_1 * 0x84] = param_2 + -10;
  iVar3 = (short)(param_2 + -10) * 4;
  iVar1 = (&DAT_00f55bb4)[*(int *)(&DAT_005d03ec + iVar3) * 2];
  if (*(char *)(&DAT_007999dc + param_1 * 0x42) == '\0') {
    iVar4 = iVar1 + *(int *)(&DAT_005d043c + iVar3);
    if (*(int *)(&DAT_005d034c + iVar3) != 0) {
      iVar4 = iVar4 + ((uint)(byte)(&DAT_00acf85c)[*(int *)(&DAT_007999e4 + iVar2)] &
                      *(int *)(&DAT_005d034c + iVar3) - 1U);
    }
  }
  else {
    iVar4 = iVar1 + 4;
    if (*(int *)(&DAT_005d043c + iVar3) != 0) {
      iVar4 = iVar1 + 5;
    }
  }
  FUN_0047bf50(param_1,(int)*(short *)((int)&DAT_007999dc + iVar2 + 2),
               (int)*(short *)(&DAT_007999e0 + iVar2),(&DAT_007999db)[iVar2],(&DAT_007999db)[iVar2],
               iVar4,8);
  return;
}
