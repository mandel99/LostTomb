#include "restore_types.h"
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
extern void FUN_0047bf50();

void FUN_004e69c0(int param_1)

{
  uint uVar1;
  int iVar2;
  undefined4 uVar3;
  int *in_ECX;
  int iVar4;
  
  iVar2 = param_1;
  uVar3 = (**(code **)(*in_ECX + 0x1c))(param_1,&param_1);
  iVar4 = iVar2 * 0x108;
  if (in_ECX[(short)(&DAT_007999ea)[iVar2 * 0x84] * 0xd + 0x16] == 1) {
    iVar4 = DAT_00bca2d8 + *(short *)((int)&DAT_007999dc + iVar4 + 2) +
            *(short *)(&DAT_007999e0 + iVar4) * 0xe4;
    uVar1 = (&DAT_00af598c)[iVar4];
    *(short *)(&DAT_00b4e6fc + iVar4 * 2) = (short)iVar2;
    (&DAT_00af598c)[iVar4] = uVar1 | 0x10000000;
    return;
  }
  FUN_0047bf50(iVar2,(int)*(short *)((int)&DAT_007999dc + iVar4 + 2),
               (int)*(short *)(&DAT_007999e0 + iVar4),
               in_ECX[(short)(&DAT_007999ea)[iVar2 * 0x84] * 0xd + 0x15],
               in_ECX[(short)(&DAT_007999ea)[iVar2 * 0x84] * 0xd + 0x15],uVar3,8);
  return;
}
