#include "restore_types.h"
#define DAT_00605798 (*(char *)0x00605798u)
#define DAT_006059d4 (*(undefined *)0x006059d4u)
#define DAT_006059d8 (*(undefined *)0x006059d8u)
#define DAT_006059dc (*(undefined *)0x006059dcu)
#define DAT_006059e0 (*(undefined *)0x006059e0u)
#define DAT_006059e4 (*(undefined *)0x006059e4u)
#define DAT_006059e8 (*(undefined *)0x006059e8u)
#define DAT_00605a3c (*(undefined *)0x00605a3cu)
#define DAT_00605a5c (*(char *)0x00605a5cu)
#define DAT_00bca2e4 (*(undefined *)0x00bca2e4u)
#define DAT_00bca300 (*(undefined *)0x00bca300u)
#define DAT_00bca304 (*(undefined *)0x00bca304u)
#define DAT_00bca308 (*(undefined *)0x00bca308u)
extern void FUN_0046fef0();
extern void FUN_00478440();

void FUN_00405d90(void)

{
  if ((DAT_006059e4 != 0) && (DAT_00bca300 != 0)) {
    DAT_00605798 = 1;
    if (DAT_00605a5c == '\0') {
      DAT_00bca2e4 = DAT_00bca300;
      DAT_006059d4 = DAT_00bca308;
      DAT_006059dc = DAT_00bca304;
    }
    if (DAT_00605a3c == 0) {
      FUN_0046fef0(DAT_006059e0,DAT_006059d8,DAT_006059dc,DAT_006059d4,DAT_006059e8);
      return;
    }
    if (DAT_006059e8 == 5) {
      FUN_0046fef0(DAT_006059e0,DAT_006059d8,DAT_006059dc,DAT_006059d4,5);
      return;
    }
    if (DAT_006059e8 == 0x58) {
      FUN_0046fef0(DAT_006059e0,DAT_006059d8,DAT_006059dc,DAT_006059d4,0x58);
      return;
    }
    if (DAT_006059e8 == 0x59) {
      FUN_0046fef0(DAT_006059e0,DAT_006059d8,DAT_006059dc,DAT_006059d4,0x59);
      return;
    }
    if (DAT_006059e8 == 0x5d) {
      FUN_0046fef0(DAT_006059e0,DAT_006059d8,DAT_006059dc,DAT_006059d4,0x5d);
      return;
    }
    FUN_00478440(DAT_006059dc,DAT_006059d4);
  }
  return;
}
