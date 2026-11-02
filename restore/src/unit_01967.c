#include "restore_types.h"
#define PTR_LAB_0056f698 (*(unsigned char * *)0x0056f698u)
extern void FUN_0051f780();
extern void FUN_00561363();

void FUN_004e5290(void)

{
  undefined4 *in_ECX;
  
  *in_ECX = &PTR_LAB_0056f698;
  FUN_00561363(in_ECX + 3,0x34,1,FUN_0051f780);
  return;
}
