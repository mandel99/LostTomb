#include "music_manager.h"
#include "sound_streams.h"
#include <stdint.h>
#define U(a) (*(uint32_t *)(uintptr_t)(a))
extern uint32_t FUN_00562348(void);

void FUN_00539820(RestoreMusicManager *self)
{
    RestoreMusicCategory *category;
    int32_t index;
    if (!U(0xf55b88) || !*(uint8_t *)0xe38e64u) return;
    if (!self->phase) FUN_00539730(self,0);
    FUN_0055f38d(RESTORE_SOUND,(int32_t)U(0xe38ec8));
    switch (self->phase) {
    case 0:
        category = &self->categories[*(int8_t *)0xe38ea8u];
        if (category->count > 0) {
            FUN_0055f449(RESTORE_SOUND,category->tracks[self->current].path,0,1);
            category = &self->categories[*(int8_t *)0xe38ea8u];
            U(0xf55b98) = (uint32_t)category->tracks[self->current].delay * 1000u;
            self->waiting = 0;
            self->current = (int32_t)((uint32_t)self->current + 1u);
            /* The native look-ahead at index 50 reads the adjacent count
               field; address bytes instead of indexing beyond the array. */
            self->phase = (U((uintptr_t)category + 0x1a40u + (uint32_t)self->current * 4u) != 1) + 1;
        }
        break;
    case 1:
        index = *(int8_t *)0xe38ea8u;
        category = &self->categories[index];
        if (category->ambient_count > 0) {
            uint32_t random = FUN_00562348();
            int32_t product = (int32_t)(random * (uint32_t)category->ambient_count);
            int32_t ambient = product / 32768;
            FUN_0055f449(RESTORE_SOUND,category->tracks[50 + ambient].path,0,1);
        }
        self->phase = 2;
        break;
    case 2:
        self->phase = 3;
        self->tick = restore_music_milliseconds();
        break;
    case 3:
    {
        uint32_t now = restore_music_milliseconds();
        if (self->tick + U(0xf55b98) < now) {
            if (self->current > self->last) self->current = self->first;
            self->phase = 0;
            self->waiting = 1;
        }
        break;
    }
    }
}
