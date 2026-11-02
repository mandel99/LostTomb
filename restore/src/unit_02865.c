#include "restore_types.h"
#define DAT_01266418 (*(undefined4 *)0x01266418u)
#define DAT_0126641c (*(undefined4 *)0x0126641cu)
extern int FUN_00561852();
extern void FUN_0056400a();

void FUN_00563dd6(void)

{
  int unaff_EBX;
  int unaff_EBP;
  int unaff_ESI;
  int *unaff_EDI;
  
  *(undefined4 *)(unaff_ESI + -4) = *(undefined4 *)(unaff_EBP + -0x28);
  DAT_01266418 = *(undefined4 *)(unaff_EBP + -0x1c);
  DAT_0126641c = *(undefined4 *)(unaff_EBP + -0x20);
  if ((((*unaff_EDI == -0x1f928c9d) && (unaff_EDI[4] == 3)) && (unaff_EDI[5] == 0x19930520)) &&
     ((*(int *)(unaff_EBP + -0x24) == unaff_EBX && (*(int *)(unaff_EBP + -0x2c) != unaff_EBX)))) {
    FUN_00561852();
    FUN_0056400a();
  }
  return;
}
