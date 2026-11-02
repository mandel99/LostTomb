#include "restore_types.h"
#define DAT_0077646c (*(undefined4 *)0x0077646cu)
#define DAT_00e950dc (*(undefined *)0x00e950dcu)
extern bool FUN_00444980();
extern int FUN_004449d0();
extern uint FUN_004496d0();
extern void FUN_0044c490();
extern bool FUN_0044d680();
extern void FUN_0044e6f0();
extern int FUN_0044f660();
extern undefined4 FUN_0044fd30();
extern void FUN_004e0cf0();

void FUN_0044d780(void)

{
  short sVar1;
  int iVar2;
  char cVar3;
  int iVar4;
  undefined4 uVar5;
  int in_ECX;
  int iVar6;
  undefined2 local_20;
  undefined2 local_1e;
  undefined2 local_10;
  
  cVar3 = FUN_0044d680(0);
  if (cVar3 != '\0') {
    sVar1 = *(short *)(in_ECX + 4);
    *(uint *)(in_ECX + 0x28) = *(uint *)(in_ECX + 0x28) | 4;
    iVar6 = (int)*(short *)(in_ECX + 0x1c);
    iVar4 = (int)sVar1 + iVar6 * 0x24;
    iVar2 = *(int *)(&DAT_0077646c + iVar4 * 4);
    if (iVar2 < 0x28) {
      if (iVar2 < 0x19) {
        *(uint *)(&DAT_0077646c + iVar4 * 4) = ((iVar2 < 0xf) - 1 & 10) + 0xf;
      }
      else {
        *(undefined4 *)(&DAT_0077646c + iVar4 * 4) = 0x28;
      }
      iVar4 = FUN_00444980(iVar6);
      if (iVar4 != 0) {
        local_1e = 0xcf;
        local_20 = 0xd0;
        local_10 = FUN_0044f660();
        FUN_0044e6f0(&local_20);
        DAT_00e950dc = 1;
        uVar5 = FUN_004449d0(iVar6,(int)sVar1,&local_20,0);
        FUN_004e0cf0(0x111,uVar5);
      }
    }
    cVar3 = FUN_004496d0((int)*(short *)(in_ECX + 0x24));
    if (cVar3 != '\0') {
      uVar5 = FUN_0044fd30(0);
      FUN_0044c490((int)*(short *)(in_ECX + 0x24),uVar5);
    }
  }
  return;
}
