#include "restore_types.h"
extern void FUN_00561a2b();

void FUN_004f66f0(void)

{
  int in_ECX;
  
  if (*(int *)(in_ECX + 0x14) != 0) {
    FUN_00561a2b(*(int *)(in_ECX + 0x14));
    *(undefined4 *)(in_ECX + 0x14) = 0;
  }
  *(undefined4 *)(in_ECX + 0x10) = 0;
  *(undefined4 *)(in_ECX + 8) = 0;
  *(undefined4 *)(in_ECX + 0xc) = 0;
  *(undefined4 *)(in_ECX + 4) = 1;
  return;
}
