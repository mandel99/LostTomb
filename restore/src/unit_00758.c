#include "restore_types.h"
#define DAT_00e950dc (*(undefined *)0x00e950dcu)
extern uint FUN_004496d0();
extern void FUN_0044c490();
extern bool FUN_0044d680();
extern undefined4 FUN_0044fd30();
extern undefined4 FUN_00469f80();
extern void FUN_004e0cf0();

void FUN_0044df90(void)

{
  char cVar1;
  int iVar2;
  undefined4 uVar3;
  int in_ECX;
  
  cVar1 = FUN_0044d680(0);
  if (cVar1 != '\0') {
    *(uint *)(in_ECX + 0x28) = *(uint *)(in_ECX + 0x28) | 4;
    iVar2 = FUN_00469f80(0x6d);
    if (iVar2 != 0) {
      DAT_00e950dc = 1;
      FUN_004e0cf0(0x10f,0,iVar2,0,0);
    }
    cVar1 = FUN_004496d0((int)*(short *)(in_ECX + 0x24));
    if (cVar1 != '\0') {
      uVar3 = FUN_0044fd30(0);
      FUN_0044c490((int)*(short *)(in_ECX + 0x24),uVar3);
    }
  }
  return;
}
