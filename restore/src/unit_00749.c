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
extern uint FUN_005599c0();

void FUN_0044d870(void)

{
  int iVar1;
  char cVar2;
  int iVar3;
  undefined4 uVar4;
  int in_ECX;
  int iVar5;
  int iVar6;
  undefined2 local_20;
  undefined2 local_1e;
  undefined2 local_10;
  
  cVar2 = FUN_0044d680(0);
  if (cVar2 != '\0') {
    iVar5 = (int)*(short *)(in_ECX + 4);
    *(uint *)(in_ECX + 0x28) = *(uint *)(in_ECX + 0x28) | 4;
    iVar6 = (int)*(short *)(in_ECX + 0x1c);
    iVar3 = iVar5 + iVar6 * 0x24;
    iVar1 = *(int *)(&DAT_0077646c + iVar3 * 4);
    if (0 < iVar1) {
      if (iVar1 < 0x1a) {
        *(uint *)(&DAT_0077646c + iVar3 * 4) = (iVar1 < 0x10) - 1 & 0xf;
      }
      else {
        *(undefined4 *)(&DAT_0077646c + iVar3 * 4) = 0x19;
      }
      FUN_005599c0(iVar5);
      iVar3 = FUN_00444980(iVar6);
      if (iVar3 != 0) {
        local_1e = 0xd3;
        local_20 = 0xd4;
        local_10 = FUN_0044f660();
        FUN_0044e6f0(&local_20);
        DAT_00e950dc = 1;
        uVar4 = FUN_004449d0(iVar6,iVar5,&local_20,0);
        FUN_004e0cf0(0x112,uVar4);
      }
    }
    cVar2 = FUN_004496d0((int)*(short *)(in_ECX + 0x24));
    if (cVar2 != '\0') {
      uVar4 = FUN_0044fd30(0);
      FUN_0044c490((int)*(short *)(in_ECX + 0x24),uVar4);
    }
  }
  return;
}
