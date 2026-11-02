#include "restore_types.h"
#define DAT_00605a3c (*(int32_t *)0x00605a3cu)
#define DAT_00bca2d0 (*(int32_t *)0x00bca2d0u)
#define DAT_00bca2d4 (*(int32_t *)0x00bca2d4u)
extern void FUN_00475720();
extern void FUN_00475bf0();
extern void FUN_00475cc0();
extern void FUN_00475dc0();
extern void FUN_00476530();
extern void FUN_004767d0();
extern void FUN_004770f0();
extern void FUN_00477210();
extern void FUN_00477500();
extern void FUN_004776f0();
extern void FUN_004777c0();
extern void FUN_00477890();
extern void FUN_0047b1a0();
extern void FUN_0047b570();
extern void FUN_0047bae0();
extern void FUN_0047d870();
extern void FUN_0051f780();

void FUN_004dea50(void)

{
  FUN_0051f780(0,0,DAT_00bca2d0 + -2,DAT_00bca2d4 + -2);
  FUN_004770f0(0,0,DAT_00bca2d0 + -1,DAT_00bca2d4 + -1);
  FUN_00475dc0();
  FUN_0051f780(0,0,DAT_00bca2d0 + -1,DAT_00bca2d4 + -1);
  FUN_00475bf0(0,0,DAT_00bca2d0 + -1,DAT_00bca2d4 + -1);
  FUN_00476530();
  if (DAT_00605a3c == 0) {
    FUN_0047d870(0);
  }
  FUN_004767d0(0,0,DAT_00bca2d0 + -1,DAT_00bca2d4 + -1,0,0);
  FUN_004777c0(0,0,DAT_00bca2d0 + -1,DAT_00bca2d4 + -1);
  FUN_00477500(0,0,DAT_00bca2d0 + -1,DAT_00bca2d4 + -1);
  FUN_004776f0(0,0,DAT_00bca2d0 + -1,DAT_00bca2d4 + -1);
  FUN_00477210();
  FUN_004777c0(0,0,DAT_00bca2d0 + -1,DAT_00bca2d4 + -1);
  FUN_00475cc0(0,0,DAT_00bca2d0 + -1,DAT_00bca2d4 + -1,0);
  FUN_00477890(0,0,DAT_00bca2d0 + -1,DAT_00bca2d4 + -1);
  FUN_0047b570();
  FUN_00475720(0,0,DAT_00bca2d0 + -1,DAT_00bca2d4 + -1,1);
  FUN_0047b1a0();
  FUN_0047bae0(0,0,DAT_00bca2d0 + -1,DAT_00bca2d4 + -1,0);
  return;
}
