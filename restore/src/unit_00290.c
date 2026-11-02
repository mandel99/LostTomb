#include "restore_types.h"
#define PTR_FUN_0056f548 (*(unsigned char * *)0x0056f548u)
extern void FUN_00561a2b();

void FUN_0041e090(void)

{
  undefined4 *in_ECX;
  
  *in_ECX = &PTR_FUN_0056f548;
  if (in_ECX[4] != 0) {
    FUN_00561a2b(in_ECX[4]);
  }
  if (in_ECX[1] != 0) {
    FUN_00561a2b(in_ECX[1]);
  }
  return;
}
