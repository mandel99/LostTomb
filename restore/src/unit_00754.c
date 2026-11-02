#include "restore_types.h"
#define DAT_00e950dc (*(undefined *)0x00e950dcu)
#define DAT_00ea447c (*(undefined4 *)0x00ea447cu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern uint FUN_004496d0();
extern void FUN_0044c490();
extern bool FUN_0044d680();
extern void FUN_0044e6f0();
extern int FUN_0044f660();
extern undefined4 FUN_0044fd30();
extern undefined4 FUN_004c38f0();
extern void FUN_004e0cf0();

void FUN_0044dca0(void)

{
  char cVar1;
  int iVar2;
  undefined4 uVar3;
  int in_ECX;
  int iVar4;
  undefined2 local_20;
  undefined2 local_1e;
  undefined2 local_10;
  
  cVar1 = FUN_0044d680(0);
  if (cVar1 != '\0') {
    *(uint *)(in_ECX + 0x28) = *(uint *)(in_ECX + 0x28) | 4;
    iVar2 = FUN_004c38f0();
    iVar4 = (int)*(short *)(in_ECX + 0xc) + (&DAT_00ea447c)[DAT_00eaac28 * 0x1276];
    if (iVar2 + iVar4 < 0x2e) {
      (&DAT_00ea447c)[DAT_00eaac28 * 0x1276] = iVar4;
      local_1e = 0xe3;
      local_20 = 0xe4;
      local_10 = FUN_0044f660();
      FUN_0044e6f0(&local_20);
      DAT_00e950dc = 1;
      FUN_004e0cf0(0x10b,0,0,&local_20,0);
    }
    cVar1 = FUN_004496d0((int)*(short *)(in_ECX + 0x24));
    if (cVar1 != '\0') {
      uVar3 = FUN_0044fd30(0);
      FUN_0044c490((int)*(short *)(in_ECX + 0x24),uVar3);
    }
  }
  return;
}
