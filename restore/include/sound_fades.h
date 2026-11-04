#ifndef RESTORE_SOUND_FADES_H
#define RESTORE_SOUND_FADES_H
#include <stdint.h>
void restore_sound_fade_to_stop(int32_t channel);
void restore_sound_fade_out_step(int32_t channel);
void restore_sound_fade_in_step(int32_t channel);
void FUN_0053b690(int32_t channel);
void FUN_0053b6f0(int32_t channel);
void FUN_0053b790(int32_t channel);
#endif
