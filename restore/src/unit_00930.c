#include "restore_types.h"
#define DAT_007999d8 (*(undefined4 *)0x007999d8u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999e4 (*(undefined4 *)0x007999e4u)
#define DAT_00799a0d (*(undefined4 *)0x00799a0du)
#define DAT_00799a18 (*(undefined4 *)0x00799a18u)
#define DAT_00ac2d4c (*(undefined4 *)0x00ac2d4cu)
#define DAT_00f56644 (*(undefined *)0x00f56644u)
extern void FUN_0047bf50();
extern undefined4 FUN_004816b0();

void FUN_00467310(int param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar3 = param_1 * 0x108;
  if ((&DAT_007999d8)[iVar3] == '\x01') {
    iVar1 = *(int *)(&DAT_007999e4 + iVar3);
    iVar2 = FUN_004816b0((int)*(short *)((int)&DAT_007999dc + iVar3 + 2),
                         (int)*(short *)(&DAT_007999e0 + iVar3),2,0x80);
    if (iVar2 == 0) {
      (&DAT_00799a0d)[iVar3] = 0;
    }
    else {
      (&DAT_00799a0d)[iVar3] = 1;
    }
    if (((&DAT_00799a18)[param_1 * 0x84] != 0) && ((&DAT_00799a0d)[iVar3] != '\0')) {
      FUN_0047bf50(param_1,(int)*(short *)((int)&DAT_007999dc + iVar3 + 2),
                   (int)*(short *)(&DAT_007999e0 + iVar3),2,2,
                   (((char)(&DAT_00ac2d4c)[iVar1] < '\x1e') - 1 & 2) + DAT_00f56644,8);
    }
  }
  return;
}
