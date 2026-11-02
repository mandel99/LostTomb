#include "restore_types.h"
#define DAT_00605a3c (*(undefined *)0x00605a3cu)
#define DAT_00e950dc (*(undefined *)0x00e950dcu)
extern uint FUN_004496d0();
extern void FUN_0044c490();
extern bool FUN_0044d680();
extern void FUN_0044e6f0();
extern int FUN_0044f660();
extern undefined4 FUN_0044fd30();
extern void FUN_004bed00();
extern void FUN_004bed40();
extern void FUN_004e0cf0();

void FUN_0044e7b0(int param_1)

{
  char cVar1;
  int in_ECX;
  undefined4 uVar2;
  undefined2 local_20;
  undefined2 local_1e;
  undefined2 local_10;
  
  if (DAT_00605a3c != 1) {
    cVar1 = FUN_0044d680(0);
    if (cVar1 != '\0') {
      *(uint *)(in_ECX + 0x28) = *(uint *)(in_ECX + 0x28) | 4;
      if (param_1 == 0) {
        FUN_004bed00();
        local_1e = 0xf7;
        local_20 = 0xf8;
        local_10 = FUN_0044f660();
        uVar2 = 0x158;
      }
      else {
        FUN_004bed40();
        local_1e = 0xfb;
        local_20 = 0xfc;
        local_10 = FUN_0044f660();
        uVar2 = 0x15d;
      }
      FUN_0044e6f0(&local_20);
      DAT_00e950dc = 1;
      FUN_004e0cf0(uVar2,0,0,&local_20,0);
      cVar1 = FUN_004496d0((int)*(short *)(in_ECX + 0x24));
      if (cVar1 != '\0') {
        uVar2 = FUN_0044fd30(0);
        FUN_0044c490((int)*(short *)(in_ECX + 0x24),uVar2);
      }
    }
  }
  return;
}
