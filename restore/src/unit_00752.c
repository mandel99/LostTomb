#include "restore_types.h"
#define DAT_00e950dc (*(undefined *)0x00e950dcu)
#define DAT_00ea5cec (*(undefined4 *)0x00ea5cecu)
#define DAT_00ea5cf0 (*(undefined4 *)0x00ea5cf0u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern uint FUN_004496d0();
extern void FUN_0044c490();
extern bool FUN_0044d680();
extern void FUN_0044e6f0();
extern int FUN_0044f660();
extern undefined4 FUN_0044fd30();
extern void FUN_004e0cf0();

void FUN_0044db20(void)

{
  char cVar1;
  undefined4 uVar2;
  int in_ECX;
  undefined2 local_20;
  undefined2 local_1e;
  undefined2 local_10;
  
  cVar1 = FUN_0044d680(0);
  if (cVar1 != '\0') {
    *(uint *)(in_ECX + 0x28) = *(uint *)(in_ECX + 0x28) | 4;
    if (0 < (short)(&DAT_00ea5cec)[DAT_00eaac28 * 0x24ec]) {
      (&DAT_00ea5cf0)[DAT_00eaac28 * 0x24ec] = 0x30;
      DAT_00e950dc = 1;
      local_1e = 0xdb;
      local_20 = 0xdc;
      local_10 = FUN_0044f660();
      FUN_0044e6f0(&local_20);
      FUN_004e0cf0(0x109,0,0,&local_20,0);
    }
    cVar1 = FUN_004496d0((int)*(short *)(in_ECX + 0x24));
    if (cVar1 != '\0') {
      uVar2 = FUN_0044fd30(0);
      FUN_0044c490((int)*(short *)(in_ECX + 0x24),uVar2);
    }
  }
  return;
}
