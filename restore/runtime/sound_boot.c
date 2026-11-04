#include "sound_device.h"
#include "sound_file_lookup.h"
#include "sound_banks.h"
#include "sound_samples.h"
#include "music_manager.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
void restore_sound_boot(void)
{
    U(0xf55b88u)=(uint32_t)FUN_0055e6c4(RESTORE_SOUND);
    (void)FUN_0055dc63(&RESTORE_SOUND->files,500);
    FUN_00539a40(RESTORE_MUSIC,0);FUN_00539250(RESTORE_MUSIC);FUN_00539730(RESTORE_MUSIC,1);
    U(0xf55b80u)=UINT32_MAX;
    FUN_00539d60();FUN_0053bcf0();FUN_0053af90();FUN_0053b130();
}
