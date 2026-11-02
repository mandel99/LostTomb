#include "restore_types.h"
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
#define PTR_DAT_0059edf0 (*(unsigned char * *)0x0059edf0u)
extern uint FUN_0041f8b0();
extern void FUN_0041f8c0();
extern void FUN_0041f910();
extern int FUN_0053e590();

void FUN_004206f0(int param_1,int param_2)

{
  uint uVar1;
  uint uVar2;
  undefined **ppuVar3;
  int local_c;
  int local_8;
  int local_4;
  
  uVar2 = DAT_00bca2e0 / 2 & 1;
  uVar1 = FUN_0041f8b0();
  if (uVar2 != uVar1) {
    FUN_0041f8c0(uVar2);
  }
  ppuVar3 = &PTR_DAT_0059edf0;
  do {
    FUN_0041f910(&local_4,&local_8,&local_c);
    if (local_c != 0) {
      FUN_0053e590(local_c,local_4 + param_1,local_8 + param_2);
    }
    ppuVar3 = ppuVar3 + 4;
  } while ((int)ppuVar3 < 0x59efa0);
  return;
}
