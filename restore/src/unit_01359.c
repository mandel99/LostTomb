#include "restore_types.h"
#define DAT_00bcd608 (*(undefined4 *)0x00bcd608u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
extern void FUN_004b3250();
extern void FUN_004f6560();
extern undefined4 FUN_004f6620();
extern int FUN_004f6630();

void FUN_004a3630(void)

{
  int iVar1;
  int iVar2;
  
  iVar1 = FUN_004f6630();
  if (iVar1 != 0) {
    iVar2 = 0;
    iVar1 = FUN_004f6630();
    if (0 < iVar1) {
      do {
        iVar1 = FUN_004f6620();
        (&DAT_00bcd630)[iVar1 * 0x184] = (&DAT_00bcd608)[iVar1 * 0x184];
        FUN_004b3250(iVar1);
        iVar2 = iVar2 + 1;
        iVar1 = FUN_004f6630();
      } while (iVar2 < iVar1);
    }
    FUN_004f6560();
  }
  return;
}
