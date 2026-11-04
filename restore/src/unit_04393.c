#include "sound_file_lookup.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
void restore_sound_file_table_cleanup_frame(uint32_t frame)
{FUN_0055dbf8((RestoreSoundFileTable *)(uintptr_t)U(frame-16u));}
void FUN_0056e168(uint32_t frame)
{restore_sound_file_table_cleanup_frame(frame);}
