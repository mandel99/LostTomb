#include "restore_types.h"
#define DAT_00605798 (*(char *)0x00605798u)
#define DAT_006059d4 (*(undefined *)0x006059d4u)
#define DAT_006059d8 (*(undefined *)0x006059d8u)
#define DAT_006059dc (*(undefined *)0x006059dcu)
#define DAT_006059e0 (*(undefined *)0x006059e0u)
#define DAT_006059e4 (*(undefined *)0x006059e4u)
#define DAT_006059e8 (*(undefined *)0x006059e8u)
#define DAT_00605a3c (*(undefined *)0x00605a3cu)
#define DAT_00bca2c4 (*(undefined *)0x00bca2c4u)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
#define DAT_00bca2e4 (*(undefined *)0x00bca2e4u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern void FUN_004728a0();
extern void FUN_004795e0();
extern undefined4 FUN_0047d350();
extern void FUN_005178b0();

void FUN_00405e90(void)

{
  if (DAT_00605a3c == 1) {
    FUN_005178b0();
  }
  if (DAT_006059e8 == 0x7b) {
    FUN_004795e0();
    DAT_006059e4 = 0;
    return;
  }
  if (DAT_006059e8 == 0x7c) {
    FUN_004795e0();
    DAT_006059e4 = 0;
    return;
  }
  if (DAT_006059e8 != 0x81) {
    if (DAT_006059e4 != 0) {
      DAT_006059e4 = 0;
      if (DAT_00bca2c4 == 0) {
        if (DAT_00605798 == '\0') {
          DAT_006059e4 = 0;
          return;
        }
        DAT_00bca2c4 = DAT_00bca2e4;
      }
      if (0 < DAT_006059e8) {
        FUN_0047d350(4);
      }
      FUN_004728a0(DAT_00eaac28,DAT_00bca2e0,DAT_006059e0,DAT_006059d8,DAT_006059dc,DAT_006059d4,
                   DAT_006059e8);
    }
    return;
  }
  FUN_004795e0();
  DAT_006059e4 = 0;
  return;
}
