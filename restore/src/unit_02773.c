#include "restore_types.h"
#define DAT_005705dc (*(undefined4 *)0x005705dcu)
#define DAT_005705e4 (*(undefined4 *)0x005705e4u)
#define PTR_LAB_005705d8 (*(unsigned char * *)0x005705d8u)
extern void FUN_005605fa();
extern int * FUN_00560d22();
extern int * FUN_005611c8();
extern void FUN_00563354();

int * FUN_00561278(void)

{
  int *extraout_ECX;
  int unaff_EBP;
  
  FUN_00563354();
  *(int **)(unaff_EBP + -0x14) = extraout_ECX;
  *(undefined4 *)(unaff_EBP + -0x10) = 0;
  if (*(int *)(unaff_EBP + 0xc) != 0) {
    *extraout_ECX = (int)&DAT_005705e4;
    extraout_ECX[3] = (int)&DAT_005705dc;
    FUN_005605fa();
    *(undefined4 *)(unaff_EBP + -0x10) = 1;
    *(undefined4 *)(unaff_EBP + -4) = 0;
  }
  FUN_005611c8(*(undefined4 *)(unaff_EBP + 8),0);
  *(undefined4 *)(unaff_EBP + -4) = 1;
  FUN_00560d22(*(undefined4 *)(unaff_EBP + 8),0);
  ExceptionList = *(void **)(unaff_EBP + -0xc);
  *(undefined ***)(*(int *)(*extraout_ECX + 4) + (int)extraout_ECX) = &PTR_LAB_005705d8;
  return extraout_ECX;
}
