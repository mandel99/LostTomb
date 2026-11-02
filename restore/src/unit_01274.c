#include "restore_types.h"
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_00799a0d (*(undefined4 *)0x00799a0du)
#define DAT_00799a82 (*(undefined4 *)0x00799a82u)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
#define DAT_00bca32c (*(char *)0x00bca32cu)
#define DAT_00bca32d (*(undefined1 *)0x00bca32du)
extern uint32_t FUN_0047a1c0(int32_t);
extern void FUN_0047cf00();
extern int FUN_00487d30();
extern undefined4 FUN_00487e30();

void FUN_00487ec0(int param_1)

{
  undefined1 uVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  
  DAT_00bca32c = FUN_00487e30(param_1);
  iVar3 = param_1 * 0x108;
  DAT_00bca32d = (&DAT_00799a0d)[iVar3] != '\0';
  iVar4 = (uint)*(byte *)(&DAT_00799a82 + param_1 * 0x84) - DAT_00bca2e0 / 2;
  if (iVar4 < 0) {
    iVar4 = iVar4 + 4;
  }
  uVar1 = FUN_0047a1c0((int)*(short *)((int)&DAT_007999dc + iVar3 + 2));
  uVar2 = FUN_00487d30(iVar4,uVar1);
  FUN_0047cf00(param_1,(int)*(short *)((int)&DAT_007999dc + iVar3 + 2),
               (int)*(short *)(&DAT_007999e0 + iVar3),uVar2);
  return;
}
