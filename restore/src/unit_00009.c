#include "restore_types.h"
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
extern void FUN_0047bf50();

void FUN_004017a0(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  int *in_ECX;
  
  iVar1 = param_1;
  uVar2 = (**(code **)(*in_ECX + 0x1c))(param_1,&param_1);
  FUN_0047bf50(iVar1,(int)*(short *)((int)&DAT_007999dc + iVar1 * 0x108 + 2),
               (int)*(short *)(&DAT_007999e0 + iVar1 * 0x108),
               in_ECX[(short)(&DAT_007999ea)[iVar1 * 0x84] * 0xd + 0x15],
               in_ECX[(short)(&DAT_007999ea)[iVar1 * 0x84] * 0xd + 0x15],uVar2,8);
  return;
}
