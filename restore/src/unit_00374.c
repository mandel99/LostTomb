#include "restore_types.h"
#define DAT_005be748 (*(undefined4 *)0x005be748u)
#define DAT_00799a34 (*(undefined4 *)0x00799a34u)
#define DAT_00799a38 (*(undefined4 *)0x00799a38u)
#define DAT_00799a58 (*(undefined4 *)0x00799a58u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd65d (*(undefined4 *)0x00bcd65du)
#define PTR_DAT_005be750 (*(unsigned char * *)0x005be750u)
#include "audio_state.h"
extern void FUN_004223f0();

void FUN_004237b0(int param_1,int param_2,undefined1 param_3)

{
  int iVar1;
  undefined **ppuVar2;
  int *piVar3;
  int iVar4;
  undefined4 *puVar5;
  int iVar6;
  
  iVar6 = param_2 * 0x184;
  iVar1 = *(int *)(&DAT_00799a38 + param_1 * 0x108);
  iVar4 = 0;
  (&DAT_00bcd65d)[iVar6] = 1;
  ppuVar2 = &PTR_DAT_005be750;
  do {
    if (*(short *)(*ppuVar2 + 0x12) == param_2) {
      puVar5 = &DAT_005be748 + iVar4 * 4;
      if ((&DAT_005be748)[iVar4 * 4] != iVar1) {
        (&DAT_00bcd630)[iVar6] = param_3;
        return;
      }
      if (puVar5 != (undefined4 *)0x0) goto LAB_00423885;
      break;
    }
    ppuVar2 = ppuVar2 + 4;
    iVar4 = iVar4 + 1;
  } while ((int)ppuVar2 < 0x5be780);
  iVar4 = 0;
  piVar3 = &DAT_005be748;
  while ((((*(short *)(piVar3[2] + 0x12) != 0 || (*piVar3 != iVar1)) ||
          (piVar3[1] != (uint)(byte)(&DAT_00bcd602)[iVar6])) ||
         (*(int *)(&DAT_00799a58 + (iVar1 + param_1 * 0x42) * 4) < 1))) {
    piVar3 = piVar3 + 4;
    iVar4 = iVar4 + 1;
    if (0x5be777 < (int)piVar3) {
      (&DAT_00bcd630)[iVar6] = param_3;
      return;
    }
  }
  puVar5 = &DAT_005be748 + iVar4 * 4;
  if (puVar5 != (undefined4 *)0x0) {
LAB_00423885: ;
    if (*(short *)(puVar5[2] + 0x12) == 0) {
      FUN_0041fd80((void *)(uintptr_t)puVar5[2],param_2);
      FUN_0041fca0((void *)(uintptr_t)puVar5[2],0);
    }
    iVar1 = iVar1 + param_1 * 0x42;
    iVar4 = *(int *)(&DAT_00799a58 + iVar1 * 4);
    *(int *)(&DAT_00799a58 + iVar1 * 4) = iVar4 + -1;
    if (0 < iVar4 + -1) {
      return;
    }
    *(undefined4 *)(&DAT_00799a34 + param_1 * 0x108) = 0;
    FUN_004223f0(param_1);
  }
  (&DAT_00bcd630)[iVar6] = param_3;
  return;
}
