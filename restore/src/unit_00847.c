#include "restore_types.h"
#define DAT_007848d8 (*(undefined *)0x007848d8u)
#define DAT_0078d7bc (*(undefined *)0x0078d7bcu)
#define DAT_00e95144 (*(undefined *)0x00e95144u)
#define DAT_00ea446c (*(undefined4 *)0x00ea446cu)
#define DAT_00ea4470 (*(undefined4 *)0x00ea4470u)
#define DAT_00ea4474 (*(undefined4 *)0x00ea4474u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern void FUN_00459400();
extern void FUN_004e0cf0();

void FUN_00459350(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  iVar1 = (&DAT_00ea4474)[DAT_00eaac28 * 0x1276];
  FUN_00459400();
  iVar4 = DAT_00eaac28;
  iVar2 = (&DAT_00ea446c)[DAT_00eaac28 * 0x1276];
  iVar3 = (&DAT_00ea4470)[DAT_00eaac28 * 0x1276];
  if (iVar2 < iVar3) {
    (&DAT_00ea4470)[DAT_00eaac28 * 0x1276] = iVar2;
  }
  else if (iVar2 + -5 < iVar3) {
    (&DAT_00ea4470)[DAT_00eaac28 * 0x1276] = iVar2;
  }
  else {
    (&DAT_00ea4470)[DAT_00eaac28 * 0x1276] = iVar3 + 5;
  }
  if (100 < (int)(&DAT_00ea4470)[iVar4 * 0x1276]) {
    (&DAT_00ea4470)[iVar4 * 0x1276] = 100;
  }
  if ((((iVar1 == 0) && (0 < (int)(&DAT_00ea4474)[iVar4 * 0x1276])) &&
      (DAT_007848d8 <= DAT_0078d7bc)) && (DAT_00e95144 < 1)) {
    DAT_00e95144 = 6;
    FUN_004e0cf0(0x88,0,0,0,0);
  }
  return;
}
