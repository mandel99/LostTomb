#include "restore_types.h"
#define DAT_00eadd60 (*(undefined *)0x00eadd60u)
#define DAT_00eadd84 (*(undefined *)0x00eadd84u)
#define DAT_00eadd88 (*(undefined *)0x00eadd88u)
#define DAT_00eadd8c (*(undefined *)0x00eadd8cu)
#define DAT_00eadd90 (*(undefined *)0x00eadd90u)
#define DAT_00eadd94 (*(undefined *)0x00eadd94u)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define DAT_00f56fb4 (*(undefined *)0x00f56fb4u)
#define _DAT_00eadd54 (*(undefined4 *)0x00eadd54u)
#define s_wavs_aquaduct_wav_005f7a04 (*(char (*)[18])0x005f7a04u)
extern undefined4 FUN_0047d350();
extern void FUN_004d6160();
extern void FUN_004d6250();
extern void FUN_00536820();

void FUN_004fe630(void)

{
  _DAT_00eadd54 = 0x3b;
  if (DAT_00eadd94 != 0) {
    FUN_0047d350(s_wavs_aquaduct_wav_005f7a04,1,0);
    DAT_00eadd94 = 0;
  }
  FUN_00536820(DAT_00eadd90,DAT_00eadd8c,DAT_00eadd88,DAT_00eadd84);
  FUN_004d6250(0x8d,0,DAT_00eadd90,DAT_00eadd8c + 10,DAT_00eadd88 << 4,DAT_00f56fb4,0,DAT_00eadd90,
               DAT_00eadd8c);
  if (DAT_00eadd60 != 0) {
    FUN_004d6160(0x8d,1,DAT_00eadd90 + 0x20,(DAT_00eadd84 + -8) * 0x10 + DAT_00eadd8c,
                 (DAT_00eadd88 + -4) * 0x10,DAT_00f56f8c,0,DAT_00eadd90,DAT_00eadd8c);
    return;
  }
  FUN_004d6160(0x8d,2,DAT_00eadd90 + 0x20,(DAT_00eadd84 + -8) * 0x10 + DAT_00eadd8c,
               (DAT_00eadd88 + -4) * 0x10,DAT_00f56f8c,0,DAT_00eadd90,DAT_00eadd8c);
  return;
}
