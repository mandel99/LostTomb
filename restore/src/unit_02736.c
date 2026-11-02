#include "restore_types.h"
#define PTR_FUN_00570534 (*(unsigned char * *)0x00570534u)

void FUN_005605fa(void)

{
  undefined4 *in_ECX;
  
  *in_ECX = &PTR_FUN_00570534;
  in_ECX[1] = 0;
  in_ECX[2] = 4;
  in_ECX[3] = 0;
  in_ECX[4] = 0;
  in_ECX[8] = 0;
  in_ECX[9] = 0;
  in_ECX[10] = 6;
  *(undefined1 *)(in_ECX + 0xb) = 0x20;
  in_ECX[0xc] = 0;
  in_ECX[7] = 0;
  return;
}
