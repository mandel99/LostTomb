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
extern int FUN_00420f50();
extern int FUN_0045bb50();

undefined4 FUN_004176a0(int param_1,int param_2)

{
  undefined2 uVar1;
  undefined4 uVar2;
  undefined2 uVar3;
  char cVar4;
  int iVar5;
  int iVar6;
  int *in_ECX;
  int iVar7;
  int iVar8;
  int iVar9;
  int unaff_retaddr;
  
  iVar6 = param_1;
  iVar8 = param_1 * 0x108;
  iVar7 = param_1 * 0x84;
  RESTORE_SLICE2(param_1,0) = (short)param_1 - (&DAT_007999ea)[iVar7];
  if (in_ECX[(short)(&DAT_007999ea)[iVar7] * 0xd + 0x16] == 0) {
    iVar7 = param_2 * 0x184;
    if ((&DAT_00bcd602)[iVar7] == 'U') {
      cVar4 = (**(code **)(*in_ECX + 0x24))(iVar6);
      if (cVar4 != '\0') {
        iVar8 = *(int *)(&DAT_00799a34 + iVar8);
        iVar9 = iVar8 * 0x184;
        if (((&DAT_00bcd602)[iVar9] == 'Q') && ((&DAT_00bcd630)[iVar9] == '\n')) {
          if (*(short *)(&DAT_00bcd660 + iVar9) != 0) {
            iVar5 = *(short *)(&DAT_00bcd660 + iVar9) * 0x184;
            if (((((&DAT_00bcd606)[iVar5] != '\0') && ((&DAT_00bcd602)[iVar5] == 'U')) &&
                (((cVar4 = (&DAT_00bcd630)[iVar5], cVar4 == '\x06' ||
                  (((cVar4 == '\a' || (cVar4 == '\x0f')) || (cVar4 == '\x10')))) || (cVar4 == '\"'))
                )) && (*(short *)(&DAT_00bcd660 + iVar5) == iVar8)) {
              return 0;
            }
            *(undefined2 *)(&DAT_00bcd660 + iVar9) = 0;
          }
          if ((char)param_2 == '\0') {
            return 1;
          }
          cVar4 = (-((&DAT_00799a30)[unaff_retaddr * 0x42] != 0x12) & 9U) + 0x1a;
          iVar6 = FUN_0045bb50(iVar6,cVar4,1);
          if (iVar6 == 0) {
            (&DAT_00bcd606)[iVar7] = 2;
            return 1;
          }
          (&DAT_00bcd672)[iVar7] = cVar4;
          *(undefined2 *)(&DAT_00bcd66c + iVar7) = 0;
          (&DAT_00bcd656)[param_2 * 0xc2] = (short)iVar6;
          uVar3 = DAT_00d3b4e4;
          *(short *)(&DAT_00bcd660 + iVar7) = (short)iVar8;
          (&DAT_00bcd630)[iVar7] = 0xf;
          *(undefined2 *)(&DAT_00bcd62e + iVar7) = 0;
          uVar1 = DAT_00d3b4e0;
          *(undefined2 *)(&DAT_00bcd61c + iVar7) = uVar3;
          *(undefined2 *)(&DAT_00bcd61e + iVar7) = uVar1;
          *(short *)(&DAT_00bcd660 + iVar9) = (short)param_1;
          return 1;
        }
      }
    }
    else {
      cVar4 = (**(code **)(*in_ECX + 0x24))(iVar6);
      if ((cVar4 == '\0') && (iVar6 = FUN_00420f50(iVar6,param_2), iVar6 != 0)) {
        if ((char)param_2 == '\0') {
          return 1;
        }
        if (((&DAT_00bcd602)[iVar7] == 'O') || ((&DAT_00bcd602)[iVar7] == 'Q')) {
          (&DAT_00bcd630)[iVar7] = 9;
        }
        *(int *)(&DAT_00799a34 + iVar6 * 0x108) = param_2;
        uVar1 = (&DAT_00799a00)[iVar6 * 0x84];
        uVar2 = *(undefined4 *)(&DAT_007999e4 + iVar6 * 0x108);
        *(undefined2 *)(&DAT_00bcd61c + iVar7) = (&DAT_007999fe)[iVar6 * 0x84];
        *(undefined2 *)(&DAT_00bcd61e + iVar7) = uVar1;
        *(undefined4 *)(&DAT_00bcd620 + iVar7) = uVar2;
        *(undefined2 *)(&DAT_00bcd63c + iVar7) = 0;
        *(undefined2 *)(&DAT_00bcd62e + iVar7) = 0;
        return 1;
      }
    }
  }
  return 0;
}
