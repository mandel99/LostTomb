#include "restore_types.h"
extern int FUN_00560210();

void FUN_0055dc18(void)

{
  int in_ECX;
  
  if (*(int *)(in_ECX + 8) != 0) {
    if (*(int *)(in_ECX + 8) != 0) {
      FUN_00560210(3);
    }
    *(undefined4 *)(in_ECX + 8) = 0;
  }
  return;
}
