#include "restore_types.h"
#include "sound_streams.h"
#define DAT_00e38d9c (*(int32_t *)0x00e38d9cu)
#define DAT_00e38da4 (*(int32_t *)0x00e38da4u)
#define DAT_00e38da8 (*(int32_t *)0x00e38da8u)
#define DAT_00e38e64 (*(char *)0x00e38e64u)
#define DAT_00f55b88 (*(uint32_t *)0x00f55b88u)
#define DAT_00f55bec (*(uint32_t *)0x00f55becu)
#define DAT_00f56f84 (*(uint32_t *)0x00f56f84u)
#define s_audio_music_setup_mp3_005761ac (*(char (*)[22])0x005761acu)
extern void FUN_004cf1c0();
extern void FUN_004d6250();
extern int FUN_0053e590();

void FUN_00520e00(void)

{
  int iVar1;
  
  if (DAT_00f55b88 != 0) {
    iVar1 = FUN_0055f5a1(RESTORE_SOUND,0);
    if ((iVar1 == 0) && (DAT_00e38e64 != '\0')) {
      FUN_0055f449(RESTORE_SOUND,s_audio_music_setup_mp3_005761ac,0,0);
    }
  }
  FUN_004cf1c0();
  FUN_0053e590(DAT_00f55bec,DAT_00e38da8,DAT_00e38da4);
  FUN_004d6250(0xd,7,(DAT_00e38d9c + -0x140) / 2,DAT_00e38da4 + 0x1ce,0x140,DAT_00f56f84,0xffff,
               DAT_00e38da8,DAT_00e38da4);
  return;
}
