#include "restore_types.h"
#define DAT_0078d83c (*(undefined *)0x0078d83cu)
#define DAT_0078d840 (*(undefined *)0x0078d840u)
#define DAT_0078e178 (*(undefined4 *)0x0078e178u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_007999f4 (*(undefined4 *)0x007999f4u)
#define DAT_00ea3f94 (*(undefined4 *)0x00ea3f94u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern void FUN_00458d80();

void FUN_00458b60(int param_1)

{
  short sVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  iVar3 = 0;
  iVar5 = param_1;
  do {
    if ((iVar5 < 1) || (9 < iVar3)) {
      *(int *)(&DAT_00ea3f94 + DAT_00eaac28 * 0x49d8) =
           *(int *)(&DAT_00ea3f94 + DAT_00eaac28 * 0x49d8) + (param_1 - iVar5);
      return;
    }
    DAT_0078d840 = 0;
    if (0 < DAT_0078d83c) {
      do {
        if (iVar5 < 1) break;
        iVar2 = (int)(short)(&DAT_0078e178)[DAT_0078d840];
        DAT_0078d840 = DAT_0078d840 + 1;
        sVar1 = (&DAT_007999f4)[iVar2 * 0x84];
        if ((0 < sVar1) && ((short)(&DAT_007999ea)[iVar2 * 0x84] == iVar3)) {
          iVar4 = (int)sVar1;
          if (3 < sVar1) {
            iVar4 = 4;
          }
          if (iVar4 < iVar5) {
            FUN_00458d80(iVar2,iVar4);
            iVar5 = iVar5 - iVar4;
          }
          else {
            FUN_00458d80(iVar2,iVar5);
            iVar5 = 0;
          }
        }
      } while (DAT_0078d840 < DAT_0078d83c);
    }
    iVar3 = iVar3 + 1;
  } while( true );
}
