#include "restore_types.h"
extern undefined4 FUN_0055d990();
extern void FUN_00561a2b();

void FUN_0055db90(void)

{
  int in_ECX;
  
  if (*(int *)(in_ECX + 8) != 0) {
    FUN_00561a2b(*(int *)(in_ECX + 8));
    *(undefined4 *)(in_ECX + 8) = 0;
  }
  if ((*(int *)(in_ECX + 4) != 0) && (-1 < *(int *)(in_ECX + 0x10))) {
    FUN_0055d990(*(int *)(in_ECX + 0x10));
  }
  return;
}
