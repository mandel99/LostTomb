#include "restore_types.h"
#define DAT_00705854 (*(undefined *)0x00705854u)
#define DAT_0070cdf4 (*(undefined4 *)0x0070cdf4u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_007999f4 (*(undefined4 *)0x007999f4u)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
extern int FUN_004cde60();

int FUN_004342c0(int param_1)

{
  short sVar1;
  short *psVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  sVar1 = (&DAT_007999e8)[param_1 * 0x84];
  if ((((sVar1 != 0x46) && (sVar1 != 0xbe)) && (sVar1 != 0x4c)) &&
     (((sVar1 != 0x47 && (sVar1 != 0xbf)) &&
      ((*(char *)((int)&DAT_007999dc + param_1 * 0x108 + 1) != '\0' && (DAT_00705854 != 0)))))) {
    iVar5 = 0;
    if (*(int *)(&DAT_0070cdf4 + (short)(&DAT_007999ea)[param_1 * 0x84] * 0x58) != 0) {
      iVar5 = (int)(short)(&DAT_007999f4)[param_1 * 0x84];
    }
    iVar3 = 0;
    psVar2 = &DAT_00799a2c + param_1 * 0x84;
    iVar4 = 8;
    do {
      iVar3 = iVar3 + *psVar2;
      psVar2 = psVar2 + 1;
      iVar4 = iVar4 + -1;
    } while (iVar4 != 0);
    if (iVar5 != 0) {
      iVar5 = FUN_004cde60(iVar3,iVar5);
      if (iVar5 < 0xc9) {
        if (99 < iVar5) {
          return 1;
        }
        return ((iVar5 < 1) - 1 & 0xfffffffb) + 10;
      }
    }
    return 0;
  }
  return 0;
}
