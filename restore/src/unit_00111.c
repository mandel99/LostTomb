#include "restore_types.h"
#define DAT_007999e4 (*(undefined4 *)0x007999e4u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_007999fe (*(undefined4 *)0x007999feu)
#define DAT_00799a00 (*(undefined4 *)0x00799a00u)
#define DAT_00799a30 (*(undefined4 *)0x00799a30u)
#define DAT_00799a34 (*(undefined4 *)0x00799a34u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd606 (*(undefined4 *)0x00bcd606u)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd620 (*(undefined4 *)0x00bcd620u)
#define DAT_00bcd62e (*(undefined4 *)0x00bcd62eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd63c (*(undefined4 *)0x00bcd63cu)
#define DAT_00bcd656 (*(undefined4 *)0x00bcd656u)
#define DAT_00bcd660 (*(undefined4 *)0x00bcd660u)
#define DAT_00bcd66c (*(undefined4 *)0x00bcd66cu)
#define DAT_00bcd672 (*(undefined4 *)0x00bcd672u)
#define DAT_00d3b4e0 (*(undefined *)0x00d3b4e0u)
#define DAT_00d3b4e4 (*(undefined *)0x00d3b4e4u)
extern int FUN_00420910();
extern int FUN_0045bb50();

uint FUN_0040ed70(int param_1,int param_2,char param_3)

