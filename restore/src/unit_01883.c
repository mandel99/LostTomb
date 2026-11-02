#include "restore_types.h"
#define DAT_00e38db0 (*(uint32_t *)0x00e38db0u)
extern void FUN_004303a0();

void FUN_004ddf20(void)

{
  undefined4 uVar1;
  undefined4 uVar2;
  
  if (DAT_00e38db0 != 0x280) {
    if (DAT_00e38db0 == 800) {
      uVar2 = 0x27;
      uVar1 = 10;
      goto LAB_004ddf56;
    }
    if (DAT_00e38db0 == 0x400) {
      uVar2 = 0x32;
      uVar1 = 0xe;
      goto LAB_004ddf56;
    }
  }
  uVar2 = 0x1f;
  uVar1 = 8;
LAB_004ddf56: ;
  FUN_004303a0(0xe4,0xe4,0,0x1e,uVar1,uVar2,0x3a,0x1e);
  return;
}
