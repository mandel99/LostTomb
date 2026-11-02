#include "restore_types.h"
#define PTR_LAB_005701a0 (*(unsigned char * *)0x005701a0u)
extern void FUN_004e5210();

void FUN_00545f80(void)

{
  undefined4 *in_ECX;
  
  FUN_004e5210();
  *in_ECX = &PTR_LAB_005701a0;
  return;
}
