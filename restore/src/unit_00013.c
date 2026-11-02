#include "restore_types.h"
#define DAT_007999e4 (*(undefined4 *)0x007999e4u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_007999fe (*(undefined4 *)0x007999feu)
#define DAT_00799a00 (*(undefined4 *)0x00799a00u)
#define DAT_00799a34 (*(undefined4 *)0x00799a34u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd620 (*(undefined4 *)0x00bcd620u)
#define DAT_00bcd62e (*(undefined4 *)0x00bcd62eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd63c (*(undefined4 *)0x00bcd63cu)
#define DAT_00bcd656 (*(undefined4 *)0x00bcd656u)
extern int FUN_00420c40();

uint FUN_00401d40(int param_1,char param_2,char param_3)

{
  undefined2 uVar1;
  undefined2 uVar2;
  undefined4 uVar3;
  uint uVar4;
  int iVar5;
  int *in_ECX;
  int iVar6;
  int iVar7;
  
  iVar7 = param_1 - (short)(&DAT_007999ea)[param_1 * 0x84];
  iVar6 = iVar7 * 0x108;
  uVar4 = in_ECX[(short)(&DAT_007999ea)[iVar7 * 0x84] * 0xd + 0x16];
  if ((-1 < (int)uVar4) && ((int)uVar4 < 0xc)) {
    uVar4 = (**(code **)(*in_ECX + 0x24))(param_1);
    if ((char)uVar4 == '\0') {
      iVar5 = FUN_00420c40(param_1,param_1);
      uVar4 = 0;
      if (iVar5 != 0) {
        iVar5 = CONCAT31((int3)((uint)iVar5 >> 8),param_2);
        if (param_2 != '\0') {
          iVar5 = param_1 * 0x184;
          if ((&DAT_00bcd602)[iVar5] == 'O') {
            if (param_3 == '\0') {
              uVar1 = (&DAT_007999fe)[iVar7 * 0x84];
              uVar2 = (&DAT_00799a00)[iVar7 * 0x84];
              (&DAT_00bcd630)[iVar5] = 9;
              *(undefined2 *)(&DAT_00bcd61c + iVar5) = uVar1;
              uVar3 = *(undefined4 *)(&DAT_007999e4 + iVar6);
              *(undefined2 *)(&DAT_00bcd61e + iVar5) = uVar2;
              *(undefined4 *)(&DAT_00bcd620 + iVar5) = uVar3;
              *(undefined2 *)(&DAT_00bcd63c + iVar5) = 0;
              *(undefined2 *)(&DAT_00bcd62e + iVar5) = 0;
            }
            else {
              (&DAT_00bcd630)[iVar5] = 10;
            }
            (&DAT_00bcd656)[param_1 * 0xc2] = (short)iVar7;
            *(int *)(&DAT_00799a34 + iVar6) = param_1;
            *(undefined2 *)(&DAT_00bcd62e + iVar5) = 0;
          }
          else if ((&DAT_00bcd602)[iVar5] == 'Q') {
            *(int *)(&DAT_00799a34 + iVar6) = param_1;
            uVar1 = (&DAT_007999fe)[iVar7 * 0x84];
            uVar2 = (&DAT_00799a00)[iVar7 * 0x84];
            (&DAT_00bcd630)[iVar5] = 9;
            (&DAT_00bcd656)[param_1 * 0xc2] = (short)iVar7;
            *(undefined2 *)(&DAT_00bcd61c + iVar5) = uVar1;
            uVar3 = *(undefined4 *)(&DAT_007999e4 + iVar6);
            *(undefined2 *)(&DAT_00bcd61e + iVar5) = uVar2;
            *(undefined4 *)(&DAT_00bcd620 + iVar5) = uVar3;
            *(undefined2 *)(&DAT_00bcd63c + iVar5) = 0;
            *(undefined2 *)(&DAT_00bcd62e + iVar5) = 0;
            return CONCAT31((int3)((uint)iVar5 >> 8),1);
          }
        }
        return CONCAT31((int3)((uint)iVar5 >> 8),1);
      }
    }
  }
  return uVar4 & 0xffffff00;
}
