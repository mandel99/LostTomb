#include "restore_types.h"
#include "animation_state.h"
#define DAT_00586b08 (*(undefined4 *)0x00586b08u)
#define DAT_0058fe48 (*(undefined4 *)0x0058fe48u)
#define DAT_007999e4 (*(undefined4 *)0x007999e4u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999fe (*(undefined4 *)0x007999feu)
#define DAT_00799a00 (*(undefined4 *)0x00799a00u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd620 (*(undefined4 *)0x00bcd620u)
#define DAT_00bcd63c (*(undefined4 *)0x00bcd63cu)
extern int FUN_004e5a30();

int FUN_004e5c00(undefined4 param_1,undefined4 param_2,int param_3)

{
  undefined2 uVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  undefined4 *puVar6;
  int local_8;
  
  iVar3 = FUN_004e59c0();
  iVar5 = 0;
  if (iVar3 != 0) {
    for (local_8 = 0; local_8 < (int)((-(uint)((&DAT_007999e8)[iVar3 * 0x84] != 0xcf) & 8) + 10);
        local_8 = local_8 + 1) {
      puVar6 = (undefined4 *)&DAT_00586b08;
      if ((&DAT_007999e8)[iVar3 * 0x84] != 0xcf) {
        puVar6 = &DAT_0058fe48;
      }
      if ((((*(short *)(*(int *)((int)puVar6 + iVar5 + 8) + 0x12) == 0) &&
           (iVar4 = FUN_004e5a30(iVar3), *(int *)((int)puVar6 + iVar5) == iVar4)) &&
          (*(uint *)((int)puVar6 + iVar5 + 4) == (uint)(byte)(&DAT_00bcd602)[param_3 * 0x184])) &&
         (0 < *(int *)(*(int *)((int)puVar6 + iVar5 + 8) + 0xc))) {
        uVar1 = (&DAT_00799a00)[iVar3 * 0x84];
        param_3 = param_3 * 0x184;
        *(undefined2 *)(&DAT_00bcd61c + param_3) = (&DAT_007999fe)[iVar3 * 0x84];
        uVar2 = *(undefined4 *)(&DAT_007999e4 + iVar3 * 0x108);
        *(undefined2 *)(&DAT_00bcd61e + param_3) = uVar1;
        *(undefined4 *)(&DAT_00bcd620 + param_3) = uVar2;
        *(undefined2 *)(&DAT_00bcd63c + param_3) = 0;
        return iVar3;
      }
      iVar5 = iVar5 + 0x10;
    }
  }
  return 0;
}
