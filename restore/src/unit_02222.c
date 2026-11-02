#include "restore_types.h"
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00eadce4 (*(undefined *)0x00eadce4u)
#define DAT_00eadd24 (*(undefined4 *)0x00eadd24u)
#define DAT_00eadd40 (*(undefined *)0x00eadd40u)
#define DAT_00eadd4c (*(undefined *)0x00eadd4cu)
#define DAT_00eadd84 (*(undefined *)0x00eadd84u)
#define DAT_00eadd88 (*(undefined *)0x00eadd88u)
#define DAT_00eadd8c (*(undefined *)0x00eadd8cu)
#define DAT_00eadd90 (*(undefined *)0x00eadd90u)
#define DAT_00eadd94 (*(undefined *)0x00eadd94u)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define DAT_00f56fb4 (*(undefined *)0x00f56fb4u)
#define _DAT_00eadd54 (*(undefined4 *)0x00eadd54u)
#define s_wavs_empty_land_wav_005f792c (*(char (*)[20])0x005f792cu)
extern undefined4 FUN_0047d350();
extern void FUN_004d6160();
extern void FUN_004d6250();
extern void FUN_004fa3c0();
extern void FUN_004fa780();
extern void FUN_004fe300();
extern void FUN_004fe3c0();
extern void FUN_004fe490();
extern void FUN_004fe560();
extern void FUN_004fe630();
extern void FUN_004fe730();
extern void FUN_005058f0();
extern void FUN_00536820();
extern void FUN_0053d0b0();

void FUN_004fa530(void)

{
  switch((int)(DAT_00eadd4c)) {
  case 6: ;
    _DAT_00eadd54 = 0x39;
    break;
  case 7: ;
    _DAT_00eadd54 = 0x3b;
    break;
default: ;
    _DAT_00eadd54 = 0;
    break;
  case 9: ;
  case 0x17: ;
  case 0x18: ;
    _DAT_00eadd54 = 0x55;
    break;
  case 0xb: ;
    _DAT_00eadd54 = 0x3a;
  }
  if (DAT_00eadd4c == 0xc) {
    FUN_004fe300();
    return;
  }
  if (DAT_00eadd4c == 0xd) {
    if (DAT_00eadd40 < 1) {
      FUN_004fe730();
      return;
    }
  }
  else {
    if (DAT_00eadd4c == 9) {
      FUN_004fe3c0();
      return;
    }
    if (DAT_00eadd4c == 0x17) {
      FUN_004fe490();
      return;
    }
    if (DAT_00eadd4c == 0x18) {
      FUN_004fe560();
      return;
    }
    if (DAT_00eadd4c == 7) {
      FUN_004fe630();
      return;
    }
    if (DAT_00eadd4c == 8) {
      FUN_005058f0();
      return;
    }
  }
  if (DAT_00eadd94 != 0) {
    DAT_00eadd94 = 0;
    if (DAT_00eadd40 < 1) {
      FUN_0047d350(s_wavs_empty_land_wav_005f792c,1,0);
    }
    else {
      FUN_0053d0b0();
    }
  }
  if ((0 < DAT_00eadd40) && ((&DAT_00eadd24)[DAT_00eadce4] != 0)) {
    _DAT_00eadd54 =
         (-(uint)((byte)(&DAT_00bcd602)[(&DAT_00eadd24)[DAT_00eadce4] * 0x184] < 0x43) & 0xfffffee0)
         + 0x14a;
  }
  FUN_004fa3c0();
  FUN_00536820(DAT_00eadd90,DAT_00eadd8c,DAT_00eadd88,DAT_00eadd84);
  if (DAT_00eadd40 == 0) {
    FUN_004d6250(0x46,DAT_00eadd4c + 10,DAT_00eadd90,DAT_00eadd8c + 10,DAT_00eadd88 << 4,
                 DAT_00f56fb4,0,DAT_00eadd90,DAT_00eadd8c);
  }
  if (DAT_00eadd4c == 3) {
    FUN_004d6160(0x46,0x27,DAT_00eadd90 + 0x24,DAT_00eadd84 * 0x10 + -0x71 + DAT_00eadd8c,
                 (DAT_00eadd88 + -4) * 0x10,DAT_00f56f8c,0,DAT_00eadd90,DAT_00eadd8c);
    FUN_004fa780();
    return;
  }
  if ((DAT_00eadd4c != 6) && (DAT_00eadd4c != 0xd)) {
    FUN_004d6160(0x46,DAT_00eadd4c + 0x24,DAT_00eadd90 + 0x24,
                 DAT_00eadd84 * 0x10 + -0x71 + DAT_00eadd8c,(DAT_00eadd88 + -4) * 0x10,DAT_00f56f8c,
                 0,DAT_00eadd90,DAT_00eadd8c);
  }
  FUN_004fa780();
  return;
}
