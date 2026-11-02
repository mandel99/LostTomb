#include "restore_types.h"
#define DAT_00e950dc (*(undefined *)0x00e950dcu)
extern uint FUN_004496d0();
extern undefined2 * FUN_0044c0d0();
extern void FUN_0044c490();
extern bool FUN_0044d680();
extern void FUN_0044e6f0();
extern int FUN_0044f660();
extern undefined4 FUN_0044fd30();
extern void FUN_004e0cf0();
extern void FUN_004e96c0();

void FUN_0044c7e0(void)

{
  short sVar1;
  char cVar2;
  short *psVar3;
  undefined4 uVar4;
  short *in_ECX;
  undefined2 local_20;
  undefined2 local_1e;
  undefined2 local_10;
  
  cVar2 = FUN_0044d680(0);
  if (cVar2 != '\0') {
    psVar3 = (short *)FUN_0044c0d0((int)in_ECX[0x1a]);
    sVar1 = *in_ECX;
    local_1e = 0xbf;
    local_20 = 0xc0;
    local_10 = FUN_0044f660();
    FUN_0044e6f0(&local_20);
    if (psVar3 != (short *)0x0) {
      sVar1 = *psVar3;
    }
    DAT_00e950dc = 1;
    FUN_004e0cf0(0x83,(int)sVar1,0,&local_20,0);
    FUN_004e96c0((int)in_ECX[6]);
    *(undefined1 *)(in_ECX + 0x19) = 0;
    *(uint *)(in_ECX + 0x14) = *(uint *)(in_ECX + 0x14) | 4;
    cVar2 = FUN_004496d0((int)in_ECX[0x12]);
    if (cVar2 != '\0') {
      uVar4 = FUN_0044fd30(0);
      FUN_0044c490((int)in_ECX[0x12],uVar4);
    }
  }
  return;
}
