#include "restore_types.h"
#define DAT_00e950dc (*(undefined *)0x00e950dcu)
#define DAT_00ea5df8 (*(undefined4 *)0x00ea5df8u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern uint FUN_004496d0();
extern void FUN_0044c490();
extern bool FUN_0044d680();
extern void FUN_0044e6f0();
extern int FUN_0044f660();
extern undefined4 FUN_0044fd30();
extern void FUN_004e0cf0();
extern void FUN_004e96c0();

void FUN_0044c8a0(void)

{
  char cVar1;
  undefined4 uVar2;
  short *in_ECX;
  undefined2 local_20;
  undefined2 local_1e;
  undefined2 local_10;
  
  cVar1 = FUN_0044d680(0);
  if (cVar1 != '\0') {
    local_1e = 0xc3;
    local_20 = 0xc4;
    local_10 = FUN_0044f660();
    FUN_0044e6f0(&local_20);
    DAT_00e950dc = 1;
    FUN_004e0cf0(0x83,(int)*in_ECX,0,&local_20,0);
    FUN_004e96c0(-(int)in_ECX[6]);
    (&DAT_00ea5df8)[DAT_00eaac28 * 0x1276] = (int)in_ECX[6];
    *(uint *)(in_ECX + 0x14) = *(uint *)(in_ECX + 0x14) | 4;
    *(undefined1 *)(in_ECX + 0x19) = 0;
    cVar1 = FUN_004496d0((int)in_ECX[0x12]);
    if (cVar1 != '\0') {
      uVar2 = FUN_0044fd30(0);
      FUN_0044c490((int)in_ECX[0x12],uVar2);
    }
  }
  return;
}
