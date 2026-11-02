#include "restore_types.h"
#define DAT_00f54120 (*(undefined4 *)0x00f54120u)
#define s_audio_wavs_antelop_r_wav_005c92a8 (*(char (*)[25])0x005c92a8u)
#define s_audio_wavs_antelopdie_wav_005fa9b4 (*(char (*)[26])0x005fa9b4u)
#define s_audio_wavs_anteloproam_wav_005fa9d0 (*(char (*)[27])0x005fa9d0u)
#define s_audio_wavs_anteloprun_wav_005fa998 (*(char (*)[26])0x005fa998u)
#define s_audio_wavs_arch_atk_wav_005fa888 (*(char (*)[24])0x005fa888u)
#define s_audio_wavs_asp_r_wav_005faa74 (*(char (*)[21])0x005faa74u)
#define s_audio_wavs_aspdie_wav_005faa5c (*(char (*)[22])0x005faa5cu)
#define s_audio_wavs_bird_r_wav_005c92e0 (*(char (*)[22])0x005c92e0u)
#define s_audio_wavs_birddie_wav_005fa8d8 (*(char (*)[23])0x005fa8d8u)
#define s_audio_wavs_birdroam_wav_005fa8f0 (*(char (*)[24])0x005fa8f0u)
#define s_audio_wavs_chariot_atk_wav_005fa850 (*(char (*)[27])0x005fa850u)
#define s_audio_wavs_chariot_die_wav_005fa834 (*(char (*)[27])0x005fa834u)
#define s_audio_wavs_croc_r_wav_005fa938 (*(char (*)[22])0x005fa938u)
#define s_audio_wavs_crocatk_wav_005fa920 (*(char (*)[23])0x005fa920u)
#define s_audio_wavs_crocdie_wav_005fa908 (*(char (*)[23])0x005fa908u)
#define s_audio_wavs_die1_wav_005fa778 (*(char (*)[20])0x005fa778u)
#define s_audio_wavs_die2_wav_005fa764 (*(char (*)[20])0x005fa764u)
#define s_audio_wavs_die3_wav_005fa750 (*(char (*)[20])0x005fa750u)
#define s_audio_wavs_die4_wav_005fa73c (*(char (*)[20])0x005fa73cu)
#define s_audio_wavs_die5_wav_005fa728 (*(char (*)[20])0x005fa728u)
#define s_audio_wavs_die6_wav_005fa714 (*(char (*)[20])0x005fa714u)
#define s_audio_wavs_die7_wav_005fa700 (*(char (*)[20])0x005fa700u)
#define s_audio_wavs_die8_wav_005fa6ec (*(char (*)[20])0x005fa6ecu)
#define s_audio_wavs_dieg1_wav_005fa6d4 (*(char (*)[21])0x005fa6d4u)
#define s_audio_wavs_dieg2_wav_005fa6bc (*(char (*)[21])0x005fa6bcu)
#define s_audio_wavs_dieg3_wav_005fa6a4 (*(char (*)[21])0x005fa6a4u)
#define s_audio_wavs_dieg4_wav_005fa68c (*(char (*)[21])0x005fa68cu)
#define s_audio_wavs_hippo_r_wav_005fa980 (*(char (*)[23])0x005fa980u)
#define s_audio_wavs_hippoatk_wav_005fa968 (*(char (*)[24])0x005fa968u)
#define s_audio_wavs_hippodie_wav_005fa950 (*(char (*)[24])0x005fa950u)
#define s_audio_wavs_hunt_ant_atk_wav_005fa818 (*(char (*)[28])0x005fa818u)
#define s_audio_wavs_hunt_bird_atk_wav_005fa7dc (*(char (*)[29])0x005fa7dcu)
#define s_audio_wavs_hunt_ost_atk_wav_005fa7fc (*(char (*)[28])0x005fa7fcu)
#define s_audio_wavs_hyena_r_wav_005fab40 (*(char (*)[23])0x005fab40u)
#define s_audio_wavs_hyenaatk_wav_005fab0c (*(char (*)[24])0x005fab0cu)
#define s_audio_wavs_hyenadie_wav_005faaf4 (*(char (*)[24])0x005faaf4u)
#define s_audio_wavs_hyenaroam_wav_005fab24 (*(char (*)[25])0x005fab24u)
#define s_audio_wavs_lion_r_wav_005faadc (*(char (*)[22])0x005faadcu)
#define s_audio_wavs_liondie_wav_005faac4 (*(char (*)[23])0x005faac4u)
#define s_audio_wavs_ostrich_r_wav_005faa40 (*(char (*)[25])0x005faa40u)
#define s_audio_wavs_ostrichdie_wav_005faa08 (*(char (*)[26])0x005faa08u)
#define s_audio_wavs_ostrichroam_wav_005faa24 (*(char (*)[27])0x005faa24u)
#define s_audio_wavs_ostrichrun_wav_005fa9ec (*(char (*)[26])0x005fa9ecu)
#define s_audio_wavs_police_atk_wav_005fa8a0 (*(char (*)[26])0x005fa8a0u)
#define s_audio_wavs_scorpion_r_wav_005faaa8 (*(char (*)[26])0x005faaa8u)
#define s_audio_wavs_scorpiondie_wav_005faa8c (*(char (*)[27])0x005faa8cu)
#define s_audio_wavs_sentry_atk_wav_005fa8bc (*(char (*)[26])0x005fa8bcu)
#define s_audio_wavs_ship_arrow_wav_005fa7a4 (*(char (*)[26])0x005fa7a4u)
#define s_audio_wavs_ship_die_wav_005fa78c (*(char (*)[24])0x005fa78cu)
#define s_audio_wavs_spear_atk_wav_005fa86c (*(char (*)[25])0x005fa86cu)
#define s_audio_wavs_sword_atk_wav_005fa7c0 (*(char (*)[25])0x005fa7c0u)
extern void FUN_0053b100();

