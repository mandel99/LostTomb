#include "restore_types.h"
#define PTR_FUN_00570534 (*(unsigned char * *)0x00570534u)

void FUN_00560648(void)

{
  undefined4 *in_ECX;
  
  *in_ECX = &PTR_FUN_00570534;
  if ((in_ECX[7] != 0) && ((undefined4 *)in_ECX[1] != (undefined4 *)0x0)) {
    (*(code *)**(undefined4 **)in_ECX[1])(1);
  }
  in_ECX[1] = 0;
  in_ECX[2] = 4;
  return;
}
