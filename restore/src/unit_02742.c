#include "restore_types.h"
#define PTR_FUN_0057053c (*(unsigned char * *)0x0057053cu)
extern void FUN_00560f4b();
extern void FUN_00561a2b();
extern void FUN_00563354();

void FUN_005606e2(void)

{
  undefined4 *extraout_ECX;
  int unaff_EBP;
  
  FUN_00563354();
  *(undefined4 **)(unaff_EBP + -0x10) = extraout_ECX;
  *extraout_ECX = &PTR_FUN_0057053c;
  *(undefined4 *)(unaff_EBP + -4) = 0;
  if ((extraout_ECX[0xc] != 0) && (extraout_ECX[4] != 0)) {
    if ((code *)extraout_ECX[0x11] == (code *)0x0) {
      FUN_00561a2b(extraout_ECX[4]);
    }
    else {
      (*(code *)extraout_ECX[0x11])();
    }
  }
  *(undefined4 *)(unaff_EBP + -4) = 0xffffffff;
  FUN_00560f4b();
  ExceptionList = *(void **)(unaff_EBP + -0xc);
  return;
}
