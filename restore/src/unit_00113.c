#include "restore_types.h"
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a00 (*(undefined4 *)0x00799a00u)
#define DAT_00799a30 (*(undefined4 *)0x00799a30u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd62e (*(undefined4 *)0x00bcd62eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
extern void FUN_0040ecf0();
extern void FUN_004207e0();

void FUN_0040f0d0(int param_1,int param_2,undefined4 param_3)

{
  bool bVar1;
  int iVar2;
  int iVar3;
  int *in_ECX;
  int *piVar4;
  undefined2 unaff_BX;
  undefined2 unaff_BP;
  undefined2 *puVar5;
  int iVar6;
  
  iVar6 = param_1;
  iVar2 = param_1 - (short)(&DAT_007999ea)[param_1 * 0x84];
  bVar1 = true;
  iVar3 = iVar2 + 0x19;
  if (iVar2 < iVar3) {
    piVar4 = &DAT_00799a30 + iVar2 * 0x42;
    do {
      if (!bVar1) goto LAB_0040f128;
      if (*piVar4 < 2) {
        bVar1 = false;
      }
      iVar2 = iVar2 + 1;
      piVar4 = piVar4 + 0x42;
    } while (iVar2 < iVar3);
    if (!bVar1) {
LAB_0040f128: ;
      iVar3 = param_2 * 0x184;
      if (((&DAT_00bcd602)[iVar3] != 'U') || (iVar2 = (&DAT_00799a30)[param_1 * 0x42], 1 < iVar2)) {
        (&DAT_00bcd630)[iVar3] = (undefined1)param_3;
        return;
      }
      *(short *)(&DAT_00bcd62e + iVar3) = *(short *)(&DAT_00bcd62e + iVar3) + 1;
      if (*(short *)(&DAT_00bcd62e + iVar3) < 0x65) {
        return;
      }
      *(undefined2 *)(&DAT_00bcd62e + iVar3) = 0;
      (&DAT_00bcd630)[iVar3] = 8;
      (&DAT_00799a30)[param_1 * 0x42] = iVar2 + 1;
      (**(code **)(*in_ECX + 0x18))(param_1);
      iVar6 = iVar6 - (short)(&DAT_007999ea)[iVar6 * 0x84];
      bVar1 = true;
      iVar3 = iVar6 + 0x19;
      if (iVar6 < iVar3) {
        piVar4 = &DAT_00799a30 + iVar6 * 0x42;
        iVar2 = iVar6;
        do {
          if (!bVar1) {
            return;
          }
          if (*piVar4 < 2) {
            bVar1 = false;
          }
          iVar2 = iVar2 + 1;
          piVar4 = piVar4 + 0x42;
        } while (iVar2 < iVar3);
        if (!bVar1) {
          return;
        }
      }
      FUN_0040ecf0(iVar6);
      if (iVar3 <= iVar6) {
        return;
      }
      puVar5 = &DAT_00799a00 + iVar6 * 0x84;
      do {
        (**(code **)(*in_ECX + 0x20))(iVar6,&param_2,&param_1);
        puVar5[-1] = unaff_BX;
        *puVar5 = unaff_BP;
        (**(code **)(*in_ECX + 0x18))(iVar6);
        iVar6 = iVar6 + 1;
        puVar5 = puVar5 + 0x84;
      } while (iVar6 < iVar3);
      return;
    }
  }
  FUN_004207e0(param_1,param_2,param_3);
  return;
}
