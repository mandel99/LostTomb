#include "restore_types.h"
#define DAT_0078d83c (*(undefined *)0x0078d83cu)
#define DAT_0078d840 (*(undefined *)0x0078d840u)
#define DAT_0078e178 (*(undefined4 *)0x0078e178u)
#define DAT_007999d8 (*(undefined4 *)0x007999d8u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999f4 (*(undefined4 *)0x007999f4u)
#define DAT_007999f6 (*(undefined4 *)0x007999f6u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern void FUN_00459020();

void FUN_00459070(void)

{
  short sVar1;
  int iVar2;
  int iVar3;
  
  iVar3 = 0;
  DAT_0078d840 = 0;
  if (0 < DAT_0078d83c) {
    do {
      iVar2 = (int)(short)(&DAT_0078e178)[iVar3];
      DAT_0078d840 = iVar3 + 1;
      iVar3 = iVar2 * 0x108;
      sVar1 = (&DAT_007999f6)[iVar2 * 0x84];
      if (sVar1 < 0) {
        FUN_00459020(DAT_00eaac28,(int)*(short *)((int)&DAT_007999dc + iVar3 + 2),
                     (int)*(short *)(&DAT_007999e0 + iVar3),-(int)sVar1);
        if (-(int)sVar1 < (int)(short)(&DAT_007999f4)[iVar2 * 0x84]) {
          (&DAT_007999f4)[iVar2 * 0x84] = (&DAT_007999f4)[iVar2 * 0x84] + sVar1;
        }
        else {
          (&DAT_007999d8)[iVar3] = 2;
        }
      }
      iVar3 = DAT_0078d840;
    } while (DAT_0078d840 < DAT_0078d83c);
  }
  return;
}
