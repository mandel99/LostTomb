#include "restore_types.h"
#define DAT_0070ce14 (*(undefined4 *)0x0070ce14u)
#define DAT_0078d83c (*(undefined *)0x0078d83cu)
#define DAT_0078d840 (*(undefined *)0x0078d840u)
#define DAT_0078e178 (*(undefined4 *)0x0078e178u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_007999f4 (*(undefined4 *)0x007999f4u)
#define DAT_007999f6 (*(undefined4 *)0x007999f6u)
#define DAT_007999f8 (*(undefined4 *)0x007999f8u)
#define DAT_007999fa (*(undefined4 *)0x007999fau)
#define DAT_00799a5f (*(undefined4 *)0x00799a5fu)
#define DAT_00ea18a4 (*(undefined4 *)0x00ea18a4u)
#define DAT_00ea18a8 (*(undefined4 *)0x00ea18a8u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern void FUN_004583f0();
extern void FUN_004e8610();

void FUN_00458440(void)

{
  short sVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  short *psVar5;
  int local_4;
  
  FUN_004e8610();
  FUN_004583f0();
  DAT_0078d840 = 0;
  if (0 < DAT_0078d83c) {
    psVar5 = &DAT_0078e178;
    local_4 = DAT_0078d83c;
    DAT_0078d840 = DAT_0078d83c;
    do {
      iVar2 = (int)*psVar5;
      psVar5 = psVar5 + 1;
      (&DAT_007999f6)[iVar2 * 0x84] = 0;
      if (0 < (short)(&DAT_007999f8)[iVar2 * 0x84]) {
        iVar4 = *(int *)(&DAT_0070ce14 + (short)(&DAT_007999ea)[iVar2 * 0x84] * 0x58);
        if (*(char *)(&DAT_007999dc + iVar2 * 0x42) != '\0') {
          iVar4 = iVar4 << 2;
        }
        sVar1 = (&DAT_007999f4)[iVar2 * 0x84];
        iVar3 = DAT_00eaac28 * 0x49d8;
        *(int *)(&DAT_00ea18a4 + iVar3) = *(int *)(&DAT_00ea18a4 + iVar3) + iVar4;
        *(int *)(&DAT_00ea18a8 + iVar3) = *(int *)(&DAT_00ea18a8 + iVar3) + (iVar4 - sVar1);
        if ((&DAT_00799a5f)[iVar2 * 0x108] == '\0') {
          (&DAT_007999f6)[iVar2 * 0x84] = (short)iVar4 - sVar1;
        }
        if ((short)(&DAT_007999fa)[iVar2 * 0x84] < (short)(&DAT_007999f4)[iVar2 * 0x84]) {
          (&DAT_007999fa)[iVar2 * 0x84] = (&DAT_007999f4)[iVar2 * 0x84];
        }
      }
      local_4 = local_4 + -1;
    } while (local_4 != 0);
  }
  return;
}
