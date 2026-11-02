#include "restore_types.h"
#define DAT_00e950dc (*(undefined *)0x00e950dcu)
extern uint FUN_004496d0();
extern void FUN_0044c490();
extern bool FUN_0044d680();
extern void FUN_0044e6f0();
extern int FUN_0044f660();
extern undefined4 FUN_0044fd30();
extern void FUN_004e0cf0();

void FUN_0044e5d0(void)

{
  char cVar1;
  int in_ECX;
  undefined4 uVar2;
  undefined2 local_20;
  undefined2 local_1e;
  undefined2 local_10;
  
  cVar1 = FUN_0044d680(0);
  if (cVar1 != '\0') {
    *(uint *)(in_ECX + 0x28) = *(uint *)(in_ECX + 0x28) | 4;
    DAT_00e950dc = 1;
    uVar2 = 0x83;
    switch(*(undefined1 *)(in_ECX + 0x60)) {
    case 0: ;
      local_1e = 0x92;
      local_20 = 0x93;
      local_10 = FUN_0044f660();
      uVar2 = 0x106;
      break;
    case 1: ;
      local_1e = 0x96;
      local_20 = 0x97;
      local_10 = FUN_0044f660();
      uVar2 = 0x107;
      break;
    case 2: ;
      local_1e = 0x9a;
      local_20 = 0x9b;
      local_10 = FUN_0044f660();
      break;
    case 3: ;
      local_1e = 0x9e;
      local_20 = 0x9f;
      local_10 = FUN_0044f660();
      break;
default: ;
      local_10 = 0xffff;
      local_20 = 0xffff;
      local_1e = 0xffff;
    }
    FUN_0044e6f0(&local_20);
    FUN_004e0cf0(uVar2,0,0,&local_20,0);
    cVar1 = FUN_004496d0((int)*(short *)(in_ECX + 0x24));
    if (cVar1 != '\0') {
      uVar2 = FUN_0044fd30(0);
      FUN_0044c490((int)*(short *)(in_ECX + 0x24),uVar2);
    }
  }
  return;
}
