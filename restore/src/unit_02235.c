#include "restore_types.h"
#define DAT_00eadd84 (*(undefined *)0x00eadd84u)
#define DAT_00eadd88 (*(undefined *)0x00eadd88u)
#define DAT_00eadd8c (*(undefined *)0x00eadd8cu)
#define DAT_00eadd90 (*(undefined *)0x00eadd90u)
#define DAT_00eadd94 (*(undefined *)0x00eadd94u)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define DAT_00f56fb4 (*(undefined *)0x00f56fb4u)
#define _DAT_00eadd54 (*(undefined4 *)0x00eadd54u)
#define s_wavs_wall_wav_005f79f4 (*(char (*)[14])0x005f79f4u)
extern undefined4 FUN_0047d350();
extern void FUN_004d6160();
extern void FUN_004d6250();
extern void FUN_00536820();

void FUN_004fe3c0(void)

{
  _DAT_00eadd54 = 0x55;
  if (DAT_00eadd94 != 0) {
    FUN_0047d350(s_wavs_wall_wav_005f79f4,1,0);
    DAT_00eadd94 = 0;
  }
  FUN_00536820(DAT_00eadd90,DAT_00eadd8c,DAT_00eadd88,DAT_00eadd84);
  FUN_004d6250(0x8b,0,DAT_00eadd90,DAT_00eadd8c + 10,DAT_00eadd88 << 4,DAT_00f56fb4,0,DAT_00eadd90,
               DAT_00eadd8c);
  FUN_004d6160(0x8b,1,DAT_00eadd90 + 0x20,DAT_00eadd84 * 0x10 + -0x9e + DAT_00eadd8c,
               (DAT_00eadd88 + -4) * 0x10,DAT_00f56f8c,0,DAT_00eadd90,DAT_00eadd8c);
  return;
}
