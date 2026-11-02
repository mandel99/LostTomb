#include "restore_types.h"
#define DAT_005da2f4 (*(undefined4 *)0x005da2f4u)
#define DAT_0078d76c (*(undefined4 *)0x0078d76cu)
#define DAT_0078d770 (*(undefined4 *)0x0078d770u)
#define DAT_007999db (*(undefined4 *)0x007999dbu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
extern void FUN_0046be90();
extern void FUN_0046c1c0();
extern void FUN_0046c500();

void FUN_0046b940(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  
  iVar2 = DAT_0078d770 + DAT_0078d76c * 0xe4 + DAT_00bca2d8;
  if (0 < param_2) {
    piVar3 = &DAT_005da2f4;
    do {
      iVar1 = *piVar3 + iVar2;
      if ((((*(byte *)(&DAT_00af598c + iVar1) & 8) != 0) &&
          (iVar1 = (int)*(short *)(&DAT_00b4e6fc + iVar1 * 2), iVar1 != param_1)) &&
         (*(char *)((int)&DAT_007999dc + iVar1 * 0x108 + 1) != '\0')) {
        if (*(char *)(&DAT_007999dc + iVar1 * 0x42) == '\x01') {
          FUN_0046be90(iVar1);
        }
        else if ((&DAT_007999db)[iVar1 * 0x108] == '\x02') {
          FUN_0046c1c0(iVar1);
        }
        else if ((&DAT_007999db)[iVar1 * 0x108] == '\x03') {
          FUN_0046c500(iVar1);
        }
      }
      piVar3 = piVar3 + 1;
      param_2 = param_2 + -1;
    } while (param_2 != 0);
  }
  return;
}
