#include "restore_types.h"
#define DAT_007027d4 (*(undefined4 *)0x007027d4u)
#define s_audio_ambient_hailstorm_mp3_005c92f8 (*(char (*)[28])0x005c92f8u)
#define s_audio_wavs_antelop_r_wav_005c92a8 (*(char (*)[25])0x005c92a8u)
#define s_audio_wavs_bird_r_wav_005c92e0 (*(char (*)[22])0x005c92e0u)
#define s_audio_wavs_zoo_r_wav_005c9314 (*(char (*)[21])0x005c9314u)
#define s_binks_high_Intro_big_bik_005c932c (*(char (*)[25])0x005c932cu)
#define s_binks_high_intro_big_bik_005c92c4 (*(char (*)[25])0x005c92c4u)
extern undefined4 FUN_004265d0();
extern void FUN_00426810();

undefined4 FUN_004268c0(byte *drive)
{
  /* Standalone local-data mode, explicitly requested by the user. Empty
     media prefix keeps later fallback filenames relative to the asset folder. */
  *drive=0;
  return 1;
}
