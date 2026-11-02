#include "restore_types.h"
#define DAT_005da330 (*(undefined4 *)0x005da330u)
#define DAT_00e39080 (*(undefined *)0x00e39080u)
#define DAT_00e950dc (*(undefined *)0x00e950dcu)
#define DAT_00ea187c (*(undefined4 *)0x00ea187cu)
#define DAT_00ea609c (*(undefined4 *)0x00ea609cu)
#define DAT_00ea60a0 (*(undefined4 *)0x00ea60a0u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern void FUN_004e0cf0();
extern void FUN_004e96c0();

int FUN_00452160(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar1 = DAT_00eaac28;
  iVar3 = *(int *)(&DAT_00ea187c + DAT_00eaac28 * 0x49d8);
  if (iVar3 < 0) {
    iVar2 = (&DAT_00ea609c)[DAT_00eaac28 * 0x1276] + 1;
    (&DAT_00ea609c)[DAT_00eaac28 * 0x1276] = iVar2;
    iVar3 = 0;
    if (iVar2 != 0) {
      iVar3 = iVar2 / 0xc;
      if (iVar2 % 0xc == 0) {
        iVar3 = (&DAT_00ea60a0)[iVar1 * 0x1276] + 1;
        (&DAT_00ea60a0)[iVar1 * 0x1276] = iVar3;
        if (9 < iVar3) {
          iVar3 = 10;
        }
        FUN_004e96c0(*(undefined4 *)(&DAT_005da330 + (iVar3 + DAT_00e39080 * 10) * 4));
        DAT_00e950dc = 1;
        iVar3 = (FUN_004e0cf0(0x79,0,0,0,0), restore_unresolved_return(0x004e0cf0u));
        return iVar3;
      }
    }
  }
  else {
    (&DAT_00ea609c)[DAT_00eaac28 * 0x1276] = 0;
    (&DAT_00ea60a0)[iVar1 * 0x1276] = 0;
  }
  return iVar3;
}
