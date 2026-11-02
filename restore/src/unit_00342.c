#include "restore_types.h"
#define DAT_0059ede8 (*(undefined4 *)0x0059ede8u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a30 (*(undefined4 *)0x00799a30u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd65d (*(undefined4 *)0x00bcd65du)
#define PTR_DAT_0059edf0 (*(unsigned char * *)0x0059edf0u)
#include "audio_state.h"
extern void FUN_00420770();

void FUN_004207e0(int param_1,int param_2,undefined1 param_3)

{
  undefined *puVar1;
  undefined **ppuVar2;
  int iVar3;
  int iVar4;
  undefined4 *puVar5;
  
  iVar4 = param_2 * 0x184;
  puVar1 = (undefined *)(&DAT_00799a30)[(param_1 - (short)(&DAT_007999ea)[param_1 * 0x84]) * 0x42];
  iVar3 = 0;
  (&DAT_00bcd65d)[iVar4] = 1;
  ppuVar2 = &PTR_DAT_0059edf0;
  do {
    if (*(short *)(*ppuVar2 + 0x12) == param_2) {
      puVar5 = &DAT_0059ede8 + iVar3 * 4;
      if ((undefined *)(&DAT_0059ede8)[iVar3 * 4] != puVar1) {
        (&DAT_00bcd630)[iVar4] = param_3;
        return;
      }
      if (puVar5 != (undefined4 *)0x0) goto LAB_004208c8;
      break;
    }
    ppuVar2 = ppuVar2 + 4;
    iVar3 = iVar3 + 1;
  } while ((int)ppuVar2 < 0x59efa0);
  iVar3 = 0;
  ppuVar2 = &PTR_DAT_0059edf0;
  while ((((*(short *)(*ppuVar2 + 0x12) != 0 || (ppuVar2[-2] != puVar1)) ||
          (ppuVar2[-1] != (undefined *)(uint)(byte)(&DAT_00bcd602)[iVar4])) ||
         (*(int *)(*ppuVar2 + 0xc) < 1))) {
    ppuVar2 = ppuVar2 + 4;
    iVar3 = iVar3 + 1;
    if (0x59ef9f < (int)ppuVar2) {
      (&DAT_00bcd630)[iVar4] = param_3;
      return;
    }
  }
  puVar5 = &DAT_0059ede8 + iVar3 * 4;
  if (puVar5 == (undefined4 *)0x0) {
    (&DAT_00bcd630)[iVar4] = param_3;
    return;
  }
LAB_004208c8: ;
  if (*(short *)(puVar5[2] + 0x12) == 0) {
    FUN_0041fd80((void *)(uintptr_t)puVar5[2],param_2);
    FUN_0041fca0((void *)(uintptr_t)puVar5[2],0);
  }
  *(int *)(puVar5[2] + 0xc) = *(int *)(puVar5[2] + 0xc) + -1;
  if (*(int *)(puVar5[2] + 0xc) < 1) {
    FUN_00420770(param_1);
    (&DAT_00bcd630)[iVar4] = param_3;
  }
  return;
}
