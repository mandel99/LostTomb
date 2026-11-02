#include "restore_types.h"
#define DAT_005ad210 (*(undefined4 *)0x005ad210u)
#define DAT_00799a30 (*(undefined4 *)0x00799a30u)
#define DAT_00799a34 (*(undefined4 *)0x00799a34u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd65d (*(undefined4 *)0x00bcd65du)
#define PTR_DAT_005ad218 (*(unsigned char * *)0x005ad218u)
#include "audio_state.h"
extern void FUN_00420dc0();

void FUN_00420e20(int param_1,int param_2,undefined1 param_3)

{
  undefined *puVar1;
  undefined **ppuVar2;
  undefined *puVar3;
  int iVar4;
  undefined4 *puVar5;
  int iVar6;
  
  iVar6 = param_2 * 0x184;
  puVar1 = (undefined *)(&DAT_00799a30)[param_1 * 0x42];
  (&DAT_00bcd65d)[iVar6] = 1;
  if ((int)puVar1 < 0x1b) {
    ppuVar2 = &PTR_DAT_005ad218 + (int)puVar1 * 4;
    puVar3 = puVar1;
    do {
      if (*(short *)(*ppuVar2 + 0x12) == param_2) {
        puVar5 = &DAT_005ad210 + (int)puVar3 * 4;
        if ((undefined *)(&DAT_005ad210)[(int)puVar3 * 4] != puVar1) goto LAB_00420f3a;
        if (puVar5 != (undefined4 *)0x0) goto LAB_00420ef2;
        break;
      }
      ppuVar2 = ppuVar2 + 4;
      puVar3 = puVar3 + 1;
    } while ((int)ppuVar2 < 0x5ad3c8);
  }
  iVar4 = 0;
  ppuVar2 = &PTR_DAT_005ad218;
  while ((((*(short *)(*ppuVar2 + 0x12) != 0 || (ppuVar2[-2] != puVar1)) ||
          (ppuVar2[-1] != (undefined *)(uint)(byte)(&DAT_00bcd602)[iVar6])) ||
         (*(int *)(*ppuVar2 + 0xc) < 1))) {
    ppuVar2 = ppuVar2 + 4;
    iVar4 = iVar4 + 1;
    if (0x5ad3c7 < (int)ppuVar2) {
      (&DAT_00bcd630)[iVar6] = param_3;
      return;
    }
  }
  puVar5 = &DAT_005ad210 + iVar4 * 4;
  if (puVar5 == (undefined4 *)0x0) {
    (&DAT_00bcd630)[iVar6] = param_3;
    return;
  }
LAB_00420ef2: ;
  if (*(short *)(puVar5[2] + 0x12) == 0) {
    FUN_0041fd80((void *)(uintptr_t)puVar5[2],param_2);
    FUN_0041fca0((void *)(uintptr_t)puVar5[2],0);
  }
  *(int *)(puVar5[2] + 0xc) = *(int *)(puVar5[2] + 0xc) + -1;
  if (*(int *)(puVar5[2] + 0xc) < 1) {
    FUN_00420dc0(param_1);
    *(undefined4 *)(&DAT_00799a34 + param_1 * 0x108) = 0;
LAB_00420f3a: ;
    (&DAT_00bcd630)[iVar6] = param_3;
  }
  return;
}
