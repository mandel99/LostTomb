#include "restore_types.h"
#define DAT_00f543b8 (*(undefined4 *)0x00f543b8u)
#define s_audio_wavs_build_wav_005fa5dc (*(char (*)[21])0x005fa5dcu)
#define s_audio_wavs_button_wav_005fa674 (*(char (*)[22])0x005fa674u)
#define s_audio_wavs_crash_wav_005fa620 (*(char (*)[21])0x005fa620u)
#define s_audio_wavs_dig_wav_005fa64c (*(char (*)[19])0x005fa64cu)
#define s_audio_wavs_fanfare1_wav_005fa5c4 (*(char (*)[24])0x005fa5c4u)
#define s_audio_wavs_fanfare2_wav_005fa5ac (*(char (*)[24])0x005fa5acu)
#define s_audio_wavs_fire_wav_005fa638 (*(char (*)[20])0x005fa638u)
#define s_audio_wavs_menu_wav_005fa5f4 (*(char (*)[20])0x005fa5f4u)
#define s_audio_wavs_no_wav_005fa660 (*(char (*)[18])0x005fa660u)
#define s_audio_wavs_ship_collision_aft_qu_005fa530 (*(char (*)[42])0x005fa530u)
#define s_audio_wavs_ship_collision_beam_w_005fa588 (*(char (*)[35])0x005fa588u)
#define s_audio_wavs_ship_collision_bow_wa_005fa50c (*(char (*)[34])0x005fa50cu)
#define s_audio_wavs_ship_collision_fore_q_005fa55c (*(char (*)[43])0x005fa55cu)
#define s_audio_wavs_ship_collision_stern__005fa4e8 (*(char (*)[36])0x005fa4e8u)
#define s_audio_wavs_ship_order_top_speed__005fa4c4 (*(char (*)[36])0x005fa4c4u)
#define s_audio_wavs_splash_wav_005fa608 (*(char (*)[22])0x005fa608u)
extern void FUN_0053af70();

void FUN_0053af90(void)

{
  int iVar1;
  undefined4 *puVar2;
  
  puVar2 = &DAT_00f543b8;
  for (iVar1 = 100; iVar1 != 0; iVar1 = iVar1 + -1) {
    *puVar2 = 0xffffffff;
    puVar2 = puVar2 + 1;
  }
  FUN_0053af70(0,s_audio_wavs_button_wav_005fa674);
  FUN_0053af70(1,s_audio_wavs_no_wav_005fa660);
  FUN_0053af70(2,s_audio_wavs_dig_wav_005fa64c);
  FUN_0053af70(3,s_audio_wavs_fire_wav_005fa638);
  FUN_0053af70(4,s_audio_wavs_crash_wav_005fa620);
  FUN_0053af70(5,s_audio_wavs_splash_wav_005fa608);
  FUN_0053af70(6,s_audio_wavs_menu_wav_005fa5f4);
  FUN_0053af70(7,s_audio_wavs_build_wav_005fa5dc);
  FUN_0053af70(8,s_audio_wavs_build_wav_005fa5dc);
  FUN_0053af70(9,s_audio_wavs_fanfare1_wav_005fa5c4);
  FUN_0053af70(10,s_audio_wavs_fanfare2_wav_005fa5ac);
  FUN_0053af70(0x11,s_audio_wavs_crash_wav_005fa620);
  FUN_0053af70(0x12,s_audio_wavs_fire_wav_005fa638);
  FUN_0053af70(0xb,s_audio_wavs_ship_collision_beam_w_005fa588);
  FUN_0053af70(0xc,s_audio_wavs_ship_collision_fore_q_005fa55c);
  FUN_0053af70(0xd,s_audio_wavs_ship_collision_aft_qu_005fa530);
  FUN_0053af70(0xe,s_audio_wavs_ship_collision_bow_wa_005fa50c);
  FUN_0053af70(0xf,s_audio_wavs_ship_collision_stern__005fa4e8);
  FUN_0053af70(0x10,s_audio_wavs_ship_order_top_speed__005fa4c4);
  return;
}
