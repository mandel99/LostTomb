#include "restore_types.h"
#define DAT_005da2c4 (*(undefined4 *)0x005da2c4u)
#define DAT_005da2c8 (*(undefined4 *)0x005da2c8u)
#define DAT_005da2f4 (*(undefined4 *)0x005da2f4u)
#define DAT_0078d76c (*(undefined4 *)0x0078d76cu)
#define DAT_0078d770 (*(undefined4 *)0x0078d770u)
#define DAT_007999d8 (*(undefined4 *)0x007999d8u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999e4 (*(undefined4 *)0x007999e4u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00acf85c (*(undefined4 *)0x00acf85cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
extern int FUN_0046b9e0();
extern void FUN_0046bda0();

undefined4 FUN_0046b840(int param_1)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  int iVar4;
  int *piVar5;
  
  iVar1 = param_1;
  iVar4 = param_1 * 0x108;
  if ((*(char *)(&DAT_007999dc + param_1 * 0x42) == '\0') &&
     (((&DAT_00acf85c)[*(int *)(&DAT_007999e4 + iVar4)] & 7) < 5)) {
    param_1 = 0;
    piVar5 = &DAT_005da2f4;
    do {
      iVar2 = *piVar5 + *(int *)(&DAT_007999e4 + iVar4);
      if (((*(byte *)(&DAT_00af598c + iVar2) & 8) != 0) &&
         ((iVar2 = (int)*(short *)(&DAT_00b4e6fc + iVar2 * 2), iVar2 == iVar1 ||
          (((((&DAT_007999d8)[iVar2 * 0x108] == '\x01' &&
             (*(char *)((int)&DAT_007999dc + iVar2 * 0x108 + 1) != '\0')) &&
            ((&DAT_007999ea)[iVar2 * 0x84] == (&DAT_007999ea)[iVar1 * 0x84])) &&
           (*(char *)(&DAT_007999dc + iVar2 * 0x42) == '\0')))))) {
        param_1 = param_1 + 1;
      }
      piVar5 = piVar5 + 1;
    } while ((int)piVar5 < 0x5da304);
    if (param_1 == 4) {
      DAT_0078d770 = *(short *)((int)&DAT_007999dc + iVar4 + 2) + DAT_005da2c4;
      DAT_0078d76c = *(short *)(&DAT_007999e0 + iVar4) + DAT_005da2c8;
      uVar3 = FUN_0046b9e0(iVar1,4);
      FUN_0046bda0(iVar1,uVar3);
      return 1;
    }
  }
  return 0;
}
