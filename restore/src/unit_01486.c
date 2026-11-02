#include "restore_types.h"
#define DAT_00bcb9f0 (*(undefined4 *)0x00bcb9f0u)
extern void FUN_004b7b50();
extern void FUN_004b7c80();
extern int FUN_004cde60();

void FUN_004b7ab0(int param_1)

{
  int iVar1;
  
  if (param_1 != 0) {
    FUN_004b7c80();
    iVar1 = FUN_004cde60(1,(&DAT_00bcb9f0)[param_1 * 0x90]);
    if (iVar1 < 8) {
      FUN_004b7b50(param_1,0xfffffffb);
      return;
    }
    if (iVar1 < 10) {
      FUN_004b7b50(param_1,0xfffffff9);
      return;
    }
    if (iVar1 < 0xe) {
      FUN_004b7b50(param_1,0xfffffff6);
      return;
    }
    if (iVar1 < 0x14) {
      FUN_004b7b50(param_1,0xfffffff4);
      return;
    }
    if (iVar1 < 0x1e) {
      FUN_004b7b50(param_1,0xfffffff1);
      return;
    }
    FUN_004b7b50(param_1,0xffffffec);
  }
  return;
}
