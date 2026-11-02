#include "restore_types.h"
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00a9cc1c (*(undefined4 *)0x00a9cc1cu)
#define DAT_00adc36c (*(undefined4 *)0x00adc36cu)
#define DAT_00ae8e7c (*(undefined4 *)0x00ae8e7cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b285cc (*(undefined4 *)0x00b285ccu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00b67d1c (*(undefined4 *)0x00b67d1cu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)

void FUN_00546660(int param_1,int param_2)

{
  int *piVar1;
  byte bVar2;
  short sVar3;
  short sVar4;
  short sVar5;
  int iVar6;
  int in_ECX;
  int iVar7;
  int *piVar8;
  int iVar9;
  int local_10;
  
  sVar3 = (&DAT_007999ea)[param_1 * 0x84];
  sVar4 = *(short *)((int)&DAT_007999dc + param_1 * 0x108 + 2);
  sVar5 = *(short *)(&DAT_007999e0 + param_1 * 0x108);
  local_10 = 0;
  piVar1 = (int *)(param_2 + 8 + sVar3 * 0x34);
  iVar7 = *piVar1;
  if (0 < iVar7) {
    do {
      iVar9 = 0;
      if (0 < iVar7) {
        iVar6 = (int)sVar4 + DAT_00bca2d8 + sVar5 * 0xe4 + local_10;
        iVar7 = iVar6 * 2;
        piVar8 = &DAT_00af598c + iVar6;
        do {
          bVar2 = (&DAT_00b285cc)[iVar6];
          *(undefined2 *)(&DAT_00b4e6fc + iVar7) = 0;
          *piVar8 = 0;
          (&DAT_00b285cc)[iVar6] = bVar2 & 0xe0;
          bVar2 = (&DAT_00b67d1c)[iVar6];
          (&DAT_00ae8e7c)[iVar6] = 0;
          *(undefined2 *)(&DAT_00a9cc1c + iVar7) = 0;
          (&DAT_00b67d1c)[iVar6] = bVar2 & 0xc0 | 0x40;
          (&DAT_00adc36c)[iVar6] = 0;
          iVar7 = iVar7 + 0x1c8;
          iVar9 = iVar9 + 1;
          *piVar8 = (-(uint)((int)sVar3 != *(int *)(in_ECX + 0x50)) & 0x1fffff82) + 0x80;
          iVar6 = iVar6 + 0xe4;
          piVar8 = piVar8 + 0xe4;
        } while (iVar9 < *piVar1);
      }
      iVar7 = *piVar1;
      local_10 = local_10 + 1;
    } while (local_10 < iVar7);
  }
  return;
}
