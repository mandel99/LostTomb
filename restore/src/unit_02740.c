#include "restore_types.h"
#define PTR_FUN_0057053c (*(unsigned char * *)0x0057053cu)
extern void FUN_00560f02();

void FUN_005606a0(void)

{
  undefined4 *in_ECX;
  
  FUN_00560f02();
  *in_ECX = &PTR_FUN_0057053c;
  in_ECX[0xc] = 1;
  in_ECX[0xd] = 1;
  in_ECX[0xf] = 0;
  in_ECX[0x10] = 0;
  in_ECX[0x11] = 0;
  return;
}
