#include "restore_types.h"
extern void FUN_00448960();
extern void FUN_00448a60();
extern void FUN_0051f780();
extern void FUN_00561441();

void FUN_0044ba20(void)

{
  int iVar1;
  undefined4 *in_ECX;
  undefined4 *puVar2;
  
  puVar2 = in_ECX + 1;
  FUN_00561441(puVar2,0x7c,0x96,FUN_00448960,FUN_0051f780);
  *in_ECX = 0;
  in_ECX[0x122b] = 0xffffffff;
  in_ECX[0x122c] = 0;
  iVar1 = 0;
  do {
    *(short *)puVar2 = (short)iVar1;
    iVar1 = iVar1 + 1;
    puVar2 = puVar2 + 0x1f;
  } while (iVar1 < 0x96);
  FUN_00448a60();
  *(undefined1 *)((int)in_ECX + 0x43d6) = 2;
  *(undefined1 *)(in_ECX + 0x122d) = 0;
  *(undefined1 *)((int)in_ECX + 0x48b5) = 0;
  return;
}
