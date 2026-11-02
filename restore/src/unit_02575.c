#include "restore_types.h"
#define PTR_LAB_005701a0 (*(unsigned char * *)0x005701a0u)
extern void FUN_004e5290();

void FUN_00545fa0(void)

{
  undefined4 *in_ECX;
  
  *in_ECX = &PTR_LAB_005701a0;
  FUN_004e5290();
  return;
}
