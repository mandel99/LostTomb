#include "restore_types.h"
#define DAT_005d2c48 (*(undefined4 *)0x005d2c48u)
#define DAT_005d2c4c (*(undefined4 *)0x005d2c4cu)
#define DAT_00e950dc (*(undefined *)0x00e950dcu)
extern uint FUN_004496d0();
extern void FUN_0044c490();
extern bool FUN_0044d680();
extern void FUN_0044e6f0();
extern int FUN_0044f660();
extern undefined4 FUN_0044fd30();
extern void FUN_004e0cf0();

void FUN_0044da30(void)

{
  char cVar1;
  undefined4 uVar2;
  int in_ECX;
  int iVar3;
  int iVar4;
  undefined2 local_20;
  undefined2 local_1e;
  undefined2 local_10;
  
  cVar1 = FUN_0044d680(0);
  if (cVar1 != '\0') {
    iVar4 = (int)*(short *)(in_ECX + 4);
    iVar3 = (int)*(short *)(in_ECX + 0xc);
    *(uint *)(in_ECX + 0x28) = *(uint *)(in_ECX + 0x28) | 4;
    if (0 < *(int *)(&DAT_005d2c4c + iVar4 * 8)) {
      if (iVar3 < *(int *)(&DAT_005d2c4c + iVar4 * 8)) {
        *(int *)(&DAT_005d2c48 + iVar4 * 8) = *(int *)(&DAT_005d2c48 + iVar4 * 8) - iVar3;
        *(int *)(&DAT_005d2c4c + iVar4 * 8) = *(int *)(&DAT_005d2c4c + iVar4 * 8) - iVar3;
      }
      else {
        *(undefined4 *)(&DAT_005d2c48 + iVar4 * 8) = 2;
        *(undefined4 *)(&DAT_005d2c4c + iVar4 * 8) = 0;
      }
      local_1e = 0xcb;
      local_20 = 0xcc;
      local_10 = FUN_0044f660();
      FUN_0044e6f0(&local_20);
      DAT_00e950dc = 1;
      FUN_004e0cf0(0x116,iVar3,iVar4,&local_20,0);
      cVar1 = FUN_004496d0((int)*(short *)(in_ECX + 0x24));
      if (cVar1 != '\0') {
        uVar2 = FUN_0044fd30(0);
        FUN_0044c490((int)*(short *)(in_ECX + 0x24),uVar2);
      }
    }
  }
  return;
}
