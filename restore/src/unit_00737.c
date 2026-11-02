#include "restore_types.h"

int FUN_0044c960(void)

{
  int in_ECX;
  
  if (-1 < *(short *)(in_ECX + 2)) {
    if (*(short *)(in_ECX + 4) < 0) {
      return 1;
    }
    return (-1 < *(short *)(in_ECX + 6)) + 2;
  }
  if ((-1 < *(short *)(in_ECX + 4)) && (-1 < *(short *)(in_ECX + 6))) {
    return ((int)*(short *)(in_ECX + 6) - (int)*(short *)(in_ECX + 4)) + 1;
  }
  return 0;
}
