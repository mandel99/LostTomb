#include "restore_types.h"
#define DAT_012663c0 (*(undefined4 *)0x012663c0u)
#define _DAT_006050a0 (*(undefined4 *)0x006050a0u)
#define _DAT_006050aa (*(undefined4 *)0x006050aau)
extern float10 FUN_00567517();
extern void FUN_00567575();
extern undefined4 FUN_0056758c();
extern void FUN_0056760b();

float10 FUN_00562d1d(int param_1,uint param_2)

{
  uint in_EAX;
  bool in_ZF;
  short in_FPUControlWord;
  float10 in_ST0;
  float10 extraout_ST0;
  float10 fVar1;
  
  if (in_ZF) {
    if (((in_EAX & 0xfffff) != 0) || (fVar1 = _DAT_006050a0, param_1 != 0)) {
      fVar1 = (float10)FUN_0056758c();
    }
LAB_00562dac: ;
    if (DAT_012663c0 == 0) {
      fVar1 = (float10)FUN_00567517();
      return fVar1;
    }
  }
  else {
    if (in_FPUControlWord != 0x27f) {
      in_EAX = (FUN_00567575(), restore_unresolved_return(0x00567575u));
      in_ST0 = extraout_ST0;
    }
    if (in_EAX < 0x3ff00000) {
      fVar1 = (float10)restore_x87_fpatan(in_ST0,restore_x87_sqrt(((float10)1 - in_ST0) * ((float10)1 + in_ST0)));
    }
    else {
      fVar1 = _DAT_006050a0;
      if ((0x3ff00000 < in_EAX) || ((param_2 & 0xfffff) != 0 || param_1 != 0)) goto LAB_00562dac;
      fVar1 = _DAT_006050aa;
      if ((param_2 & 0x80000000) != 0) {
        fVar1 = -_DAT_006050aa;
      }
    }
    if (DAT_012663c0 == 0) {
      fVar1 = (float10)(FUN_0056760b(), restore_unresolved_return(0x0056760bu));
      return fVar1;
    }
  }
  return fVar1;
}
