#include "restore_types.h"
#define PTR_LAB_005705d8 (*(unsigned char * *)0x005705d8u)
extern void FUN_00560d87();
extern void FUN_0056123e();
extern void FUN_00563354();

void FUN_005612ed(void)

{
  int iVar1;
  int extraout_ECX;
  int unaff_EBP;
  
  FUN_00563354();
  iVar1 = *(int *)(extraout_ECX + -0x14);
  *(int *)(unaff_EBP + -0x10) = extraout_ECX;
  *(undefined ***)(*(int *)(iVar1 + 4) + -0x14 + extraout_ECX) = &PTR_LAB_005705d8;
  *(undefined4 *)(unaff_EBP + -4) = 0;
  FUN_00560d87();
  *(undefined4 *)(unaff_EBP + -4) = 0xffffffff;
  FUN_0056123e();
  ExceptionList = *(void **)(unaff_EBP + -0xc);
  return;
}
