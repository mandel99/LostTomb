#include "restore_types.h"
#define DAT_005da2c4 (*(undefined4 *)0x005da2c4u)
#define DAT_005da2c8 (*(undefined4 *)0x005da2c8u)
#define DAT_005da2e4 (*(undefined4 *)0x005da2e4u)
#define DAT_005da2f4 (*(undefined4 *)0x005da2f4u)
#define DAT_0078d76c (*(undefined4 *)0x0078d76cu)
#define DAT_0078d770 (*(undefined4 *)0x0078d770u)
#define DAT_007999d8 (*(undefined4 *)0x007999d8u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999e4 (*(undefined4 *)0x007999e4u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a56 (*(undefined4 *)0x00799a56u)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)

undefined4 FUN_0046b480(int param_1,int param_2)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int *piVar6;
  int iVar7;
  int *piVar8;
  int local_c;
  
  iVar5 = param_1 * 0x108;
  local_c = 0;
  iVar1 = *(int *)(&DAT_007999e4 + iVar5);
  piVar6 = &DAT_005da2e4;
  do {
    iVar3 = 0;
    if (0 < param_2) {
      piVar8 = &DAT_005da2f4;
      iVar7 = param_2;
      do {
        iVar4 = *piVar8 + *piVar6 + iVar1;
        if (((*(byte *)(&DAT_00af598c + iVar4) & 8) != 0) &&
           ((iVar4 = (int)*(short *)(&DAT_00b4e6fc + iVar4 * 2), iVar4 == param_1 ||
            ((((&DAT_007999d8)[iVar4 * 0x108] == '\x01' &&
              (*(char *)((int)&DAT_007999dc + iVar4 * 0x108 + 1) != '\0')) &&
             ((short)(&DAT_007999ea)[iVar4 * 0x84] <= (short)(&DAT_007999ea)[param_1 * 0x84])))))) {
          iVar3 = iVar3 + 1;
        }
        piVar8 = piVar8 + 1;
        iVar7 = iVar7 + -1;
      } while (iVar7 != 0);
    }
    if (iVar3 == param_2) {
      DAT_0078d770 = (int)*(short *)((int)&DAT_007999dc + iVar5 + 2) + (&DAT_005da2c4)[local_c * 2];
      DAT_0078d76c = (int)*(short *)(&DAT_007999e0 + iVar5) + (&DAT_005da2c8)[local_c * 2];
      return 1;
    }
    piVar6 = piVar6 + 1;
    local_c = local_c + 1;
  } while ((int)piVar6 < 0x5da2f4);
  local_c = 0;
  piVar6 = &DAT_005da2e4;
  do {
    iVar3 = 0;
    if (0 < param_2) {
      piVar8 = &DAT_005da2f4;
      iVar7 = param_2;
      do {
        iVar4 = *piVar8 + iVar1 + *piVar6;
        if (((&DAT_00af598c)[iVar4] & 0xeefed77f) == 0) {
          iVar3 = iVar3 + 1;
        }
        if ((((&DAT_00af598c)[iVar4] & 8) != 0) &&
           ((iVar4 = (int)*(short *)(&DAT_00b4e6fc + iVar4 * 2), iVar4 == param_1 ||
            ((((&DAT_007999d8)[iVar4 * 0x108] == '\x01' &&
              (*(char *)((int)&DAT_007999dc + iVar4 * 0x108 + 1) != '\0')) &&
             ((short)(&DAT_007999ea)[iVar4 * 0x84] <= (short)(&DAT_007999ea)[param_1 * 0x84])))))) {
          iVar3 = iVar3 + 1;
        }
        piVar8 = piVar8 + 1;
        iVar7 = iVar7 + -1;
      } while (iVar7 != 0);
    }
    if (iVar3 == param_2) goto LAB_0046b6f0;
    piVar6 = piVar6 + 1;
    local_c = local_c + 1;
  } while ((int)piVar6 < 0x5da2f4);
  local_c = 0;
  piVar6 = &DAT_005da2e4;
  do {
    iVar3 = 0;
    if (0 < param_2) {
      piVar8 = &DAT_005da2f4;
      iVar7 = param_2;
      do {
        iVar4 = *piVar8 + iVar1 + *piVar6;
        uVar2 = (&DAT_00af598c)[iVar4];
        if ((uVar2 & 0xeefed77f) == 0) {
          iVar3 = iVar3 + 1;
        }
        if ((uVar2 & 8) == 0) {
          if ((uVar2 & 0x20) != 0) goto LAB_0046b6b0;
        }
        else {
          iVar4 = (int)*(short *)(&DAT_00b4e6fc + iVar4 * 2);
          if ((iVar4 == param_1) ||
             ((((&DAT_007999d8)[iVar4 * 0x108] == '\x01' &&
               (*(char *)((int)&DAT_007999dc + iVar4 * 0x108 + 1) != '\0')) &&
              ((short)(&DAT_007999ea)[iVar4 * 0x84] <= (short)(&DAT_007999ea)[param_1 * 0x84])))) {
LAB_0046b6b0: ;
            iVar3 = iVar3 + 1;
          }
        }
        piVar8 = piVar8 + 1;
        iVar7 = iVar7 + -1;
      } while (iVar7 != 0);
    }
    if (iVar3 == param_2) {
LAB_0046b6f0: ;
      DAT_0078d770 = (int)*(short *)((int)&DAT_007999dc + iVar5 + 2) + (&DAT_005da2c4)[local_c * 2];
      DAT_0078d76c = (int)*(short *)(&DAT_007999e0 + iVar5) + (&DAT_005da2c8)[local_c * 2];
      return 1;
    }
    piVar6 = piVar6 + 1;
    local_c = local_c + 1;
    if (0x5da2f3 < (int)piVar6) {
      (&DAT_00799a56)[iVar5] = 1;
      return 0;
    }
  } while( true );
}
