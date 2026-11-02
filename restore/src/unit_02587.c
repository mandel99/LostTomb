#include "restore_types.h"
#define DAT_007999e4 (*(undefined4 *)0x007999e4u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_007999fe (*(undefined4 *)0x007999feu)
#define DAT_00799a00 (*(undefined4 *)0x00799a00u)
#define DAT_00799a34 (*(undefined4 *)0x00799a34u)
#define DAT_00799a6e (*(undefined4 *)0x00799a6eu)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd620 (*(undefined4 *)0x00bcd620u)
#define DAT_00bcd62e (*(undefined4 *)0x00bcd62eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd63c (*(undefined4 *)0x00bcd63cu)
#define DAT_00bcd654 (*(undefined4 *)0x00bcd654u)
#define DAT_00bcd656 (*(undefined4 *)0x00bcd656u)
#define DAT_00bcd660 (*(undefined4 *)0x00bcd660u)
#define DAT_00bcd66c (*(undefined4 *)0x00bcd66cu)
#define DAT_00bcd672 (*(undefined4 *)0x00bcd672u)
#define DAT_00d3b4e0 (*(undefined *)0x00d3b4e0u)
#define DAT_00d3b4e4 (*(undefined *)0x00d3b4e4u)
extern int FUN_00422a90();
extern int FUN_0045bb50();

undefined4 FUN_00547c10(int param_1,int param_2)

{
  int *piVar1;
  char cVar2;
  undefined4 uVar3;
  undefined2 uVar4;
  char cVar5;
  int iVar6;
  int *in_ECX;
  int iVar7;
  
  iVar7 = param_2 * 0x184;
  cVar2 = (&DAT_00bcd602)[iVar7];
  cVar5 = (**(code **)(*in_ECX + 0x24))(param_1);
  if (cVar5 == '\0') {
    if (((((char)param_1 == '\0') || ((&DAT_007999ea)[param_1 * 0x84] != 0)) ||
        ((char)in_ECX[0x13] != '\0')) || (399 < *(int *)((int)&DAT_00799a6e + param_1 * 0x108 + 2)))
    {
      piVar1 = (int *)((int)&DAT_00799a6e +
                      (param_1 - (short)(&DAT_007999ea)[param_1 * 0x84]) * 0x108 + 2);
      if ((99 < *(int *)((int)&DAT_00799a6e +
                        (param_1 - (short)(&DAT_007999ea)[param_1 * 0x84]) * 0x108 + 2)) &&
         (iVar6 = FUN_00422a90(param_1,param_2), iVar6 != 0)) {
        if (cVar2 != 'U') {
          return 1;
        }
        if (((&DAT_00bcd602)[iVar7] == 'Q') || ((&DAT_00bcd602)[iVar7] == 'l')) {
          (&DAT_00bcd630)[iVar7] = 9;
        }
        *piVar1 = *piVar1 + -100;
        *(int *)(&DAT_00799a34 + iVar6 * 0x108) = param_2;
        *(undefined2 *)(&DAT_00bcd61c + iVar7) = (&DAT_007999fe)[iVar6 * 0x84];
        uVar3 = *(undefined4 *)(&DAT_007999e4 + iVar6 * 0x108);
        *(undefined2 *)(&DAT_00bcd61e + iVar7) = (&DAT_00799a00)[iVar6 * 0x84];
        *(undefined4 *)(&DAT_00bcd620 + iVar7) = uVar3;
        *(undefined2 *)(&DAT_00bcd63c + iVar7) = 0;
        *(undefined2 *)(&DAT_00bcd62e + iVar7) = 0;
        return 1;
      }
    }
    else {
      iVar6 = FUN_0045bb50(param_1,0x22,1);
      uVar4 = DAT_00d3b4e4;
      if (iVar6 != 0) {
        if (cVar2 != 'U') {
          return 1;
        }
        (&DAT_00bcd672)[iVar7] = 0x22;
        *(undefined2 *)(&DAT_00bcd66c + iVar7) = 0;
        (&DAT_00bcd656)[param_2 * 0xc2] = (short)iVar6;
        *(short *)(&DAT_00bcd660 + iVar7) = (short)param_2;
        (&DAT_00bcd654)[param_2 * 0xc2] = (short)param_1;
        (&DAT_00bcd630)[iVar7] = 0xf;
        *(undefined2 *)(&DAT_00bcd62e + iVar7) = 0;
        *(undefined2 *)(&DAT_00bcd61c + iVar7) = uVar4;
        *(undefined2 *)(&DAT_00bcd61e + iVar7) = DAT_00d3b4e0;
        *(undefined4 *)(&DAT_00bcd620 + iVar7) = *(undefined4 *)(&DAT_007999e4 + iVar6 * 0x108);
        *(undefined2 *)(&DAT_00bcd63c + iVar7) = 0;
        *(undefined1 *)(in_ECX + 0x13) = 1;
        return 1;
      }
    }
  }
  return 0;
}
