#include "restore_types.h"
#define DAT_0078d83c (*(undefined *)0x0078d83cu)
#define DAT_0078d840 (*(undefined *)0x0078d840u)
#define DAT_0078e178 (*(undefined4 *)0x0078e178u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_007999f4 (*(undefined4 *)0x007999f4u)
#define DAT_00ea4390 (*(undefined4 *)0x00ea4390u)
#define DAT_00ea4394 (*(undefined4 *)0x00ea4394u)
#define DAT_00ea4398 (*(undefined4 *)0x00ea4398u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern int FUN_004cde40();
extern int FUN_004cde60();
extern int FUN_004e9460();

void FUN_00458880(void)

{
  short sVar1;
  short sVar2;
  undefined4 uVar3;
  short *psVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  
  iVar6 = 0;
  iVar7 = 0;
  DAT_0078d840 = 0;
  if (0 < DAT_0078d83c) {
    psVar4 = &DAT_0078e178;
    DAT_0078d840 = DAT_0078d83c;
    iVar5 = DAT_0078d83c;
    do {
      sVar1 = *psVar4;
      psVar4 = psVar4 + 1;
      sVar2 = (&DAT_007999f4)[sVar1 * 0x84];
      if (0 < sVar2) {
        if ((short)(&DAT_007999ea)[sVar1 * 0x84] < 0xe) {
          iVar6 = iVar6 + sVar2;
        }
        else {
          iVar7 = iVar7 + sVar2;
        }
      }
      iVar5 = iVar5 + -1;
    } while (iVar5 != 0);
  }
  uVar3 = FUN_004cde60(iVar6,iVar7 + iVar6);
  (&DAT_00ea4390)[DAT_00eaac28 * 0x1276] = uVar3;
  uVar3 = FUN_004e9460();
  (&DAT_00ea4394)[DAT_00eaac28 * 0x1276] = uVar3;
  uVar3 = FUN_004cde40(uVar3,0x3c);
  iVar6 = DAT_00eaac28;
  (&DAT_00ea4394)[DAT_00eaac28 * 0x1276] = uVar3;
  uVar3 = FUN_004cde40(uVar3,(&DAT_00ea4390)[iVar6 * 0x1276]);
  (&DAT_00ea4398)[DAT_00eaac28 * 0x1276] = uVar3;
  return;
}
