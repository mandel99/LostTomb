#include "restore_types.h"
#define DAT_00799ab8 (*(undefined4 *)0x00799ab8u)
#define DAT_00d3b198 (*(undefined *)0x00d3b198u)
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define DAT_00eadd6c (*(undefined *)0x00eadd6cu)
#define DAT_00f53e6c (*(undefined *)0x00f53e6cu)
extern undefined4 FUN_00502780();

void FUN_0040c6b0(void)

{
  int iVar1;
  int iVar2;
  
  if (DAT_00d3b198 == 0) {
    iVar2 = (&DAT_00799ab8)[DAT_00eadd6c * 0x42];
    iVar1 = DAT_00eadd6c * 0x42;
    (&DAT_00799ab8)[iVar1] = iVar2 + -1;
    if (iVar2 + -1 < 1) {
      (&DAT_00799ab8)[iVar1] = 1;
      DAT_00e38dca = 1;
      return;
    }
  }
  else {
    (&DAT_00799ab8)[DAT_00eadd6c * 0x42] = (&DAT_00799ab8)[DAT_00eadd6c * 0x42] + 1;
    iVar2 = FUN_00502780();
    if (DAT_00f53e6c < iVar2 + -1 + (&DAT_00799ab8)[DAT_00eadd6c * 0x42]) {
      iVar2 = FUN_00502780();
      (&DAT_00799ab8)[DAT_00eadd6c * 0x42] = (DAT_00f53e6c - iVar2) + 1;
    }
  }
  DAT_00e38dca = 1;
  return;
}
