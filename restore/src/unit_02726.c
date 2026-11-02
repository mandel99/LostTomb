#include "restore_types.h"
extern void FUN_00561bdc();

void FUN_005602b0(void)

{
  int *in_ECX;
  
  if ((in_ECX[1] != 0) && (*in_ECX != 0)) {
    FUN_00561bdc(*in_ECX);
    *in_ECX = 0;
  }
  return;
}
