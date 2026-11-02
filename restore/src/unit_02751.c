#include "restore_types.h"
#define DAT_007027d4 (*(undefined4 *)0x007027d4u)
extern uint FUN_00560b2a();
extern undefined4 FUN_00560bfd();
extern int FUN_00560c26();
extern void FUN_00560d96();

void FUN_00560aae(undefined4 param_1)

{
  uint *puVar1;
  int iVar2;
  int *in_ECX;
  
  iVar2 = FUN_00560bfd();
  if (iVar2 != 0) {
    if (*(int *)((int)in_ECX + *(int *)(*in_ECX + 4) + 0x30) == 0) {
      iVar2 = FUN_00560b2a((undefined1)param_1);
      if (iVar2 == -1) {
        iVar2 = (**(code **)(**(int **)(*(int *)(*in_ECX + 4) + 4 + (int)in_ECX) + 0x1c))
                          ((undefined1)param_1);
        if (iVar2 == -1) {
          puVar1 = (uint *)(*(int *)(*in_ECX + 4) + 8 + (int)in_ECX);
          *puVar1 = *puVar1 | 6;
        }
      }
    }
    else {
      RESTORE_SLICE1(param_1,3) = 0;
      RESTORE_SLICE1(param_1,2) = (undefined1)param_1;
      FUN_00560d96(&DAT_007027d4,(int)&param_1 + 2);
    }
    FUN_00560c26();
  }
  return;
}