{
  char cVar1;
  undefined2 uVar2;
  undefined2 uVar3;
  undefined4 uVar4;
  uint uVar5;
  undefined3 uVar7;
  int iVar6;
  int *in_ECX;
  int iVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  undefined4 unaff_retaddr;
  
  uVar5 = param_1 * 0x108;
  iVar8 = param_1;
  if (2 < (int)(&DAT_00799a30)[param_1 * 0x42]) {
    iVar8 = param_1 - (short)(&DAT_007999ea)[param_1 * 0x84];
  }
  iVar11 = iVar8 * 0x108;
  if ((-1 < in_ECX[(short)(&DAT_007999ea)[iVar8 * 0x84] * 0xd + 0x16]) &&
     (in_ECX[(short)(&DAT_007999ea)[iVar8 * 0x84] * 0xd + 0x16] < 0x19)) {
    iVar10 = param_2 * 0x184;
    if ((&DAT_00bcd602)[iVar10] == 'U') {
      if ((int)(&DAT_00799a30)[iVar8 * 0x42] < 2) {
        if (*(int *)(&DAT_00799a34 + iVar11) == 0) {
          if (param_3 != '\0') {
            *(int *)(&DAT_00799a34 + iVar11) = param_2;
            uVar2 = (&DAT_007999fe)[param_1 * 0x84];
            (&DAT_00bcd630)[iVar10] = 0x20;
            (&DAT_00bcd656)[param_2 * 0xc2] = (short)iVar8;
            *(undefined2 *)(&DAT_00bcd61c + iVar10) = uVar2;
            uVar5 = *(uint *)(&DAT_007999e4 + uVar5);
            *(undefined2 *)(&DAT_00bcd61e + iVar10) = (&DAT_00799a00)[param_1 * 0x84];
            *(uint *)(&DAT_00bcd620 + iVar10) = uVar5;
            *(undefined2 *)(&DAT_00bcd62e + iVar10) = 0;
            *(undefined2 *)(&DAT_00bcd63c + iVar10) = 0;
          }
          return CONCAT31((int3)(uVar5 >> 8),1);
        }
      }
      else if (2 < (int)(&DAT_00799a30)[iVar8 * 0x42]) {
        uVar5 = (**(code **)(*in_ECX + 0x24))(param_1);
        if ((char)uVar5 != '\0') {
          iVar11 = *(int *)(&DAT_00799a34 + iVar11);
          iVar9 = iVar11 * 0x184;
          if (((&DAT_00bcd602)[iVar9] == 'Q') && ((&DAT_00bcd630)[iVar9] == '\n')) {
            if (*(short *)(&DAT_00bcd660 + iVar9) != 0) {
              uVar5 = *(short *)(&DAT_00bcd660 + iVar9) * 0x184;
              if (((((&DAT_00bcd606)[uVar5] != '\0') && ((&DAT_00bcd602)[uVar5] == 'U')) &&
                  (((cVar1 = (&DAT_00bcd630)[uVar5], cVar1 == '\x06' ||
                    (((cVar1 == '\a' || (cVar1 == '\x0f')) || (cVar1 == '\x10')))) ||
                   (cVar1 == '\x1f')))) && (*(short *)(&DAT_00bcd660 + uVar5) == iVar11))
              goto LAB_0040f057;
              *(undefined2 *)(&DAT_00bcd660 + iVar9) = 0;
            }
            uVar7 = (undefined3)(uVar5 >> 8);
            if ((char)param_2 != '\0') {
              if ((int)(&DAT_00799a30)[iVar8 * 0x42] < 0x10) {
                iVar8 = 0x23;
              }
              else {
                iVar8 = ((0x13 < (int)(&DAT_00799a30)[iVar8 * 0x42]) - 1 & 0xfffffffa) + 0x23;
              }
              iVar6 = FUN_0045bb50(unaff_retaddr,iVar8,1);
              if (iVar6 != 0) {
                (&DAT_00bcd672)[iVar10] = (char)iVar8;
                *(undefined2 *)(&DAT_00bcd66c + iVar10) = 0;
                uVar2 = DAT_00d3b4e0;
                (&DAT_00bcd656)[param_2 * 0xc2] = (short)iVar6;
                *(short *)(&DAT_00bcd660 + iVar10) = (short)iVar11;
                (&DAT_00bcd630)[iVar10] = 0xf;
                *(undefined2 *)(&DAT_00bcd62e + iVar10) = 0;
                *(undefined2 *)(&DAT_00bcd61c + iVar10) = DAT_00d3b4e4;
                *(undefined2 *)(&DAT_00bcd61e + iVar10) = uVar2;
                *(undefined2 *)(&DAT_00bcd660 + iVar9) = (undefined2)param_1;
                return CONCAT31((int3)(CONCAT22((short)((uint)iVar6 >> 0x10),(undefined2)param_1) >>
                                      8),1);
              }
              (&DAT_00bcd606)[iVar10] = 2;
              uVar7 = 0;
            }
            return CONCAT31(uVar7,1);
          }
        }
      }
    }
    else {
      uVar5 = (**(code **)(*in_ECX + 0x24))(param_1);
      if (((char)uVar5 == '\0') && (2 < (int)(&DAT_00799a30)[iVar8 * 0x42])) {
        iVar9 = FUN_00420910(unaff_retaddr,param_2);
        uVar5 = 0;
        if (iVar9 != 0) {
          uVar7 = (undefined3)((uint)iVar9 >> 8);
          if ((char)param_2 != '\0') {
            if (((&DAT_00bcd602)[iVar10] == 'O') || ((&DAT_00bcd602)[iVar10] == 'Q')) {
              (&DAT_00bcd630)[iVar10] = 9;
            }
            *(int *)(&DAT_00799a34 + iVar11) = param_2;
            uVar2 = (&DAT_007999fe)[iVar8 * 0x84];
            uVar3 = (&DAT_00799a00)[iVar8 * 0x84];
            uVar4 = *(undefined4 *)(&DAT_007999e4 + iVar11);
            (&DAT_00bcd656)[param_2 * 0xc2] = (short)iVar8 + 0xf;
            *(undefined2 *)(&DAT_00bcd61c + iVar10) = uVar2;
            *(undefined2 *)(&DAT_00bcd61e + iVar10) = uVar3;
            uVar7 = 0;
            *(undefined4 *)(&DAT_00bcd620 + iVar10) = uVar4;
            *(undefined2 *)(&DAT_00bcd63c + iVar10) = 0;
            *(undefined2 *)(&DAT_00bcd62e + iVar10) = 0;
          }
          return CONCAT31(uVar7,1);
        }
      }
    }
  }
LAB_0040f057: ;
  return uVar5 & 0xffffff00;
}