void FUN_0053b130(void)

{
  int iVar1;
  undefined4 *puVar2;
  
  puVar2 = &DAT_00f54120;
  for (iVar1 = 0xa5; iVar1 != 0; iVar1 = iVar1 + -1) {
    *puVar2 = 0xffffffff;
    puVar2 = puVar2 + 1;
  }
  FUN_0053b100(0,s_audio_wavs_hyena_r_wav_005fab40,0);
  FUN_0053b100(0,s_audio_wavs_hyenaroam_wav_005fab24,1);
  FUN_0053b100(0,s_audio_wavs_hyenaatk_wav_005fab0c,2);
  FUN_0053b100(0,s_audio_wavs_hyenadie_wav_005faaf4,3);
  FUN_0053b100(1,s_audio_wavs_lion_r_wav_005faadc,0);
  FUN_0053b100(1,s_audio_wavs_lion_r_wav_005faadc,1);
  FUN_0053b100(1,s_audio_wavs_lion_r_wav_005faadc,2);
  FUN_0053b100(1,s_audio_wavs_liondie_wav_005faac4,3);
  FUN_0053b100(2,s_audio_wavs_scorpion_r_wav_005faaa8,0);
  FUN_0053b100(2,s_audio_wavs_scorpion_r_wav_005faaa8,1);
  FUN_0053b100(2,s_audio_wavs_scorpion_r_wav_005faaa8,2);
  FUN_0053b100(2,s_audio_wavs_scorpiondie_wav_005faa8c,3);
  FUN_0053b100(3,s_audio_wavs_asp_r_wav_005faa74,0);
  FUN_0053b100(3,s_audio_wavs_asp_r_wav_005faa74,1);
  FUN_0053b100(3,s_audio_wavs_asp_r_wav_005faa74,2);
  FUN_0053b100(3,s_audio_wavs_aspdie_wav_005faa5c,3);
  FUN_0053b100(4,s_audio_wavs_ostrich_r_wav_005faa40,0);
  FUN_0053b100(4,s_audio_wavs_ostrichroam_wav_005faa24,1);
  FUN_0053b100(4,s_audio_wavs_ostrichdie_wav_005faa08,3);
  FUN_0053b100(4,s_audio_wavs_ostrichrun_wav_005fa9ec,4);
  FUN_0053b100(5,s_audio_wavs_antelop_r_wav_005c92a8,0);
  FUN_0053b100(5,s_audio_wavs_anteloproam_wav_005fa9d0,1);
  FUN_0053b100(5,s_audio_wavs_antelopdie_wav_005fa9b4,3);
  FUN_0053b100(5,s_audio_wavs_anteloprun_wav_005fa998,4);
  FUN_0053b100(6,s_audio_wavs_hippo_r_wav_005fa980,0);
  FUN_0053b100(6,s_audio_wavs_hippoatk_wav_005fa968,2);
  FUN_0053b100(6,s_audio_wavs_hippodie_wav_005fa950,3);
  FUN_0053b100(7,s_audio_wavs_croc_r_wav_005fa938,0);
  FUN_0053b100(7,s_audio_wavs_crocatk_wav_005fa920,2);
  FUN_0053b100(7,s_audio_wavs_crocdie_wav_005fa908,3);
  FUN_0053b100(8,s_audio_wavs_bird_r_wav_005c92e0,0);
  FUN_0053b100(8,s_audio_wavs_birdroam_wav_005fa8f0,1);
  FUN_0053b100(8,s_audio_wavs_birddie_wav_005fa8d8,3);
  FUN_0053b100(9,s_audio_wavs_sentry_atk_wav_005fa8bc,2);
  FUN_0053b100(10,s_audio_wavs_police_atk_wav_005fa8a0,2);
  FUN_0053b100(0xb,s_audio_wavs_arch_atk_wav_005fa888,2);
  FUN_0053b100(0xc,s_audio_wavs_spear_atk_wav_005fa86c,2);
  FUN_0053b100(0xd,s_audio_wavs_chariot_atk_wav_005fa850,2);
  FUN_0053b100(0xd,s_audio_wavs_chariot_die_wav_005fa834,3);
  FUN_0053b100(0xe,s_audio_wavs_hunt_ant_atk_wav_005fa818,2);
  FUN_0053b100(0xf,s_audio_wavs_hunt_ost_atk_wav_005fa7fc,2);
  FUN_0053b100(0x10,s_audio_wavs_hunt_bird_atk_wav_005fa7dc,2);
  FUN_0053b100(0x12,s_audio_wavs_arch_atk_wav_005fa888,2);
  FUN_0053b100(0x13,s_audio_wavs_sword_atk_wav_005fa7c0,2);
  FUN_0053b100(0x14,s_audio_wavs_spear_atk_wav_005fa86c,2);
  FUN_0053b100(0x11,s_audio_wavs_ship_arrow_wav_005fa7a4,2);
  FUN_0053b100(0x11,s_audio_wavs_ship_die_wav_005fa78c,3);
  FUN_0053b100(0x15,s_audio_wavs_die1_wav_005fa778,3);
  FUN_0053b100(0x16,s_audio_wavs_die2_wav_005fa764,3);
  FUN_0053b100(0x17,s_audio_wavs_die3_wav_005fa750,3);
  FUN_0053b100(0x18,s_audio_wavs_die4_wav_005fa73c,3);
  FUN_0053b100(0x19,s_audio_wavs_die5_wav_005fa728,3);
  FUN_0053b100(0x1a,s_audio_wavs_die6_wav_005fa714,3);
  FUN_0053b100(0x1b,s_audio_wavs_die7_wav_005fa700,3);
  FUN_0053b100(0x1c,s_audio_wavs_die8_wav_005fa6ec,3);
  FUN_0053b100(0x1d,s_audio_wavs_dieg1_wav_005fa6d4,3);
  FUN_0053b100(0x1e,s_audio_wavs_dieg2_wav_005fa6bc,3);
  FUN_0053b100(0x1f,s_audio_wavs_dieg3_wav_005fa6a4,3);
  FUN_0053b100(0x20,s_audio_wavs_dieg4_wav_005fa68c,3);
  return;
}
