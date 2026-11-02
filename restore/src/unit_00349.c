#include "restore_types.h"
#define DAT_005a8140 (*(undefined4 *)0x005a8140u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd65d (*(undefined4 *)0x00bcd65du)
#define PTR_DAT_005a8148 (*(unsigned char * *)0x005a8148u)
#include "audio_state.h"
extern void FUN_00420a90();

void FUN_00420b10(int param_1,int param_2,undefined1 param_3)

{
  undefined *puVar1;
  undefined **ppuVar2;
  int iVar3;
  undefined4 *puVar4;
  int iVar5;
  
  iVar5 = param_2 * 0x184;
  puVar1 = *(undefined **)
            (&DAT_00799a2c + (param_1 - (short)(&DAT_007999ea)[param_1 * 0x84]) * 0x84);
  iVar3 = 0;
  (&DAT_00bcd65d)[iVar5] = 1;
  ppuVar2 = &PTR_DAT_005a8148;
  while (((*(short *)(*ppuVar2 + 0x12) != param_2 || (ppuVar2[-2] != puVar1)) ||
         (*(int *)(*ppuVar2 + 0xc) < 1))) {
    ppuVar2 = ppuVar2 + 4;
    iVar3 = iVar3 + 1;
    if (0x5a84c7 < (int)ppuVar2) {
LAB_00420b8e: ;
      iVar3 = 0;
      ppuVar2 = &PTR_DAT_005a8148;
      while (((*(short *)(*ppuVar2 + 0x12) != 0 || (ppuVar2[-2] != puVar1)) ||
             ((ppuVar2[-1] != (undefined *)(uint)(byte)(&DAT_00bcd602)[iVar5] ||
              (*(int *)(*ppuVar2 + 0xc) < 1))))) {
        ppuVar2 = ppuVar2 + 4;
        iVar3 = iVar3 + 1;
        if (0x5a84c7 < (int)ppuVar2) {
          (&DAT_00bcd630)[iVar5] = param_3;
          return;
        }
      }
      puVar4 = &DAT_005a8140 + iVar3 * 4;
      if (puVar4 == (undefined4 *)0x0) {
        (&DAT_00bcd630)[iVar5] = param_3;
        return;
      }
LAB_00420bf1: ;
      if (*(short *)(puVar4[2] + 0x12) == 0) {
        FUN_0041fd80((void *)(uintptr_t)puVar4[2],param_2);
        FUN_0041fca0((void *)(uintptr_t)puVar4[2],0);
      }
      *(int *)(puVar4[2] + 0xc) = *(int *)(puVar4[2] + 0xc) + -1;
      if (*(int *)(puVar4[2] + 0xc) < 1) {
        FUN_00420a90(param_1,iVar3);
        iVar3 = puVar4[2];
        (&DAT_00bcd630)[iVar5] = param_3;
        *(undefined2 *)(iVar3 + 0x10) = 3;
      }
      return;
    }
  }
  puVar4 = &DAT_005a8140 + iVar3 * 4;
  if (puVar4 != (undefined4 *)0x0) goto LAB_00420bf1;
  goto LAB_00420b8e;
}
