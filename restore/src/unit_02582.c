#include "restore_types.h"
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a30 (*(undefined4 *)0x00799a30u)
#define DAT_00799a38 (*(undefined4 *)0x00799a38u)
#define DAT_00799a3c (*(undefined4 *)0x00799a3cu)
#define DAT_00799a44 (*(undefined4 *)0x00799a44u)
extern uint FUN_004ea930();
extern undefined4 FUN_00545740();
extern void FUN_005468d0();
extern undefined4 FUN_0054f470();
extern undefined4 FUN_00555e80();
extern undefined4 FUN_00558be0();

void FUN_00547870(int param_1,int param_2)

{
  char cVar1;
  int iVar2;
  int iVar3;
  int *in_ECX;
  int *piVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  
  iVar5 = param_1;
  iVar6 = param_1 * 0x108;
  iVar7 = param_1 - (short)(&DAT_007999ea)[param_1 * 0x84];
  if (*(int *)(&DAT_00799a38 + iVar6) < 4) {
    iVar2 = *(int *)(&DAT_00799a38 + iVar6) + 1;
    *(int *)(&DAT_00799a38 + iVar6) = iVar2;
    if (iVar2 == 3) {
      piVar4 = (int *)(&DAT_00799a44 + param_1 * 0x84);
      param_1 = 6;
      do {
        if (-1 < *piVar4) {
          iVar2 = *piVar4 + iVar7;
          iVar3 = iVar2 * 0x108;
          *(int *)(&DAT_00799a38 + iVar3) = *(int *)(&DAT_00799a38 + iVar3) + 1;
          *(undefined4 *)(&DAT_00799a3c + iVar2 * 0x84) = 1;
        }
        piVar4 = piVar4 + 1;
        param_1 = param_1 + -1;
      } while (param_1 != 0);
    }
    if (*(int *)(&DAT_00799a38 + iVar6) == 4) {
      *(undefined4 *)(&DAT_00799a3c + iVar5 * 0x84) = 0;
    }
    iVar6 = 0;
    if (0 < param_2) {
      do {
        (**(code **)(*in_ECX + 0x18))(iVar6 + iVar7);
        iVar6 = iVar6 + 1;
      } while (iVar6 < param_2);
    }
  }
  if ((*(int *)(&DAT_00799a3c + iVar5 * 0x84) == 0) && (cVar1 = FUN_004ea930(), cVar1 != '\0')) {
    iVar5 = iVar5 - (short)(&DAT_007999ea)[iVar5 * 0x84];
    switch((&DAT_007999e8)[iVar5 * 0x84]) {
    case 0xe5: ;
      iVar6 = FUN_00545740(iVar5,0);
      if (iVar6 == 100) {
        (&DAT_00799a30)[iVar5 * 0x42] = 1;
        FUN_005468d0(iVar5);
        return;
      }
      break;
    case 0xea: ;
      iVar6 = FUN_00558be0(iVar5,0);
      if (iVar6 == 100) {
        (&DAT_00799a30)[iVar5 * 0x42] = 1;
        FUN_005468d0(iVar5);
        return;
      }
      break;
    case 0xeb: ;
      iVar6 = FUN_00555e80(iVar5,0);
      if (iVar6 == 100) {
        (&DAT_00799a30)[iVar5 * 0x42] = 1;
        FUN_005468d0(iVar5);
        return;
      }
      break;
    case 0xec: ;
      iVar6 = FUN_0054f470(iVar5,0);
      if (iVar6 == 100) {
        (&DAT_00799a30)[iVar5 * 0x42] = 1;
        FUN_005468d0(iVar5);
      }
    }
  }
  return;
}
