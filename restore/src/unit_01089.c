#include "restore_types.h"
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
extern void FUN_00475dc0();
extern void FUN_004767d0();
extern void FUN_00477500();
extern void FUN_004777c0();
extern void FUN_00479670();
extern void FUN_0051f780();

void FUN_004795e0(void)

{
  FUN_00479670();
  FUN_00479670();
  FUN_00479670();
  FUN_00479670();
  FUN_0051f780(0,0,DAT_00bca2d0 + -1,DAT_00bca2d4 + -1);
  FUN_00475dc0();
  FUN_004767d0(0,0,DAT_00bca2d0 + -1,DAT_00bca2d4 + -1,0,0);
  FUN_00477500(0,0,DAT_00bca2d0 + -1,DAT_00bca2d4 + -1);
  FUN_004777c0(0,0,DAT_00bca2d0 + -1,DAT_00bca2d4 + -1);
  return;
}
