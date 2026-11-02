#include "restore_types.h"
#define DAT_00eadd84 (*(undefined *)0x00eadd84u)
#define DAT_00eadd88 (*(undefined *)0x00eadd88u)
#define DAT_00eadd8c (*(undefined *)0x00eadd8cu)
#define DAT_00eadd90 (*(undefined *)0x00eadd90u)
#define DAT_00eadd94 (*(undefined *)0x00eadd94u)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define DAT_00f56fb4 (*(undefined *)0x00f56fb4u)
#define _DAT_00eadd54 (*(undefined4 *)0x00eadd54u)
#define s_wavs_plaza_wav_005f7a18 (*(char (*)[15])0x005f7a18u)
extern undefined4 FUN_0047d350();
extern void FUN_004d6160();
extern void FUN_004d6250();
extern void FUN_004fa3c0();
extern void FUN_004fa780();
extern void FUN_00536820();

void FUN_004fe730(void)

{
  _DAT_00eadd54 = 0x4f;
  if (DAT_00eadd94 != 0) {
    FUN_0047d350(s_wavs_plaza_wav_005f7a18,1,0);
    DAT_00eadd94 = 0;
  }
  FUN_004fa3c0();
  FUN_00536820(DAT_00eadd90,DAT_00eadd8c,DAT_00eadd88,DAT_00eadd84);
  FUN_004d6250(0x89,0,DAT_00eadd90,DAT_00eadd8c + 10,DAT_00eadd88 << 4,DAT_00f56fb4,0,DAT_00eadd90,
               DAT_00eadd8c);
  FUN_004fa780();
  FUN_004d6160(0x89,1,DAT_00eadd90 + 0x20,DAT_00eadd84 * 0x10 + -0x71 + DAT_00eadd8c,
               (DAT_00eadd88 + -4) * 0x10,DAT_00f56f8c,0,DAT_00eadd90,DAT_00eadd8c);
  return;
}
