#include "restore_types.h"
#define DAT_00570570 (*(undefined4 *)0x00570570u)
#define DAT_00570578 (*(undefined4 *)0x00570578u)
#define PTR_LAB_0057056c (*(unsigned char * *)0x0057056cu)
extern void FUN_005605fa();
extern void FUN_005606a0();
extern int * FUN_00561278();
extern void * FUN_00561a1d();
extern void FUN_00563354();

int * FUN_005609da(void)

{
  void *pvVar1;
  undefined4 uVar2;
  int *extraout_ECX;
  int unaff_EBP;
  
  FUN_00563354();
  *(int **)(unaff_EBP + -0x14) = extraout_ECX;
  *(undefined4 *)(unaff_EBP + -0x10) = 0;
  if (*(int *)(unaff_EBP + 8) != 0) {
    *extraout_ECX = (int)&DAT_00570578;
    extraout_ECX[3] = (int)&DAT_00570570;
    FUN_005605fa();
    *(undefined4 *)(unaff_EBP + -0x10) = 1;
    *(undefined4 *)(unaff_EBP + -4) = 0;
  }
  pvVar1 = FUN_00561a1d(0x48);
  *(void **)(unaff_EBP + 8) = pvVar1;
  *(undefined4 *)(unaff_EBP + -4) = 1;
  if (pvVar1 == (void *)0x0) {
    uVar2 = 0;
  }
  else {
    uVar2 = (FUN_005606a0(), restore_unresolved_return(0x005606a0u));
  }
  *(undefined1 *)(unaff_EBP + -4) = 0;
  FUN_00561278(uVar2,0);
  ExceptionList = *(void **)(unaff_EBP + -0xc);
  *(undefined ***)(*(int *)(*extraout_ECX + 4) + (int)extraout_ECX) = &PTR_LAB_0057056c;
  *(undefined4 *)(*(int *)(*extraout_ECX + 4) + 0x1c + (int)extraout_ECX) = 1;
  *(undefined4 *)(*(int *)(*extraout_ECX + 4) + 0x1c + (int)extraout_ECX) = 1;
  return extraout_ECX;
}
