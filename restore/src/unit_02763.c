#include "restore_types.h"
#define PTR_FUN_00570598 (*(unsigned char * *)0x00570598u)
extern undefined4 FUN_00561090();
extern void FUN_00561a2b();

void FUN_00560f4b(void)

{
  undefined4 *in_ECX;
  
  *in_ECX = &PTR_FUN_00570598;
  FUN_00561090();
  if ((in_ECX[1] != 0) && (in_ECX[4] != 0)) {
    FUN_00561a2b(in_ECX[4]);
  }
  return;
}
