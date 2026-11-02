#include "restore_types.h"
#define PTR_FUN_00570598 (*(unsigned char * *)0x00570598u)

void FUN_00560f02(void)

{
  undefined4 *in_ECX;
  
  in_ECX[3] = 0xffffffff;
  *in_ECX = &PTR_FUN_00570598;
  in_ECX[1] = 0;
  in_ECX[2] = 0;
  in_ECX[4] = 0;
  in_ECX[5] = 0;
  in_ECX[6] = 0;
  in_ECX[7] = 0;
  in_ECX[8] = 0;
  in_ECX[9] = 0;
  in_ECX[10] = 0;
  in_ECX[0xb] = 0;
  return;
}
