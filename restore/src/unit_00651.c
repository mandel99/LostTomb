#include "restore_types.h"
extern void FUN_00561a2b();

void FUN_004454a0(void)

{
  int in_ECX;
  
  if (*(int *)(in_ECX + 4) != 0) {
    FUN_00561a2b(*(int *)(in_ECX + 4));
    *(undefined4 *)(in_ECX + 4) = 0;
    *(undefined4 *)(in_ECX + 0x138) = 0;
  }
  return;
}
