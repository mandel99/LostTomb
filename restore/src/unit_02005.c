#include "restore_types.h"
#define DAT_007999e4 (*(undefined4 *)0x007999e4u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_007999fe (*(undefined4 *)0x007999feu)
#define DAT_00799a00 (*(undefined4 *)0x00799a00u)
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
extern int FUN_00421230();
extern int FUN_0045bb50();

undefined4 FUN_004e6c10(int param_1,int param_2)

{
  undefined4 uVar1;
  undefined2 uVar2;
  char cVar3;
  int iVar4;
  int iVar5;
  int *in_ECX;
  int iVar6;
  int iVar7;
  
  if (in_ECX[(short)(&DAT_007999ea)[param_1 * 0x84] * 0xd + 0x16] == 0) {
    iVar7 = param_2 * 0x184;
    if ((&DAT_00bcd602)[iVar7] == 'U') {
      cVar3 = (**(code **)(*in_ECX + 0x24))();
      if (cVar3 != '\0') {
        iVar5 = *(int *)(&DAT_00799a34 + param_1 * 0x108);
        iVar6 = iVar5 * 0x184;
        if (((&DAT_00bcd602)[iVar6] == 'Q') && ((&DAT_00bcd630)[iVar6] == '\n')) {
          if (*(short *)(&DAT_00bcd660 + iVar6) != 0) {
            iVar4 = *(short *)(&DAT_00bcd660 + iVar6) * 0x184;
            if (((((&DAT_00bcd606)[iVar4] != '\0') && ((&DAT_00bcd602)[iVar4] == 'U')) &&
                (((cVar3 = (&DAT_00bcd630)[iVar4], cVar3 == '\x06' ||
                  (((cVar3 == '\a' || (cVar3 == '\x0f')) || (cVar3 == '\x10')))) || (cVar3 == '#')))
                ) && (*(short *)(&DAT_00bcd660 + iVar4) == iVar5)) {
              return 0;
            }
            *(undefined2 *)(&DAT_00bcd660 + iVar6) = 0;
          }
          if ((char)param_2 == '\0') {
            return 1;
          }
          iVar4 = FUN_0045bb50(param_1,0x23,1);
          uVar2 = DAT_00d3b4e0;
          if (iVar4 == 0) {
            (&DAT_00bcd606)[iVar7] = 2;
            return 1;
          }
          (&DAT_00bcd672)[iVar7] = 0x23;
          *(undefined2 *)(&DAT_00bcd66c + iVar7) = 0;
          (&DAT_00bcd656)[param_2 * 0xc2] = (short)iVar4;
          *(short *)(&DAT_00bcd660 + iVar7) = (short)iVar5;
          (&DAT_00bcd630)[iVar7] = 0xf;
          *(undefined2 *)(&DAT_00bcd62e + iVar7) = 0;
          *(undefined2 *)(&DAT_00bcd61c + iVar7) = DAT_00d3b4e4;
          *(undefined2 *)(&DAT_00bcd61e + iVar7) = uVar2;
          *(undefined2 *)(&DAT_00bcd660 + iVar6) = (undefined2)param_1;
          return 1;
        }
      }
    }
    else {
      cVar3 = (**(code **)(*in_ECX + 0x24))(param_1);
      if ((cVar3 == '\0') && (iVar5 = FUN_00421230(param_1,param_2), iVar5 != 0)) {
        if ((char)param_2 == '\0') {
          return 1;
        }
        if (((&DAT_00bcd602)[iVar7] == 'O') || ((&DAT_00bcd602)[iVar7] == 'Q')) {
          (&DAT_00bcd630)[iVar7] = 9;
        }
        *(int *)(&DAT_00799a34 + iVar5 * 0x108) = param_2;
        *(undefined2 *)(&DAT_00bcd61c + iVar7) = (&DAT_007999fe)[iVar5 * 0x84];
        uVar1 = *(undefined4 *)(&DAT_007999e4 + iVar5 * 0x108);
        *(undefined2 *)(&DAT_00bcd61e + iVar7) = (&DAT_00799a00)[iVar5 * 0x84];
        *(undefined4 *)(&DAT_00bcd620 + iVar7) = uVar1;
        *(undefined2 *)(&DAT_00bcd63c + iVar7) = 0;
        *(undefined2 *)(&DAT_00bcd62e + iVar7) = 0;
        return 1;
      }
    }
  }
  return 0;
}
