#include "restore_types.h"
#define DAT_0057058c (*(undefined4 *)0x0057058cu)
#define PTR_LAB_00570588 (*(unsigned char * *)0x00570588u)
extern void FUN_005605fa();
extern void FUN_00560671();
extern void FUN_00563354();

int * FUN_00560d22(void)

{
  undefined4 uVar1;
  int *extraout_ECX;
  int unaff_EBP;
  
  FUN_00563354();
  *(undefined4 *)(unaff_EBP + -0x10) = 0;
  *(int **)(unaff_EBP + -0x14) = extraout_ECX;
  if (*(int *)(unaff_EBP + 0xc) != 0) {
    *extraout_ECX = (int)&DAT_0057058c;
    FUN_005605fa();
    *(undefined4 *)(unaff_EBP + -0x10) = 1;
    *(undefined4 *)(unaff_EBP + -4) = 0;
  }
  uVar1 = *(undefined4 *)(unaff_EBP + 8);
  *(undefined ***)(*(int *)(*extraout_ECX + 4) + (int)extraout_ECX) = &PTR_LAB_00570588;
  FUN_00560671(uVar1);
  ExceptionList = *(void **)(unaff_EBP + -0xc);
  extraout_ECX[1] = 0;
  return extraout_ECX;
}
