#include "restore_types.h"
#define PTR_LAB_0057056c (*(unsigned char * *)0x0057056cu)
extern void FUN_005612ed();

void FUN_00560a9b(void)

{
  int in_ECX;
  
  *(undefined ***)(*(int *)(*(int *)(in_ECX + -0x14) + 4) + -0x14 + in_ECX) = &PTR_LAB_0057056c;
  FUN_005612ed();
  return;
}
