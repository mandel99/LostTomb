#include "restore_types.h"
#define DAT_005c2748 (*(undefined4 *)0x005c2748u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd65d (*(undefined4 *)0x00bcd65du)
#define DAT_00ea5ba8 (*(undefined4 *)0x00ea5ba8u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define PTR_DAT_005c2750 (*(unsigned char * *)0x005c2750u)
#include "audio_state.h"
extern void FUN_004200b0();

void FUN_00420130(undefined4 param_1,int param_2,undefined1 param_3)

{
  undefined **ppuVar1;
  int iVar2;
  int *piVar3;
  int iVar4;
  
  ppuVar1 = &PTR_DAT_005c2750;
  iVar4 = param_2 * 0x184;
  iVar2 = 0;
  (&DAT_00bcd65d)[iVar4] = 1;
  do {
    if (*(short *)(*ppuVar1 + 0x12) == param_2) {
      piVar3 = &DAT_005c2748 + iVar2 * 4;
      if (*piVar3 != (int)*(short *)((int)&DAT_00ea5ba8 + DAT_00eaac28 * 0x49d8 + 2))
      goto LAB_00420231;
      if (piVar3 != (int *)0x0) goto LAB_004201ff;
      break;
    }
    ppuVar1 = ppuVar1 + 4;
    iVar2 = iVar2 + 1;
  } while ((int)ppuVar1 < 0x5c28b0);
  iVar2 = 0;
  ppuVar1 = &PTR_DAT_005c2750;
  while ((((*(short *)(*ppuVar1 + 0x12) != 0 ||
           (ppuVar1[-2] !=
            (undefined *)(int)*(short *)((int)&DAT_00ea5ba8 + DAT_00eaac28 * 0x49d8 + 2))) ||
          (ppuVar1[-1] != (undefined *)(uint)(byte)(&DAT_00bcd602)[iVar4])) ||
         (*(int *)(*ppuVar1 + 0xc) < 1))) {
    ppuVar1 = ppuVar1 + 4;
    iVar2 = iVar2 + 1;
    if (0x5c28af < (int)ppuVar1) {
      (&DAT_00bcd630)[iVar4] = param_3;
      return;
    }
  }
  piVar3 = &DAT_005c2748 + iVar2 * 4;
  if (piVar3 == (int *)0x0) {
    (&DAT_00bcd630)[iVar4] = param_3;
    return;
  }
LAB_004201ff: ;
  if (*(short *)(piVar3[2] + 0x12) == 0) {
    FUN_0041fd80((void *)(uintptr_t)piVar3[2],param_2);
    FUN_0041fca0((void *)(uintptr_t)piVar3[2],0);
  }
  *(int *)(piVar3[2] + 0xc) = *(int *)(piVar3[2] + 0xc) + -1;
  if (*(int *)(piVar3[2] + 0xc) < 1) {
    FUN_004200b0();
LAB_00420231: ;
    (&DAT_00bcd630)[iVar4] = param_3;
  }
  return;
}
