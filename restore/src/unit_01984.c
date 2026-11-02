#include "restore_types.h"
#define DAT_00586b08 (*(undefined4 *)0x00586b08u)
#define DAT_0058fe48 (*(undefined4 *)0x0058fe48u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd65d (*(undefined4 *)0x00bcd65du)
#include "audio_state.h"
extern int FUN_004e5a30();
extern bool FUN_004e5b20();

void FUN_004e5d10(int param_1,int param_2,undefined1 param_3)

{
  short sVar1;
  undefined4 *puVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int *piVar6;
  int iVar7;
  
  iVar5 = param_2 * 0x184;
  iVar7 = 0;
  (&DAT_00bcd65d)[iVar5] = 1;
  sVar1 = (&DAT_007999e8)[param_1 * 0x84];
  for (iVar4 = 0; iVar4 < (int)((-(uint)(sVar1 != 0xcf) & 8) + 10); iVar4 = iVar4 + 1) {
    puVar2 = (undefined4 *)&DAT_00586b08;
    if (sVar1 != 0xcf) {
      puVar2 = &DAT_0058fe48;
    }
    piVar6 = (int *)(iVar7 + (int)puVar2);
    if (*(short *)(piVar6[2] + 0x12) == param_2) {
      iVar4 = FUN_004e5a30(param_1);
      if (*piVar6 != iVar4) {
        (&DAT_00bcd630)[iVar5] = param_3;
        return;
      }
      if (piVar6 != (int *)0x0) goto LAB_004e5e38;
      break;
    }
    iVar7 = iVar7 + 0x10;
  }
  iVar7 = 0;
  iVar4 = 0;
  do {
    sVar1 = (&DAT_007999e8)[param_1 * 0x84];
    if ((int)((-(uint)(sVar1 != 0xcf) & 8) + 10) <= iVar7) {
LAB_004e5e28: ;
      (&DAT_00bcd630)[iVar5] = param_3;
      return;
    }
    puVar2 = (undefined4 *)&DAT_00586b08;
    if (sVar1 != 0xcf) {
      puVar2 = &DAT_0058fe48;
    }
    piVar6 = (int *)(iVar4 + (int)puVar2);
    if (*(short *)(*(int *)(iVar4 + 8 + (int)puVar2) + 0x12) == 0) {
      iVar3 = FUN_004e5a30(param_1);
      if (((*piVar6 == iVar3) && (piVar6[1] == (uint)(byte)(&DAT_00bcd602)[iVar5])) &&
         (0 < *(int *)(piVar6[2] + 0xc))) {
        if (piVar6 != (int *)0x0) {
LAB_004e5e38: ;
          if (*(short *)(piVar6[2] + 0x12) == 0) {
            FUN_0041fd80((void *)(uintptr_t)piVar6[2],param_2);
            FUN_0041fca0((void *)(uintptr_t)piVar6[2],0);
          }
          *(int *)(piVar6[2] + 0xc) = *(int *)(piVar6[2] + 0xc) + -1;
          if (*(int *)(piVar6[2] + 0xc) < 1) {
            FUN_004e5b20(param_1);
            (&DAT_00bcd630)[iVar5] = param_3;
          }
          return;
        }
        goto LAB_004e5e28;
      }
    }
    iVar7 = iVar7 + 1;
    iVar4 = iVar4 + 0x10;
  } while( true );
}
