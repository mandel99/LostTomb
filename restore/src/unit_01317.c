#include "restore_types.h"
#define DAT_00799b98 (*(undefined4 *)0x00799b98u)
#define DAT_0096e6b0 (*(undefined4 *)0x0096e6b0u)
#define DAT_0096e6b4 (*(undefined4 *)0x0096e6b4u)
#define DAT_00972530 (*(undefined4 *)0x00972530u)
#define DAT_00972534 (*(undefined4 *)0x00972534u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00ea187c (*(undefined4 *)0x00ea187cu)
#define DAT_00ea5cc0 (*(undefined4 *)0x00ea5cc0u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern undefined4 FUN_00459ec0();
extern undefined4 FUN_00459ef0();
extern undefined4 FUN_00459ff0();
extern int FUN_004b28f0();

undefined4 FUN_00492650(undefined4 param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int *piVar6;
  int iVar7;
  
  iVar5 = 0;
  iVar4 = (&DAT_00ea5cc0)[DAT_00eaac28 * 0x1276];
  DAT_0096e6b0 = 0;
  iVar1 = *(int *)(&DAT_00ea187c + DAT_00eaac28 * 0x49d8);
  if ((0 < iVar4) || (0 < iVar1)) {
    iVar7 = 1;
    piVar6 = &DAT_00799b98;
    do {
      if (((((char)piVar6[-0x2e] != '\0') &&
           (iVar2 = FUN_00459ff0(iVar7), iVar5 = DAT_0096e6b0, iVar2 != 0)) &&
          ((iVar5 = FUN_00459ec0(iVar7), iVar5 == 0 || (iVar5 = DAT_0096e6b0, 0 < iVar4)))) &&
         (((iVar3 = FUN_00459ef0(iVar7), iVar2 = DAT_00bca2d8, iVar5 = DAT_0096e6b0, iVar3 == 0 ||
           (0 < iVar1)) && (0 < *piVar6)))) {
        iVar3 = piVar6[-0x24];
        (&DAT_00972534)[DAT_0096e6b0] = iVar7;
        iVar5 = DAT_0096e6b0 + 1;
        DAT_0096e6b0 = iVar5;
        (&DAT_0096e6b0)[iVar5] = iVar2 + *(short *)((int)piVar6 + -0x92) + (short)iVar3 * 0xe4;
      }
      piVar6 = piVar6 + 0x42;
      iVar7 = iVar7 + 1;
    } while ((int)piVar6 < 0x89b790);
    if ((iVar5 != 0) && (iVar4 = FUN_004b28f0(param_1,&DAT_0096e6b4,iVar5), 0 < iVar4)) {
      return (&DAT_00972530)[iVar4];
    }
  }
  return 0;
}
