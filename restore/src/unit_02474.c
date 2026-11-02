#include "restore_types.h"
#include "sound_initialization.h"
#include "music_manager.h"
#define DAT_00f55b80 (*(int32_t *)0x00f55b80u)
#define DAT_00f55b88 (*(int32_t *)0x00f55b88u)
extern void FUN_00539d60();
extern void FUN_0053af90();
extern void FUN_0053b130();
extern void FUN_0053bcf0();

void FUN_005394f0(void)

{
  restore_sound_initialize_owned();
  DAT_00f55b88 = FUN_0055e6c4(RESTORE_SOUND);
  FUN_0055dc63(&RESTORE_SOUND->files,500);
  restore_music_initialize_owned();
  FUN_00539a40(RESTORE_MUSIC,0);
  FUN_00539250(RESTORE_MUSIC);
  FUN_00539730(RESTORE_MUSIC,1);
  DAT_00f55b80 = 0xffffffff;
  FUN_00539d60();
  FUN_0053bcf0();
  FUN_0053af90();
  FUN_0053b130();
  return;
}
