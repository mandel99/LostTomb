#include "restore_types.h"
extern void FUN_005602b0();
extern void FUN_00561363();
extern void FUN_00561a2b();

int FUN_00560210(uint param_1)

{
  int in_ECX;
  
  if ((param_1 & 2) == 0) {
    FUN_005602b0();
    if ((param_1 & 1) != 0) {
      FUN_00561a2b(in_ECX);
    }
  }
  else {
    FUN_00561363(in_ECX,8,*(undefined4 *)(in_ECX + -4),FUN_005602b0);
    if ((param_1 & 1) != 0) {
      FUN_00561a2b(in_ECX + -4);
    }
    in_ECX = in_ECX + -4;
  }
  return in_ECX;
}
