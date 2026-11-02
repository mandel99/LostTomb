#include "restore_types.h"
extern void FUN_00448a60();

void FUN_004489c0(void)

{
  undefined2 *in_ECX;
  
  *in_ECX = 0xffff;
  *(undefined1 *)(in_ECX + 1) = 0;
  *(undefined1 *)((int)in_ECX + 3) = 0;
  in_ECX[0x12] = 0xffff;
  in_ECX[0x13] = 0xffff;
  in_ECX[0x29] = 0xffff;
  in_ECX[0x2a] = 0xffff;
  *(undefined4 *)(in_ECX + 0x14) = 2;
  in_ECX[0xc] = 1;
  in_ECX[0xd] = 2;
  in_ECX[0xb] = 0xffff;
  in_ECX[8] = 5;
  in_ECX[9] = 10;
  in_ECX[7] = 0xffff;
  in_ECX[0x2e] = 0;
  in_ECX[0x2f] = 0x31;
  in_ECX[0x2d] = 0xffff;
  in_ECX[3] = 1;
  in_ECX[4] = 0xffff;
  in_ECX[5] = 0xffff;
  in_ECX[0x10] = 1;
  in_ECX[0x11] = 8;
  in_ECX[0xf] = 0xffff;
  in_ECX[0x16] = 2;
  in_ECX[0x17] = 0;
  in_ECX[0x18] = 0;
  *(undefined1 *)(in_ECX + 0x19) = 0;
  *(undefined1 *)((int)in_ECX + 0x33) = 0;
  in_ECX[0x1a] = 0xffff;
  *(undefined1 *)(in_ECX + 0x1b) = 0;
  in_ECX[0x1c] = 0;
  FUN_00448a60();
  return;
}
